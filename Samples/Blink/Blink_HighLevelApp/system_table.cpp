
#include <unistd.h>

#include <applibs/log.h>

#include "descriptor_factory.h"
#include "system_table.h"



SystemTable::SystemTable(){
    Log_Debug("SystemTable::SystemTable()\n");
    magic = 0x47;
    table_id = 0x00;
    section_syntax_indicator = 0x1;
    section_length = 0;
}


SystemTable::SystemTable(unsigned char* raw_buffer) : Table(raw_buffer, ((raw_buffer[2]<<8 | raw_buffer[3]) & 0x0FFF) + 4){
    Log_Debug("SystemTable::SystemTable(unsigned char* raw_buffer)\n");

    __decode_table_body__();
}


SystemTable::SystemTable(unsigned char* raw_buffer, unsigned int raw_length) : Table(raw_buffer, raw_length){
    Log_Debug("SystemTable::SystemTable(unsigned char* raw_buffer, unsigned int raw_lenght)\n");

    __decode_table_body__();
}


SystemTable::~SystemTable(){
    Log_Debug("SystemTable::~SystemTable()\n");

}


void SystemTable::PrintTable(){
    Log_Debug("SystemTable::PrintTable()\n");

    Table::PrintTable("SystemTable");

    Log_Debug("core_id : 0x%x\n", core_id);
    Log_Debug("version_number : 0x%x\n", version_number);

    descriptor->PrintDescriptor();
}


int SystemTable::GetVersionNumber(){
    Log_Debug("SystemTable::GetVersionNumber()\n");

    return version_number;
}


void SystemTable::SetVersionNumber(int value){
    Log_Debug("SystemTable::SetVersionNumber()\n");

    version_number = value;
}


int SystemTable::GetCoreId(){
    Log_Debug("SystemTable::GetCoreId()\n");

    return core_id;
}


void SystemTable::SetCoreId(int id){
    Log_Debug("SystemTable::SetCoreId()\n");

    core_id = id;
}


void SystemTable::__decode_table_body__(){
    Log_Debug("SystemTable::__decode_table_body__()\n");

    core_id = Read_On_Buffer(16);
    Skip_On_Buffer(2);
    version_number = Read_On_Buffer(5);
    Skip_On_Buffer(1);
    Skip_On_Buffer(8);
    Skip_On_Buffer(8);

    descriptor = DescriptorFactory::CreateDescriptor(this);

}


void SystemTable::__encode_update_table_length__(){
    Log_Debug("SystemTable::__encode_update_table_length__()\n");

    int default_length = 5; // from core_id to reserved

    section_length = 0;

    section_length += descriptor->GetDescriptorLength();
    section_length += default_length + 4/*crc32*/;

}


void SystemTable::__encode_prepare_table__(){
    Log_Debug("SystemTable::__encode_prepare_table__()\n");

}


void SystemTable::__encode_write_table_body__(){
    Log_Debug("SystemTable::__encode_write_table_body__()\n");

    Write_On_Buffer(core_id, 16);
    Write_On_Buffer(0xFF, 2);
    Write_On_Buffer(version_number, 5);
    Write_On_Buffer(0xFF, 1);
    Write_On_Buffer(0xFF, 8);
    Write_On_Buffer(0xFF, 8);

    descriptor->WriteDescriptor(this);
}


void SystemTable::SetDescriptor(Descriptor* desc){
    Log_Debug("SystemTable::SetDescriptor(Descriptor* desc)\n");

    descriptor = desc;
}




