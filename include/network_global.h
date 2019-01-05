#ifndef NETWORK_GLOBAL_H
#define NETWORK_GLOBAL_H

#include <QtCore/qglobal.h>

#ifndef QMT_NETWORK_STATIC

#ifdef QMT_NETWORK_LIB
# define NETWORK_EXPORT Q_DECL_EXPORT
#else
# define NETWORK_EXPORT Q_DECL_IMPORT
#endif

#else

# define NETWORK_EXPORT

#endif

#endif // NETWORK_GLOBAL_H
