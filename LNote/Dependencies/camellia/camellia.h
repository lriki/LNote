
#pragma once

typedef unsigned char Byte;
typedef unsigned long Word;

void Camellia_Ekeygen(  int, const Byte *, Byte * );
void Camellia_Encrypt(  int, const Byte *, const Byte *, Byte * );
void Camellia_Decrypt(  int, const Byte *, const Byte *, Byte * );

/*
#pragma once

typedef unsigned char came_Byte;

void Camellia_Ekeygen( const int block_size_, const came_Byte* key_, came_Byte* out_ex_key_ );
void Camellia_Encrypt( const int block_size_, const came_Byte* data_, const came_Byte* out_ex_key_, came_Byte* out_data_ );
void Camellia_Decrypt( const int block_size_, const came_Byte* data_, const came_Byte* out_ex_key_, came_Byte* out_data_ );
*/

