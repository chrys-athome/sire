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

#include "SireBase/property.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class CuttingFunction;
}

QDataStream& operator<<(QDataStream&, const SireMol::CuttingFunction&);
QDataStream& operator>>(QDataStream&, SireMol::CuttingFunction&);

namespace SireMol
{

class Molecule;
class MolStructureEditor;

/** This is the base class of all cutting functions. These are
    functions that divide a molecule up into CutGroups.
    
    @author Christopher Woods
*/
class SIREMOL_EXPORT CuttingFunctionBase : public SireBase::PropertyBase
{
public:
    CuttingFunctionBase();
    CuttingFunctionBase(const CuttingFunctionBase &other);
    
    virtual ~CuttingFunctionBase();
    
    static const char* typeName()
    {
        return "SireMol::CuttingFunctionBase";
    }

    virtual CuttingFunctionBase* clone() const=0;
    
    virtual Molecule operator()(const Molecule &molecule) const;
    virtual MolStructureEditor operator()(MolStructureEditor &moleditor) const=0;
};

/** This is the holder of a cutting function object */
class SIREMOL_EXPORT CuttingFunction : public SireBase::Property
{

friend QDataStream& ::operator<<(QDataStream&, const CuttingFunction&);
friend QDataStream& ::operator>>(QDataStream&, CuttingFunction&);

public:
    CuttingFunction();
    CuttingFunction(const CuttingFunctionBase &other);
    CuttingFunction(const SireBase::PropertyBase &property);
    
    CuttingFunction(const CuttingFunction &other);
    
    ~CuttingFunction();
    
    virtual CuttingFunction& operator=(const CuttingFunctionBase &other);
    virtual CuttingFunction& operator=(const SireBase::PropertyBase &other);
    
    static const char* typeName()
    {
        return QMetaType::typeName( qMetaTypeId<CuttingFunction>() );
    }
    
    Molecule operator()(const Molecule &molecule) const;
    MolStructureEditor operator()(MolStructureEditor &moleditor) const;
    
    const CuttingFunctionBase* operator->() const;
    const CuttingFunctionBase& operator*() const;
    
    const CuttingFunctionBase& read() const;
    CuttingFunctionBase& edit();
    
    const CuttingFunctionBase* data() const;
    const CuttingFunctionBase* constData() const;
    
    CuttingFunctionBase* data();
    
    operator const CuttingFunctionBase&() const;

    static const CuttingFunction& shared_null();    
};

}

Q_DECLARE_METATYPE( SireMol::CuttingFunction );

SIRE_EXPOSE_CLASS( SireMol::CuttingFunctionBase )

SIRE_END_HEADER

#endif
