#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

int dim; // dimensão da matriz
int nthreads; // número de threads

int *m1; // matriz 1
int *m2; // matriz 2
int *resultado; // matriz resultante

pthread_t *tid; // id thread


void* tarefa(void *arg){
    int ident = *(int*) arg; // typecast ident
    
    for(int i = ident ; i < dim; i+=nthreads){
        for(int j = 0; j < dim; j++){
            for(int k = 0 ; k < dim; k++){
            resultado[i * dim + j]+= m1[i*dim + k ] * m2[k*dim + j]; // cada thread pega uma linha
            }
        }
    }
    pthread_exit(NULL);
}

void corretude(int *matrix){ // corretude é identificar se todos os valores das matrizes são iguais ao valor dimensão, já que m1 e m2 são matrizes que contém somente o elemento 1
    for(int i = 0; i<dim;i++){
        for(int j =0 ; j<dim;j++)
            if(matrix[i*dim+j] != dim)
                printf("deu ruim");
    } printf("Multiplicacao correta\n");
}

int main(int argc , char* argv[]){
    double inicio, fim ,delta; // variáveis para marcar o tempo
    
    GET_TIME(inicio);
    if(argc<3){
        puts("<dimensão da matriz> <numero de threads>");
        return 1;
    }
    dim = atoi(argv[1]); // dimensão
    nthreads = atoi(argv[2]); // numero de threads
    if(nthreads > dim ) nthreads = dim;
    int ident[nthreads];
    
    //aloca memoria para as matrizes
    
    m1 = (int*) malloc(sizeof(int) * dim * dim); // alocação de memoria de m1 e m2
    if(m1 == NULL){puts("erro malloc m1\n"); return 2; }
    
    m2 = (int*) malloc(sizeof(int) * dim * dim);
    if(m2 == NULL){puts("erro malloc m2\n"); return 2;}

    resultado = (int*)malloc(sizeof(int) * dim * dim);
    if(resultado == NULL){puts("erro malloc m2\n"); return 2;}
    
    //carrega as matrizes de entrada
    
    for(int i = 0 ; i<dim;i++ ){
        for(int j = 0 ; j< dim; j++){
            m1[i * dim + j] = 1;
            m2[i * dim + j] = 1;
        }
    }
    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo inicializão: %lf\n",delta);
    
    // alocacao das estruturas das threads e criação de threads

    GET_TIME(inicio);
    tid = (pthread_t*) malloc(sizeof(pthread_t) * nthreads); 
    if(tid == NULL){puts("Erro malloc tid"); return 2;}

    
    for(int i = 0 ; i<nthreads ; i++){
        ident[i] = i;
        if(pthread_create(tid+i, NULL, tarefa, (void*)&ident[i])){
            puts("Erro --pthread_create"); return 3;
        }
    }

     
     for(int i = 0 ; i<nthreads ; i++){
           pthread_join(*(tid+i),NULL);
        }
        
    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo multiplica: %lf\n",delta);

    GET_TIME(inicio);
        corretude(resultado);
    GET_TIME(fim);

    delta = fim - inicio;
    printf("Tempo Corretude: %lf\n",delta);

    GET_TIME(inicio);
    free(m1);
    free(m2);
    free(resultado);
    free(tid);
    GET_TIME(fim);

    delta = fim - inicio;

    printf("Tempo Finalizacao: %lf\n",delta);
}