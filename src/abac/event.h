

#ifndef OPEN5GS_ABAC_EVENT_H
#define OPEN5GS_ABAC_EVENT_H

#include "ogs-proto.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct abac_event_s {
    ogs_event_t h;
} abac_event_t;

OGS_STATIC_ASSERT(OGS_EVENT_SIZE >= sizeof(abac_event_t));

abac_event_t *abac_event_new(int id);

const char *abac_event_get_name(abac_event_t *e);

#ifdef __cplusplus
}
#endif

#endif //OPEN5GS_ABAC_EVENT_H
