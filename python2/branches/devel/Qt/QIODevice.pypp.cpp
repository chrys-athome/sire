// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include <QString>
#include <QByteArray>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QTextStream>
#include <QDateTime>
#include <QLocale>
#include <QUuid>
#include <qnamespace.h>
#include <QVariant>
#include <QUrl>
#include <QBitArray>
#include "QIODevice.pypp.hpp"

namespace bp = boost::python;

void register_QIODevice_class(){

    { //::QIODevice
        typedef bp::class_< QIODevice, boost::noncopyable > QIODevice_exposer_t;
        QIODevice_exposer_t QIODevice_exposer = QIODevice_exposer_t( "QIODevice", bp::no_init );
        bp::scope QIODevice_scope( QIODevice_exposer );
        bp::enum_< QIODevice::OpenModeFlag>("OpenModeFlag")
            .value("NotOpen", QIODevice::NotOpen)
            .value("ReadOnly", QIODevice::ReadOnly)
            .value("WriteOnly", QIODevice::WriteOnly)
            .value("ReadWrite", QIODevice::ReadWrite)
            .value("Append", QIODevice::Append)
            .value("Truncate", QIODevice::Truncate)
            .value("Text", QIODevice::Text)
            .value("Unbuffered", QIODevice::Unbuffered)
            .export_values()
            ;
        { //::QIODevice::atEnd
        
            typedef bool ( ::QIODevice::*atEnd_function_type )(  ) const;
            atEnd_function_type atEnd_function_value( &::QIODevice::atEnd );
            
            QIODevice_exposer.def( 
                "atEnd"
                , atEnd_function_value );
        
        }
        { //::QIODevice::bytesAvailable
        
            typedef ::qint64 ( ::QIODevice::*bytesAvailable_function_type )(  ) const;
            bytesAvailable_function_type bytesAvailable_function_value( &::QIODevice::bytesAvailable );
            
            QIODevice_exposer.def( 
                "bytesAvailable"
                , bytesAvailable_function_value );
        
        }
        { //::QIODevice::bytesToWrite
        
            typedef ::qint64 ( ::QIODevice::*bytesToWrite_function_type )(  ) const;
            bytesToWrite_function_type bytesToWrite_function_value( &::QIODevice::bytesToWrite );
            
            QIODevice_exposer.def( 
                "bytesToWrite"
                , bytesToWrite_function_value );
        
        }
        { //::QIODevice::canReadLine
        
            typedef bool ( ::QIODevice::*canReadLine_function_type )(  ) const;
            canReadLine_function_type canReadLine_function_value( &::QIODevice::canReadLine );
            
            QIODevice_exposer.def( 
                "canReadLine"
                , canReadLine_function_value );
        
        }
        { //::QIODevice::close
        
            typedef void ( ::QIODevice::*close_function_type )(  ) ;
            close_function_type close_function_value( &::QIODevice::close );
            
            QIODevice_exposer.def( 
                "close"
                , close_function_value );
        
        }
        { //::QIODevice::errorString
        
            typedef ::QString ( ::QIODevice::*errorString_function_type )(  ) const;
            errorString_function_type errorString_function_value( &::QIODevice::errorString );
            
            QIODevice_exposer.def( 
                "errorString"
                , errorString_function_value );
        
        }
        { //::QIODevice::getChar
        
            typedef bool ( ::QIODevice::*getChar_function_type )( char * ) ;
            getChar_function_type getChar_function_value( &::QIODevice::getChar );
            
            QIODevice_exposer.def( 
                "getChar"
                , getChar_function_value
                , ( bp::arg("c") ) );
        
        }
        { //::QIODevice::isOpen
        
            typedef bool ( ::QIODevice::*isOpen_function_type )(  ) const;
            isOpen_function_type isOpen_function_value( &::QIODevice::isOpen );
            
            QIODevice_exposer.def( 
                "isOpen"
                , isOpen_function_value );
        
        }
        { //::QIODevice::isReadable
        
            typedef bool ( ::QIODevice::*isReadable_function_type )(  ) const;
            isReadable_function_type isReadable_function_value( &::QIODevice::isReadable );
            
            QIODevice_exposer.def( 
                "isReadable"
                , isReadable_function_value );
        
        }
        { //::QIODevice::isSequential
        
            typedef bool ( ::QIODevice::*isSequential_function_type )(  ) const;
            isSequential_function_type isSequential_function_value( &::QIODevice::isSequential );
            
            QIODevice_exposer.def( 
                "isSequential"
                , isSequential_function_value );
        
        }
        { //::QIODevice::isTextModeEnabled
        
            typedef bool ( ::QIODevice::*isTextModeEnabled_function_type )(  ) const;
            isTextModeEnabled_function_type isTextModeEnabled_function_value( &::QIODevice::isTextModeEnabled );
            
            QIODevice_exposer.def( 
                "isTextModeEnabled"
                , isTextModeEnabled_function_value );
        
        }
        { //::QIODevice::isWritable
        
            typedef bool ( ::QIODevice::*isWritable_function_type )(  ) const;
            isWritable_function_type isWritable_function_value( &::QIODevice::isWritable );
            
            QIODevice_exposer.def( 
                "isWritable"
                , isWritable_function_value );
        
        }
        { //::QIODevice::open
        
            typedef bool ( ::QIODevice::*open_function_type )( ::QFlags< QIODevice::OpenModeFlag > ) ;
            open_function_type open_function_value( &::QIODevice::open );
            
            QIODevice_exposer.def( 
                "open"
                , open_function_value
                , ( bp::arg("mode") ) );
        
        }
        { //::QIODevice::openMode
        
            typedef ::QFlags< QIODevice::OpenModeFlag > ( ::QIODevice::*openMode_function_type )(  ) const;
            openMode_function_type openMode_function_value( &::QIODevice::openMode );
            
            QIODevice_exposer.def( 
                "openMode"
                , openMode_function_value );
        
        }
        { //::QIODevice::peek
        
            typedef ::qint64 ( ::QIODevice::*peek_function_type )( char *,::qint64 ) ;
            peek_function_type peek_function_value( &::QIODevice::peek );
            
            QIODevice_exposer.def( 
                "peek"
                , peek_function_value
                , ( bp::arg("data"), bp::arg("maxlen") ) );
        
        }
        { //::QIODevice::peek
        
            typedef ::QByteArray ( ::QIODevice::*peek_function_type )( ::qint64 ) ;
            peek_function_type peek_function_value( &::QIODevice::peek );
            
            QIODevice_exposer.def( 
                "peek"
                , peek_function_value
                , ( bp::arg("maxlen") ) );
        
        }
        { //::QIODevice::pos
        
            typedef ::qint64 ( ::QIODevice::*pos_function_type )(  ) const;
            pos_function_type pos_function_value( &::QIODevice::pos );
            
            QIODevice_exposer.def( 
                "pos"
                , pos_function_value );
        
        }
        { //::QIODevice::putChar
        
            typedef bool ( ::QIODevice::*putChar_function_type )( char ) ;
            putChar_function_type putChar_function_value( &::QIODevice::putChar );
            
            QIODevice_exposer.def( 
                "putChar"
                , putChar_function_value
                , ( bp::arg("c") ) );
        
        }
        { //::QIODevice::read
        
            typedef ::qint64 ( ::QIODevice::*read_function_type )( char *,::qint64 ) ;
            read_function_type read_function_value( &::QIODevice::read );
            
            QIODevice_exposer.def( 
                "read"
                , read_function_value
                , ( bp::arg("data"), bp::arg("maxlen") ) );
        
        }
        { //::QIODevice::read
        
            typedef ::QByteArray ( ::QIODevice::*read_function_type )( ::qint64 ) ;
            read_function_type read_function_value( &::QIODevice::read );
            
            QIODevice_exposer.def( 
                "read"
                , read_function_value
                , ( bp::arg("maxlen") ) );
        
        }
        { //::QIODevice::readAll
        
            typedef ::QByteArray ( ::QIODevice::*readAll_function_type )(  ) ;
            readAll_function_type readAll_function_value( &::QIODevice::readAll );
            
            QIODevice_exposer.def( 
                "readAll"
                , readAll_function_value );
        
        }
        { //::QIODevice::readLine
        
            typedef ::qint64 ( ::QIODevice::*readLine_function_type )( char *,::qint64 ) ;
            readLine_function_type readLine_function_value( &::QIODevice::readLine );
            
            QIODevice_exposer.def( 
                "readLine"
                , readLine_function_value
                , ( bp::arg("data"), bp::arg("maxlen") ) );
        
        }
        { //::QIODevice::readLine
        
            typedef ::QByteArray ( ::QIODevice::*readLine_function_type )( ::qint64 ) ;
            readLine_function_type readLine_function_value( &::QIODevice::readLine );
            
            QIODevice_exposer.def( 
                "readLine"
                , readLine_function_value
                , ( bp::arg("maxlen")=(long int)(0) ) );
        
        }
        { //::QIODevice::reset
        
            typedef bool ( ::QIODevice::*reset_function_type )(  ) ;
            reset_function_type reset_function_value( &::QIODevice::reset );
            
            QIODevice_exposer.def( 
                "reset"
                , reset_function_value );
        
        }
        { //::QIODevice::seek
        
            typedef bool ( ::QIODevice::*seek_function_type )( ::qint64 ) ;
            seek_function_type seek_function_value( &::QIODevice::seek );
            
            QIODevice_exposer.def( 
                "seek"
                , seek_function_value
                , ( bp::arg("pos") ) );
        
        }
        { //::QIODevice::setTextModeEnabled
        
            typedef void ( ::QIODevice::*setTextModeEnabled_function_type )( bool ) ;
            setTextModeEnabled_function_type setTextModeEnabled_function_value( &::QIODevice::setTextModeEnabled );
            
            QIODevice_exposer.def( 
                "setTextModeEnabled"
                , setTextModeEnabled_function_value
                , ( bp::arg("enabled") ) );
        
        }
        { //::QIODevice::size
        
            typedef ::qint64 ( ::QIODevice::*size_function_type )(  ) const;
            size_function_type size_function_value( &::QIODevice::size );
            
            QIODevice_exposer.def( 
                "size"
                , size_function_value );
        
        }
        { //::QIODevice::ungetChar
        
            typedef void ( ::QIODevice::*ungetChar_function_type )( char ) ;
            ungetChar_function_type ungetChar_function_value( &::QIODevice::ungetChar );
            
            QIODevice_exposer.def( 
                "ungetChar"
                , ungetChar_function_value
                , ( bp::arg("c") ) );
        
        }
        { //::QIODevice::waitForBytesWritten
        
            typedef bool ( ::QIODevice::*waitForBytesWritten_function_type )( int ) ;
            waitForBytesWritten_function_type waitForBytesWritten_function_value( &::QIODevice::waitForBytesWritten );
            
            QIODevice_exposer.def( 
                "waitForBytesWritten"
                , waitForBytesWritten_function_value
                , ( bp::arg("msecs") ) );
        
        }
        { //::QIODevice::waitForReadyRead
        
            typedef bool ( ::QIODevice::*waitForReadyRead_function_type )( int ) ;
            waitForReadyRead_function_type waitForReadyRead_function_value( &::QIODevice::waitForReadyRead );
            
            QIODevice_exposer.def( 
                "waitForReadyRead"
                , waitForReadyRead_function_value
                , ( bp::arg("msecs") ) );
        
        }
        { //::QIODevice::write
        
            typedef ::qint64 ( ::QIODevice::*write_function_type )( char const *,::qint64 ) ;
            write_function_type write_function_value( &::QIODevice::write );
            
            QIODevice_exposer.def( 
                "write"
                , write_function_value
                , ( bp::arg("data"), bp::arg("len") ) );
        
        }
        { //::QIODevice::write
        
            typedef ::qint64 ( ::QIODevice::*write_function_type )( ::QByteArray const & ) ;
            write_function_type write_function_value( &::QIODevice::write );
            
            QIODevice_exposer.def( 
                "write"
                , write_function_value
                , ( bp::arg("data") ) );
        
        }
    }

}
