
#include "SireIO/pdb.h"
#include "SireError/errors.h"

#include "SireMol/mover.hpp"

#include <QDebug>

using namespace SireIO;

int main(int argc, char **argv)
{
    PDB pdb;

    for (int i=1; i<argc; ++i)
    {
        try
        {
            qDebug() << "Reading " << argv[i] << "...";
            pdb.read(QLatin1String(argv[i]));
        }
        catch(const SireError::exception &e)
        {
            qDebug() << "Error reading " << argv[i];
            qDebug() << e.toString();
        }
        catch(...)
        {
            qDebug() << "CAUGHT UNKNOWN EXCEPTION!!!";
        }
    }
    
    return 0;
}
