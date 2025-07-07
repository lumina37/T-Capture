#pragma once

#ifdef _TCAP_LIB_HEADER_ONLY
#    define TCAP_API
#else
#    ifdef _TCAP_LIB_DYNAMIC
#        ifdef _MSC_VER
#            ifdef tcap_lib_dynamic_EXPORTS
#                define TCAP_API __declspec(dllexport)
#            else
#                define TCAP_API __declspec(dllimport)
#            endif
#        else
#            ifdef tcap_lib_dynamic_EXPORTS
#                define TCAP_API __attribute__((visibility("default")))
#            else
#                define TCAP_API
#            endif
#        endif
#    else
#        define TCAP_API
#    endif
#endif

#ifdef _WIN32
#    define TCAP_ENABLE_MF 1
#else
#    define TCAP_ENABLE_MF 0
#endif

#ifdef __linux__
#    define TCAP_ENABLE_V4L2 1
#    define TCAP_ENABLE_X11 1
#    define TCAP_ENABLE_PW 1
#else
#    define TCAP_ENABLE_V4L2 0
#    define TCAP_ENABLE_X11 0
#    define TCAP_ENABLE_PW 0
#endif
