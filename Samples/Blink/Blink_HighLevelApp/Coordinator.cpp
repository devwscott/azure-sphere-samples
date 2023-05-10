
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



void Coordinator::onNetworkConnected(){
    Log_Debug("Coordinator::onNetworkConnected\n");

    return;
}



void Coordinator::onNetworkDisconnected(){
    Log_Debug("Coordinator::onNetworkDisconnected\n");

    return;
}

