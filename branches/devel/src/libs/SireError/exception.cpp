
#include "SireError/exception.h"
#include "SireStream/datastream.h"

#include "getbacktrace.h"

#include <QDataStream>

using namespace SireError;
using namespace SireStream;

/** Construct a null exception */
exception::exception()
{}

/** Constructor.
    \param error The error message associated with this exception.
    \param place From where in the code this exception was thrown. This
                 is supplied automatically via the 'CODELOC' macro.
*/
exception::exception(QString error, QString place) : err(error), plce(place)
{
    pidstr = QObject::tr("unidentified thread");
    bt = getBackTrace();

    //remove the top two items from the backtrace as they are 'getBackTrace'
    //and this constructor
    bt.removeFirst();
    bt.removeFirst();
}

/** Copy constructor */
exception::exception(const exception &other)
          : std::exception(other), err(other.err), plce(other.plce), bt(other.bt)
{}

/** Destructor */
exception::~exception() throw()
{}

/** Return a pretty string representation of all of the information stored
    in the exception, suitable for printing to the screen or to a log file. */
QString exception::toString() const throw()
{
    return QObject::tr("Exception '%1' thrown by process '%2'.\n"
                       "%3\n"
                       "Thrown from %4\n"
                       "__Backtrace__\n"
                       "%5\n"
                       "__EndTrace__")
             .arg(what()).arg(pid()).arg(why()).arg(where()).arg(bt.join("\n"));
}

/** Return the type of exception. This should be overloaded by each derived
    class so that it remains correct.
*/
const char* exception::what() const throw()
{
    return "SireError::exception";
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
