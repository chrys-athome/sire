
#include <Python.h>
#include <boost/python.hpp>

#include <QString>

#include "SireQt/qhash.hpp"

using namespace SireQt;
    
void  
SIREQT_EXPORT
export_QHash_QString_QString()
{                            
    wrap_QHash<QString,QString>("QHash_QString_QString_");
}
