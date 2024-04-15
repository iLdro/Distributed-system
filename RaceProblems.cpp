#include <iostream>
#include <thread>
#include <windows.h>    
#include <unistd.h>

using namespace std;

class RaceProblems {
    public :
        int iprim = 65;

    void increment() {
        this->iprim++;
    }

    void decrement() {
        this->iprim--;
    }
};

class RaceProblemsEvolved {
    public :
        int iprim = 65;
        int sleepTimeIncrement = 1;
        int sleepTimeDecrement = 1;

    RaceProblemsEvolved(int sleepTimeIncrement = 1 , int sleepTimeDecrement = 1) {
        this->sleepTimeIncrement = sleepTimeIncrement;
        this->sleepTimeDecrement = sleepTimeDecrement;
    }

    void increment() {
        int reg;
        reg = this->iprim++;
        sleep(this->sleepTimeIncrement);
        reg  ++;
        this->iprim = reg;
    }

    void decrement() {
        int reg;
        reg = this->iprim--;
        sleep(this->sleepTimeDecrement);
        reg  --;
        this->iprim = reg;
    }
};


int main(){
    RaceProblems rp;

    
    std::thread t1(&RaceProblems::increment, &rp);
    std::thread t2(&RaceProblems::decrement, &rp);

    t1.join();
    t2.join();

    std::cout << "Value of iprim: " << rp.iprim << std::endl;

    RaceProblemsEvolved rpe(1, 2);


    std::thread t4(&RaceProblemsEvolved::decrement, &rpe);
    std::thread t3(&RaceProblemsEvolved::increment, &rpe);

    t3.join();
    t4.join();

    std::cout << "Value of iprim: " << rpe.iprim << std::endl;

    
    RaceProblemsEvolved rpe2(2, 1);

    std::thread t5(&RaceProblemsEvolved::increment, &rpe2);
    std::thread t6(&RaceProblemsEvolved::decrement, &rpe2);

    t5.join();
    t6.join();

    std::cout << "Value of iprim: " << rpe2.iprim << std::endl;

    



    return 0;
}

