
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


/* Test codes for SystemTable encoding/decoding

#include "system_table.h"

    // System Table with Light Control desc?
    unsigned char buff_data[] = {0x47, 0x00, 0xb0, 0x0c, 0x52, 0x54, 0xd5, 0xff, 0xff, 0x00, 0x01, 0x1f, 0x12, 0x34, 0x56, 0x78};

    int value;
    unsigned char* buff;
    buff = new unsigned char[20];
    
    memcpy(buff, buff_data, sizeof(buff_data));

#if 0 //encoding
    SystemTable* table = new SystemTable();
    table->SetVersionNumber(10);
    table->SetCoreId(0x5254);

    Descriptor* desc = DescriptorFactory::CreateDescriptor(DescriptorFactory::LIGHT_CONTROL_DESCRIPTOR);
    table->SetDescriptor(desc);

    ((LightControlDescriptor*)desc)->SetDeviceStatus(1,1);
    ((LightControlDescriptor*)desc)->SetDeviceStatus(2,1);
    desc->GetDescriptorLength();

    table->EncodeTable();
    table->PrintTable();

#else //decoding
    SystemTable* table = new SystemTable(buff, sizeof(buff_data));

    table->PrintTable();
    value = table->GetVersionNumber();
    Log_Debug("GetVersionNumber : %d\n", value);

    value = table->GetCoreId();
    Log_Debug("GetCoreId = 0x%x\n", value);

    value = table->GetSectionLength();
    Log_Debug("GetSectionLength : %d\n", value);
#endif

    unsigned char* buff2;
    int length;
   
    buff2 = table->GetSection();
    length = table->GetSectionLength();
    
    for(int i=0; i<length; i++){
        Log_Debug("[%d] 0x%02x\n", i, buff2[i]);
    }

*/

