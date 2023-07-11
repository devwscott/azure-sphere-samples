
#include <unistd.h>

#include <applibs/log.h>

#include "table.h"


Table::Table(){
    Log_Debug("Table::Table()\n");
    
    m_buffer = NULL;
    m_buffer_length = 0;
}


Table::Table(unsigned char* raw_buffer, unsigned int raw_length){
    Log_Debug("Table::Table(unsigned char* raw_buffer, unsigned int raw_length)\n");

    m_buffer_length = raw_length;

    m_buffer = new unsigned char[m_buffer_length];
    memcpy(m_buffer, raw_buffer, m_buffer_length);

    SetBuffer(m_buffer);

    __decode_table_header__();
}


Table::~Table(){
    Log_Debug("Table::~Table()\n");

}


void Table::PrintTable(){
    Log_Debug("Table::PrintTable()\n");

    Log_Debug("magic : 0x%x\n", magic);
    Log_Debug("table_id : 0x%x\n", table_id);
    Log_Debug("section_syntax_indicator : 0x%x\n", section_syntax_indicator);
    Log_Debug("section_length : 0x%x\n", section_length);

}


void Table::PrintTable(string section_name){
    Log_Debug("Table::PrintTable(string section_name)\n");

    Log_Debug("section_name : %s\n", section_name.c_str());
    Log_Debug("magic : 0x%x\n", magic);
    Log_Debug("table_id : 0x%x\n", table_id);
    Log_Debug("section_syntax_indicator : 0x%x\n", section_syntax_indicator);
    Log_Debug("section_length : 0x%x \n", section_length);

}


unsigned char* Table::GetSection(){
    Log_Debug("Table::GetSection()\n");

    return m_buffer;
}


unsigned int Table::GetSectionLength(){
    Log_Debug("Table::GetSectionLength()\n");

    return m_buffer_length;
}


void Table::EncodeTable(){
    Log_Debug("Table::EncodeTable()\n");

    __encode_update_table_length__();
    __encode_prepare_table__();

    __encode_prepare_buffer__();
    __encode_write_table_header__();
    __encode_write_table_body();
    __encode_make_crc__();
}


void Table::__decode_table_header__(){
    Log_Debug("Table::__decode_table_header__()\n");
    
    magic = Read_On_Buffer(8);
    table_id = Read_On_Buffer(8);
    section_syntax_indicator = Read_On_Buffer(1);
    Skip_On_Buffer(1);
    Skip_On_Buffer(2);
    section_length = Read_On_Buffer(12);

}


void Table::__encode_prepare_buffer__(){
    Log_Debug("Table::__encode_prepare_buffer__()\n");

    if(m_buffer != NULL){
        delete []m_buffer;
    }

    m_buffer_length = section_length + 4; /* magic + table_id + section_syntax_indicator + reserved(3) + section_length*/

    m_buffer = new unsigned char[m_buffer_length];
    memset(m_buffer, 0xFF, m_buffer_length);

    SetBuffer(m_buffer);

}


void Table::__encode_write_table_header__(){
    Log_Debug("Table::__encode_write_table_header__()\n");

    Write_On_Buffer(magic, 8);
    Write_On_Buffer(table_id, 8);
    Write_On_Buffer(section_syntax_indicator, 1);
    Write_On_Buffer(0xFF, 1);
    Write_On_Buffer(0xFF, 2);
    Write_On_Buffer(section_length, 12);    
}


void Table::__encode_make_crc__(){
    Log_Debug("Table::__encode_make_crc__()\n");

    unsigned char* crc_buf;

    crc_buf = new unsigned char[section_length - 4];    /* section_length - crc32(4bytes) */
    memcpy(crc_buf, &m_buffer[4], section_length-4);

    // TODO: created the calc_CRC32() function
    // CRC_32 = calc_CRC32(crc_buf, section_length-4);

    Write_On_Buffer(CRC_32, 32);
}

