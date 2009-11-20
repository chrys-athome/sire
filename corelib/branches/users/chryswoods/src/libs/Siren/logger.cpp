/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2009  Christopher Woods
  *
  *  This program is free software; you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation; either version 2 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program; if not, write to the Free Software
  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  *  For full details of the license please see the COPYING file
  *  that should have come with this distribution.
  *
  *  You can contact the authors via the developer's mailing list
  *  at http://siremol.org
  *
\*********************************************/

#include <QFile>
#include <QProcess>
#include <QBuffer>

#include "logger.h"

#include "Siren/errors.h"

using namespace Siren;

Q_GLOBAL_STATIC_WITH_ARGS( Logger, getDefaultLogger, 
                           (new QTextStream(stdout, QIODevice::WriteOnly)) );

/** Constructor - by default this writes to standard output */
Logger::Logger() : ImplementsHandle<Logger, Handles<QTextStream> >()
{
    this->operator=( *(getDefaultLogger()) );
}

/** Construct to handle the passed QTextStream */
Logger::Logger(QTextStream *textstream)
       : ImplementsHandle<Logger, Handles<QTextStream> >(textstream)
{
    if (textstream == 0)
        throw Siren::invalid_arg( QObject::tr(
                "You cannot create a Logger from a null QTextStream!"), CODELOC );
                
    if (textstream->device() == 0)
        throw Siren::invalid_arg( QObject::tr(
                "You cannot create a Logger from a QTextStream that is "
                "not attached to a valid device!"), CODELOC );
                
    if ( not (textstream->device()->openMode() != 
                QIODevice::WriteOnly|QIODevice::Append) )
        throw Siren::invalid_arg( QObject::tr(
                "You cannot create a Logger from a QTextStream that is connected "
                "to a device that is not opened in WriteOnly or Append mode."),
                    CODELOC );
}

/** Copy constructor */
Logger::Logger(const Logger &other)
       : ImplementsHandle<Logger, Handles<QTextStream> >(other)
{}

/** Destructor */
Logger::~Logger()
{}

/** Return a string representation of this logger */
QString Logger::toString() const
{
    const QIODevice *device = this->resource().device();

    {
        const QFile *f = dynamic_cast<const QFile*>(device);
    
        if (f)
            return QObject::tr("Logger( file:%1 )").arg(f->fileName());
    }
    {
        const QProcess *p = dynamic_cast<const QProcess*>(device);
    
        if (p)
            return QObject::tr("Logger( process )");
    }
    {
        const QBuffer *b = dynamic_cast<const QBuffer*>(device);
        
        if (b)
            return QObject::tr("Logger( buffer )");
    }
    
    return QObject::tr("Logger( unknown )");
}

uint Logger::hashCode() const
{
    return toInt( &(resource()) );
}

bool Logger::test(Logger &logger) const
{
    return false;
}

/** Write the passed text to the logger. */
void Logger::write(const QString &text)
{
    HandleLocker lkr(*this);
    resource() << text;
}
