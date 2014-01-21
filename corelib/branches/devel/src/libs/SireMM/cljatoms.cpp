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

#include <QElapsedTimer>

#include "cljatoms.h"

#include "atomljs.h"

#include "SireMol/atomcoords.h"
#include "SireMol/atomcharges.h"
#include "SireMol/molecules.h"
#include "SireMol/molecule.h"
#include "SireMol/partialmolecule.h"
#include "SireMol/selector.hpp"
#include "SireMol/atom.h"

#include "SireID/index.h"

#include "SireError/errors.h"

#include "SireUnits/units.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMM;
using namespace SireMol;
using namespace SireStream;

///////
/////// Implementation of CLJAtom
///////

static const RegisterMetaType<CLJAtom> r_cljatom(NO_ROOT);

QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const CLJAtom &cljatom)
{
    writeHeader(ds, r_cljatom, 1);
    
    ds << cljatom.x << cljatom.y << cljatom.z
       << cljatom.chg << cljatom.sig << cljatom.eps
       << cljatom.idnum;
    
    return ds;
}

QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, CLJAtom &cljatom)
{
    VersionID v = readHeader(ds, r_cljatom);
    
    if (v == 1)
    {
        ds >> cljatom.x >> cljatom.y >> cljatom.z
           >> cljatom.chg >> cljatom.sig >> cljatom.eps
           >> cljatom.idnum;
    }
    else
        throw version_error(v, "1", r_cljatom, CODELOC);
    
    return ds;
}

/** Null constructor */
CLJAtom::CLJAtom() : x(0), y(0), z(0), chg(0), sig(0), eps(0), idnum(0)
{}

/** Construct from the passed coordinates, charge and LJ parameters */
CLJAtom::CLJAtom(Vector coords, Charge charge, LJParameter ljparam, qint32 atomid)
        : x(coords.x()), y(coords.y()), z(coords.z()),
          chg( charge.value() * std::sqrt(SireUnits::one_over_four_pi_eps0) ),
          sig( sqrt(ljparam.sigma()) ), eps( sqrt(4.0 * ljparam.epsilon()) ),
          idnum(atomid)
{}

/** Copy constructor */
CLJAtom::CLJAtom(const CLJAtom &other)
        : x(other.x), y(other.y), z(other.z), chg(other.chg),
          sig(other.sig), eps(other.eps), idnum(other.idnum)
{}

/** Destructor */
CLJAtom::~CLJAtom()
{}

/** Copy assignment operator */
CLJAtom& CLJAtom::operator=(const CLJAtom &other)
{
    if (this != &other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
        chg = other.chg;
        sig = other.sig;
        eps = other.eps;
        idnum = other.idnum;
    }
    
    return *this;
}

/** Comparison operator */
bool CLJAtom::operator==(const CLJAtom &other) const
{
    return x == other.x and
           y == other.y and
           z == other.z and
           chg == other.chg and
           sig == other.sig and
           eps == other.eps and
           idnum == other.idnum;
}

/** Comparison operator */
bool CLJAtom::operator!=(const CLJAtom &other) const
{
    return not operator==(other);
}

const char* CLJAtom::typeName()
{
    return QMetaType::typeName( qMetaTypeId<CLJAtom>() );
}

const char* CLJAtom::what() const
{
    return CLJAtom::typeName();
}

/** Return the coordinates of the atom */
Vector CLJAtom::coordinates() const
{
    return Vector(x,y,z);
}

/** Return the partial charge of the atom */
Charge CLJAtom::charge() const
{
    return Charge(chg / std::sqrt(SireUnits::one_over_four_pi_eps0) );
}

/** Return the LJ parameters of the atom */
LJParameter CLJAtom::ljParameter() const
{
    double s = sig * sig;
    double e = eps * eps;
    e /= 4.0;
    
    return LJParameter( SireUnits::Dimension::Length(s), SireUnits::Dimension::MolarEnergy(e) );
}

/** Return the ID number for the atom */
qint32 CLJAtom::ID() const
{
    return idnum;
}

///////
/////// Implementation of CLJAtoms
///////

static const RegisterMetaType<CLJAtoms> r_cljatoms(NO_ROOT);

QDataStream SIREMM_EXPORT &operator<<(QDataStream &ds, const CLJAtoms &cljatoms)
{
    writeHeader(ds, r_cljatoms, 1);
    
    SharedDataStream sds(ds);
    sds << cljatoms.atoms();
    
    return ds;
}

QDataStream SIREMM_EXPORT &operator>>(QDataStream &ds, CLJAtoms &cljatoms)
{
    VersionID v = readHeader(ds, r_cljatoms);
    
    if (v == 1)
    {
        SharedDataStream sds(ds);
        
        QVector<CLJAtom> atoms;
        
        sds >> atoms;
        
        cljatoms = CLJAtoms(atoms);
    }
    else
        throw version_error(v, "1", r_cljatoms, CODELOC);
    
    return ds;
}

static float castFromInt(qint32 value, int check=1);
static qint32 castFromFloat(float value, int check=1);

static qint32 castFromFloat(float value, int check)
{
    qint32 ivalue = *(reinterpret_cast<qint32*>(&value));

    qDebug() << "Converted" << value << "to" << ivalue;
    
    if (check)
        qDebug() << "Checking" << ivalue << "is" << castFromInt(ivalue, 0);
    
    return ivalue;
}

static float castFromInt(qint32 value, int check)
{
    float fvalue = *(reinterpret_cast<float*>(&value));
    
    qDebug() << "Converted" << value << "to" << fvalue;
    
    if (check)
        qDebug() << "Checking" << fvalue << "is" << castFromFloat(fvalue, 0);
    
    return fvalue;
}

/** Return a MultiFloat of the ID of a dummy atom */
MultiFloat CLJAtoms::idOfDummy()
{
    return MultiFloat( castFromInt(0) );
}

/** Null constructor */
CLJAtoms::CLJAtoms()
{}

/** Construct from the passed array of CLJAtom atoms */
CLJAtoms::CLJAtoms(const QVector<CLJAtom> &atoms)
{
    if (atoms.isEmpty())
        return;
    
    //vectorise all of the parameters
    QVector<float> xf(atoms.count());
    QVector<float> yf(atoms.count());
    QVector<float> zf(atoms.count());
    
    QVector<float> cf(atoms.count());
    QVector<float> sf(atoms.count());
    QVector<float> ef(atoms.count());
    
    QVector<float> idf(atoms.count());
    
    float *xa = xf.data();
    float *ya = yf.data();
    float *za = zf.data();
    
    float *ca = cf.data();
    float *sa = sf.data();
    float *ea = ef.data();
    
    float *ida = idf.data();
    
    const CLJAtom *atms = atoms.constData();
    
    for (int i=0; i<atoms.count(); ++i)
    {
        const CLJAtom &atm = atms[i];
    
        xa[i] = atm.x;
        ya[i] = atm.y;
        za[i] = atm.z;
        ca[i] = atm.chg;
        sa[i] = atm.sig;
        ea[i] = atm.eps;
        ida[i] = castFromInt(atm.idnum);
    }
    
    _x = MultiFloat::fromArray(xf);
    _y = MultiFloat::fromArray(yf);
    _z = MultiFloat::fromArray(zf);
    _q = MultiFloat::fromArray(cf);
    _sig = MultiFloat::fromArray(sf);
    _eps = MultiFloat::fromArray(ef);
    _id = MultiFloat::fromArray(idf);
}

/** Construct from the passed set of coordinates, partial charges and LJ parameters.
    Each atom is assumed to be part of the same molecule, with atom ID 'atomid' */
CLJAtoms::CLJAtoms(const QVector<Vector> &coordinates,
                   const QVector<Charge> &charges,
                   const QVector<LJParameter> &ljparams,
                   qint32 atomid)
{
    if (coordinates.count() != charges.count() or
        coordinates.count() != ljparams.count())
    {
        throw SireError::incompatible_error( QObject::tr(
                "You cannot construct a set of CLJAtoms where the number of coordinates "
                "(%1), charges (%2) and Lennard Jones parameters (%3) are different!")
                    .arg(coordinates.count())
                    .arg(charges.count())
                    .arg(ljparams.count()), CODELOC );
    }

    //vectorise the parameters, and convert to float as we can control
    //precision in the energy calculation
    {
        QVector<float> xf(coordinates.count());
        QVector<float> yf(coordinates.count());
        QVector<float> zf(coordinates.count());
        
        QVector<float> cf(charges.count());
        QVector<float> sf(ljparams.count());
        QVector<float> ef(ljparams.count());
        
        QVector<float> idf(coordinates.count());
        
        float *xa = xf.data();
        float *ya = yf.data();
        float *za = zf.data();
        
        float *ca = cf.data();
        float *sa = sf.data();
        float *ea = ef.data();
        
        float *ida = idf.data();
        float id_float = castFromInt(atomid);
        
        const Vector *coords = coordinates.constData();
        const Charge *chgs = charges.constData();
        const LJParameter *ljs = ljparams.constData();
        
        for (int i=0; i<coordinates.count(); ++i)
        {
            xa[i] = coords[i].x();
            ya[i] = coords[i].y();
            za[i] = coords[i].z();
            
            ca[i] = chgs[i].value();
            sa[i] = ljs[i].sigma();
            ea[i] = ljs[i].epsilon();
            
            ida[i] = id_float;
        }
        
        _x = MultiFloat::fromArray(xf);
        _y = MultiFloat::fromArray(yf);
        _z = MultiFloat::fromArray(zf);
        
        _q = MultiFloat::fromArray(cf);
        _sig = MultiFloat::fromArray(sf);
        _eps = MultiFloat::fromArray(ef);
        
        _id = MultiFloat::fromArray(idf);
    }
    
    MultiFloat *q = _q.data();
    MultiFloat *s = _sig.data();
    MultiFloat *e = _eps.data();
    
    const MultiFloat four(4.0);
    const MultiFloat one_over_4_pi_eps_0( std::sqrt(SireUnits::one_over_four_pi_eps0) );
    
    //now reduce the charge, sigma and epsilon parameters
    for (int i=0; i<_q.count(); ++i)
    {
        q[i] = q[i] * one_over_4_pi_eps_0;
        s[i] = s[i].sqrt();
        e[i] = (e[i] * four).sqrt();
    }
}

/** Construct from the passed set of coordinates, partial charges, LJ parameters
    and atom IDs */
CLJAtoms::CLJAtoms(const QVector<Vector> &coordinates,
                   const QVector<Charge> &charges,
                   const QVector<LJParameter> &ljparams,
                   const QVector<qint32> &atomids)
{
    if (coordinates.count() != charges.count() or
        coordinates.count() != ljparams.count() or
        coordinates.count() != atomids.count())
    {
        throw SireError::incompatible_error( QObject::tr(
                "You cannot construct a set of CLJAtoms where the number of coordinates "
                "(%1), charges (%2), Lennard Jones parameters (%3) or "
                "atom IDs (%4) are different!")
                    .arg(coordinates.count())
                    .arg(charges.count())
                    .arg(ljparams.count())
                    .arg(atomids.count()), CODELOC );
    }

    //vectorise the parameters, and convert to float as we can control
    //precision in the energy calculation
    {
        QVector<float> xf(coordinates.count());
        QVector<float> yf(coordinates.count());
        QVector<float> zf(coordinates.count());
        
        QVector<float> cf(charges.count());
        QVector<float> sf(ljparams.count());
        QVector<float> ef(ljparams.count());
        
        QVector<float> idf(atomids.count());
        
        float *xa = xf.data();
        float *ya = yf.data();
        float *za = zf.data();
        
        float *ca = cf.data();
        float *sa = sf.data();
        float *ea = ef.data();
        
        float *ida = idf.data();
        
        const Vector *coords = coordinates.constData();
        const Charge *chgs = charges.constData();
        const LJParameter *ljs = ljparams.constData();
        const qint32 *atmids = atomids.constData();
        
        for (int i=0; i<coordinates.count(); ++i)
        {
            xa[i] = coords[i].x();
            ya[i] = coords[i].y();
            za[i] = coords[i].z();
            
            ca[i] = chgs[i].value();
            sa[i] = ljs[i].sigma();
            ea[i] = ljs[i].epsilon();
            
            ida[i] = castFromInt(atmids[i]);
        }
        
        _x = MultiFloat::fromArray(xf);
        _y = MultiFloat::fromArray(yf);
        _z = MultiFloat::fromArray(zf);
        
        _q = MultiFloat::fromArray(cf);
        _sig = MultiFloat::fromArray(sf);
        _eps = MultiFloat::fromArray(ef);
        
        _id = MultiFloat::fromArray(idf);
    }
    
    MultiFloat *q = _q.data();
    MultiFloat *s = _sig.data();
    MultiFloat *e = _eps.data();
    
    const MultiFloat four(4.0);
    const MultiFloat one_over_4_pi_eps_0( std::sqrt(SireUnits::one_over_four_pi_eps0) );
    
    //now reduce the charge, sigma and epsilon parameters
    for (int i=0; i<_q.count(); ++i)
    {
        q[i] = q[i] * one_over_4_pi_eps_0;
        s[i] = s[i].sqrt();
        e[i] = (e[i] * four).sqrt();
    }
}

/** Construct from the parameters in the passed set of Molecules */
void CLJAtoms::constructFrom(const Molecules &molecules, const PropertyMap &map)
{
    if (molecules.isEmpty())
        return;
   
    QElapsedTimer t;
    t.start();
    
    //extract all of the data from the passed molecules
    {
        const PropertyName coords_property = map["coordinates"];
        const PropertyName chg_property = map["charge"];
        const PropertyName lj_property = map["LJ"];
        
        //calculate the number of atoms...
        int nats = 0;
        
        for (Molecules::const_iterator it = molecules.constBegin();
             it != molecules.constEnd();
             ++it)
        {
            nats += it.value().join().nAtoms();
        }
        
        //reserve space for the data
        QVector<float> xf(nats);
        QVector<float> yf(nats);
        QVector<float> zf(nats);
        
        QVector<float> qf(nats);
        QVector<float> sigf(nats);
        QVector<float> epsf(nats);
        
        QVector<float> idf(nats);
        
        float *xa = xf.data();
        float *ya = yf.data();
        float *za = zf.data();
        
        float *qa = qf.data();
        float *siga = sigf.data();
        float *epsa = epsf.data();
        
        float *ida = idf.data();
        
        int idx = 0;
        
        for (Molecules::const_iterator it = molecules.constBegin();
             it != molecules.constEnd();
             ++it)
        {
            Selector<Atom> atoms = it.value().atoms();
            
            QList<Vector> coords = atoms.property<Vector>(coords_property);
            QList<Charge> chgs = atoms.property<Charge>(chg_property);
            QList<LJParameter> ljs = atoms.property<LJParameter>(lj_property);
            
            if (coords.count() != chgs.count() or
                coords.count() != ljs.count())
            {
                throw SireError::program_bug( QObject::tr(
                        "It should not be possible for the number of coordinates (%1) "
                        "to be different to the number of charges (%2) or LJs (%3) "
                        "for molecule %4.")
                            .arg(coords.count())
                            .arg(chgs.count())
                            .arg(ljs.count())
                            .arg(atoms.molecule().toString()), CODELOC );
            }
            
            for (int i=0; i<coords.count(); ++i)
            {
                xa[idx] = coords[i].x();
                ya[idx] = coords[i].y();
                za[idx] = coords[i].z();
                
                qa[idx] = chgs[i].value();
                siga[idx] = ljs[i].sigma();
                epsa[idx] = ljs[i].epsilon();
                
                quint32 molid = it.key().value();
                ida[idx] = castFromInt( *(reinterpret_cast<qint32*>(&molid)) );
                
                idx += 1;
            }
        }
        
        _x = MultiFloat::fromArray(xf);
        _y = MultiFloat::fromArray(yf);
        _z = MultiFloat::fromArray(zf);
        _q = MultiFloat::fromArray(qf);
        _sig = MultiFloat::fromArray(sigf);
        _eps = MultiFloat::fromArray(epsf);
        _id = MultiFloat::fromArray(idf);
    }
    
    MultiFloat *q = _q.data();
    MultiFloat *s = _sig.data();
    MultiFloat *e = _eps.data();
    
    const MultiFloat four(4.0);
    const MultiFloat one_over_4_pi_eps_0( std::sqrt(SireUnits::one_over_four_pi_eps0) );
    
    //now reduce the charge, sigma and epsilon parameters
    for (int i=0; i<_q.count(); ++i)
    {
        q[i] = q[i] * one_over_4_pi_eps_0;
        s[i] = s[i].sqrt();
        e[i] = (e[i] * four).sqrt();
    }
    
    quint64 ns = t.nsecsElapsed();
    
    qDebug() << "Converting" << (_q.count() * MultiFloat::count()) << "atoms took"
             << (0.000001*ns) << "ms";
}

/** Construct from the parameters in the passed molecule view */
CLJAtoms::CLJAtoms(const MoleculeView &view, const PropertyMap &map)
{
    constructFrom(Molecules(view), map);
}

/** Construct from the parameters in the passed set of Molecules */
CLJAtoms::CLJAtoms(const Molecules &molecules, const PropertyMap &map)
{
    constructFrom(molecules, map);
}

/** Copy constructor */
CLJAtoms::CLJAtoms(const CLJAtoms &other)
         : _x(other._x), _y(other._y), _z(other._z),
           _q(other._q), _sig(other._sig), _eps(other._eps),
           _id(other._id)
{}

/** Destructor */
CLJAtoms::~CLJAtoms()
{}

/** Copy assignment operator */
CLJAtoms& CLJAtoms::operator=(const CLJAtoms &other)
{
    if (this != &other)
    {
        _x = other._x;
        _y = other._y;
        _z = other._z;
        _q = other._q;
        _sig = other._sig;
        _eps = other._eps;
        _id = other._id;
    }
    
    return *this;
}

/** Comparison operator */
bool CLJAtoms::operator==(const CLJAtoms &other) const
{
    return this == &other or
           (_x == other._x and _y == other._y and _z == other._z and
            _q == other._q and _sig == other._sig and _eps == other._eps and
            _id == other._id);
}

/** Comparison operator */
bool CLJAtoms::operator!=(const CLJAtoms &other) const
{
    return not operator==(other);
}

const char* CLJAtoms::typeName()
{
    return QMetaType::typeName( qMetaTypeId<CLJAtoms>() );
}

const char* CLJAtoms::what() const
{
    return CLJAtoms::typeName();
}

/** Return whether or not this array is empty */
bool CLJAtoms::isEmpty() const
{
    return _x.isEmpty();
}

/** Return the number of atoms in this set. Note that vectorisation
    may mean that the array of atoms has been padded with dummy atoms */
int CLJAtoms::count() const
{
    return MultiFloat::size() * _x.count();
}

/** Return the number of atoms in this set. Note that vectorisation
    may mean that the array of atoms has been padded with dummy atoms */
int CLJAtoms::size() const
{
    return count();
}

/** Return the ith atom in the vector */
CLJAtom CLJAtoms::operator[](int i) const
{
    i = SireID::Index(i).map(count());
    
    int idx = i / MultiFloat::count();
    int sub_idx = i % MultiFloat::count();
    
    CLJAtom atom;
    atom.x = _x[idx][sub_idx];
    atom.y = _y[idx][sub_idx];
    atom.z = _z[idx][sub_idx];
    atom.chg = _q[idx][sub_idx];
    atom.sig = _sig[idx][sub_idx];
    atom.eps = _eps[idx][sub_idx];
    atom.idnum = castFromFloat(_id[idx][sub_idx]);
    
    return atom;
}

/** Return the ith atom in the vector */
CLJAtom CLJAtoms::at(int i) const
{
    return operator[](i);
}

/** Return the ith atom in the vector */
CLJAtom CLJAtoms::getitem(int i) const
{
    return operator[](i);
}

/** Overwrite the atom at index i with the data in 'atom' */
void CLJAtoms::set(int i, const CLJAtom &atom)
{
    i = SireID::Index(i).map(count());
    
    int idx = i / MultiFloat::count();
    int sub_idx = i % MultiFloat::count();
    
    _x[idx].set(sub_idx, atom.x);
    _y[idx].set(sub_idx, atom.y);
    _z[idx].set(sub_idx, atom.z);
    _q[idx].set(sub_idx, atom.chg);
    _sig[idx].set(sub_idx, atom.sig);
    _eps[idx].set(sub_idx, atom.eps);
    _id[idx].set(sub_idx, castFromInt(atom.idnum));
}

/** Set the coordinates of the ith atom to 'coords' */
void CLJAtoms::setCoordinates(int i, Vector coords)
{
    i = SireID::Index(i).map(count());
    
    int idx = i / MultiFloat::count();
    int sub_idx = i % MultiFloat::count();
    
    _x[idx].set(sub_idx, coords.x());
    _y[idx].set(sub_idx, coords.y());
    _z[idx].set(sub_idx, coords.z());
}

/** Set the charge of the ith atom to 'charge' */
void CLJAtoms::setCharge(int i, Charge charge)
{
    float c = charge.value() * std::sqrt(SireUnits::one_over_four_pi_eps0);
    
    int idx = i / MultiFloat::count();
    int sub_idx = i % MultiFloat::count();

    _q[idx].set(sub_idx, c);
}

/** Set the LJ parameter of the ith atom to 'ljparam' */
void CLJAtoms::setLJParameter(int i, LJParameter ljparam)
{
    float s = ljparam.sigma();
    float e = ljparam.epsilon() * 4.0;
    
    int idx = i / MultiFloat::count();
    int sub_idx = i % MultiFloat::count();

    _sig[idx].set(sub_idx, sqrt(s));
    _eps[idx].set(sub_idx, sqrt(e));
}

/** Set the ID number for the ith atom to 'idnum' */
void CLJAtoms::setID(int i, qint32 idnum)
{
    int idx = i / MultiFloat::count();
    int sub_idx = i % MultiFloat::count();

    _id[idx].set(sub_idx, castFromInt(idnum));
}

/** Make the ith atom into a dummy atom (set the atom ID to 0) */
void CLJAtoms::makeDummy(int i)
{
    int idx = i / MultiFloat::count();
    int sub_idx = i % MultiFloat::count();
    _id[idx].set(sub_idx, castFromInt(0));
}

/** Return whether or not the ith atom is a dummy atom (has an ID of 0) */
bool CLJAtoms::isDummy(int i)
{
    int idx = i / MultiFloat::count();
    int sub_idx = i % MultiFloat::count();

    return castFromFloat(_id[idx][sub_idx]) == 0;
}

/** Return an array of all of the atoms */
QVector<CLJAtom> CLJAtoms::atoms() const
{
    if (this->isEmpty())
        return QVector<CLJAtom>();

    QVector<CLJAtom> atms( _x.count() * MultiFloat::count() );
    CLJAtom *a = atms.data();
    
    int idx = 0;
    
    for (int i=0; i<_x.count(); ++i)
    {
        const MultiFloat &xf = _x[i];
        const MultiFloat &yf = _y[i];
        const MultiFloat &zf = _z[i];
        const MultiFloat &qf = _q[i];
        const MultiFloat &sigf = _sig[i];
        const MultiFloat &epsf = _eps[i];
        const MultiFloat &idf = _id[i];
        
        for (int j=0; j<MultiFloat::count(); ++j)
        {
            CLJAtom &atom = a[idx];
            ++idx;
            
            atom.x = xf[j];
            atom.y = yf[j];
            atom.z = zf[j];
            atom.chg = qf[j];
            atom.sig = sigf[j];
            atom.eps = epsf[j];
            atom.idnum = castFromFloat(idf[j]);
        }
    }
    
    return atms;
}

/** Return the coordinates of all of the atoms */
QVector<Vector> CLJAtoms::coordinates() const
{
    if (this->isEmpty())
        return QVector<Vector>();

    QVector<Vector> coords( _x.count() * MultiFloat::count() );
    Vector *c = coords.data();
    
    int idx = 0;
    
    for (int i=0; i<_x.count(); ++i)
    {
        const MultiFloat &xf = _x[i];
        const MultiFloat &yf = _y[i];
        const MultiFloat &zf = _z[i];
        
        for (int j=0; j<MultiFloat::count(); ++j)
        {
            c[idx] = Vector(xf[j], yf[j], zf[j]);
            ++idx;
        }
    }
    
    return coords;
}

/** Return the charges on all of the atoms */
QVector<Charge> CLJAtoms::charges() const
{
    if (this->isEmpty())
        return QVector<Charge>();

    QVector<Charge> chgs( _x.count() * MultiFloat::count() );
    Charge *c = chgs.data();
    
    int idx = 0;
    
    for (int i=0; i<_x.count(); ++i)
    {
        const MultiFloat &qf = _q[i];
        
        for (int j=0; j<MultiFloat::count(); ++j)
        {
            c[idx] = Charge( qf[j] / std::sqrt(SireUnits::one_over_four_pi_eps0) );
            ++idx;
        }
    }
    
    return chgs;
}

/** Return the LJ parameters on all of the atoms */
QVector<LJParameter> CLJAtoms::ljParameters() const
{
    if (this->isEmpty())
        return QVector<LJParameter>();

    QVector<LJParameter> ljs( _x.count() * MultiFloat::count() );
    LJParameter *lj = ljs.data();
    
    int idx = 0;
    
    for (int i=0; i<_x.count(); ++i)
    {
        const MultiFloat &sigf = _sig[i];
        const MultiFloat &epsf = _eps[i];
        
        for (int j=0; j<MultiFloat::count(); ++j)
        {
            double sig = sigf[j] * sigf[j];
            double eps = epsf[j] * epsf[j];
            
            lj[idx] = LJParameter(SireUnits::Dimension::Length(sig),
                                  SireUnits::Dimension::MolarEnergy(eps / 4.0));
            ++idx;
        }
    }
    
    return ljs;
}

/** Return the IDs of all of the atoms */
QVector<qint32> CLJAtoms::IDs() const
{
    if (this->isEmpty())
        return QVector<qint32>();

    QVector<qint32> ids( _id.count() * MultiFloat::count() );
    qint32 *idval = ids.data();

    int idx = 0;

    for (int i=0; i<_id.count(); ++i)
    {
        const MultiFloat &idf = _id[i];

        for (int j=0; j<MultiFloat::count(); ++j)
        {
            idval[idx] = castFromFloat(idf[j]);
            ++idx;
        }
    }

    return ids;
}
