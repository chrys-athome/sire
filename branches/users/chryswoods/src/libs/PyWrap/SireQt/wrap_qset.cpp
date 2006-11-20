    
#include "sireglobal.h"
    
void export_QSet_QString();
void export_QSet_real();
    
void export_QSet_int8();
void export_QSet_int16();
void export_QSet_int32();
void export_QSet_int64();
    
void  
SIREQT_EXPORT
export_QSet()
{
    export_QSet_QString();
    export_QSet_real();

    export_QSet_int8();
    export_QSet_int16();
    export_QSet_int32();
    export_QSet_int64();
}
