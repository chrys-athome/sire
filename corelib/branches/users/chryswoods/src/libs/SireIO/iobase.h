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

#ifndef SIREIO_IOBASE_H
#define SIREIO_IOBASE_H

#include <QString>
#include <QIODevice>

#include "SireBase/property.h"
#include "SireBase/propertymap.h"

#include "SireMol/molecules.h"
#include "SireMol/moleculegroup.h"

SIRE_BEGIN_HEADER

namespace SireIO
{

using SireBase::PropertyMap;
using SireBase::PropertyName;

using SireMol::Molecules;
using SireMol::MoleculeGroup;
using SireMol::MolGroup;

/** This is the base class of the object that contains the 
    default sources of the properties into which this molecule
    reader/writer will place data, and the default values of
    the parameters that control how this reader/writer works.
    
    @author Christopher Woods
*/
class SIREIO_EXPORT IOParametersBase
{
public:
    IOParametersBase();
    ~IOParametersBase();
    
    /** Return the default name of the property into which
        the coordinates of the atoms will be placed 
        
        default == "coordinates"
    */
    const PropertyName& coordinates() const
    {
        return coords_property;
    }
    
    /** Return the default source of the property into which
        the chemical elements of each atom will be placed 
        
        default == "element"
    */
    const PropertyName& element() const
    {
        return elements_property;
    }
    
    /** Return the function used to split the molecule
        into CutGroups (must be an object of type CuttingFunction)
        
        source  == "cutting-function"
        default == ResidueCutting()
    */
    const PropertyName& cuttingFunction() const
    {
        return cutting_function;
    }
    
private:
    ///////
    /////// Properties that hold the data of the molecule
    ///////

    /** The default property in which to store the coordinates */
    static PropertyName coords_property;
    
    /** The default property in which to store the elements */
    static PropertyName elements_property;
    
    ///////
    /////// Parameters that control how the reader works
    ///////
    
    /** The function used to split a molecule into CutGroups
         - by default a ResidueCutting() function is used that
           places each residue into its own CutGroup */
    static PropertyName cutting_function;
};

/** This is the virtual base class of all molecule readers and writers.

    @author Christopher Woods
*/
class SIREIO_EXPORT IOBase : public SireBase::PropertyBase
{
public:
    IOBase();
    IOBase(const IOBase &other);
    
    virtual ~IOBase();

    static const char* typeName()
    {
        return "SireIO::IOBase";
    }

    virtual IOBase* clone() const=0;

    MoleculeGroup read(const QString &filename,
                       const PropertyMap &map = PropertyMap()) const;

    MoleculeGroup read(QIODevice &dev,
                       const PropertyMap &map = PropertyMap()) const;

    void write(const MolGroup &molecules, const QString &filename,
               const PropertyMap &map = PropertyMap()) const;

    void write(const Molecules &molecules, const QString &filename,
               const PropertyMap &map = PropertyMap()) const;

    void write(const MolGroup &molecules, QIODevice &dev,
               const PropertyMap &map = PropertyMap()) const;

    void write(const Molecules &molecules, QIODevice &dev,
               const PropertyMap &map = PropertyMap()) const;

protected:

    /**
      * You need to override the below two functions for your custom
      * IO class.
      *
      */

    /** Read and return some Molecules from the data */
    virtual MoleculeGroup readMols(const QByteArray &data,
                                   const PropertyMap &map) const=0;

    /** Write the Molecules to a QByteArray and return it */
    virtual QByteArray writeMols(const MolGroup &molecules,
                                 const PropertyMap &map) const=0;

    /** Write the Molecules to a QByteArray and return it */
    virtual QByteArray writeMols(const Molecules &molecules,
                                 const PropertyMap &map) const=0;
};

}

SIRE_END_HEADER

#endif
