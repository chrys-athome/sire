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

static const RegisterHandle<Logger> r_logger;

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
                
    if (textstream->device() == 0 and textstream->string() == 0)
        throw Siren::invalid_arg( QObject::tr(
                "You cannot create a Logger from a QTextStream that is "
                "not attached to a valid device!"), CODELOC );
                
    if (textstream->device() != 0 and not (textstream->device()->openMode() != 
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

/** Copy assignment operator */
Logger& Logger::operator=(const Logger &other)
{
    Handles<QTextStream>::operator=(other);
    return *this;
}

/** Comparison operator */
bool Logger::operator==(const Logger &other) const
{
    return Handles<QTextStream>::operator==(other);
}

/** Comparison operator */
bool Logger::operator!=(const Logger &other) const
{
    return Handles<QTextStream>::operator!=(other);
}

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

/** Write the passed text to the logger. */
void Logger::write(const QString &text)
{
    HandleLocker lkr(*this);
    resource() << text;
    endl(resource());
}

static QString repeated(const QString &s, int n)
{
    #if QT_VERSION >= 0x040500
        return s.repeated(n);
    #else
        QString r = s;
        
        for (int i=1; i<n; ++i)
        {
            r += s;
        }
        
        return r;
    #endif
}

static void printBox(const QString &line, QTextStream &stream)
{
    QStringList lines = line.split("\n");
    
    int maxlength = 0;
    
    for (QStringList::iterator it = lines.begin(); it != lines.end(); ++it)
    {
        *it = it->simplified();
    
        if (it->length() > maxlength)
            maxlength = it->length();
    }
    
    const int max_maxlength = 80;
    
    if (maxlength > max_maxlength)
        maxlength = max_maxlength;
    
    QString hashline = ::repeated( "-", maxlength + 2 );
    
    endl(stream);
    stream << "*" << hashline << "*";
    endl(stream);
    
    foreach (const QString &l, lines)
    {
        if (l.length() > max_maxlength)
        {
            for (int j=0; j<l.length(); j+=max_maxlength)
            {
                stream << "| " << l.mid(j,max_maxlength).leftJustified(maxlength)
                       << " |";

                endl(stream);
            }
        }
        else
        {
            stream << "| " << l.leftJustified(maxlength) << " |";
            endl(stream);
        }
    }
    
    stream << "*" << hashline << "*";
    endl(stream);
    endl(stream);
}

static void printStarred(const QString &line, QTextStream &stream)
{
    QStringList lines = line.split("\n");
    
    int maxlength = 0;
    
    for (QStringList::iterator it = lines.begin(); it != lines.end(); ++it)
    {
        *it = it->simplified();
    
        if (it->length() > maxlength)
            maxlength = it->length();
    }
    
    const int max_maxlength = 80;
    
    if (maxlength > max_maxlength)
        maxlength = max_maxlength;
    
    QString hashline = ::repeated( "*", maxlength + 2 );
    
    endl(stream);
    stream << "*" << hashline << "*";
    endl(stream);
    
    foreach (const QString &l, lines)
    {
        if (l.length() > max_maxlength)
        {
            for (int j=0; j<l.length(); j+=max_maxlength)
            {
                stream << "* " << l.mid(j,max_maxlength).leftJustified(maxlength)
                       << " *";

                endl(stream);
            }
        }
        else
        {
            stream << "* " << l.leftJustified(maxlength) << " *";
            endl(stream);
        }
    }
    
    stream << "*" << hashline << "*";
    endl(stream);
    endl(stream);
}

static void printUnderlined(const QString &line, QTextStream &stream)
{
    QStringList lines = line.split("\n");
    
    int maxlength = 0;
    
    for (QStringList::iterator it = lines.begin(); it != lines.end(); ++it)
    {
        *it = it->simplified();
    
        if (it->length() > maxlength)
            maxlength = it->length();
    }
    
    const int max_maxlength = 80;
    
    if (maxlength > max_maxlength)
        maxlength = max_maxlength;
    
    QString hashline = ::repeated( "-", maxlength);
    
    endl(stream);
    stream << hashline;
    endl(stream);
    
    foreach (const QString &l, lines)
    {
        if (l.length() > max_maxlength)
        {
            for (int j=0; j<l.length(); j+=max_maxlength)
            {
                stream <<l.mid(j,max_maxlength).leftJustified(maxlength);
                endl(stream);
            }
        }
        else
        {
            stream << l.leftJustified(maxlength);
            endl(stream);
        }
    }
    
    stream << hashline;
    endl(stream);
    endl(stream);
}

/** Write the passed text to the logger, in 'header' format. 
    This is a level 'level' header (headers can run from h1 to h6,
    as in html) */
void Logger::writeHeader(const QString &text, int level)
{
    HandleLocker lkr(*this);
    
    if (level <= 1)
        ::printBox(text, resource());

    else if (level == 2)
        ::printStarred(text, resource());
        
    else
        ::printUnderlined(text, resource());
}
