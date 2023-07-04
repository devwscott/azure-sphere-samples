
#include <unistd.h>

#include <applibs/log.h>

#include "light_status_descriptor.h"




LightStatusDescriptor::LightStatusDescriptor(){
    Log_Debug("LightStatusDescriptor::LightStatusDescriptor()\n");

    descriptor_tag = 0x01;
    descriptor_length = 0;

    m_deviceCount = 0;
    memset(m_deviceId, 0x0, sizeof(m_deviceId));
    memset(m_deviceStatus, 0x0, sizeof(m_deviceStatus));
}


LightStatusDescriptor::LightStatusDescriptor(BitReadWriter* rw) : Descriptor(rw){
    Log_Debug("LightStatusDescriptor::LightStatusDescriptor(BitReadWriter* rw)\n");
    int j=0;

    for(int i=descriptor_length; i>0; j++){
        m_deviceId[j] = rw->Read_On_Buffer(4);
        rw->Skip_On_Buffer(3);
        m_deviceStatus[j] = rw->Read_On_Buffer(1);

        i -= 1;
        m_deviceCount = j+1;
    }
}


LightStatusDescriptor::~LightStatusDescriptor(){
    Log_Debug("LightStatusDescriptor::~LightStatusDescriptor()\n");

}


void LightStatusDescriptor::PrintDescriptor(){
    Log_Debug("LightStatusDescriptor::PrintDescriptor()\n");

    Descriptor::PrintDescriptor("LightStatusDescriptor");

    Log_Debug("m_deviceCount : %d\n", m_deviceCount);
    for(int i=0; i<m_deviceCount; i++){
        Log_Debug("deviceId : 0x%x\n", m_deviceId[i]);
        Log_Debug("deviceStatus : 0x%x\n", m_deviceStatus[i]);
    }
}


void LightStatusDescriptor::WriteDescriptor(BitReadWriter* rw){
    Log_Debug("LightStatusDescriptor::WriteDescriptor()\n");

    Descriptor::WriteDescriptor(rw);

    for(int i=0; i<m_deviceCount; i++){
        rw->Write_On_Buffer(m_deviceId[i], 4);
        rw->Skip_On_Buffer(3);
        rw->Write_On_Buffer(m_deviceStatus[i], 1);
    }
}


int LightStatusDescriptor::GetDeviceCount(){
    Log_Debug("LightStatusDescriptor::GetDeviceCount()\n");

    return m_deviceCount;
}


int LightStatusDescriptor::GetDeviceStatus(int deviceIndex){
    Log_Debug("LightStatusDescriptor::GetDeviceStatus()\n");

    return m_deviceStatus[deviceIndex];
}


void LightStatusDescriptor::SetDeviceStatus(int deviceId, int deviceStatus){
    Log_Debug("LightStatusDescriptor::SetDeviceStatus()\n");

    m_deviceId[m_deviceCount] = deviceId;
    m_deviceStatus[m_deviceCount] = deviceStatus;
    m_deviceCount++;
}


void LightStatusDescriptor::updateDescriptorLength(){
    Log_Debug("LightStatusDescriptor::updateDescriptorLength()\n");

    descriptor_length = m_deviceCount * 1;
}


