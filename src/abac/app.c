

#include "ogs-sctp.h"
#include "ogs-app.h"

int app_initialize(const char *const argv[])
{
    int rv;

    ogs_sctp_init(ogs_app()->usrsctp.udp_port);
    rv = abac_initialize();
    if (rv != OGS_OK) {
        ogs_error("Failed to initialize ABAC");
        return rv;
    }
    ogs_info("ABAC initialize...done");

    return OGS_OK;
}

void app_terminate(void)
{
    abac_terminate();
    ogs_sctp_final();
    ogs_info("ABAC terminate...done");
}
