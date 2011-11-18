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
#include "Siren/bytearray.h"
#include "Siren/testreport.h"
#include "Siren/forages.h"
#include "Siren/siren.hpp"

using namespace Siren;

#define HAVE_BACKTRACE 1
#define HAVE_EXECINFO_H

////////////
//////////// Code used to generate a backtrace in a live program
////////////

#ifdef HAVE_BACKTRACE
  #ifdef HAVE_EXECINFO_H
    #include <execinfo.h>  // CONDITIONAL_INCLUDE
    #include <cxxabi.h>    // CONDITIONAL_INCLUDE
  #endif
#endif

//need to extract the symbol from the output of 'backtrace_symbols'

//a typical output from backtrace_symbols will look like;
//a.out(_ZNK1A12getBackTraceEv+0x12) [0x804ad36]

// This needs to be split into;
//  (1) The program or library containing the symbol (a.out)
//  (2) The symbol itself (_ZNK1A12getBackTraceEv)
//  (3) The offset? +0x12
//  (4) The symbol address ([0x804ad36])

// This is achieved by the following regexp
//              (unit )  (symbol)   (offset)          (address)
RegExp regexp("([^(]+)\\(([^)^+]+)(\\+[^)]+)\\)\\s(\\[[^]]+\\])");

//However, on OS X the output looks something like this;
//2 libSireBase.0.dylib 0x00da01a5 _ZNK8SireBase10PropertiesixERKNS_12PropertyNameE + 595
//
// Word 2 is the library, word 3 is the symbol address, word 4 is the symbol
// itself and word 6 is the offset(?)

/** Obtain a backtrace and return as a QStringList.
    This is not well-optimised, requires compilation with "-rdynamic" on linux
    and doesn't do a great job of demangling the symbol names. It is sufficient
    though to work out call trace. */
static StringList getBackTrace()
{
    //now get the backtrace of the code at this point
    //(we can only do this if we have 'execinfo.h'
#ifdef HAVE_BACKTRACE
    
    //create a void* array to hold the function addresses. We will only go at most 128 deep
    void *func_addresses[128];
    int nfuncs = backtrace(func_addresses, 128);

    //now get the function names associated with these symbols. This should work for elf
    //binaries, though additional linker options may need to have been called 
    //(e.g. -rdynamic for GNU ld. See the glibc documentation for 'backtrace')
    char **symbols = backtrace_symbols(func_addresses, nfuncs);
    
    //save all of the function names onto the QStringList....
    //(note that none of this will work if we have run out of memory)
    QStringList ret;

    if (nfuncs == 1)
    {
        //we have probably been compiled with -fomit-frame-pointer, so this
        //has only been able to get the backtrace back to the current function
        ret.append( String::tr("This is an incomplete backtrace as it looks "
                    "like this code was compiled without a frame pointer\n"
                    "(e.g. using -fomit-frame-pointer)") );
    }

    for (int i=0; i<nfuncs; i++)
    {
        if (regexp.indexIn(symbols[i]) != -1)
        {
            //get the library or app that contains this symbol
            String unit = regexp.cap(1);
            //get the symbol
            String symbol = regexp.cap(2);
            ByteArray symbol_data = symbol.toAscii();
            //get the offset
            String offset = regexp.cap(3);
            //get the address
            String address = regexp.cap(4);
        
            //now try and demangle the symbol
            int stat;
            char *demangled = 
                    abi::__cxa_demangle(symbol_data.data(),0,0,&stat);
        
            if (demangled)
            {
                symbol = demangled;
                delete demangled;
            }


            //put this all together
            ret.append( String("(%1) %2 (%3 +%4)\n  -- %5\n")
                                .arg(String::number(i), 3)
                                .arg(unit).arg(address,offset)
                                .arg(symbol) );
        }
        else
        {
            //split line into words
            StringList words = String(symbols[i]).split(" ");
            
            if (words.count() == 6 and words[4] == "+")
            {
                //this is probably an OS X line...

                //get the library or app that contains this symbol
                String unit = words[1];
                //get the symbol
                String symbol = words[3];
                ByteArray symbol_data = symbol.toAscii();
                //get the offset
                String offset = words[5];
                //get the address
                String address = words[2];
        
                //now try and demangle the symbol
                int stat;
                char *demangled = 
                        abi::__cxa_demangle(symbol_data.data(),0,0,&stat);
        
                if (demangled)
                {
                    symbol = demangled;
                    delete demangled;
                }

                //put this all together
                ret.append( String("(%1) %2 (%3 +%4)\n  -- %5\n")
                                    .arg(String::number(i), 3)
                                    .arg(unit).arg(address,offset)
                                    .arg(symbol) );
            }
            else
                //I don't recognise this string - just add the raw
                //string to the backtrace
                ret.append(symbols[i]);
        }
    }
    
    //we now need to release the memory of the symbols array. Since it was allocated using
    //malloc, we must release it using 'free'
    free(symbols);

    return ret;

#else
    return StringList( String::tr(
                "Backtrace is not available on this system. Backtrace is "
                "available on Linux, Mac OS X (>=10.5) and any other system "
                "that provides the backtrace_symbols() API found in "
                "execinfo.h")
                      );
#endif

}

////////////
//////////// Implementation of Siren::Exception
////////////

REGISTER_SIREN_VIRTUAL_CLASS( Siren::Exception )

/** Null constructor */
Exception::Exception() : Object()
{}

/** Construct the exception with the specified error and code location */
Exception::Exception(const String &error, CODELOC_ARGS)
          : Object(), err(error)
{
    plce = String::tr("\"%1\", on line %2 "
                      "of file \"%3\".")
                        .arg(current_function).arg(line).arg(file);
                        
//    node_name = Thread::getNodeName();
//    pid_thrid = String:tr("PID: %1, THRID: %2")
//                    .arg(Thread::getPID(), Thread::getTHRID());
                        
    date_time = DateTime::current();
    pid_thrid = for_ages::getThisThreadName();                    
    bt = Exception::generateBackTrace();
}

/** Copy constructor */
Exception::Exception(const Exception &other)
          : Object(other), err(other.err), plce(other.plce), 
            node_name(other.node_name), pid_thrid(other.pid_thrid),
            date_time(other.date_time), bt(other.bt)
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
    
/** Return the date and time at which the exception was thrown */
DateTime Exception::dateTime() const
{
    return date_time;
}
    
/** Copy assignment operator */
void Exception::copy_object(const Exception &other)
{
    err = other.err;
    plce = other.plce;
    node_name = other.node_name;
    pid_thrid = other.pid_thrid;
    date_time = other.date_time;
    bt = other.bt;
    super::copy_object(other);
}

/** Comparison operator */
bool Exception::compare_object(const Exception &other) const
{
    return err == other.err and
           plce == other.plce and
           node_name == other.node_name and
           pid_thrid == other.pid_thrid and
           date_time == other.date_time and
           bt == other.bt and
           super::compare_object(other);
}

/** Return a string representation of the exception */
String Exception::toString() const
{
    if (bt.isEmpty())
        //this is an empty exception - just return the name
        return this->what();

    String box = String::tr("*****************************************\n"
                            "|ERROR: %1\n"
                            "|\n"
                            "|  **************************************\n"
                            "|Exception class %2.\n"
                            "|\n"
                            "|  **************************************\n"
                            "|Thrown at: %3.\n"
                            "|\n"
                            "|  **************************************\n"
                            "|Thrown from: %4.\n"
                            "|\n"
                            "|  **************************************\n"
                            "|Node / process: %5 | %6\n"
                            "|\n"
                            "|****************************************\n")
                    .arg(err, this->what(),
                         this->dateTime().toString(),
                         this->where(), this->node(), this->pid());

    return String::tr("%1\n"
                      "Backtrace:\n%2\n\n"
                      "%3")
                .arg(box, bt.join("\n"), box);
}

/** Generate and return a live backtrace */
StringList Exception::generateBackTrace()
{
    return ::getBackTrace();
}

/** Test this exception */
void Exception::test(TestReportEditor &report) const
{
    try
    {
        this->throwSelf();
    }
    catch(const Exception &e)
    {
        report.testTrue( e.isA(this->what()), 
                    String::tr("Exception type %1 is caught correctly.")
                                .arg(this->what()) );
                                
        report.testEqual( *this, e,
                    String::tr("Caught exception is equal to itself, from throwSelf()") );
    }
}
