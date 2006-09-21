#ifndef SIREMOL_EDITMOL_H
#define SIREMOL_EDITMOL_H

#include "editmoldata.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class EditMol;
}

class QDataStream;
QDataStream& operator<<(QDataStream &ds, const SireMol::EditMol &editmol);
QDataStream& operator>>(QDataStream &ds, SireMol::EditMol &editmol);

uint qHash(const SireMol::EditMol &editmol);

namespace SireMol
{

class TemplateFunction;
class EditRes;
class Molecule;

/**
An EditMol is a molecule that may be edited. It is not based on CutGroup, but is instead optimised to have atoms and bonds easily added or removed, and its entire data structure is designed for rapid molecular editing. An EditMol represents an entire, single molecule, and it may be converted into any CutGroup that represents an entire molecule. In addition, any CutGroup that contains an entire molecule may be converted into an EditMol. This is the way that you edit molecules, convert from a CutGroup to an EditMol, edit the molecule, and then convert back again.

An EditMol consists of a set of EditRes (each representing a residue), each consisting of Atoms. The EditMol may also contain Bonds that connect Atoms together.

Note that the functions in this class are re-entrant, though this class is not thread-safe.

The implementation of EditMol and EditRes is as views onto a common EditMolData class. This
class is explicitly shared between the different EditRes and EditMol views. In addition, the
data of the EditMol is implicitly shared, so even deep copying an EditMol or EditRes is
fast.

@author Christopher Woods
*/
class SIREMOL_EXPORT EditMol
{

friend class EditMolData;  //friend so can call the EditMolData constructor
friend QDataStream& ::operator<<(QDataStream&, const EditMol&);
friend QDataStream& ::operator>>(QDataStream&, EditMol&);

public:
    EditMol(const QString &name = QString::null);
    EditMol(const Molecule &mol);

    EditMol(const EditMol &other);

    ~EditMol();

   ////// Operators ///////////////////////////////
    bool operator==(const EditMol &other) const;
    bool operator!=(const EditMol &other) const;

    EditMol& operator=(const EditMol &other);

    EditRes operator[](ResNum resnum);
   ////////////////////////////////////////////////


   ////// Memory management ///////////////////////
    void detach();
    EditMol deepCopy() const;
    EditMol shallowCopy();
   ////////////////////////////////////////////////


   ///// Query functions //////////////////////////
    QString name() const;

    QString toString() const;

    MoleculeSignature signature() const;

    int count() const;
    int size() const;
    int nResidues() const;

    int nBonds() const;

    EditRes at(int i);

    QString residueName(ResNum resnum) const;

    QList<ResNum> residueNumbers() const;
    QList<ResNum> residueNumbers(const QString &resnam) const;

    QStringList residueNames() const;

    bool isEmpty() const;
    int nAtoms() const;

    EditRes residue(ResNum resnum);
    EditRes residue(const QString &resname);

    QVector<Atom> atoms() const;
    QVector<Atom> atoms(ResNum resnum) const;

    QVector<Vector> coordinates() const;
    QVector<Vector> coordinates(ResNum resnum) const;

    bool contains(const AtomIndex &atm) const;
    bool contains(ResNum resnum) const;
    bool contains(const QString &resname) const;

    const Atom& atom(const AtomIndex &idx) const;

    SireMaths::Line bond(const Bond &bond) const;
    SireMaths::Triangle angle(const Angle &angle) const;
    SireMaths::Torsion dihedral(const Dihedral &dihedral) const;
    SireMaths::Torsion improper(const Improper &improper) const;

    double measure(const Bond &bond) const;
    SireMaths::Angle measure(const Angle &angle) const;
    SireMaths::Angle measure(const Dihedral &dihedral) const;
    SireMaths::Angle measure(const Improper &improper) const;

    QList<Bond> bonds() const;
    QList<Bond> bonds(const AtomIndex &atom) const;
    QList<Bond> bonds(ResNum resnum) const;

    MoleculeBonds connectivity() const;
    ResidueBonds connectivity(ResNum resnum) const;

    double getWeight(const AtomIDGroup &group0, const AtomIDGroup &group1) const;

    double getWeight(const AtomIDGroup &group0, const AtomIDGroup &group1,
                     const WeightFunction &weightfunc) const;

    SireMaths::Vector center() const;
   ////////////////////////////////////////////////


   //// Editing functions /////////////////////////
    void setName(const QString &name);
    void clear();

    ResNum getNewResidueNumber() const;

    EditRes addResidue(ResNum resnum, const QString &resnam);
    EditRes addResidue(ResNum resnum, const QString &resnam, const EditRes &tmpl);

    EditRes addResidue(const QString &resnam);
    EditRes addResidue(const QString &resnam, const EditRes &tmpl);

    EditRes addResidue(ResNum resnum);
    EditRes addResidue(ResNum resnum, const EditRes &tmpl);

    EditRes addResidue(const EditRes &tmpl);

    void removeResidue(ResNum resnum);
    void removeResidue(const QString &resnam);
    void removeAllResidues(const QString &resnam);

    void add(const Atom &atm);
    void add(const QList<Atom> &atoms);
    void add(const QVector<Atom> &atoms);

    void remove(const AtomIndex &atm);
    void remove(const QSet<AtomIndex> &atoms);
    void remove(const QList<AtomIndex> &atoms);

    void add(const Bond &bond);
    void remove(const Bond &bond);

    void add(const QList<Bond> &bonds);
    void remove(const QList<Bond> &bonds);

    void addAutoBonds();

    void update(const Atom &atom);
    void update(const AtomIndex &atm, const Vector &coords);
    void update(const AtomIndex &atm, const Element &element);
    void update(const AtomIndex &atm, const Atom &newatom);

    void removeAllBonds(ResNum resnum);
    void removeAllBonds(const AtomIndex &atm);
    void removeAllBonds();

    void applyTemplate(const EditRes &tmpl, ResNum resnum, const TemplateFunction &tmplfunc);

    void applyTemplate(const EditRes &tmpl, const QString &resnam,
                       const TemplateFunction &tmplfunc);

    void applyTemplate(const EditMol &tmpl, const TemplateFunction &tmplfunc);
   ////////////////////////////////////////////////


   //// Moving the molecule ///////////////////////
    void translate(const Vector &delta);
    void translate(const AtomIndex &atom, const Vector &delta);
    void translate(ResNum resnum, const Vector &delta);
    void translate(const AtomIDGroup &group, const Vector &delta);

    void rotate(const Quaternion &quat, const Vector &point);
    void rotate(const AtomIndex &atom, const Quaternion &quat, const Vector &point);
    void rotate(ResNum resnum, const Quaternion &quat, const Vector &point);
    void rotate(const AtomIDGroup &group, const Quaternion &quat, const Vector &point);
   ////////////////////////////////////////////////


   //// Internal geometry moves ///////////////////
    void change(const Bond &bond, double delta,
                const QSet<AtomIndex> &anchors = QSet<AtomIndex>());
    void change(const Bond &bond, double delta, const WeightFunction &func,
                const QSet<AtomIndex> &anchors = QSet<AtomIndex>());

    void change(const Angle &angle, const SireMaths::Angle &delta,
                const QSet<AtomIndex> &anchors = QSet<AtomIndex>());
    void change(const Angle &angle, const SireMaths::Angle &delta,
                const WeightFunction &func, const QSet<AtomIndex> &anchors = QSet<AtomIndex>());

    void change(const Dihedral &dihedral, const SireMaths::Angle &delta,
                const QSet<AtomIndex> &anchors = QSet<AtomIndex>());
    void change(const Dihedral &dihedral, const SireMaths::Angle &delta,
                const WeightFunction &func, const QSet<AtomIndex> &anchors = QSet<AtomIndex>());

    void change(const Bond &bond, const SireMaths::Angle &delta,
                const QSet<AtomIndex> &anchors = QSet<AtomIndex>());
    void change(const Bond &bond, const SireMaths::Angle &delta,
                const WeightFunction &func, const QSet<AtomIndex> &anchors = QSet<AtomIndex>());

    void change(const Improper &improper, const SireMaths::Angle &delta,
                const QSet<AtomIndex> &anchors = QSet<AtomIndex>());
    void change(const Improper &improper, const SireMaths::Angle &delta,
                const WeightFunction &func, const QSet<AtomIndex> &anchors = QSet<AtomIndex>());

    void set(const Bond &bond, double lgth, const QSet<AtomIndex> &anchors = QSet<AtomIndex>());
    void set(const Bond &bond, double lgth, const WeightFunction &func,
             const QSet<AtomIndex> &anchors = QSet<AtomIndex>());

    void set(const SireMol::Angle &angle, const SireMaths::Angle &ang,
             const QSet<AtomIndex> &anchors = QSet<AtomIndex>());
    void set(const SireMol::Angle &angle, const SireMaths::Angle &ang,
             const WeightFunction &func, const QSet<AtomIndex> &anchors = QSet<AtomIndex>());

    void set(const Dihedral &dihedral, const SireMaths::Angle &ang,
             const QSet<AtomIndex> &anchors = QSet<AtomIndex>());
    void set(const Dihedral &dihedral, const SireMaths::Angle &ang,
             const WeightFunction &func, const QSet<AtomIndex> &anchors = QSet<AtomIndex>());

    void setAll(const Dihedral &dihedral, const SireMaths::Angle &ang,
                const QSet<AtomIndex> &anchors = QSet<AtomIndex>());
    void setAll(const Dihedral &dihedral, const SireMaths::Angle &ang,
                const WeightFunction &func, const QSet<AtomIndex> &anchors = QSet<AtomIndex>());

    void set(const Improper &improper, const SireMaths::Angle &ang,
             const QSet<AtomIndex> &anchors = QSet<AtomIndex>());
    void set(const Improper &improper, const SireMaths::Angle &ang,
             const WeightFunction &func, const QSet<AtomIndex> &anchors = QSet<AtomIndex>());
   ////////////////////////////////////////////////

private:
    EditMol(const EditMolData &moldata);

    void addAutoBonds(const EditRes &res0, const EditRes &res1);

    /** Actual data of the molecule */
    EditMolData moldata;
};

}

Q_DECLARE_METATYPE(SireMol::EditMol)

SIRE_END_HEADER

#endif
