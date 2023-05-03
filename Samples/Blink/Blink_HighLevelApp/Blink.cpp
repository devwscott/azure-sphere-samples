#include <unistd.h>
// #include <iostream>
// #include <stdexcept>
// #include <thread>
#include "Blink.h"
#include <applibs/log.h>

Blink::Blink(int pin, string c) : m_pin(pin), m_interval(500), m_running(false), color(c)
{
    m_ledFd = GPIO_OpenAsOutput(m_pin, GPIO_OutputMode_PushPull, GPIO_Value_High);
}

Blink::~Blink()
{
    stop();
    close(m_ledFd);
}

void Blink::start(int interval)
{
    m_interval = interval;
    m_running = true;
    // std::thread t(blinkThread, this);
    // t.detach();
    int ret = pthread_create(&m_thread, NULL, &Blink::blinkThread, this);
    if(ret != 0)
    {
        m_running = false;
    }
}

void Blink::stop()
{
    m_running = false;
}

void* Blink::blinkThread(void *blink)
{
    Blink* pBlink = static_cast<Blink*>(blink);
    pBlink->blink();

    pthread_exit((void *) 0);
}

void Blink::blink()
{
    while(m_running) {
        GPIO_SetValue(m_ledFd, GPIO_Value_Low);
        Log_Debug(">>> LED On\n");
        usleep(m_interval * 1000);
        GPIO_SetValue(m_ledFd, GPIO_Value_High);
        Log_Debug(">>> LED Off\n");
        usleep(m_interval * 1000);
    }
}

string Blink::getColor()
{
    return color;
}
