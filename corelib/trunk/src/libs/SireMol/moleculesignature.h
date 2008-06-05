#ifndef SIREMOL_MOLECULESIGNATURE_H
#define SIREMOL_MOLECULESIGNATURE_H

#include <QHash>
#include <QString>
#include <QSet>

#include "moleculebonds.h"
#include "residueidset.h"

SIRE_BEGIN_HEADER

typedef QSet<QString> QStringSet;

namespace SireMol
{
class MoleculeSignature;
}

QDataStream& operator<<(QDataStream&, const SireMol::MoleculeSignature&);
QDataStream& operator>>(QDataStream&, SireMol::MoleculeSignature&);

uint qHash(const SireMol::MoleculeSignature&);

namespace SireMol
{

/**
This class contains the signature of a molecule. The signature is the the set of information that uniquely identifies the molecule, e.g. its name, the names and numbers of its residues, the names of all of the atoms, and which atoms are bonded together. It does *not* contain any information about the coordinates of the atoms, nor their element types, nor about how the atoms are ordered in the molecule or stored in memory. The signature can be used to see if two Molecule or EditMol objects are describing the same type of molecule.

@author Christopher Woods
*/
class SIREMOL_EXPORT MoleculeSignature
{

friend QDataStream& ::operator<<(QDataStream&, const MoleculeSignature&);
friend QDataStream& ::operator>>(QDataStream&, MoleculeSignature&);

public:
    MoleculeSignature();
    
    MoleculeSignature(const QString &name, 
                      const QHash<ResidueID, QStringSet> &atminfo,
                      const MoleculeBonds &bonding = MoleculeBonds());
                      
    MoleculeSignature(const MoleculeSignature &other);
    MoleculeSignature(const MoleculeSignature &other, const MoleculeBonds &bonding);
    
    ~MoleculeSignature();

    bool operator==(const MoleculeSignature &other) const;
    bool operator!=(const MoleculeSignature &other) const;

    QString toString() const;

private:
    
    /** The name of the molecule */
    QString molname;
    
    /** The names of all of the atoms in the molecule
        indexed by ResidueID */
    QHash<ResidueID, QStringSet> atominfo;
    
    /** The bonding of the molecule */
    MoleculeBonds bondinfo;
};

}

Q_DECLARE_METATYPE(SireMol::MoleculeSignature)

SIRE_END_HEADER

#endif
