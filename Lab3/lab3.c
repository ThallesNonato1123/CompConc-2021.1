#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

long int dim;
int nthreads;
float *vetor;

pthread_t *tid;

typedef struct
{
   int id;
   float maior;
   float menor;
}fluxo;
fluxo *aux;


void *tarefa(void *arg){
    
    long int ini , fim , tamBloco;
    float menorLocal , maiorLocal;

    tamBloco = dim / nthreads;
    fluxo *args = (fluxo *) arg;
    ini = args->id * tamBloco;
    
    menorLocal = vetor[ini];
    maiorLocal = vetor[ini];
    
    
    if( args->id == nthreads - 1){ fim = dim; } else {fim = ini + tamBloco;} 
    
    for(long int i = ini ; i < fim ; i++){
        if(vetor[i] > maiorLocal) maiorLocal = vetor[i];
        if(vetor[i] < menorLocal) menorLocal = vetor[i]; 
    }
    
    args->maior = maiorLocal;
    args->menor = menorLocal;
    
    pthread_exit((void*)args);
}


int main(int argc , char* argv[]){
    
    if(argc < 3){
        fprintf(stderr, "Digite: %s <dimensao do vetor> <numero threads>\n", argv[0]);
        return 1;
    }
     
    dim = atoll(argv[1]);
    nthreads = atoi(argv[2]);
    double inicio, fim ,delta; 
    fluxo *args;

    //alocacao das estruturas e armazenamento de valores no vetor
    
    vetor = (float*) malloc(sizeof(float)*dim);
    if(vetor == NULL) fprintf(stderr, "Erro Malloc"); 
    
    args = malloc(sizeof(fluxo) * nthreads);
    aux = malloc(sizeof(fluxo));

    for(long int i = 0 ; i < dim ; i++)
        vetor[i] = 1000.1/(i+1);
    
    //forma sequencial
    GET_TIME(inicio);
    
    float maior;
    float menor;
    fluxo maiormenor;

    maior = vetor[0];
    menor = vetor[0];
    
    for(long int i = 0 ; i < dim ; i++){
        if(vetor[i] > maior) maior = vetor[i];
        if(vetor[i] < menor) menor = vetor[i];    
    }
   
    printf("Maior Sequencial: %f\n", maior);
    printf("Menor Sequencial: %f\n" , menor);
    
    GET_TIME(fim);
    
    delta = fim - inicio;
    printf("Tempo sequencial: %f\n",delta);

    
    //forma concorrente   
   
    GET_TIME(inicio);
    float maiores[nthreads];
    float menores[nthreads];

    tid = (pthread_t*) malloc(sizeof(pthread_t) * nthreads);
    if(tid == NULL) fprintf(stderr, "Erro pthread");

    for(int i = 0 ; i < nthreads ; i++){
        args[i].id = i;
        args[i].maior = 0;
        args[i].menor = 0;
        if(pthread_create(tid+i , NULL , tarefa , &args[i])){
            fprintf(stderr, "Erro -- pthread_create\n");
            return 3;
        }
    }

    for(int i = 0 ; i<nthreads ; i++){
        if(pthread_join(*(tid+i), (void**) &aux)){
            fprintf(stderr,"Erro - pthread_create\n");
            return 3;
        }
        maiores[i] = aux->maior;
        menores[i] = aux->menor;
    }
    
    float maiorConc = maiores[0];
    float menorConc = menores[0];

     for(long int i = 0 ; i < nthreads ; i++){
        if(maiores[i] > maiorConc) maiorConc = maiores[i];
        if(menores[i] < menorConc){

        } menorConc = menores[i];    
    }

    printf("Maior Concorrente: %f\n", maiorConc);
    printf("Menor Concorrente: %f\n" , menorConc);
    
    GET_TIME(fim);
    
    delta = fim - inicio;
    printf("Tempo concorrente: %f\n",delta);
}