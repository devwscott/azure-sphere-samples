
#include <unistd.h>

#include <applibs/log.h>

#include "light_control_descriptor.h"



LightControlDescriptor::LightControlDescriptor(){
    Log_Debug("LightControlDescriptor::LightControlDescriptor()\n");
    descriptor_tag = 0x0;   // 0x0 - LightControlDescriptor TAG
    descriptor_length = 0;

    m_deviceCount = 0;
    memset(m_deviceId, 0x0, sizeof(m_deviceId));
    memset(m_deviceStatus, 0x0, sizeof(m_deviceStatus));
}


LightControlDescriptor::LightControlDescriptor(BitReadWriter* rw) : Descriptor(rw){
    Log_Debug("LightControlDescriptor::LightControlDescriptor(BitReadWriter* rw)\n");

    int j=0;

    for(int i=descriptor_length; i>0; j++){
        m_deviceId[j] = rw->Read_On_Buffer(4);
        rw->Skip_On_Buffer(3);
        m_deviceStatus[j] = rw->Read_On_Buffer(1);

        i -= 1;
        m_deviceCount = j+1;
    }
}


LightControlDescriptor::~LightControlDescriptor(){
    Log_Debug("LightControlDescriptor::~LightControlDescriptor()\n");

}


void LightControlDescriptor::PrintDescriptor(){
    Log_Debug("LighControlDescriptor::PrintDescriptor()\n");

    Descriptor::PrintDescriptor("LightControlDescriptor");

    Log_Debug("m_deviceCount : %d\n", m_deviceCount);
    
    for(int i=0; i<m_deviceCount; i++){
        Log_Debug("deviceId : 0x%x\n", m_deviceId[i]);
        Log_Debug("deviceStatus : 0x%x\n", m_deviceStatus[i]);
    }
}


void LightControlDescriptor::WriteDescriptor(BitReadWriter* rw){
    Log_Debug("LightControlDescriptr::WriteDescriptor()\n");

    Descriptor::WriteDescriptor(rw);

    for(int i=0; i<m_deviceCount; i++){
        rw->Write_On_Buffer(m_deviceId[i], 4);
        rw->Skip_On_Buffer(3);
        rw->Write_On_Buffer(m_deviceStatus[i], 1);
    }
}


int LightControlDescriptor::GetDeviceCount(){
    Log_Debug("LightControlDescriptor::GetDeviceCount()\n");

    return m_deviceCount;
}


int LightControlDescriptor::GetDeviceStatus(int deviceIndex){
    Log_Debug("LightControlDescriptor::GetDeviceStatus()\n");

    return m_deviceStatus[deviceIndex];
}


void LightControlDescriptor::SetDeviceStatus(int deviceId, int deviceStatus){
    Log_Debug("LightControlDescriptor::SetDeviceStatus()\n");

    m_deviceId[m_deviceCount] = deviceId;
    m_deviceStatus[m_deviceCount] = deviceStatus;

    m_deviceCount++;
}


void LightControlDescriptor::updateDescriptorLength(){
    Log_Debug("LightControlDescriptor::updateDescriptorLength()\n");

    descriptor_length = m_deviceCount * 1;
}

