#include <iostream>
#include <thread>
#include <unistd.h>
#include <semaphore.h>

class RaceProblemsEvolved {
public:
    int iprim = 65;
    int sleepTimeIncrement = 1;
    int sleepTimeDecrement = 1;

    RaceProblemsEvolved(int sleepTimeIncrement = 1, int sleepTimeDecrement = 1, sem_t* mutex = nullptr)
        : mutex(mutex), sleepTimeIncrement(sleepTimeIncrement), sleepTimeDecrement(sleepTimeDecrement) {}

    void increment() {
        sem_wait(mutex); 
        int reg = this->iprim;
        sleep(this->sleepTimeIncrement); 
        reg++; 
        this->iprim = reg; 
        sem_post(mutex); 
    }

    void decrement() {
        sem_wait(mutex); 
        int reg = this->iprim;
        sleep(this->sleepTimeDecrement); 
        reg--; 
        this->iprim = reg; 
        sem_post(mutex); 
    }

private:
    sem_t* mutex;
};

int main() {
    sem_t mutex;
    sem_init(&mutex, 0, 1);
    RaceProblemsEvolved rpe(1, 5, &mutex);

    std::thread t4(&RaceProblemsEvolved::increment, &rpe);
    std::thread t5(&RaceProblemsEvolved::decrement, &rpe);
    std::thread t6(&RaceProblemsEvolved::decrement, &rpe);


    t4.join();
    t5.join();
    t6.join();

    std::cout << "Value of iprim: " << rpe.iprim << std::endl;
    sem_destroy(&mutex);
    return 0;
}
