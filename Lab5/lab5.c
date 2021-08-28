#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int nthreads;
int *vetor;
int k = 0;
pthread_t *tid;

int bloqueadas = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;


void barreira(int nthreads){
    pthread_mutex_lock(&x_mutex);
    if (bloqueadas == (nthreads - 1 )){
        pthread_cond_broadcast(&x_cond);
        bloqueadas = 0;
    } else {
        bloqueadas++;
        pthread_cond_wait(&x_cond, &x_mutex);
    }
    pthread_mutex_unlock(&x_mutex);
}

void printaVetor(int nthreads){
    for(int i = 0 ; i < nthreads ; i++){
        printf("%d", vetor[i]);
    }
    printf("\n");
}

void *tarefa(void *args){
    int id = *(int*) args;
    int *somaLocal;
    somaLocal = (int*) malloc(sizeof(int*));
    
    for(int i = 0 ; i < nthreads; i++){

        for(int j = 0 ; j<nthreads; j++){
            *somaLocal += vetor[j];
        }
        printf("Thread %d:  v[%d]: %d passo:%d somaLocal: %d\n", id , id,vetor[id], i, *somaLocal);
        barreira(nthreads);
        vetor[id] = rand()% 10;
        printf("Novo valor para thread %d : %d\n", id, vetor[id]);
        barreira(nthreads);
    }
    pthread_exit(somaLocal);
}


int main(int argc, char* argv[]){
    
    nthreads = atoi(argv[1]);
    vetor = (int*) malloc(sizeof(int) * nthreads);
    int id[nthreads];
    int resultado[nthreads];
    int *resultThread;
    int teste;

    resultThread = (int*) malloc(sizeof(int));
    
    pthread_cond_init(&x_cond, NULL);
    pthread_mutex_init(&x_mutex, NULL);

    tid = (pthread_t*) malloc(sizeof(pthread_t) * nthreads);

        printf("vetor inicial: ");
    for(int i = 0 ; i < nthreads ; i++){
        vetor[i] = rand() % 10;
        printf("v[%d] = %d ", i , vetor[i]);
    }
    printf("\n");
    for(int i = 0 ; i<nthreads ; i++){
        id[i] = i;
        pthread_create(tid+i, NULL, tarefa, (void*) &id[i]);
    }

    for(int i = 0 ; i < nthreads ; i++){
        pthread_join(*(tid+i),(void**) &resultThread);
        resultado[i] = *resultThread;
    }

    teste = resultado[0];
    for(int i = 0 ; i<nthreads; i++){
        if(resultado[i] != teste)
            printf("teste falhou");
    }
    printf("teste passou\n");

    free(vetor);
    free(resultThread);
    free(tid);
    pthread_mutex_destroy(&x_mutex);
    pthread_cond_destroy(&x_cond);
}