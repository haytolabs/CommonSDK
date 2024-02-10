/*
  This file associated with Hayato Labs project.

  For license and copyright information please follow this link:
  https://github.com/hayatolabs/general/blob/main/LEGAL
*/

#ifndef HAYATOLABS_COMMON_CMD_HPP
#define HAYATOLABS_COMMON_CMD_HPP

#include "public.h"

#ifdef commandline
#undef commandline
#endif

/*
===========================================================================

   ICommandLine
   Парсер командной строки

===========================================================================
*/
class ICommandLine {
public:
   virtual void         ParseFromArguments(int argc, char* *args) = 0;
   virtual bool         IsParsed() = 0;

   virtual void         AddParameter(const char* pszParameterName) = 0;
   virtual void         RemoveParameter(const char* pszParameterName) = 0;
   virtual int          FindParameter(const char* pszParameterName) = 0;
   virtual const char*  GetParameterByIndex(int index) = 0;
};

extern ICommandLine*   commandline();

#endif /* HAYATOLABS_COMMON_CMD_HPP */
