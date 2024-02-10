/*
  This file associated with Hayato Labs project.

  For license and copyright information please follow this link:
  https://github.com/hayatolabs/general/blob/main/LEGAL
*/

#ifndef NFCXX_DEBUGTOOLS_HPP
#define NFCXX_DEBUGTOOLS_HPP

/*
=====================

=====================
*/
typedef void (*Callback_assertionReportFn)(const char* szMessage,
  const char* pszFilename,
  unsigned int iLine);

/*
=====================

=====================
*/
extern Callback_assertionReportFn g_pCallback_assertionReportFn;

/*
=====================

=====================
*/
extern Callback_assertionReportFn g_pCallback_assertionNonFatalReportFn;

/*
=====================

=====================
*/
#define CALLBACK_ASSERTION(pszName)                                                       \
   void __##pszName##(const char* szMessage, const char* pszFilename, unsigned int iLine); \
   class __##pszName##__CallBack_assertion                                                \
   {                                                                                   \
   public:                                                                             \
      __##pszName##__CallBack_assertion()                                                 \
      {                                                                                \
         g_pCallback_assertionReportFn = __##pszName##;                                   \
      }                                                                                \
   };                                                                                  \
   __##pszName##__CallBack_assertion __##pszName##__CallBack_assertionInstance;              \
   void __##pszName##(const char* szMessage, const char* pszFilename, unsigned int iLine)

bool NFCCassertionReport(const char* file, int iLine, const char* expression);


/*
================================================================================================

Getting assert() to work as we want on all platforms and code analysis tools
can be tricky.

================================================================================================
*/

bool NFCCassertionReport(const char* file, int iLine, const char* expression);

// tell PC-Lint that assert failed won't return, which means it can assume the
// conditions are true for subsequent analysis.
// lint -function( exit, AssertFailed )

//====================== assert in debug mode =======================
#if defined(_DEBUG) || defined(_lint)

#undef assert

// idassert is useful for cases where some external hModule (think MFC, etc.)
// decides it's a good idea to redefine assert on us
#define dpassert(x) (void)((!!(x)) || (NFCCassertionReport (__FILE__, __LINE__, #x)))

// We have the code analysis tools on the 360 compiler,
// so let it know what our asserts are.
// The VS ultimate editions also get it on win32, but not x86

// RB: __analysis_assume only necessary with MSVC
#if defined(_MSC_VER)
#define assert(x)                                                             \
  __analysis_assume (x);                                                      \
  dpassert (x)
#else
#define assert(x) dpassert (x)
#endif
// RB end

#define verify(x) ((x) ? true : (NFCCassertionReport (__FILE__, __LINE__, #x), false))

#else // _DEBUG

//====================== assert in release mode =======================

#define dpassert(x)                                                           \
  {                                                                           \
    ((void)0);                                                                \
  }

#undef assert

#define assert(x) dpassert (x)

#define verify(x) ((x) ? true : false)

#endif // _DEBUG

//=====================================================================

#define dpreleaseassert(x)                                                    \
  (void)((!!(x)) || (AssertFailed (__FILE__, __LINE__, #x)));

#define release_assert(x) dpreleaseassert (x)

// RB: bChanged UINT_PTR to uintptr_t
#define assert_2_byte_aligned(ptr) assert ((((uintptr_t)(ptr)) & 1) == 0)
#define assert_4_byte_aligned(ptr) assert ((((uintptr_t)(ptr)) & 3) == 0)
#define assert_8_byte_aligned(ptr) assert ((((uintptr_t)(ptr)) & 7) == 0)
#define assert_16_byte_aligned(ptr) assert ((((uintptr_t)(ptr)) & 15) == 0)
#define assert_32_byte_aligned(ptr) assert ((((uintptr_t)(ptr)) & 31) == 0)
#define assert_64_byte_aligned(ptr) assert ((((uintptr_t)(ptr)) & 63) == 0)
#define assert_128_byte_aligned(ptr) assert ((((uintptr_t)(ptr)) & 127) == 0)
#define assert_aligned_to_type_size(ptr)                                      \
  assert ((((uintptr_t)(ptr)) & (sizeof ((ptr)[0]) - 1)) == 0)
// RB end

#if !defined(__TYPEINFOGEN__)                                                 \
    && !defined(_lint) // pcLint has problems with assert_offsetof()

#if __cplusplus >= 201103L
#define compile_time_assert(x) static_assert (x, "Assertion failure")
#else
template <bool> struct compile_time_assert_failed;
template <> struct compile_time_assert_failed<true> {
};
template <int x> struct compile_time_assert_test {
};
#define compile_time_assert_join2(a, b) a##b
#define compile_time_assert_join(a, b) compile_time_assert_join2 (a, b)
#define compile_time_assert(x)                                                \
  typedef compile_time_assert_test<sizeof (                                   \
      compile_time_assert_failed<(bool)(x)>)>                                 \
  compile_time_assert_join (compile_time_assert_typedef_, __LINE__)
#endif

#define assert_sizeof(type, nSize) compile_time_assert (sizeof (type) == nSize)
#define assert_sizeof_8_byte_multiple(type)                                   \
  compile_time_assert ((sizeof (type) & 7) == 0)
#define assert_sizeof_16_byte_multiple(type)                                  \
  compile_time_assert ((sizeof (type) & 15) == 0)
#define assert_offsetof(type, field, offset)                                  \
  compile_time_assert (offsetof (type, field) == offset)
#define assert_offsetof_8_byte_multiple(type, field)                          \
  compile_time_assert ((offsetof (type, field) & 7) == 0)
#define assert_offsetof_16_byte_multiple(type, field)                         \
  compile_time_assert ((offsetof (type, field) & 15) == 0)

#else

#define compile_time_assert(x)
#define assert_sizeof(type, nSize)
#define assert_sizeof_8_byte_multiple(type)
#define assert_sizeof_16_byte_multiple(type)
#define assert_offsetof(type, field, offset)
#define assert_offsetof_8_byte_multiple(type, field)
#define assert_offsetof_16_byte_multiple(type, field)

#endif

// useful for verifying that an array of items has the same number of elements
// in it as an enum type
#define verify_array_size(_array_name_, _max_enum_)                           \
  compile_time_assert (sizeof (_array_name_)                                  \
                       == (_max_enum_) * sizeof (_array_name_[0]))


#endif