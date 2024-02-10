/*
  This file associated with Hayato Labs project.

  For license and copyright information please follow this link:
  https://github.com/hayatolabs/general/blob/main/LEGAL
*/

#ifndef HAYATOLABS_COMMON_LOG_HPP
#define HAYATOLABS_COMMON_LOG_HPP

#include "public.h"

/*
=====================
уровни логгинга
=====================
*/
enum class ELogLevel_t
{
    k_eInfoLevel = 0,
    k_eWarningLevel,
    k_eErrorLevel,
    k_eDeveloper, // не выводится в релиз-моде, но можно включить параметром -dev
    k_eConsolePrint,
};

/*
===========================================================================

   IProgramLog

===========================================================================
*/
class IProgramLog
{
  public:
    virtual bool OpenLogFile(Path_t strFile) = 0;
    virtual void CloseLogFile() = 0;
    virtual void Print(ELogLevel_t iLevel, const char *szText, ...) = 0;
};
IProgramLog *GetDefaultLogger();

#endif
