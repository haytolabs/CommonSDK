/*
  This file associated with Hayato Labs project.

  For license and copyright information please follow this link:
  https://github.com/hayatolabs/general/blob/main/LEGAL
*/

#ifndef HAYATOLABS_MEM_H
#define HAYATOLABS_MEM_H

#include "memzone.h"
#include "public.h"

/*
===========================================================================

   IMemoryManager занимается выделением памяти, позволяет создавать
   свои собственные блоки памяти для локального участка кода

===========================================================================
*/
class IMemoryManager
{
  public:
    virtual void ReleaseGeneralMemory() = 0;
    virtual size_t GetAvailableGeneralMemory() = 0;
    virtual void *MallocDebug(size_t nSize, memorytag_t iTag, const char *pszLabel, const char *pszFilename,
                              int iLine) = 0;
    virtual void *Malloc(size_t nSize, memorytag_t iTag) = 0;
    virtual void Free(void *ptr) = 0;

    virtual void ReleaseZoneMemory(memoryzone_t *pMemoryZone) = 0;
    virtual size_t GetAvailableZoneMemory(memoryzone_t *pMemoryZone) = 0;
    virtual memoryzone_t *MallocZoneMemory(size_t nSize) = 0;
};

extern IMemoryManager *g_pMemoryManager;

#define NO_MEM_REDEFINTION
#ifndef NO_MEM_REDEFINTION

#undef malloc
#ifdef NDEBUG
#define malloc(nSize) g_pMemoryManager->Malloc(nSize, TAG_GENERAL)
#define mallocTag(nSize, iTag) g_pMemoryManager->Malloc(nSize, iTag)
#else
#define malloc(nSize) g_pMemoryManager->MallocDebug(nSize, TAG_GENERAL, "", __FILE__, __LINE__)
#define mallocTag(nSize, iTag) g_pMemoryManager->MallocDebug(nSize, iTag, "", __FILE__, __LINE__)
#endif

#undef free
#define free g_pMemoryManager->Free

/*

inline void operator delete( void* p ) noexcept
{
   g_pMemoryManager->Free( p );
};

inline void* operator new[]( size_t s )
   {
      return g_pMemoryManager->Malloc( s, TAG_NEW );
   };

inline void operator delete[]( void* p ) noexcept
   {
      g_pMemoryManager->Free( p );
   };
#ifdef _MSC_VER
#pragma warning( pop )
#endif
*/

inline void *operator new(size_t s, memorytag_t iTag)
{
    return g_pMemoryManager->Malloc(s, iTag);
};

inline void operator delete(void *p, memorytag_t iTag)
{
    g_pMemoryManager->Free(p);
};

inline void *operator new[](size_t s, memorytag_t iTag)
{
    return g_pMemoryManager->Malloc(s, iTag);
};

inline void operator delete[](void *p, memorytag_t iTag)
{
    g_pMemoryManager->Free(p);
};

#endif //! NO_MEM_REDEFINTION

#endif /* HAYATOLABS_MEM_H */
