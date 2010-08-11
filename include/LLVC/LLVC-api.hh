/*
 *  Copyright
 */

#ifndef LLVC_API_HH
#define LLVC_API_HH

#if defined (WIN32)
#  ifdef LLVC_EXPORTS
#    define LLVC_EXPORT __declspec(dllexport)
#  else
#    define LLVC_EXPORT __declspec(dllimport)
#  endif
#else
#  define LLVC_EXPORT
#endif

#endif
