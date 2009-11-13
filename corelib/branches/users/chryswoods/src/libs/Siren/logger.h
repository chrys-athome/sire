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

#ifndef SIREN_LOGGER_H
#define SIREN_LOGGER_H

#include "object.h"

#include "mutable.h"
#include "shared.h"

SIREN_BEGIN_HEADER

namespace Siren
{

class Logger;

template<class T> class ObjPtr;

typedef ObjPtr<Logger> LoggerPtr;

/** This is the base class of all of the Loggers. These are 
    simple classes that provide a means of recording output
    during a function call (e.g. by either printing directly
    to the screen or by sending the output to a file or other sink)
    
    @author Christopher Woods
*/
class SIREN_EXPORT Logger 
            : public Extends<Logger,Object>, 
              public Interfaces<Logger,Mutable,Shared>
{
public:
    Logger();
    
    Logger(const Logger &other);
    
    virtual ~Logger();

    static LoggerPtr getDefault();
    static void setDefault(const Logger &logger);

    ///////////////////////
    // Logger            //
    ///////////////////////

    virtual void write(const QString &text)=0;

    ///////////////////////
    // Shared Interface  //
    ///////////////////////
    
    virtual void detach() const=0;

protected:
    friend class Interfaces<Logger,Mutable,Shared>;
    static void registerInterfaces();
};

class SIREN_EXPORT StdoutLogger : public Implements<StdoutLogger,Logger>
{
public:
    StdoutLogger();
    
    StdoutLogger(const StdoutLogger &other);
    
    ~StdoutLogger();
    
    StdoutLogger& operator=(const StdoutLogger &other);
    
    bool operator==(const StdoutLogger &other) const;
    bool operator!=(const StdoutLogger &other) const;

    ////////////////////////
    // Implements Object  //
    ////////////////////////
    
    HASH_CODE hashCode() const;
    QString toString() const;
    bool test(Logger &logger) const;

    ////////////////////////
    // Implements Logger  //
    ////////////////////////
    
    void write(const QString &text);

    void detach() const;

    ///////////////////////
    // Mutable Interface //
    ///////////////////////

    ObjRef saveState() const;
    void restoreState(const Object &object);
};

}

Q_DECLARE_METATYPE( Siren::StdoutLogger )

SIREN_EXPOSE_CLASS( Siren::Logger )
SIREN_EXPOSE_CLASS( Siren::StdoutLogger )

SIREN_END_HEADER

#endif
