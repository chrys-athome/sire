/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2014  Christopher Woods
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

#ifndef SIREMM_INTERGROUPFF_H
#define SIREMM_INTERGROUPFF_H

#include "cljcomponent.h"
#include "cljgrid.h"
#include "cljfunction.h"
#include "cljgroup.h"

#include "SireFF/g2ff.h"

SIRE_BEGIN_HEADER

namespace SireMM
{
class InterGroupFF;
}

QDataStream& operator<<(QDataStream&, const SireMM::InterGroupFF&);
QDataStream& operator>>(QDataStream&, SireMM::InterGroupFF&);

namespace SireMM
{

namespace detail
{
class InterGroupFFData;
}

/** This is a forcefield that calculates the intermolecular coulomb
    and Lennard Jones (LJ) energy between all of the molecules in 
    group 0 and all of the molecules in group 1.
 
    @author Christopher Woods
*/
class SIREMM_EXPORT InterGroupFF : public SireBase::ConcreteProperty<InterGroupFF,SireFF::G2FF>
{

friend QDataStream& ::operator<<(QDataStream&, const SireMM::InterGroupFF&);
friend QDataStream& ::operator>>(QDataStream&, SireMM::InterGroupFF&);

public:
    InterGroupFF();
    InterGroupFF(const QString &name);
    
    InterGroupFF(const InterGroupFF &other);
    
    ~InterGroupFF();
    
    static const char* typeName();
    
    const char* what() const;
    
    InterGroupFF& operator=(const InterGroupFF &other);
    
    bool operator==(const InterGroupFF &other) const;
    bool operator!=(const InterGroupFF &other) const;

    InterGroupFF* clone() const;

    const CLJComponent& components() const;

    void setCLJFunction(const CLJFunction &cljfunc);
    const CLJFunction& cljFunction() const;

    void enableParallelCalculation();
    void disableParallelCalculation();
    void setUseParallelCalculation(bool on);
    bool usesParallelCalculation() const;

    void enableReproducibleCalculation();
    void disableReproducibleCalculation();
    void setUseReproducibleCalculation(bool on);
    bool usesReproducibleCalculation() const;

    bool setProperty(const QString &name, const Property &property);
    const Property& property(const QString &name) const;
    bool containsProperty(const QString &name) const;
    const Properties& properties() const;

    void mustNowRecalculateFromScratch();    

    void accept();
    bool needsAccepting() const;

private:
    void mustNowReallyRecalculateFromScratch();

    void recalculateEnergy();
    void rebuildProps();

    void _pvt_added(quint32 groupid,
                    const PartialMolecule &mol,
                    const PropertyMap &map);

    void _pvt_removed(quint32 groupid,
                      const PartialMolecule &mol);

    void _pvt_changed(quint32 groupid,
                      const Molecule &molecule,
                      bool auto_commit);
                              
    void _pvt_changed(quint32 groupid,
                      const QList<Molecule> &molecules,
                      bool auto_commit);
    
    void _pvt_removedAll(quint32 groupid);
        
    bool _pvt_wouldChangeProperties(quint32 groupid,
                                    MolNum molnum,
                                    const PropertyMap &map) const;

    void _pvt_updateName();

    /** The CLJGroup containing all of the extracted molecules
        in the two groups */
    CLJGroup cljgroup[2];

    /** Implicitly shared pointer to the (mostly) const data for this forcefield */
    QSharedDataPointer<detail::InterGroupFFData> d;
    
    /** Whether or not we need to 'accept' this move */
    bool needs_accepting;
};

}

Q_DECLARE_METATYPE( SireMM::InterGroupFF )

SIRE_EXPOSE_CLASS( SireMM::InterGroupFF )

SIRE_END_HEADER

#endif
