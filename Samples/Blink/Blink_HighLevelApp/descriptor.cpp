
#include <unistd.h>

#include <applibs/log.h>

#include "descriptor.h"


Descriptor::Descriptor(){
    Log_Debug("Descriptor::Descriptor()\n");

    descriptor_tag = 0xFF;
    descriptor_length = 0;
}


Descriptor::Descriptor(BitReadWriter* rw){
    Log_Debug("Descriptor::Descriptor(BitReadWriter* rw)\n");

    descriptor_tag = rw->Read_On_Buffer(8);
    descriptor_length = rw->Read_On_Buffer(8);
}


Descriptor::~Descriptor(){
    Log_Debug("Descriptor::~Descriptor()\n");

}


int Descriptor::GetDescriptorTag(){
    Log_Debug("Descriptor::GetDescriptorTag()\n");

    return descriptor_tag;
}


int Descriptor::GetDescriptorLength(){
    Log_Debug("Descriptor::GetDescriptorLength()\n");

    updateDescriptorLength();

    return descriptor_length + 2;   /* 2 : descriptor_tag + descriptor_length */
}


void Descriptor::PrintDescriptor(){
    Log_Debug("Descriptor::PrintDescriptor()\n");

    Log_Debug("descriptor_tag : 0x%02x\n", descriptor_tag);
    Log_Debug("descriptor_length : %d\n", descriptor_length);
}


void Descriptor::PrintDescriptor(string descriptor_name){
    Log_Debug("Descriptor::PrintDescriptor(string descriptor_name)\n");

    Log_Debug("descriptor_tag : 0x%02x (%s)\n", descriptor_tag, descriptor_name.c_str());
    Log_Debug("descriptor_length : %d\n", descriptor_length);
}


void Descriptor::WriteDescriptor(BitReadWriter* rw){
    Log_Debug("Descriptor::WriteDescriptor()\n");

    rw->Write_On_Buffer(descriptor_tag, 8);
    rw->Write_On_Buffer(descriptor_length, 8);
}

