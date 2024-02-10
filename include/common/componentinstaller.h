/*
  This file associated with Hayato Labs project.

  For license and copyright information please follow this link:
  https://github.com/hayatolabs/general/blob/main/LEGAL
*/

#ifndef HAYATOLABS_COMPONENTINSTALLER_H
#define HAYATOLABS_COMPONENTINSTALLER_H

#include "public.h"

struct component_decl_t {
  const char*     m_pszModule;
  const char*     m_pszComponent;
  IComponent**    m_lpComponent;
};

#endif /* HAYATOLABS_COMPONENTINSTALLER_H */
