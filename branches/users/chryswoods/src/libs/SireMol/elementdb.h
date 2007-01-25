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

#ifndef SIREMOL_ELEMENTDB_H
#define SIREMOL_ELEMENTDB_H

#include <QString>
#include <QHash>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireMol
{
class Element;
}

class QDataStream;
QDataStream& operator<<(QDataStream&, const SireMol::Element&);
QDataStream& operator>>(QDataStream&, SireMol::Element&);

namespace SireMol
{

class ElementData;

/**
  * This class is used to represent a chemical element. The implementation
  * of this class is such that it is quick and easy to pass and copy
  * Elements, while the storage requirements are very low (just a single
  * pointer). The actual element data is held in a private ElementData
  * class that is declared in elementdb.cpp
  *
  * @author Christopher Woods
  */
class SIREMOL_EXPORT Element
{

public:
    /** Construct a dummy element */
    Element();
    /** Construct an element from the string 'element'. If the string
        contains 1 or 2 characters, then it is interpreted as an IUPAC
        chemical symbol (e.g. 'C', or 'Al'), else it is interpreted as
        the name of the element in the local language of the application.
        If the string cannot be interpreted then the element is set to
        the dummy element. Note 'ca' is Calcium, not C-alpha! */
    Element(QString element);
    /** Overload so that 'const char*' is interpreted as a QString, and not
        as an unsigned int */
    Element(const char *element);
    /** Construct an element with proton number 'nprot'. If there is no
        element with this number of protons, or if the proton number is 0,
        then a dummy elements is constructed */
    Element(unsigned int nprotons);
    /** Overload to disambiguate the call to Element(int) */
    Element(int nprotons);
    /** Copy constructor. This is very quick as it involves copying
        only a single pointer. */
    Element(const Element &element);
    
    ~Element();
    
    /** Comparison operators. These are very quick */
    bool operator==(const Element &other) const;
    bool operator!=(const Element &other) const;
    
    /** Sorting operators. Elements are compared based on their
        proton numbers, with elements with greater numbers being higher.
        The functions are also very quick. */
    bool operator>(const Element &other) const;
    bool operator<(const Element &other) const;
    bool operator>=(const Element &other) const;
    bool operator<=(const Element &other) const;
    
    /** Assignment operator */
    const Element& operator=(const Element &element);

    /** Return the number of protons in the element */    
    unsigned int nProtons() const;
    /** Return the IUPAC symbol for the element */
    QString symbol() const;
    /** Return the name of the element in the local language */
    QString name() const;
    /** Return a string representation of the Element */
    QString toString() const;
    /** Return the element's covalent radius */
    double covalentRadius() const;
    /** Return the bond order radius */
    double bondOrderRadius() const;
    /** Return the van der waals radius */
    double vdwRadius() const;
    /** Return the maximum number of simultaneous bonds that this
        element can form */
    unsigned int maxBonds() const;
    /** Return the average mass of this element */
    double mass() const;
    /** Return the element's electronegativity */
    double electroNegativity() const;
    /** Return the red/green/blue colour components (0.0->1.0) for
        the colour of this element */
    float red() const;
    float green() const;
    float blue() const;
    
    int group() const;
    int period() const;
    
    bool nobleGas() const;
    bool halogen() const;
    bool biological() const;
    bool alkaliMetal() const;
    bool alkaliEarthMetal() const;
    bool transitionMetal() const;
    bool lanthanide() const;
    bool actinide() const;
    bool rareEarth() const;
    
    /** Return an element which has the closest mass to 'mass' (in atomic 
        mass units, g mol-1) */
    static Element elementWithMass(double mass);
    
    static Element biologicalElement(const QString &name);
    
private:
    /** Pointer to the object containing the data for this element */
    ElementData *eldata;
};

inline bool Element::operator==(const Element &other) const
{
    return eldata == other.eldata;
}

inline bool Element::operator!=(const Element &other) const
{
    return eldata != other.eldata;
}

/**
This singleton class holds a database of element properties and information indexed by atomic number or element symbol. You must initialise this database before you can create any Element objects.
 
@author Christopher Woods
*/
class ElementDB
{
friend class Element;

public:
    /** You must call this function before you can access the database */
    static void initialise();
protected:
    ElementDB();
    ~ElementDB();
    
    /** Populate this database with data. This function is defined
        in element-data.h, which is automatically generated from the
        openbabel data. */
    void populate();

    /** Import an individual element to the database */
    void import(ElementData *element);
      
    /** Return the ElementData for the string 'element'. See the QString
        constructor for Element for a description of how 'element' is 
        interpreted. This will never return a null pointer. */
    ElementData* element(const QString &element) const;
    
    /** Return the element with proton number 'nprotons'. This will
        never return a null pointer. */
    ElementData* element(unsigned int nprotons) const;
    
    /** Pointer to the single instance of this class */
    static ElementDB *db;

    /** A hash indexing the elements by proton number */
    QHash<unsigned int, ElementData*> protonindex;
    
    /** A hash indexing the elements by symbol */
    QHash<QString, ElementData*> symbolindex;
    /** A hash indexing the elements by their name */
    QHash<QString, ElementData*> nameindex;
};

}

inline uint qHash(const SireMol::Element &element)
{
    return element.nProtons();
}

Q_DECLARE_METATYPE(SireMol::Element)

SIRE_END_HEADER

#endif
