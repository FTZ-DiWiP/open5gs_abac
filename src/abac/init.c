#include "sbi-path.h"  // Include the header file for Service-Based Interface (SBI) path definitions and functions.

static ogs_thread_t *thread;  // Pointer to a thread structure for handling concurrent execution.
static void abac_main(void *data);  // Forward declaration of the main function for ABAC processing.
static int initialized = 0;  // Flag indicating whether ABAC has been initialized.

int abac_initialize(void)
{
    int rv;  // Variable for storing return values from function calls.

#define APP_NAME "abac"  // Define a macro for the application name.

    // Parse the local configuration file for the ABAC application.
    rv = ogs_app_parse_local_conf(APP_NAME);
    if (rv != OGS_OK) return rv;  // Return on failure.

    // Initialize the SBI context for ABAC and the custom ABAC context.
    ogs_sbi_context_init(OpenAPI_nf_type_ABAC);
    abac_context_init();

    // Parse the configuration for the SBI context and custom ABAC context.
    rv = ogs_sbi_context_parse_config(APP_NAME, "nrf", "scp");
    if (rv != OGS_OK) return rv;

    rv = abac_context_parse_config();
    if (rv != OGS_OK) return rv;

    // Configure logging settings for the application.
    rv = ogs_log_config_domain(ogs_app()->logger.domain, ogs_app()->logger.level);
    if (rv != OGS_OK) return rv;

    // Open the ABAC SBI interface.
    rv = abac_sbi_open();
    if (rv != OGS_OK) return rv;

    // Create a new thread to run the ABAC main function.
    thread = ogs_thread_create(abac_main, NULL);
    if (!thread) return OGS_ERROR;  // Return an error if thread creation fails.

    initialized = 1;  // Mark as initialized.

    return OGS_OK;  // Return success.
}

static ogs_timer_t *t_termination_holding = NULL;  // Timer for handling termination delays.

static void event_termination(void)
{
    ogs_sbi_nf_instance_t *nf_instance = NULL;  // Pointer to an NF instance structure.

    /* Sending NF Instance De-registration to NRF */
    // Iterate through the NF instance list and finalize each instance.
    ogs_list_for_each(&ogs_sbi_self()->nf_instance_list, nf_instance)
        ogs_sbi_nf_fsm_fini(nf_instance);

    /* Starting holding timer */
    // Add and start a holding timer for termination.
    t_termination_holding = ogs_timer_add(ogs_app()->timer_mgr, NULL, NULL);
    ogs_assert(t_termination_holding);
#define TERMINATION_HOLDING_TIME ogs_time_from_msec(300)
    ogs_timer_start(t_termination_holding, TERMINATION_HOLDING_TIME);

    /* Sending termination event to the queue */
    ogs_queue_term(ogs_app()->queue);  // Send termination signal to the event queue.
    ogs_pollset_notify(ogs_app()->pollset);  // Notify the pollset about the termination event.
}

void abac_terminate(void)
{
    if (!initialized) return;  // Return if not initialized.

    /* Daemon terminating */
    event_termination();  // Handle the termination event.
    ogs_thread_destroy(thread);  // Destroy the thread.
    ogs_timer_delete(t_termination_holding);  // Delete the holding timer.

    abac_sbi_close();  // Close the ABAC SBI interface.

    abac_context_final();  // Finalize and clean up the ABAC context.
    ogs_sbi_context_final();  // Finalize the SBI context.
}

static void abac_main(void *data)
{
    ogs_fsm_t abac_sm;  // Finite State Machine for ABAC processing.
    int rv;  // Return value for loop control.

    // Initialize the ABAC state machine with initial and final states.
    ogs_fsm_init(&abac_sm, abac_state_initial, abac_state_final, 0);

    for ( ;; ) {  // Infinite loop for handling events.
        // Poll for events in the application pollset and process the next timer event.
        ogs_pollset_poll(ogs_app()->pollset, ogs_timer_mgr_next(ogs_app()->timer_mgr));

        /*
         * After ogs_pollset_poll(), ogs_timer_mgr_expire() must be called to process expired timers.
         * This ensures ogs_timer_mgr_next() works correctly if a timer is stopped internally in ogs_timer_mgr_expire().
         * The event queue should not be used before calling ogs_timer_mgr_expire().
         */
        ogs_timer_mgr_expire(ogs_app()->timer_mgr);

        for ( ;; ) {  // Loop to handle all queued events.
            abac_event_t *e = NULL;  // Pointer to an ABAC event.

            // Try to pop an event from the queue.
            rv = ogs_queue_trypop(ogs_app()->queue, (void**)&e);
            ogs_assert(rv != OGS_ERROR);  // Assert if an error occurs.

            if (rv == OGS_DONE)
                goto done;  // Exit loop if no more events.

            if (rv == OGS_RETRY)
                break;  // Retry if the event retrieval is incomplete.

            ogs_assert(e);  // Ensure the event is valid.
            ogs_fsm_dispatch(&abac_sm, e);  // Dispatch the event to the state machine.
            ogs_event_free(e);  // Free the event after processing.
        }
    }
    done:

    // Finalize the state machine.
    ogs_fsm_fini(&abac_sm, 0);
}
