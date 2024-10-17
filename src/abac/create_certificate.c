#include <stdio.h>
#include <string.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/pem.h>
#include <openssl/evp.h>
#include <openssl/err.h>

#define KEY_FILE  "key.pem"

// Function prototype
char* create_certificate_with_json_extension(const char *json);

char* create_certificate_with_json_extension(const char *json) {
    EVP_PKEY *pkey = NULL;
    X509 *x509 = NULL;
    char *cert_str = NULL;
    BIO *mem_bio = NULL;

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
    X509_NAME_add_entry_by_txt(name, "O", MBSTRING_ASC, (unsigned char *)"My Company", -1, -1, 0);
    X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC, (unsigned char *)"example.com", -1, -1, 0);
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

    // Write the private key to a file
    FILE *key_file = fopen(KEY_FILE, "wb");
    if (key_file) {
        PEM_write_PrivateKey(key_file, pkey, NULL, NULL, 0, NULL, NULL);
        fclose(key_file);
    } else {
        fprintf(stderr, "Failed to open key file for writing\n");
    }

    // Capture the certificate in a BIO for conversion to a string
    mem_bio = BIO_new(BIO_s_mem());
    if (PEM_write_bio_X509(mem_bio, x509)) {
        BUF_MEM *bptr;
        BIO_get_mem_ptr(mem_bio, &bptr);
        cert_str = strndup(bptr->data, bptr->length);
    } else {
        fprintf(stderr, "Failed to write certificate to BIO\n");
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

int main(void) {
    const char *json = "{\"key1\": \"value1\", \"key2\": \"value2\"}";
    char *cert = create_certificate_with_json_extension(json);
    if (cert) {
        printf("Certificate:\n%s\n", cert);
        free(cert); // Remember to free the allocated string
    }
    return 0;
}