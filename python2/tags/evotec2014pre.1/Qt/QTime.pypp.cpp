// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License

#include "boost/python.hpp"
#include <QByteArray>
#include <QDateTime>
#include <QUuid>
#include <QBitArray>
#include "QTime.pypp.hpp"

namespace bp = boost::python;

QTime __copy__(const QTime &other){ return QTime(other); }

#include "Qt/qdatastream.hpp"

void register_QTime_class(){

    { //::QTime
        typedef bp::class_< QTime > QTime_exposer_t;
        QTime_exposer_t QTime_exposer = QTime_exposer_t( "QTime", bp::init< >() );
        bp::scope QTime_scope( QTime_exposer );
        QTime_exposer.def( bp::init< int, int, bp::optional< int, int > >(( bp::arg("h"), bp::arg("m"), bp::arg("s")=(int)(0), bp::arg("ms")=(int)(0) )) );
        { //::QTime::addMSecs
        
            typedef ::QTime ( ::QTime::*addMSecs_function_type )( int ) const;
            addMSecs_function_type addMSecs_function_value( &::QTime::addMSecs );
            
            QTime_exposer.def( 
                "addMSecs"
                , addMSecs_function_value
                , ( bp::arg("ms") ) );
        
        }
        { //::QTime::addSecs
        
            typedef ::QTime ( ::QTime::*addSecs_function_type )( int ) const;
            addSecs_function_type addSecs_function_value( &::QTime::addSecs );
            
            QTime_exposer.def( 
                "addSecs"
                , addSecs_function_value
                , ( bp::arg("secs") ) );
        
        }
        { //::QTime::currentTime
        
            typedef ::QTime ( *currentTime_function_type )(  );
            currentTime_function_type currentTime_function_value( &::QTime::currentTime );
            
            QTime_exposer.def( 
                "currentTime"
                , currentTime_function_value );
        
        }
        { //::QTime::elapsed
        
            typedef int ( ::QTime::*elapsed_function_type )(  ) const;
            elapsed_function_type elapsed_function_value( &::QTime::elapsed );
            
            QTime_exposer.def( 
                "elapsed"
                , elapsed_function_value );
        
        }
  /*      { //::QTime::fromString
        
            typedef ::QTime ( *fromString_function_type )( ::QString const &,::Qt::DateFormat );
            fromString_function_type fromString_function_value( &::QTime::fromString );
            
            QTime_exposer.def( 
                "fromString"
                , fromString_function_value
                , ( bp::arg("s"), bp::arg("f")=::Qt::TextDate ) );
        
        } */
        { //::QTime::fromString
        
            typedef ::QTime ( *fromString_function_type )( ::QString const &,::QString const & );
            fromString_function_type fromString_function_value( &::QTime::fromString );
            
            QTime_exposer.def( 
                "fromString"
                , fromString_function_value
                , ( bp::arg("s"), bp::arg("format") ) );
        
        }
        { //::QTime::hour
        
            typedef int ( ::QTime::*hour_function_type )(  ) const;
            hour_function_type hour_function_value( &::QTime::hour );
            
            QTime_exposer.def( 
                "hour"
                , hour_function_value );
        
        }
        { //::QTime::isNull
        
            typedef bool ( ::QTime::*isNull_function_type )(  ) const;
            isNull_function_type isNull_function_value( &::QTime::isNull );
            
            QTime_exposer.def( 
                "isNull"
                , isNull_function_value );
        
        }
        { //::QTime::isValid
        
            typedef bool ( ::QTime::*isValid_function_type )(  ) const;
            isValid_function_type isValid_function_value( &::QTime::isValid );
            
            QTime_exposer.def( 
                "isValid"
                , isValid_function_value );
        
        }
        { //::QTime::isValid
        
            typedef bool ( *isValid_function_type )( int,int,int,int );
            isValid_function_type isValid_function_value( &::QTime::isValid );
            
            QTime_exposer.def( 
                "isValid"
                , isValid_function_value
                , ( bp::arg("h"), bp::arg("m"), bp::arg("s"), bp::arg("ms")=(int)(0) ) );
        
        }
        { //::QTime::minute
        
            typedef int ( ::QTime::*minute_function_type )(  ) const;
            minute_function_type minute_function_value( &::QTime::minute );
            
            QTime_exposer.def( 
                "minute"
                , minute_function_value );
        
        }
        { //::QTime::msec
        
            typedef int ( ::QTime::*msec_function_type )(  ) const;
            msec_function_type msec_function_value( &::QTime::msec );
            
            QTime_exposer.def( 
                "msec"
                , msec_function_value );
        
        }
        { //::QTime::msecsTo
        
            typedef int ( ::QTime::*msecsTo_function_type )( ::QTime const & ) const;
            msecsTo_function_type msecsTo_function_value( &::QTime::msecsTo );
            
            QTime_exposer.def( 
                "msecsTo"
                , msecsTo_function_value
                , ( bp::arg("arg0") ) );
        
        }
        QTime_exposer.def( bp::self != bp::self );
        QTime_exposer.def( bp::self < bp::self );
        QTime_exposer.def( bp::self <= bp::self );
        QTime_exposer.def( bp::self == bp::self );
        QTime_exposer.def( bp::self > bp::self );
        QTime_exposer.def( bp::self >= bp::self );
        { //::QTime::restart
        
            typedef int ( ::QTime::*restart_function_type )(  ) ;
            restart_function_type restart_function_value( &::QTime::restart );
            
            QTime_exposer.def( 
                "restart"
                , restart_function_value );
        
        }
        { //::QTime::second
        
            typedef int ( ::QTime::*second_function_type )(  ) const;
            second_function_type second_function_value( &::QTime::second );
            
            QTime_exposer.def( 
                "second"
                , second_function_value );
        
        }
        { //::QTime::secsTo
        
            typedef int ( ::QTime::*secsTo_function_type )( ::QTime const & ) const;
            secsTo_function_type secsTo_function_value( &::QTime::secsTo );
            
            QTime_exposer.def( 
                "secsTo"
                , secsTo_function_value
                , ( bp::arg("arg0") ) );
        
        }
        { //::QTime::setHMS
        
            typedef bool ( ::QTime::*setHMS_function_type )( int,int,int,int ) ;
            setHMS_function_type setHMS_function_value( &::QTime::setHMS );
            
            QTime_exposer.def( 
                "setHMS"
                , setHMS_function_value
                , ( bp::arg("h"), bp::arg("m"), bp::arg("s"), bp::arg("ms")=(int)(0) ) );
        
        }
        { //::QTime::start
        
            typedef void ( ::QTime::*start_function_type )(  ) ;
            start_function_type start_function_value( &::QTime::start );
            
            QTime_exposer.def( 
                "start"
                , start_function_value );
        
        }
   /*     { //::QTime::toString
        
            typedef ::QString ( ::QTime::*toString_function_type )( ::Qt::DateFormat ) const;
            toString_function_type toString_function_value( &::QTime::toString );
            
            QTime_exposer.def( 
                "toString"
                , toString_function_value
                , ( bp::arg("f")=::Qt::TextDate ) );
        
        } */
        { //::QTime::toString
        
            typedef ::QString ( ::QTime::*toString_function_type )( ::QString const & ) const;
            toString_function_type toString_function_value( &::QTime::toString );
            
            QTime_exposer.def( 
                "toString"
                , toString_function_value
                , ( bp::arg("format") ) );
        
        }
        QTime_exposer.staticmethod( "currentTime" );
        QTime_exposer.staticmethod( "fromString" );
        QTime_exposer.staticmethod( "isValid" );
        QTime_exposer.def( "__copy__", &__copy__);
        QTime_exposer.def( "__deepcopy__", &__copy__);
        QTime_exposer.def( "clone", &__copy__);
        QTime_exposer.def( "__rlshift__", &__rlshift__QDataStream< ::QTime >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
        QTime_exposer.def( "__rrshift__", &__rrshift__QDataStream< ::QTime >,
                            bp::return_internal_reference<1, bp::with_custodian_and_ward<1,2> >() );
    }

}
