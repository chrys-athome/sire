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
#include <QSet>

#include "SireBase/property.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class ConnectivityBase;
}

QDataStream& operator<<(QDataStream&, const SireMol::ConnectivityBase&);
QDataStream& operator>>(QDataStream&, SireMol::ConnectivityBase&);

namespace SireMol
{

class ConnectivityEditor;

/** The base class of Connectivity and ConnectivityEditor

    @author Christopher Woods
*/
class SIREMOL_EXPORT ConnectivityBase
{

friend QDataStream& ::operator<<(QDataStream&, const SireMol::ConnectivityBase&);
friend QDataStream& ::operator>>(QDataStream&, SireMol::ConnectivityBase&);

public:
    ~ConnectivityBase();

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

protected:
    ConnectivityBase();
    ConnectivityBase(const MoleculeInfo &info);
    
    ConnectivityBase(const ConnectivityBase &other);

    ConnectivityBase& operator=(const ConnectivityBase &other);

    bool operator==(const ConnectivityBase &other) const;
    bool operator!=(const ConnectivityBase &other) const;

    /** The which atoms are connected to which other atoms
        in this molecule */
    QVector< QSet<AtomIdx> > connected_atoms;
    
    /** Which residues are connected to which other residues */
    QVector< QSet<ResIdx> > connected_res;
    
    /** The info object that describes the molecule */
    MoleculeInfo molinfo;
   
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
class SIREMOL_EXPORT Connectivity : public ConnectivityBase
{

public:
    Connectivity();
    
    Connectivity(const MoleculeInfo &molinfo);

    Connectivity(const ConnectivityEditor &editor);
    Connectivity(const Connectivity &other);

    ~Connectivity();

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
class SIREMOL_EXPORT ConnectivityEditor : public ConnectivityBase
{
public:
    ConnectivityEditor();
    
    ConnectivityEditor(const Connectivity &connectivity);
    
    ConnectivityEditor(const ConnectivityEditor &other);
    
    ~ConnectivityEditor();
    
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


};

}

Q_DECLARE_METATYPE(SireMol::ConnectivityBase);
Q_DECLARE_METATYPE(SireMol::Connectivity);
Q_DECLARE_METATYPE(SireMol::ConnectivityEditor);

SIRE_END_HEADER

#endif
