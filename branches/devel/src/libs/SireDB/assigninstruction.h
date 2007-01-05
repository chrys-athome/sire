#ifndef SIREDB_ASSIGNINSTRUCTION_H
#define SIREDB_ASSIGNINSTRUCTION_H

#include "sireglobal.h"

#include <QSharedData>

SIRE_BEGIN_HEADER

namespace SireDB
{
class AssignInstruction;
}

class QDataStream;
QDataStream& operator<<(QDataStream&, const SireDB::AssignInstruction&);
QDataStream& operator>>(QDataStream&, SireDB::AssignInstruction&);

namespace SireDB
{

/** This is the virtual base class of all assignment instructions.
    There are currently three types of instruction;

    (1) assignment instructions - these say what should be assigned and how
                                  (see AssignBase and assign_atoms, assign_bonds etc.)

    (2) relationship instructions - these instruct which relationships to use
                                    during the assignment (see using_relationships)

    (3) parameter instructions - these instruct which parameter databases to use
                                 during the assignment (see using_parameters)

    This class is empty, acting only to provide a common virtual base
    class so that dynamic casting may be performed.

    @author Christopher Woods
*/
class SIREDB_EXPORT AssignInstruction : public QSharedData
{
public:
    AssignInstruction()
    {}

    AssignInstruction(const AssignInstruction&)
    {}

    virtual ~AssignInstruction()
    {}

    virtual AssignInstruction* clone() const=0;
    virtual const char* what() const=0;

};

}

SIRE_END_HEADER

#endif
