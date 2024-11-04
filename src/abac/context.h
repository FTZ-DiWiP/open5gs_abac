
#ifndef OPEN5GS_ABAC_CONTEXT_H
#define OPEN5GS_ABAC_CONTEXT_H

#include "ogs-app.h"
#include "ogs-dbi.h"
#include "ogs-sbi.h"

#include "abac-sm.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int __abac_log_domain;

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __abac_log_domain

typedef struct abac_context_s {
} abac_context_t;

void abac_context_init(void);
void abac_context_final(void);
abac_context_t *abac_self(void);

int abac_context_parse_config(void);

#ifdef __cplusplus
}
#endif

#endif //OPEN5GS_ABAC_CONTEXT_H
