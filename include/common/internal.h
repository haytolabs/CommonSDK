/*
  This file associated with Hayato Labs project.

  For license and copyright information please follow this link:
  https://github.com/hayatolabs/general/blob/main/LEGAL
*/

#ifndef __NFCXX__H__
#define __NFCXX__H__

#include "public.h"
#include "iprogramlog.h"

class CCommon : public ICommon {
private:
   IProgramLog* pProgramLog = NULL;

public:
   void     Init();                                   // Инициализация всех базовых интерфейсов библиотеки nfcxx
   void     Shutdown();                               // Завершение работы библиотеки nfcxx
   void     Frame();
   bool     IsDeveloperMode();                        // Запущено ли приложение в режиме разработчика
   void     Nuke();                                   // Экстренное завершение работы приложения
   void     FatalError(const char* pszText, ...);     // Фатальная ошибка, завершающая работу приложения

   void     EnterErrorState();
   bool     InErrorState();

   void     InitApplicationLogger(IProgramLog* pProgramLog);
   void     ShutdownApplicationLogger();
   void     Print(const char* pszText, ...);
   void     Warning(const char* pszText, ...);
   void     Error(const char* pszText, ...);
   void     PrintDev(const char* pszText, ...);

   int      Msec();
   void     ansiPrint(const char* pszText);

   void     WaitForDebugger();
   bool     IsDebuggerPresent();
   void     BreakIfDebug();

};

class ICommandLine;
class IPlatform;
class IFileSystem;
class IConvarSystem;

extern ICommon*         g_pCommon;
extern ICommandLine*    g_pCommandLine;
extern IPlatform*       g_pPlatform;
extern IFileSystem*     g_pFilesystem;
extern IConvarSystem*   g_pConvarSystem;

#endif  //!__NFCXX__H__