
// It seems that the below includes can't be used in Azure Sphere
// #include <iostream>
// #include <stdexcept>
// #include <thread>

#include <applibs/log.h>

#include "WifiStation.h"
#include "Coordinator.h"


Coordinator::Coordinator(){
    Log_Debug("Coordinator::Coordinator()\n");

    m_wifistation = new WifiStation(this);
    if(m_wifistation == NULL)
    {
        Log_Debug("new WifiSTation() error\n");
    }
}



Coordinator::~Coordinator(){
    Log_Debug("Coordinator::~Coordinator()\n");

    delete m_wifistation;
}


bool Coordinator::initialize(){
    bool ret = true;

    Log_Debug("Coordinator::initialize\n");

    m_wifistation->init("TestAP");
    
    return ret;
}


void Coordinator::onNetworkConnected(){
    Log_Debug("Coordinator::onNetworkConnected\n");

    return;
}



void Coordinator::onNetworkDisconnected(){
    Log_Debug("Coordinator::onNetworkDisconnected\n");

    return;
}

