
#include "context.h"

#include "sbi-path.h"

static abac_context_t self;

int __abac_log_domain;

static OGS_POOL(abac_ue_pool, abac_ue_t);

static int context_initialized = 0;

void abac_context_init(void)
{
    ogs_assert(context_initialized == 0);

    /* Initialize ABAC context */
    memset(&self, 0, sizeof(abac_context_t));

    ogs_log_install_domain(&__abac_log_domain, "abac", ogs_core()->log.level);

    ogs_pool_init(&abac_ue_pool, ogs_global_conf()->max.ue);

    ogs_list_init(&self.abac_ue_list);
    self.suci_hash = ogs_hash_make();
    ogs_assert(self.suci_hash);
    self.supi_hash = ogs_hash_make();
    ogs_assert(self.supi_hash);

    context_initialized = 1;
}

void abac_context_final(void)
{
    ogs_assert(context_initialized == 1);

    abac_ue_remove_all();

    ogs_assert(self.suci_hash);
    ogs_hash_destroy(self.suci_hash);
    ogs_assert(self.supi_hash);
    ogs_hash_destroy(self.supi_hash);

    ogs_pool_final(&abac_ue_pool);

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

    document = ogs_app()->document;
    ogs_assert(document);

    rv = abac_context_prepare();
    if (rv != OGS_OK) return rv;

    ogs_yaml_iter_init(&root_iter, document);
    while (ogs_yaml_iter_next(&root_iter)) {
        const char *root_key = ogs_yaml_iter_key(&root_iter);
        ogs_assert(root_key);
        if (!strcmp(root_key, "abac")) {
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

abac_ue_t *abac_ue_add(char *suci)
{
    abac_event_t e;
    abac_ue_t *abac_ue = NULL;

    ogs_assert(suci);

    ogs_pool_alloc(&abac_ue_pool, &abac_ue);
    if (!abac_ue) {
        ogs_error("ogs_pool_alloc() failed");
        return NULL;
    }
    ogs_assert(abac_ue);
    memset(abac_ue, 0, sizeof *abac_ue);

    abac_ue->ctx_id =
            ogs_msprintf("%d", (int)ogs_pool_index(&abac_ue_pool, abac_ue));
    ogs_assert(abac_ue->ctx_id);

    abac_ue->suci = ogs_strdup(suci);
    ogs_assert(abac_ue->suci);
    ogs_hash_set(self.suci_hash, abac_ue->suci, strlen(abac_ue->suci), abac_ue);

    memset(&e, 0, sizeof(e));
    e.abac_ue = abac_ue;
    ogs_fsm_init(&abac_ue->sm, abac_ue_state_initial, abac_ue_state_final, &e);

    ogs_list_add(&self.abac_ue_list, abac_ue);

    return abac_ue;
}

void abac_ue_remove(abac_ue_t *abac_ue)
{
    abac_event_t e;

    ogs_assert(abac_ue);

    ogs_list_remove(&self.abac_ue_list, abac_ue);

    memset(&e, 0, sizeof(e));
    e.abac_ue = abac_ue;
    ogs_fsm_fini(&abac_ue->sm, &e);

    /* Free SBI object memory */
    ogs_sbi_object_free(&abac_ue->sbi);

    ogs_assert(abac_ue->ctx_id);
    ogs_free(abac_ue->ctx_id);

    ogs_assert(abac_ue->suci);
    ogs_hash_set(self.suci_hash, abac_ue->suci, strlen(abac_ue->suci), NULL);
    ogs_free(abac_ue->suci);

    if (abac_ue->supi) {
        ogs_hash_set(self.supi_hash,
                     abac_ue->supi, strlen(abac_ue->supi), NULL);
        ogs_free(abac_ue->supi);
    }

    if (abac_ue->auth_events_url)
        ogs_free(abac_ue->auth_events_url);

    if (abac_ue->serving_network_name)
        ogs_free(abac_ue->serving_network_name);

    ogs_pool_free(&abac_ue_pool, abac_ue);
}

void abac_ue_remove_all(void)
{
    abac_ue_t *abac_ue = NULL, *next = NULL;;

    ogs_list_for_each_safe(&self.abac_ue_list, next, abac_ue)
        abac_ue_remove(abac_ue);
}

abac_ue_t *abac_ue_find_by_suci(char *suci)
{
    ogs_assert(suci);
    return (abac_ue_t *)ogs_hash_get(self.suci_hash, suci, strlen(suci));
}

abac_ue_t *abac_ue_find_by_supi(char *supi)
{
    ogs_assert(supi);
    return (abac_ue_t *)ogs_hash_get(self.supi_hash, supi, strlen(supi));
}

abac_ue_t *abac_ue_find_by_suci_or_supi(char *suci_or_supi)
{
    ogs_assert(suci_or_supi);
    if (strncmp(suci_or_supi, "suci-", strlen("suci-")) == 0)
        return abac_ue_find_by_suci(suci_or_supi);
    else
        return abac_ue_find_by_supi(suci_or_supi);
}

abac_ue_t *abac_ue_find_by_ctx_id(char *ctx_id)
{
    ogs_assert(ctx_id);
    return ogs_pool_find(&abac_ue_pool, atoll(ctx_id));
}

abac_ue_t *abac_ue_cycle(abac_ue_t *abac_ue)
{
    return ogs_pool_cycle(&abac_ue_pool, abac_ue);
}

int get_ue_load(void)
{
    return (((ogs_pool_size(&abac_ue_pool) -
              ogs_pool_avail(&abac_ue_pool)) * 100) /
            ogs_pool_size(&abac_ue_pool));
}

