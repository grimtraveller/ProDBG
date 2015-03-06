#ifndef _PRODBGAPI_MENU_H_
#define _PRODBGAPI_MENU_H_

#include "pd_keys.h"

#ifdef __cplusplus
extern "C" {
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct PDMenuItem
{
    const char* name;
    unsigned int id;
    int key;
    int macMod;
    int winMod;

} PDMenuItem;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct PDMenu
{
    const char* name;
    PDMenuItem* items;
} PDMenu;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus

}
#endif

#endif

