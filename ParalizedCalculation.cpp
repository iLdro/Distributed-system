#include <iostream>
#include <thread>
#include <semaphore.h>

const int SEMA = 3;
int resultTable[4] = {0, 0, 0, 0};
sem_t sem[SEMA];

void Task1(int a , int b){
    int result = a + b;
    sem_wait(&sem[0]);
    resultTable[0] = result;
    sem_post(&sem[1]);
}

void Task2(int c , int d){
    int result = c + d;
    sem_wait(&sem[0]);
    resultTable[1] = result;
    sem_post(&sem[1]);
}

void Task3(int e , int f){
    int result = e + f;
    sem_wait(&sem[0]);
    resultTable[2] = result;
    sem_post(&sem[1]);
}