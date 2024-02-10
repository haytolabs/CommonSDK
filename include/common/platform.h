/*
  This file associated with Hayato Labs project.

  For license and copyright information please follow this link:
  https://github.com/hayatolabs/general/blob/main/LEGAL
*/

#ifndef __PLATFORM__H__
#define __PLATFORM__H__

#pragma once

#ifdef _WIN32
//
// Prevent tons of unused windows definitions
//
#define WIN32_LEAN_AND_MEAN
#define NOWINRES
#define NOSERVICE
#define NOMCX
#define NOIME
#if !defined(_X360)
#pragma warning(push, 1)
#pragma warning(disable : 4005)
#include <Windows.h>

#include <WinUser.h>
#pragma warning(pop)
#endif
#undef PostMessage
#undef LoadLibrary

#endif // WIN32

/*
===========================================================================

   mutex, signal, atomic integer & memory barrier.

===========================================================================
*/

// RB begin
#if defined(_WIN32)
typedef CRITICAL_SECTION mutexHandle_t;
typedef HANDLE signalHandle_t;
typedef LONG interlockedInt_t;
#else
#include <pthread.h>

struct signalHandle_t
{
    // DG: all this stuff is needed to emulate Window's Event API
    //     (CreateEvent(), SetEvent(), WaitForSingleObject(), ...)
    pthread_cond_t cond;
    pthread_mutex_t mutex;
    int waiting; // number of threads waiting for a signal
    bool manualReset;
    bool signaled; // is it signaled right now?
};

typedef pthread_mutex_t mutexHandle_t;
typedef int interlockedInt_t;
#endif

// _ReadWriteBarrier() does not translate to any instructions but keeps the compiler
// from reordering read and write instructions across the barrier.
// MemoryBarrier() inserts and CPU instruction that keeps the CPU from reordering reads and writes.
#if defined(_MSC_VER)
#pragma intrinsic(_ReadWriteBarrier)
#define SYS_MEMORYBARRIER                                                                                              \
    _ReadWriteBarrier();                                                                                               \
    MemoryBarrier()
#elif defined(__GNUC__) // FIXME: what about clang?
// according to http://en.wikipedia.org/wiki/Memory_ordering the following should be equivalent to the stuff above..
// #ifdef __sync_syncronize
#define SYS_MEMORYBARRIER                                                                                              \
    asm volatile("" ::: "memory");                                                                                     \
    __sync_synchronize()
#endif

/*
================================================================================================

   Локальное хранилище потоков с реализацией под различные платформы
   Может хранить указатели или integer

================================================================================================
*/

#if defined(_WIN32)
class CSystemThreadLocalStorage
{
  public:
    CSystemThreadLocalStorage()
    {
        tlsIndex = TlsAlloc();
    }

    CSystemThreadLocalStorage(const ptrdiff_t &val)
    {
        tlsIndex = TlsAlloc();
        TlsSetValue(tlsIndex, (LPVOID)val);
    }

    ~CSystemThreadLocalStorage()
    {
        TlsFree(tlsIndex);
    }

    operator ptrdiff_t()
    {
        return (ptrdiff_t)TlsGetValue(tlsIndex);
    }

    const ptrdiff_t &operator=(const ptrdiff_t &val)
    {
        TlsSetValue(tlsIndex, (LPVOID)val);
        return val;
    }

    DWORD tlsIndex;
};
#else
class CSystemThreadLocalStorage
{
  public:
    CSystemThreadLocalStorage()
    {
        pthread_key_create(&key, NULL);
    }

    CSystemThreadLocalStorage(const ptrdiff_t &val)
    {
        pthread_key_create(&key, NULL);
        pthread_setspecific(key, (const void *)val);
    }

    ~CSystemThreadLocalStorage()
    {
        pthread_key_delete(key);
    }

    operator ptrdiff_t()
    {
        return (ptrdiff_t)pthread_getspecific(key);
    }

    const ptrdiff_t &operator=(const ptrdiff_t &val)
    {
        pthread_setspecific(key, (const void *)val);
        return val;
    }

    pthread_key_t key;
};
#endif

#define NF_TLS CSystemThreadLocalStorage

#ifdef Yield
#undef Yield
#endif

enum core_t
{
    CORE_ANY = -1,
    CORE_0A,
    CORE_0B,
    CORE_1A,
    CORE_1B,
    CORE_2A,
    CORE_2B
};

typedef unsigned int (*xthread_t)(void *);

enum xthreadPriority
{
    THREAD_LOWEST,
    THREAD_BELOW_NORMAL,
    THREAD_NORMAL,
    THREAD_ABOVE_NORMAL,
    THREAD_HIGHEST
};

#define DEFAULT_THREAD_STACK_SIZE (256 * 1024)

const int MAX_CRITICAL_SECTIONS = 4;

enum
{
    CRITICAL_SECTION_ZERO = 0,
    CRITICAL_SECTION_ONE,
    CRITICAL_SECTION_TWO,
    CRITICAL_SECTION_THREE
};

/*
===========================================================================

   IPlatform содержит в себе непортируемые ф-ии,
   определяемые для каждой ОС по отдельности

===========================================================================
*/
class IPlatform
{
  public:
    virtual void RestoreTimerResolution() = 0;
    virtual void MakeHighTimerResolution() = 0;
    virtual void MakeHighProcessPriority() = 0;
    virtual void SetUnhandledExceptionFilter(LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter) = 0;
    virtual const char *GetExceptionName(DWORD code) = 0;
    virtual int GetCurrentTime() = 0;

    virtual FARPROC GetProcAddress(dynamicLibrary_t hModule, const char *procName) = 0;

    virtual dynamicLibrary_t LoadLibraryFromDirectory(std::string path) = 0;
    virtual dynamicLibrary_t LoadLibrary(const char *path) = 0;
    virtual void UnloadLibrary(dynamicLibrary_t hModule) = 0;
    virtual void UnloadAllLibraries() = 0;
    virtual dynamicLibrary_t FindLibrary(const char *path) = 0;
    virtual ExportedComponent_t FindComponent(const char *pszName) = 0;
    virtual ExportedComponent_t ExportComponent(dynamicLibrary_t hModule, const char *interface) = 0;

    virtual void CallMessageBox(const char *szTitle, const char *szMessage) = 0;

    virtual bool ConsoleInit(const char *szTitle, bool useNewThread, bool useXYpos = false, int xPos = 0,
                             int yPos = 0) = 0;
    virtual void ConsoleDestroy() = 0;
    virtual void ConsoleShow(int visLevel, bool quitOnClose) = 0;
    virtual void ConsoleSetErrorText(const char *buf) = 0;
    virtual void ConsoleSetStatus(const char *pszFormat, ...) = 0;
    virtual void ConsoleAppendText(const char *msg) = 0;
    virtual void ConsoleHandleEvents() = 0;
    virtual void ConsoleWaitForQuit() = 0;

    virtual uintptr_t GetCurrentThreadID() = 0;

    virtual uintptr_t CreateThread(xthread_t function, void *parms, xthreadPriority priority, const char *pszName,
                                   core_t core, int stackSize = DEFAULT_THREAD_STACK_SIZE, bool suspended = false) = 0;

    virtual void DestroyThread(uintptr_t threadHandle) = 0;

    virtual void SignalCreate(signalHandle_t &handle, bool manualReset) = 0;
    virtual void SignalDestroy(signalHandle_t &handle) = 0;
    virtual void SignalRaise(signalHandle_t &handle) = 0;
    virtual void SignalClear(signalHandle_t &handle) = 0;
    virtual bool SignalWait(signalHandle_t &handle, int iTimeout) = 0;

    virtual void MutexCreate(mutexHandle_t &handle) = 0;
    virtual void MutexDestroy(mutexHandle_t &handle) = 0;
    virtual bool MutexLock(mutexHandle_t &handle, bool blocking) = 0;
    virtual void MutexUnlock(mutexHandle_t &handle) = 0;

    virtual interlockedInt_t InterlockedIncrement(interlockedInt_t &value) = 0;
    virtual interlockedInt_t InterlockedDecrement(interlockedInt_t &value) = 0;

    virtual interlockedInt_t InterlockedAdd(interlockedInt_t &value, interlockedInt_t i) = 0;
    virtual interlockedInt_t InterlockedSub(interlockedInt_t &value, interlockedInt_t i) = 0;

    virtual interlockedInt_t InterlockedExchange(interlockedInt_t &value, interlockedInt_t exchange) = 0;
    virtual interlockedInt_t InterlockedCompareExchange(interlockedInt_t &value, interlockedInt_t comparand,
                                                        interlockedInt_t exchange) = 0;

    virtual void *InterlockedExchangePointer(void *&ptr, void *exchange) = 0;
    virtual void *InterlockedCompareExchangePointer(void *&ptr, void *comparand, void *exchange) = 0;

    virtual void Yield() = 0;
};

extern IPlatform *platform();

#endif