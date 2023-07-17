
#include <unistd.h>

#include <applibs/log.h>

#include "descriptor_factory.h"
#include "light_table.h"



LightTable::LightTable(){
    Log_Debug("LightTable::LightTable()\n");
    magic = 0x47;
    table_id = 0x01;
    section_syntax_indicator = 0x1;
    section_length = 0;
}


LightTable::LightTable(unsigned char* raw_buffer) : Table(raw_buffer, ((raw_buffer[2]<<8 | raw_buffer[3])&0x0FFF)+4){
    Log_Debug("LightTable::LightTable(unsigned char* raw_buffer)\n");

    __decode_table_body__();
}


LightTable::LightTable(unsigned char* raw_buffer, unsigned int raw_length) : Table(raw_buffer, raw_length){
    Log_Debug("LightTable::LightTable(unsigned char* raw_buffer, unsigned int raw_lenght)\n");

    __decode_table_body__();
}


LightTable::~LightTable(){
    Log_Debug("LightTable::~LightTable()\n");

    //TODO: need to be checked
    // delete descriptor;
}


void LightTable::PrintTable(){
    Log_Debug("LightTable::PrintTable()\n");

    Table::PrintTable("LightTable");
    Log_Debug("core_id : 0x%x\n", core_id);
    Log_Debug("version_number : 0x%x\n", version_number);

    descriptor->PrintDescriptor();

}


int LightTable::GetVersionNumber(){
    Log_Debug("LightTable::GetVersionNumber()\n");
    return version_number;
}


void LightTable::SetVersionNumber(int version){
    Log_Debug("LightTable::SetVersionNumber()\n");

    version_number = version;
}


int LightTable::GetCoreId(){
    Log_Debug("LightTable::GetCoreId()\n");
    return core_id;
}


void LightTable::SetCoreId(int id){
    Log_Debug("LightTable::SetCoreId()\n");
    core_id = id;
}



void LightTable::SetDescriptor(Descriptor* desc){
    Log_Debug("LightTable::SetDescriptor()\n");

    descriptor = desc;
}


void LightTable::__decode_table_body__(){
    Log_Debug("LightTable::__decode_table_body__()\n");
    
    core_id = Read_On_Buffer(16);
    Skip_On_Buffer(2);
    version_number = Read_On_Buffer(5);
    Skip_On_Buffer(1);
    Skip_On_Buffer(8);
    Skip_On_Buffer(8);

    descriptor = DescriptorFactory::CreateDescriptor(this);
}


void LightTable::__encode_update_table_length__(){
    Log_Debug("LightTable::__encode_update_table_length__()\n");

    int default_length = 5; // from core_id to reserved

    section_length = 0;

    section_length += descriptor->GetDescriptorLength();
    section_length += default_length + 4/*crc32*/;
}


void LightTable::__encode_prepare_table__(){
    Log_Debug("LightTable::__encode_prepare_table__()\n");

    //Nothing todo!!
}


void LightTable::__encode_write_table_body__(){
    Log_Debug("LightTable::__encode_write_table_body__()\n");

    Write_On_Buffer(core_id, 16);
    Write_On_Buffer(0xFF, 2);
    Write_On_Buffer(version_number, 5);
    Write_On_Buffer(0xFF, 1);
    Write_On_Buffer(0xFF, 8);
    Write_On_Buffer(0xFF, 8);

    descriptor->WriteDescriptor(this);
}


/* Test codes for Light control encoding/decoding

#include "light_table.h"

    // Light control
    unsigned char buff_data[] = {0x47, 0x01, 0xb0, 0x0c, 0x52, 0x54, 0xd5, 0xff, 0xff, 0x00, 0x01, 0x1f, 0x12, 0x34, 0x56, 0x78};
    // Light Status
    // unsigned char buff_data[] = {0x47, 0x01, 0xb0, 0x0c, 0x52, 0x54, 0xd5, 0xff, 0xff, 0x01, 0x01, 0x3f, 0x12, 0x34, 0x56, 0x78};

    int value;

    unsigned char* buff;
    buff = new unsigned char[20];
    
    memcpy(buff, buff_data, sizeof(buff_data));

#if 0 //encoding
    LightTable* table = new LightTable();
    table->SetVersionNumber(10);
    table->SetCoreId(0x5254);

    Descriptor* desc = DescriptorFactory::CreateDescriptor(DescriptorFactory::LIGHT_CONTROL_DESCRIPTOR);
    table->SetDescriptor(desc);

    ((LightControlDescriptor*)desc)->SetDeviceStatus(1,1);
    desc->GetDescriptorLength();

    // table->SetDescriptor(DescriptorFactory::LIGHT_CONTROL_DESCRIPTOR);
    table->EncodeTable();
    table->PrintTable();

#else //decoding
    LightTable* table = new LightTable(buff, sizeof(buff_data));

    table->PrintTable();
    value = table->GetVersionNumber();
    Log_Debug("GetVersionNumber : %d\n", value);

    value = table->GetCoreId();
    Log_Debug("GetCoreId = 0x%x\n", value);

    value = table->GetSectionLength();
    Log_Debug("GetSectionLength : %d\n", value);
#endif

    unsigned char* buff2;
    buff2 = table->GetSection();
    for(int i=0; i<16; i++){
        Log_Debug("[%d] 0x%02x\n", i, buff2[i]);
    }
*/