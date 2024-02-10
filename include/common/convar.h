/*
  This file associated with Hayato Labs project.

  For license and copyright information please follow this link:
  https://github.com/hayatolabs/general/blob/main/LEGAL
*/

#ifndef HAYATOLABS_CONVAR_HPP
#define HAYATOLABS_CONVAR_HPP

#include "public.h"

#define CVAR_TYPE_STRING (1 << 0)
#define CVAR_TYPE_INTEGER (1 << 1)

#define CVAR_ARCHIVE (1 << 2)
#define CVAR_PROTECTED (1 << 3)
#define CVAR_USERCREATED (1 << 4)
#define CVAR_DESCRIPTION (1 << 5)

/*
===========================================================================

   cvar

===========================================================================
*/
struct cvar
{
  public:
    bool m_bFree = true;
    char m_szName[128];
    char m_szValue[128];
    char m_szDescription[128];
    int m_iBitflags = CVAR_TYPE_STRING;
    int m_iValue = 0;
    int m_iMinimum = 0;
    int m_iMaximum = 1;
    cvar *m_pLink = NULL;
    cvar *m_pOwner = NULL;

  public:
    void SetValue(const char *pszValue);
    void SetRules(int iMinimum, int iMaximum);
    void SetDescription(const char *pszDescription);

    const char *ToString();
    int ToInteger();

    cvar() = default;
    cvar(const char *pszName, const char *pszValue, const char *pszDescription, int iBits = CVAR_TYPE_STRING,
         int iMinimum = 0, int iMaximum = 1);
    cvar(const char *pszName, const char *pszValue, int iBits = CVAR_TYPE_STRING, int iMinimum = 0, int iMaximum = 1);
};

typedef void (*command_callback)(std::vector<std::string> command_line);

/*
===========================================================================

   command

===========================================================================
*/
struct command_t
{
    bool m_bFree = true;
    char m_szName[128];
    command_callback m_Callback;
    command_t *m_pLink = NULL;
    command_t *m_pOwner = NULL;

    command_t() = default;
    command_t(const char *pszCommandName, command_callback);
};

struct reginfo_t
{
    char m_szName[128];
};

/*
===========================================================================

   IConvarSystem

===========================================================================
*/
class IConvarSystem
{
  public:
    virtual cvar *RegisterConvar(const char *pszName) = 0;
    virtual cvar *SearchConvar(const char *pszName) = 0;
    virtual void SetConvarValue(const char *pszName, const char *szValue) = 0;
    virtual void FreeConvarHandles() = 0;
    virtual void SaveConfig(Path_t strPath) = 0;
    virtual void LoadConfig(Path_t strPath) = 0;
    virtual std::vector<reginfo_t> GetConvarList() = 0;

    virtual command_t *RegisterCommand(const char *pszName, command_callback) = 0;
    virtual command_t *FindCommand(const char *pszName) = 0;
    virtual std::vector<reginfo_t> GetCommandNames() = 0;
};

extern IConvarSystem *convar();

#endif /* HAYATOLABS_CONVAR_HPP */
