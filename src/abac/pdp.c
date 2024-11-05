//
// Created by sascha on 05.11.24.
//

#include "pdp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struktur zur Speicherung der Richtlinie
typedef struct {
    char *key;
    char *value;
} policy_condition_t;

// Struktur zur Speicherung einer gesamten Richtlinie
typedef struct {
    char *policy_name;
    policy_condition_t *conditions;
    int num_conditions;
} policy_t;

// Beispielrichtlinien erstellen
policy_t *create_policies(int *num_policies) {
    *num_policies = 2;
    policy_t *policies = malloc(sizeof(policy_t) * (*num_policies));

    // Richtlinie 1
    policies[0].policy_name = "access_policy_1";
    policies[0].num_conditions = 2;
    policies[0].conditions = malloc(sizeof(policy_condition_t) * policies[0].num_conditions);
    policies[0].conditions[0] = (policy_condition_t){"role", "admin"};
    policies[0].conditions[1] = (policy_condition_t){"location", "internal_network"};

    // Richtlinie 2
    policies[1].policy_name = "access_policy_2";
    policies[1].num_conditions = 2;
    policies[1].conditions = malloc(sizeof(policy_condition_t) * policies[1].num_conditions);
    policies[1].conditions[0] = (policy_condition_t){"role", "user"};
    policies[1].conditions[1] = (policy_condition_t){"time_of_day", "business_hours"};

    return policies;
}

// Funktion zur Evaluierung der Benutzerattribute gegen Richtlinien
int evaluate_request(policy_t *policies, int num_policies, policy_condition_t *user_attributes, int num_attributes) {
    for (int i = 0; i < num_policies; i++) {
        policy_t policy = policies[i];
        int match = 1;

        for (int j = 0; j < policy.num_conditions; j++) {
            policy_condition_t condition = policy.conditions[j];
            int found = 0;

            for (int k = 0; k < num_attributes; k++) {
                if (strcmp(condition.key, user_attributes[k].key) == 0 &&
                    strcmp(condition.value, user_attributes[k].value) == 0) {
                    found = 1;
                    break;
                }
            }

            if (!found) {
                match = 0;
                break;
            }
        }

        if (match) {
            printf("Policy '%s' erfüllt. Zugriff erlaubt.\n", policy.policy_name);
            return 1; // Zugriff erlaubt
        }
    }

    printf("Keine passende Policy gefunden. Zugriff verweigert.\n");
    return 0; // Zugriff verweigert
}
