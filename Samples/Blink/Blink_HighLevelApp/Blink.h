#pragma once

#include <pthread.h>
#include <applibs/gpio.h>
#include <string>

using namespace std;

class Blink
{
public:
    Blink(int pini, string c);
    ~Blink();

    void start(int interval);
    void stop();
    string getColor();

private:
    static void* blinkThread(void *blink);
    void blink();

    int m_pin;
    int m_interval;
    int m_ledFd;
    bool m_running;
    pthread_t m_thread;
    string color;

};

