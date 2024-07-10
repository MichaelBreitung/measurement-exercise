#ifndef LOGOUT_H
#define LOGOUT_H

#if !defined(NDEBUG) && defined(LOGGING)
#define LOUT(format, ...) printf(format, __VA_ARGS__)
#else
#define LOUT(format, ...) (void)0
#endif

#endif // LOGOUT_H