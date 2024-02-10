/*
  This file associated with Hayato Labs project.

  For license and copyright information please follow this link:
  https://github.com/hayatolabs/general/blob/main/LEGAL
*/

#ifndef __STRING__H__
#define __STRING__H__

#include "public.h"

#define NEWLINE "\n"

#define COLOUR_ESCAPE '^'
#define COLOUR_BLACK '0'
#define COLOUR_RED '1'
#define COLOUR_GREEN '2'
#define COLOUR_YELLOW '3'
#define COLOUR_BLUE '4'
#define COLOUR_CYAN '5'
#define COLOUR_MAGENTA '6'
#define COLOUR_WHITE '7'
#define ColourIndexForNumber(c) ((c)&0x07)
#define ColourIndex(c) (ColourIndexForNumber((c) - '0'))

#define COLOUR_BLACK_STR "^0"
#define COLOUR_RED_STR "^1"
#define COLOUR_GREEN_STR "^2"
#define COLOUR_YELLOW_STR "^3"
#define COLOUR_BLUE_STR "^4"
#define COLOUR_CYAN_STR "^5"
#define COLOUR_MAGENTA_STR "^6"
#define COLOUR_WHITE_STR "^7"

#ifdef copy
#undef copy
#endif //copy

/*
===========================================================================

   CStringTools

===========================================================================
*/
class CStringTools {
public:
    static int          vsnprintf(char* szBuffer, size_t nSize, const char* pszFormat, va_list ap);
    static const char*  sprintf(char* szBuffer, size_t nSize, const char* pszFormat, ...);

    static int          icmp(const char* s1, const char* s2);              // stricmp
    static int          ncmp(const char* s1, const char* s2, int n);       // strncmp
    static int          icmpn(const char* s1, const char* s2, int n);      // stricmpn
    static int          casecmp(const char* s1, const char* s2, int n);

    static const char*  lower(char* s1);        // Перевести строку в нижние буквы
    static const char*  upper(char* s1);        // Перевести строку в заглавные буквы

    static const char*  substr(const char* pszSource, const char* pszFind);         // Содержит ли строка подстроку

    static void         copy(char* szBuffer, const char* pszSource, size_t nSize);  // Скопировать строку
    static char*        add(char* szBuffer, const char* pszSource);                // Добавить текст к строке
    static size_t       nSize(const char* szBuffer);                             // Длина строки

    static size_t       countchar(const char* szBuffer, char ch);

    static bool         is_colour_string(const char* szBuffer);     // Содержит ли строка цветные символы?
    static const char*  remove_colours(char* szBuffer);             // Очищает от цветов строку
    static size_t       colour_string_size(const char* szBuffer);   // Длина строки без учета цветовых символов

    static char*        temp1024();                                 // Временный буффер размером 1024 символа
    static char*        va(const char* pszFormat, ...);

    static bool         hasprefix(const char* szBuffer, const char* szPrefix);
    static bool         hassuffix(const char* szBuffer, const char* szSuffix);

    static std::vector<std::string>
        tokenize(const char* szBuffer, char chToken);

    static std::string  utf16to8(const wchar_t *in);
    static std::string  utf16to8(const std::wstring &in);
    static std::wstring utf8to16(const char* in);
    static std::wstring utf8to16(const std::string &in);

};

using str = CStringTools;
using path = CStringTools;
#define va CStringTools::va



#endif  //!__STRING__H__