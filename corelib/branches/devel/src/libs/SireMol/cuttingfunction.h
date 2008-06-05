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

#ifndef SIREMOL_CUTTINGFUNCTION_H
#define SIREMOL_CUTTINGFUNCTION_H

#include "SireBase/sharedpolypointer.hpp"

SIRE_BEGIN_HEADER

namespace SireMol
{
class CuttingFunction;
class CuttingFunctionBase;
}

QDataStream& operator<<(QDataStream&, const SireMol::CuttingFunction&);
QDataStream& operator>>(QDataStream&, SireMol::CuttingFunction&);

QDataStream& operator<<(QDataStream&, const SireMol::CuttingFunctionBase&);
QDataStream& operator>>(QDataStream&, SireMol::CuttingFunctionBase&);

namespace SireMol
{

class CutGroupNum;
class AtomIndex;
class EditMolData;


/** This is the virtual base class of all of the Cutting functions
    (e.g. ResidueCutting and MoleculeCutting)

    @author Christopher Woods
*/
class SIREMOL_EXPORT CuttingFunctionBase : public QSharedData
{

friend QDataStream& ::operator<<(QDataStream&, const CuttingFunctionBase&);
friend QDataStream& ::operator>>(QDataStream&, CuttingFunctionBase&);

public:
    CuttingFunctionBase();

    CuttingFunctionBase(const CuttingFunctionBase &other);

    virtual ~CuttingFunctionBase();

    virtual CutGroupNum operator()(const AtomIndex &atom,
                                   const EditMolData &moldata) const=0;

    static const char* typeName()
    {
        return "SireMol::CuttingFunctionBase";
    }

    virtual const char* what() const=0;

    virtual CuttingFunctionBase* clone() const=0;
};

/** This is a function class, which is used to return the number of
    the CutGroup into which the passed atom should be placed
    (in the molecule represented by the passed EditMolData)

    @author Christopher Woods
*/
class SIREMOL_EXPORT CuttingFunction
{

friend QDataStream& ::operator<<(QDataStream&, const CuttingFunction&);
friend QDataStream& ::operator>>(QDataStream&, CuttingFunction&);

public:
    CuttingFunction();
    CuttingFunction(const CuttingFunctionBase &func);

    CuttingFunction(const CuttingFunction &other);

    ~CuttingFunction();

    CutGroupNum operator()(const AtomIndex &atom,
                           const EditMolData &moldata) const;

    const char* what() const;

private:
    /** Shared pointer to the actual function that is used to
        calculate the CutGroupNum */
    SireBase::SharedPolyPointer<CuttingFunctionBase> func;
};

}

Q_DECLARE_METATYPE(SireMol::CuttingFunction)

SIRE_END_HEADER

#endif
