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

#ifndef SIREMOL_CONNECTIVITY_H
#define SIREMOL_CONNECTIVITY_H

#include <QHash>
#include <QVector>
#include <QSet>

#include <boost/tuple/tuple.hpp>

#include "SireBase/property.h"
#include "SireBase/shareddatapointer.hpp"

SIRE_BEGIN_HEADER

namespace SireMol
{
class ConnectivityBase;
}

QDataStream& operator<<(QDataStream&, const SireMol::ConnectivityBase&);
QDataStream& operator>>(QDataStream&, SireMol::ConnectivityBase&);

namespace SireMol
{

using boost::tuple;

class AtomIdx;
class ResIdx;

class AtomID;
class ResID;

class MoleculeData;
class MoleculeInfoData;

class ConnectivityEditor;

/** The base class of Connectivity and ConnectivityEditor

    @author Christopher Woods
*/
class SIREMOL_EXPORT ConnectivityBase : public SireBase::PropertyBase
{

friend QDataStream& ::operator<<(QDataStream&, const SireMol::ConnectivityBase&);
friend QDataStream& ::operator>>(QDataStream&, SireMol::ConnectivityBase&);

public:
    ~ConnectivityBase();

    static const char* typeName()
    {
        return "SireMol::ConnectivityBase";
    }

    bool areConnected(AtomIdx atom0, AtomIdx atom1) const;
    bool areConnected(const AtomID &atom0, const AtomID &atom1) const;

    bool areConnected(ResIdx res0, ResIdx res1) const;
    bool areConnected(const ResID &res0, const ResID &res1) const;

    int nConnections() const;
    int nConnections(AtomIdx atomidx) const;
    int nConnections(const AtomID &atomid) const;

    int nConnections(ResIdx residx) const;
    int nConnections(const ResID &resid) const;

    int nConnections(ResIdx res0, ResIdx res1) const;
    int nConnections(const ResID &res0, const ResID &res1) const;

    const QSet<AtomIdx>& connectionsTo(AtomIdx atomidx) const;
    const QSet<AtomIdx>& connectionsTo(const AtomID &atomid) const;

    const QSet<ResIdx>& connectionsTo(ResIdx residx) const;
    const QSet<ResIdx>& connectionsTo(const ResID &resid) const;

    //anchors should be a separate consideration to the SPLIT
    // - should not have anchors in here - they should be in mover.cpp

    tuple<AtomSelection,AtomSelection> split(AtomIdx atom0, AtomIdx atom1) const;
    tuple<AtomSelection,AtomSelection> split(const AtomID &atom0,
                                             const AtomID &atom1) const;

    tuple<AtomSelection,AtomSelection> split(const BondID &bond) const;

    tuple<AtomSelection,AtomSelection>
    split(AtomIdx atom0, AtomIdx atom1, const AtomSelection &movable_atoms,
          const AtomSelection &anchors = AtomSelection()) const;

    tuple<AtomSelection,AtomSelection>
    split(const AtomID &atom0, const AtomID &atom1,
          const AtomSelection &movable_atoms,
          const AtomSelection &anchors = AtomSelection()) const;

    tuple<AtomSelection,AtomSelection>
    split(const BondID &bond, const AtomSelection &movable_atoms,
          const AtomSelection &anchors = AtomSelection()) const;

    tuple<AtomSelection,AtomSelection> split(AtomIdx atom0, AtomIdx atom1,
                                             AtomIdx atom2) const;
    tuple<AtomSelection,AtomSelection> split(const AtomID &atom0,
                                             const AtomID &atom1,
                                             const AtomID &atom2) const;

    tuple<AtomSelection,AtomSelection> split(const AngleID &angle) const;

    tuple<AtomSelection,AtomSelection>
    split(AtomIdx atom0, AtomIdx atom1, AtomIdx atom2,
          const AtomSelection &movable_atoms,
          const AtomSelection &anchors = AtomSelection()) const;

protected:
    ConnectivityBase();
    ConnectivityBase(const MoleculeData &moldata);

    ConnectivityBase(const ConnectivityBase &other);

    ConnectivityBase& operator=(const ConnectivityBase &other);

    bool operator==(const ConnectivityBase &other) const;
    bool operator!=(const ConnectivityBase &other) const;

    inline const MoleculeInfoData& info() const;

    /** The which atoms are connected to which other atoms
        in this molecule */
    QVector< QSet<AtomIdx> > connected_atoms;

    /** Which residues are connected to which other residues */
    QVector< QSet<ResIdx> > connected_res;

    /** The info object that describes the molecule */
    SireBase::SharedDataPointer<MoleculeInfoData> d;

};

/** This class contains the connectivity of the molecule, namely which
atoms are connected to which other atoms. The connectivity is used
to move parts of the molecule (e.g. moving an atom also moves all
of the atoms that it is connected to), and to automatically generate
the internal geometry of the molecule (e.g. to auto-generate
all of the bonds, angles and dihedrals). Note that the connectivity
is not the same as the bonding - the connectivity is used to move
parts of the molecule (e.g. moving an atom should move all of the
atoms it is connected to) and also to auto-generate internal angles
(e.g. auto-generation of bonds, angles and dihedrals)

    @author Christopher Woods

*/
class SIREMOL_EXPORT Connectivity
            : public SireBase::ConcreteProperty<Connectivity,
                                                ConnectivityBase>
{

public:
    Connectivity();

    Connectivity(const MoleculeData &moldata);

    Connectivity(const ConnectivityEditor &editor);
    Connectivity(const Connectivity &other);

    ~Connectivity();

    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<Connectivity>() );
    }

    using SireBase::PropertyBase::operator=;
    using SireBase::PropertyBase::operator==;
    using SireBase::PropertyBase::operator!=;

    Connectivity& operator=(const Connectivity &other);
    Connectivity& operator=(const ConnectivityEditor &editor);

    bool operator==(const Connectivity &other) const;
    bool operator!=(const Connectivity &other) const;

    ConnectivityEditor edit() const;

private:
    void squeeze();
};

/** An editor that can be used to edit a Connectivity object

    @author Christopher Woods
*/
class SIREMOL_EXPORT ConnectivityEditor
        : public SireBase::ConcreteProperty<ConnectivityEditor,
                                            ConnectivityBase>
{
public:
    ConnectivityEditor();

    ConnectivityEditor(const Connectivity &connectivity);

    ConnectivityEditor(const ConnectivityEditor &other);

    ~ConnectivityEditor();

    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<ConnectivityEditor>() );
    }

    using SireBase::PropertyBase::operator=;
    using SireBase::PropertyBase::operator==;
    using SireBase::PropertyBase::operator!=;

    ConnectivityEditor& operator=(const ConnectivityBase &other);

    bool operator==(const ConnectivityEditor &other) const;
    bool operator!=(const ConnectivityEditor &other) const;

    ConnectivityEditor& connect(AtomIdx atom0, AtomIdx atom1);
    ConnectivityEditor& disconnect(AtomIdx atom0, AtomIdx atom1);

    ConnectivityEditor& connect(const AtomID &atom0, const AtomID &atom1);
    ConnectivityEditor& disconnect(const AtomID &atom0, const AtomID &atom1);

    ConnectivityEditor& disconnectAll(AtomIdx atomidx);
    ConnectivityEditor& disconnectAll(ResIdx residx);

    ConnectivityEditor& disconnectAll(const AtomID &atomid);
    ConnectivityEditor& disconnectAll(const ResID &resid);
};

}

Q_DECLARE_METATYPE(SireMol::Connectivity);
Q_DECLARE_METATYPE(SireMol::ConnectivityEditor);

SIRE_END_HEADER

#endif
