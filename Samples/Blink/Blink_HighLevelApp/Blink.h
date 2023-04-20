#pragma once

#include <pthread.h>
#include <applibs/gpio.h>

class Blink
{
public:
    Blink(int pin);
    ~Blink();

    void start(int interval);
    void stop();

private:
    static void* blinkThread(void *blink);
    void blink();

    int m_pin;
    int m_interval;
    int m_ledFd;
    bool m_running;
    pthread_t m_thread;

};

