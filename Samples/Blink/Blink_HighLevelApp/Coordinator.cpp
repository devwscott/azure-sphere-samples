
// It seems that the below includes can't be used in Azure Sphere
// #include <iostream>
// #include <stdexcept>
// #include <thread>

#include <unistd.h>

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

    m_mqttclient = new MqttClient(this);
    if(m_mqttclient == NULL)
    {
        Log_Debug("new MqttClient() error\n");
    }

}



Coordinator::~Coordinator(){
    Log_Debug("Coordinator::~Coordinator()\n");

    delete m_wifistation;
    delete m_mqttclient;
}


bool Coordinator::initialize(){
    bool ret = true;

    Log_Debug("Coordinator::initialize\n");

    if(m_wifistation->init("TestAP") != true){
        Log_Debug("[Coordinator::initialize] error m_wifistation->init()\n");
        return false;
    }

    Log_Debug("AP NAME after is %s\n", m_wifistation->TEST_getAPName().c_str());

    // QOS(0), keepAlive(400ms)
    if(m_mqttclient->init(0, 400) != true){
        Log_Debug("[Coordinator::initialize] error : m_mqttclient->init()\n");
        return false;
    }
    

    return ret;
}


bool Coordinator::run(){
    bool ret = true;

    Log_Debug("Coordinator::run\n");
    
    if(m_wifistation->connect() != true){
        // for testing : storeAPInfo()
        m_wifistation->storeAPInfo("SK_WiFiGIGA4E04_5G", WifiStation::Security_Wpa2Psk, "KMR24@7966");
    }


    return ret;
}


void Coordinator::onNetworkConnected(){
    bool bConnected;
    string ipAddress;
    string macAddress;

    Log_Debug("Coordinator::onNetworkConnected\n");

    m_wifistation->getInfo(bConnected, ipAddress, macAddress);

    Log_Debug("Status : %s\n", bConnected==true?"Connected":"DisConnected");
    Log_Debug("IP : %s\n", ipAddress.c_str());
    Log_Debug("MAC : %s\n", macAddress.c_str());

    // for testing, disConnect()
    // usleep(2000*1000);
    // m_wifistation->disConnect();

    // m_mqttclient->connect(string("192.168.45.197"), string("1883"), NULL);

    /* MQTT Connect without CA Cert */
    if(m_mqttclient->connect(string("192.168.45.197"), string("1883")) != true){
        Log_Debug("[Coordinator::onNetworkConnected] error : m_mqttclient->connect()\n");
    }

    /* For Testing, publish MQTT */
    usleep(1000*1000);
    string test_topic = "hello";
    string test_msg = "mqttclient_msg";
    
    if(m_mqttclient->publish(test_topic, test_msg) != true){
        Log_Debug("[Coordinator::onNetworkConnected] error : m_mqttclient->publish()\n");
    }
 
    return;
}



void Coordinator::onNetworkDisconnected(){
    bool bConnected;
    string ipAddress;
    string macAddress;

    Log_Debug("Coordinator::onNetworkDisconnected\n");

    m_wifistation->getInfo(bConnected, ipAddress, macAddress);

    Log_Debug("Status : %s\n", bConnected==true?"Connected":"DisConnected");
    Log_Debug("IP : %s\n", ipAddress.c_str());
    Log_Debug("MAC : %s\n", macAddress.c_str());


    // for testing, connect()
    usleep(2000*1000);
    m_wifistation->connect();

    return;
}



void Coordinator::onReceiveTopic(string &topic, string &msg){
    Log_Debug("Coordinator::onReceiveTopic\n");

    Log_Debug("     topic:%s\n", topic.c_str());
    Log_Debug("     msg:%s\n", msg.c_str());

    return;
}


