/** 
  * @file
  *
  * C++ Implementation: MovedParts
  *
  * Description: 
  * Implementation of the MovedParts class
  *
  * @author Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */

#include "movedparts.h"

#include "SireMol/residue.h"
#include "SireStream/datastream.h"

using namespace SireFF;
using namespace SireMol;
using namespace SireStream;

static const RegisterMetaType<MovedParts> r_movedparts("SireFF::MovedParts");

/** Serialise to a binary data stream */
QDataStream SIREFF_EXPORT &operator<<(QDataStream &ds, const MovedParts &movedparts)
{
    writeHeader(ds, r_movedparts, 1) 
                  << movedparts.molid
                  << movedparts.molversion
                  << movedparts.movedgroups;
                  << static_cast<const ChangeRecordBase&>(movedparts);
    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREFF_EXPORT &operator>>(QDataStream &ds, MovedParts &movedparts)
{
    VersionID v = readHeader(ds, r_movedparts);
    
    if (v == 1)
    {
        ds >> movedparts.molid
           >> movedparts.molversion
           >> movedparts.movedgroups
           >> static_cast<ChangeRecordBase&>(movedparts);
    }
    else
        throw version_error(v, "1", r_movedparts, CODELOC);
    
    return ds;
}

/** Empty constructor */
MovedParts::MovedParts() : molid(0), molversion(0,0)
{}

/** Construct to hold the move information for the moved residue 'res' */
MovedParts::MovedParts(const Residue &res)
           : molid(res.

/** Destructor */
MovedParts::~MovedParts()
{}
