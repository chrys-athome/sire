#ifndef SIREMOL_RESIDUEBONDS_H
#define SIREMOL_RESIDUEBONDS_H

#include <QSharedDataPointer>

#include <QList>
#include <QString>

SIRE_BEGIN_HEADER

namespace SireMol
{
class ResidueBonds;
}

class QDataStream;
QDataStream& operator<<(QDataStream&, const SireMol::ResidueBonds&);
QDataStream& operator>>(QDataStream&, SireMol::ResidueBonds&);

class QString;

namespace SireMol
{

class ResidueBondsPvt;

class MoleculeBonds;
class AtomIndex;
class Bond;
class ResNum;

/**
This class contains information about all of the bonds that are in a residue. This class is intended to be used as a child of a MoleculeBonds object, which contains the set of ResidueBonds for each residue in the molecule.

Note that these bonds are *not energetic bonds*, they are just structural
bonds that specify how one atom is connected to another atom. It is not possible for
the connectivity of a molecule to change with lambda, as the connectivity is merely
used to move the skeleton of the molecule, not to calculate the bond energy. If you
wish to 'break' a bond as part of a perturbation, then you do so by turning off
the bond energy function, not by removing the connecting bond (as how would you
account for the change in the number of degrees of freedom across lambda, and
at which point along lambda will the bond be on, or off?)

Note also that the bond names refer to the names of the skeleton atoms of perturbing
molecules.

This class is implicitly shared, so is fast to copy.

@author Christopher Woods
*/
class SIREMOL_EXPORT ResidueBonds
{

friend class MoleculeBonds;
friend QDataStream& ::operator<<(QDataStream&, const ResidueBonds&);
friend QDataStream& ::operator>>(QDataStream&, ResidueBonds&);

public:
    ResidueBonds();
    ResidueBonds(ResNum resnum);
    ResidueBonds(const ResidueBonds &other);

    ~ResidueBonds();

    ResidueBonds& operator=(const ResidueBonds &other);

    bool operator==(const ResidueBonds &other) const;
    bool operator!=(const ResidueBonds &other) const;

    bool isEmpty() const;

    QString toString() const;

    ResNum resNum() const;

    QList<ResNum> residuesBondedTo() const;
    QList<ResNum> residuesBondedTo(const QString &atom) const;

    bool bondedTo(ResNum resnum) const;
    bool bonded(const AtomIndex &atom0, const AtomIndex &atom1) const;

    QList<Bond> interBonds() const;
    QList<Bond> intraBonds() const;
    QList<Bond> asymmetricBonds() const;
    QList<Bond> bonds() const;

    int nBonds() const;
    int nAsymmetricBonds() const;

    int nIntraBonds() const;
    int nInterBonds() const;

    QList<Bond> bonds(const QString &atomname) const;
    QList<Bond> interBonds(const QString &atomname) const;
    QList<Bond> intraBonds(const QString &atomname) const;

    QList<Bond> bonds(ResNum resnum) const;

    QSet<AtomIndex> anchors() const;
    QSet<AtomIndex> atoms() const;

    bool contains(const QString &atom) const;
    bool contains(const Bond &bond) const;

private:

  //// Functions only accessible to MoleculeBonds////////
    void add(const Bond &bond);
    void add(const QString &atom0, const QString &atom1);
    void add(const AtomIndex &atom0, const AtomIndex &atom1);

    void remove(const AtomIndex &atom);
    void remove(const QString &atom);

    void remove(const Bond &bond);
    void remove(ResNum resnum);
    void remove(const QString &atom0, const QString &atom1);
    void remove(const AtomIndex &atom0, const AtomIndex &atom1);

    void finalise();
    void clear();
  ///////////////////////////////////////////////////////

    QSharedDataPointer<ResidueBondsPvt> d;
};

}

Q_DECLARE_METATYPE(SireMol::ResidueBonds)

SIRE_END_HEADER

#endif
