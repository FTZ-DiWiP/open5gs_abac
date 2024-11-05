#include "sbi-path.h"  // Include the header file for Service-Based Interface (SBI) path definitions and functions.

static abac_context_t self;  // Static instance of the ABAC context structure.

int __abac_log_domain;  // Log domain variable for ABAC.

static int context_initialized = 0;  // Flag to track if the ABAC context has been initialized.

void abac_context_init(void)
{
    ogs_assert(context_initialized == 0);  // Ensure that context has not been initialized before.

    /* Initialize ABAC context */
    memset(&self, 0, sizeof(abac_context_t));  // Clear the ABAC context structure to ensure all fields are zero-initialized.

    // Install a logging domain for ABAC with the logging level from the core.
    ogs_log_install_domain(&__abac_log_domain, "abac", ogs_core()->log.level);

    context_initialized = 1;  // Set the initialized flag to true.
}

void abac_context_final(void)
{
    ogs_assert(context_initialized == 1);  // Ensure that context is initialized before finalizing.

    context_initialized = 0;  // Reset the initialized flag to false.
}

abac_context_t *abac_self(void)
{
    return &self;  // Return a pointer to the static ABAC context instance.
}

static int abac_context_prepare(void)
{
    return OGS_OK;  // Placeholder for preparing the ABAC context; currently always returns success.
}

static int abac_context_validation(void)
{
    return OGS_OK;  // Placeholder for validating the ABAC context; currently always returns success.
}

int abac_context_parse_config(void)
{
    int rv;  // Variable to hold return values.
    yaml_document_t *document = NULL;  // Pointer to the YAML document for configuration parsing.
    ogs_yaml_iter_t root_iter;  // Iterator for traversing the YAML document.
    int idx = 0;  // Index for matching the configuration section.

    document = ogs_app()->document;  // Get the YAML document from the application context.
    ogs_assert(document);  // Ensure the document is valid.

    rv = abac_context_prepare();  // Prepare the ABAC context before parsing.
    if (rv != OGS_OK) return rv;  // Return on failure.

    ogs_yaml_iter_init(&root_iter, document);  // Initialize the iterator for the YAML document.
    while (ogs_yaml_iter_next(&root_iter)) {  // Iterate over the root keys in the document.
        const char *root_key = ogs_yaml_iter_key(&root_iter);  // Get the current key.
        ogs_assert(root_key);  // Assert that the key is valid.

        // Check if the key is "abac" and matches the expected section index.
        if ((!strcmp(root_key, "abac")) && (idx++ == ogs_app()->config_section_id)) {
            ogs_yaml_iter_t abac_iter;  // Iterator for the ABAC section.
            ogs_yaml_iter_recurse(&root_iter, &abac_iter);  // Recurse into the "abac" section.
            while (ogs_yaml_iter_next(&abac_iter)) {  // Iterate over keys in the ABAC section.
                const char *abac_key = ogs_yaml_iter_key(&abac_iter);  // Get the current key.
                ogs_assert(abac_key);  // Assert that the key is valid.

                // Handle various keys in the ABAC configuration.
                if (!strcmp(abac_key, "default")) {
                    /* handle config in sbi library */
                } else if (!strcmp(abac_key, "sbi")) {
                    /* handle config in sbi library */
                } else if (!strcmp(abac_key, "nrf")) {
                    /* handle config in sbi library */
                } else if (!strcmp(abac_key, "scp")) {
                    /* handle config in sbi library */
                } else if (!strcmp(abac_key, "service_name")) {
                    /* handle config in sbi library */
                } else if (!strcmp(abac_key, "discovery")) {
                    /* handle config in sbi library */
                } else {
                    ogs_warn("unknown key `%s`", abac_key);  // Log a warning for unrecognized keys.
                }
            }
        }
    }

    rv = abac_context_validation();  // Validate the ABAC context after parsing.
    if (rv != OGS_OK) return rv;  // Return on failure.

    return OGS_OK;  // Return success after parsing the configuration.
}
