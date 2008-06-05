#ifndef SIREERROR_EXCEPTION_H
#define SIREERROR_EXCEPTION_H

#include <exception>

#include <QString>
#include <QStringList>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireError
{
class exception;
}

class QDataStream;
QDataStream& operator<<(QDataStream&, const SireError::exception&);
QDataStream& operator>>(QDataStream&, SireError::exception&);

namespace SireError
{

/** This is the base class of all Sire specific exceptions. The python wrapping
    allows for automatic conversion of any exception derived from this base.
    The exception system in Sire is very basic, namely there are a collection
    of named exceptions. Each exception only has basic information such as
    from where is was thrown, and an optional message to say why is was thrown.
     
    @author Christopher Woods
*/
class SIREERROR_EXPORT exception : public std::exception
{

friend QDataStream& ::operator<<(QDataStream&, const exception&);
friend QDataStream& ::operator>>(QDataStream&, exception&);

public:
    exception();
    exception(QString error, QString place = QString::null);
    
    exception(const exception &other);
    
    virtual ~exception() throw();

    virtual const char* what() const throw();
    
    QString error() const throw();
    QString from() const throw();
    QStringList trace() const throw();
    QString where() const throw();
    QString why() const throw();
    QString pid() const throw();

    QString toString() const throw();

protected:
    QString err;  ///< The error associated with the exception.
    QString plce; ///< Description of from where the exception was thrown.
    QStringList bt; ///< Backtrace at the point that the exception was constructed
    QString pidstr; /**< String identifying the process from which 
                             process/thread this exception was thrown */
};

}

Q_DECLARE_METATYPE(SireError::exception);

SIRE_END_HEADER

#endif
