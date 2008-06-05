#ifndef SIREMM_DIHEDRALTABLE_HPP
#define SIREMM_DIHEDRALTABLE_HPP

#include "internaltable.hpp"
#include "moldihedralinfo.h"

SIRE_BEGIN_HEADER

namespace SireMM
{
template<class Param>
class DihedralTableT;
}

template<class Param>
QDataStream& operator<<(QDataStream&, const SireMM::DihedralTableT<Param>&);
template<class Param>
QDataStream& operator>>(QDataStream&, SireMM::DihedralTableT<Param>&);

namespace SireMM
{

using SireMol::Dihedral;

/**
This class holds all of the parameters that are used to obtain the energy of the dihedrals in the molecule.

This function is just an overload of InternalTable<MolDihedralInfo,Param> that adds some nice synactic sugar with functions named 'addDihedral', 'removeDihedral' etc., rather than addParameter, removeParameter. This enables this class to be easily used with other InternalTable-derived classes without too many function-name clashing problems.

@author Christopher Woods
*/
template<class Param>
class DihedralTableT : public InternalTable<MolDihedralInfo,Param>
{
public:
    DihedralTableT();
    DihedralTableT(const MolDihedralInfo &dihedralinfo);
    
    DihedralTableT(const DihedralTableT<Param> &other);
    
    ~DihedralTableT();

    int nDihedrals() const;
    int nDihedrals(ResNum resnum) const;
    int nDihedrals(GroupID groupid) const;
    int nIntraDihedrals() const;
    int nInterDihedrals() const;
    int nIntraDihedrals(ResNum resnum) const;
    int nInterDihedrals(ResNum resnum) const;
    
    int nDihedralParameters() const;
    int nDihedralParameters(ResNum resnum) const;
    int nDihedralParameters(GroupID groupid) const;
    int nIntraDihedralParameters() const;
    int nInterDihedralParameters() const;
    int nIntraDihedralParameters(ResNum resnum) const;
    int nInterDihedralParameters(ResNum resnum) const;
    
    void addDihedral(const Dihedral &dihedral);
    void addDihedral(const Dihedral &dihedral, const Param &param);
    
    template<class C>
    void addDihedrals(const C &dihedrals);
    
    void removeDihedral(const Dihedral &dihedral);
    
    template<class C>
    void removeDihedrals(const C &dihedrals);
    
    void removeDihedrals();
    void removeDihedrals(ResNum resnum);
    void removeDihedrals(GroupID groupid);
    void removeDihedrals(const QSet<GroupID> &groupids);
    
    void removeIntraDihedrals();
    void removeIntraDihedrals(ResNum resnum);
    
    void removeInterDihedrals();
    void removeInterDihedrals(ResNum resnum);
    
    void removeMissingDihedrals();
    void removeMissingDihedrals(ResNum resnum);
    void removeMissingDihedrals(GroupID groupid);
    void removeMissingDihedrals(const QSet<GroupID> &groupids);
    
    void removeMissingIntraDihedrals();
    void removeMissingIntraDihedrals(ResNum resnum);
    void removeMissingInterDihedrals();
    void removeMissingInterDihedrals(ResNum resnum);
    
    void clearDihedral(const Dihedral &dihedral);
    void clearDihedrals();
    void clearDihedrals(ResNum resnum);
    void clearDihedrals(GroupID groupid);
    
    template<class C>
    void clearDihedrals(const C &dihedrals);
    
    void clearIntraDihedrals();
    void clearIntraDihedrals(ResNum resnum);
    
    void clearInterDihedrals();
    void clearInterDihedrals(ResNum resnum);

    template<class C>
    void clearIntraDihedrals(const C &dihedrals);
    template<class C>
    void clearInterDihedrals(const C &dihedrals);

    QVector<Dihedral> dihedrals() const;
    QVector<Dihedral> dihedrals(ResNum resnum) const;
    QVector<Dihedral> dihedrals(GroupID groupid) const;
    
    QVector<Dihedral> interDihedrals() const;
    QVector<Dihedral> intraDihedrals() const;
    QVector<Dihedral> interDihedrals(ResNum resnum) const;
    QVector<Dihedral> intraDihedrals(ResNum resnum) const;
    
    GroupedVector<GroupIndexID,Dihedral> dihedralsByGroup() const;
    GroupedVector<GroupIndexID,Dihedral> 
                      dihedralsByGroup(const QSet<GroupID> &gids) const;
    GroupedVector<ResNumIndexID,Dihedral> dihedralsByResidue() const;
    GroupedVector<ResNumIndexID,Dihedral> 
                      dihedralsByResidue(const QSet<ResNum> &resnums) const;
    
    GroupedVector<ResNumIndexID,Dihedral> interDihedralsByResidue() const;
    GroupedVector<ResNumIndexID,Dihedral> intraDihedralsByResidue() const;
    GroupedVector<ResNumIndexID,Dihedral> 
                      interDihedralsByResidue(const QSet<ResNum> &resnums) const;
    GroupedVector<ResNumIndexID,Dihedral> 
                      intraDihedralsByResidue(const QSet<ResNum> &resnums) const;
    
    const Param& getDihedralParameter(const Dihedral &dihedral) const;
    const Param& getDihedralParameter(const GroupIndexID &id) const;
    
    void setDihedralParameter(const Dihedral &dihedral, const Param &param);
    void setDihedralParameter(const GroupIndexID &id, const Param &param);

    bool assignedDihedralParameter(const Dihedral &dihedral) const;
    bool assignedDihedralParameter(const GroupIndexID &id) const;
    
    bool hasMissingDihedralParameters() const;
    bool hasMissingDihedralParameters(ResNum resnum) const;
    bool hasMissingDihedralParameters(GroupID id) const;
    
    bool hasMissingIntraDihedralParameters() const;
    bool hasMissingIntraDihedralParameters(ResNum resnum) const;
    
    bool hasMissingInterDihedralParameters() const;
    bool hasMissingInterDihedralParameters(ResNum resnum) const;
    
    QSet<Dihedral> missingDihedralParameters() const;
    QSet<Dihedral> missingDihedralParameters(ResNum resnum) const;
    QSet<Dihedral> missingDihedralParameters(const QSet<GroupID> &groupids) const;
    
    QSet<Dihedral> missingIntraDihedralParameters() const;
    QSet<Dihedral> missingIntraDihedralParameters(ResNum resnum) const;
    
    QSet<Dihedral> missingInterDihedralParameters() const;
    QSet<Dihedral> missingInterDihedralParameters(ResNum resnum) const;
    
    QVector<Param> dihedralParameters() const;
    QVector<Param> dihedralParameters(ResNum resnum) const;
    QVector<Param> dihedralParameters(GroupID groupid) const;
    
    QVector<Param> interDihedralParameters() const;
    QVector<Param> intraDihedralParameters() const;
    QVector<Param> interDihedralParameters(ResNum resnum) const;
    QVector<Param> intraDihedralParameters(ResNum resnum) const;
    
    GroupedVector<GroupIndexID,Param> dihedralParametersByGroup() const;
    GroupedVector<GroupIndexID,Param> dihedralParametersByGroup(
                                                const QSet<GroupID> &gids) const;
    GroupedVector<ResNumIndexID,Param> dihedralParametersByResidue() const;
    GroupedVector<ResNumIndexID,Param> dihedralParametersByResidue(
                                                const QSet<ResNum> &resnums) const;
                                                    
    GroupedVector<ResNumIndexID,Param> interDihedralParametersByResidue() const;
    GroupedVector<ResNumIndexID,Param> intraDihedralParametersByResidue() const;
    GroupedVector<ResNumIndexID,Param> interDihedralParametersByResidue(
                                                const QSet<ResNum> &resnums) const;
    GroupedVector<ResNumIndexID,Param> intraDihedralParametersByResidue(
                                                const QSet<ResNum> &resnums) const;    
};

/** Empty constructor */
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
DihedralTableT<Param>::DihedralTableT() : InternalTable<MolDihedralInfo,Param>()
{}

/** Create a table to hold the parameters for the dihedrals described by 'dihedralinfo' */
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
DihedralTableT<Param>::DihedralTableT(const MolDihedralInfo &dihedralinfo) 
                  : InternalTable<MolDihedralInfo,Param>(dihedralinfo)
{}

/** Copy constructor */
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
DihedralTableT<Param>::DihedralTableT(const DihedralTableT<Param> &other)
                  : InternalTable<MolDihedralInfo,Param>(other)
{}

/** Destructor */
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
DihedralTableT<Param>::~DihedralTableT()
{}

/** Synonym for nInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
int DihedralTableT<Param>::nDihedrals() const
{
    return this->nInternals();
}

/** Synonym for nInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
int DihedralTableT<Param>::nDihedrals(ResNum resnum) const
{
    return this->nInternals(resnum);
}

/** Synonym for nInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
int DihedralTableT<Param>::nDihedrals(GroupID groupid) const
{
    return this->nInternals(groupid);
}

/** Synonym for nIntraInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
int DihedralTableT<Param>::nIntraDihedrals() const
{
    return this->nIntraInternals();
}

/** Synonym for nInterInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
int DihedralTableT<Param>::nInterDihedrals() const
{
    return this->nInterInternals();
}

/** Synonym for nIntraInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
int DihedralTableT<Param>::nIntraDihedrals(ResNum resnum) const
{
    return this->nIntraInternals(resnum);
}

/** Synonym for nInterInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
int DihedralTableT<Param>::nInterDihedrals(ResNum resnum) const
{
    return this->nInterInternals(resnum);
}

/** Synonym for nParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
int DihedralTableT<Param>::nDihedralParameters() const
{
    return this->nParameters();
}

/** Synonym for nParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
int DihedralTableT<Param>::nDihedralParameters(ResNum resnum) const
{
    return this->nParameters(resnum);
}

/** Synonym for nParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
int DihedralTableT<Param>::nDihedralParameters(GroupID groupid) const
{
    return this->nParameters(groupid);
}

/** Synonym for nIntraParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
int DihedralTableT<Param>::nIntraDihedralParameters() const
{
    return this->nIntraParameters();
}

/** Synonym for nInterParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
int DihedralTableT<Param>::nInterDihedralParameters() const
{
    return this->nInterParameters();
}

/** Synonym for nIntraParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
int DihedralTableT<Param>::nIntraDihedralParameters(ResNum resnum) const
{
    return this->nIntraParameters(resnum);
}

/** Synonym for nInterParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
int DihedralTableT<Param>::nInterDihedralParameters(ResNum resnum) const
{
    return this->nInterParameters(resnum);
}

/** Synonym for addInternal(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void DihedralTableT<Param>::addDihedral(const Dihedral &dihedral)
{
    this->addInternal(dihedral);
}

/** Synonym for addInternal(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void DihedralTableT<Param>::addDihedral(const Dihedral &dihedral, const Param &param)
{
    this->addInternal(dihedral,param);
}

/** Synonym for addInternals(...) */
template<class Param>
template<class C>
SIRE_INLINE_TEMPLATE
void DihedralTableT<Param>::addDihedrals(const C &dihedrals)
{
    this->addInternals(dihedrals);
}

/** Synonym for removeInternal(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void DihedralTableT<Param>::removeDihedral(const Dihedral &dihedral)
{
    this->removeInternal(dihedral);
}

/** Synonym for removeInternals(...) */
template<class Param>
template<class C>
SIRE_INLINE_TEMPLATE
void DihedralTableT<Param>::removeDihedrals(const C &dihedrals)
{
    this->removeInternals(dihedrals);
}

/** Synonym for removeInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void DihedralTableT<Param>::removeDihedrals()
{
    this->removeInternals();
}

/** Synonym for removeInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void DihedralTableT<Param>::removeDihedrals(ResNum resnum)
{
    this->removeInternals(resnum);
}

/** Synonym for removeInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void DihedralTableT<Param>::removeDihedrals(GroupID groupid)
{
    this->removeInternals(groupid);
}

/** Synonym for removeInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void DihedralTableT<Param>::removeDihedrals(const QSet<GroupID> &groupids)
{
    this->removeInternals(groupids);
}

/** Synonym for removeIntraInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void DihedralTableT<Param>::removeIntraDihedrals()
{
    this->removeIntraInternals();
}

/** Synonym for removeIntraInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void DihedralTableT<Param>::removeIntraDihedrals(ResNum resnum)
{
    this->removeIntraInternals(resnum);
}

/** Synonym for removeInterInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void DihedralTableT<Param>::removeInterDihedrals()
{
    this->removeInterInternals();
}

/** Synonym for removeInterInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void DihedralTableT<Param>::removeInterDihedrals(ResNum resnum)
{
    this->removeInterInternals(resnum);
}

/** Synonym for removeMissingInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void DihedralTableT<Param>::removeMissingDihedrals()
{
    this->removeMissingInternals();
}

/** Synonym for removeMissingInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void DihedralTableT<Param>::removeMissingDihedrals(ResNum resnum)
{
    this->removeMissingInternals(resnum);
}

/** Synonym for removeMissingInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void DihedralTableT<Param>::removeMissingDihedrals(GroupID groupid)
{
    this->removeMissingInternals(groupid);
}

/** Synonym for removeMissingInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void DihedralTableT<Param>::removeMissingDihedrals(const QSet<GroupID> &groupids)
{
    this->removeMissingInternals(groupids);
}

/** Synonym for removeMissingIntraInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void DihedralTableT<Param>::removeMissingIntraDihedrals()
{
    this->removeMissingIntraInternals();
}

/** Synonym for removeMissingIntraInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void DihedralTableT<Param>::removeMissingIntraDihedrals(ResNum resnum)
{
    this->removeMissingIntraInternals(resnum);
}

/** Synonym for removeMissingInterInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void DihedralTableT<Param>::removeMissingInterDihedrals()
{
    this->removeMissingInterInternals();
}

/** Synonym for removeMissingInterInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void DihedralTableT<Param>::removeMissingInterDihedrals(ResNum resnum)
{
    this->removeMissingInterInternals(resnum);
}

/** Synonym for clear(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void DihedralTableT<Param>::clearDihedral(const Dihedral &dihedral)
{
    this->clear(dihedral);
}

/** Synonym for clear(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void DihedralTableT<Param>::clearDihedrals()
{
    this->clear();
}

/** Synonym for clear(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void DihedralTableT<Param>::clearDihedrals(ResNum resnum)
{
    this->clear(resnum);
}

/** Synonym for clear(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void DihedralTableT<Param>::clearDihedrals(GroupID groupid)
{
    this->clear(groupid);
}

/** Synonym for clear(...) */
template<class Param>
template<class C>
SIRE_OUTOFLINE_TEMPLATE
void DihedralTableT<Param>::clearDihedrals(const C &dihedrals)
{
    this->clear(dihedrals);
}

/** Synonym for clearIntra(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void DihedralTableT<Param>::clearIntraDihedrals()
{
    this->clearIntra();
}

/** Synonym for clearIntra(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void DihedralTableT<Param>::clearIntraDihedrals(ResNum resnum)
{
    this->clearIntra(resnum);
}

/** Synonym for clearInter(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void DihedralTableT<Param>::clearInterDihedrals()
{
    this->clearInter();
}

/** Synonym for clearInter(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void DihedralTableT<Param>::clearInterDihedrals(ResNum resnum)
{
    this->clearInter(resnum);
}

/** Synonym for clearInter(...) */
template<class Param>
template<class C>
SIRE_OUTOFLINE_TEMPLATE
void DihedralTableT<Param>::clearInterDihedrals(const C &dihedrals)
{
    this->clearInter(dihedrals);
}

/** Synonym for clearIntra(...) */
template<class Param>
template<class C>
SIRE_OUTOFLINE_TEMPLATE
void DihedralTableT<Param>::clearIntraDihedrals(const C &dihedrals)
{
    this->clearIntra(dihedrals);
}

/** Synonym for internals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QVector<Dihedral> DihedralTableT<Param>::dihedrals() const
{
    return this->internals();
}

/** Synonym for internals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QVector<Dihedral> DihedralTableT<Param>::dihedrals(ResNum resnum) const
{
    return this->internals(resnum);
}

/** Synonym for internals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QVector<Dihedral> DihedralTableT<Param>::dihedrals(GroupID groupid) const
{
    return this->internals(groupid);
}

/** Synonym for interInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QVector<Dihedral> DihedralTableT<Param>::interDihedrals() const
{
    return this->interInternals();
}

/** Synonym for intraInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QVector<Dihedral> DihedralTableT<Param>::intraDihedrals() const
{
    return this->intraInternals();
}

/** Synonym for interInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QVector<Dihedral> DihedralTableT<Param>::interDihedrals(ResNum resnum) const
{
    return this->interInternals(resnum);
}

/** Synonym for intraInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QVector<Dihedral> DihedralTableT<Param>::intraDihedrals(ResNum resnum) const
{
    return this->intraInternals(resnum);
}

/** Synonym for internalsByGroup(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
GroupedVector<GroupIndexID,Dihedral> DihedralTableT<Param>::dihedralsByGroup() const
{
    return this->internalsByGroup();
}

/** Synonym for internalsByGroup(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
GroupedVector<GroupIndexID,Dihedral> DihedralTableT<Param>::dihedralsByGroup(
                                          const QSet<GroupID> &gids) const
{
    return this->internalsByGroup(gids);
}

/** Synonym for internalsByResidue(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
GroupedVector<ResNumIndexID,Dihedral> DihedralTableT<Param>::dihedralsByResidue() const
{
    return this->internalsByResidue();
}

/** Synonym for internalsByResidue(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
GroupedVector<ResNumIndexID,Dihedral> DihedralTableT<Param>::dihedralsByResidue(
                                          const QSet<ResNum> &resnums) const
{
    return this->internalsByResidue(resnums);
}

/** Synonym for interInternalsByResidue(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
GroupedVector<ResNumIndexID,Dihedral> DihedralTableT<Param>::interDihedralsByResidue() const
{
    return this->interInternalsByResidue();
}

/** Synonym for intraInternalsByResidue(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
GroupedVector<ResNumIndexID,Dihedral> DihedralTableT<Param>::intraDihedralsByResidue() const
{
    return this->intraInternalsByResidue();
}

/** Synonym for interInternalsByResidue(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
GroupedVector<ResNumIndexID,Dihedral> 
DihedralTableT<Param>::interDihedralsByResidue(const QSet<ResNum> &resnums) const
{
    return this->interInternalsByResidue(resnums);
}

/** Synonym for intraInternalsByResidue(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
GroupedVector<ResNumIndexID,Dihedral> 
DihedralTableT<Param>::intraDihedralsByResidue(const QSet<ResNum> &resnums) const
{
    return this->intraInternalsByResidue(resnums);
}

/** Synonym for getParameter(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
const Param& DihedralTableT<Param>::getDihedralParameter(const Dihedral &dihedral) const
{
    return this->getParameter(dihedral);
}

/** Synonym for getParameter(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
const Param& DihedralTableT<Param>::getDihedralParameter(const GroupIndexID &id) const
{
    return this->getParameter(id);
}

/** Synonym for setParameter(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void DihedralTableT<Param>::setDihedralParameter(const Dihedral &dihedral, const Param &param)
{
    return this->setParameter(dihedral, param);
}

/** Synonym for setParameter(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void DihedralTableT<Param>::setDihedralParameter(const GroupIndexID &id, const Param &param)
{
    return this->setParameter(id, param);
}

/** Synonym for assignedParameter(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
bool DihedralTableT<Param>::assignedDihedralParameter(const Dihedral &dihedral) const
{
    return this->assignedParameter(dihedral);
}

/** Synonym for assignedParameter(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
bool DihedralTableT<Param>::assignedDihedralParameter(const GroupIndexID &id) const
{
    return this->assignedParameter(id);
}

/** Synonym for hasMissingParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
bool DihedralTableT<Param>::hasMissingDihedralParameters() const
{
    return this->hasMissingParameters();
}

/** Synonym for hasMissingParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
bool DihedralTableT<Param>::hasMissingDihedralParameters(ResNum resnum) const
{
    return this->hasMissingParameters(resnum);
}

/** Synonym for hasMissingParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
bool DihedralTableT<Param>::hasMissingDihedralParameters(GroupID id) const
{
    return this->hasMissingParameters(id);
}

/** Synonym for hasMissingIntraParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
bool DihedralTableT<Param>::hasMissingIntraDihedralParameters() const
{
    return this->hasMissingIntraParameters();
}

/** Synonym for hasMissingIntraParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
bool DihedralTableT<Param>::hasMissingIntraDihedralParameters(ResNum resnum) const
{
    return this->hasMissingIntraParameters(resnum);
}

/** Synonym for hasMissingInterParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
bool DihedralTableT<Param>::hasMissingInterDihedralParameters() const
{
    return this->hasMissingInterParameters();
}

/** Synonym for hasMissingInterParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
bool DihedralTableT<Param>::hasMissingInterDihedralParameters(ResNum resnum) const
{
    return this->hasMissingInterParameters(resnum);
}

/** Synonym for missingParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QSet<Dihedral> DihedralTableT<Param>::missingDihedralParameters() const
{
    return this->missingParameters();
}

/** Synonym for missingParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QSet<Dihedral> DihedralTableT<Param>::missingDihedralParameters(ResNum resnum) const
{
    return this->missingParameters(resnum);
}

/** Synonym for missingParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QSet<Dihedral> DihedralTableT<Param>::missingDihedralParameters(const QSet<GroupID> &groupids) const
{
    return this->missingParameters(groupids);
}

/** Synonym for missingIntraParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QSet<Dihedral> DihedralTableT<Param>::missingIntraDihedralParameters() const
{
    return this->missingIntraParameters();
}

/** Synonym for missingIntraParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QSet<Dihedral> DihedralTableT<Param>::missingIntraDihedralParameters(ResNum resnum) const
{
    return this->missingIntraParameters(resnum);
}

/** Synonym for missingInterParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QSet<Dihedral> DihedralTableT<Param>::missingInterDihedralParameters() const
{
    return this->missingInterParameters();
}

/** Synonym for missingInterParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QSet<Dihedral> DihedralTableT<Param>::missingInterDihedralParameters(ResNum resnum) const
{
    return this->missingInterParameters(resnum);
}

/** Synonym for parameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QVector<Param> DihedralTableT<Param>::dihedralParameters() const
{
    return this->parameters();
}

/** Synonym for parameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QVector<Param> DihedralTableT<Param>::dihedralParameters(ResNum resnum) const
{
    return this->parameters(resnum);
}

/** Synonym for parameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QVector<Param> DihedralTableT<Param>::dihedralParameters(GroupID groupid) const
{
    return this->parameters(groupid);
}

/** Synonym for intraParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QVector<Param> DihedralTableT<Param>::interDihedralParameters() const
{
    return this->interParameters();
}

/** Synonym for intraParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QVector<Param> DihedralTableT<Param>::intraDihedralParameters() const
{
    return this->intraParameters();
}

/** Synonym for interParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QVector<Param> DihedralTableT<Param>::interDihedralParameters(ResNum resnum) const
{
    return this->interParameters(resnum);
}

/** Synonym for interParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QVector<Param> DihedralTableT<Param>::intraDihedralParameters(ResNum resnum) const
{
    return this->intraParameters(resnum);
}

/** Synonym for parametersByGroup(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
GroupedVector<GroupIndexID,Param> DihedralTableT<Param>::dihedralParametersByGroup() const
{
    return this->parametersByGroup();
}

/** Synonym for parametersByGroup(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
GroupedVector<GroupIndexID,Param> 
DihedralTableT<Param>::dihedralParametersByGroup(const QSet<GroupID> &gids) const
{
    return this->parametersByGroup(gids);
}

/** Synonym for parametersByResidue(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
GroupedVector<ResNumIndexID,Param> DihedralTableT<Param>::dihedralParametersByResidue() const
{
    return this->parametersByResidue();
}

/** Synonym for parametersByResidue(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
GroupedVector<ResNumIndexID,Param> 
DihedralTableT<Param>::dihedralParametersByResidue(const QSet<ResNum> &resnums) const
{
    return this->parametersByResidue(resnums);
}

/** Synonym for interParametersByResidue(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
GroupedVector<ResNumIndexID,Param> DihedralTableT<Param>::interDihedralParametersByResidue() const
{
    return this->interParametersByResidue();
}

/** Synonym for intraParametersByResidue(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
GroupedVector<ResNumIndexID,Param> DihedralTableT<Param>::intraDihedralParametersByResidue() const
{
    return this->intraParametersByResidue();
}

/** Synonym for interParametersByResidue(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
GroupedVector<ResNumIndexID,Param> 
DihedralTableT<Param>::interDihedralParametersByResidue(const QSet<ResNum> &resnums) const
{
    return this->interParametersByResidue(resnums);
}

/** Synonym for intraParametersByResidue(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
GroupedVector<ResNumIndexID,Param> 
DihedralTableT<Param>::intraDihedralParametersByResidue(const QSet<ResNum> &resnums) const
{
    return this->intraParametersByResidue(resnums);
}

const SireStream::MagicID dihedraltablet_magic = SireStream::getMagic(
                                                          "SireMM::DihedralTableT");

}

/** Serialise to a binary data stream */
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
QDataStream& operator<<(QDataStream &ds, 
                        const SireMM::DihedralTableT<Param> &table)
{
    SireStream::writeHeader(ds, SireMM::dihedraltablet_magic, 0)
             << static_cast<const SireMM::InternalTable<SireMM::MolDihedralInfo,Param>&>(table);
             
    return ds;
}

/** Deserialise from a binary data stream */
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
QDataStream& operator>>(QDataStream &ds, 
                        SireMM::DihedralTableT<Param> &table)
{
    SireStream::VersionID v = SireStream::readHeader(ds, SireMM::dihedraltablet_magic,
                                                     "SireMM::DihedralTableT");
                                                     
    if (v == 0)
    {
        ds >> static_cast<SireMM::InternalTable<SireMM::MolDihedralInfo, Param>&>(table);
    }
    else
        throw SireStream::version_error(v, "0", "SireMM::DihedralTableT", CODELOC);
        
    return ds;
}

SIRE_END_HEADER

#endif
