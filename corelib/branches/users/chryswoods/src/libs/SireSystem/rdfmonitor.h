/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2008  Christopher Woods
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

#ifndef SIRESYSTEM_RDFMONITOR_H
#define SIRESYSTEM_RDFMONITOR_H

#include "systemmonitor.h"

SIRE_BEGIN_HEADER

namespace SireSystem
{
class RDFMonitor;
}

QDataStream& operator<<(QDataStream&, const SireSystem::RDFMonitor&);
QDataStream& operator>>(QDataStream&, SireSystem::RDFMonitor&);

namespace SireSystem
{

/** This monitor collects the radial distribution function (RDF)
    between two sets of atoms
    
    @author Christopher Woods
*/
class SIRESYSTEM_EXPORT RDFMonitor 
         : public SireBase::ConcreteProperty<RDFMonitor,SystemMonitor>
{

friend QDataStream& ::operator<<(QDataStream&, const RDFMonitor&);
friend QDataStream& ::operator>>(QDataStream&, RDFMonitor&);

public:
    RDFMonitor();
    
    RDFMonitor(const RDFMonitor &other);
    
    ~RDFMonitor();
    
    RDFMonitor& operator=(const RDFMonitor &other);
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<RDFMonitor>() );
    }
    
    RDFMonitor* clone() const
    {
        return new RDFMonitor(*this);
    }
    
    bool operator==(const RDFMonitor &other) const;
    bool operator!=(const RDFMonitor &other) const;
    
    void add(const AtomID &atom0, const AtomID &atom1) const;

    void setRange(double min, double max, int nbins);
    void setRange(double min, double max, double binwidth);
    
    double minimum() const;
    double maximum() const;
    
    double binWidth() const;
    
    int nBins() const;
    
    
};

}

Q_DECLARE_METATYPE( SireSystem::RDFMonitor )

SIRE_EXPOSE_CLASS( SireSystem::RDFMonitor )

SIRE_END_HEADER

#endif
