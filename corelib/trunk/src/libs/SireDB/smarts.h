#ifndef SIREDB_SMARTS_H
#define SIREDB_SMARTS_H

#include <QString>

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireDB
{

/** This class will eventually provide Smarts string functionality
    for atom searching (of Smiles strings). For now, this is just 
    a placeholder.
    
    It is very much my intention to provide a Smiles and Smarts
    parser/searcher so that it will be possible to describe
    an atom using a Smarts string, and to use this string to 
    automatically assign basic parameters, e.g. atom types,
    LJ parameters and semi-empirical QM parameters.
    
    Building a Smiles/Smarts system into the code may also
    provide other interesting opportunities for use, e.g.
    searching in Spier. It will however require a significant
    investment of time, and also perhaps some changes to
    the Molecule and EditMol classes (as neither of these
    have concepts of bond type, nor of the formal 
    charge of their constituent atoms).
    
    @author Christopher Woods
*/
class SIREDB_EXPORT Smarts
{
public:
    Smarts();
    
    Smarts(const Smarts &other);
    
    ~Smarts();

    bool operator==(const Smarts &other) const;

    QString toString() const;

    bool isEmpty() const;

};

}

SIRE_END_HEADER

#endif
