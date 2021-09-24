#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t mutex_x;
pthread_cond_t condX;
int contador = 0;

void fazAlgo(int x){
  int boba1, boba2;
  boba1=100; boba2=-100; while (boba2 < boba1) boba2++;
}

void* T1(void* arg) {
    while(1) {
        pthread_mutex_lock(&mutex_x);
        fazAlgo(contador);
        contador++;
        if(contador %100 == 0){
          pthread_cond_signal(&condX);
        }
        pthread_mutex_unlock(&mutex_x);
    }
}

void* T2(void* arg) {
    
    pthread_mutex_lock(&mutex_x);
    pthread_cond_wait(&condX, &mutex_x);
   
    while (contador%100 != 0) {
        pthread_cond_wait(&condX, &mutex_x);
    }
    printf("Multiplo de 100 encontrado: %d\n", contador);
    exit(1);
    pthread_mutex_unlock(&mutex_x);
}

int main(int argc, char* argv[]) {
    pthread_t threads[2];
    pthread_mutex_init(&mutex_x, NULL);
    pthread_cond_init(&condX, NULL);
    
    pthread_create(&threads[0], NULL, T1, NULL);
    pthread_create(&threads[1], NULL, T2, NULL);

    for (int i = 0; i < 2; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            fprintf(stderr, "errro pthread join");
        }
    }
    pthread_mutex_destroy(&mutex_x);
    pthread_cond_destroy(&condX);
    return 0;
}