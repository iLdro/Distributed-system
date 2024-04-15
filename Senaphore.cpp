#include <iostream>
#include <thread>
#include <unistd.h>
#include <semaphore.h>

class RaceProblemsEvolved {
public:
    int iprim = 65;
    int sleepTimeIncrement = 1;
    int sleepTimeDecrement = 1;

    RaceProblemsEvolved(int sleepTimeIncrement = 1, int sleepTimeDecrement = 1, sem_t* mutex = nullptr) : mutex(mutex), sleepTimeIncrement(sleepTimeIncrement), sleepTimeDecrement(sleepTimeDecrement) {}

    void increment() {
        int reg;
        reg = this->iprim++;
        sem_wait(mutex); // Wait for the semaphore (lock)
        sleep(this->sleepTimeIncrement);
        reg++;
        this->iprim = reg;
        sem_post(mutex); // Release the semaphore (unlock)
    }

    void decrement() {
        int reg;
        reg = this->iprim--;
        sem_wait(mutex); // Wait for the semaphore (lock)
        sleep(this->sleepTimeDecrement);
        reg--;
        this->iprim = reg;
        sem_post(mutex); // Release the semaphore (unlock)
    }

private:
    sem_t* mutex;
};

int main() {
    sem_t mutex;
    sem_init(&mutex, 0, 1); // Initialize the semaphore with initial value 1

    RaceProblemsEvolved rpe(1, 2, &mutex);

    // Create multiple threads that access shared resource without proper synchronization
    std::thread t4(&RaceProblemsEvolved::decrement, &rpe);
    std::thread t5(&RaceProblemsEvolved::decrement, &rpe);
    std::thread t3(&RaceProblemsEvolved::increment, &rpe);

    t3.join();
    t4.join();
    t5.join();

    std::cout << "Value of iprim: " << rpe.iprim << std::endl;
    sem_destroy(&mutex);
    return 0;
}
