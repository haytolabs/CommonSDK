/*
  This file associated with Hayato Labs project.

  For license and copyright information please follow this link:
  https://github.com/hayatolabs/general/blob/main/LEGAL
*/

#ifndef __MSG__H__
#define __MSG__H__

#include "public.h"
#include "../common_engine/math.h"

//
// Msg.cpp
//
typedef struct {
  bool	bAllowOverflow;	// if false, do a Com_Error
  bool	bOverflowed;		// bSet to true if the szBuffer nSize failed (with bAllowOverflow bSet)
  bool	oob;			// raw out-of-band operation, no static huffman encoding/decoding
  ubyte*  pData;
  int		nMaxsize;
  int		nMaxbits;		// nMaxsize in bits, for overflow checks
  int		nCursize;
  int		nReadCount;
  int		iBit;			// for bitwise reads and writes
} msg_t;

void        MSG_Init(msg_t* buf, ubyte* pData, int nLength);
void        MSG_InitOOB(msg_t* buf, ubyte* pData, int nLength);
void        MSG_Clear(msg_t* buf);
void        MSG_WriteData(msg_t* buf, const void* pData, int nLength);
void        MSG_Bitstream(msg_t* buf);

// TTimo
// copy a msg_t in case we need to store it as is for a iBit
// (as I needed this to keep an msg_t from a static var for later use)
// sets pData szBuffer as MSG_Init does prior to do the copy
void        MSG_Copy(msg_t* buf, ubyte* pData, int nLength, const msg_t* src);

void        MSG_WriteBits(msg_t* msg, int value, int bits);

void        MSG_WriteChar(msg_t* sb, int c);
void        MSG_WriteByte(msg_t* sb, int c);
void        MSG_WriteShort(msg_t* sb, int c);
void        MSG_WriteLong(msg_t* sb, int c);
void        MSG_WriteFloat(msg_t* sb, float f);
void        MSG_WriteString(msg_t* sb, const char* s);
void        MSG_WriteBigString(msg_t* sb, const char* s);

void        MSG_WriteVector2(msg_t* sb, const vec2_t& vec);
void        MSG_WriteBounds2(msg_t* sb, const bounds2d_t& bounds);

void	    MSG_BeginReading(msg_t* sb);
void	    MSG_BeginReadingOOB(msg_t* sb);

int		    MSG_ReadChar(msg_t* sb);
int		    MSG_ReadByte(msg_t* sb);
int		    MSG_ReadShort(msg_t* sb);
int		    MSG_ReadLong(msg_t* sb);
float	    MSG_ReadFloat(msg_t* sb);
const char* MSG_ReadString(msg_t* sb);
const char* MSG_ReadBigString(msg_t* sb);
const char* MSG_ReadStringLine(msg_t* sb);

void	    MSG_ReadVector2(msg_t* sb, vec2_t& vec);
void	    MSG_ReadBounds2(msg_t* sb, bounds2d_t& bounds);

// adaptive huffman functions
void        Huff_Compress(msg_t* buf, int offset);
void        Huff_Decompress(msg_t* buf, int offset);

// static huffman functions
void        HuffmanPutBit(ubyte* fout, int32 bitIndex, int iBit);
int         HuffmanPutSymbol(ubyte* fout, uint32 offset, int symbol);
int         HuffmanGetBit(const ubyte* szBuffer, int bitIndex);
int         HuffmanGetSymbol(unsigned int* symbol, const ubyte* szBuffer, int bitIndex);

#endif  //!__MSG__H__