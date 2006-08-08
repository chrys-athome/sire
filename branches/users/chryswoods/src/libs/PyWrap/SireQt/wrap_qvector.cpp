    
void export_QVector_QString();
void export_QVector_real();
    
void export_QVector_int8();
void export_QVector_int16();
void export_QVector_int32();
void export_QVector_int64();
    
void export_QVector()
{
    export_QVector_QString();
    export_QVector_real();

    export_QVector_int8();
    export_QVector_int16();
    export_QVector_int32();
    export_QVector_int64();
}
