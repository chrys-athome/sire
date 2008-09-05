
#include "SireError/errors.h"

#include <QByteArray>
#include <QDataStream>

#include <QDebug>

using namespace SireError;

int main(int argc, const char **argv)
{
    try
    {

    unknown_exception u = unknown_exception( "This is an unknown error!", CODELOC );

    QByteArray data = u.pack();

    qDebug() << data;   

    try
    {
        exception::unpackAndThrow(data);
    }
    catch(const SireError::unknown_exception &e)
    {
        qDebug() << "Caught unknown_error!!!" << e.toString();
        return 0;
    }
    catch(const SireError::exception &e)
    {
        qDebug() << "Something went wrong...!";
        qDebug() << e.toString();
    }
    
    }
    catch(const SireError::exception &e)
    {
        qDebug() << "Something went really wrong!";
        qDebug() << e.toString();
    }

    return -1;
}

