#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "h264decoder.h"

//char errortext[ET_SIZE];
/*!
 ************************************************************************
 * \brief
 *    Error handling procedure. Print error message to stderr and exit
 *    with supplied code.
 * \param text
 *    Error message
 ************************************************************************
 */

void error(char *text, int code)
{
  //fprintf(stderr, "%s\n", text);
  //exit(code);
  printf("%s\n", text);
}


/*!
 ************************************************************************
 * \brief
 *    Exit program if memory allocation failed (using error())
 * \param where
 *    string indicating which memory allocation failed
 ************************************************************************
 */
void no_mem_exit(char *where)
{
//   snprintf(errortext, ET_SIZE, "Could not allocate memory: %s",where);
   error (where, 100);
}



/////////////////////////
// vlc.c ////////////////
/////////////////////////

/*!
 ************************************************************************
 * \brief
 *    mapping rule for ue(v) syntax elements
 * \par Input:
 *    lenght and info
 * \par Output:
 *    number in the code table
 ************************************************************************
 */
void linfo_ue(int len, int info, int *value1, int *dummy)
{
//  *value1 = (int)pow(2,(len/2))+info-1; // *value1 = (int)(2<<(len>>1))+info-1;
  *value1 = (int)(1<<(len>>1))+info-1; 
}

/*!
 ************************************************************************
 * \brief
 *    mapping rule for se(v) syntax elements
 * \par Input:
 *    lenght and info
 * \par Output:
 *    signed mvd
 ************************************************************************
 */
void linfo_se(int len,  int info, int *value1, int *dummy)
{
  int n;
//  n = (int)pow(2,(len/2))+info-1;
  int len2 = len/2;
  if (len2<0) {
          len2 = -len2;
          n = (int)(1>>len2)+info-1;
  }
  else
          n = (int)(1<<len2)+info-1;

  *value1 = (n+1)/2;
  if((n & 0x01)==0)                           // lsb is signed bit
    *value1 = -*value1;
}

/*!
 ************************************************************************
 * \brief
 *  Moves the read pointer of the partition forward by one symbol
 *
 * \param byte buffer[]
 *    containing VLC-coded data bits
 * \param int totbitoffset
 *    bit offset from start of partition
 * \param int type
 *    expected data type (Partiotion ID)
 * \return  int info, len
 *    Length and Value of the next symbol
 *
 * \note
 *    As in both nal_bits.c and nal_part.c all data of one partition, slice,
 *    picture was already read into a buffer, there is no need to read any data
 *    here again.
 * \par
 *    GetVLCInfo was extracted because there should be only one place in the
 *    source code that has knowledge about symbol extraction, regardless of
 *    the number of different NALs.
 * \par
 *    This function could (and should) be optimized considerably
 * \par
 *    If it is ever decided to have different VLC tables for different symbol
 *    types, then this would be the place for the implementation
 * \par
 *    An alternate VLC table is implemented based on exponential Golomb codes.
 *    The encoder must have a matching define selected.
 *  
 ************************************************************************
 */
int GetVLCSymbol (byte buffer[],int totbitoffset,int *info, int bytecount)
{

  register int inf;
  long byteoffset;      // byte from start of buffer
  int bitoffset;      // bit from start of byte
  int ctr_bit=0;      // control bit for current bit posision
  int bitcounter=1;
  int len;
  int info_bit;

  byteoffset= totbitoffset/8;
  bitoffset= 7-(totbitoffset%8);
  ctr_bit = (buffer[byteoffset] & (0x01<<bitoffset));   // set up control bit

  len=1;
  while (ctr_bit==0)
  {                 // find leading 1 bit
    len++;
    bitoffset-=1;           
    bitcounter++;
    if (bitoffset<0)
    {                 // finish with current byte ?
      bitoffset=bitoffset+8;
      byteoffset++;
    }
    ctr_bit=buffer[byteoffset] & (0x01<<(bitoffset));
  }
    // make infoword
  inf=0;                          // shortest possible code is 1, then info is always 0
  for(info_bit=0;(info_bit<(len-1)); info_bit++)
  {
    bitcounter++;
    bitoffset-=1;
    if (bitoffset<0)
    {                 // finished with current byte ?
      bitoffset=bitoffset+8;
      byteoffset++;
    }
    if (byteoffset > bytecount)
    {
      return -1;
    }
    inf=(inf<<1);
    if(buffer[byteoffset] & (0x01<<(bitoffset)))
      inf |=1;
  }

  *info = inf;
  return bitcounter;           // return absolute offset in bit from start of frame
}

/*!
 ************************************************************************
 * \brief
 *    read FLC codeword from UVLC-partition 
 ************************************************************************
 */
int readSyntaxElement_FLC(SyntaxElement *sym, Bitstream *currStream)
{
  int frame_bitoffset = currStream->frame_bitoffset;
  byte *buf = currStream->streamBuffer;
  int BitstreamLengthInBytes = currStream->bitstream_length;

  if ((GetBits(buf, frame_bitoffset, &(sym->inf), BitstreamLengthInBytes, sym->len)) < 0)
    return -1;

  currStream->frame_bitoffset += sym->len; // move bitstream pointer
  sym->value1 = sym->inf;

  return 1;
}



/*!
 ************************************************************************
 * \brief
 *  Reads bits from the bitstream buffer
 *
 * \param byte buffer[]
 *    containing VLC-coded data bits
 * \param int totbitoffset
 *    bit offset from start of partition
 * \param int bytecount
 *    total bytes in bitstream
 * \param int numbits
 *    number of bits to read
 *
 ************************************************************************
 */


int GetBits (byte buffer[],int totbitoffset,int *info, int bytecount, 
             int numbits)
{

  register int inf;
  long byteoffset;      // byte from start of buffer
  int bitoffset;      // bit from start of byte

  int bitcounter=numbits;

  byteoffset= totbitoffset/8;
  bitoffset= 7-(totbitoffset%8);

  inf=0;
  while (numbits)
  {
    inf <<=1;
    inf |= (buffer[byteoffset] & (0x01<<bitoffset))>>bitoffset;
    numbits--;
    bitoffset--;
    if (bitoffset < 0)
    {
      byteoffset++;
      bitoffset += 8;
      if (byteoffset > bytecount)
      {
        return -1;
      }
    }
  }

  *info = inf;
  return bitcounter;           // return absolute offset in bit from start of frame
}     
