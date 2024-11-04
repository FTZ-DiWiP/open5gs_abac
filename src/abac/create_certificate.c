#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/pem.h>
#include <openssl/evp.h>
#include <openssl/err.h>

#define KEY_FILE  "key.pem"

// Function prototype
char* create_certificate_with_json_extension(const char *json);
char* extract_json_from_certificate(const char *cert_str);
char* read_json_from_file(const char *filename);

char* create_certificate_with_json_extension(const char *json) {
    EVP_PKEY *pkey = NULL;
    X509 *x509 = NULL;
    char *cert_str = NULL;
    BIO *mem_bio = NULL;

    const char *cert_file_path = "../../../configs/open5gs/tls/abac_certificate.pem";

    // Initialize OpenSSL
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();

    // Generate Key Pair
    EVP_PKEY_CTX *pkey_ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
    if (!pkey_ctx || EVP_PKEY_keygen_init(pkey_ctx) <= 0 || EVP_PKEY_CTX_set_rsa_keygen_bits(pkey_ctx, 2048) <= 0) {
        fprintf(stderr, "Failed to initialize or configure keygen context\n");
        goto cleanup;
    }

    if (EVP_PKEY_keygen(pkey_ctx, &pkey) <= 0) {
        fprintf(stderr, "Failed to generate key pair\n");
        goto cleanup;
    }

    // Create new X.509 certificate
    x509 = X509_new();
    if (!x509) {
        fprintf(stderr, "Failed to create X.509 certificate\n");
        goto cleanup;
    }

    ASN1_INTEGER_set(X509_get_serialNumber(x509), 1);
    X509_gmtime_adj(X509_getm_notBefore(x509), 0);
    X509_gmtime_adj(X509_getm_notAfter(x509), 31536000L);
    X509_set_pubkey(x509, pkey);

    X509_NAME *name = X509_get_subject_name(x509);
    X509_NAME_add_entry_by_txt(name, "C", MBSTRING_ASC, (unsigned char *)"DE", -1, -1, 0);
    X509_NAME_add_entry_by_txt(name, "O", MBSTRING_ASC, (unsigned char *)"HAW Hamburg", -1, -1, 0);
    X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC, (unsigned char *)"haw-hamburg.de", -1, -1, 0);
    X509_set_issuer_name(x509, name);

    // Add custom JSON extension
    if (json) {
        X509_EXTENSION *ext;
        ASN1_OCTET_STRING *ext_data = ASN1_OCTET_STRING_new();
        if (ext_data == NULL || !ASN1_OCTET_STRING_set(ext_data, (unsigned char *)json, strlen(json))) {
            fprintf(stderr, "Failed to create ASN1 octet string\n");
            ASN1_OCTET_STRING_free(ext_data);
            goto cleanup;
        }

        ext = X509_EXTENSION_create_by_OBJ(NULL, OBJ_nid2obj(NID_netscape_comment), 0, ext_data);
        ASN1_OCTET_STRING_free(ext_data);

        if (ext) {
            X509_add_ext(x509, ext, -1);
            X509_EXTENSION_free(ext);
        } else {
            fprintf(stderr, "Failed to create custom extension\n");
            goto cleanup;
        }
    }

    // Sign the certificate with the private key
    if (!X509_sign(x509, pkey, EVP_sha256())) {
        fprintf(stderr, "Failed to sign certificate\n");
        goto cleanup;
    }

    // Capture the certificate in a BIO for conversion to a string
    mem_bio = BIO_new(BIO_s_mem());
    if (PEM_write_bio_X509(mem_bio, x509)) {
        BUF_MEM *bptr;
        BIO_get_mem_ptr(mem_bio, &bptr);
        cert_str = strndup(bptr->data, bptr->length);
    } else {
        fprintf(stderr, "Failed to write certificate to BIO\n");
        goto cleanup;
    }

    // Write the certificate to the specified file
    FILE *cert_file = fopen(cert_file_path, "w");
    if (cert_file) {
        PEM_write_X509(cert_file, x509);
        fclose(cert_file);
    } else {
        fprintf(stderr, "Failed to open cert file for writing\n");
    }

    cleanup:
    if (x509) X509_free(x509);
    if (pkey) EVP_PKEY_free(pkey);
    if (pkey_ctx) EVP_PKEY_CTX_free(pkey_ctx);
    if (mem_bio) BIO_free(mem_bio);

    EVP_cleanup();
    ERR_free_strings();

    return cert_str;
}

char* read_json_from_file(const char *filename) {
    char str[80];
    strcpy(str, "../../../src/abac/");
    FILE *file = fopen(strcat(str, filename) , "r");
    if (!file) {
        perror("Failed to open file");
        return NULL;
    }

    // Determine the file size and allocate buffer accordingly
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *json_data = (char *)malloc(file_size + 1);
    if (!json_data) {
        perror("Failed to allocate memory");
        fclose(file);
        return NULL;
    }

    // Read the file contents into the buffer
    fread(json_data, 1, file_size, file);
    json_data[file_size] = '\0'; // Null-terminate the string

    fclose(file);
    return json_data;
}

char* extract_json_from_certificate(const char *cert_str) {
    BIO *mem_bio = BIO_new_mem_buf(cert_str, -1);
    if (!mem_bio) {
        fprintf(stderr, "Failed to create BIO for certificate\n");
        return NULL;
    }

    X509 *x509 = PEM_read_bio_X509(mem_bio, NULL, NULL, NULL);
    if (!x509) {
        fprintf(stderr, "Failed to parse X.509 certificate\n");
        BIO_free(mem_bio);
        return NULL;
    }

    char *json_data = NULL;
    int ext_count = X509_get_ext_count(x509);
    int i; // Declare the loop variable outside
    for (i = 0; i < ext_count; i++) {
        X509_EXTENSION *ext = X509_get_ext(x509, i);
        ASN1_OBJECT *obj = X509_EXTENSION_get_object(ext);

        // Check if the extension is the one with our JSON data
        if (OBJ_obj2nid(obj) == NID_netscape_comment) { // Assuming netscape_comment was used
            ASN1_OCTET_STRING *data = X509_EXTENSION_get_data(ext);
            if (data) {
                json_data = strndup((char *)data->data, data->length);
                break;
            }
        }
    }

    X509_free(x509);
    BIO_free(mem_bio);

    if (!json_data) {
        fprintf(stderr, "JSON data not found in certificate\n");
    }
    return json_data;
}

int main(void) {
    char *json_data = read_json_from_file("attributes.json");
    if (!json_data) {
        return 1; // Exit if reading fails
    }
    char *cert = create_certificate_with_json_extension(json_data);
    if (cert) {
        printf("Certificate:\n%s\n", cert);
        char *json_2 = extract_json_from_certificate(cert);
        if (json_2) {
            printf("Extracted JSON:\n%s\n", json_2);
            free(json_2);
        }
        free(cert);
    }
    free(json_data);
    return 0;
}