#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/pem.h>
#include <openssl/err.h>

// Funktion zum Erstellen eines X.509-Attributszertifikats
X509 *create_attribute_certificate(EVP_PKEY *signing_key, X509 *holder_cert, const char *attribute_oid, const char *attribute_value) {
    X509 *ac = NULL;
    X509_ATTRIBUTE *attribute = NULL;
    ASN1_OBJECT *obj = NULL;
    ASN1_TYPE *asn1_type = NULL;
    X509_NAME *subject_name = NULL;
    X509_NAME *issuer_name = NULL;

    // Erstellen des X.509-Attributszertifikats
    ac = X509_new();
    if (ac == NULL) {
        fprintf(stderr, "Fehler beim Erstellen des Attributszertifikats\n");
        return NULL;
    }

    // Setzen der Seriennummer
    ASN1_INTEGER_set(X509_get_serialNumber(ac), 1);

    // Setzen der Gültigkeitsdauer (1 Jahr)
    X509_gmtime_adj(X509_get_notBefore(ac), 0);
    X509_gmtime_adj(X509_get_notAfter(ac), 31536000L); // 365 Tage

    // Setzen des Inhaber- und Ausstellerzertifikats
    issuer_name = X509_get_issuer_name(holder_cert);
    subject_name = X509_get_subject_name(holder_cert);
    X509_set_issuer_name(ac, issuer_name);
    X509_set_subject_name(ac, subject_name);

    // Setzen des öffentlichen Schlüssels
    if (X509_set_pubkey(ac, EVP_PKEY_get1_RSA(signing_key)) == 0) {
        fprintf(stderr, "Fehler beim Setzen des öffentlichen Schlüssels\n");
        X509_free(ac);
        return NULL;
    }

    // Erstellen des Attributs
    attribute = X509_ATTRIBUTE_new();
    if (attribute == NULL) {
        fprintf(stderr, "Fehler beim Erstellen des Attributs\n");
        X509_free(ac);
        return NULL;
    }

    obj = OBJ_txt2obj(attribute_oid, 1);
    if (obj == NULL) {
        fprintf(stderr, "Fehler beim Konvertieren der OID\n");
        X509_ATTRIBUTE_free(attribute);
        X509_free(ac);
        return NULL;
    }

    asn1_type = ASN1_TYPE_new();
    if (asn1_type == NULL) {
        fprintf(stderr, "Fehler beim Erstellen des ASN.1-Typs\n");
        X509_ATTRIBUTE_free(attribute);
        X509_free(ac);
        return NULL;
    }

    ASN1_TYPE_set(asn1_type, V_ASN1_UTF8STRING, ASN1_STRING_new());
    ASN1_STRING_set(asn1_type->value.utf8string, attribute_value, -1);

    X509_ATTRIBUTE_set1_object(attribute, obj);
    X509_ATTRIBUTE_set1_data(attribute, V_ASN1_UTF8STRING, asn1_type->value.utf8string->data, asn1_type->value.utf8string->length);

    // Fügen Sie das Attribut zum Zertifikat hinzu
    if (X509_add1_attr(ac, attribute) == 0) {
        fprintf(stderr, "Fehler beim Hinzufügen des Attributs\n");
        ASN1_TYPE_free(asn1_type);
        X509_ATTRIBUTE_free(attribute);
        X509_free(ac);
        return NULL;
    }

    // Signieren des Attributszertifikats
    if (X509_sign(ac, signing_key, EVP_sha256()) == 0) {
        fprintf(stderr, "Fehler beim Signieren des Attributszertifikats\n");
        ASN1_TYPE_free(asn1_type);
        X509_ATTRIBUTE_free(attribute);
        X509_free(ac);
        return NULL;
    }

    // Aufräumen
    ASN1_TYPE_free(asn1_type);
    X509_ATTRIBUTE_free(attribute);

    return ac;
}

// Beispiel für die Verwendung der Funktion
int main() {
    // OpenSSL initialisieren
    OpenSSL_add_all_algorithms();
    ERR_load_crypto_strings();

    // Generieren eines privaten Schlüssels
    EVP_PKEY *signing_key = EVP_PKEY_new();
    RSA *rsa = RSA_generate_key(2048, RSA_F4, NULL, NULL);
    EVP_PKEY_assign_RSA(signing_key, rsa);

    // Erstellen eines Dummy-Inhaberzertifikats
    X509 *holder_cert = X509_new();
    X509_gmtime_adj(X509_get_notBefore(holder_cert), 0);
    X509_gmtime_adj(X509_get_notAfter(holder_cert), 31536000L);
    X509_set_pubkey(holder_cert, EVP_PKEY_get1_RSA(signing_key));
    ASN1_INTEGER_set(X509_get_serialNumber(holder_cert), 1);
    X509_set_issuer_name(holder_cert, X509_get_subject_name(holder_cert));

    // Erstellen des Attributszertifikats
    X509 *ac = create_attribute_certificate(signing_key, holder_cert, "1.2.3.4.5", "MyCustomAttribute");
    if (ac) {
        // Speichern des Attributszertifikats
        FILE *f = fopen("attribute_cert.pem", "wb");
        PEM_write_X509(f, ac);
        fclose(f);
        X509_free(ac);
    }

    // Aufräumen
    X509_free(holder_cert);
    EVP_PKEY_free(signing_key);
    ERR_free_strings();

    return 0;
}
