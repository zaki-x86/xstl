#ifndef _XSTL_CONFIG_H_

#define _XSTL_CONFIG_H_

// **************************************
/// Define internal and public components
// **************************************

#if !defined(XSTL_API) && !defined(XSTL_INTERNAL) && defined(_WIN32)
#define XSTL_API __declspec(dllexport)
#define XSTL_INTERNAL 
#endif // WIN32 || _WIN32

#if !defined(XSTL_API) && !defined(XSTL_INTERNAL) && !defined(_WIN32)
#define XSTL_API __attribute__((visibility("default")))
#define XSTL_INTERNAL __attribute__((visibility("hidden")))
#endif

// **************************************
/// Features Test
// **************************************

/// inline
/// see:

#if __cpp_inline_variables >= 201606L || __cplusplus >= 201703L
#define XSTL_FORCE_INLINE inline
#else
#define XSTL_FORCE_INLINE
#endif

/// constexpr
/// See: https://en.cppreference.com/w/cpp/language/constexpr

#if __cpp_constexpr >= 200704L
#define CONSTEXPR11 constexpr
#else
#define CONSTEXPR11
#endif

#if __cpp_constexpr >= 201304L
#define CONSTEXPR14 constexpr
#else
#define CONSTEXPR14
#endif

#if __cpp_constexpr >= 201603L
#define CONSTEXPR17 constexpr
#endif

#if __cpp_constexpr >= 201907L
#define CONSTEXPR20 constexpr
#else
#define CONSTEXPR20
#endif

#if __cpp_constexpr >= 202110L
#define CONSTEXPR23 constexpr
#else
#define CONSTEXPR23
#endif

/// Attributes
/// See: https://en.cppreference.com/w/cpp/feature_test#:~:text=of%20said%20features.-,Attributes,-__has_cpp_attribute(%20attribute

#if __has_cpp_attribute( deprecated ) >= 201309L
#define XSTL_DEPRECATED [[deprecated]]
#else
#define XSTL_DEPRECATED
#endif

#if __has_cpp_attribute( nodiscard ) >= 201603L
#define XSTL_NODISCARD [[nodiscard]]
#else
#define XSTL_NODISCARD
#endif

#if __has_cpp_attribute( noreturn ) >= 200809L
#define XSTL_NORETURN [[noreturn]]
#else
#define XSTL_NORETURN
#endif

#if __has_cpp_attribute( maybe_unused ) >= 201603L
#define XSTL_UNUSED [[maybe_unused]]
#else
#define XSTL_UNUSED
#endif

#if __has_cpp_attribute( assume ) >= 202207L
#define XSTL_ASSUME(exp) [[maybe_unused(exp)]]
#else
#define XSTL_ASSUME(exp)
#endif

#if __has_cpp_attribute( likely ) >= 201803L && __has_cpp_attribute( unlikely ) >= 201803L
#define XSTL_LIKELY [[likely]]
#define XSTL_UNLIKELY [[unlikely]]
#else
#define XSTL_LIKELY
#define XSTL_UNLIKELY
#endif

// **************************************
/// Namespace
// **************************************

#define _BEGIN_XSTL namespace xstl {
#define _END_XSTL }

#if __cplusplus >= 201703L
#define _BEGIN_XSTL_TEST namespace xstl::test {
#define _END_XSTL_TEST }
#else                                   
#define _BEGIN_XSTL_TEST                \
    namespace xstl {                    \
        namespace test {            

#define _END_XSTL_TEST  }}
#endif



#endif // !_XSTL_CONFIG_H
