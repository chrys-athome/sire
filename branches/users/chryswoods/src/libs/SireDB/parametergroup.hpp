#ifndef SIREDB_PARAMETERGROUP_HPP
#define SIREDB_PARAMETERGROUP_HPP

#include "atomparameter.hpp"

SIRE_BEGIN_HEADER

namespace SireDB
{
template<class Param>
class ParameterGroup;
}

template<class Param>
QDataStream& operator<<(QDataStream&, const SireDB::ParameterGroup<Param>&);
template<class Param>
QDataStream& operator>>(QDataStream&, SireDB::ParameterGroup<Param>&);

namespace SireDB
{

/** This class holds a group of AtomParameters. This class is the parameter
    equivalent of SireMol::CutGroup. SireMol::CutGroup holds a group of
    atoms, holding the metadata in an AtomInfoGroup, and the coordinates
    in a QVector<SireMaths::Vector>. This class is very similar, also
    holding the metadata for the atoms in an AtomInfoGroup, but now holding
    the parameters in a QVector<Param>.

    \param Param  The type of parameter that is stored in this group

    @author Christopher Woods
*/
template<class Param>
class ParameterGroup
{

friend QDataStream& ::operator<<<>(QDataStream&, const ParameterGroup<Param>&);
friend QDataStream& ::operator>><>(QDataStream&, ParameterGroup<Param>&);

public:
    ParameterGroup();
    ParameterGroup(const QVector< AtomParameter<Param> > &params);
    ParameterGroup(const QList< AtomParameter<Param> > &params);

    ParameterGroup(const AtomInfoGroup &infogroup);
    ParameterGroup(const AtomInfoGroup &infogroup,
                   const QVector<Param> &parameters);

    ParameterGroup(const ParameterGroup<Param> &other);

    ~ParameterGroup();

    AtomParameter<Param> at(AtomID i) const;
    AtomParameter<Param> operator[](AtomID i) const;

    ParameterGroup<Param>& operator=(const ParameterGroup<Param> &other);
    ParameterGroup<Param>& operator=(const QVector<Param> &params);

    bool operator==(const ParameterGroup<Param> &other) const;
    bool operator!=(const ParameterGroup<Param> &other) const;

    bool isEmpty() const;

    QString toString() const;

    QVector< AtomParameter<Param> > atomParameters() const;
    QVector< AtomParameter<Param> > atomParameters(AtomID strt, AtomID end) const;

    const QVector<Param>& parameters() const;
    QVector<Param> parameters(AtomID strt, AtomID end) const;

    AtomParameter<Param> atomParameter(AtomID i) const;
    Param parameter(AtomID i) const;

    int nAtoms() const;

    void setParameters(const QVector<Param> &newparams);

    const AtomInfoGroup& atomGroup() const;

private:
    /** The metainformation about the atoms whose parameters
        this group corresponds to */
    AtomInfoGroup atominfos;

    /** Array of the parameters in this group */
    QVector<Param> params;
};

/** Null constructor */
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
ParameterGroup<Param>::ParameterGroup()
{}

/** Construct from the passed array of AtomParameters */
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
ParameterGroup<Param>::ParameterGroup(
                          const QVector< AtomParameter<Param> > &params)
{
    int nats = params.count();

    if (nats > 0)
    {
        QVector<AtomInfo> atominfovector;
        atominfovector.reserve(nats);
        params.reserve(nats);

        const AtomParameter<Param> *array = params.constData();

        for (int i=0; i<nats; ++i)
        {
            atominfovector.append(array[i]);
            params.append(array[i]);
        }

        atominfos = atominfovector;
    }
}

/** Construct from the list of AtomParameters */
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
ParameterGroup<Param>::ParameterGroup(const QList< AtomParameter<Param> > &params)
{
    int nats = params.count();

    if (nats > 0)
    {
        QVector<AtomInfo> atominfovector;
        atominfovector.reserve(nats);
        params.reserve(nats);

        for (typename QList< AtomParameter<Param> >::const_iterator it = params.begin();
             it != params.end();
             ++it)
        {
            atominfovector.append(*it);
            params.append(*it);
        }

        atominfos = atominfovector;
    }
}

/** Construct using default-constructed parameters for each of the
    atoms in the passed AtomInfoGroup */
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
ParameterGroup<Param>::ParameterGroup(const AtomInfoGroup &infogroup)
                      : atominfos(infogroup), params( infogroup.nAtoms() )
{}

/** Construct giving the parameters in 'parameters' to each of the
    atoms in the passed AtomInfoGroup - there must be the same number
    of parameters as atoms or else an exception will be thrown

    \throw SireError::incompatible_error
*/
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
ParameterGroup<Param>::ParameterGroup(const AtomInfoGroup &infogroup,
                                      const QVector<Param> &parameters)
{
    da de da de da...
}

template<class Param>
SIRE_OUTOFLINE_TEMPLATE
ParameterGroup<Param>::ParameterGroup(const ParameterGroup<Param> &other);

template<class Param>
SIRE_OUTOFLINE_TEMPLATE
ParameterGroup<Param>::~ParameterGroup();

template<class Param>
SIRE_OUTOFLINE_TEMPLATE
AtomParameter<Param> ParameterGroup<Param>::at(AtomID i) const;

template<class Param>
SIRE_OUTOFLINE_TEMPLATE
AtomParameter<Param> ParameterGroup<Param>::operator[](AtomID i) const;

template<class Param>
SIRE_OUTOFLINE_TEMPLATE
ParameterGroup<Param>& ParameterGroup<Param>::operator=(const ParameterGroup<Param> &other);

template<class Param>
SIRE_OUTOFLINE_TEMPLATE
ParameterGroup<Param>& ParameterGroup<Param>::operator=(const QVector<Param> &params);

template<class Param>
SIRE_OUTOFLINE_TEMPLATE
bool ParameterGroup<Param>::operator==(const ParameterGroup<Param> &other) const;

template<class Param>
SIRE_OUTOFLINE_TEMPLATE
bool ParameterGroup<Param>::operator!=(const ParameterGroup<Param> &other) const;

template<class Param>
SIRE_OUTOFLINE_TEMPLATE
bool ParameterGroup<Param>::isEmpty() const;

template<class Param>
SIRE_OUTOFLINE_TEMPLATE
QString ParameterGroup<Param>::toString() const;

template<class Param>
SIRE_OUTOFLINE_TEMPLATE
QVector< AtomParameter<Param> > ParameterGroup<Param>::atomParameters() const;

template<class Param>
SIRE_OUTOFLINE_TEMPLATE
QVector< AtomParameter<Param> > ParameterGroup<Param>::atomParameters(AtomID strt, AtomID end) const;

template<class Param>
SIRE_OUTOFLINE_TEMPLATE
const QVector<Param>& ParameterGroup<Param>::parameters() const;

template<class Param>
SIRE_OUTOFLINE_TEMPLATE
QVector<Param> ParameterGroup<Param>::parameters(AtomID strt, AtomID end) const;

template<class Param>
SIRE_OUTOFLINE_TEMPLATE
AtomParameter<Param> ParameterGroup<Param>::atomParameter(AtomID i) const;

template<class Param>
SIRE_OUTOFLINE_TEMPLATE
Param ParameterGroup<Param>::parameter(AtomID i) const;

template<class Param>
SIRE_OUTOFLINE_TEMPLATE
int ParameterGroup<Param>::nAtoms() const;

template<class Param>
SIRE_OUTOFLINE_TEMPLATE
void ParameterGroup<Param>::setParameters(const QVector<Param> &newparams);

template<class Param>
SIRE_OUTOFLINE_TEMPLATE
const AtomInfoGroup& ParameterGroup<Param>::atomGroup() const;

static const SireStream::MagicID parametergroup_magic
                        = SireStream::getMagic("SireDB::ParameterGroup");

}

/** Serialise to a binary datastream */
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
QDataStream &operator<<(QDataStream &ds, const SireDB::ParameterGroup<Param> &group)
{
    writeHeader(ds, parametergroup_magic, 1)
            << group.atominfos
            << group.params;

    return ds;
}

/** Deserialise from a binary datastream */
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
QDataStream &operator>>(QDataStream &ds, SireDB::ParameterGroup<Param> &group)
{
    VersionID v = readHeader(ds, parametergroup_magic,
                                 "SireDB::ParameterGroup");

    if (v == 1)
    {
        ds >> group.atominfos >> group.params;
    }
    else
        throw version_error(v, "1", "SireDB::ParameterGroup", CODELOC);

    return ds;
}

SIRE_END_HEADER

#endif
