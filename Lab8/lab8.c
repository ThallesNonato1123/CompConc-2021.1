#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>

#define NTHREADS 4 
int x = 0;
sem_t t2_sem , t1_sem, t4_sem;


// thread 1 = A. Thread 2 = B . Thread 3 = C. Thread 4 = d// thread 2 tem que imprimir antes de 1 e 4. thread 3 tem que imprimir depois de 1 e 4, e consequentemente 2
void *T1(void *t){
    
   
    sem_wait(&t2_sem);
    printf("Fique a vontade\n");
    sem_post(&t2_sem);
    sem_post(&t1_sem);
    pthread_exit(NULL);

}

void *T2(void *t){
    
    sem_post(&t2_sem);
    printf("Seja bem-vindo\n");
    pthread_exit(NULL);
}

void *T3(void *t){
    
    sem_wait(&t4_sem);
    sem_wait(&t1_sem);
    printf("Volte sempre!\n");
    sem_post(&t4_sem);
    sem_post(&t1_sem);
    pthread_exit(NULL);
}

void *T4(void *t){
    sem_wait(&t2_sem);
    printf("Sente-se por favor.\n");
    sem_post(&t2_sem);
    sem_post(&t4_sem);
    pthread_exit(NULL);
}

int main(){
    pthread_t threads[NTHREADS];

    sem_init(&t1_sem,0,0);
    sem_init(&t2_sem,0,1);
    sem_init(&t4_sem,0,0);
    pthread_create(&threads[1], NULL,T2,NULL);
    pthread_create(&threads[0], NULL,T3,NULL);
    pthread_create(&threads[2], NULL,T4,NULL);
    pthread_create(&threads[3], NULL, T1,NULL);
      
    for (int i = 0; i < NTHREADS; i++) {
        pthread_join(threads[i], NULL);
    }

   sem_destroy(&t1_sem);
   sem_destroy(&t2_sem);
   sem_destroy(&t4_sem);

}