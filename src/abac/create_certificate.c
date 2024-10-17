//
// Created by abac on 17.10.24.
//
#include <stdio.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/evp.h>
#include <openssl/bn.h>
#include <openssl/err.h>

#define CERT_FILE "cert.pem"
#define KEY_FILE  "key.pem"

void create_certificate_with_json_extension(const char *json) {
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();

    EVP_PKEY *pkey = EVP_PKEY_new();
    RSA *rsa = RSA_new();

    BIGNUM *bn = BN_new();
    BN_set_word(bn, RSA_F4);

    if (!RSA_generate_key_ex(rsa, 2048, bn, NULL)) {
        fprintf(stderr, "Failed to generate RSA key\n");
        goto cleanup;
    }

    EVP_PKEY_assign_RSA(pkey, rsa);
    rsa = NULL;  // pkey owns rsa now

    X509 *x509 = X509_new();
    ASN1_INTEGER_set(X509_get_serialNumber(x509), 1);
    X509_gmtime_adj(X509_getm_notBefore(x509), 0);
    X509_gmtime_adj(X509_getm_notAfter(x509), 31536000L);
    X509_set_pubkey(x509, pkey);

    X509_NAME *name = X509_get_subject_name(x509);
    X509_NAME_add_entry_by_txt(name, "C", MBSTRING_ASC, (unsigned char *)"DE", -1, -1, 0);
    X509_NAME_add_entry_by_txt(name, "O", MBSTRING_ASC, (unsigned char *)"My Company", -1, -1, 0);
    X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC, (unsigned char *)"example.com", -1, -1, 0);
    X509_set_issuer_name(x509, name);

    if (!X509_sign(x509, pkey, EVP_sha256())) {
        fprintf(stderr, "Failed to sign certificate\n");
        goto cleanup;
    }

    // Add custom JSON extension
    if (json) {
        X509_EXTENSION *ext;
        ASN1_OCTET_STRING *ext_data;
        ext_data = ASN1_OCTET_STRING_new();
        ASN1_OCTET_STRING_set(ext_data, (unsigned char *)json, strlen(json));

        ext = X509_EXTENSION_create_by_NID(NULL, NID_netscape_comment, 0, ext_data);
        ASN1_OCTET_STRING_free(ext_data);

        if (ext) {
            X509_add_ext(x509, ext, -1);
            X509_EXTENSION_free(ext);
        } else {
            fprintf(stderr, "Failed to create custom extension\n");
            goto cleanup;
        }
    }

    FILE *cert_file = fopen(CERT_FILE, "wb");
    if (cert_file) {
        PEM_write_X509(cert_file, x509);
        fclose(cert_file);
    } else {
        fprintf(stderr, "Failed to open cert file for writing\n");
    }

    FILE *key_file = fopen(KEY_FILE, "wb");
    if (key_file) {
        PEM_write_PrivateKey(key_file, pkey, NULL, NULL, 0, NULL, NULL);
        fclose(key_file);
    } else {
        fprintf(stderr, "Failed to open key file for writing\n");
    }

    cleanup:
    if (x509) X509_free(x509);
    if (pkey) EVP_PKEY_free(pkey);
    if (rsa) RSA_free(rsa);
    if (bn) BN_free(bn);

    EVP_cleanup();
    ERR_free_strings();
}

int main() {
    const char *json = "{\"key1\": \"value1\", \"key2\": \"value2\"}";
    create_certificate_with_json_extension(json);
    return 0;
}