
// It seems that the below includes can't be used in Azure Sphere
// #include <iostream>
// #include <stdexcept>
// #include <thread>

#include <unistd.h>

#include <applibs/log.h>

#include "WifiStation.h"
#include "Coordinator.h"

#include "MQTTMessageCodecFactoryImpl.h"

Coordinator::Coordinator(){
    Log_Debug("Coordinator::Coordinator()\n");

    // m_wifistation = new WifiStation(this);
    // if(m_wifistation == NULL)
    // {
    //     Log_Debug("new WifiSTation() error\n");
    // }

    // m_mqttclient = new MqttClient(this);
    // if(m_mqttclient == NULL)
    // {
    //     Log_Debug("new MqttClient() error\n");
    // }

    m_mqttfactory = new MQTTMessageCodecFactoryImpl();
    if(m_mqttfactory == NULL){
        Log_Debug("new MQTTMessageCodecFactoryImpl() error\n");
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

#if 0
    if(m_wifistation->init("TestAP") != true){
    // if(m_wifistation->init("TestAP_LAB") != true){
        Log_Debug("[Coordinator::initialize] error m_wifistation->init()\n");
        return false;
    }

    Log_Debug("AP NAME after is %s\n", m_wifistation->TEST_getAPName().c_str());

    // QOS(0), keepAlive(60 sec)
    if(m_mqttclient->init(0, 60) != true){
        Log_Debug("[Coordinator::initialize] error : m_mqttclient->init()\n");
        return false;
    }
#endif 

    return ret;
}


bool Coordinator::run(){
    bool ret = true;

    Log_Debug("Coordinator::run\n");
    
    if(m_wifistation->connect() != true){
        // for testing : storeAPInfo()
        m_wifistation->storeAPInfo("SK_WiFiGIGA4E04_5G", WifiStation::Security_Wpa2Psk, "KMR24@7966");
        // m_wifistation->storeAPInfo("ZIGBANG", WifiStation::Security_Wpa2Psk, "wlrqkd0#");
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

#if defined(MQTT_USE_MBEDTLS)
    if(m_mqttclient->connect(string("192.168.45.197"), string("8883"), "ca_cert.der") != true){
        Log_Debug("[Coordinator::onNetworkConnected] error : m_mqttclient->connect() with CA Cert\n");
    }
#else 
    /* MQTT Connect without CA Cert */
    if(m_mqttclient->connect(string("192.168.45.197"), string("1883")) != true){
        Log_Debug("[Coordinator::onNetworkConnected] error : m_mqttclient->connect() without CA Cert\n");
    }
#endif 

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


bool Coordinator::_processMessage(string& topic, string &message){
    Log_Debug("Coordinator::_processMessage\n");

    MQTTMessageCodec *mqttcodec;
    MQTTMessage *mqttmessage;

    string topic_;
    map<string, string> payload_;
    string jsonstr_;

    mqttcodec = m_mqttfactory->createCodec(topic);
    if(mqttcodec != NULL){
        mqttmessage = mqttcodec->decode(message);
        if(mqttmessage != NULL){
            topic_ = mqttmessage->getTopic();
            payload_ = mqttmessage->getPayload();
            jsonstr_ = mqttmessage->toJson();
            
            Log_Debug("Get Topic : %s\n", topic_.c_str());

            for (auto it = payload_.begin(); it != payload_.end(); ++it) {
                Log_Debug("[Payload] %s:%s\n", it->first.c_str(), it->second.c_str());
            }

            Log_Debug("Get json : %s\n", jsonstr_.c_str());
        }
        else{
            delete mqttcodec;
            return false;
        }
    }
    else{
        return false;
    }

    delete mqttmessage;
    delete mqttcodec;

    return true;
}


bool Coordinator::_generateMessage(string& topic, map<string,string>& payload){
    Log_Debug("Coordinator::_generateMessage()\n");

    MQTTMessageCodec *mqttcodec;
    MQTTMessage* mqttmessage;

    string topic_;
    map<string,string> payload_;
    string jsonstr_;

    mqttcodec = m_mqttfactory->createCodec(topic);
    if(mqttcodec != NULL){
        mqttmessage = mqttcodec->encode(payload);
        if(mqttmessage != NULL){
            topic_ = mqttmessage->getTopic();
            payload_ = mqttmessage->getPayload();
            jsonstr_ = mqttmessage->toJson();
            
            Log_Debug("Get Topic : %s\n", topic_.c_str());

            for (auto it = payload_.begin(); it != payload_.end(); ++it) {
                Log_Debug("[Payload] %s:%s\n", it->first.c_str(), it->second.c_str());
            }

            Log_Debug("Get json : %s\n", jsonstr_.c_str());
        }
        else{
            delete mqttcodec;
            return false;
        }
    }
    else{
        return false;
    }

    delete mqttmessage;
    delete mqttcodec;
    return false;
}