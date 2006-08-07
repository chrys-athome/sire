
#include "mol2.h"

#include "SireMol/editmol.h"

using namespace SireMol;
using namespace SireIO;

Mol2::Mol2()
{}

Mol2::~Mol2()
{}

EditMolList Mol2::readGroups(const QByteArray &data) const
{
    return EditMolList();
}

QByteArray Mol2::writeGroups(const EditMolList &groups) const
{}
