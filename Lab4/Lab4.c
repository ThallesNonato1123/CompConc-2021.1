#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define NTHREADS 4 
int x = 0;
int y = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;

// thread 1 = A. Thread 2 = B . Thread 3 = C. Thread 4 = d// thread 2 tem que imprimir antes de 1 e 4. thread 3 tem que imprimir depois de 1 e 4, e consequentemente 2
void *T1(void *t){
    
    pthread_mutex_lock(&x_mutex);
    while(x<1){
        pthread_cond_wait(&x_cond, &x_mutex);
    }
    printf("Fique a vontade\n");
    x++;
    pthread_cond_broadcast(&x_cond);
    pthread_mutex_unlock(&x_mutex);
    pthread_exit(NULL);

}

void *T2(void *t){
    printf("Seja bem-vindo\n");
    
    pthread_mutex_lock(&x_mutex);
    x++;
    pthread_mutex_unlock(&x_mutex);
    
    pthread_cond_broadcast(&x_cond);
    
    pthread_exit(NULL);
}

void *T3(void *t){
    
    pthread_mutex_lock(&x_mutex);
    while(x<3){
        pthread_cond_wait(&x_cond,&x_mutex);
    }
    printf("Volte sempre!\n");
    pthread_mutex_unlock(&x_mutex);
    pthread_exit(NULL);

}

void *T4(void *t){
    pthread_mutex_lock(&x_mutex);
    while(x<1){
        pthread_cond_wait(&x_cond,&x_mutex);
    }
    printf("Sente-se por favor.\n");
    x++;
    pthread_cond_broadcast(&x_cond);
    pthread_mutex_unlock(&x_mutex);
    pthread_exit(NULL);
    

}

int main(){
    pthread_t threads[NTHREADS];

    pthread_mutex_init(&x_mutex, NULL);
    pthread_cond_init(&x_cond, NULL);

    pthread_create(&threads[1], NULL,T2,NULL);
    pthread_create(&threads[0], NULL,T1,NULL);
    pthread_create(&threads[2], NULL,T3,NULL);
    pthread_create(&threads[3], NULL, T4,NULL);
      
    for (int i = 0; i < NTHREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&x_mutex);
    pthread_cond_destroy(&x_cond);

}