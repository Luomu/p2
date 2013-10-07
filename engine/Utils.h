#pragma once
/*
 * Simple utility functions
 */

#ifndef __GNUC__
#define __attribute(x)
#endif /* __GNUC__ */

// GCC warns when a function marked __attribute((noreturn)) actually returns a value
// but other compilers which don't see the noreturn attribute of course require that
// a function with a non-void return type should return something.
#ifndef __GNUC__
#define RETURN_ZERO_NONGNU_ONLY return 0;
#else
#define RETURN_ZERO_NONGNU_ONLY
#endif

template<class T> inline const T& Clamp(const T& x, const T& min, const T& max) { return x > max ? max : (x < min ? min : x); }

inline bool starts_with(const char *s, const char *t) {
	assert(s && t);
	while ((*s == *t) && *t) { ++s; ++t; }
	return (*t == '\0');
}

inline bool starts_with(const std::string &s, const char *t) {
	assert(t);
	return starts_with(s.c_str(), t);
}

inline bool starts_with(const std::string &s, const std::string &t) {
	return starts_with(s.c_str(), t.c_str());
}

inline bool ends_with(const char *s, size_t ns, const char *t, size_t nt) {
	return (ns >= nt) && (memcmp(s+(ns-nt), t, nt) == 0);
}

inline bool ends_with(const char *s, const char *t) {
	return ends_with(s, strlen(s), t, strlen(t));
}

inline bool ends_with(const std::string &s, const char *t) {
	return ends_with(s.c_str(), s.size(), t, strlen(t));
}

inline bool ends_with(const std::string &s, const std::string &t) {
	return ends_with(s.c_str(), s.size(), t.c_str(), t.size());
}

// add a few things that MSVC is missing
#ifdef _MSC_VER

// round & roundf. taken from http://cgit.freedesktop.org/mesa/mesa/tree/src/gallium/auxiliary/util/u_math.h
static inline double round(double x)
{
   return x >= 0.0 ? floor(x + 0.5) : ceil(x - 0.5);
}

static inline float roundf(float x)
{
   return x >= 0.0f ? floorf(x + 0.5f) : ceilf(x - 0.5f);
}
#endif /* _MSC_VER */

// from StackOverflow: http://stackoverflow.com/a/1500517/52251
// Q: "Compile time sizeof_array without using a macro"
template <typename T, size_t N>
char ( &COUNTOF_Helper( T (&array)[N] ))[N];
#define COUNTOF( array ) (sizeof( COUNTOF_Helper( array ) ))

struct Exception {
	Exception(const std::string &text) : message(text) { }
	std::string message;
};
