#include <iostream>
#include <thread>
#include <unistd.h>
#include <semaphore.h>
#include <cstdlib>

class ApplicationSequence{
    public :
        ApplicationSequence(){
            sem_init(&sem_firefox, 0, 1);
            sem_init(&sem_chrome, 0, 0);
            sem_init(&sem_code, 0, 0);
        }

        ~ApplicationSequence(){
            sem_destroy(&sem_firefox);
            sem_destroy(&sem_chrome);
            sem_destroy(&sem_code);
        }

        void LaunchFirefox(){
            sem_wait(&sem_firefox);
            std::system("pip");
            std::cout << "Firefox Launched" << std::endl;
        }   

        void LaunchChrome(){
            sem_wait(&sem_chrome);
            std::system("npm");
            std::cout << "Chrome Launched" << std::endl;
        }

        void LaunchCode(){
            sem_wait(&sem_code);
            std::system("code");
            std::cout << "Code Launched" << std::endl;
        }

        void RunSequence(){
            sem_post(&sem_firefox);
            sleep(1);
            sem_post(&sem_chrome);
            sleep(1);
            sem_post(&sem_code);
        }

    private:
        sem_t sem_firefox;
        sem_t sem_chrome;
        sem_t sem_code;
};


int main(){
    ApplicationSequence app;
    std::thread t1(&ApplicationSequence::LaunchFirefox, &app);
    std::thread t2(&ApplicationSequence::LaunchChrome, &app);
    std::thread t3(&ApplicationSequence::LaunchCode, &app);

    app.RunSequence();

    t1.join();
    t2.join();
    t3.join();

    return 0;
}