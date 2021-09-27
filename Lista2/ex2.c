#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

pthread_mutex_t mutex_x;
pthread_cond_t condX, condY;
long long int contador = 0, vez = true;  // contador global e variavel vez, funciona com uma ideia próxima a do algoritmo da ultima questão da lista 1
// quando a variavel vez = true, significa que t1 tem que rodar, quando vez = false, significa que t2 tem q rodar

void fazAlgo(int x){ // função faz algo, mesma função que os laboratórios anteriores
  int boba1, boba2;
  boba1=100; boba2=-100; while (boba2 < boba1) boba2++;
}

void* T1(void* arg) {
    while(1) {
        pthread_mutex_lock(&mutex_x); //lock
        
        if (vez == false) { // Vez = false, significa que t1 tem que esperar o sinal de t2
            pthread_cond_wait(&condX, &mutex_x);
        }
        fazAlgo(contador);
        contador++;
        vez = false; // seta o estado para a próxima thread
        
        pthread_mutex_unlock(&mutex_x); // unlock
        pthread_cond_signal(&condY); // seta o sinal para  T2
    }
    pthread_exit(NULL);
}

// thread que verifica se 'contador' é um múltiplo de 100
void* T2(void* arg) {
    while(1) {
        pthread_mutex_lock(&mutex_x); // lock
        
        if (vez == true) { // Vez = true, significa que t1 tem que esperar o sinal de t2
            pthread_cond_wait(&condY, &mutex_x);
        }

        if (contador % 100 == 0) { // print do contador para multiplos de 100
            printf("multiplo de 100 %lld\n", contador);
        }
        vez = true; // seta o sinal para t1
        pthread_cond_signal(&condX); // da sinal para  T1
        pthread_mutex_unlock(&mutex_x); // unlock
    }
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    pthread_t threads[2];
    
    // inicializa o mutex (lock de exclusão mutua) e a variavel de condição
    pthread_mutex_init(&mutex_x, NULL);
    pthread_cond_init(&condY, NULL);
    pthread_cond_init(&condX, NULL);
    // cria as threads
    pthread_create(&threads[0], NULL, T1, NULL);
    pthread_create(&threads[1], NULL, T2, NULL);
   // espera elas completarem
    for (int i = 0; i < 2; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            fprintf(stderr, "errro pthread join");
        }
    }
    //desaloca variáveis e termina
    pthread_mutex_destroy(&mutex_x);
    pthread_cond_destroy(&condX);
    pthread_cond_destroy(&condY);
    return 0;
}