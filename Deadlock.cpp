#include <iostream>
#include <thread>
#include <unistd.h>
#include <semaphore.h>

class DeadlockExample {
public:
    DeadlockExample(sem_t* mutex1, sem_t* mutex2) : mutex1(mutex1), mutex2(mutex2) {}

    void process1() {
        // Acquire mutex1
        sem_wait(mutex1);
        std::cout << "Thread 1 acquired mutex1" << std::endl;
        sleep(1); // Simulate some work

        // Attempt to acquire mutex2
        std::cout << "Thread 1 attempting to acquire mutex2" << std::endl;
        sem_wait(mutex2); // This will cause a deadlock
        std::cout << "Thread 1 acquired mutex2" << std::endl;

        // Release mutexes
        sem_post(mutex2);
        sem_post(mutex1);
    }

    void process2() {
        // Acquire mutex2
        sem_wait(mutex2);
        std::cout << "Thread 2 acquired mutex2" << std::endl;
        sleep(1); // Simulate some work

        // Attempt to acquire mutex1
        std::cout << "Thread 2 attempting to acquire mutex1" << std::endl;
        sem_wait(mutex1); // This will cause a deadlock
        std::cout << "Thread 2 acquired mutex1" << std::endl;

        // Release mutexes
        sem_post(mutex1);
        sem_post(mutex2);
    }

private:
    sem_t* mutex1;
    sem_t* mutex2;
};

int main() {
    sem_t mutex1, mutex2;
    sem_init(&mutex1, 0, 1);
    sem_init(&mutex2, 0, 1);

    DeadlockExample example(&mutex1, &mutex2);

    // Create two threads representing two processes
    std::thread t1(&DeadlockExample::process1, &example);
    std::thread t2(&DeadlockExample::process2, &example);

    t1.join();
    t2.join();

    sem_destroy(&mutex1);
    sem_destroy(&mutex2);

    return 0;
}
