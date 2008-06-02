
#include "SireIO/pdb.h"
#include "SireError/errors.h"

#include "SireMol/moleculegroup.h"
#include "SireMol/molecule.h"
#include "SireMol/atom.h"

#include "SireMol/moleditor.h"
#include "SireMol/atomeditor.h"

#include "SireMol/mover.hpp"
#include "SireMol/selector.hpp"

#include "SireMol/atomcharges.h"

#include "SireMM/atomljs.h"
#include "SireMM/intercljff.h"
#include "SireMM/switchingfunction.h"

#include "SireVol/periodicbox.h"
#include "SireVol/cartesian.h"

#include "SireUnits/dimensions.h"
#include "SireUnits/units.h"

#include <QDebug>

using namespace SireIO;
using namespace SireMol;
using namespace SireMM;
using namespace SireVol;

using SireUnits::Dimension::Charge;
using SireUnits::mod_electron;
using SireUnits::angstrom;
using SireUnits::kcal_per_mol;

int main(int argc, char **argv)
{
    PDB pdb;

    try
    {
        MoleculeGroup mols = pdb.read("test/io/water.pdb");
        
        qDebug() << "nmols =" << mols->nMolecules();
        
        //BOOST_ASSERT(mols->nMolecules() == 1679);
        
        //parameterise each molecule...
        qDebug() << "Parameterising the molecules...";
        MolGroup::const_iterator it = mols->constBegin();
        
        //parameterise the first water molecule
        MolEditor moleditor = it->molecule().edit();
        
        moleditor = moleditor.select( AtomName("O00") )
                             .setProperty("charge", 0 * mod_electron)
                             .setProperty("LJ", LJParameter(3.15363 * angstrom,
                                                            0.155 * kcal_per_mol) )
                             //.setProperty("LJ", LJParameter::dummy())
                             .molecule();
                 
        moleditor = moleditor.select( AtomName("H01") )
                             .setProperty("charge", 0.52 * mod_electron)
                             .setProperty("LJ", LJParameter::dummy())
                             .molecule();
                             
        moleditor = moleditor.select( AtomName("H02") )
                             .setProperty("charge", 0.52 * mod_electron)
                             .setProperty("LJ", LJParameter::dummy())
                             .molecule();
                             
        moleditor = moleditor.select( AtomName("M03") )
                             .setProperty("charge", -1.04 * mod_electron )
                             .setProperty("LJ", LJParameter::dummy())
                             .molecule();

        Molecule tip4p = moleditor.commit();

        AtomCharges charges = tip4p.property("charge")->asA<AtomCharges>();
        AtomLJs ljs = tip4p.property("LJ")->asA<AtomLJs>();
        
        for (it = mols->constBegin(); it != mols->constEnd(); ++it)
        {
            tip4p = it->molecule().edit()
                                  .setProperty("charge", charges)
                                  .setProperty("LJ", ljs)
                                  .commit();
                                  
            mols.edit().update(tip4p);
        }
        
        qDebug() << "Adding to the forcefield...";
        InterCLJFF cljff;
        
        //cljff.setSwitchingFunction( HarmonicSwitchingFunction(150.0, 0.0) );
        
        //cljff.setSpace( PeriodicBox(Vector(-18.3854,-18.66855,-18.4445),
        //                            Vector( 18.3854, 18.66855, 18.4445)) );

        //cljff.setSpace( Cartesian() );

        
        cljff.add( *mols );
        
        qDebug() << "Calculating the the energy...";
        qDebug() << cljff.energy() << "kcal mol-1";
    }
    catch(const SireError::exception &e)
    {
        qDebug() << "**EXCEPTION**";
        qDebug() << e.toString();
    }
    catch(...)
    {
        qDebug() << "CAUGHT UNKNOWN EXCEPTION!!!";
    }
    
    return 0;
}
