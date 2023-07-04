
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
        // rw->Skip_On_Buffer(3);
        rw->Write_On_Buffer(0xFF, 3);
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


/*  TEST codes for LightStatusDescriptor which should be located at main.cpp

#include "BitReadWriter.h"
#include "descriptor.h"
#include "light_status_descriptor.h"

    unsigned char buff_data[] = {0x47, 0x01, 0xb0, 0x0c, 0x52, 0x54, 0xd5, 0xff, 0xff, 0x00, 0x01, 0x1f, 0x12, 0x34, 0x56, 0x78};
    int value;

    unsigned char* buff;
    buff = new unsigned char[20];
    
    memcpy(buff, buff_data, sizeof(buff_data));

    BitReadWriter *bitRW = new BitReadWriter();
    bitRW->SetBuffer(buff);

    value = bitRW->Read_On_Buffer(8);
    Log_Debug("magic : 0x%x\n", value);

    value = bitRW->Read_On_Buffer(8);
    Log_Debug("table_id : 0x%x\n", value);

    value = bitRW->Read_On_Buffer(1);
    Log_Debug("section_syntax : 0x%x\n", value);

    bitRW->Skip_On_Buffer(3);

    value = bitRW->Read_On_Buffer(12);
    Log_Debug("section_length : 0x%x\n", value);

    value = bitRW->Read_On_Buffer(16);
    Log_Debug("coreid : 0x%x\n", value);
    Log_Debug("coreid : %c%c\n", (value&0xFF00)>>8, value&0xFF);

    bitRW->Skip_On_Buffer(2);

    value = bitRW->Read_On_Buffer(5);
    Log_Debug("version : 0x%x\n", value);

    bitRW->Skip_On_Buffer(17);

#if 1 // decoding
    LightStatusDescriptor* desc = new LightStatusDescriptor(bitRW);
#else //encoding
    LightStatusDescriptor* desc = new LightStatusDescriptor();

    desc->SetDeviceStatus(3, 1);

    value = desc->GetDescriptorLength();
    Log_Debug("desc.length : 0x%x\n", value);

    desc->WriteDescriptor(bitRW);
#endif

    desc->PrintDescriptor();

    value = desc->GetDescriptorTag();
    Log_Debug("desc.tag : 0x%x\n", value);

    value = desc->GetDescriptorLength();
    Log_Debug("desc.length : 0x%x\n", value);

    value = desc->GetDeviceCount();
    Log_Debug("dev count : 0x%x\n", value);

    value = desc->GetDeviceStatus(0);
    Log_Debug("dev status : 0x%x\n", value);



#if 0
    value = bitRW->Read_On_Buffer(8);
    Log_Debug("desc.tag : 0x%x\n", value);

    value = bitRW->Read_On_Buffer(8);
    Log_Debug("desc.length : 0x%x\n", value);


    value = bitRW->Read_On_Buffer(4);
    Log_Debug("device id : 0x%x\n", value);


    bitRW->Skip_On_Buffer(3);

    value = bitRW->Read_On_Buffer(1);
    Log_Debug("device status : 0x%x\n", value);
#endif


    value = bitRW->Read_On_Buffer(32);
    Log_Debug("CRC_32 : 0x%x\n", value);

    unsigned char* buff2;
    buff2 = bitRW->GetBufAdd();

    for(int i=0; i<16; i++){
        Log_Debug("[%d] 0x%02x\n", i, buff2[i]);
    }
*/