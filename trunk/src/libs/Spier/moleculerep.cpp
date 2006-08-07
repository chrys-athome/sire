
#include "moleculerep.h"
#include "moldisplaydata.h"

#include "SireMol/editmol.h"

using namespace Spier;
using namespace SireMol;
using namespace SireMaths;
using namespace SireStream;

/** Construct the molecule representation from the data held in moldata */
MoleculeRep::MoleculeRep(const MolDisplayDataPtr &moldata) 
            : ViewObj(), prnt(moldata)
{
    if (moldata.get() != 0 and not moldata->isNull())
        renderatms = moldata->molecule().atoms();
}

/** Destructor */
MoleculeRep::~MoleculeRep()
{}

/** Return the molecule being represented */
EditMol MoleculeRep::molecule() const
{
    if (prnt.expired())
        return EditMol();
    else
        return prnt.lock()->molecule();
}

/** Return whether this is a null molecule rep - a null rep is one that 
    does not have a valid parent */
bool MoleculeRep::isNull() const
{
    return (prnt.expired() or prnt.lock()->isNull());
}
