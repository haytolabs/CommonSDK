/*
  This file associated with Hayato Labs project.

  For license and copyright information please follow this link:
  https://github.com/hayatolabs/general/blob/main/LEGAL
*/

#ifndef NFCXX_COMMON_HPP
#define NFCXX_COMMON_HPP

// #undef UNICODE
#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>							// no malloc.h on mac or unix
#undef FindText								// fix namespace pollution

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <ctype.h>
#include <typeinfo>
#include <errno.h>
#include <math.h>
#include <limits.h>
#include <memory>
#include <stdint.h>
#include <stddef.h>

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> //

#define ARRAY_LEN(x) (sizeof(x) / sizeof(*(x)))
#define STRARRAY_LEN(x) (ARRAY_LEN(x) - 1)

#define DEPRECATED __declspec(deprecated)

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> //
// Константы

const float PI = 3.14159265358979323846264338327950288f;
const float EPS = 0.000001f;

#define NAMESPACE_BEGIN(x) namespace x {
#define NAMESPACE_END }
#define NFCXX_BEGIN
#define NFCXX_END

#define nf_inline inline
#define nf_decl   __cdecl  

// angle indexes
#define	PITCH				0		// up / down
#define	YAW				1		// left / right
#define	ROLL				2		// fall over

// the game guarantees that no string from the network will ever
// exceed MAX_STRING_CHARS
#define	MAX_STRING_CHARS	1024
#define	MAX_STRING_TOKENS	1024
#define	MAX_TOKEN_CHARS	1024

#define	MAX_INFO_STRING	1024
#define	MAX_INFO_KEY		1024
#define	MAX_INFO_VALUE		1024

#define	BIG_INFO_STRING   8192
#define	BIG_INFO_KEY		8192
#define	BIG_INFO_VALUE		8192

#define	MAX_QPATH			64
#ifdef PATH_MAX
#define MAX_OSPATH			PATH_MAX
#else
#define	MAX_OSPATH			256
#endif

#define	MAX_NAME_LENGTH	32		// max nLength of a client pszName


// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> //
// Определения типов

typedef unsigned char		   ubyte;	// 8 bits
typedef unsigned short		   word;		// 16 bits
typedef unsigned int		      dword;	// 32 bits
typedef unsigned int		      uint;
// typedef unsigned long		ulong; // DG: long should be avoided.

typedef signed char			   int8;
typedef unsigned char		   uint8;
typedef short int			      int16;
typedef unsigned short int	   uint16;
typedef int					      int32;
typedef unsigned int		      uint32;
typedef long long			      int64;
typedef unsigned long long	   uint64;

#define MAX_TYPE( x )			( ( ( ( 1 << ( ( sizeof( x ) - 1 ) * 8 - 1 ) ) - 1 ) << 8 ) | 255 )
#define MIN_TYPE( x )			( - MAX_TYPE( x ) - 1 )
#define MAX_UNSIGNED_TYPE( x )	( ( ( ( 1U << ( ( sizeof( x ) - 1 ) * 8 ) ) - 1 ) << 8 ) | 255U )
#define MIN_UNSIGNED_TYPE( x )	0

template< typename _type_ >
bool IsSignedType(const _type_ t) {
   return _type_(-1) < 0;
}

template<class T> T	Max(T x, T y) {
   return (x > y) ? x : y;
}
template<class T> T	Min(T x, T y) {
   return (x < y) ? x : y;
}

#ifndef BIT
#define BIT( num )				( 1ULL << ( num ) )
#endif

#ifndef NUMBITS
#define NUMBITS( _type_ )		( sizeof( _type_ ) * 8 )
#endif

// filesystem.hpp
#define INVALID_HANDLE -1
using File_t = int16;
using Path_t = std::string;

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define EXPORT extern "C" __declspec(dllexport)
#define IMPORT extern "C" __declspec(dllimport)
#define DLL_EXT_STRING ".dll"
#else
#define EXPORT extern "C" __attribute__((visibility("default")))
#define IMPORT extern "C" __attribute__((visibility("default")))
#define DLL_EXT_STRING ".so"
#endif

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> //
// Работа с интерфейсами  

#define ExportFnName       ExportInterface_nfcxx2
#define ExportFnNameString "ExportInterface_nfcxx2"

class IComponent {};

// экспортированный интерфейс
using ExportedComponent_t = void*;

// прототип ф-ии для создания интерфейса
using ComponentCreationFn = std::function<ExportedComponent_t()>;
// указатель на библиотеку
using dynamicLibrary_t = void*;

class CComponentRegistration {
public:
   //
   ComponentCreationFn m_pfnCreateInstance;
   //
   char pszName[128];
   //
   CComponentRegistration* m_pNext;
   CComponentRegistration(ComponentCreationFn pFn, const char* pszNametag);
};

#define DECLARE_IMPORT(m_pszClassname, inetfacename, exportname)               \
   static void *__##m_pszClassname##__CreateInstance()                         \
   {                                                                      \
      return static_cast<inetfacename *>(new m_pszClassname);                  \
   }                                                                      \
   static CComponentRegistration __##m_pszClassname##__AddImportableComponent( \
       __##m_pszClassname##__CreateInstance, exportname);

typedef ExportedComponent_t(*COM_SearchReferencedComponent_t)(const char* pszName);
extern COM_SearchReferencedComponent_t SearchExternalComponent;

#define IFACEBIT_UNREFERENCED (1 << 1)

void HL_ReferenceInterface(ExportedComponent_t pInterface, const char* sName, int iBitflags = 0);

ExportedComponent_t HL_SearchReferencedComponentInternal(const char* pszName);


// convar.cpp
void HL_RegisterThisModuleConvars();
void HL_DisableConvarProtection();
void HL_EnableConvarProtection();

// componentinstaller.cpp
struct component_decl_t;
void   HL_AppendComponents(component_decl_t* pList);
void   HL_InstallComponents();
/*
===========================================================================

   ICommon

===========================================================================
*/
class ICommon {
public:
   friend class   IProgramLog;

public:
   virtual void   Init() = 0;                               // Инициализация всех базовых интерфейсов библиотеки nfcxx
   virtual void   Shutdown() = 0;                           // Завершение работы библиотеки nfcxx
   virtual void   Frame() = 0;
   virtual bool   IsDeveloperMode() = 0;                    // Запещно ли приложение в режиме разработчика

   virtual void   Nuke() = 0;                               // Экстренное завершение работы приложения
   virtual void   FatalError(const char* pszText, ...) = 0;  // Фатальная ошибка, завершающая работу приложения

   virtual void   EnterErrorState() = 0;
   virtual bool   InErrorState() = 0;

   virtual void   InitApplicationLogger(IProgramLog* pProgramLog) = 0;
   virtual void   ShutdownApplicationLogger() = 0;
   virtual void   Print(const char* pszText, ...) = 0;
   virtual void   Warning(const char* pszText, ...) = 0;
   virtual void   Error(const char* pszText, ...) = 0;
   virtual void   PrintDev(const char* pszText, ...) = 0;

   virtual int    Msec() = 0;


   // Выводит в консоль цветной текст,
   // с использованием цветовых кодов
   // обозначенных в заголовке String.hpp
   //
   // Пример: common()->ansiPrint( S_COLOUR_RED "Error: " S_COLOUR_WHITE " szBuffer is NULL" )
   // Напечатает слово Error красным, все остальное будет белым
   virtual void   ansiPrint(const char* pszText) = 0;

   virtual void   WaitForDebugger() = 0;
   virtual bool   IsDebuggerPresent() = 0;
   virtual void   BreakIfDebug() = 0;
};

extern ICommon* common();

#endif