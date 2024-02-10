/*
  This file associated with Hayato Labs project.

  For license and copyright information please follow this link:
  https://github.com/hayatolabs/general/blob/main/LEGAL
*/

#ifndef HAYATOLABS_MATH_H
#define HAYATOLABS_MATH_H

#include "internal.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846f // matches value in gcc v2 math.h
#endif

#ifndef M_LN2
#define M_LN2 0.693147180559945309417f
#endif

typedef union floatint_u {
    int32_t i;
    uint32_t u;
    float f;
    ubyte b[4];
} floatint_t;

using angle_t = float;
using vec3_t = float[3];
using vec4_t = float[4];

/*
===========================================================================

   vec2_t

===========================================================================
*/
class vec2_t
{
  private:
    float m_Value[2];

  public:
    vec2_t() = default;
    vec2_t(const vec2_t &vec);
    explicit vec2_t(float x, float y);

    float x() const
    {
        return m_Value[0];
    }
    float y() const
    {
        return m_Value[1];
    }

    float sqrAbs() const;
    float abs() const;

    DEPRECATED vec2_t Normalize2() const;
    vec2_t Normalize() const;
    float Distance(const vec2_t &vec) const;
    vec2_t Direction(const vec2_t &vec) const;
    float Dot(const vec2_t &vec) const;

    vec2_t operator-() const;
    bool operator==(const vec2_t &vec) const;
    bool operator!=(const vec2_t &vec) const;
    float &operator[](const int index);
    const float &operator[](const int index) const;
    vec2_t &operator=(const vec2_t &vec) = default;
    vec2_t operator+(const vec2_t &vec) const;
    vec2_t operator-(const vec2_t &vec) const;
    vec2_t operator-(const float &var) const;
    vec2_t operator*(float number) const;
    vec2_t operator*(const vec2_t &vec) const;
    vec2_t operator/(float number) const;
};

/*
===========================================================================

   bounds2d_t

===========================================================================
*/
class bounds2d_t
{
  private:
    vec2_t m_vecSize;                // размер хитбокса
    vec2_t m_vecCorrectBoundingSize; // ? repaired size of bounds
    vec2_t m_vecOrigin;              // центр хитбокса
    vec2_t m_vecUpperLeftCorner;     // верхний левый угол
    vec2_t m_vecUpperRightCorner;    // верхний правый угол
    vec2_t m_vecLowerLeftCorner;     // нижний правый угол
    vec2_t m_vecLowerRightCorner;    // нижий левый угол
    float m_fSizeSum;                // сумма хитбока

  public:
    bounds2d_t() = default;
    bounds2d_t(const vec2_t &vecOrigin, const vec2_t &vecSize);
    bounds2d_t(const vec2_t &vecUpperLeftCorner, const vec2_t &vecUpperRightCorner, const vec2_t &vecLowerLeftCorner,
               const vec2_t &vecLowerRightCorner);

    vec2_t Origin() const;
    vec2_t Size() const;
    vec2_t OriginOffsetSize() const;
    float Sum() const;

    vec2_t GetHitboxA() const;
    vec2_t GetHitboxB() const;
    vec2_t GetHitboxC() const;
    vec2_t GetHitboxD() const;

    void Resize(const vec2_t &vecSize);
    void Translate(const vec2_t &vecOrigin);
    void TranslateVertex(const vec2_t &vecOrigin);
    bool Intersects(const vec2_t &vecOrigin);
    bool Intersects(const bounds2d_t &bounds);
};

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif /* HAYATOLABS_MATH_H */

int max(int x, int y);
int min(int x, int y);

float Q_rsqrt(float number);
float Q_fabs(float f);

int Q_rand(int *seed);
float Q_random(int *seed);
float Q_crandom(int *seed);

signed char ClampChar(int i);
signed short ClampShort(int i);
void Clamp(int &iValue, int iMinumum, int iMaximum);
void ClampFloat(float &fValue, float fMinimum, float fMaximum);

int Q_isfinite(float f);
float Q_atof(const char *str);
float Q_log2f(float f);
float Q_exp2f(float f);
angle_t Q_acos(float c);

angle_t LerpAngle(angle_t from, angle_t to, angle_t frac);
angle_t AngleSubtract(angle_t angle1, angle_t angle2);
angle_t AngleMod(angle_t angle);
angle_t AngleNormalize360(angle_t angle);
angle_t AngleNormalize180(angle_t angle);
angle_t AngleDelta(angle_t angle1, angle_t angle2);

#endif //!__MATH__H__