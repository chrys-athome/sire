/********************************************\
  *
  *  Siren - C++ metaobject library
  *
  *  Copyright (C) 2011  Christopher Woods
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

#include "Siren/exception.h"
#include "Siren/thread.h"
#include "Siren/siren.hpp"

using namespace Siren;

REGISTER_SIREN_VIRTUAL_CLASS( Siren::Exception )

/** Null constructor */
Exception::Exception() : Object()
{}

/** Construct the exception with the specified error and code location */
Exception::Exception(const String &error, CODELOC_ARGS)
          : Object(), err(error)
{
    plce = String::tr("Error thrown from function \"%1\", on line %2 "
                      "of file \"%3\".")
                        .arg(current_function).arg(line).arg(file);
                        
//    node_name = Thread::getNodeName();
//    pid_thrid = String:tr("PID: %1, THRID: %2")
//                    .arg(Thread::getPID(), Thread::getTHRID());
                        
    bt = Exception::generateBackTrace();
}

/** Copy constructor */
Exception::Exception(const Exception &other)
          : Object(other), err(other.err), plce(other.plce), 
            node_name(other.node_name), pid_thrid(other.pid_thrid),
            bt(other.bt)
{}

/** Destructor */
Exception::~Exception() throw()
{}

/** Return the backtrace showing the call stack when this exception
    was thrown */
StringList Exception::backTrace() const
{
    return bt;
}

/** Return the place (function, line and file) from where this
    exception was thrown */
String Exception::where() const
{
    return plce;
}

/** Return the reason why this exception was thrown */
String Exception::why() const
{
    return err;
}

/** Return the process and thread ID of the process/thread that
    originally threw this exception */
String Exception::pid() const
{
    return pid_thrid;
}

/** Return the name of the node that ran the process on which
    this exception was thrown */
String Exception::node() const
{
    return node_name;
}
    
/** Copy assignment operator */
void Exception::copy_object(const Exception &other)
{
    err = other.err;
    plce = other.plce;
    node_name = other.node_name;
    pid_thrid = other.pid_thrid;
    bt = other.bt;
    super::operator=(other);
}

/** Comparison operator */
bool Exception::compare_object(const Exception &other) const
{
    return err == other.err and
           plce == other.plce and
           node_name == other.node_name and
           pid_thrid == other.pid_thrid and
           bt == other.bt and
           super::operator==(other);
}
