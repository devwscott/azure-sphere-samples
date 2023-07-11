
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


/* Test Code for UnknownTable
#include "unknown_table.h"

    // Light control
    unsigned char buff_data[] = {0x47, 0x01, 0xb0, 0x0c, 0x52, 0x54, 0xd5, 0xff, 0xff, 0x00, 0x01, 0x1f, 0x12, 0x34, 0x56, 0x78};
    // Light Status
    // unsigned char buff_data[] = {0x47, 0x01, 0xb0, 0x0c, 0x52, 0x54, 0xd5, 0xff, 0xff, 0x01, 0x01, 0x3f, 0x12, 0x34, 0x56, 0x78};

    int value;

    unsigned char* buff;
    buff = new unsigned char[20];
    
    memcpy(buff, buff_data, sizeof(buff_data));

    // Table* table = new Table(buff, sizeof(buff_data));
    UnknownTable* table = new UnknownTable(buff, sizeof(buff_data));
    table->PrintTable();
    table->PrintTable("UnknownTable");
    value = table->GetSectionLength();
    Log_Debug("GetSectionLength : %d\n", value);

    unsigned char* buff2;
    buff2 = table->GetSection();
    for(int i=0; i<16; i++){
        Log_Debug("[%d] 0x%02x\n", i, buff2[i]);
    }

*/