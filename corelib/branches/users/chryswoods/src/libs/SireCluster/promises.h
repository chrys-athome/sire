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

#ifndef SIRECLUSTER_PROMISES_H
#define SIRECLUSTER_PROMISES_H

#include "promise.h"

SIRE_BEGIN_HEADER

namespace SireCluster
{

/** This class holds a collection of promises, ordered 
    in the same order as the workpackets that were 
    submitted to be run
    
    @author Christopher Woods
*/
class SIRECLUSTER_EXPORT Promises
        : public Siren::ImplementsHandle< Promises,Siren::Handles< QVector<Promise> > >
{
public:
    Promises();
    Promises(const Promises &other);
    
    ~Promises();
    
    Promises& operator=(const Promises &other);
    
    bool operator==(const Promises &other) const;
    bool operator!=(const Promises &other) const;
    
    Promise operator[](int i) const;
    
    Promise at(int i) const;

    bool isEmpty() const;
    
    int nPromises() const;
    int count() const;
    
    QString toString() const;
    uint hashCode() const;
    
    void wait();
    bool wait(int ms);
    
    static Promises runLocal(const QList<WorkPacketPtr> &workpackets);
    static Promises runLocal(const QVector<WorkPacketPtr> &workpackets);

protected:
    friend class resources::WorkQueue;
    Promises(const QVector<Promise> &promises);
};

}

Q_DECLARE_METATYPE( SireCluster::Promises )

SIRE_EXPOSE_CLASS( SireCluster::Promises )

SIRE_END_HEADER

#endif
