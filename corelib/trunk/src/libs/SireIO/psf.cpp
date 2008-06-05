// 
// #include "psf.h"
// 
// #include "SireStream/txtstream.h"
// #include "SireMol/atom.h"
// #include "SireFF/atomtype.h"
// #include "SireFF/cljparameter.h"
// #include "SireUnits/units.h"
// 
// #include <QFile>
// #include <QTextStream>
// #include <QString>
// 
// using namespace SireIO;
// using namespace SireStream;
// using namespace SireMol;
// using namespace SireFF;
// using namespace SireUnits;
// 
// PSF::PSF()
// {}
// 
// PSF::~PSF()
// {}
// 
// /** Read the parameters from the CHARMM PSF file 'filename' into the 
//     MM parameter database 'db' */
// void PSF::read(const QString &filename, SireFF::FFParentDB &db)
// {
//     //we need to be reading these parameters into a CLJ database
//     CLJDB &cljdb = dynamic_cast<CLJDB&>(db);
// 
//     //open the file
//     QFile f(filename);
//     
//     if (not f.open(QIODevice::ReadOnly))
//     {
//         printLine(WARNING,QObject::tr("Cannot open PSF file %1: %2")
//             .arg(filename).arg(f.error()) );
//         return;
//     }
//     
//     //create a textstream to read the file contents...
//     QTextStream ts(&f);
//     
//     //keep a record of the line number
//     int linenum = 0;
//     
//     //read the first three characters... these should be 'PSF'
//     QString line;
//     linenum++;
//     ts >> line;
//     if (line != "PSF")
//     {
//         printLine(WARNING, QObject::tr("This does not look like a PSF file? %1").arg(line));
//         return;
//     }
//     
//     printLine(DEBUG,QObject::tr("Reading PSF file '%1'").arg(filename));
//     
//     //keep reading the lines until we get a line reading !NATOM
//     int natoms = 0;
//     
//     while (not ts.atEnd())
//     {
//         linenum++;
//         line = ts.readLine();
//         if (line.indexOf("!NATOM") != -1)
//         {
//             //extract the number of atoms in the file...
//             QStringList words = line.split(" ", QString::SkipEmptyParts);
//             natoms = words[0].toInt();
//             break;
//         }
//     }
//     
//     if (natoms <= 0)
//     {
//         printLine(WARNING,QObject::tr("Problem reading PSF file - not enough atoms!"));
//         return;
//     }
//     
//     //read in all of the atoms
//     for (int i=0; i<natoms; i++)
//     {
//         if (ts.atEnd())
//         {
//             printLine(WARNING,QObject::tr("Premature end of atom list, %1 of %2")
//                         .arg(i).arg(natoms));
//             break;
//         }
//         
//         linenum++;
//         line = ts.readLine();
//         QStringList words = line.split(" ", QString::SkipEmptyParts);
//         
//         //the format is;
//         // idnum  typestring residue_number residue_name atom_name atom_type charge atom_mass  0
//         
//         // we are interested in the atom_type and atom_mass, which will give us an AtomType,
//         // and also we want the charge.
//         if (words.count() < 9)
//         {
//             printLine(WARNING,QObject::tr("Could not recognise PSF line '%1'").arg(line));
//             continue;
//         }
//         
//         //get the atom_type, and atom_mass (in g mol-1)
//         double atom_mass = words[7].toDouble() * g_per_mol;
//         
//         AtomType atomtype( words[5], Element::elementWithMass( atom_mass ) );
//         
//         //now read the charge (in unit electrons)
//         double chg = words[6].toDouble() * mod_electrons;
//         
//         //add this parameter to the database. The PSF file assigns a unique number 
//         //to every atom in the PSF file and corresponding PDB file, and it is this 
//         //unique number that is used to identify the parameter
//         cljdb.add( words[0], atomtype, chg );
//         
//         if (i % 1000 == 0)
//             printLine(DEBUG,QObject::tr("Read %1 of %2...").arg(i).arg(natoms));
//     }
//     
//     printLine(DEBUG,"Finished reading the PSF file!");
// }
// 
// /** Read the parameters from a CHARMM .param file into the MM parameter database 'db' */
// void PSF::readParam(const QString&, SireFF::FFParentDB&)
// {}
// 
// /** Read the force field parameters from a CHARMM .prm file */
// void PSF::readPrm(const QString &filename, SireFF::FFParentDB &db)
// {
//     //we need to be working with at least an LJDB
//     LJDB &ljdb = dynamic_cast<LJDB&>(db);
// 
//     //open the file
//     QFile f(filename);
//     
//     if (not f.open(QIODevice::ReadOnly))
//     {
//         printLine(WARNING,QObject::tr("Cannot open PSF file %1: %2")
//             .arg(filename).arg(f.error()) );
//         return;
//     }
//     
//     //create a textstream to read the file contents...
//     QTextStream ts(&f);
//     
//     //keep a record of the line number
//     int linenum = 0;
//     
//     //this is a keyword based file, e.g. there are keywords which switch switch
//     //between modes. At the moment I am only interested in the NONBONDED mode
//     //though I will need to watch out for other modes. These are;
//     //"BONDS", "ANGLES", "DIHEDRALS", "IMPROPER", "NONBONDED", "HBOND"
//     enum PRM_MODES{ INVALID, BONDS, ANGLES, DIHEDRALS, IMPROPER, NONBONDED, HBOND };
//     
//     PRM_MODES mode = INVALID;
//     
//     while (not ts.atEnd())
//     {
//         linenum++;
//         //read a line from the file
//         QString line = ts.readLine();
//         
//         //remove spaces from the start and end
//         line = line.simplified();
//         
//         //if the line starts with a '*' or '!' then skip it
//         if (line.startsWith("!") or line.startsWith("*") or line.isEmpty())
//             continue;
//             
//         //look at the first word in the line, and see if it will cause a mode change
//         QStringList words = line.split(" ", QString::SkipEmptyParts);
//         
//         QString word0 = words[0];
//         
//         if (word0 == "BONDS")
//             mode = BONDS;
//         else if (word0 == "ANGLES")
//             mode = ANGLES;
//         else if (word0 == "DIHEDRALS")
//             mode = DIHEDRALS;
//         else if (word0 == "IMPROPER")
//             mode = IMPROPER;
//         else if (word0 == "NONBONDED")
//             mode = NONBONDED;
//         else if (word0 == "HBOND")
//             mode = HBOND;
//             
//         //now what we do depends on the mode
//         switch(mode)
//         {
//             case NONBONDED:
//                 //very simple nonbonded line - "atom-type  ignore  epsilon  rmin/2"
//                 if (words.count() >= 4)
//                 {
//                     //to complicate things slightly, the file only contains the 
//                     //name of the atom type, and not the element - we will have to 
//                     //ask the database if it has come across this type before...
//                     QString typnam = words[0];
//                     AtomType atomtype = db.getAtomType( typnam );
//                     
//                     if (atomtype.isNull())
//                         //the database couldn't find it - we will have to guess...
//                         atomtype = AtomType( typnam, Element(typnam) );
//                     
//                     //now get the LJ parameter from the line (epsilon in kcal mol-1 and
//                     //rmin in A)
//                     double epsilon = words[2].toDouble() * kcal_per_mol;
//                     double rmin = 2.0 * words[3].toDouble() * angstrom;
//                     
//                     LJParameter ljparam = LJParameter::fromRMinAndEpsilon(rmin, epsilon);
//                     
//                     //now add the parameter to the database
//                     ljdb.add(atomtype, ljparam);
//                 }
//                 break;
//            default:
//                 break;
//         };
//     }
// }
