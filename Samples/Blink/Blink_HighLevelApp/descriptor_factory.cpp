
#include <unistd.h>

#include <applibs/log.h>

#include "descriptor_factory.h"
#include "unknown_descriptor.h"
#include "light_control_descriptor.h"
#include "light_status_descriptor.h"



Descriptor* DescriptorFactory::CreateDescriptor(BitReadWriter* rw){
    Log_Debug("DescriptorFactory::CreateDescriptor(BitReadWriter* rw)\n");

    int descriptor_tag = (*(rw->GetCurrentBuffer()) & 0xFF);

    Log_Debug("[DescriptorFactory::CreateDescriptor] descriptor_tag = 0x%x\n", descriptor_tag);

    switch(descriptor_tag){
        case LIGHT_CONTROL_DESCRIPTOR:
            return new LightControlDescriptor(rw);
        
        case LIGHT_STATUS_DESCRIPTOR:
            return new LightStatusDescriptor(rw);
        
        default:
            return new UnknownDescriptor(rw);

    }
}


Descriptor* DescriptorFactory::CreateDescriptor(DescriptorTag tag){
    Log_Debug("DescriptorFactory::CreateDescriptor(DescriptorTag)\n");

    Log_Debug("[DescriptorFactory::CreateDescriptor] tag = 0x%x\n", tag);

    switch(tag){
        case LIGHT_CONTROL_DESCRIPTOR:
            return new LightControlDescriptor();
        
        case LIGHT_STATUS_DESCRIPTOR:
            return new LightStatusDescriptor();
        
        default:
            return new UnknownDescriptor();

    }
}


/* TEST codes for DescriptorFactory which should be located at main.cpp
#include "BitReadWriter.h"
#include "light_control_descriptor.h"
#include "light_status_descriptor.h"
#include "descriptor_factory.h"

    // Light control
    unsigned char buff_data[] = {0x47, 0x01, 0xb0, 0x0c, 0x52, 0x54, 0xd5, 0xff, 0xff, 0x00, 0x01, 0x1f, 0x12, 0x34, 0x56, 0x78};
    // Light Status
    // unsigned char buff_data[] = {0x47, 0x01, 0xb0, 0x0c, 0x52, 0x54, 0xd5, 0xff, 0xff, 0x01, 0x01, 0x3f, 0x12, 0x34, 0x56, 0x78};

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

#if 1
    Descriptor* desc = DescriptorFactory::CreateDescriptor(bitRW);

#else
    Descriptor* desc = DescriptorFactory::CreateDescriptor(DescriptorFactory::LIGHT_STATUS_DESCRIPTOR);
    ((LightStatusDescriptor*)desc)->SetDeviceStatus(3,1);
    desc->GetDescriptorLength();
    desc->WriteDescriptor(bitRW);
#endif

    desc->PrintDescriptor();

    value = desc->GetDescriptorTag();
    Log_Debug("desc.tag : 0x%x\n", value);

    if(value == DescriptorFactory::LIGHT_CONTROL_DESCRIPTOR){
        value = ((LightControlDescriptor*)desc)->GetDeviceCount();
        Log_Debug("[LightControl] dev count : 0x%x\n", value);

        value = ((LightControlDescriptor*)desc)->GetDeviceStatus(0);
        Log_Debug("[LightControl] dev status : 0x%x\n", value);
    }
    else if(value == DescriptorFactory::LIGHT_STATUS_DESCRIPTOR){
        value = ((LightStatusDescriptor*)desc)->GetDeviceCount();
        Log_Debug("[LightStatus] dev count : 0x%x\n", value);

        value = ((LightStatusDescriptor*)desc)->GetDeviceStatus(0);
        Log_Debug("[LightStatus] dev status : 0x%x\n", value);
    }


    value = desc->GetDescriptorLength();
    Log_Debug("desc.length : 0x%x\n", value);


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