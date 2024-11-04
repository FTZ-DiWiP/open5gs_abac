
#include "sbi-path.h"  // Include the header file for the SBI (Service Based Architecture) path definitions and functions

// Function to open and initialize the SBI (Service Based Interface) for the NF (Network Function)
int abac_sbi_open(void)
{
    // Declare pointers for network function instance and service
    ogs_sbi_nf_instance_t *nf_instance = NULL; // Pointer to hold the network function instance
    ogs_sbi_nf_service_t *service = NULL;       // Pointer to hold the network function service

    /* Initialize SELF NF instance */
    nf_instance = ogs_sbi_self()->nf_instance; // Retrieve the current network function instance
    ogs_assert(nf_instance);                    // Assert that the network function instance is valid
    ogs_sbi_nf_fsm_init(nf_instance);          // Initialize the finite state machine for the NF instance

    /* Build NF instance information. It will be transmitted to NRF (Network Repository Function). */
    ogs_sbi_nf_instance_build_default(nf_instance); // Build default information for the NF instance
    ogs_sbi_nf_instance_add_allowed_nf_type(nf_instance, OpenAPI_nf_type_SCP); // Add allowed NF types
    ogs_sbi_nf_instance_add_allowed_nf_type(nf_instance, OpenAPI_nf_type_PCF);
    ogs_sbi_nf_instance_add_allowed_nf_type(nf_instance, OpenAPI_nf_type_UDM);

    /* Build NF service information. It will be transmitted to NRF. */
    // Check if the NUDR_DR service is available
    if (ogs_sbi_nf_service_is_available(OGS_SBI_SERVICE_NAME_NUDR_DR)) {
        // Build a default service for the NUDR_DR
        service = ogs_sbi_nf_service_build_default(nf_instance, OGS_SBI_SERVICE_NAME_NUDR_DR);
        ogs_assert(service); // Assert that the service was created successfully
        // Add version information for the service
        ogs_sbi_nf_service_add_version(service, OGS_SBI_API_V1, OGS_SBI_API_V1_0_0, NULL);
        // Specify allowed NF types for this service
        ogs_sbi_nf_service_add_allowed_nf_type(service, OpenAPI_nf_type_PCF);
        ogs_sbi_nf_service_add_allowed_nf_type(service, OpenAPI_nf_type_UDM);
    }

    /* Initialize NRF NF Instance */
    nf_instance = ogs_sbi_self()->nrf_instance; // Retrieve the NRF instance
    if (nf_instance) // If the NRF instance is valid
        ogs_sbi_nf_fsm_init(nf_instance); // Initialize its finite state machine

    /* Setup Subscription-Data */
    // Add subscription specifications for SEPP (Security Edge Protection Proxy)
    ogs_sbi_subscription_spec_add(OpenAPI_nf_type_SEPP, NULL);

    // Start the SBI server with the specified handler
    if (ogs_sbi_server_start_all(ogs_sbi_server_handler) != OGS_OK) // Check if server starts successfully
        return OGS_ERROR; // Return an error if it fails

    return OGS_OK; // Return success if everything initialized correctly
}

// Function to close and stop the SBI for the NF
void abac_sbi_close(void)
{
    ogs_sbi_client_stop_all(); // Stop all SBI clients
    ogs_sbi_server_stop_all(); // Stop all SBI servers
}

