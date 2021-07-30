#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

long int dim;
int nthreads;
int contador = 0;
float *vetor;
float *maiores;
float *menores;


pthread_t *tid;

typedef struct{
    float maior;
    float menor;
}fluxo;


void *tarefa(void *arg){
    
    fluxo *aux;
    aux = malloc(sizeof(fluxo));
    int id = * (int*) arg;
    long int ini , fim , tamBloco;
    float menorConc , maiorConc;
    
    tamBloco = dim / nthreads;
    ini = id *  tamBloco;
    
    if( id == nthreads - 1){fim = dim;} else{ fim = ini + tamBloco;}

    menorConc = vetor[ini];
    maiorConc = vetor[ini];
     
     for(long int i = ini ; i < fim ; i++){
        if(vetor[i] > maiorConc) maiorConc = vetor[i];
        if(vetor[i] < menorConc) menorConc = vetor[i]; 
    }
    
    
    maiores[contador] = maiorConc;
    menores[contador] = menorConc;
    contador++;

    if(contador == nthreads ){
        menorConc = menores[0];
        maiorConc = maiores[0];
        
        for(int i = 0 ; i < contador; i++ ){
            if(maiores[i] > maiorConc) maiorConc = maiores[i];
            if(menores[i] < menorConc) menorConc = menores[i];
        }
        aux->maior = maiorConc;
        aux->menor = menorConc;
    }
    
    pthread_exit((void*) aux);
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
    int ident[nthreads];
    float maiorConc = 0 , menorConc = 0;
    
    vetor = (float*) malloc(sizeof(float)*dim);
    if(vetor == NULL) fprintf(stderr, "Erro Malloc"); 
    

    maiores = (float*)malloc(sizeof(float)*nthreads);
    menores = (float*)malloc(sizeof(float)*nthreads);
    
    for(long int i = 0 ; i < dim ; i++)
        vetor[i] = 1000.1/(i+1);
    
    //forma sequencial
   
    GET_TIME(inicio);
    float maior;
    float menor;

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
    
    tid = (pthread_t*) malloc(sizeof(pthread_t) * nthreads);
    if(tid == NULL) fprintf(stderr, "Erro pthread");

    for(int i = 0 ; i < nthreads ; i++){
        ident[i] = i;
        if(pthread_create(tid+i , NULL , tarefa , (void*) &ident[i])){
            fprintf(stderr, "Erro -- pthread_create\n");
            return 3;
        }
    }

    for(int i = 0 ; i<nthreads ; i++){
        if(pthread_join(*(tid+i), (void**) &args)){
            fprintf(stderr,"Erro - pthread_create\n");
            return 3;
        }
        maiorConc += args->maior;
        menorConc += args->menor;
        free(args);
    }
    GET_TIME(fim);
    
    delta = fim - inicio;
        printf("Maior Concorrente: %f\n", maiorConc);
        printf("Menor Concorrente: %f\n" , menorConc);
        printf("Tempo Concorrente: %f\n",delta);
        
        free(vetor);
        free(args);
        free(maiores);
        free(menores);
        free(tid);

}
