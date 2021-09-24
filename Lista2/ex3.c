#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t mutex_x;
pthread_cond_t condX;
int contador = 0; // contador global 

void fazAlgo(int x){ // função faz algo, mesma função que os laboratórios anteriores
  int boba1, boba2;
  boba1=100; boba2=-100; while (boba2 < boba1) boba2++;
}

void* T1(void* arg) {
    while(1) { // loop while para incrementar o contador
        pthread_mutex_lock(&mutex_x); // lock 
        fazAlgo(contador); // chamada da função
        if(contador %100 == 0){
          pthread_cond_signal(&condX); // quando o numero do contador for divisivel por 100, é enviado um sinal para a segunda thread que estava aguardando, ser excutada
        }
        contador++; // incremento
        pthread_mutex_unlock(&mutex_x); // 
    }
}

void* T2(void* arg) {

    pthread_mutex_lock(&mutex_x);
    pthread_cond_wait(&condX, &mutex_x); // bloqueando a thread na fila da variável de condição, esperando receber o sinal que será dado quando for um multiplo de 100 
   // sem esse wait, casos como contador = 0 passaram, justamente por t2 começar em alguns casos primeiro que T1.
    while (contador%100 != 0) {  //  para o caso de T2 ser sinalizada e antes de retornar a execucao o valor do contador mudar
        pthread_cond_wait(&condX, &mutex_x); // fica em espera
    }
    printf("Multiplo de 100 encontrado: %d\n", contador);
    exit(1);
    pthread_mutex_unlock(&mutex_x);
}

int main(int argc, char* argv[]) {
    pthread_t threads[2];
    
    // inicializa o mutex (lock de exclusão mutua) e a variavel de condição
    pthread_mutex_init(&mutex_x, NULL);
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
    return 0;
}