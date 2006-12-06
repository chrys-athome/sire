#ifndef SIRECLUSTER_ERRORS_H
#define SIRECLUSTER_ERRORS_H

#include "SireError/exception.h"

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireCluster
{

/** This is the base class of all SireCluster errors */
class SIRECAS_EXPORT sirecluster_error : public SireError::exception
{
public:
    sirecluster_error() : exception()
    {}

    sirecluster_error(QString err, QString place = QString::null)
                  : exception(err,place)
    {}

    sirecluster_error(const sirecluster_error &other) : exception(other)
    {}

    ~sirecluster_error() throw()
    {}

    const char* what() const throw()
    {
        return "SireCluster::sirecluster_error";
    }
};

/** This class is thrown when an attempt is made to do something
    that is not valid with an already active processor. */
class already_active : public sirecluster_error
{
public:
    already_active() : sirecluster_error()
    {}

    already_active(QString err, QString place = QString::null)
              : sirecluster_error(err,place)
    {}

    already_active(const already_active &other) : sirecluster_error(other)
    {}

    ~already_active() throw()
    {}

    const char* what() const throw()
    {
        return "SireCluster::already_active";
    }
};

}

Q_DECLARE_METATYPE(SireCluster::sirecluster_error)
Q_DECLARE_METATYPE(SireCluster::already_active)

SIRE_END_HEADER

#endif
