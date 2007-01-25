/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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

#ifndef SIREMM_ANGLETABLE_HPP
#define SIREMM_ANGLETABLE_HPP

#include "internaltable.hpp"
#include "molangleinfo.h"

SIRE_BEGIN_HEADER

namespace SireMM
{
template<class Param>
class AngleTableT;
}

template<class Param>
QDataStream& operator<<(QDataStream&, const SireMM::AngleTableT<Param>&);
template<class Param>
QDataStream& operator>>(QDataStream&, SireMM::AngleTableT<Param>&);

namespace SireMM
{

using SireMol::Angle;

/**
This class holds all of the parameters that are used to obtain the energy of the angles in the molecule.

This function is just an overload of InternalTable<MolAngleInfo,Param> that adds some nice synactic sugar with functions named 'addAngle', 'removeAngle' etc., rather than addParameter, removeParameter. This enables this class to be easily used with other InternalTable-derived classes without too many function-name clashing problems.

@author Christopher Woods
*/
template<class Param>
class AngleTableT : public InternalTable<MolAngleInfo,Param>
{
public:
    AngleTableT();
    AngleTableT(const MolAngleInfo &angleinfo);

    AngleTableT(const AngleTableT<Param> &other);

    ~AngleTableT();

    int nAngles() const;
    int nAngles(ResNum resnum) const;
    int nAngles(GroupID groupid) const;
    int nIntraAngles() const;
    int nInterAngles() const;
    int nIntraAngles(ResNum resnum) const;
    int nInterAngles(ResNum resnum) const;

    int nAngleParameters() const;
    int nAngleParameters(ResNum resnum) const;
    int nAngleParameters(GroupID groupid) const;
    int nIntraAngleParameters() const;
    int nInterAngleParameters() const;
    int nIntraAngleParameters(ResNum resnum) const;
    int nInterAngleParameters(ResNum resnum) const;

    void addAngle(const Angle &angle);
    void addAngle(const Angle &angle, const Param &param);

    template<class C>
    void addAngles(const C &angles);

    void removeAngle(const Angle &angle);

    template<class C>
    void removeAngles(const C &angles);

    void removeAngles();
    void removeAngles(ResNum resnum);
    void removeAngles(GroupID groupid);
    void removeAngles(const QSet<GroupID> &groupids);

    void removeIntraAngles();
    void removeIntraAngles(ResNum resnum);

    void removeInterAngles();
    void removeInterAngles(ResNum resnum);

    void removeMissingAngles();
    void removeMissingAngles(ResNum resnum);
    void removeMissingAngles(GroupID groupid);
    void removeMissingAngles(const QSet<GroupID> &groupids);

    void removeMissingIntraAngles();
    void removeMissingIntraAngles(ResNum resnum);
    void removeMissingInterAngles();
    void removeMissingInterAngles(ResNum resnum);

    void clearAngle(const Angle &angle);
    void clearAngles();
    void clearAngles(ResNum resnum);
    void clearAngles(GroupID groupid);

    template<class C>
    void clearAngles(const C &angles);

    void clearIntraAngles();
    void clearIntraAngles(ResNum resnum);

    void clearInterAngles();
    void clearInterAngles(ResNum resnum);

    template<class C>
    void clearIntraAngles(const C &angles);
    template<class C>
    void clearInterAngles(const C &angles);

    QVector<Angle> angles() const;
    QVector<Angle> angles(ResNum resnum) const;
    QVector<Angle> angles(GroupID groupid) const;

    QVector<Angle> interAngles() const;
    QVector<Angle> intraAngles() const;
    QVector<Angle> interAngles(ResNum resnum) const;
    QVector<Angle> intraAngles(ResNum resnum) const;

    GroupedVector<GroupIndexID,Angle> anglesByGroup() const;
    GroupedVector<GroupIndexID,Angle> anglesByGroup(const QSet<GroupID> &gids) const;
    GroupedVector<ResNumIndexID,Angle> anglesByResidue() const;
    GroupedVector<ResNumIndexID,Angle> anglesByResidue(const QSet<ResNum> &resnums) const;

    GroupedVector<ResNumIndexID,Angle> interAnglesByResidue() const;
    GroupedVector<ResNumIndexID,Angle> intraAnglesByResidue() const;
    GroupedVector<ResNumIndexID,Angle> interAnglesByResidue(const QSet<ResNum> &resnums) const;
    GroupedVector<ResNumIndexID,Angle> intraAnglesByResidue(const QSet<ResNum> &resnums) const;

    const Param& getAngleParameter(const Angle &angle) const;
    const Param& getAngleParameter(const GroupIndexID &id) const;

    void setAngleParameter(const Angle &angle, const Param &param);
    void setAngleParameter(const GroupIndexID &id, const Param &param);

    bool assignedAngleParameter(const Angle &angle) const;
    bool assignedAngleParameter(const GroupIndexID &id) const;

    bool hasMissingAngleParameters() const;
    bool hasMissingAngleParameters(ResNum resnum) const;
    bool hasMissingAngleParameters(GroupID id) const;

    bool hasMissingIntraAngleParameters() const;
    bool hasMissingIntraAngleParameters(ResNum resnum) const;

    bool hasMissingInterAngleParameters() const;
    bool hasMissingInterAngleParameters(ResNum resnum) const;

    QSet<Angle> missingAngleParameters() const;
    QSet<Angle> missingAngleParameters(ResNum resnum) const;
    QSet<Angle> missingAngleParameters(const QSet<GroupID> &groupids) const;

    QSet<Angle> missingIntraAngleParameters() const;
    QSet<Angle> missingIntraAngleParameters(ResNum resnum) const;

    QSet<Angle> missingInterAngleParameters() const;
    QSet<Angle> missingInterAngleParameters(ResNum resnum) const;

    QVector<Param> angleParameters() const;
    QVector<Param> angleParameters(ResNum resnum) const;
    QVector<Param> angleParameters(GroupID groupid) const;

    QVector<Param> interAngleParameters() const;
    QVector<Param> intraAngleParameters() const;
    QVector<Param> interAngleParameters(ResNum resnum) const;
    QVector<Param> intraAngleParameters(ResNum resnum) const;

    GroupedVector<GroupIndexID,Param> angleParametersByGroup() const;
    GroupedVector<GroupIndexID,Param> angleParametersByGroup(
                                                const QSet<GroupID> &gids) const;
    GroupedVector<ResNumIndexID,Param> angleParametersByResidue() const;
    GroupedVector<ResNumIndexID,Param> angleParametersByResidue(
                                                const QSet<ResNum> &resnums) const;

    GroupedVector<ResNumIndexID,Param> interAngleParametersByResidue() const;
    GroupedVector<ResNumIndexID,Param> intraAngleParametersByResidue() const;
    GroupedVector<ResNumIndexID,Param> interAngleParametersByResidue(
                                                const QSet<ResNum> &resnums) const;
    GroupedVector<ResNumIndexID,Param> intraAngleParametersByResidue(
                                                const QSet<ResNum> &resnums) const;
};

/** Empty constructor */
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
AngleTableT<Param>::AngleTableT() : InternalTable<MolAngleInfo,Param>()
{}

/** Create a table to hold the parameters for the angles described by 'angleinfo' */
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
AngleTableT<Param>::AngleTableT(const MolAngleInfo &angleinfo)
                  : InternalTable<MolAngleInfo,Param>(angleinfo)
{}

/** Copy constructor */
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
AngleTableT<Param>::AngleTableT(const AngleTableT<Param> &other)
                  : InternalTable<MolAngleInfo,Param>(other)
{}

/** Destructor */
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
AngleTableT<Param>::~AngleTableT()
{}

/** Synonym for nInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
int AngleTableT<Param>::nAngles() const
{
    return this->nInternals();
}

/** Synonym for nInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
int AngleTableT<Param>::nAngles(ResNum resnum) const
{
    return this->nInternals(resnum);
}

/** Synonym for nInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
int AngleTableT<Param>::nAngles(GroupID groupid) const
{
    return this->nInternals(groupid);
}

/** Synonym for nIntraInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
int AngleTableT<Param>::nIntraAngles() const
{
    return this->nIntraInternals();
}

/** Synonym for nInterInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
int AngleTableT<Param>::nInterAngles() const
{
    return this->nInterInternals();
}

/** Synonym for nIntraInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
int AngleTableT<Param>::nIntraAngles(ResNum resnum) const
{
    return this->nIntraInternals(resnum);
}

/** Synonym for nInterInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
int AngleTableT<Param>::nInterAngles(ResNum resnum) const
{
    return this->nInterInternals(resnum);
}

/** Synonym for nParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
int AngleTableT<Param>::nAngleParameters() const
{
    return this->nParameters();
}

/** Synonym for nParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
int AngleTableT<Param>::nAngleParameters(ResNum resnum) const
{
    return this->nParameters(resnum);
}

/** Synonym for nParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
int AngleTableT<Param>::nAngleParameters(GroupID groupid) const
{
    return this->nParameters(groupid);
}

/** Synonym for nIntraParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
int AngleTableT<Param>::nIntraAngleParameters() const
{
    return this->nIntraParameters();
}

/** Synonym for nInterParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
int AngleTableT<Param>::nInterAngleParameters() const
{
    return this->nInterParameters();
}

/** Synonym for nIntraParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
int AngleTableT<Param>::nIntraAngleParameters(ResNum resnum) const
{
    return this->nIntraParameters(resnum);
}

/** Synonym for nInterParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
int AngleTableT<Param>::nInterAngleParameters(ResNum resnum) const
{
    return this->nInterParameters(resnum);
}

/** Synonym for addInternal(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void AngleTableT<Param>::addAngle(const Angle &angle)
{
    this->addInternal(angle);
}

/** Synonym for addInternal(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void AngleTableT<Param>::addAngle(const Angle &angle, const Param &param)
{
    this->addInternal(angle,param);
}

/** Synonym for addInternals(...) */
template<class Param>
template<class C>
SIRE_INLINE_TEMPLATE
void AngleTableT<Param>::addAngles(const C &angles)
{
    this->addInternals(angles);
}

/** Synonym for removeInternal(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void AngleTableT<Param>::removeAngle(const Angle &angle)
{
    this->removeInternal(angle);
}

/** Synonym for removeInternals(...) */
template<class Param>
template<class C>
SIRE_INLINE_TEMPLATE
void AngleTableT<Param>::removeAngles(const C &angles)
{
    this->removeInternals(angles);
}

/** Synonym for removeInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void AngleTableT<Param>::removeAngles()
{
    this->removeInternals();
}

/** Synonym for removeInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void AngleTableT<Param>::removeAngles(ResNum resnum)
{
    this->removeInternals(resnum);
}

/** Synonym for removeInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void AngleTableT<Param>::removeAngles(GroupID groupid)
{
    this->removeInternals(groupid);
}

/** Synonym for removeInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void AngleTableT<Param>::removeAngles(const QSet<GroupID> &groupids)
{
    this->removeInternals(groupids);
}

/** Synonym for removeIntraInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void AngleTableT<Param>::removeIntraAngles()
{
    this->removeIntraInternals();
}

/** Synonym for removeIntraInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void AngleTableT<Param>::removeIntraAngles(ResNum resnum)
{
    this->removeIntraInternals(resnum);
}

/** Synonym for removeInterInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void AngleTableT<Param>::removeInterAngles()
{
    this->removeInterInternals();
}

/** Synonym for removeInterInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void AngleTableT<Param>::removeInterAngles(ResNum resnum)
{
    this->removeInterInternals(resnum);
}

/** Synonym for removeMissingInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void AngleTableT<Param>::removeMissingAngles()
{
    this->removeMissingInternals();
}

/** Synonym for removeMissingInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void AngleTableT<Param>::removeMissingAngles(ResNum resnum)
{
    this->removeMissingInternals(resnum);
}

/** Synonym for removeMissingInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void AngleTableT<Param>::removeMissingAngles(GroupID groupid)
{
    this->removeMissingInternals(groupid);
}

/** Synonym for removeMissingInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void AngleTableT<Param>::removeMissingAngles(const QSet<GroupID> &groupids)
{
    this->removeMissingInternals(groupids);
}

/** Synonym for removeMissingIntraInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void AngleTableT<Param>::removeMissingIntraAngles()
{
    this->removeMissingIntraInternals();
}

/** Synonym for removeMissingIntraInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void AngleTableT<Param>::removeMissingIntraAngles(ResNum resnum)
{
    this->removeMissingIntraInternals(resnum);
}

/** Synonym for removeMissingInterInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void AngleTableT<Param>::removeMissingInterAngles()
{
    this->removeMissingInterInternals();
}

/** Synonym for removeMissingInterInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void AngleTableT<Param>::removeMissingInterAngles(ResNum resnum)
{
    this->removeMissingInterInternals(resnum);
}

/** Synonym for clear(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void AngleTableT<Param>::clearAngle(const Angle &angle)
{
    this->clear(angle);
}

/** Synonym for clear(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void AngleTableT<Param>::clearAngles()
{
    this->clear();
}

/** Synonym for clear(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void AngleTableT<Param>::clearAngles(ResNum resnum)
{
    this->clear(resnum);
}

/** Synonym for clear(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void AngleTableT<Param>::clearAngles(GroupID groupid)
{
    this->clear(groupid);
}

/** Synonym for clear(...) */
template<class Param>
template<class C>
SIRE_OUTOFLINE_TEMPLATE
void AngleTableT<Param>::clearAngles(const C &angles)
{
    this->clear(angles);
}

/** Synonym for clearIntra(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void AngleTableT<Param>::clearIntraAngles()
{
    this->clearIntra();
}

/** Synonym for clearIntra(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void AngleTableT<Param>::clearIntraAngles(ResNum resnum)
{
    this->clearIntra(resnum);
}

/** Synonym for clearInter(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void AngleTableT<Param>::clearInterAngles()
{
    this->clearInter();
}

/** Synonym for clearInter(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void AngleTableT<Param>::clearInterAngles(ResNum resnum)
{
    this->clearInter(resnum);
}

/** Synonym for clearInter(...) */
template<class Param>
template<class C>
SIRE_OUTOFLINE_TEMPLATE
void AngleTableT<Param>::clearInterAngles(const C &angles)
{
    this->clearInter(angles);
}

/** Synonym for clearIntra(...) */
template<class Param>
template<class C>
SIRE_OUTOFLINE_TEMPLATE
void AngleTableT<Param>::clearIntraAngles(const C &angles)
{
    this->clearIntra(angles);
}

/** Synonym for internals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QVector<Angle> AngleTableT<Param>::angles() const
{
    return this->internals();
}

/** Synonym for internals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QVector<Angle> AngleTableT<Param>::angles(ResNum resnum) const
{
    return this->internals(resnum);
}

/** Synonym for internals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QVector<Angle> AngleTableT<Param>::angles(GroupID groupid) const
{
    return this->internals(groupid);
}

/** Synonym for interInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QVector<Angle> AngleTableT<Param>::interAngles() const
{
    return this->interInternals();
}

/** Synonym for intraInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QVector<Angle> AngleTableT<Param>::intraAngles() const
{
    return this->intraInternals();
}

/** Synonym for interInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QVector<Angle> AngleTableT<Param>::interAngles(ResNum resnum) const
{
    return this->interInternals(resnum);
}

/** Synonym for intraInternals(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QVector<Angle> AngleTableT<Param>::intraAngles(ResNum resnum) const
{
    return this->intraInternals(resnum);
}

/** Synonym for internalsByGroup(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
GroupedVector<GroupIndexID,Angle> AngleTableT<Param>::anglesByGroup() const
{
    return this->internalsByGroup();
}

/** Synonym for internalsByGroup(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
GroupedVector<GroupIndexID,Angle> AngleTableT<Param>::anglesByGroup(
                                          const QSet<GroupID> &gids) const
{
    return this->internalsByGroup(gids);
}

/** Synonym for internalsByResidue(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
GroupedVector<ResNumIndexID,Angle> AngleTableT<Param>::anglesByResidue() const
{
    return this->internalsByResidue();
}

/** Synonym for internalsByResidue(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
GroupedVector<ResNumIndexID,Angle> AngleTableT<Param>::anglesByResidue(
                                          const QSet<ResNum> &resnums) const
{
    return this->internalsByResidue(resnums);
}

/** Synonym for interInternalsByResidue(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
GroupedVector<ResNumIndexID,Angle> AngleTableT<Param>::interAnglesByResidue() const
{
    return this->interInternalsByResidue();
}

/** Synonym for intraInternalsByResidue(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
GroupedVector<ResNumIndexID,Angle> AngleTableT<Param>::intraAnglesByResidue() const
{
    return this->intraInternalsByResidue();
}

/** Synonym for interInternalsByResidue(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
GroupedVector<ResNumIndexID,Angle>
AngleTableT<Param>::interAnglesByResidue(const QSet<ResNum> &resnums) const
{
    return this->interInternalsByResidue(resnums);
}

/** Synonym for intraInternalsByResidue(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
GroupedVector<ResNumIndexID,Angle>
AngleTableT<Param>::intraAnglesByResidue(const QSet<ResNum> &resnums) const
{
    return this->intraInternalsByResidue(resnums);
}

/** Synonym for getParameter(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
const Param& AngleTableT<Param>::getAngleParameter(const Angle &angle) const
{
    return this->getParameter(angle);
}

/** Synonym for getParameter(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
const Param& AngleTableT<Param>::getAngleParameter(const GroupIndexID &id) const
{
    return this->getParameter(id);
}

/** Synonym for setParameter(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void AngleTableT<Param>::setAngleParameter(const Angle &angle, const Param &param)
{
    return this->setParameter(angle, param);
}

/** Synonym for setParameter(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
void AngleTableT<Param>::setAngleParameter(const GroupIndexID &id, const Param &param)
{
    return this->setParameter(id, param);
}

/** Synonym for assignedParameter(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
bool AngleTableT<Param>::assignedAngleParameter(const Angle &angle) const
{
    return this->assignedParameter(angle);
}

/** Synonym for assignedParameter(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
bool AngleTableT<Param>::assignedAngleParameter(const GroupIndexID &id) const
{
    return this->assignedParameter(id);
}

/** Synonym for hasMissingParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
bool AngleTableT<Param>::hasMissingAngleParameters() const
{
    return this->hasMissingParameters();
}

/** Synonym for hasMissingParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
bool AngleTableT<Param>::hasMissingAngleParameters(ResNum resnum) const
{
    return this->hasMissingParameters(resnum);
}

/** Synonym for hasMissingParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
bool AngleTableT<Param>::hasMissingAngleParameters(GroupID id) const
{
    return this->hasMissingParameters(id);
}

/** Synonym for hasMissingIntraParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
bool AngleTableT<Param>::hasMissingIntraAngleParameters() const
{
    return this->hasMissingIntraParameters();
}

/** Synonym for hasMissingIntraParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
bool AngleTableT<Param>::hasMissingIntraAngleParameters(ResNum resnum) const
{
    return this->hasMissingIntraParameters(resnum);
}

/** Synonym for hasMissingInterParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
bool AngleTableT<Param>::hasMissingInterAngleParameters() const
{
    return this->hasMissingInterParameters();
}

/** Synonym for hasMissingInterParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
bool AngleTableT<Param>::hasMissingInterAngleParameters(ResNum resnum) const
{
    return this->hasMissingInterParameters(resnum);
}

/** Synonym for missingParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QSet<Angle> AngleTableT<Param>::missingAngleParameters() const
{
    return this->missingParameters();
}

/** Synonym for missingParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QSet<Angle> AngleTableT<Param>::missingAngleParameters(ResNum resnum) const
{
    return this->missingParameters(resnum);
}

/** Synonym for missingParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QSet<Angle> AngleTableT<Param>::missingAngleParameters(const QSet<GroupID> &groupids) const
{
    return this->missingParameters(groupids);
}

/** Synonym for missingIntraParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QSet<Angle> AngleTableT<Param>::missingIntraAngleParameters() const
{
    return this->missingIntraParameters();
}

/** Synonym for missingIntraParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QSet<Angle> AngleTableT<Param>::missingIntraAngleParameters(ResNum resnum) const
{
    return this->missingIntraParameters(resnum);
}

/** Synonym for missingInterParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QSet<Angle> AngleTableT<Param>::missingInterAngleParameters() const
{
    return this->missingInterParameters();
}

/** Synonym for missingInterParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QSet<Angle> AngleTableT<Param>::missingInterAngleParameters(ResNum resnum) const
{
    return this->missingInterParameters(resnum);
}

/** Synonym for parameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QVector<Param> AngleTableT<Param>::angleParameters() const
{
    return this->parameters();
}

/** Synonym for parameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QVector<Param> AngleTableT<Param>::angleParameters(ResNum resnum) const
{
    return this->parameters(resnum);
}

/** Synonym for parameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QVector<Param> AngleTableT<Param>::angleParameters(GroupID groupid) const
{
    return this->parameters(groupid);
}

/** Synonym for intraParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QVector<Param> AngleTableT<Param>::interAngleParameters() const
{
    return this->interParameters();
}

/** Synonym for intraParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QVector<Param> AngleTableT<Param>::intraAngleParameters() const
{
    return this->intraParameters();
}

/** Synonym for interParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QVector<Param> AngleTableT<Param>::interAngleParameters(ResNum resnum) const
{
    return this->interParameters(resnum);
}

/** Synonym for interParameters(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
QVector<Param> AngleTableT<Param>::intraAngleParameters(ResNum resnum) const
{
    return this->intraParameters(resnum);
}

/** Synonym for parametersByGroup(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
GroupedVector<GroupIndexID,Param> AngleTableT<Param>::angleParametersByGroup() const
{
    return this->parametersByGroup();
}

/** Synonym for parametersByGroup(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
GroupedVector<GroupIndexID,Param>
AngleTableT<Param>::angleParametersByGroup(const QSet<GroupID> &gids) const
{
    return this->parametersByGroup(gids);
}

/** Synonym for parametersByResidue(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
GroupedVector<ResNumIndexID,Param> AngleTableT<Param>::angleParametersByResidue() const
{
    return this->parametersByResidue();
}

/** Synonym for parametersByResidue(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
GroupedVector<ResNumIndexID,Param>
AngleTableT<Param>::angleParametersByResidue(const QSet<ResNum> &resnums) const
{
    return this->parametersByResidue(resnums);
}

/** Synonym for interParametersByResidue(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
GroupedVector<ResNumIndexID,Param> AngleTableT<Param>::interAngleParametersByResidue() const
{
    return this->interParametersByResidue();
}

/** Synonym for intraParametersByResidue(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
GroupedVector<ResNumIndexID,Param> AngleTableT<Param>::intraAngleParametersByResidue() const
{
    return this->intraParametersByResidue();
}

/** Synonym for interParametersByResidue(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
GroupedVector<ResNumIndexID,Param>
AngleTableT<Param>::interAngleParametersByResidue(const QSet<ResNum> &resnums) const
{
    return this->interParametersByResidue(resnums);
}

/** Synonym for intraParametersByResidue(...) */
template<class Param>
SIRE_INLINE_TEMPLATE
GroupedVector<ResNumIndexID,Param>
AngleTableT<Param>::intraAngleParametersByResidue(const QSet<ResNum> &resnums) const
{
    return this->intraParametersByResidue(resnums);
}

const MagicID angletablet_magic = getMagic("SireMM::AngleTableT");

}

/** Serialise to a binary data stream */
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
QDataStream& operator<<(QDataStream &ds,
                        const SireMM::AngleTableT<Param> &table)
{
    SireStream::writeHeader(ds, SireMM::angletablet_magic, 0)
             << static_cast<const SireMM::InternalTable<SireMM::MolAngleInfo,Param>&>(table);

    return ds;
}

/** Deserialise from a binary data stream */
template<class Param>
SIRE_OUTOFLINE_TEMPLATE
QDataStream& operator>>(QDataStream &ds,
                        SireMM::AngleTableT<Param> &table)
{
    SireStream::VersionID v = SireStream::readHeader(ds, SireMM::angletablet_magic,
                                                     "SireMM::AngleTableT");

    if (v == 0)
    {
        ds >> static_cast<SireMM::InternalTable<SireMM::MolAngleInfo, Param>&>(table);
    }
    else
        throw SireStream::version_error(v, "0", "SireMM::AngleTableT", CODELOC);

    return ds;
}

SIRE_END_HEADER

#endif
