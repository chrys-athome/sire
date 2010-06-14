/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2010  Christopher Woods
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

#ifndef SIREN_WAITCONDITION_H
#define SIREN_WAITCONDITION_H

#include <QWaitCondition>

#include "sirenglobal.h"

SIREN_BEGIN_HEADER

namespace Siren
{

class Mutex;

/** This class provides a WaitCondition which respects the end
    of for_ages (i.e. .wait() can be interupted
    by signalling the end of for_ages, in which case it will
    throw a Siren::interupted exception. In all other
    respects, this is identical to a QWaitCondition
    
    @author Christopher Woods
*/
class SIREN_EXPORT WaitCondition
{
public:
    WaitCondition();
    ~WaitCondition();
    
    bool wait( Mutex *mutex, unsigned long time = ULONG_MAX );
    bool wait( QMutex *mutex, unsigned long time = ULONG_MAX );
    
    void wakeOne();
    void wakeAll();
    
private:
    QWaitCondition w;
};

}

SIREN_EXPOSE_CLASS( Siren::WaitCondition )

SIREN_END_HEADER

#endif
