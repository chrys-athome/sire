#ifndef SIREFF_CHANGERECORD_H
#define SIREFF_CHANGERECORD_H
/**
  * @file
  *
  * C++ Interface: ChangeRecord
  *
  * Description: 
  * File contains the interface to ChangeRecord.
  * 
  * Author: Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */

#include "SireBase/dynamicsharedptr.hpp"

#include "changerecordbase.h"

SIRE_BEGIN_HEADER

namespace SireFF
{
class ChangeRecord;
}

QDataStream& operator<<(QDataStream&, const SireFF::ChangeRecord&);
QDataStream& operator>>(QDataStream&, SireFF::ChangeRecord&);

namespace SireFF
{

/**
This class holds the record of how a molecule has been changed. This class is implicitly shared and uses a polymorphic container class (ChangeRecordBase) to hold the actual information.

@author Christopher Woods
*/
class ChangeRecord
{

friend QDataStream& ::operator<<(QDataStream&, const ChangeRecord&);
friend QDataStream& ::operator>>(QDataStream&, ChangeRecord&);

public:
    ChangeRecord();

    ~ChangeRecord();
    
    const char* what() const;
    
    /** Return whether or not this is a change record of type 'T' */
    template<class T>
    bool isA() const
    {
        return d->isA<T>();
    }

    /** Return this record cast as type 'T' - will have undefined
        results unless isA<T>() returns true */
    template<class T>
    const T& asA() const
    {
        return d->asA<T>();
    }

    /** Return this record cast as type 'T' - will have undefined
        results unless isA<T>() returns true */
    template<class T>
    T& asA()
    {
        return d->asA<T>();
    }

    void moveResidue(const Residue &res);
    void moveMolecule(const Molecule &mol);
    
    void changeMolecule(const ParameterTable &mol_and_params);
    
    void removeMolecule(const Molecule &mol);
    
    void addMolecule(const ParameterTable &mol_and_params);

private:
    void updatePointer(ChangeRecordBase *recordpointer);
    
    /** Implicitly shared pointer to the change record */
    SireBase::DynamicSharedPointer<ChangeRecordBase> d;
};

}

Q_DECLARE_METATYPE(SireFF::ChangeRecord)

SIRE_END_HEADER

#endif
