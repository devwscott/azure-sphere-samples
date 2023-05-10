
#include <unistd.h>
#include <applibs/log.h>

#include "WifiStation.h"


WifiStation::WifiStation(IWifiStationListener *callback):m_apName(""), m_bConnected(false), m_running(false){
    Log_Debug("WifiStation::WifiStation()\n");
    m_listener = callback;
}



WifiStation::~WifiStation(){
    Log_Debug("WifiStation::~WifiStation()\n");
}



bool WifiStation::init(string apName){
    bool ret = true;
    
    Log_Debug("WifiStation::init()\n");
    m_apName = apName;

    if(pthread_create(&m_thread, NULL, &WifiStation::__statusThread__, this) != 0)
    {
        Log_Debug("[WifiStation::init()] error : pthread_create()\n");
        ret = false;
    }

    return ret;
}


bool WifiStation::connect(){
    bool ret = true;
    
    Log_Debug("WifiStation::connect()\n");

    return ret;
}


void WifiStation::disConnect(){
    Log_Debug("WifiStation::disConnect()\n");

    return;
}


bool WifiStation::storeAPInfo(string ssid, SecurityType secType, string passwd){
    bool ret = true;

    Log_Debug("WifiStation::storeAPInfo()\n");

    return ret;
}



void WifiStation::getInfo(bool &bConneced, string &ipAddr, string &macAddr){
    Log_Debug("WifiStation::getInfo()\n");

    return;
}



string WifiStation::TEST_getAPName(){
    return m_apName;
}



void* WifiStation::__statusThread__(void *arg){
    WifiStation *pWifiStation = static_cast<WifiStation*>(arg);

    static int cnt;
    int runInterval = 500*1000;     //500ms sleep()

    Log_Debug("WifiStation::__statusThread()\n");

    while(1){
        Log_Debug("[WifiStation::__statusThread()] running!(%d)\n", cnt++);
        usleep(runInterval);
    }

    return nullptr;
}



