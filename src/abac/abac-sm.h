//
// Created by sascha on 04.11.24.
//

#ifndef OPEN5GS_ABAC_ABAC_SM_H
#define OPEN5GS_ABAC_ABAC_SM_H

#include "event.h"

#ifdef __cplusplus
extern "C" {
#endif

void abac_state_initial(ogs_fsm_t *s, abac_event_t *e);
void abac_state_final(ogs_fsm_t *s, abac_event_t *e);
void abac_state_operational(ogs_fsm_t *s, abac_event_t *e);

void abac_nf_fsm_init(ogs_sbi_nf_instance_t *nf_instance);
void abac_nf_fsm_fini(ogs_sbi_nf_instance_t *nf_instance);

void abac_nf_state_initial(ogs_fsm_t *s, abac_event_t *e);
void abac_nf_state_final(ogs_fsm_t *s, abac_event_t *e);
void abac_nf_state_will_register(ogs_fsm_t *s, abac_event_t *e);
void abac_nf_state_registered(ogs_fsm_t *s, abac_event_t *e);
void abac_nf_state_de_registered(ogs_fsm_t *s, abac_event_t *e);
void abac_nf_state_exception(ogs_fsm_t *s, abac_event_t *e);

#define abac_sm_debug(__pe) \
    ogs_debug("%s(): %s", __func__, abac_event_get_name(__pe))

#ifdef __cplusplus
}
#endif

#endif //OPEN5GS_ABAC_ABAC_SM_H
