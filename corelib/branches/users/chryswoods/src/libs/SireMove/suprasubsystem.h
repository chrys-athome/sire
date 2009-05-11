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

#ifndef SIREMOVE_SUPRASUBSYSTEM_H
#define SIREMOVE_SUPRASUBSYSTEM_H

#include "SireBase/property.h"

#include "simstore.h"

SIRE_BEGIN_HEADER

namespace SireMove
{
class SupraSubSystem;
}

QDataStream& operator<<(QDataStream&, const SireMove::SupraSubSystem&);
QDataStream& operator>>(QDataStream&, SireMove::SupraSubSystem&);

namespace SireMove
{

/** This is the base class of all of the sub-systems that make up
    each SupraSystem. A SupraSystem is a collection of systems
    that together make up a supra-ensemble (e.g. a collection of 
    replicas that make up a single replica exchange ensemble).
    
    Each SupraSubSystem consists of the SireSystem::System object
    that contains the molecule info of the subsystem, and the 
    SireMove::Moves object that contains all of the sub-moves
    that are to be applied to the sub-system during each
    supra-move
    
    @author Christopher Woods
*/
class SIREMOVE_EXPORT SupraSubSystem 
        : public SireBase::ConcreteProperty<SupraSubSystem,SireBase::Property>
{

friend QDataStream& ::operator<<(QDataStream&, const SireMove::SupraSubSystem&);
friend QDataStream& ::operator>>(QDataStream&, SireMove::SupraSubSystem&);

public:
    SupraSubSystem();
    
    SupraSubSystem(const SupraSubSystem &other);
    
    virtual ~SupraSubSystem();
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<SupraSubSystem>() );
    }
    
    virtual SupraSubSystem* clone() const
    {
        return new SupraSubSystem(*this);
    }
    
    SupraSubSystem& operator=(const SupraSubSystem &other);
    
    bool operator==(const SupraSubSystem &other) const;
    bool operator!=(const SupraSubSystem &other) const;
    
    const System& subSystem() const;
    const Moves& subMoves() const;

    const SimStore& subSystemAndMoves() const;
    
    int nSubMoves() const;
    bool recordingSubStatistics() const;
    
    bool isPacked() const;
    bool isPackedToDisk() const;
    bool isPackedToMemory() const;
    
    void pack();
    void unpack();
    
    void packToDisk();
    void packToDisk(const QString &tempdir);
    
    void packToMemory();

    virtual void subMove();

    virtual void clearStats();
    virtual void clearSubStats();
    
    virtual void collectStats();

    void mustNowRecalculateFromScratch();

protected:
    virtual void setSubSystem(const System &subsystem);
    virtual void setSubMoves(const Moves &submoves);

    virtual void setSubSystemAndMoves(const SimStore &simstore);

    virtual void setNSubMoves(int n);
    
    virtual void setRecordSubStatistics(bool record_stats);

    virtual void _pre_pack();
    virtual void _post_pack();
    
    virtual void _pre_unpack();
    virtual void _post_unpack();

private:
    /** The store containing both the sub-system and sub-moves */
    SimStore simstore;
    
    /** The number of sub-moves to apply to the sub-system for each
        supra-move */
    quint32 nsubmoves;
    
    /** Whether or not to record sub-system statistics during the sub-moves */
    bool record_sub_stats;
    
    /** Whether or not we need to recalculate the next energy from scratch */
    bool recalc_next_from_scratch;
};

typedef SireBase::PropPtr<SupraSubSystem> SupraSubSystemPtr;

}

Q_DECLARE_METATYPE( SireMove::SupraSubSystem )

SIRE_EXPOSE_CLASS( SireMove::SupraSubSystem )

SIRE_EXPOSE_PROPERTY( SireMove::SupraSubSystemPtr, SireMove::SupraSubSystem )

SIRE_END_HEADER

#endif
