/*
  This file associated with Hayato Labs project.

  For license and copyright information please follow this link:
  https://github.com/hayatolabs/general/blob/main/LEGAL
*/

#ifndef NFCXX_NETMSG_H
#define NFCXX_NETMSG_H

#include "public.h"

/*
===========================================================================

   CBinaryPackage

===========================================================================
*/

class CBinaryPackage
{
  private:
    ubyte *pWriteData;      // указатель на буфер
    const ubyte *nReadData; // указатель на буфер
    size_t nAllocatedSize;  // размер буфера
    size_t nCurrentSize;
    mutable size_t nWriteBit;
    mutable size_t nReadCount;
    mutable size_t nReadBit;

    bool bAllowOverflow; // если false, то выскакивает ошибка при выходе за пределы буфера
    bool bOverflowed; // если вышли за предел буфера
    bool bChanged;
    mutable uint64 temp;

  public:
    CBinaryPackage()
    {
        InitWrite(NULL, 0);
    }
    CBinaryPackage(ubyte *pData, size_t nLength)
    {
        InitWrite(pData, nLength);
    }
    CBinaryPackage(const ubyte *pData, size_t nLength)
    {
        InitRead(pData, nLength);
    }

    void InitWrite(ubyte *pData, size_t nLength);
    void InitRead(const ubyte *pData, size_t nLength);

    ubyte *GetWriteData();
    const ubyte *GetReadData() const;
    int GetMaxSize() const;

    void SetbAllowOverflow(bool bSet);
    bool IsbOverflowed() const;
    int GetSize() const;
    void SetSize(size_t nSize);

    int GetWriteBit() const;
    void SetWriteBit(int bit);
    int GetNumBitsWritten() const;
    int GetRemainingSpace() const;
    int GetRemainingWriteBits() const;
    void SaveWriteState(int &s, int &b, uint64 &t) const;
    void RestoreWriteState(int s, int b, uint64 t);
    int GetReadCount() const;
    void SetReadCount(size_t nBytes);
    int GetReadBit() const;
    void SetReadBit(int nBit);
    int GetNumBitsRead() const;
    int GetRemainingData() const;
    int GetRemainingnReadBits() const;
    void SaveReadState(int &c, int &b) const;
    void RestoreReadState(int c, int b);
    void BeginWriting();
    void WriteByteAlign();
    void WriteBits(int m_iValue, int nBits);
    void BeginReading() const;
    void ReadByteAlign() const;
    int nReadBits(int nBits) const;
    void SetHasChanged(bool b)
    {
        bChanged = b;
    }
    bool HasChanged() const
    {
        return bChanged;
    }

    void WriteBool(bool c);
    void WriteInt8(int8 c);
    void WriteUInt8(uint8 c);
    void WriteInt16(int16 c);
    void WriteUInt16(uint16 c);
    void WriteInt32(int32 c);
    void WriteInt64(int64 c);
    void WriteFloat(float f);
    void WriteString(const char *szBuffer, size_t nBufferSize);
    void WriteData(const void *pData, size_t nLength);

    bool ReadBool(bool &c) const;
    int8 ReadInt8(int8 &c) const;
    uint8 ReadUInt8(uint8 &c) const;
    int16 ReadInt16(int16 &c) const;
    uint16 ReadUInt16(uint16 &c) const;
    int32 ReadInt32(int32 &c) const;
    int64 ReadInt64(int64 &c) const;
    float ReadFloat(float &f) const;
    int ReadString(char *szBuffer, size_t nBufferSize) const;
    int ReadData(void *pData, size_t nLength) const;

  private:
    bool CheckOverflow(int nBits);
    ubyte *GetByteSpace(int nLength);
};

#endif //!__NETMSG__H__
