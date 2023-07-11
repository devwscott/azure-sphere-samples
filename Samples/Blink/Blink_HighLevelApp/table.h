#pragma once

#include <pthread.h>
#include <string>

#include "BitReadWriter.h"

using namespace std;


class Table : public BitReadWriter{
public:
    Table();
    Table(unsigned char* raw_buffer, unsigned int raw_length);
    virtual ~Table();

    virtual void PrintTable();
    virtual void PrintTable(string section_name);

    unsigned char* GetSection();
    unsigned int GetSectionLength();

    void EncodeTable();


protected:
    unsigned int magic;
    unsigned int table_id;
    unsigned int section_syntax_indicator;
    unsigned int section_length;
    
    unsigned int CRC_32;

    unsigned char* m_buffer;
    int m_buffer_length;

    virtual void __decode_table_body__() = 0;

    virtual void __encode_update_table_length__(){};
    virtual void __encode_prepare_table__(){};
    virtual void __encode_write_table_body(){};


private:
    void __decode_table_header__();
    void __encode_prepare_buffer__();
    void __encode_write_table_header__();
    void __encode_make_crc__();

};


