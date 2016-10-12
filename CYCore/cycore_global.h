/****************************************************************************
**
** 
** 
** 
****************************************************************************/

#pragma once

#include <qglobal.h>

#if defined(CYCORE_LIBRARY)
#  define CYCORE_EXPORT Q_DECL_EXPORT
#else
#  define CYCORE_EXPORT Q_DECL_IMPORT
#endif
