#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <math.h>

long int ntermos;
int nthreads;
int contador = 0;

double somatorio = 0.0;
pthread_t *tid;
pthread_mutex_t alock;


void *tarefa( void *arg){
    int id = * (int*) arg;
    for(int i = id ; i <ntermos ; i+=nthreads){
        pthread_mutex_lock(&alock);
        somatorio+=  pow(-1, i) * (1.0/(2.0*i+1));
        pthread_mutex_unlock(&alock);
    }
    
    
    pthread_mutex_lock(&alock);
    contador++;
    pthread_mutex_unlock(&alock);
    
    if(contador == nthreads){
        somatorio = 4* somatorio;
    }
    pthread_exit(NULL);
}

int main(int argc, char* argv[]){
    
    if(argc<3){
        fprintf(stderr, "<numero de termos> <numero de threads>\n");
        return 0;
    }
    
    ntermos =  atoll(argv[1]);
    nthreads = atoi(argv[2]);
    
    int ident[nthreads];
    
    tid = (pthread_t*) malloc(sizeof(pthread_t) * nthreads);
    if(tid == NULL) fprintf(stderr,"Erro pthread");

    for(int i = 0; i < nthreads ; i++){
        ident[i] = i;
        if(pthread_create(tid + i , NULL , tarefa , (void*)&ident[i])){
            fprintf(stderr, "Errro pthread_create");
            return 2;
        }
    }
    for(int i = 0 ; i<nthreads; i++){
        if(pthread_join(*(tid+i),NULL));
    }
    printf("somatorio: %lf\n", somatorio);
}