#include <stdio.h>
#include <pthread.h>

#define TAM 10000 // define o tamanho do vetor
#define NTHREADS 2 // define o numero de threads

int v[TAM];//declara o vetor com o tamanho de 100000


void * tarefa (void* arg){
    int ident = * (int *) arg; // identificador da thread, que também foi utilizada para definir a paridade que cada thread irá lidar (0 paridade par e 1 paridade impar)
    for(int i = ident ; i<= TAM ; i = i + 2){ // 
        v[i] = i * i;
    }
    pthread_exit(NULL);
}

int main(void){
    pthread_t tid[NTHREADS]; //identificador da thread no sistema
    
    int ident[NTHREADS]; // identificador da thread local
    
    for(int i = 0; i<NTHREADS ; i++){  // threads criadas com o identificador que define a paridade passada como parametro 
    
    ident[i] = i;
    
    if(pthread_create(&tid[i],NULL,tarefa,(void*) &ident[i]))
        printf("Erro pthread_create\n");
    }

    for(int i = 0 ; i<NTHREADS ; i++){ // espera as threads terminarem
        if(pthread_join(tid[i],NULL))
        printf("Erro");
    }
    
    for(int i = 0 ; i<=TAM ; i++){ // testa se os valores estão corretos
        if(v[i] != i*i)
            printf("Deu ruim");
    }
    
    printf("Deu bom");
    
    printf("\n");

    pthread_exit(NULL);
    return 0;
}