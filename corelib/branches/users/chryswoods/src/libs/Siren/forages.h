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

#ifndef SIREN_FORAGES_H
#define SIREN_FORAGES_H

#include <QMutex>

class QThread;

#include "sirenglobal.h"

SIREN_BEGIN_HEADER

namespace Siren
{

void register_this_thread();
void unregister_this_thread();

bool for_ages();

void check_for_ages();

void pause_for_ages();
void pause_for_ages(const QThread *thread);

void play_for_ages();
void play_for_ages(const QThread *thread);

void end_for_ages();
void end_for_ages(const QThread *thread);

/** This class provides a Mutex which respects the end
    of for_ages (i.e. .lock() and .tryLock() can be interupted
    by signalling the end of for_ages, in which case they will
    throw a Siren::interupted exception. In all other
    respects, this is identical to a QMutex
    
    @author Christopher Woods
*/
class Mutex
{
public:
    Mutex( QMutex::RecursionMode mode = QMutex::NonRecursive );
    ~Mutex();
    
    void lock();
    bool tryLock();
    bool tryLock(int ms);
    
    void unlock();
    
private:
    /** The actual QMutex */
    QMutex m;
};

}

SIREN_END_HEADER

#endif
