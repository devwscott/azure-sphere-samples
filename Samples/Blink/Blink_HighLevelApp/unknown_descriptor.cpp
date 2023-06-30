
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

