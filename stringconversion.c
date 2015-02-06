// Module..: STRING_CONVERSION.C
// Version.: 1.0
// Compiler: CodeVisionAVR
// Chip....: ATmega88
// Date....: April 2008
// Author..: Udo Juerss
//-----------------------------------------------------------------------------

#include "stringconversion.h"
//-----------------------------------------------------------------------------

char SC_HEX_ARRAY[]  = "0123456789ABCDEF";
//-----------------------------------------------------------------------------

char sc_buffer[SC_BUFFER_LENGTH]; // Conversion buffer
//-----------------------------------------------------------------------------

char* sc_format(char* pstr,char digits) // Do formatted output
{
  char len = (char)(pstr - SC_BUFFER_OFS_PTR); // Get length of string

  while (digits--) // Sting loop
  {
    if (len) // If character created by conversion, just skip left in buffer
    {
      pstr--;
      len--;
    }
    else // It must be a fill character
    {
      *--pstr = SC_FILL_CHAR;
    }
  }

  return pstr;
}
//-----------------------------------------------------------------------------

char* sc_U08_to_str(unsigned char value,char digits)
{
  char* pstr = SC_BUFFER_OFS_PTR;

  if (value >= 100) *pstr++ = ('0' + (value % 1000 / 100));
  if (value >= 10) *pstr++ = ('0' + (value % 100 / 10));
  *pstr++ = ('0' + (value % 10));
  *pstr = 0;

  if (!digits) // If digits = 0, then return buffer start
  {
    return SC_BUFFER_OFS_PTR;
  }
  else // Do formatted output
  {
    return sc_format(pstr,digits);
  }
}
//-----------------------------------------------------------------------------

char* sc_S08_to_str(char value,char digits)
{
  char* pstr = SC_BUFFER_OFS_PTR;

  if (value < 0)
  {
    *pstr++ = '-';
    value = -value;
  }

  if (value >= 100) *pstr++ = ('0' + (value % 1000 / 100));
  if (value >= 10) *pstr++ = ('0' + (value % 100 / 10));
  *pstr++ = ('0' + (value % 10));
  *pstr = 0;

  if (!digits) // If digits = 0, then return buffer start
  {
    return SC_BUFFER_OFS_PTR;
  }
  else // Do formatted output
  {
    return sc_format(pstr,digits);
  }
}
//-----------------------------------------------------------------------------

char* sc_U8_to_hex(unsigned char value)
{
  char* pstr = sc_buffer;

  *pstr++ = SC_HEX_ARRAY[value >> 4];
  *pstr++ = SC_HEX_ARRAY[value & 0x0F];
  *pstr = 0;

  return (sc_buffer);
}
//-----------------------------------------------------------------------------

char* sc_char_to_bin(char value)
{
  char n;

  char* pstr = sc_buffer;

  for (n = 7; n < 8; n--)
  {
    if (value & (1 << n))
    {
      *pstr++ = '1';
    }
    else
    {
      *pstr++ = '0';
    }
  }

  *pstr = 0;

  return (sc_buffer);
}
//-----------------------------------------------------------------------------

char* sc_U16_to_str(unsigned int value,char digits)
{
  char* pstr = SC_BUFFER_OFS_PTR;

  if (value >= 10000) *pstr++ = ('0' + (value / 10000));
  if (value >= 1000) *pstr++ = ('0' + (value % 10000 / 1000));
  if (value >= 100) *pstr++ = ('0' + (value % 1000 / 100));
  if (value >= 10)*pstr++ = ('0' + (value % 100 / 10));
  *pstr++ = ('0' + (value % 10));
  *pstr = 0;

  if (!digits) // If digits = 0, then return buffer start
  {
    return SC_BUFFER_OFS_PTR;
  }
  else // Do formatted output
  {
    return sc_format(pstr,digits);
  }
}
//-----------------------------------------------------------------------------

char* sc_S16_to_str(int value,char digits)
{
  char* pstr = SC_BUFFER_OFS_PTR;

  if (value < 0)
  {
    *pstr++ = '-';
    value = -value;
  }

  if (value >= 10000) *pstr++ = ('0' + (value / 10000));
  if (value >= 1000) *pstr++ = ('0' + (value % 10000 / 1000));
  if (value >= 100) *pstr++ = ('0' + (value % 1000 / 100));
  if (value >= 10) *pstr++ = ('0' + (value % 100 / 10));
  *pstr++ = ('0' + (value % 10));
  *pstr = 0;

  if (!digits) // If digits = 0, then return buffer start
  {
    return SC_BUFFER_OFS_PTR;
  }
  else // Do formatted output
  {
    return sc_format(pstr,digits);
  }
}
//-----------------------------------------------------------------------------

char* sc_U16_to_hex(unsigned int value)
{
  tMEM16 data;
  char* pstr = sc_buffer;

  data.word = value;

  *pstr++ = SC_HEX_ARRAY[(data.byte[1]) >> 4];
  *pstr++ = SC_HEX_ARRAY[(data.byte[1]) & 0x0F];

  *pstr++ = SC_HEX_ARRAY[(data.byte[0]) >> 4];
  *pstr++ = SC_HEX_ARRAY[(data.byte[0]) & 0x0F];

  *pstr = 0;

  return (sc_buffer);
}
//-----------------------------------------------------------------------------

char* sc_internal_U32_to_str(unsigned long value,char* pstr)
{
  if (value >= 1000000000) *pstr++ = ('0' + (value / 1000000000));
  if (value >= 100000000) *pstr++ = ('0' + (value % 1000000000 / 100000000));
  if (value >= 10000000) *pstr++ = ('0' + (value % 100000000 / 10000000));
  if (value >= 1000000) *pstr++ = ('0' + (value % 10000000 / 1000000));
  if (value >= 100000) *pstr++ = ('0' + (value % 1000000 / 100000));
  if (value >= 10000) *pstr++ = ('0' + (value % 100000 / 10000));
  if (value >= 1000) *pstr++ = ('0' + (value % 10000 / 1000));
  if (value >= 100) *pstr++ = ('0' + (value % 1000 / 100));
  if (value >= 10) *pstr++ = ('0' + (value % 100 / 10));

  *pstr++ = ('0' + (value % 10));

  return pstr;
}
//-----------------------------------------------------------------------------

char* sc_U32_to_str(unsigned long value,char digits)
{
  char* pstr = SC_BUFFER_OFS_PTR;

  pstr = sc_internal_U32_to_str(value,pstr);
  *pstr = 0;

  if (!digits) // If digits = 0, then return buffer start
  {
    return SC_BUFFER_OFS_PTR;
  }
  else // Do formatted output
  {
    return sc_format(pstr,digits);
  }
}
//-----------------------------------------------------------------------------

char* sc_S32_to_str(long value,char digits)
{
  char* pstr = SC_BUFFER_OFS_PTR;

  if (value < 0)
  {
    *pstr++ = '-';
    value = -value;
  }

  if (value >= 1000000000) *pstr++ = ('0' + (value / 1000000000));
  if (value >= 100000000) *pstr++ = ('0' + (value % 1000000000 / 100000000));
  if (value >= 10000000) *pstr++ = ('0' + (value % 100000000 / 10000000));
  if (value >= 1000000) *pstr++ = ('0' + (value % 10000000 / 1000000));
  if (value >= 100000) *pstr++ = ('0' + (value % 1000000 / 100000));
  if (value >= 10000) *pstr++ = ('0' + (value % 100000 / 10000));
  if (value >= 1000) *pstr++ = ('0' + (value % 10000 / 1000));
  if (value >= 100) *pstr++ = ('0' + (value % 1000 / 100));
  if (value >= 10) *pstr++ = ('0' + (value % 100 / 10));
  *pstr++ = ('0' + (value % 10));
  *pstr = 0;

  if (!digits) // If digits = 0, then return buffer start
  {
    return SC_BUFFER_OFS_PTR;
  }
  else // Do formatted output
  {
    return sc_format(pstr,digits);
  }
}
//-----------------------------------------------------------------------------

char* sc_U32_to_hex(unsigned long value)
{
  tMEM32 data;
  char* pstr = sc_buffer;

  data.dword = value;

  *pstr++ = SC_HEX_ARRAY[(data.byte[3]) >> 4];
  *pstr++ = SC_HEX_ARRAY[(data.byte[3]) & 0x0F];

  *pstr++ = SC_HEX_ARRAY[(data.byte[2]) >> 4];
  *pstr++ = SC_HEX_ARRAY[(data.byte[2]) & 0x0F];

  *pstr++ = SC_HEX_ARRAY[(data.byte[1]) >> 4];
  *pstr++ = SC_HEX_ARRAY[(data.byte[1]) & 0x0F];

  *pstr++ = SC_HEX_ARRAY[(data.byte[0]) >> 4];
  *pstr++ = SC_HEX_ARRAY[(data.byte[0]) & 0x0F];

  *pstr = 0;

  return (sc_buffer);
}
//-----------------------------------------------------------------------------

char* sc_float_to_str(float value,char decimals)
{
  char* pstr = sc_buffer;

  if (value < 0)
  {
    *pstr++ = '-';
    value = -value;
  }

  pstr = sc_internal_U32_to_str((unsigned long)value,pstr);

  value -= (unsigned long)value;

  *pstr++ = '.';

  while (decimals--)
  {
    value *= 10.0;
  }

  pstr = sc_internal_U32_to_str((unsigned long)value,pstr);

  *pstr = 0;

  return sc_buffer;
}
