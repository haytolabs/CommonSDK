/*
  This file associated with Hayato Labs project.

  For license and copyright information please follow this link:
  https://github.com/hayatolabs/general/blob/main/LEGAL
*/


#ifndef HAYATOLABS_SHARED_H
#define HAYATOLABS_SHARED_H
#pragma warning(default : 4820)

#include <csignal>

#include "../common/public.h"
#include "../common/dbgtools.h"
#include "../common/iprogramlog.h"
#include "../common/filesystem_public.h"
#include "../common/mem.h"
#include "../common/string.h"
#include "../common/platform.h"
#include "../common/convar.h"
#include "../common/icommandline.h"
#include "math.h"

/*
===========================================================================

   trace_t
   Содежрит в себе информацию о пройденном луче

===========================================================================
*/
struct trace_t {
   bool     collided = false;          // произошло ли столкновение
   vec2_t   collisionPoint{ 0, 0 };    // точка столкновения
   uint64   collidedEntityNumber = 0;  // номер игрового объекта, с которым произошла коллизия
};

// для хранения цвета
using rgba_t = uint8[4];

// установить параметры цвета
#define MakeColor(vec, r, g, b, a) \
   vec[0] = r;                     \
   vec[1] = g;                     \
   vec[2] = b;                     \
   vec[3] = a;

// быстрый обратный корень Джона Кармака из Quake 3
float Q_rsqrt(float number);

class CEntity;
bool COM_ValidateObject(CEntity* *object);

#ifdef string
#undef string
#endif

using string = std::string;

struct refdef_t {
   int   m_iWidth;
   int   m_iHeight;
   int   m_iScreenIdx;
   int   m_iFramerate;
   void* m_pRenderWnd;
   void* m_pRenderInstance;
   void* m_pImguiContext;
};

typedef float vec4_t[4];
extern vec4_t colourTable[64];
int ColourIndexFromChar(char ccode);

#define InvalidMedia 0     // Если не удалось загрузить текстуру
using MediaHandle = int32;

struct parms_t {
   bool Has(int flag) const { return m_iFlags & flag; }
   void Add(int flag) { if (!Has(flag)) { m_iFlags |= flag; } }
   void Remove(int flag) { if (Has(flag)) { m_iFlags ^= flag; } }
private:
   int m_iFlags = 0;
};

void AdjustFrom1600(vec2_t* vecOrigin, vec2_t* vecSize);
void AdjustFrom1920(vec2_t* vecOrigin, vec2_t* vecSize);

#endif /* HAYATOLABS_SHARED_H */
