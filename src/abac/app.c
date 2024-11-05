// Include the header file for application-level definitions and function declarations specific to the OGS (Open5GS) framework.
#include "ogs-app.h"

// Function to initialize the application.
int app_initialize(const char *const argv[])
{
    int rv;  // Declare a variable to hold the return value of the initialization process.

    // Call the ABAC (Attribute-Based Access Control) initialization function.
    rv = abac_initialize();
    if (rv != OGS_OK) {  // Check if the initialization was unsuccessful.
        ogs_error("Failed to initialize ABAC");  // Log an error message if initialization fails.
        return rv;  // Return the error code to indicate failure.
    }
    ogs_info("ABAC initialize...done");  // Log an informational message indicating successful initialization.

    return OGS_OK;  // Return OGS_OK to indicate successful initialization.
}

// Function to terminate the application and clean up resources.
void app_terminate(void)
{
    abac_terminate();  // Call the ABAC termination function to release resources.
    ogs_info("ABAC terminate...done");  // Log an informational message indicating that termination is complete.
}

