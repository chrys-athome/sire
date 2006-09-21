#ifndef SIREMOL_EDITRES_H
#define SIREMOL_EDITRES_H

#include "editmoldata.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class EditRes;
}

class QDataStream;
QDataStream& operator<<(QDataStream&, const SireMol::EditRes&);
QDataStream& operator>>(QDataStream&, SireMol::EditRes&);

uint qHash(const SireMol::EditRes &editres);

namespace SireMol
{

class TemplateFunction;
class Element;

/**
This class represents an editable residue within an EditMol. This contains functions
to add and query the atoms and connectivity of this residue.

This class is implemented as a view of the EditMolData class, which is used as the
underlying private object of both EditMol and EditRes.

Note that this class is reentrant, though not thread-safe.

@author Christopher Woods
*/
class SIREMOL_EXPORT EditRes
{

friend class EditMolData;  // friend so that it can call the EditMolData constructor
friend QDataStream& ::operator<<(QDataStream&, const EditRes&);
friend QDataStream& ::operator>>(QDataStream&, EditRes&);

public:
    EditRes(ResNum resnum=1, const QString &resname=QObject::tr("UNK"));
    EditRes(const QString &resnam);
    EditRes(const EditRes &other);

    ~EditRes();

   //// Operators /////////////////////////////////
    bool operator==(const EditRes &other) const;
    bool operator!=(const EditRes &other) const;

    EditRes& operator=(const EditRes &other);

    const Atom& operator[](int i) const;
    const Atom& operator[](const QString &nam) const;

    operator ResNum() const;
   ////////////////////////////////////////////////


   //// Memory management /////////////////////////
    void detach();
    EditRes deepCopy() const;
    EditRes shallowCopy();

    EditRes extract() const;
    EditRes extract(ResNum newnum) const;
   ////////////////////////////////////////////////


   //// Query functions ///////////////////////////
    QString name() const;
    QString toString() const;

    ResNum number() const;
    ResNum resNum() const;

    EditMol molecule();

    const Atom& at(int i) const;

    int count() const;
    int size() const;
    int nAtoms() const;

    QStringList atomNames() const;

    int nBonds() const;
    ResidueBonds connectivity() const;

    bool contains(const QString &nam) const;
    bool contains(const AtomIndex &atom) const;

    const Atom& atom(const QString &nam) const;
    const Atom& atom(int i) const;

    SireMaths::Line bond(const QString &atom0, const QString &atom1) const;
    SireMaths::Triangle angle(const QString &atom0, const QString &atom1,
                              const QString &atom2) const;
    SireMaths::Torsion dihedral(const QString &atom0, const QString &atom1,
                                const QString &atom2, const QString &atom3) const;
    SireMaths::Torsion improper(const QString &atom0, const QString &atom1,
                                const QString &atom2, const QString &atom3) const;

    double measure(const QString &atom0, const QString &atom1) const;
    SireMaths::Angle measure(const QString &atom0, const QString &atom1,
                             const QString &atom2) const;
    SireMaths::Angle measure(const QString &atom0, const QString &atom1,
                             const QString &atom2, const QString &atom3) const;
    SireMaths::Angle measureImproper(const QString &atom0, const QString &atom1,
                                     const QString &atom2, const QString &atom3) const;

    QVector<Atom> atoms() const;
    QVector<Vector> coordinates() const;

    QList<Bond> bonds() const;
    QList<Bond> bonds(const QString &atom) const;

    QHash<ResNum,EditRes> residuesBondedTo();
   ////////////////////////////////////////////////


   //// Editing functions /////////////////////////
    void setName(QString name);
    void setNumber(ResNum newnum);

    void clear();
    void finalise();

    void add(const QString &atm);
    void add(const QStringList &atoms);

    void add(const Atom &atm);
    void add(const QList<Atom> &atoms);
    void add(const QVector<Atom> &atoms);

    void add(const EditRes &editres);
    void merge(const EditRes &editres);

    void update(const Atom &atm);
    void update(const QString &nam, const Vector &coords);
    void update(const QString &nam, const Element &element);
    void update(const QString &nam, const Atom &newatom);

    void remove(const QString &atm);
    void remove(const QStringList &atoms);
    void remove(const AtomIndex &atm);
    void remove(const QSet<AtomIndex> &atoms);
    void remove(const QList<AtomIndex> &atoms);

    void addBond(const QString &atmnam0, const QString &atmnam1);
    void removeBond(const QString &atmnam0, const QString &atmnam1);

    void add(const Bond &bond);
    void remove(const Bond &bond);

    void add(const QList<Bond> &bonds);
    void remove(const QList<Bond> &bonds);

    void addAutoBonds();

    void removeAllBonds(const QString &atomname);
    void removeAllBonds();

    void applyTemplate(const EditRes &tmpl, const TemplateFunction &tmplfunc);
   ////////////////////////////////////////////////


   //// Moving the residue ////////////////////////
    void translate(const Vector &delta);
    void translate(const QString &atom, const Vector &delta);
    void translate(const QStringList &atoms, const Vector &delta);

    void rotate(const Quaternion &quat, const Vector &point);
    void rotate(const QString &atom, const Quaternion &quat, const Vector &point);
    void rotate(const QStringList &atoms, const Quaternion &quat, const Vector &point);
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
    EditRes(const EditMolData &moldata, ResNum resnum);

    void checkAtom(const QString &nam) const;

    /** The object containing the actual data of the residue (and molecule!) */
    EditMolData moldata;

    /** The residue number of this residue */
    ResNum rnum;
};

}

Q_DECLARE_METATYPE(SireMol::EditRes)

SIRE_END_HEADER

#endif
