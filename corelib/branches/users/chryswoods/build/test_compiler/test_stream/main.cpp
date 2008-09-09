
#include "SireStream/streamdata.hpp"

#include "SireMol/molecule.h"
#include "SireMol/mover.hpp"

#include "SireError/errors.h"

#include <QByteArray>
#include <QDataStream>

#include <QDebug>

using boost::tuple;
using boost::shared_ptr;

using namespace SireMol;
using namespace SireStream;

int main(int argc, const char **argv)
{
    Molecule mol;

    qDebug() << "Saving a molecule...";

    QByteArray data = save(mol);

    qDebug() << "Loading a molecule...";

    Molecule new_mol = loadType<Molecule>(data);      

    qDebug() << "Done!";

    return 0;
}

