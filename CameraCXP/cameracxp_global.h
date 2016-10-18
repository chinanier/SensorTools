#ifndef CAMERACXP_GLOBAL_H
#define CAMERACXP_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef CAMERACXP_LIB
# define CAMERACXP_EXPORT Q_DECL_EXPORT
#else
# define CAMERACXP_EXPORT Q_DECL_IMPORT
#endif

#endif // CAMERACXP_GLOBAL_H
