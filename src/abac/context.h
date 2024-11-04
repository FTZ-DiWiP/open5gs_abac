
#ifndef OPEN5GS_ABAC_CONTEXT_H
#define OPEN5GS_ABAC_CONTEXT_H



#include "ogs-app.h"
#include "ogs-crypt.h"
#include "ogs-sbi.h"

#include "abac-sm.h"

extern int __abac_log_domain;

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __abac_log_domain

typedef struct abac_context_s {
    ogs_list_t      abac_ue_list;
    ogs_hash_t      *suci_hash;
    ogs_hash_t      *supi_hash;

} abac_context_t;

struct abac_ue_s {
    ogs_sbi_object_t sbi;
    ogs_fsm_t sm;

    char *ctx_id;
    char *suci;
    char *supi;
    char *serving_network_name;

    OpenAPI_auth_type_e auth_type;
    char *auth_events_url;
    OpenAPI_auth_result_e auth_result;

    uint8_t rand[OGS_RAND_LEN];
    uint8_t xres_star[OGS_MAX_RES_LEN];
    uint8_t hxres_star[OGS_MAX_RES_LEN];
    uint8_t kausf[OGS_SHA256_DIGEST_SIZE];
    uint8_t kseaf[OGS_SHA256_DIGEST_SIZE];
};

void abac_context_init(void);
void abac_context_final(void);
abac_context_t *abac_self(void);

int abac_context_parse_config(void);

abac_ue_t *abac_ue_add(char *suci);
void abac_ue_remove(abac_ue_t *abac_ue);
void abac_ue_remove_all(void);
abac_ue_t *abac_ue_find_by_suci(char *suci);
abac_ue_t *abac_ue_find_by_supi(char *supi);
abac_ue_t *abac_ue_find_by_suci_or_supi(char *suci_or_supi);
abac_ue_t *abac_ue_find_by_ctx_id(char *ctx_id);

abac_ue_t *abac_ue_cycle(abac_ue_t *abac_ue);
int get_ue_load(void);

#endif //OPEN5GS_ABAC_CONTEXT_H