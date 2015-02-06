/* 
 * File:   stringconversion.h
 * Author: rene
 *
 * Created on 18. August 2013, 19:57
 */

#ifndef STRINGCONVERSION_H
#define	STRINGCONVERSION_H

#define SC_BUFFER_LENGTH 16 // Conversion buffer size
#define SC_BUFFER_OFS 5 // Conversion buffer offset for alignment
#define SC_FILL_CHAR 32 // Character for post digits

#define SC_BUFFER_OFS_PTR (sc_buffer + SC_BUFFER_OFS) // Buffer alignment addr

typedef union uMEM16 // Word and Byte access to 16-Bit data
{
  unsigned int word; // Word
  unsigned int* pword; // Pointer to Word
  unsigned char byte[2]; // Byte
  unsigned char* pbyte; // Pointer to Byte
} tMEM16;

typedef union uMEM32 // DWord, Word and Byte access to 32-Bit data
{
  unsigned long dword; // DWORD
  unsigned long* pdword[2]; // Pointer to U32

  float fp; // Float
  float* pfp[2]; // Pointer to float

  unsigned int word[2]; // Word
  unsigned int* pword[2]; // Pointer to U16

  unsigned char byte[4]; // Byte
  unsigned char* pbyte[2]; // Pointer to U08

} tMEM32;

char* sc_format(char* pstr,char digits);
char* sc_U08_to_str(unsigned char value,char digits);
char* sc_S08_to_str(char value,char digits);
char* sc_U8_to_hex(unsigned char value);
char* sc_char_to_bin(char value);
char* sc_U16_to_str(unsigned int value,char digits);
char* sc_S16_to_str(int value,char digits);
char* sc_U16_to_hex(unsigned int value);
char* sc_internal_U32_to_str(unsigned long value,char* pstr);
char* sc_U32_to_str(unsigned long value,char digits);
char* sc_S32_to_str(long value,char digits);
char* sc_U32_to_hex(unsigned long value);
char* sc_float_to_str(float value,char decimals);

#endif	/* STRINGCONVERSION_H */

