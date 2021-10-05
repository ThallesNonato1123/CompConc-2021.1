#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define N 5
#define P 4
#define C 2

int Buffer[N];
int count = 0 , in = 0 , out = 0;
sem_t mutex;
sem_t cond_cons, cond_prod;
sem_t mutex;

void IniciaBuffer(int n){
    for( int i = 0 ; i < n ; i++)
        Buffer[i] = 0;
}

void ImprimeBuffer(int n){
    for(int i = 0 ; i < n ; i++){
        printf("%d " , Buffer[i]);
    }
    printf("\n");
}

void Insere(int item , int id){
    sem_wait(&mutex);
    printf("P[%d] quer inserir\n", id);

    while(count == N){
        printf("P[%d] bloqueou\n",id);
        sem_post(&cond_cons);
        sem_wait(&cond_prod);
        printf("P[%d] desbloqueou\n",id);
    }
    
    Buffer[in] = item;
    in = (in + 1) % N;
    count++;
    
    printf("P[%d] inseriu\n", id);
    ImprimeBuffer(N);
    sem_post(&mutex);
    
}

void Retira(int id){
   
    while(count < N){
        printf("C[%d] bloqueou\n",id);
        sem_wait(&cond_cons);  
        printf("C[%d] desbloqueou\n",id);
    }
    
    for(int i = 0 ; i < N ; i++)
        Buffer[i] = 0;
    
    printf("C[%d] consumiu tudo\n", id);
    ImprimeBuffer(N);
    count = 0;
    
    sem_post(&cond_prod);
    
}

void* produtor(void *arg){
    int *id = (int *) arg;
    printf("Sou a thread produtora %d\n", *id);
    
    while(1){
        Insere(*id,*id);
        sleep(1);
    }
}

void* consumidor(void *arg){
    int *id = (int *) arg;
    printf("Sou a thread consumidora %d\n",*id);
    while(1){
        Retira(*id);
        sleep(1);
    }
}

int main(){
    int i;
    sem_init(&cond_cons, 0 , 0);
    sem_init(&cond_prod, 0 , 0);
    sem_init(&mutex, 0 , 1);

    pthread_t tid[P+C];
    int *id[P+C];

    for(int i = 0 ; i < P + C; i++){
        id[i] = malloc(sizeof(int));
        if(id[i] == NULL) exit(-1);
        *id[i] = i+1;
    }
    
    IniciaBuffer(N);
    
    for( i = 0 ; i < P ; i++){
        if(pthread_create(&tid[i], NULL, produtor , (void *) id[i])) exit(-1);
        }
   
    for( i = 0 ; i < C ; i++){
        if(pthread_create(&tid[i+P],NULL, consumidor , (void *) id[i+P])) exit(-1);
        }

    for (int i = 0; i < P; i++) {
        pthread_join(tid[i], NULL);
        } 

    for (int i = 0; i < C; i++) {
        pthread_join(tid[i + P], NULL);
        } 
   
   sem_destroy(&cond_cons);
   sem_destroy(&cond_prod);
   sem_destroy(&mutex);
   return 0;
}