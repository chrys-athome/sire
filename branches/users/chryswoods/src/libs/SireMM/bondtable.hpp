#ifndef SIREMM_BONDTABLE_HPP
#define SIREMM_BONDTABLE_HPP

#include "internaltable.hpp"
#include "molbondinfo.h"

SIRE_BEGIN_HEADER

namespace SireMM
{
template<class Param>
class BondTableT;
}

template<class Param>
QDataStream& operator<<(QDataStream&, const SireMM::BondTableT<Param>&);
template<class Param>
QDataStream& operator>>(QDataStream&, SireMM::BondTableT<Param>&);

namespace SireMM
{

using SireMol::Bond;

/**
This class holds all of the parameters that are used to obtain the energy of the bonds in the molecule.

This function is just an overload of InternalTable<MolBondInfo,Param> that adds some nice synactic sugar with functions named 'addBond', 'removeBond' etc., rather than addParameter, removeParameter. This enables this class to be easily used with other InternalTable-derived classes without too many function-name clashing problems.

@author Christopher Woods
*/
template<class Param>
class BondTableT : public InternalTable<MolBondInfo,Param>
{
public:
    BondTableT();
    BondTableT(const MolBondInfo &bondinfo);

    BondTableT(const BondTableT<Param> &other);

    ~BondTableT();

    int nBonds() const;
    int nBonds(ResNum resnum) const;
    int nBonds(GroupID groupid) const;
    int nIntraBonds() const;
    int nInterBonds() const;
    int nIntraBonds(ResNum resnum) const;
    int nInterBonds(ResNum resnum) const;

    int nBondParameters() const;
    int nBondParameters(ResNum resnum) const;
    int nBondParameters(GroupID groupid) const;
    int nIntraBondParameters() const;
    int nInterBondParameters() const;
    int nIntraBondParameters(ResNum resnum) const;
    int nInterBondParameters(ResNum resnum) const;

    void addBond(const Bond &bond);
    void addBond(const Bond &bond, const Param &param);

    template<class C>
    void addBonds(const C &bonds);

    void removeBond(const Bond &bond);

    template<class C>
    void removeBonds(const C &bonds);

    void removeBonds();
    void removeBonds(ResNum resnum);
    void removeBonds(GroupID groupid);
    void removeBonds(const QSet<GroupID> &groupids);

    void removeIntraBonds();
    void removeIntraBonds(ResNum resnum);

    void removeInterBonds();
    void removeInterBonds(ResNum resnum);

    void removeMissingBonds();
    void removeMissingBonds(ResNum resnum);
    void removeMissingBonds(GroupID groupid);
    void removeMissingBonds(const QSet<GroupID> &groupids);

    void removeMissingIntraBonds();
    void removeMissingIntraBonds(ResNum resnum);
    void removeMissingInterBonds();
    void removeMissingInterBonds(ResNum resnum);

    void clearBond(const Bond &bond);
    void clearBonds();
    void clearBonds(ResNum resnum);
    void clearBonds(GroupID groupid);

    template<class C>
    void clearBonds(const C &bonds);

    void clearIntraBonds();
    void clearIntraBonds(ResNum resnum);

    void clearInterBonds();
    void clearInterBonds(ResNum resnum);

    template<class C>
    void clearIntraBonds(const C &bonds);
    template<class C>
    void clearInterBonds(const C &bonds);

    QVector<Bond> bonds() const;
    QVector<Bond> bonds(ResNum resnum) const;
    QVector<Bond> bonds(GroupID groupid) const;

    QVector<Bond> interBonds() const;
    QVector<Bond> intraBonds() const;
    QVector<Bond> interBonds(ResNum resnum) const;
    QVector<Bond> intraBonds(ResNum resnum) const;

    GroupedVector<GroupIndexID,Bond> bondsByGroup() const;
    GroupedVector<GroupIndexID,Bond> bondsByGroup(const QSet<GroupID> &gids) const;
    GroupedVector<ResNumIndexID,Bond> bondsByResidue() const;
    GroupedVector<ResNumIndexID,Bond> bondsByResidue(const QSet<ResNum> &resnums) const;

    GroupedVector<ResNumIndexID,Bond> interBondsByResidue() const;
    GroupedVector<ResNumIndexID,Bond> intraBondsByResidue() const;
    GroupedVector<ResNumIndexID,Bond> interBondsByResidue(const QSet<ResNum> &resnums) const;
    GroupedVector<ResNumIndexID,Bond> intraBondsByResidue(const QSet<ResNum> &resnums) const;

    const Param& getBondParameter(const Bond &bond) const;
    const Param& getBondParameter(const GroupIndexID &id) const;

    void setBondParameter(const Bond &bond, const Param &param);
    void setBondParameter(const GroupIndexID &id, const Param &param);

    bool assignedBondParameter(const Bond &bond) const;
    bool assignedBondParameter(const GroupIndexID &id) const;

    bool hasMissingBondParameters() const;
    bool hasMissingBondParameters(ResNum resnum) const;
    bool hasMissingBondParameters(GroupID id) const;

    bool hasMissingIntraBondParameters() const;
    bool hasMissingIntraBondParameters(ResNum resnum) const;

    bool hasMissingInterBondParameters() const;
    bool hasMissingInterBondParameters(ResNum resnum) const;

    QSet<Bond> missingBondParameters() const;
    QSet<Bond> missingBondParameters(ResNum resnum) const;
    QSet<Bond> missingBondParameters(const QSet<GroupID> &groupids) const;

    QSet<Bond> missingIntraBondParameters() const;
    QSet<Bond> missingIntraBondParameters(ResNum resnum) const;

    QSet<Bond> missingInterBondParameters() const;
    QSet<Bond> missingInterBondParameters(ResNum resnum) const;

    QVector<Param> bondParameters() const;
    QVector<Param> bondParameters(ResNum resnum) const;
    QVector<Param> bondParameters(GroupID groupid) const;

    QVector<Param> interBondParameters() const;
    QVector<Param> intraBondParameters() const;
    QVector<Param> interBondParameters(ResNum resnum) const;
    QVector<Param> intraBondParameters(ResNum resnum) const;

    GroupedVector<GroupIndexID,Param> bondParametersByGroup() const;
    GroupedVector<GroupIndexID,Param> bondParametersByGroup(
                                                const QSet<GroupID> &gids) const;
    GroupedVector<ResNumIndexID,Param> bondParametersByResidue() const;
    GroupedVector<ResNumIndexID,Param> bondParametersByResidue(
                                                const QSet<ResNum> &resnums) const;

    GroupedVector<ResNumIndexID,Param> interBondParametersByResidue() const;
    GroupedVector<ResNumIndexID,Param> intraBondParametersByResidue() const;
    GroupedVector<ResNumIndexID,Param> interBondParametersByResidue(
                                                const QSet<ResNum> &resnums) const;
    GroupedVector<ResNumIndexID,Param> intraBondParametersByResidue(
                                                const QSet<ResNum> &resnums) const;
};

/** Empty constructor */
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
BondTableT<Param>::BondTableT() : InternalTable<MolBondInfo,Param>()
{}

/** Create a table to hold the parameters for the bonds described by 'bondinfo' */
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
BondTableT<Param>::BondTableT(const MolBondInfo &bondinfo)
                  : InternalTable<MolBondInfo,Param>(bondinfo)
{}

/** Copy constructor */
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
BondTableT<Param>::BondTableT(const BondTableT<Param> &other)
                  : InternalTable<MolBondInfo,Param>(other)
{}

/** Destructor */
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
BondTableT<Param>::~BondTableT()
{}

/** Synonym for nInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
int BondTableT<Param>::nBonds() const
{
    return this->nInternals();
}

/** Synonym for nInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
int BondTableT<Param>::nBonds(ResNum resnum) const
{
    return this->nInternals(resnum);
}

/** Synonym for nInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
int BondTableT<Param>::nBonds(GroupID groupid) const
{
    return this->nInternals(groupid);
}

/** Synonym for nIntraInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
int BondTableT<Param>::nIntraBonds() const
{
    return this->nIntraInternals();
}

/** Synonym for nInterInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
int BondTableT<Param>::nInterBonds() const
{
    return this->nInterInternals();
}

/** Synonym for nIntraInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
int BondTableT<Param>::nIntraBonds(ResNum resnum) const
{
    return this->nIntraInternals(resnum);
}

/** Synonym for nInterInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
int BondTableT<Param>::nInterBonds(ResNum resnum) const
{
    return this->nInterInternals(resnum);
}

/** Synonym for nParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
int BondTableT<Param>::nBondParameters() const
{
    return this->nParameters();
}

/** Synonym for nParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
int BondTableT<Param>::nBondParameters(ResNum resnum) const
{
    return this->nParameters(resnum);
}

/** Synonym for nParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
int BondTableT<Param>::nBondParameters(GroupID groupid) const
{
    return this->nParameters(groupid);
}

/** Synonym for nIntraParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
int BondTableT<Param>::nIntraBondParameters() const
{
    return this->nIntraParameters();
}

/** Synonym for nInterParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
int BondTableT<Param>::nInterBondParameters() const
{
    return this->nInterParameters();
}

/** Synonym for nIntraParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
int BondTableT<Param>::nIntraBondParameters(ResNum resnum) const
{
    return this->nIntraParameters(resnum);
}

/** Synonym for nInterParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
int BondTableT<Param>::nInterBondParameters(ResNum resnum) const
{
    return this->nInterParameters(resnum);
}

/** Synonym for addInternal(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void BondTableT<Param>::addBond(const Bond &bond)
{
    this->addInternal(bond);
}

/** Synonym for addInternal(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void BondTableT<Param>::addBond(const Bond &bond, const Param &param)
{
    this->addInternal(bond,param);
}

/** Synonym for addInternals(...) */
template<class Param>
template<class C>
SIRE_INLINE_TEMPLATE
void BondTableT<Param>::addBonds(const C &bonds)
{
    this->addInternals(bonds);
}

/** Synonym for removeInternal(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void BondTableT<Param>::removeBond(const Bond &bond)
{
    this->removeInternal(bond);
}

/** Synonym for removeInternals(...) */
template<class Param>
template<class C>
SIRE_INLINE_TEMPLATE
void BondTableT<Param>::removeBonds(const C &bonds)
{
    this->removeInternals(bonds);
}

/** Synonym for removeInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void BondTableT<Param>::removeBonds()
{
    this->removeInternals();
}

/** Synonym for removeInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void BondTableT<Param>::removeBonds(ResNum resnum)
{
    this->removeInternals(resnum);
}

/** Synonym for removeInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void BondTableT<Param>::removeBonds(GroupID groupid)
{
    this->removeInternals(groupid);
}

/** Synonym for removeInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void BondTableT<Param>::removeBonds(const QSet<GroupID> &groupids)
{
    this->removeInternals(groupids);
}

/** Synonym for removeIntraInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void BondTableT<Param>::removeIntraBonds()
{
    this->removeIntraInternals();
}

/** Synonym for removeIntraInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void BondTableT<Param>::removeIntraBonds(ResNum resnum)
{
    this->removeIntraInternals(resnum);
}

/** Synonym for removeInterInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void BondTableT<Param>::removeInterBonds()
{
    this->removeInterInternals();
}

/** Synonym for removeInterInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void BondTableT<Param>::removeInterBonds(ResNum resnum)
{
    this->removeInterInternals(resnum);
}

/** Synonym for removeMissingInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void BondTableT<Param>::removeMissingBonds()
{
    this->removeMissingInternals();
}

/** Synonym for removeMissingInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void BondTableT<Param>::removeMissingBonds(ResNum resnum)
{
    this->removeMissingInternals(resnum);
}

/** Synonym for removeMissingInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void BondTableT<Param>::removeMissingBonds(GroupID groupid)
{
    this->removeMissingInternals(groupid);
}

/** Synonym for removeMissingInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void BondTableT<Param>::removeMissingBonds(const QSet<GroupID> &groupids)
{
    this->removeMissingInternals(groupids);
}

/** Synonym for removeMissingIntraInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void BondTableT<Param>::removeMissingIntraBonds()
{
    this->removeMissingIntraInternals();
}

/** Synonym for removeMissingIntraInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void BondTableT<Param>::removeMissingIntraBonds(ResNum resnum)
{
    this->removeMissingIntraInternals(resnum);
}

/** Synonym for removeMissingInterInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void BondTableT<Param>::removeMissingInterBonds()
{
    this->removeMissingInterInternals();
}

/** Synonym for removeMissingInterInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void BondTableT<Param>::removeMissingInterBonds(ResNum resnum)
{
    this->removeMissingInterInternals(resnum);
}

/** Synonym for clear(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void BondTableT<Param>::clearBond(const Bond &bond)
{
    this->clear(bond);
}

/** Synonym for clear(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void BondTableT<Param>::clearBonds()
{
    this->clear();
}

/** Synonym for clear(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void BondTableT<Param>::clearBonds(ResNum resnum)
{
    this->clear(resnum);
}

/** Synonym for clear(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void BondTableT<Param>::clearBonds(GroupID groupid)
{
    this->clear(groupid);
}

/** Synonym for clear(...) */
template<class Param>
template<class C>
SIRE_OUTOFLINE_TEMPLATE
void BondTableT<Param>::clearBonds(const C &bonds)
{
    this->clear(bonds);
}

/** Synonym for clearIntra(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void BondTableT<Param>::clearIntraBonds()
{
    this->clearIntra();
}

/** Synonym for clearIntra(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void BondTableT<Param>::clearIntraBonds(ResNum resnum)
{
    this->clearIntra(resnum);
}

/** Synonym for clearInter(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void BondTableT<Param>::clearInterBonds()
{
    this->clearInter();
}

/** Synonym for clearInter(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void BondTableT<Param>::clearInterBonds(ResNum resnum)
{
    this->clearInter(resnum);
}

/** Synonym for clearInter(...) */
template<class Param>
template<class C>
SIRE_OUTOFLINE_TEMPLATE
void BondTableT<Param>::clearInterBonds(const C &bonds)
{
    this->clearInter(bonds);
}

/** Synonym for clearIntra(...) */
template<class Param>
template<class C>
SIRE_OUTOFLINE_TEMPLATE
void BondTableT<Param>::clearIntraBonds(const C &bonds)
{
    this->clearIntra(bonds);
}

/** Synonym for internals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QVector<Bond> BondTableT<Param>::bonds() const
{
    return this->internals();
}

/** Synonym for internals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QVector<Bond> BondTableT<Param>::bonds(ResNum resnum) const
{
    return this->internals(resnum);
}

/** Synonym for internals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QVector<Bond> BondTableT<Param>::bonds(GroupID groupid) const
{
    return this->internals(groupid);
}

/** Synonym for interInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QVector<Bond> BondTableT<Param>::interBonds() const
{
    return this->interInternals();
}

/** Synonym for intraInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QVector<Bond> BondTableT<Param>::intraBonds() const
{
    return this->intraInternals();
}

/** Synonym for interInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QVector<Bond> BondTableT<Param>::interBonds(ResNum resnum) const
{
    return this->interInternals(resnum);
}

/** Synonym for intraInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QVector<Bond> BondTableT<Param>::intraBonds(ResNum resnum) const
{
    return this->intraInternals(resnum);
}

/** Synonym for internalsByGroup(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
GroupedVector<GroupIndexID,Bond> BondTableT<Param>::bondsByGroup() const
{
    return this->internalsByGroup();
}

/** Synonym for internalsByGroup(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
GroupedVector<GroupIndexID,Bond> BondTableT<Param>::bondsByGroup(
                                          const QSet<GroupID> &gids) const
{
    return this->internalsByGroup(gids);
}

/** Synonym for internalsByResidue(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
GroupedVector<ResNumIndexID,Bond> BondTableT<Param>::bondsByResidue() const
{
    return this->internalsByResidue();
}

/** Synonym for internalsByResidue(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
GroupedVector<ResNumIndexID,Bond> BondTableT<Param>::bondsByResidue(
                                          const QSet<ResNum> &resnums) const
{
    return this->internalsByResidue(resnums);
}

/** Synonym for interInternalsByResidue(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
GroupedVector<ResNumIndexID,Bond> BondTableT<Param>::interBondsByResidue() const
{
    return this->interInternalsByResidue();
}

/** Synonym for intraInternalsByResidue(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
GroupedVector<ResNumIndexID,Bond> BondTableT<Param>::intraBondsByResidue() const
{
    return this->intraInternalsByResidue();
}

/** Synonym for interInternalsByResidue(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
GroupedVector<ResNumIndexID,Bond>
BondTableT<Param>::interBondsByResidue(const QSet<ResNum> &resnums) const
{
    return this->interInternalsByResidue(resnums);
}

/** Synonym for intraInternalsByResidue(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
GroupedVector<ResNumIndexID,Bond>
BondTableT<Param>::intraBondsByResidue(const QSet<ResNum> &resnums) const
{
    return this->intraInternalsByResidue(resnums);
}

/** Synonym for getParameter(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
const Param& BondTableT<Param>::getBondParameter(const Bond &bond) const
{
    return this->getParameter(bond);
}

/** Synonym for getParameter(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
const Param& BondTableT<Param>::getBondParameter(const GroupIndexID &id) const
{
    return this->getParameter(id);
}

/** Synonym for setParameter(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void BondTableT<Param>::setBondParameter(const Bond &bond, const Param &param)
{
    return this->setParameter(bond, param);
}

/** Synonym for setParameter(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void BondTableT<Param>::setBondParameter(const GroupIndexID &id, const Param &param)
{
    return this->setParameter(id, param);
}

/** Synonym for assignedParameter(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
bool BondTableT<Param>::assignedBondParameter(const Bond &bond) const
{
    return this->assignedParameter(bond);
}

/** Synonym for assignedParameter(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
bool BondTableT<Param>::assignedBondParameter(const GroupIndexID &id) const
{
    return this->assignedParameter(id);
}

/** Synonym for hasMissingParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
bool BondTableT<Param>::hasMissingBondParameters() const
{
    return this->hasMissingParameters();
}

/** Synonym for hasMissingParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
bool BondTableT<Param>::hasMissingBondParameters(ResNum resnum) const
{
    return this->hasMissingParameters(resnum);
}

/** Synonym for hasMissingParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
bool BondTableT<Param>::hasMissingBondParameters(GroupID id) const
{
    return this->hasMissingParameters(id);
}

/** Synonym for hasMissingIntraParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
bool BondTableT<Param>::hasMissingIntraBondParameters() const
{
    return this->hasMissingIntraParameters();
}

/** Synonym for hasMissingIntraParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
bool BondTableT<Param>::hasMissingIntraBondParameters(ResNum resnum) const
{
    return this->hasMissingIntraParameters(resnum);
}

/** Synonym for hasMissingInterParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
bool BondTableT<Param>::hasMissingInterBondParameters() const
{
    return this->hasMissingInterParameters();
}

/** Synonym for hasMissingInterParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
bool BondTableT<Param>::hasMissingInterBondParameters(ResNum resnum) const
{
    return this->hasMissingInterParameters(resnum);
}

/** Synonym for missingParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QSet<Bond> BondTableT<Param>::missingBondParameters() const
{
    return this->missingParameters();
}

/** Synonym for missingParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QSet<Bond> BondTableT<Param>::missingBondParameters(ResNum resnum) const
{
    return this->missingParameters(resnum);
}

/** Synonym for missingParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QSet<Bond> BondTableT<Param>::missingBondParameters(const QSet<GroupID> &groupids) const
{
    return this->missingParameters(groupids);
}

/** Synonym for missingIntraParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QSet<Bond> BondTableT<Param>::missingIntraBondParameters() const
{
    return this->missingIntraParameters();
}

/** Synonym for missingIntraParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QSet<Bond> BondTableT<Param>::missingIntraBondParameters(ResNum resnum) const
{
    return this->missingIntraParameters(resnum);
}

/** Synonym for missingInterParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QSet<Bond> BondTableT<Param>::missingInterBondParameters() const
{
    return this->missingInterParameters();
}

/** Synonym for missingInterParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QSet<Bond> BondTableT<Param>::missingInterBondParameters(ResNum resnum) const
{
    return this->missingInterParameters(resnum);
}

/** Synonym for parameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QVector<Param> BondTableT<Param>::bondParameters() const
{
    return this->parameters();
}

/** Synonym for parameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QVector<Param> BondTableT<Param>::bondParameters(ResNum resnum) const
{
    return this->parameters(resnum);
}

/** Synonym for parameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QVector<Param> BondTableT<Param>::bondParameters(GroupID groupid) const
{
    return this->parameters(groupid);
}

/** Synonym for intraParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QVector<Param> BondTableT<Param>::interBondParameters() const
{
    return this->interParameters();
}

/** Synonym for intraParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QVector<Param> BondTableT<Param>::intraBondParameters() const
{
    return this->intraParameters();
}

/** Synonym for interParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QVector<Param> BondTableT<Param>::interBondParameters(ResNum resnum) const
{
    return this->interParameters(resnum);
}

/** Synonym for interParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QVector<Param> BondTableT<Param>::intraBondParameters(ResNum resnum) const
{
    return this->intraParameters(resnum);
}

/** Synonym for parametersByGroup(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
GroupedVector<GroupIndexID,Param> BondTableT<Param>::bondParametersByGroup() const
{
    return this->parametersByGroup();
}

/** Synonym for parametersByGroup(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
GroupedVector<GroupIndexID,Param>
BondTableT<Param>::bondParametersByGroup(const QSet<GroupID> &gids) const
{
    return this->parametersByGroup(gids);
}

/** Synonym for parametersByResidue(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
GroupedVector<ResNumIndexID,Param> BondTableT<Param>::bondParametersByResidue() const
{
    return this->parametersByResidue();
}

/** Synonym for parametersByResidue(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
GroupedVector<ResNumIndexID,Param>
BondTableT<Param>::bondParametersByResidue(const QSet<ResNum> &resnums) const
{
    return this->parametersByResidue(resnums);
}

/** Synonym for interParametersByResidue(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
GroupedVector<ResNumIndexID,Param> BondTableT<Param>::interBondParametersByResidue() const
{
    return this->interParametersByResidue();
}

/** Synonym for intraParametersByResidue(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
GroupedVector<ResNumIndexID,Param> BondTableT<Param>::intraBondParametersByResidue() const
{
    return this->intraParametersByResidue();
}

/** Synonym for interParametersByResidue(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
GroupedVector<ResNumIndexID,Param>
BondTableT<Param>::interBondParametersByResidue(const QSet<ResNum> &resnums) const
{
    return this->interParametersByResidue(resnums);
}

/** Synonym for intraParametersByResidue(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
GroupedVector<ResNumIndexID,Param>
BondTableT<Param>::intraBondParametersByResidue(const QSet<ResNum> &resnums) const
{
    return this->intraParametersByResidue(resnums);
}

const MagicID bondtablet_magic = getMagic("SireMM::BondTableT");

}

/** Serialise to a binary data stream */
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
QDataStream& operator<<(QDataStream &ds,
                        const SireMM::BondTableT<Param> &table)
{
    SireStream::writeHeader(ds, SireMM::bondtablet_magic, 0)
             << static_cast<const SireMM::InternalTable<SireMM::MolBondInfo,Param>&>(table);

    return ds;
}

/** Deserialise from a binary data stream */
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
QDataStream& operator>>(QDataStream &ds,
                        SireMM::BondTableT<Param> &table)
{
    SireStream::VersionID v = SireStream::readHeader(ds, SireMM::bondtablet_magic,
                                                     "SireMM::BondTableT");

    if (v == 0)
    {
        ds >> static_cast<SireMM::InternalTable<SireMM::MolBondInfo, Param>&>(table);
    }
    else
        throw SireStream::version_error(v, "0", "SireMM::BondTableT", CODELOC);

    return ds;
}

SIRE_END_HEADER

#endif
