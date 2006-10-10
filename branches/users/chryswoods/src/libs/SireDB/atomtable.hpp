#ifndef SIREDB_ATOMTABLE_HPP
#define SIREDB_ATOMTABLE_HPP

#include "atomtable.h"
#include "parametergroup.hpp"

#include <QDebug>

#include "SireError/errors.h"
#include "SireDB/errors.h"

#include "SireStream/datastream.h"

SIRE_BEGIN_HEADER

namespace SireDB
{
template<class Param>
class AtomTableT;
}

template<class Param>
QDataStream& operator<<(QDataStream &ds, const SireDB::AtomTableT<Param>&);
template<class Param>
QDataStream& operator>>(QDataStream &ds, SireDB::AtomTableT<Param>&);

namespace SireDB
{

/** This template class provides the additional functionality for
    an AtomTable that is used to store parameters of type 'T'.

    @author Christopher Woods
*/
template<class Param>
class SIREDB_EXPORT AtomTableT : public AtomTable
{

friend QDataStream& ::operator<<<>(QDataStream&, const AtomTableT<Param>&);
friend QDataStream& ::operator>><>(QDataStream&, AtomTableT<Param>&);

public:
    typedef Param parameter_type;

    AtomTableT();
    AtomTableT(const MoleculeInfo &molinfo);

    AtomTableT(const AtomTableT<Param> &other);

    ~AtomTableT();

    AtomParameter<Param> at(const CGAtomID &cgatomid) const;
    AtomParameter<Param> at(const CGNumAtomID &cgatomid) const;
    AtomParameter<Param> at(const ResNumAtomID &resatomid) const;
    AtomParameter<Param> at(const ResIDAtomID &resatomid) const;
    AtomParameter<Param> at(const AtomIndex &atom) const;
    AtomParameter<Param> at(AtomID atomid) const;

    AtomParameter<Param> operator[](const CGAtomID &cgatomid) const;
    AtomParameter<Param> operator[](const CGNumAtomID &cgatomid) const;
    AtomParameter<Param> operator[](const ResNumAtomID &resatomid) const;
    AtomParameter<Param> operator[](const ResIDAtomID &resatomid) const;
    AtomParameter<Param> operator[](const AtomIndex &atom) const;
    AtomParameter<Param> operator[](AtomID atomid) const;

    AtomParameter<Param> value(const CGAtomID &cgatomid) const;
    AtomParameter<Param> value(const CGNumAtomID &cgatomid) const;
    AtomParameter<Param> value(const ResNumAtomID &resatomid) const;
    AtomParameter<Param> value(const ResIDAtomID &resatomid) const;
    AtomParameter<Param> value(const AtomIndex &atom) const;
    AtomParameter<Param> value(AtomID atomid) const;

    AtomParameter<Param> value(const CGAtomID &cgatomid, const Param &defaultValue) const;
    AtomParameter<Param> value(const CGNumAtomID &cgatomid, const Param &defaultValue) const;
    AtomParameter<Param> value(const ResNumAtomID &resatomid, const Param &defaultValue) const;
    AtomParameter<Param> value(const ResIDAtomID &resatomid, const Param &defaultValue) const;
    AtomParameter<Param> value(const AtomIndex &atom, const Param &defaultValue) const;
    AtomParameter<Param> value(AtomID atomid, const Param &defaultValue) const;

    Param parameter(const CGAtomID &cgatomid) const;
    Param parameter(const CGNumAtomID &cgatomid) const;
    Param parameter(const ResNumAtomID &resatomid) const;
    Param parameter(const ResIDAtomID &resatomid) const;
    Param parameter(const AtomIndex &atom) const;
    Param parameter(AtomID atomid) const;

    void setParameter(const CGAtomID &cgatomid, const Param &param);
    void setParameter(const CGNumAtomID &cgatomid, const Param &param);
    void setParameter(const ResNumAtomID &resatomid, const Param &param);
    void setParameter(const ResIDAtomID &resatomid, const Param &param);
    void setParameter(const AtomIndex &atom, const Param &param);
    void setParameter(AtomID atomid, const Param &param);

    void add(const TableBase &table);

    ParameterGroup<Param> parameterGroup(CutGroupID cgid) const;

    QVector< ParameterGroup<Param> > parameterGroups() const;

    QHash< CutGroupID,ParameterGroup<Param> >
        parameterGroups( const QSet<CutGroupID> &cgids ) const;
    QHash< CutGroupNum,ParameterGroup<Param> >
        parameterGroups( const QSet<CutGroupNum> &cgnums) const;

    QVector<Param> parameters() const;
    QVector<Param> parameters(ResNum resnum) const;
    QVector<Param> parameters(ResID resid) const;
    QVector<Param> parameters(CutGroupID cgid) const;
    QVector<Param> parameters(CutGroupNum cgnum) const;

    QHash< CutGroupID,QVector<Param> >
          parameters(const QSet<CutGroupID> &cgids) const;
    QHash< CutGroupNum,QVector<Param> >
          parameters(const QSet<CutGroupNum> &cgnums) const;

    QHash< ResNum,QVector<Param> >
          parameters(const QSet<ResNum> &resnums) const;
    QHash< ResID,QVector<Param> >
          parameters(const QSet<ResID> &resids) const;

    template<class C>
    void parameters(CutGroupID cgid, QVector<C> &params) const;
    template<class C>
    void parameters(CutGroupNum cgnum, QVector<C> &params) const;

    template<class C>
    void parameters(ResNum resnum, QVector<C> &params) const;
    template<class C>
    void parameters(ResID resid, QVector<C> &params) const;

    QVector< AtomParameter<Param> > atomParameters() const;
    QVector< AtomParameter<Param> > atomParameters(ResNum resnum) const;
    QVector< AtomParameter<Param> > atomParameters(ResID resid) const;
    QVector< AtomParameter<Param> > atomParameters(CutGroupID cgid) const;
    QVector< AtomParameter<Param> > atomParameters(CutGroupNum cgnum) const;

    QHash< CutGroupID,QVector< AtomParameter<Param> > >
          atomParameters(const QSet<CutGroupID> &cgids) const;
    QHash< CutGroupNum,QVector< AtomParameter<Param> > >
          atomParameters(const QSet<CutGroupNum> &cgnums) const;

    QHash< ResNum,QVector< AtomParameter<Param> > >
          atomParameters(const QSet<ResNum> &resnums) const;
    QHash< ResID,QVector< AtomParameter<Param> > >
          atomParameters(const QSet<ResID> &resids) const;

    template<class C>
    void parameters(CutGroupID cgid, QVector<C> &params) const;
    template<class C>
    void parameters(CutGroupNum cgnum, QVector<C> &params) const;

    template<class C>
    void parameters(ResNum resnum, QVector<C> &params) const;
    template<class C>
    void parameters(ResID resid, QVector<C> &params) const;

protected:
    int _unsafe_nAssignedParameters(const ResidueInfo &resinfo) const;
    int _unsafe_nAssignedParameters(CutGroupID cgid) const;

    int _unsafe_nMissingParameters(const ResidueInfo &resinfo) const;
    int _unsafe_nMissingParameters(CutGroupID cgid) const;

    bool _unsafe_assignedParameter(const CGAtomID &cgatomid) const;

    bool _unsafe_hasMissingParameters(const ResidueInfo &resinfo) const;
    bool _unsafe_hasMissingParameters(CutGroupID cgid) const;

    QSet<AtomIndex> _unsafe_missingParameters(const ResidueInfo &resinfo) const;
    QSet<AtomIndex> _unsafe_missingParameters(CutGroupID cgid) const;

    Param _unsafe_parameter(const CGAtomID &cgatomid) const;
    AtomParameter<Param> _unsafe_atomParameter(const CGAtomID &cgatomid) const;

    QVector<Param> _unsafe_parameters(const ResidueInfo &resinfo) const;
    QVector<Param> _unsafe_parameters(CutGroupID cgid) const;

    QVector< AtomParameter<Param> > _unsafe_atomParameters(
                                          const ResidueInfo &resinfo) const;
    QVector< AtomParameter<Param> > _unsafe_atomParameters(
                                          CutGroupID cgid) const;

    ParameterGroup<Param> _unsafe_parameterGroup(CutGroupID cgid) const;

    void _unsafe_clear(const ResidueInfo &resinfo);
    void _unsafe_clear(CutGroupID cgid);
    void _unsafe_clear(const CGAtomID &cgatomid);


private:
    /** Array of all of the parameter hashes. The first index in
        the CutGroupID, the second index is the AtomID (thus this
        pair of arrays is indexed via CGAtomID) */
    QVector< QHash<AtomID,Param> > params;

    /** The completed parameter groups - these become populated
        as the table is populated. */
    QHash< CutGroupID, ParameterGroup<Param> > paramgroups;
};

template<class Param>
AtomTableT<Param>::AtomTableT();

template<class Param>
AtomTableT<Param>::AtomTableT(const MoleculeInfo &molinfo);

template<class Param>
AtomTableT<Param>::AtomTableT(const AtomTableT<Param> &other);

template<class Param>
AtomTableT<Param>::~AtomTableT();

template<class Param>
int AtomTableT<Param>::_unsafe_nAssignedParameters(const ResidueInfo &resinfo) const;

template<class Param>
int AtomTableT<Param>::_unsafe_nAssignedParameters(CutGroupID cgid) const;

template<class Param>
int AtomTableT<Param>::_unsafe_nMissingParameters(const ResidueInfo &resinfo) const;

template<class Param>
int AtomTableT<Param>::_unsafe_nMissingParameters(CutGroupID cgid) const;

template<class Param>
bool AtomTableT<Param>::_unsafe_assignedParameter(const CGAtomID &cgatomid) const;

template<class Param>
bool AtomTableT<Param>::_unsafe_hasMissingParameters(const ResidueInfo &resinfo) const;

template<class Param>
bool AtomTableT<Param>::_unsafe_hasMissingParameters(CutGroupID cgid) const;

template<class Param>
QSet<AtomIndex> AtomTableT<Param>::_unsafe_missingParameters(const ResidueInfo &resinfo) const;

template<class Param>
QSet<AtomIndex> AtomTableT<Param>::_unsafe_missingParameters(CutGroupID cgid) const;

template<class Param>
QVector<Param> AtomTableT<Param>::_unsafe_parameters(const ResidueInfo &resinfo) const;

template<class Param>
QVector<Param> AtomTableT<Param>::_unsafe_parameters(CutGroupID cgid) const;

template<class Param>
ParameterGroup<Param> AtomTableT<Param>::_unsafe_parameterGroup(CutGroupID cgid) const;

template<class Param>
void AtomTableT<Param>::_unsafe_clear(const ResidueInfo &resinfo);

template<class Param>
void AtomTableT<Param>::_unsafe_clear(CutGroupID cgid);

template<class Param>
void AtomTableT<Param>::_unsafe_clear(const CGAtomID &cgatomid);

template<class Param>
AtomParameter<Param> AtomTableT<Param>::at(const CGAtomID &cgatomid) const;

template<class Param>
AtomParameter<Param> AtomTableT<Param>::at(const CGNumAtomID &cgatomid) const;

template<class Param>
AtomParameter<Param> AtomTableT<Param>::at(const ResNumAtomID &resatomid) const;

template<class Param>
AtomParameter<Param> AtomTableT<Param>::at(const ResIDAtomID &resatomid) const;

template<class Param>
AtomParameter<Param> AtomTableT<Param>::at(const AtomIndex &atom) const;

template<class Param>
AtomParameter<Param> AtomTableT<Param>::at(AtomID atomid) const;

template<class Param>
AtomParameter<Param> AtomTableT<Param>::operator[](const CGAtomID &cgatomid) const;

template<class Param>
AtomParameter<Param> AtomTableT<Param>::operator[](const CGNumAtomID &cgatomid) const;

template<class Param>
AtomParameter<Param> AtomTableT<Param>::operator[](const ResNumAtomID &resatomid) const;

template<class Param>
AtomParameter<Param> AtomTableT<Param>::operator[](const ResIDAtomID &resatomid) const;

template<class Param>
AtomParameter<Param> AtomTableT<Param>::operator[](const AtomIndex &atom) const;

template<class Param>
AtomParameter<Param> AtomTableT<Param>::operator[](AtomID atomid) const;

template<class Param>
AtomParameter<Param> AtomTableT<Param>::value(const CGAtomID &cgatomid) const;

template<class Param>
AtomParameter<Param> AtomTableT<Param>::value(const CGNumAtomID &cgatomid) const;

template<class Param>
AtomParameter<Param> AtomTableT<Param>::value(const ResNumAtomID &resatomid) const;

template<class Param>
AtomParameter<Param> AtomTableT<Param>::value(const ResIDAtomID &resatomid) const;

template<class Param>
AtomParameter<Param> AtomTableT<Param>::value(const AtomIndex &atom) const;

template<class Param>
AtomParameter<Param> AtomTableT<Param>::value(AtomID atomid) const;

template<class Param>
AtomParameter<Param> AtomTableT<Param>::value(const CGAtomID &cgatomid, const Param &defaultValue) const;

template<class Param>
AtomParameter<Param> AtomTableT<Param>::value(const CGNumAtomID &cgatomid, const Param &defaultValue) const;

template<class Param>
AtomParameter<Param> AtomTableT<Param>::value(const ResNumAtomID &resatomid, const Param &defaultValue) const;

template<class Param>
AtomParameter<Param> AtomTableT<Param>::value(const ResIDAtomID &resatomid, const Param &defaultValue) const;

template<class Param>
AtomParameter<Param> AtomTableT<Param>::value(const AtomIndex &atom, const Param &defaultValue) const;

template<class Param>
AtomParameter<Param> AtomTableT<Param>::value(AtomID atomid, const Param &defaultValue) const;

template<class Param>
Param AtomTableT<Param>::parameter(const CGAtomID &cgatomid) const;

template<class Param>
Param AtomTableT<Param>::parameter(const CGNumAtomID &cgatomid) const;

template<class Param>
Param AtomTableT<Param>::parameter(const ResNumAtomID &resatomid) const;

template<class Param>
Param AtomTableT<Param>::parameter(const ResIDAtomID &resatomid) const;

template<class Param>
Param AtomTableT<Param>::parameter(const AtomIndex &atom) const;

template<class Param>
Param AtomTableT<Param>::parameter(AtomID atomid) const;

template<class Param>
void AtomTableT<Param>::setParameter(const CGAtomID &cgatomid, const Param &param);

template<class Param>
void AtomTableT<Param>::setParameter(const CGNumAtomID &cgatomid, const Param &param);

template<class Param>
void AtomTableT<Param>::setParameter(const ResNumAtomID &resatomid, const Param &param);

template<class Param>
void AtomTableT<Param>::setParameter(const ResIDAtomID &resatomid, const Param &param);

template<class Param>
void AtomTableT<Param>::setParameter(const AtomIndex &atom, const Param &param);

template<class Param>
void AtomTableT<Param>::setParameter(AtomID atomid, const Param &param);

template<class Param>
void AtomTableT<Param>::add(const TableBase &table);

template<class Param>
ParameterGroup<Param> AtomTableT<Param>::parameterGroup(CutGroupID cgid) const;

template<class Param>
QVector< ParameterGroup<Param> > AtomTableT<Param>::parameterGroups() const;

template<class Param>
QHash< CutGroupID,ParameterGroup<Param> >
AtomTableT<Param>::parameterGroups( const QSet<CutGroupID> &cgids ) const;

template<class Param>
QHash< CutGroupNum,ParameterGroup<Param> >
AtomTableT<Param>::parameterGroups( const QSet<CutGroupNum> &cgnums) const;

template<class Param>
QVector<Param> AtomTableT<Param>::parameters() const;

template<class Param>
QVector<Param> AtomTableT<Param>::parameters(ResNum resnum) const;

template<class Param>
QVector<Param> AtomTableT<Param>::parameters(ResID resid) const;

template<class Param>
QVector<Param> AtomTableT<Param>::parameters(CutGroupID cgid) const;

template<class Param>
QVector<Param> AtomTableT<Param>::parameters(CutGroupNum cgnum) const;

template<class Param>
QHash< CutGroupID,QVector<Param> >
AtomTableT<Param>::parameters(const QSet<CutGroupID> &cgids) const;

template<class Param>
QHash< CutGroupNum,QVector<Param> >
AtomTableT<Param>::parameters(const QSet<CutGroupNum> &cgnums) const;

template<class Param>
QHash< ResNum,QVector<Param> >
AtomTableT<Param>::parameters(const QSet<ResNum> &resnums) const;

template<class Param>
QHash< ResID,QVector<Param> >
AtomTableT<Param>::parameters(const QSet<ResID> &resids) const;

template<class Param>
template<class C>
void AtomTableT<Param>::parameters(CutGroupID cgid, QVector<C> &params) const;

template<class Param>
template<class C>
void AtomTableT<Param>::parameters(CutGroupNum cgnum, QVector<C> &params) const;

template<class Param>
template<class C>
void AtomTableT<Param>::parameters(ResNum resnum, QVector<C> &params) const;

template<class Param>
template<class C>
void AtomTableT<Param>::parameters(ResID resid, QVector<C> &params) const;

template<class Param>
QVector< AtomParameter<Param> > AtomTableT<Param>::atomParameters() const;

template<class Param>
QVector< AtomParameter<Param> > AtomTableT<Param>::atomParameters(ResNum resnum) const;

template<class Param>
QVector< AtomParameter<Param> > AtomTableT<Param>::atomParameters(ResID resid) const;

template<class Param>
QVector< AtomParameter<Param> > AtomTableT<Param>::atomParameters(CutGroupID cgid) const;

template<class Param>
QVector< AtomParameter<Param> > AtomTableT<Param>::atomParameters(CutGroupNum cgnum) const;

template<class Param>
QHash< CutGroupID,QVector< AtomParameter<Param> > >
AtomTableT<Param>::atomParameters(const QSet<CutGroupID> &cgids) const;

template<class Param>
QHash< CutGroupNum,QVector< AtomParameter<Param> > >
AtomTableT<Param>::atomParameters(const QSet<CutGroupNum> &cgnums) const;

template<class Param>
QHash< ResNum,QVector< AtomParameter<Param> > >
AtomTableT<Param>::atomParameters(const QSet<ResNum> &resnums) const;

template<class Param>
QHash< ResID,QVector< AtomParameter<Param> > >
AtomTableT<Param>::atomParameters(const QSet<ResID> &resids) const;

template<class Param>
template<class C>
void AtomTableT<Param>::parameters(CutGroupID cgid, QVector<C> &params) const;

template<class Param>
template<class C>
void AtomTableT<Param>::parameters(CutGroupNum cgnum, QVector<C> &params) const;

template<class Param>
template<class C>
void AtomTableT<Param>::parameters(ResNum resnum, QVector<C> &params) const;

template<class Param>
template<class C>
void AtomTableT<Param>::parameters(ResID resid, QVector<C> &params) const;

static const SireStream::MagicID atomtablet_magic
                        = SireStream::getMagic("SireDB::AtomTableT");

}

/** Serialise an AtomTable to a binary data stream */
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
QDataStream& operator<<(QDataStream &ds, const SireDB::AtomTableT<T> &table)
{
    SireStream::writeHeader(ds, SireDB::atomtablet_magic, 1)
               << table.params << static_cast<const SireDB::AtomTable&>(table);

    return ds;
}

/** Deserialise an AtomTable from a binary data stream */
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
QDataStream& operator>>(QDataStream &ds, SireDB::AtomTableT<T> &table)
{
    SireStream::VersionID v = SireStream::readHeader(ds, SireDB::atomtablet_magic,
                                                     "SireDB::AtomTableT");

    if (v == 1)
    {
        ds >> table.params >> static_cast<SireDB::AtomTable&>(table);
    }
    else
        throw SireStream::version_error(v, "1", "SireDB::AtomTableT", CODELOC);

    return ds;
}

SIRE_END_HEADER

#endif
