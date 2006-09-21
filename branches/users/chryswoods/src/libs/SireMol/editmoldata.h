#ifndef SIREMOL_EDITMOLDATA_H
#define SIREMOL_EDITMOLDATA_H

#include <QSharedDataPointer>
#include <QString>
#include <QHash>
#include <QObject>
#include <QSet>

#include <boost/shared_ptr.hpp>

#include "atomindex.h"

SIRE_BEGIN_HEADER

namespace SireMaths
{
class Vector;
class Quaternion;
class Line;
class Triangle;
class Torsion;
class Angle;
}

namespace SireMol
{
class EditMolData;
}

class QDataStream;
QDataStream& operator<<(QDataStream&, const SireMol::EditMolData&);
QDataStream& operator>>(QDataStream&, SireMol::EditMolData&);

namespace SireMol
{

class EditMolDataPvt;
typedef boost::shared_ptr< QSharedDataPointer<EditMolDataPvt> > EditMolDataPtr;

class Element;

class EditMol;
class EditRes;
class Bond;
class Angle;
class Dihedral;
class Improper;

class WeightFunction;

class Molecule;
class MoleculeBonds;
class ResidueBonds;
class AtomIDGroup;
class MoleculeSignature;

using SireMaths::Vector;
using SireMaths::Quaternion;

/**
This class holds the actual data for an EditMol. This class is used internally by EditMol and EditRes, as these two classes are actually merely viewer classes which present different views of EditMolData.

@author Christopher Woods
*/
class EditMolData
{

friend class EditMolDataPvt;

friend QDataStream& ::operator<<(QDataStream&, const EditMolData&);
friend QDataStream& ::operator>>(QDataStream&, EditMolData&);

public:
    EditMolData(const QString &molname = QObject::tr("Unnamed"));
    EditMolData(const EditMolData &other);

    EditMolData(const Molecule &mol);

    ~EditMolData();

   //////// Operators //////////////////////////////
    EditMolData& operator=(const EditMolData &other);

    bool operator==(const EditMolData &other) const;
    bool operator!=(const EditMolData &other) const;
   /////////////////////////////////////////////////

   //////// Editing the molecule ///////////////////
    void setName(const QString &name);
    void setResidueName(ResNum resnum, const QString &name);

    void clear();
    void clean();
    void clearResidue(ResNum resnum);

    void renumberResidue(ResNum resnum, ResNum newresnum);

    void finalise();

    void add(const Atom &atom);
    void remove(const AtomIndex &atom);

    void add(const Bond &bond);
    void remove(const Bond &bond);

    void removeAllBonds(ResNum resnum);
    void removeAllBonds(const AtomIndex &atom);
    void removeAllBonds();

    void update(const Atom &atom);
    void update(const AtomIndex &atm, const Atom &details);
    void update(const AtomIndex &atm, const Vector &coords);
    void update(const AtomIndex &atm, const Element &element);

    EditRes addResidue(ResNum resnum, const QString &resnam);
    EditRes addResidue(ResNum resnum, const EditRes &tmpl);
    EditRes addResidue(ResNum resnum, const QString &resnam, const EditRes &tmpl);

    void removeResidue(ResNum resnum);
    void removeResidue(const QString &resnam);
    void removeAllResidues(const QString &resnam);
   /////////////////////////////////////////////////


   //////// Querying the molecule //////////////////
    MoleculeBonds connectivity() const;
    ResidueBonds connectivity(ResNum resnum) const;

    QVector<Atom> atoms() const;
    QVector<Vector> coordinates() const;

    QVector<Atom> atoms(ResNum resnum) const;
    QVector<Vector> coordinates(ResNum resnum) const;

    const QString& name() const;
    QString residueName(ResNum resnum) const;

    bool isEmpty() const;
    bool isEmpty(ResNum resnum) const;

    QList<ResNum> residueNumbers() const;
    QStringList residueNames() const;

    QList<ResNum> residueNumbers(const QString &resnam) const;

    QHash<ResNum,EditRes> residuesBondedTo(ResNum resnum);

    const Atom& atom(const AtomIndex &atm) const;

    bool contains(ResNum resnum) const;
    bool contains(const AtomIndex &atm) const;
    bool contains(const Bond &bond) const;

    const Atom& at(ResNum resnum, int i) const;

    int nAtoms() const;
    int nAtoms(ResNum resnum) const;

    int nResidues() const;

    QStringList atomNames(ResNum resnum) const;

    SireMaths::Line bond(const Bond &bnd) const;
    SireMaths::Triangle angle(const SireMol::Angle &ang) const;
    SireMaths::Torsion dihedral(const Dihedral &dih) const;
    SireMaths::Torsion improper(const Improper &improper) const;

    double measure(const Bond &bnd) const;
    SireMaths::Angle measure(const SireMol::Angle &ang) const;
    SireMaths::Angle measure(const Dihedral &dih) const;
    SireMaths::Angle measure(const Improper &improper) const;

    double getWeight(const AtomIDGroup &group0, const AtomIDGroup &group1,
                     const WeightFunction &weightfunc) const;
   /////////////////////////////////////////////////


   //////// Moving the molecule ////////////////////
    void translate(const Vector &delta);
    void translate(const AtomIndex &atom, const Vector &delta);
    void translate(const AtomIDGroup &group, const Vector &delta);
    void translate(const QSet<AtomIndex> &atoms, const Vector &delta);
    void translate(ResNum resnum, const QStringList &atoms, const Vector &delta);
    void translate(ResNum resnum, const Vector &delta);

    void rotate(const Quaternion &quat, const Vector &point);
    void rotate(const AtomIndex &atom, const Quaternion &quat, const Vector &point);
    void rotate(const AtomIDGroup &group, const Quaternion &quat, const Vector &point);
    void rotate(const QSet<AtomIndex> &atoms, const Quaternion &quat, const Vector &point);
    void rotate(ResNum resnum, const QStringList &atoms, const Quaternion &quat,
                const Vector &point);
    void rotate(ResNum resnum, const Quaternion &quat, const Vector &point);
   /////////////////////////////////////////////////


   //////// Internal geometry moves ////////////////
    void change(const Bond &bnd, double delta,
                const AtomIDGroup &group0, const AtomIDGroup &group1,
                const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors);

    void change(const SireMol::Angle &ang, const SireMaths::Angle &delta,
                const AtomIDGroup &group0, const AtomIDGroup &group1,
                const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors);

    void change(const Bond &bnd, const SireMaths::Angle &delta,
                const AtomIDGroup &group0, const AtomIDGroup &group1,
                const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors);

    void change(const Improper &improper, const SireMaths::Angle &delta,
                const AtomIDGroup &group0, const AtomIDGroup &group1,
                const WeightFunction &weightfunc, const QSet<AtomIndex> &anchors);
   /////////////////////////////////////////////////

private:
    const EditMolDataPvt& d() const;
    EditMolDataPvt& d();

    EditMolDataPtr ptr;
};

}

SIRE_END_HEADER

#endif
