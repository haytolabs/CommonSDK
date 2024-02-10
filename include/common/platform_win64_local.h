/*
  This file associated with Hayato Labs project.

  For license and copyright information please follow this link:
  https://github.com/hayatolabs/general/blob/main/LEGAL
*/

#ifndef __PLATFORMWINDOWSLOCAL__H__
#define __PLATFORMWINDOWSLOCAL__H__

#include "internal.h"
#include "platform.h"
#include <timeapi.h>

class CPlatformWin64 : public IPlatform
{
  public:
    void RestoreTimerResolution();
    void MakeHighTimerResolution();
    void MakeHighProcessPriority();
    void SetUnhandledExceptionFilter(LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter);
    const char *GetExceptionName(DWORD code);
    int GetCurrentTime();

    FARPROC GetProcAddress(dynamicLibrary_t hModule, const char *procName);

    dynamicLibrary_t LoadLibraryFromDirectory(std::string path);
    dynamicLibrary_t LoadLibrary(const char *path);
    void UnloadLibrary(dynamicLibrary_t hModule);
    void UnloadAllLibraries();
    dynamicLibrary_t FindLibrary(const char *path);
    ExportedComponent_t FindComponent(const char *interface);
    ExportedComponent_t ExportComponent(dynamicLibrary_t hModule, const char *interface);

    void CallMessageBox(const char *szTitle, const char *szMessage);

    bool ConsoleInit(const char *szTitle, bool useNewThread, bool useXYpos = false, int xPos = 0, int yPos = 0);
    void ConsoleDestroy();
    void ConsoleShow(int visLevel, bool quitOnClose);
    void ConsoleSetErrorText(const char *buf);
    void ConsoleSetStatus(const char *pszFormat, ...);
    void ConsoleAppendText(const char *msg);
    void ConsoleHandleEvents();
    void ConsoleWaitForQuit();

    uintptr_t GetCurrentThreadID();

    uintptr_t CreateThread(xthread_t function, void *parms, xthreadPriority priority, const char *pszName, core_t core,
                           int stackSize = DEFAULT_THREAD_STACK_SIZE, bool suspended = false);

    void DestroyThread(uintptr_t threadHandle);

    void SignalCreate(signalHandle_t &handle, bool manualReset);
    void SignalDestroy(signalHandle_t &handle);
    void SignalRaise(signalHandle_t &handle);
    void SignalClear(signalHandle_t &handle);
    bool SignalWait(signalHandle_t &handle, int iTimeout);

    void MutexCreate(mutexHandle_t &handle);
    void MutexDestroy(mutexHandle_t &handle);
    bool MutexLock(mutexHandle_t &handle, bool blocking);
    void MutexUnlock(mutexHandle_t &handle);

    interlockedInt_t InterlockedIncrement(interlockedInt_t &value);
    interlockedInt_t InterlockedDecrement(interlockedInt_t &value);

    interlockedInt_t InterlockedAdd(interlockedInt_t &value, interlockedInt_t i);
    interlockedInt_t InterlockedSub(interlockedInt_t &value, interlockedInt_t i);

    interlockedInt_t InterlockedExchange(interlockedInt_t &value, interlockedInt_t exchange);
    interlockedInt_t InterlockedCompareExchange(interlockedInt_t &value, interlockedInt_t comparand,
                                                interlockedInt_t exchange);

    void *InterlockedExchangePointer(void *&ptr, void *exchange);
    void *InterlockedCompareExchangePointer(void *&ptr, void *comparand, void *exchange);

    void Yield();
};

#endif //!__PLATFORMWINDOWSLOCAL__H__