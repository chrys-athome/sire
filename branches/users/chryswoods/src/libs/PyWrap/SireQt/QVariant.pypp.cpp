// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "QVariant.pypp.hpp"
#include "boost/python.hpp"
#include "sireqt_headers.h"

namespace bp = boost::python;

void register_QVariant_class(){

    { //::QVariant
        typedef bp::class_< QVariant > QVariant_exposer_t;
        QVariant_exposer_t QVariant_exposer = QVariant_exposer_t( "QVariant" );
        bp::scope QVariant_scope( QVariant_exposer );
        bp::enum_< QVariant::Type>("Type")
            .value("Invalid", QVariant::Invalid)
            .value("Bool", QVariant::Bool)
            .value("Int", QVariant::Int)
            .value("UInt", QVariant::UInt)
            .value("LongLong", QVariant::LongLong)
            .value("ULongLong", QVariant::ULongLong)
            .value("Double", QVariant::Double)
            .value("Char", QVariant::Char)
            .value("Map", QVariant::Map)
            .value("List", QVariant::List)
            .value("String", QVariant::String)
            .value("StringList", QVariant::StringList)
            .value("ByteArray", QVariant::ByteArray)
            .value("BitArray", QVariant::BitArray)
            .value("Date", QVariant::Date)
            .value("Time", QVariant::Time)
            .value("DateTime", QVariant::DateTime)
            .value("Url", QVariant::Url)
            .value("Locale", QVariant::Locale)
            .value("Rect", QVariant::Rect)
            .value("RectF", QVariant::RectF)
            .value("Size", QVariant::Size)
            .value("SizeF", QVariant::SizeF)
            .value("Line", QVariant::Line)
            .value("LineF", QVariant::LineF)
            .value("Point", QVariant::Point)
            .value("PointF", QVariant::PointF)
            .value("RegExp", QVariant::RegExp)
            .value("LastCoreType", QVariant::LastCoreType)
            .value("Font", QVariant::Font)
            .value("Pixmap", QVariant::Pixmap)
            .value("Brush", QVariant::Brush)
            .value("Color", QVariant::Color)
            .value("Palette", QVariant::Palette)
            .value("Icon", QVariant::Icon)
            .value("Image", QVariant::Image)
            .value("Polygon", QVariant::Polygon)
            .value("Region", QVariant::Region)
            .value("Bitmap", QVariant::Bitmap)
            .value("Cursor", QVariant::Cursor)
            .value("SizePolicy", QVariant::SizePolicy)
            .value("KeySequence", QVariant::KeySequence)
            .value("Pen", QVariant::Pen)
            .value("TextLength", QVariant::TextLength)
            .value("TextFormat", QVariant::TextFormat)
            .value("Matrix", QVariant::Matrix)
            .value("LastGuiType", QVariant::LastGuiType)
            .value("UserType", QVariant::UserType)
            .value("LastType", QVariant::LastType)
            .export_values()
            ;
        QVariant_exposer.def( bp::init< QVariant::Type >(( bp::arg("type") )) );
        QVariant_exposer.def( bp::init< int, void const * >(( bp::arg("typeOrUserType"), bp::arg("copy") )) );
        QVariant_exposer.def( bp::init< QDataStream & >(( bp::arg("s") )) );
        QVariant_exposer.def( bp::init< int >(( bp::arg("i") )) );
        QVariant_exposer.def( bp::init< uint >(( bp::arg("ui") )) );
        QVariant_exposer.def( bp::init< qlonglong >(( bp::arg("ll") )) );
        QVariant_exposer.def( bp::init< qulonglong >(( bp::arg("ull") )) );
        QVariant_exposer.def( bp::init< bool >(( bp::arg("b") )) );
        QVariant_exposer.def( bp::init< double >(( bp::arg("d") )) );
        QVariant_exposer.def( bp::init< char const * >(( bp::arg("str") )) );
        QVariant_exposer.def( bp::init< QByteArray const & >(( bp::arg("bytearray") )) );
        QVariant_exposer.def( bp::init< QBitArray const & >(( bp::arg("bitarray") )) );
        QVariant_exposer.def( bp::init< QString const & >(( bp::arg("string") )) );
        QVariant_exposer.def( bp::init< QLatin1String const & >(( bp::arg("string") )) );
        QVariant_exposer.def( bp::init< QStringList const & >(( bp::arg("stringlist") )) );
        QVariant_exposer.def( bp::init< QChar const & >(( bp::arg("qchar") )) );
        QVariant_exposer.def( bp::init< QDate const & >(( bp::arg("date") )) );
        QVariant_exposer.def( bp::init< QTime const & >(( bp::arg("time") )) );
        QVariant_exposer.def( bp::init< QDateTime const & >(( bp::arg("datetime") )) );
        QVariant_exposer.def( bp::init< QList<QVariant> const & >(( bp::arg("list") )) );
        QVariant_exposer.def( bp::init< QMap<QString, QVariant> const & >(( bp::arg("map") )) );
        QVariant_exposer.def( bp::init< QSize const & >(( bp::arg("size") )) );
        QVariant_exposer.def( bp::init< QSizeF const & >(( bp::arg("size") )) );
        QVariant_exposer.def( bp::init< QPoint const & >(( bp::arg("pt") )) );
        QVariant_exposer.def( bp::init< QPointF const & >(( bp::arg("pt") )) );
        QVariant_exposer.def( bp::init< QLine const & >(( bp::arg("line") )) );
        QVariant_exposer.def( bp::init< QLineF const & >(( bp::arg("line") )) );
        QVariant_exposer.def( bp::init< QRect const & >(( bp::arg("rect") )) );
        QVariant_exposer.def( bp::init< QRectF const & >(( bp::arg("rect") )) );
        QVariant_exposer.def( bp::init< QUrl const & >(( bp::arg("url") )) );
        QVariant_exposer.def( bp::init< QLocale const & >(( bp::arg("locale") )) );
        QVariant_exposer.def( bp::init< QRegExp const & >(( bp::arg("regExp") )) );
        QVariant_exposer.def( bp::init< Qt::GlobalColor >(( bp::arg("color") )) );
        QVariant_exposer.def( bp::init< >() );
        { //::QVariant::canConvert
        
            typedef bool ( ::QVariant::*canConvert_function_type )( ::QVariant::Type ) const;
            
            QVariant_exposer.def( 
                "canConvert"
                , canConvert_function_type( &::QVariant::canConvert )
                , ( bp::arg("t") ) );
        
        }
        { //::QVariant::clear
        
            typedef void ( ::QVariant::*clear_function_type )(  ) ;
            
            QVariant_exposer.def( 
                "clear"
                , clear_function_type( &::QVariant::clear ) );
        
        }
        { //::QVariant::convert
        
            typedef bool ( ::QVariant::*convert_function_type )( ::QVariant::Type ) ;
            
            QVariant_exposer.def( 
                "convert"
                , convert_function_type( &::QVariant::convert )
                , ( bp::arg("t") ) );
        
        }
        { //::QVariant::isNull
        
            typedef bool ( ::QVariant::*isNull_function_type )(  ) const;
            
            QVariant_exposer.def( 
                "isNull"
                , isNull_function_type( &::QVariant::isNull ) );
        
        }
        { //::QVariant::isValid
        
            typedef bool ( ::QVariant::*isValid_function_type )(  ) const;
            
            QVariant_exposer.def( 
                "isValid"
                , isValid_function_type( &::QVariant::isValid ) );
        
        }
        { //::QVariant::nameToType
        
            typedef ::QVariant::Type ( *nameToType_function_type )( char const * );
            
            QVariant_exposer.def( 
                "nameToType"
                , nameToType_function_type( &::QVariant::nameToType )
                , ( bp::arg("name") ) );
        
        }
        QVariant_exposer.def( bp::self != bp::self );
        QVariant_exposer.def( bp::self == bp::self );
        { //::QVariant::toBool
        
            typedef bool ( ::QVariant::*toBool_function_type )(  ) const;
            
            QVariant_exposer.def( 
                "toBool"
                , toBool_function_type( &::QVariant::toBool ) );
        
        }
        { //::QVariant::toByteArray
        
            typedef ::QByteArray ( ::QVariant::*toByteArray_function_type )(  ) const;
            
            QVariant_exposer.def( 
                "toByteArray"
                , toByteArray_function_type( &::QVariant::toByteArray ) );
        
        }
        { //::QVariant::toChar
        
            typedef ::QChar ( ::QVariant::*toChar_function_type )(  ) const;
            
            QVariant_exposer.def( 
                "toChar"
                , toChar_function_type( &::QVariant::toChar ) );
        
        }
        { //::QVariant::toDate
        
            typedef ::QDate ( ::QVariant::*toDate_function_type )(  ) const;
            
            QVariant_exposer.def( 
                "toDate"
                , toDate_function_type( &::QVariant::toDate ) );
        
        }
        { //::QVariant::toDateTime
        
            typedef ::QDateTime ( ::QVariant::*toDateTime_function_type )(  ) const;
            
            QVariant_exposer.def( 
                "toDateTime"
                , toDateTime_function_type( &::QVariant::toDateTime ) );
        
        }
        { //::QVariant::toDouble
        
            typedef double ( ::QVariant::*toDouble_function_type )( bool * ) const;
            
            QVariant_exposer.def( 
                "toDouble"
                , toDouble_function_type( &::QVariant::toDouble )
                , ( bp::arg("ok")=bp::object() ) );
        
        }
        { //::QVariant::toInt
        
            typedef int ( ::QVariant::*toInt_function_type )( bool * ) const;
            
            QVariant_exposer.def( 
                "toInt"
                , toInt_function_type( &::QVariant::toInt )
                , ( bp::arg("ok")=bp::object() ) );
        
        }
        { //::QVariant::toLine
        
            typedef ::QLine ( ::QVariant::*toLine_function_type )(  ) const;
            
            QVariant_exposer.def( 
                "toLine"
                , toLine_function_type( &::QVariant::toLine ) );
        
        }
        { //::QVariant::toLineF
        
            typedef ::QLineF ( ::QVariant::*toLineF_function_type )(  ) const;
            
            QVariant_exposer.def( 
                "toLineF"
                , toLineF_function_type( &::QVariant::toLineF ) );
        
        }
        { //::QVariant::toList
        
            typedef ::QList<QVariant> ( ::QVariant::*toList_function_type )(  ) const;
            
            QVariant_exposer.def( 
                "toList"
                , toList_function_type( &::QVariant::toList ) );
        
        }
        { //::QVariant::toLocale
        
            typedef ::QLocale ( ::QVariant::*toLocale_function_type )(  ) const;
            
            QVariant_exposer.def( 
                "toLocale"
                , toLocale_function_type( &::QVariant::toLocale ) );
        
        }
        { //::QVariant::toLongLong
        
            typedef ::qlonglong ( ::QVariant::*toLongLong_function_type )( bool * ) const;
            
            QVariant_exposer.def( 
                "toLongLong"
                , toLongLong_function_type( &::QVariant::toLongLong )
                , ( bp::arg("ok")=bp::object() ) );
        
        }
        { //::QVariant::toMap
        
            typedef ::QMap<QString, QVariant> ( ::QVariant::*toMap_function_type )(  ) const;
            
            QVariant_exposer.def( 
                "toMap"
                , toMap_function_type( &::QVariant::toMap ) );
        
        }
        { //::QVariant::toPoint
        
            typedef ::QPoint ( ::QVariant::*toPoint_function_type )(  ) const;
            
            QVariant_exposer.def( 
                "toPoint"
                , toPoint_function_type( &::QVariant::toPoint ) );
        
        }
        { //::QVariant::toPointF
        
            typedef ::QPointF ( ::QVariant::*toPointF_function_type )(  ) const;
            
            QVariant_exposer.def( 
                "toPointF"
                , toPointF_function_type( &::QVariant::toPointF ) );
        
        }
        { //::QVariant::toRect
        
            typedef ::QRect ( ::QVariant::*toRect_function_type )(  ) const;
            
            QVariant_exposer.def( 
                "toRect"
                , toRect_function_type( &::QVariant::toRect ) );
        
        }
        { //::QVariant::toRectF
        
            typedef ::QRectF ( ::QVariant::*toRectF_function_type )(  ) const;
            
            QVariant_exposer.def( 
                "toRectF"
                , toRectF_function_type( &::QVariant::toRectF ) );
        
        }
        { //::QVariant::toRegExp
        
            typedef ::QRegExp ( ::QVariant::*toRegExp_function_type )(  ) const;
            
            QVariant_exposer.def( 
                "toRegExp"
                , toRegExp_function_type( &::QVariant::toRegExp ) );
        
        }
        { //::QVariant::toSize
        
            typedef ::QSize ( ::QVariant::*toSize_function_type )(  ) const;
            
            QVariant_exposer.def( 
                "toSize"
                , toSize_function_type( &::QVariant::toSize ) );
        
        }
        { //::QVariant::toSizeF
        
            typedef ::QSizeF ( ::QVariant::*toSizeF_function_type )(  ) const;
            
            QVariant_exposer.def( 
                "toSizeF"
                , toSizeF_function_type( &::QVariant::toSizeF ) );
        
        }
        { //::QVariant::toString
        
            typedef ::QString ( ::QVariant::*toString_function_type )(  ) const;
            
            QVariant_exposer.def( 
                "toString"
                , toString_function_type( &::QVariant::toString ) );
        
        }
        { //::QVariant::toStringList
        
            typedef ::QStringList ( ::QVariant::*toStringList_function_type )(  ) const;
            
            QVariant_exposer.def( 
                "toStringList"
                , toStringList_function_type( &::QVariant::toStringList ) );
        
        }
        { //::QVariant::toTime
        
            typedef ::QTime ( ::QVariant::*toTime_function_type )(  ) const;
            
            QVariant_exposer.def( 
                "toTime"
                , toTime_function_type( &::QVariant::toTime ) );
        
        }
        { //::QVariant::toUInt
        
            typedef ::uint ( ::QVariant::*toUInt_function_type )( bool * ) const;
            
            QVariant_exposer.def( 
                "toUInt"
                , toUInt_function_type( &::QVariant::toUInt )
                , ( bp::arg("ok")=bp::object() ) );
        
        }
        { //::QVariant::toULongLong
        
            typedef ::qulonglong ( ::QVariant::*toULongLong_function_type )( bool * ) const;
            
            QVariant_exposer.def( 
                "toULongLong"
                , toULongLong_function_type( &::QVariant::toULongLong )
                , ( bp::arg("ok")=bp::object() ) );
        
        }
        { //::QVariant::toUrl
        
            typedef ::QUrl ( ::QVariant::*toUrl_function_type )(  ) const;
            
            QVariant_exposer.def( 
                "toUrl"
                , toUrl_function_type( &::QVariant::toUrl ) );
        
        }
        { //::QVariant::type
        
            typedef ::QVariant::Type ( ::QVariant::*type_function_type )(  ) const;
            
            QVariant_exposer.def( 
                "type"
                , type_function_type( &::QVariant::type ) );
        
        }
        { //::QVariant::typeName
        
            typedef char const * ( ::QVariant::*typeName_function_type )(  ) const;
            
            QVariant_exposer.def( 
                "typeName"
                , typeName_function_type( &::QVariant::typeName ) );
        
        }
        { //::QVariant::typeToName
        
            typedef char const * ( *typeToName_function_type )( ::QVariant::Type );
            
            QVariant_exposer.def( 
                "typeToName"
                , typeToName_function_type( &::QVariant::typeToName )
                , ( bp::arg("type") ) );
        
        }
        { //::QVariant::userType
        
            typedef int ( ::QVariant::*userType_function_type )(  ) const;
            
            QVariant_exposer.def( 
                "userType"
                , userType_function_type( &::QVariant::userType ) );
        
        }
        QVariant_exposer.staticmethod( "nameToType" );
        QVariant_exposer.staticmethod( "typeToName" );
    }

}
