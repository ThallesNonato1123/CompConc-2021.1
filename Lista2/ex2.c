#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int bloqueadas = 0;
pthread_mutex_t x_mutex;
pthread_cond_t  x_cond;
 pthread_t *tid;
int nthreads;
int *vetor;



void barreira(int nthreads){
    pthread_mutex_lock(&x_mutex);
    if(bloqueadas == (nthreads-1)){
        pthread_cond_broadcast(&x_cond);
        bloqueadas = 0;
    } else{
        bloqueadas++;
        pthread_cond_wait(&x_cond,&x_mutex);
    }
    pthread_mutex_unlock(&x_mutex);
}

void *tarefa(void *arg){
    int id = *(int*)arg;
    int salto;
    int aux;
    for(salto = 1; salto<nthreads ; salto*=2){
        if(id >= salto){
            aux = vetor[id - salto];
            printf("Thread %d salto: %d aux:%d\n",(id), salto , aux);
            barreira(nthreads-salto);
            vetor[id] = aux + vetor[id];
            printf("SOMA: Thread %d salto: %d aux: %d\n vetor[%d]: %d\n",(id+1), salto ,aux, id, vetor[id]);
            barreira(nthreads-salto);
        }else break;
    }
    pthread_exit(NULL);
}

int main(int argc, char* argv[]){
    int id[nthreads];
    nthreads = atoi(argv[1]);
    pthread_cond_init(&x_cond,NULL);
    pthread_mutex_init(&x_mutex, NULL);
    
    tid =(pthread_t*) malloc(sizeof(pthread_t)*nthreads);
    vetor = (int*) malloc(sizeof(int) * nthreads);
    
    for(int i = 0 ; i<nthreads ; i++){
        vetor[i] = i;
    }

  for(int i = 0; i<nthreads ; i++){  // threads criadas com o identificador que define a paridade passada como parametro 
    id[i] = i;
    if(pthread_create(tid+i,NULL,tarefa,(void*) &id[i]))
        printf("Erro pthread_create\n");
    }

    for(int i = 0 ; i<nthreads ; i++){
        pthread_join(*(tid+i),NULL);
    }
    for(int i = 0 ; i<nthreads; i++){
        printf("%d ",vetor[i]);
    }
    printf("\n");

    free(vetor);
    pthread_mutex_destroy(&x_mutex);
    pthread_cond_destroy(&x_cond);
    return 0;
}
