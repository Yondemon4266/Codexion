#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 10
#define NUM_INCREMENTS 1000000

// La variable partagée
int global_counter = 0;

// 1. Déclaration du mutex
pthread_mutex_t counter_mutex;

// La routine exécutée par chaque thread
void *increment_routine(void *arg) {
    (void)arg; 

    for (int i = 0; i < NUM_INCREMENTS; i++) {
        
        // 2. ON VERROUILLE LA PORTE
        // Si un autre thread a déjà verrouillé ce mutex, le thread actuel
        // va se mettre en "pause" ici jusqu'à ce que la porte soit déverrouillée.
        pthread_mutex_lock(&counter_mutex);

        // --- DÉBUT DE LA ZONE CRITIQUE ---
        global_counter++;
        // --- FIN DE LA ZONE CRITIQUE ---

        // 3. ON DÉVERROUILLE LA PORTE
        // On libère le mutex pour que le prochain thread en attente puisse entrer.
        pthread_mutex_unlock(&counter_mutex);
    }
    return NULL;
}

int main(void) {
    pthread_t threads[NUM_THREADS];

    // 4. Initialisation du mutex (INDISPENSABLE avant de l'utiliser)
    if (pthread_mutex_init(&counter_mutex, NULL) != 0) {
        printf("Erreur d'initialisation du mutex.\n");
        return 1;
    }

    printf("Lancement de %d threads sécurisés par Mutex...\n", NUM_THREADS);

    // Création des threads
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, increment_routine, NULL) != 0) {
            printf("Erreur de création.\n");
            return 1;
        }
    }

    // Attente de la fin des threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Affichage du résultat
    int expected = NUM_THREADS * NUM_INCREMENTS;
    printf("\n--- RÉSULTATS ---\n");
    printf("Valeur attendue : %d\n", expected);
    printf("Valeur finale   : %d\n", global_counter);

    if (global_counter == expected) {
        printf("✅ SUCCÈS ! Aucune collision grâce au Mutex.\n");
    }

    // 5. Destruction du mutex pour libérer les ressources système
    pthread_mutex_destroy(&counter_mutex);

    return 0;
}