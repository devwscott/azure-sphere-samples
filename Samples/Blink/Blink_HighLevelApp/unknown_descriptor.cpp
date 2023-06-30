
#include <unistd.h>

#include <applibs/log.h>

#include "unknown_descriptor.h"


UnknownDescriptor::UnknownDescriptor(){
    Log_Debug("UnknownDescriptor::UnknownDescriptor()\n");

}


UnknownDescriptor::UnknownDescriptor(BitReadWriter *rw) : Descriptor(rw){
    Log_Debug("UnknownDescriptor::UnknownDescriptor(BitReadWriter *rw)\n");
    rw->Skip_On_Buffer(descriptor_length * 8);

}


UnknownDescriptor::~UnknownDescriptor(){
    Log_Debug("UnknownDescriptor::~UnknownDescriptor()\n");

}


/* TEST Codes which should be located on main.cpp

#include "BitReadWriter.h"
#include "descriptor.h"
#include "unknown_descriptor.h"

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

    // Descriptor* desc = new UnknownDescriptor(bitRW);
    UnknownDescriptor* desc = new UnknownDescriptor(bitRW);
    desc->PrintDescriptor();
    desc->PrintDescriptor("Unknown DESC");

    value = desc->GetDescriptorTag();
    Log_Debug("desc.tag : 0x%x\n", value);

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