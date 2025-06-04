#pragma once

#ifdef _TCAP_LIB_HEADER_ONLY
#    define TCAP_API
#else
#    ifdef _TCAP_LIB_DYNAMIC
#        ifdef _MSC_VER
#            ifdef tlct_lib_dynamic_EXPORTS
#                define TCAP_API __declspec(dllexport)
#            else
#                define TCAP_API __declspec(dllimport)
#            endif
#        else
#            ifdef tlct_lib_dynamic_EXPORTS
#                define TCAP_API __attribute__((visibility("default")))
#            else
#                define TCAP_API
#            endif
#        endif
#    else
#        define TCAP_API
#    endif
#endif
