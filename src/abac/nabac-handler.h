
#ifndef OPEN5GS_ABAC_NABAC_HANDLER_H
#define OPEN5GS_ABAC_NABAC_HANDLER_H

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

bool abac_nabac_dr_handle_subscription_authentication(
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *message);
bool abac_nabac_dr_handle_subscription_context(
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *message);
bool abac_nabac_dr_handle_subscription_provisioned(
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *message);

bool abac_nabac_dr_handle_policy_data(
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *message);

#ifdef __cplusplus
}
#endif

#endif //OPEN5GS_ABAC_NABAC_HANDLER_H
