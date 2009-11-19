/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007  Christopher Woods
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

#include "class.h"
#include "stream.h"
#include "tester.h"
#include "logger.h"
#include "getbacktrace.h"
#include "streamqt.h"

#include "Siren/exception.h"
#include "Siren/errors.h"

#include <QThreadStorage>

using namespace Siren;
using namespace Siren::detail;

Q_GLOBAL_STATIC( QThreadStorage<QString*>, pidStrings );
Q_GLOBAL_STATIC( QString, processString );

namespace Siren
{

/** Set the string that SireError will use to identify this process */
void SIREN_EXPORT setProcessString(const QString &s)
{
    *(processString()) = s;
}

/** Set the string that SireError will used to identify this thread
    within the process */
void SIREN_EXPORT setThreadString(const QString &s)
{
    pidStrings()->setLocalData( new QString(s) );
}

/** Return the string used by SireError to identify the process */
QString SIREN_EXPORT getProcessString()
{
    QString *s = processString();
    
    if (s->isEmpty())
    {
        return *s = QObject::tr("master");
    }
    
    return *s;
}

/** Return the string used to identify a particular thread */
QString SIREN_EXPORT getThreadString()
{
    QThreadStorage<QString*> *store = pidStrings();
    
    if (store->hasLocalData())
    {
        return *(store->localData());
    }
    else
    {
        return QString::null;
    }
}

/** Return the string used by SireError to identify a particular
    thread within a process */
QString SIREN_EXPORT getPIDString()
{
    QThreadStorage<QString*> *store = pidStrings();
    
    if (store->hasLocalData())
    {
        return QString("%1:%2").arg( getProcessString(),
                                     *(store->localData()) );
    }
    else
    {
        return getProcessString();
    }
}

} // end of namespace Siren

//////////
////////// Implementation of exception
//////////

static const RegisterMetaType<exception> r_exception( VIRTUAL_CLASS );

/** Construct a null exception */
exception::exception() : Extends<exception,Object>()
{
    pidstr = getPIDString();
}

/** Constructor.
    \param error The error message associated with this exception.
    \param place From where in the code this exception was thrown. This
                 is supplied automatically via the 'CODELOC' macro.
*/
exception::exception(QString error, QString place) 
          : Extends<exception,Object>(), err(error), plce(place)
{
    bt = getBackTrace();
    pidstr = getPIDString();
}

/** Copy constructor */
exception::exception(const exception &other)
          : Extends<exception,Object>(other), 
            err(other.err), plce(other.plce), 
            bt(other.bt), pidstr(other.pidstr)
{}

/** Destructor */
exception::~exception() throw()
{}

exception& exception::operator=(const exception &other)
{
    err = other.err;
    plce = other.plce;
    bt = other.bt;
    pidstr = other.pidstr;
    Object::operator=(other);
    
    return *this;
}

bool exception::operator==(const exception &other) const
{
    return err == other.err and plce == other.plce and
           bt == other.bt and pidstr == other.pidstr and
           Object::operator==(other);
}

bool exception::operator!=(const exception &other) const
{
    return not exception::operator==(other);
}

void exception::stream(Stream &s)
{
    s.assertVersion<exception>(1);

    Schema schema = s.item(this->getClass());
    
    schema.data("error") & err;
    schema.data("where") & plce;
    schema.data("backtrace") & bt;
    schema.data("pid") & pidstr;
    
    Object::stream( schema.base() );
}

/** Return the error message associated with this exception.
    \return The error message associated with this exception.
*/
QString exception::error() const throw()
{
    return err;
}

/** Return the location (function, line, file) from which this exception
    was originally thrown. The amount of information available will depend
    on the compiler used to compile the program, and come from the 'CODELOC'
    macro.
    \return Description of from where the exception was thrown.
*/
QString exception::from() const throw()
{
    return plce;
}

/** Return the function backtrace when the exception was constructed */
QStringList exception::trace() const throw()
{
    return bt;
}

/** Overloaded functions to give logical names... */
QString exception::where() const throw()
{
    return this->from();
}

/** Overloaded functions to give logical names... */
QString exception::why() const throw()
{
    return this->error();
}

/** Return the identifying string for the thread on the process that threw
    this exception. You should call the static SireError::exception::setPID
    function in each thread for the result of this call to be intelligable */
QString exception::pid() const throw()
{
    return pidstr;
}

bool exception::test(Logger &logger) const
{
    Tester tester(*this, logger);

    try
    {
        tester.nextTest( QObject::tr("Test throwing the exception \"%1\".")
                                .arg(this->what()) );
        this->testException();
    }
    catch(const Siren::exception &e)
    {
        tester.unexpected_error(e);
    }
    catch(const std::exception &e)
    {
        tester.unexpected_error( std_exception(e) );
    }
    catch(...)
    {
        tester.unexpected_error( unknown_error(CODELOC) );
    }
    
    return tester.allPassed();
}

HASH_CODE exception::hashCode() const
{
    return qHash(err) + qHash(plce);
}

/** Return a pretty string representation of all of the information stored
    in the exception, suitable for printing to the screen or to a log file. */
QString exception::toString() const throw()
{
    return QObject::tr("Exception '%1' thrown by the thread '%2'.\n"
                       "%3\n"
                       "Thrown from %4\n"
                       "__Backtrace__\n"
                       "%5\n"
                       "__EndTrace__\n"
                       "Exception '%1' thrown by thread '%2'.\n"
                       "%3")
             .arg(what()).arg(pid()).arg(why()).arg(where()).arg(bt.join("\n"));
}

////////
//////// Implementation of printError
////////

#include <QTextStream>

namespace Siren
{

void printAaargh(QTextStream &ts)
{
    ts << " ^   ^  \n"
       << " O   O  \n"
       << "   \"   \n"
       << " /\"\"\"\\   AAAAAARRRRGGGHHHH!!!!!!!\n"
       << " \\___/\n\n";
}

/** Print the error 'text' to standard output */
void SIREN_EXPORT printError(const QString &text)
{
    QTextStream ts(stdout);
    
    ts << "\n**********************************************************\n"
       << "Something went wrong with the program. Here's the error\n"
       << "**********************************************************\n\n"
       << text
       << "\n\nSorry - the program terminated with an error.\n\n";
    
    printAaargh(ts);
}

/** Print the error 'e' to standard output */
void SIREN_EXPORT printError(const Siren::exception &e)
{
    Siren::printError( e.toString() );
}

} // end of namespace Siren
