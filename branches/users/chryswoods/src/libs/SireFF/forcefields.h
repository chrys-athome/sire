/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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

#ifndef SIREFF_FORCEFIELDS_H
#define SIREFF_FORCEFIELDS_H

#include "forcefieldsbase.h"

SIRE_BEGIN_HEADER

namespace SireFF
{
class ForceFields;
}

QDataStream& operator<<(QDataStream&, const SireFF::ForceFields&);
QDataStream& operator>>(QDataStream&, SireFF::ForceFields&);

namespace SireFF
{

/** This is the default ForceFields class - this is the default
    class used to hold a collection of ForceFields, all of which
    will be evaluated using the local processor and local thread.

    @author Christopher Woods
*/
class SIREFF_EXPORT ForceFields : public ForceFieldsBase
{

friend QDataStream& ::operator<<(QDataStream&, const ForceFields&);
friend QDataStream& ::operator>>(QDataStream&, ForceFields&);

public:
    ForceFields();

    ForceFields(const QList<ForceField> &ffields);
    ForceFields(const QHash<ForceFieldID,ForceField> &ffields);

    ForceFields(const ForceFieldsBase &other);
    ForceFields(const ForceFields &other);

    ~ForceFields();

    ForceFields& operator=(const ForceFieldsBase &other);
    ForceFields& operator=(const ForceFields &other);

    bool add(const ForceField &ffield);
    bool change(const ForceField &ffield);

    bool remove(ForceFieldID ffid);
    bool remove(const QString &ffname);

    void add(const FFExpression &ff_equation);
    void add(const QVector<FFExpression> &ff_equations);

    void remove(const Function &function);
    void remove(const QSet<Function> &functions);

    void remove(const FFExpression &ff_equation);
    void remove(const QVector<FFExpression> &ff_equations);

    QHash<ForceFieldID,ForceField> forceFields() const;

    QSet<ForceFieldID> forceFieldIDs() const;

    bool setProperty(ForceFieldID ffid, const QString &name,
                     const Property &property);

    bool setProperty(const QString &name, const Property &property);

    bool setProperty(const QSet<ForceFieldID> &ffids,
                     const QString &name, const Property &property);

    QHash< QString, QHash<ForceFieldID,Property> > properties() const;

    void mustNowRecalculateFromScratch();

    bool change(const PartialMolecule &molecule);
    bool change(const QHash<MoleculeID,PartialMolecule> &molecules);

    bool addTo( ForceFieldID ffid, const FFBase::Group &group,
                const PartialMolecule &molecule,
                const ParameterMap &map = ParameterMap() );

    bool removeFrom(ForceFieldID ffid, const FFBase::Group &group,
                    const PartialMolecule &molecule);

    bool refersTo(MoleculeID molid, ForceFieldID ffid,
                  const FFBase::Group &group) const;

    QSet<FFGroupID> forceFieldGroupsReferringTo(MoleculeID molid) const;

    QSet<MoleculeID> moleculeIDs(ForceFieldID ffid,
                                 const FFBase::Group &group) const;

    PartialMolecule molecule(MoleculeID molid, ForceFieldID ffid,
                             const FFBase::Group &group) const;

    void assertValidComponents(const FFExpression &expression) const;
    void assertSameContents(const ForceFieldsBase &other) const;

protected:
    void reindex();

    void _pvt_copy(const ForceFieldsBase &ffields);

    QSet<ForceFieldID> getFFIDs(const QString &ffname) const;

    Values getEnergies(ForceFieldID ffid);
    Values getEnergies(const QSet<ForceFieldID> &ffids);
    Values getEnergies(const QSet<FFComponent> &components);

    double getTotalEnergy();

    double getEnergy(const FFComponent &component);

private:
    ForceField& getForceField(ForceFieldID ffid);
    const ForceField& getForceField(ForceFieldID ffid) const;

    const ForceField& getConstForceField(ForceFieldID ffid) const;

    /** All of the forcefields in this group, indexed by their ID */
    QHash<ForceFieldID, ForceField> ffields;
};

}

Q_DECLARE_METATYPE(SireFF::ForceFields);

SIRE_END_HEADER

#endif
