#include "context.h"
#include "sbi-path.h"

static abac_context_t self;

int __abac_log_domain;

static int context_initialized = 0;

void abac_context_init(void)
{
    ogs_assert(context_initialized == 0);

    /* Initialize ABAC context */
    memset(&self, 0, sizeof(abac_context_t));

    ogs_log_install_domain(&__ogs_dbi_domain, "dbi", ogs_core()->log.level);
    ogs_log_install_domain(&__abac_log_domain, "abac", ogs_core()->log.level);

    context_initialized = 1;
}

void abac_context_final(void)
{
    ogs_assert(context_initialized == 1);

    context_initialized = 0;
}

abac_context_t *abac_self(void)
{
    return &self;
}

static int abac_context_prepare(void)
{
    return OGS_OK;
}

static int abac_context_validation(void)
{
    return OGS_OK;
}

int abac_context_parse_config(void)
{
    int rv;
    yaml_document_t *document = NULL;
    ogs_yaml_iter_t root_iter;
    int idx = 0;

    document = ogs_app()->document;
    ogs_assert(document);

    rv = abac_context_prepare();
    if (rv != OGS_OK) return rv;

    ogs_yaml_iter_init(&root_iter, document);
    while (ogs_yaml_iter_next(&root_iter)) {
        const char *root_key = ogs_yaml_iter_key(&root_iter);
        ogs_assert(root_key);
        if ((!strcmp(root_key, "abac")) &&
            (idx++ == ogs_app()->config_section_id)) {
            ogs_yaml_iter_t abac_iter;
            ogs_yaml_iter_recurse(&root_iter, &abac_iter);
            while (ogs_yaml_iter_next(&abac_iter)) {
                const char *abac_key = ogs_yaml_iter_key(&abac_iter);
                ogs_assert(abac_key);
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
                } else
                    ogs_warn("unknown key `%s`", abac_key);
            }
        }
    }

    rv = abac_context_validation();
    if (rv != OGS_OK) return rv;

    return OGS_OK;
}