
#include "ffworker.h"

#include "SireMol/molecule.h"
#include "SireMol/residue.h"

#include "SireStream/datastream.h"

using namespace SireFF;
using namespace SireCAS;
using namespace SireMol;
using namespace SireDB;
using namespace SireStream;

FFWorker::FFWorker()
{}


FFWorker::~FFWorker()
{}



    /** Register the addition of the molecule 'molecule' */
    void addToRegister(const Molecule &molecule);
    
    /** Register the addition of the residue 'residue' */
    void addToRegister(const Residue &residue);

