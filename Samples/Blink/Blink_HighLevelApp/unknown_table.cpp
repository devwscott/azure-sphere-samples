
#include <unistd.h>

#include <applibs/log.h>

#include "unknown_table.h"


UnknownTable::UnknownTable():Table(){
    Log_Debug("UnknownTable::UnknownTable()\n");
    Log_Debug("Nothing to do\n");

}

UnknownTable::UnknownTable(unsigned char* raw_buffer, unsigned int raw_length) : Table(raw_buffer, raw_length){
    Log_Debug("UnknownTable::UnknownTable(unsigned char* raw_buffer, unsigned int raw_length)\n");
    Log_Debug("Nothing to do\n");

}

UnknownTable::~UnknownTable(){
    Log_Debug("UnknownTable::~UnknownTable()\n");
    Log_Debug("Nothing to do\n");

}

void UnknownTable::__decode_table_body__(){
    Log_Debug("UnknownTable::__decode_table_body__()\n");
    Log_Debug("Nothing to do\n");

}

