#include <iostream>
#include <thread>
#include <semaphore.h>

using namespace std;

class ParallelCalculation {
public:
    ParallelCalculation(int a, int b, int c, int d, int e, int f) 
        : a(a), b(b), c(c), d(d), e(e), f(f), index(0) {
        sem_init(&sem1, 0, 0);
        sem_init(&sem2, 0, 0);
        sem_init(&sem3, 0, 0);
        sem_init(&sem4, 0, 0);
    }

    ~ParallelCalculation() {
        sem_destroy(&sem1);
        sem_destroy(&sem2);
        sem_destroy(&sem3);
        sem_destroy(&sem4);
    }

    void task1() {
        results[store_result(a + b)] = a + b;
        cout << "Task1: " << a << " + " << b << " = " << a + b << endl;
        sem_post(&sem1);
    }

    void task2() {
        results[store_result(c - d)] = c - d;
        cout << "Task2: " << c << " - " << d << " = " << c - d << endl;
        sem_post(&sem2);
    }

    void task3() {
        results[store_result(e + f)] = e + f;
        cout << "Task3: " << e << " + " << f << " = " << e + f << endl;
        sem_post(&sem3);
    }

    void task4() {
        sem_wait(&sem1);
        sem_wait(&sem2);
        intermediate_result = results[0] * results[1];
        cout << "Intermediate Result: " << results[0] << " * " << results[1] << " = " << intermediate_result << endl;
        sem_post(&sem4);
    }

    void final_task() {
        sem_wait(&sem4);
        sem_wait(&sem3);
        final_result = intermediate_result * results[2];
        cout << "Final Result: " << intermediate_result << " * " << results[2] << " = " << final_result << endl;
    }

    int store_result(int value) {
        return index++;
    }

private:
    int a, b, c, d, e, f;
    int results[3];
    int index;
    int intermediate_result, final_result;
    sem_t sem1, sem2, sem3, sem4;
};

int main() {
    ParallelCalculation calc(1, 2, 3, 4, 5, 6);

    thread t1(&ParallelCalculation::task1, &calc);
    thread t2(&ParallelCalculation::task2, &calc);
    thread t3(&ParallelCalculation::task3, &calc);
    thread t4(&ParallelCalculation::task4, &calc);
    thread t5(&ParallelCalculation::final_task, &calc);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();

    return 0;
}
