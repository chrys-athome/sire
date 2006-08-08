
#include "SireCAS/qhash_sirecas.h"
#include "SireMol/qhash_siremol.h"

#include "protoms.h"
#include "protomsws.h"

#include "SireCAS/expression.h"
#include "SireCAS/function.h"
#include "SireCAS/trigfuncs.h"

#include "SireDB/atomtypedb.h"
#include "SireDB/matchhelpers.h"
#include "SireDB/matchres.h"

#include "SireMM/chargedb.h"
#include "SireMM/ljdb.h"
#include "SireMM/bonddb.h"
#include "SireMM/angledb.h"
#include "SireMM/dihedraldb.h"

#include "SireUnits/units.h"

#include <QFile>
#include <QTextStream>
#include <QHash>

#include <boost/tuple/tuple.hpp>

using boost::tuple;

using namespace SireIO;
using namespace SireCAS;
using namespace SireDB;
using namespace SireMM;
using namespace SireUnits;

/** Constructor */
ProtoMS::ProtoMS()
{}

/** Destructor */
ProtoMS::~ProtoMS()
{}
    
/** Read the ProtoMS parameter file 'filename' into the forcefield parameter database 'db' */
void ProtoMS::read(const QString &filename, ParameterDB &db)
{
    ProtoMSWS workspace( filename, db );
    
    //read each line of the file
    while ( workspace.readNextLine() )
    {
        //what we do depends on the mode we are in...
        switch( workspace.mode() )
        {
            case ProtoMSWS::INVALID_MODE:
                //do nothing for invalid modes
                break;
            case ProtoMSWS::CLJ_MODE:
                //read the CLJ line into the database
                readCLJLine(workspace);
                break;
            case ProtoMSWS::BOND_MODE:
                //read the BOND line into the database
                readBondLine(workspace);
                break;
            case ProtoMSWS::ANGLE_MODE:
                //read the ANGLE line into the database 
                readAngleLine(workspace);
                break;
            case ProtoMSWS::UREYBRADLEY_MODE:
                //read the UREYBRADLEY line into the database
                readUreyBradleyLine(workspace);
                break;
            case ProtoMSWS::DIHEDRAL_MODE:
                //read the DIHEDRAL line into the database
                readDihedralLine(workspace);
                break;
            case ProtoMSWS::TEMPLATE_MODE:
                //read the template line - this reads in which parameters
                //are associated with which atoms.
                readTemplateLine(workspace);
                break;
            default:
                //we should not get here!
                workspace.addLog( QObject::tr("Invalid MODE (%1)").arg(workspace.mode()) );
                break;
        }
    }
}

/** Read the parameters from a ProtoMS2 parameter file "CLJ" line into the database */
void ProtoMS::readCLJLine(ProtoMSWS &workspace)
{
    //the format is 
    //par  id   atom_type   element   charge   sigma  epsilon
    if (workspace.firstWord() != "par")
        return;
        
    //get the words on the line
    const QStringList &words = workspace.words();
        
    if (words.count() < 7)
    {
        //error in the line - add this to the log...
        workspace.addLog( QObject::tr("WARNING: Error parsing CLJ line %1")
                                            .arg(workspace.lineInfo()) );
        return;
    }
    
    //extract the user supplied ID for the parameter
    QString id = words[1];
    
    //extract the atom type from the line
    bool ok;
    int element = words[3].toInt(&ok);
        
    if (ok)
        workspace.atomTypeDB().addAtomType( id, AtomType(words[2],element) );
    else
        workspace.addLog( QObject::tr("Could not get the proton number from "
                                      "\"%1\" on CLJ line %2")
                                .arg(words[3],workspace.lineInfo()) );
    
    //now the charge (in unit electrons)
    double charge = words[4].toDouble(&ok) * mod_electrons;
        
    if (ok)
        workspace.chargeDB().addCharge( id, charge );
    else
        workspace.addLog( QObject::tr("Could not get the charge from \"%1\" on "
                                      "CLJ line %2")
                                .arg(words[4],workspace.lineInfo()) );
        
    //now the LJ parameter (sigma in angstrom and epsilon in kcal mol-1)
    bool ok1,ok2;
    double sigma = words[5].toDouble(&ok1) * angstrom;
    double epsilon = words[6].toDouble(&ok2) * kcal_per_mol;
        
    if (ok1 and ok2)
        workspace.ljDB().addLJ( id, LJParameter(sigma,epsilon) );
    else
    {
        if (not ok1)
            workspace.addLog( QObject::tr("Could not get sigma from \"%1\" on "
                                          "CLJ line %2")
                                .arg(words[5],workspace.lineInfo()) );
            
        if (not ok2)
            workspace.addLog( QObject::tr("Could not get epsilon from \"%1\" on "
                                          "CLJ line %2")
                                .arg(words[6],workspace.lineInfo()) );
    }
}

/** Read the parameters from a ProtoMS2 parameter file "BOND" line into the database */
void ProtoMS::readBondLine(ProtoMSWS &workspace)
{
    const QStringList &words = workspace.words();

    if (workspace.firstWord() == "atm")
    {
        //this is an 'atm' line. The format is;
        // atm  atom_type0  atom_type1   bond_par_id
        if (words.count() < 4)
        {
            workspace.addLog( QObject::tr("WARNING: Error parsing Bond 'atm' line %1")
                                      .arg(workspace.lineInfo()) );
            return;
        }
            
        //extract the information
        QString atm0 = words[1];
        QString atm1 = words[2];
        QString paramid = words[3];
            
        //set the bond parameter between atomtypes atm0 and atm1 to use the bond
        //function identified by 'paramid'.
        workspace.bondDB().relateBond( atom0_type == atm0 and atom1_type == atm1, paramid );
    }
    else if (workspace.firstWord() == "par")
    {
        //this is a 'par' line. The format is;
        // par  id   k   r0
        
        // ProtoMS uses bond function E(r) = k (r-r0)^2, so this line identifies
        // a single harmonic function, with these parameters.
    
        if (words.count() < 4)
        {
            workspace.addLog( QObject::tr("WARNING: Error parsing Bond 'par' line %1")
                                    .arg(workspace.lineInfo()) );
            return;
        }
        
        //extract the information
        bool ok1,ok2;
        
        QString paramid = words[1];
        double k = words[2].toDouble(&ok1) * kcal_per_mol / (angstrom*angstrom); // kcal mol-1 A-2
        double r0 = words[3].toDouble(&ok2) * angstrom; // units of angstroms
        
        if (ok1 and ok2)
        {
            //get the symbol used for the bond length
            const Symbol &r = workspace.bondDB().r();
        
            //add this function to the database, identified by 'paramid'
            workspace.bondDB().addBond( paramid, k * pow( r-r0, 2 ) );
        }
        else
        {
            if (not ok1)
                workspace.addLog( QObject::tr("Could not get the 'k' parameter from "
                                              "\"%1\" on Bond 'par' line %2")
                                    .arg(words[2],workspace.lineInfo()) );
                                
            if (not ok2)
                workspace.addLog( QObject::tr("Could not get the 'r0' parameter from "
                                              "\"%1\" on Bond 'par' line %2")
                                    .arg(words[3],workspace.lineInfo()) );
        }
    }
}

/** Read the parameters from a ProtoMS2 parameter file "ANGLE" line into the database */
void ProtoMS::readAngleLine(ProtoMSWS &workspace)
{
    const QStringList &words = workspace.words();

    if (workspace.firstWord() == "atm")
    {
        //this is an 'atm' line. The format is;
        // atm  atom_type0  atom_type1  atom_type2  angle_par_id
        if (words.count() < 5)
        {
            workspace.addLog( QObject::tr("WARNING: Error parsing Angle 'atm' line %1")
                                        .arg(workspace.lineInfo()) );
            return;
        }
        
        //extract the information
        QString atm0 = words[1];
        QString atm1 = words[2];
        QString atm2 = words[3];
        QString paramid = words[4];
        
        //set the angle parameter between atomtypes atm0-atm1-atm2 to use the angle
        //function identified by 'paramid'.
        workspace.angleDB().relateAngle( atom0_type == atm0 and atom1_type == atm1 
                                               and atom2_type == atm2, paramid );
    }
    else if (workspace.firstWord() == "par")
    {
        //this is a 'par' line. The format is;
        // par  id   k   t0
        
        // ProtoMS uses angle function E(t) = k (t-t0)^2, so this line identifies
        // a single harmonic function, with these parameters. It is possible that
        // Urey-Bradley terms may be later added to the angle function when these
        // parameters are assigned (e.g. E(t,r) = k_t(t-t0)^2 + k_r(r-r0)^2, where
        // t is the angle, and r is the distance between the 1-3 atoms.
        
        if (words.count() < 4)
        {
            workspace.addLog( QObject::tr("WARNING: Error parsing Angle 'par' line %1")
                                          .arg(workspace.lineInfo()) );
            return;
        }
        
        //extract the information
        bool ok1,ok2;
        
        QString paramid = words[1];
                                                //units kcal mol-1 rad-2
        double k = words[2].toDouble(&ok1) * kcal_per_mol / (radians*radians); 
        double theta0 = words[3].toDouble(&ok2) * degrees;  // units of degrees
        
        if (ok1 and ok2)
        {
            //get the symbol used to represent the size of the angle
            const Symbol &theta = workspace.angleDB().theta();
        
            //add this function to the database, identified by 'paramid'
            workspace.angleDB().addAngle( paramid, k * pow( theta-theta0, 2 ) );
        }
        else
        {
            if (not ok1)
                workspace.addLog( QObject::tr("Could not get the 'k' parameter from \"%1\" "
                                              "on Angle 'par' line %2")
                                      .arg(words[2],workspace.lineInfo()) );
        
            if (not ok2)
                workspace.addLog( QObject::tr("Could not get the 't0' parameter from \"%1\" "
                                              "on Angle 'par' line %2")
                                  .arg(words[3],workspace.lineInfo()) );
        }
    }
}

/** Read the parameters from a ProtoMS2 parameter file "DIHEDRAL" line into the database */
void ProtoMS::readDihedralLine(ProtoMSWS &workspace)
{
    const QStringList &words = workspace.words();

    if (workspace.firstWord() == "atm")
    {
        //this is an 'atm' line. The format is;
        // atm  atom_type0  atom_type1  atom_type2  atom_type3  dihedral_par_id
        if (words.count() < 6)
        {
            workspace.addLog( QObject::tr("WARNING: Error parsing Dihedral 'atm' line %1")
                                      .arg(workspace.lineInfo()) );
            return;
        }
        
        //extract the information
        QString atm0 = words[1];
        QString atm1 = words[2];
        QString atm2 = words[3];
        QString atm3 = words[4];
        QString paramid = words[5];
        
        //set the dihedral parameter between atomtypes atm0-atm1-atm2-atm3 to use the dihedral
        //function identified by 'paramid'.
        workspace.dihedralDB().relateDihedral( atom0_type == atm0 and atom1_type == atm1 and
                                               atom2_type == atm2 and atom3_type == atm3, 
                                               paramid );
    }
    else if (workspace.firstWord() == "term")
    {
        //this is a 'term' line. This gives a single term within a dihedral function.
        //The format is;
        // term  id  t0  t1  t2  t3
        
        // where t0 to t3 are parameters in the function  
        // f(phi) = t0 * { 1 + t1*[cos( t2*phi + t3 )] }
        if (words.count() < 6)
        {
            workspace.addLog( QObject::tr("WARNING: Error parsing Dihedral 'term' line %1")
                                            .arg(workspace.lineInfo()) );
            return;
        }
        
        //first, create a function for this term
        Function term( QString("Term_%1").arg(words[1].toInt()) );
        
        //now read in t0 to t3
        bool ok0,ok1,ok2,ok3;
        
        double t0 = words[2].toDouble(&ok0) * kcal_per_mol;  //units of kcal mol-1
        double t1 = words[3].toDouble(&ok1);  //dimensionless
        double t2 = words[4].toDouble(&ok2);  //dimensionless
        double t3 = words[5].toDouble(&ok3) * degrees;  //units of degrees
        
        if (ok0 and ok1 and ok2 and ok3)
        {
            //get the symbol used to represent the dihedral angle
            const Symbol &phi = workspace.dihedralDB().phi();
        
            //now convert these parameters into a function. 
            Expression cosfunc = t0 * ( 1 + t1*Cos(t2*phi + t3) );
        
            //add this term to the database
            workspace.dihedralDB().addDihedral( term(phi), cosfunc );
        }
        else
        {
            if (not &ok0)
                workspace.addLog( QObject::tr("Could not get the 't0' parameter from \"%1\" "
                                              "on Dihedral 'term' line %2")
                                      .arg(words[2],workspace.lineInfo()) );
            if (not &ok1)
                workspace.addLog( QObject::tr("Could not get the 't1' parameter from \"%1\" "
                                              "on Dihedral 'term' line %2")
                                      .arg(words[3],workspace.lineInfo()) );
            if (not &ok2)
                workspace.addLog( QObject::tr("Could not get the 't2' parameter from \"%1\" "
                                              "on Dihedral 'term' line %2")
                                      .arg(words[4],workspace.lineInfo()) );
            if (not &ok3)
                workspace.addLog( QObject::tr("Could not get the 't3' parameter from \"%1\" "
                                              "on Dihedral 'term' line %2")
                                      .arg(words[5],workspace.lineInfo()) );
        }
    }
    else if (workspace.firstWord() == "par")
    {
        //this is a 'par' line. This lists the cos terms that make up the complete dihedral parameter
        // par  id   termid0  termid1 ....  termidn
        if (words.count() < 3)
        {
            workspace.addLog( QObject::tr("WARNING: Error parsing Dihedral 'par' line %1")
                                            .arg(workspace.lineInfo()) );
            return;
        }
        
        //extract the information
        QString paramid = words[1];
        
        Expression dihsum;
        
        const Symbol &phi = workspace.dihedralDB().phi();

        //now read in the term IDs
        bool still_ok = true;
        
        for (int i=2; i<words.count(); i++)
        {
            //the dihedral is a sum of terms, e.g. Term_i(phi)
            bool ok;
            Function term( QString("Term_%1").arg( words[i].toInt(&ok) ) );
            dihsum += term(phi);
            
            still_ok = still_ok and ok;
        }
        
        if (still_ok)
            //add this function to the database, identified by 'paramid'
            workspace.dihedralDB().addDihedral(paramid, dihsum);
        else
            workspace.addLog( QObject::tr("Could not read some of the term ID numbers from "
                                          "Dihedral 'par' line %1")
                                    .arg(workspace.lineInfo()) );
    }
}

/** Read the parameters from a ProtoMS2 parameter file "UREYBRADLEY" line into the database */
void ProtoMS::readUreyBradleyLine(ProtoMSWS &workspace)
{
    const QStringList &words = workspace.words();

    if (workspace.firstWord() == "atm")
    {
        //this is an 'atm' line. The format is;
        // atm  atom_type0  atom_type1  atom_type2  ureybradley_par_id
        if (words.count() < 5)
        {
            workspace.addLog( QObject::tr("WARNING: Error parsing Urey-Bradley 'atm' line %1")
                                        .arg(workspace.lineInfo()) );
            return;
        }
        
        //extract the information
        QString atm0 = words[1];
        QString atm1 = words[2];
        QString atm2 = words[3];
        QString paramid = words[4];
        
        //set the angle parameter between atomtypes atm0-atm1-atm2 to use the angle
        //function identified by 'paramid'.
        workspace.ureyBradleyDB().relateUreyBradley( atom0_type == atm0 and 
                                                     atom1_type == atm1 and
                                                     atom2_type == atm2, paramid );
    }
    else if (workspace.firstWord() == "par")
    {
        //this is a 'par' line. The format is;
        // par  id   k   t0
        
        // ProtoMS uses Urey-Bradley function E(b) = k (r-r0)^2, so this line identifies
        // a single harmonic function, with these parameters.
        
        if (words.count() < 4)
        {
            workspace.addLog( QObject::tr("WARNING: Error parsing Urey-Bradley 'par' line %1")
                                          .arg(workspace.lineInfo()) );
            return;
        }
        
        //extract the information
        bool ok1,ok2;
        
        QString paramid = words[1];
                                                //units kcal mol-1 angstrom-2
        double k = words[2].toDouble(&ok1) * kcal_per_mol / (angstrom*angstrom); 
        double r0 = words[3].toDouble(&ok2) * angstrom;  // units of angstroms
        
        if (ok1 and ok2)
        {
            //get the symbol used to represent the size of the angle
            const Symbol &r = workspace.ureyBradleyDB().r();
        
            //add this function to the database, identified by 'paramid'
            workspace.ureyBradleyDB().addUreyBradley( paramid, k * pow( r-r0, 2 ) );
        }
        else
        {
            if (not ok1)
                workspace.addLog( QObject::tr("Could not get the 'k' parameter from \"%1\" "
                                              "on Urey-Bradley 'par' line %2")
                                      .arg(words[2],workspace.lineInfo()) );
        
            if (not ok2)
                workspace.addLog( QObject::tr("Could not get the 'r0' parameter from \"%1\" "
                                              "on Urey-Bradley 'par' line %2")
                                  .arg(words[3],workspace.lineInfo()) );
        }
    }
}

/** Master function for reading in ProtoMS2 templates. This delegates power to 
    child functions depending on what type of template is currently being read */
void ProtoMS::readTemplateLine(ProtoMSWS &workspace)
{
    //what we do now depends on the type of template
    switch( workspace.templateType() )
    {
        case ProtoMSWS::CHAIN_TMPL:
            this->readChainTemplate(workspace);
            break;
        case ProtoMSWS::RESIDUE_TMPL:
            this->readResidueTemplate(workspace);
            break;
        case ProtoMSWS::SOLUTE_TMPL:
            this->readSoluteTemplate(workspace);
            break;
        case ProtoMSWS::SOLVENT_TMPL:
            this->readSolventTemplate(workspace);
            break;
        case ProtoMSWS::INVALID_TMPL:
            return;
        default:
            //we should not get here!
            workspace.addLog( QObject::tr("Invalid TEMPLATE (%1)")
                                    .arg(workspace.templateType()) );
            break;
    }
}

/** Small, private function that is used to get the parameters from
    the ProtoMS2 bond, angle, dihedral or ureybradley lines. It does the
    same job as the getDOFInfo function in ProtoMS2 */
tuple<QString,QString> getParameters(const QStringList &words, int strt, 
                                     bool *foundparams)
{
    int i = words.count() - 1;
    
    bool found = false;
    int param0(0),param1(0);
    
    while (i >= strt and not found)
    {
        QString lower = words[i].toLower();
        if (lower == "param" or lower == "dummy")
        {
            found = true;
            param0 = 0;
            param1 = 0;
            break;
        }
        else if (i-1 >= strt and words[i-1].toLower() == "param")
        {
            bool ok;
            param0 = words[i].toInt(&ok);
            
            if (ok)
            {
                found = true;
                if (param0 < 0)
                    param0 = 0;
                
                param1 = param0;
                break;
            }
        }
        else if (i-2 >= strt and words[i-2].toLower() == "param")
        {
            bool ok0,ok1;
            param0 = words[i-1].toInt(&ok0);
            param1 = words[i].toInt(&ok1);
            
            if (ok0 and ok1)
            {
                found = true;
                if (param0 < 0)
                    param0 = 0;
                if (param1 < 0)
                    param1 = 0;
                break;
            }
        }
        
        --i;
    }
    
    if (foundparams != 0)
        *foundparams = found;
        
    if (found)
        return tuple<QString,QString>( QString::number(param0), QString::number(param1) );
    else
        return tuple<QString,QString>();
}

/** Process the ProtoMS2 residue and chain template 'bond' line, 
    including additional residue matching criteria specified in 'matchres' */
void ProtoMS::processBondLine( ProtoMSWS &workspace, const MatchRes &matchres )
{
    //format of the line is;
    // bond atm0 atm1 ....   where "..." can contain "param 3" or "param 3 4"
    
    const QStringList &words = workspace.words();
    if (words.count() <= 3)
        return;

    //scan for the parameter numbers...
    bool foundparams;
    tuple<QString,QString> params = getParameters(words, 3, &foundparams);
    
    if (foundparams)
    {
        //if there are specified parameters then extract the 
        //atom names of the bonded atoms...
        QString atm0 = words[1];
        QString atm1 = words[2];
        
        const QString &param0 = params.get<0>();
        const QString &param1 = params.get<1>();
        
        //add the relationship
        MatchMRAData matchdata = matchres and 
                                 atom0_name == atm0 and atom1_name == atm1;
                                 
        workspace.bondDB().relateBond( matchdata, param0 );
        
        if (param0 != param1)
        {
            //this bond is being perturbed
            workspace.bondDB().relateBond( mol_state == "reference" and matchdata, param0 );
            workspace.bondDB().relateBond( mol_state == "perturbed" and matchdata, param1 );
        }
    }
}

/** Process the ProtoMS2 residue and chain template 'angle' line, 
    including additional residue matching criteria specified in 'matchres' */
void ProtoMS::processAngleLine( ProtoMSWS &workspace, const MatchRes &matchres )
{
    //format of the line is;
    // angle atm0 atm1 atm2 ....   where "..." can contain "param 3" or "param 3 4"
    
    const QStringList &words = workspace.words();
    if (words.count() <= 4)
        return;

    //scan for the parameter numbers...
    bool foundparams;
    tuple<QString,QString> params = getParameters(words, 4, &foundparams);
    
    if (foundparams)
    {
        //if there are specified parameters then extract the 
        //atom names of the bonded atoms...
        QString atm0 = words[1];
        QString atm1 = words[2];
        QString atm2 = words[3];
        
        const QString &param0 = params.get<0>();
        const QString &param1 = params.get<1>();
        
        //add the relationship
        MatchMRAData matchdata = matchres and 
                                 atom0_name == atm0 and atom1_name == atm1 and
                                 atom2_name == atm2;
                                 
        workspace.angleDB().relateAngle( matchdata, param0 );
        
        if (param0 != param1)
        {
            //this bond is being perturbed
            workspace.angleDB().relateAngle( mol_state == "reference" and matchdata, param0 );
            workspace.angleDB().relateAngle( mol_state == "perturbed" and matchdata, param1 );
        }
    }
}

/** Process the ProtoMS2 residue and chain template 'dihedral' line, 
    including additional residue matching criteria specified in 'matchres' */
void ProtoMS::processDihedralLine( ProtoMSWS &workspace, const MatchRes &matchres )
{
    //format of the line is;
    // dihedral atm0 atm1 atm2 atm3 ....   where "..." can contain "param 3" or "param 3 4"
    
    const QStringList &words = workspace.words();
    if (words.count() <= 5)
        return;

    //scan for the parameter numbers...
    bool foundparams;
    tuple<QString,QString> params = getParameters(words, 5, &foundparams);
    
    if (foundparams)
    {
        //if there are specified parameters then extract the 
        //atom names of the bonded atoms...
        QString atm0 = words[1];
        QString atm1 = words[2];
        QString atm2 = words[3];
        QString atm3 = words[4];
        
        const QString &param0 = params.get<0>();
        const QString &param1 = params.get<1>();
        
        //add the relationship
        MatchMRAData matchdata = matchres and 
                                 atom0_name == atm0 and atom1_name == atm1 and
                                 atom2_name == atm2 and atom3_name == atm3;
                                 
        workspace.dihedralDB().relateDihedral( matchdata, param0 );
        
        if (param0 != param1)
        {
            //this bond is being perturbed
            workspace.dihedralDB().relateDihedral( mol_state == "reference" and matchdata, 
                                                   param0 );
            workspace.dihedralDB().relateDihedral( mol_state == "perturbed" and matchdata, 
                                                   param1 );
        }
    }
}

/** Process the ProtoMS2 residue and chain template 'ureybradley' line, 
    including additional residue matching criteria specified in 'matchres' */
void ProtoMS::processUreyBradleyLine( ProtoMSWS &workspace, const MatchRes &matchres )
{
    //format of the line is;
    // ureybradley atm0 atm1 atm2 ....   where "..." can contain "param 3" or "param 3 4"
    
    const QStringList &words = workspace.words();
    if (words.count() <= 4)
        return;

    //scan for the parameter numbers...
    bool foundparams;
    tuple<QString,QString> params = getParameters(words, 4, &foundparams);
    
    if (foundparams)
    {
        //if there are specified parameters then extract the 
        //atom names of the bonded atoms...
        QString atm0 = words[1];
        QString atm1 = words[2];
        QString atm2 = words[3];
        
        const QString &param0 = params.get<0>();
        const QString &param1 = params.get<1>();
        
        //add the relationship
        MatchMRAData matchdata = matchres and 
                                 atom0_name == atm0 and atom1_name == atm1 and
                                 atom2_name == atm2;
                                 
        workspace.ureyBradleyDB().relateUreyBradley( matchdata, param0 );
        
        if (param0 != param1)
        {
            //this bond is being perturbed
            workspace.ureyBradleyDB().relateUreyBradley( mol_state == "reference" and 
                                                         matchdata, param0 );
            workspace.ureyBradleyDB().relateUreyBradley( mol_state == "perturbed" and 
                                                         matchdata, param1 );
        }
    }
}

/** Process the ProtoMS2 solute template 'bond' line, 
    including additional molecule matching criteria specified in 'matchmol' */
void ProtoMS::processBondLine( ProtoMSWS &workspace, const MatchMol &matchmol )
{
    //format of the line is;
    // bond atm0 res0 atm1 res1 ....   where "..." can contain "param 3" or "param 3 4"
    
    const QStringList &words = workspace.words();
    if (words.count() <= 5)
        return;

    //scan for the parameter numbers...
    bool foundparams;
    tuple<QString,QString> params = getParameters(words, 5, &foundparams);
    
    if (foundparams)
    {
        //if there are specified parameters then extract the 
        //atom names of the bonded atoms...
        QString atm0 = words[1];
        QString res0 = words[2];
        QString atm1 = words[3];
        QString res1 = words[4];
        
        const QString &param0 = params.get<0>();
        const QString &param1 = params.get<1>();
        
        //add the relationship
        MatchMRAData matchdata = matchmol and 
                                 atom0_name == atm0 and res0_name == res0 and 
                                 atom1_name == atm1 and res1_name == res1;
                                 
        workspace.bondDB().relateBond( matchdata, param0 );
        
        if (param0 != param1)
        {
            //this bond is being perturbed
            workspace.bondDB().relateBond( mol_state == "reference" and matchdata, param0 );
            workspace.bondDB().relateBond( mol_state == "perturbed" and matchdata, param1 );
        }
    }
}

/** Process the ProtoMS2 solute template 'angle' line, 
    including additional molecule matching criteria specified in 'matchmol' */
void ProtoMS::processAngleLine( ProtoMSWS &workspace, const MatchMol &matchmol )
{
    //format of the line is;
    // bond atm0 res0 atm1 res1 atm2 res2 ....   where "..." can 
    // contain "param 3" or "param 3 4"
    
    const QStringList &words = workspace.words();
    if (words.count() <= 7)
        return;

    //scan for the parameter numbers...
    bool foundparams;
    tuple<QString,QString> params = getParameters(words, 7, &foundparams);
    
    if (foundparams)
    {
        //if there are specified parameters then extract the 
        //atom names of the bonded atoms...
        QString atm0 = words[1];
        QString res0 = words[2];
        QString atm1 = words[3];
        QString res1 = words[4];
        QString atm2 = words[5];
        QString res2 = words[6];
        
        const QString &param0 = params.get<0>();
        const QString &param1 = params.get<1>();
        
        //add the relationship
        MatchMRAData matchdata = matchmol and 
                                 atom0_name == atm0 and res0_name == res0 and 
                                 atom1_name == atm1 and res1_name == res1 and
                                 atom2_name == atm2 and res2_name == res2;
                                 
        workspace.angleDB().relateAngle( matchdata, param0 );
        
        if (param0 != param1)
        {
            //this bond is being perturbed
            workspace.angleDB().relateAngle( mol_state == "reference" and matchdata, param0 );
            workspace.angleDB().relateAngle( mol_state == "perturbed" and matchdata, param1 );
        }
    }
}

/** Process the ProtoMS2 solute template 'dihedral' line, 
    including additional molecule matching criteria specified in 'matchmol' */
void ProtoMS::processDihedralLine( ProtoMSWS &workspace, const MatchMol &matchmol )
{
    //format of the line is;
    // bond atm0 res0 atm1 res1 atm2 res2 atm3 res3 ....   where "..." can 
    // contain "param 3" or "param 3 4"
    
    const QStringList &words = workspace.words();
    if (words.count() <= 9)
        return;

    //scan for the parameter numbers...
    bool foundparams;
    tuple<QString,QString> params = getParameters(words, 9, &foundparams);
    
    if (foundparams)
    {
        //if there are specified parameters then extract the 
        //atom names of the bonded atoms...
        QString atm0 = words[1];
        QString res0 = words[2];
        QString atm1 = words[3];
        QString res1 = words[4];
        QString atm2 = words[5];
        QString res2 = words[6];
        QString atm3 = words[7];
        QString res3 = words[8];
        
        const QString &param0 = params.get<0>();
        const QString &param1 = params.get<1>();
        
        //add the relationship
        MatchMRAData matchdata = matchmol and 
                                 atom0_name == atm0 and res0_name == res0 and 
                                 atom1_name == atm1 and res1_name == res1 and
                                 atom2_name == atm2 and res2_name == res2 and
                                 atom3_name == atm3 and res3_name == res3;
                                 
        workspace.dihedralDB().relateDihedral( matchdata, param0 );
        
        if (param0 != param1)
        {
            //this bond is being perturbed
            workspace.dihedralDB().relateDihedral( mol_state == "reference" and 
                                                   matchdata, param0 );
            workspace.dihedralDB().relateDihedral( mol_state == "perturbed" and 
                                                   matchdata, param1 );
        }
    }
}

/** Process the ProtoMS2 solute template 'ureybradley' line, 
    including additional molecule matching criteria specified in 'matchmol' */
void ProtoMS::processUreyBradleyLine( ProtoMSWS &workspace, const MatchMol &matchmol )
{
    //format of the line is;
    // bond atm0 res0 atm1 res1 atm2 res2 ....   where "..." can 
    // contain "param 3" or "param 3 4"
    
    const QStringList &words = workspace.words();
    if (words.count() <= 7)
        return;

    //scan for the parameter numbers...
    bool foundparams;
    tuple<QString,QString> params = getParameters(words, 7, &foundparams);
    
    if (foundparams)
    {
        //if there are specified parameters then extract the 
        //atom names of the bonded atoms...
        QString atm0 = words[1];
        QString res0 = words[2];
        QString atm1 = words[3];
        QString res1 = words[4];
        QString atm2 = words[5];
        QString res2 = words[6];
        
        const QString &param0 = params.get<0>();
        const QString &param1 = params.get<1>();
        
        //add the relationship
        MatchMRAData matchdata = matchmol and 
                                 atom0_name == atm0 and res0_name == res0 and 
                                 atom1_name == atm1 and res1_name == res1 and
                                 atom2_name == atm2 and res2_name == res2;
                                 
        workspace.ureyBradleyDB().relateUreyBradley( matchdata, param0 );
        
        if (param0 != param1)
        {
            //this bond is being perturbed
            workspace.ureyBradleyDB().relateUreyBradley( mol_state == "reference" and 
                                                         matchdata, param0 );
            workspace.ureyBradleyDB().relateUreyBradley( mol_state == "perturbed" and 
                                                         matchdata, param1 );
        }
    }
}

/** This function reads in a line of a ProtoMS2 chain template */
void ProtoMS::readChainTemplate(ProtoMSWS &workspace)
{
    const QStringList &words = workspace.words();

    if (workspace.firstWord() == "bbatom")
    {
        //format is bbatom id atmnam param0 param1   (or bbatom id atmnam param0)
        if (words.count() < 4)
        {
            workspace.addLog( QObject::tr("WARNING: Error parsing chain template 'bbatom' "
                                          "line %1")
                                    .arg(workspace.lineInfo()) );
            return;
        }
        
        QString atmnam = words[2];
        QString param0 = words[3];
        const QString &resnam = workspace.templateName();
        
        workspace.relateAtom( res_alias == resnam and atom_name == atmnam, param0 );

        if (words.count() > 4 and words[3] != words[4])
        {
            //the backbone atom is being perturbed...
            //get the parameter ID of the perturbed state...
            QString param1 = words[4];
            
            workspace.relateAtom( mol_state == "reference" and res_alias == resnam and
                                  atom_name == atmnam, param0 );
                                                     
            workspace.relateAtom( mol_state == "perturbed" and res_alias == resnam and 
                                  atom_name == atmnam, param1 );
        }
    }
    else if (workspace.firstWord() == "atom")
    {
        //format is atom atmnam param0 param1 ...  or atom atmnam param0 ...
        if (words.count() < 6)
        {
            workspace.addLog( QObject::tr("WARNING: Error parsing chain template atom line %1")
                                  .arg(workspace.lineInfo()) );
            return;
        }
        
        QString atmnam = words[1];
        QString param0 = words[2];
        const QString &resnam = workspace.templateName();
        
        workspace.relateAtom( res_alias == resnam and atom_name == atmnam, param0 );
                                               
        if (words.count() > 6 and words[2] != words[3])
        {
            //the atom is being perturbed
            QString param1 = words[3];
            
            workspace.relateAtom( mol_state == "reference" and res_alias == resnam and  
                                  atom_name == atmnam, param0 );
            
            workspace.relateAtom( mol_state == "perturbed" and res_alias == resnam and 
                                  atom_name == atmnam, param1 );
        }
    }
    else if (workspace.firstWord() == "bond")
    {
        processBondLine( workspace, res_alias == workspace.templateName() );
    }
    else if (workspace.firstWord() == "angle")
    {
        processAngleLine( workspace, res_alias == workspace.templateName() );
    }
    else if (workspace.firstWord() == "dihedral")
    {
        processDihedralLine( workspace, res_alias == workspace.templateName() );
    }
    else if (workspace.firstWord() == "ureybradley")
    {
        processUreyBradleyLine( workspace, res_alias == workspace.templateName() );
    }
}

/** Return the residue bonding pattern implied by the residue position string 'respos'.
    Note that this will only work for amino acid residues that use standard atom names, 
    and when atom names are case-insensitive. The mappings are;
     
    "first" -> Bond to "C" but not bond to "N"
    "middle" -> Bond to "C" and "N"
    "last" -> Bond to "N" but no bond to "C"
    "single" -> No bond to "C" or "N"
     
    It is a shame that this doesn't map correctly for non-amino acid residues, but
    it should be remembered that the use of "first", "middle", "last" and "single"
    was a bit of a hack to begin with! :-) and that I don't know of any current
    users of ProtoMS who are writing residue templates for anything other than
    protein chains (indeed, *only* Julien and I have written residue templates...)
*/
MatchRes getResidueBonding(const QString &respos)
{
    QString lower = respos.toLower();
    
    MatchRes ret;
    
    if (lower == "first")
    {
        ret.addResResBond("C");
        ret.addNoResResBond("N");
    } 
    else if (lower == "middle")
    {
        ret.addResResBond("C");
        ret.addResResBond("N");
    }
    else if (lower == "last")
    {
        ret.addResResBond("N");
        ret.addNoResResBond("C");
    }
    else if (lower == "single")
    {
        ret.addNoResResBond("N");
        ret.addNoResResBond("C");
    }
        
    return ret;
}

/** Return the residue bonding pattern implied by the integer 'respos'.
    Note that this will only work for amino acid residues that use standard atom names, 
    and when atom names are case-insensitive. The mappings are;
     
    0 -> Bond to "C" but no bond to "N"  ("first")
    1 -> no bond requirements            ("middle" or "single")
    2 -> Bond to "N" but no bond to "C"  ("last")
*/
MatchRes getResidueBonding(int respos)
{
    MatchRes ret;

    switch(respos)
    {
        case 0:
            ret.addResResBond("C");
            ret.addNoResResBond("N");
            break;
        case 2:
            ret.addResResBond("N");
            ret.addNoResResBond("C");
            break;
    }
    
    return ret;
}

/** This function reads in a ProtoMS2 residue template line */   
void ProtoMS::readResidueTemplate(ProtoMSWS &workspace)
{
    const QString &resnam = workspace.templateName();
    const QStringList &words = workspace.words();
    
    if (workspace.firstWord() == "backbone")
    {
        //relate this residue to the misc tables representing the backbone atoms
        //format is backbone first chainf  middle chainm  last chainl....
        for (int i=1; i+1 < words.count(); i+=2)
        {
            workspace.relateMRDB().alias( res_name == resnam and 
                                          getResidueBonding(words[i]), 
                                          words[i+1] );
        }
    }
    else if (workspace.firstWord() == "atom")
    {
        //format is (a)  atom atmnam param ....
        //       or (b)  atom atmnam param0 param1 ....
        //       or (c)  atom atmnam param0-f param1-f param0-m param1-m param0-l param1-l
        
        QString atmnam = words[1];
        
        if (words.count() == 6)
        {
            //we are in case (a)
            QString param = words[2];
            
            workspace.relateAtom( res_name == resnam and atom_name == atmnam, param );
        }
        else if (words.count() == 7)
        {
            //we are in case (b)
            QString param0 = words[2];
            QString param1 = words[3];
            
            workspace.relateAtom( res_name == resnam and atom_name == atmnam, param0 );
                                                   
            if (param0 != param1)
            {
                //the atom is being perturbed
                workspace.relateAtom( mol_state == "reference" and res_name == resnam and
                                      atom_name == atmnam, param0 );
                                      
                workspace.relateAtom( mol_state == "perturbed" and res_name == resnam and 
                                      atom_name == atmnam, param1 );
            }
        }
        else if (words.count() == 11)
        {
            //we are in case (c)
            
            for (int i=0; i<3; i++)
            {
                //get the match for the position of the residue
                MatchRes respos = getResidueBonding(i);
            
                QString param0 = words[2*i + 2];
                QString param1 = words[2*i + 3];
            
                //get the match for the atom in this residue
                MatchMRAData atmmatch = (res_name == resnam and 
                                         respos and atom_name == atmnam);
            
                workspace.relateAtom( atmmatch, param0 );
                                                       
                if (param0 != param1)
                {
                    //this atom is being perturbed
                    workspace.relateAtom( mol_state == "reference" and atmmatch,
                                          param0 );
                                          
                    workspace.relateAtom( mol_state == "perturbed" and atmmatch,
                                          param1 );
                }
            }
        }
        else
        {
            workspace.addLog( QObject::tr("WARNING: Error parsing residue template "
                                          "atom line %1")
                                  .arg(workspace.lineInfo()) );
            return;
        }
    }
    else if (workspace.firstWord() == "bond")
    {
        processBondLine( workspace, res_name == workspace.templateName() );
    }
    else if (workspace.firstWord() == "angle")
    {
        processAngleLine( workspace, res_name == workspace.templateName() );
    }
    else if (workspace.firstWord() == "dihedral")
    {
        processDihedralLine( workspace, res_name == workspace.templateName() );
    }
    else if (workspace.firstWord() == "ureybradley")
    {
        processUreyBradleyLine( workspace, res_name == workspace.templateName() );
    }
}
    
/** This function reads in a ProtoMS2 solute template line */
void ProtoMS::readSoluteTemplate(ProtoMSWS &workspace)
{
    const QStringList &words = workspace.words();

    if (workspace.firstWord() == "atom")
    {
        //format is  atom atmname resname param ....
        //       or  atom atmname resname param0  param1 ....
        
        if (words.count() < 10)
        {
            workspace.addLog( QObject::tr("WARNING: Error parsing solute template line %1")
                                  .arg(workspace.lineInfo()) );
            return;
        }
        
        QString atmnam = words[1];
        QString resnam = words[2];
        const QString &molnam = workspace.templateName();
        
        workspace.relateAtom( mol_name == molnam and res_name == resnam and 
                              atom_name == atmnam, words[3] );

        if (words.count() == 11 and words[3] != words[4])
        {
            //mutating atom!
            QString param0 = words[3];
            QString param1 = words[4];
            
            workspace.relateAtom( mol_state == "reference" and mol_name == molnam and
                                  res_name == resnam and atom_name == atmnam, param0 );
                                  
            workspace.relateAtom( mol_state == "perturbed" and mol_name == molnam and 
                                  res_name == resnam and atom_name == atmnam, param1 );
        }
    }
    else if (workspace.firstWord() == "bond")
    {
        processBondLine( workspace, mol_name == workspace.templateName() );
    }
    else if (workspace.firstWord() == "angle")
    {
        processAngleLine( workspace, mol_name == workspace.templateName() );
    }
    else if (workspace.firstWord() == "dihedral")
    {
        processDihedralLine( workspace, mol_name == workspace.templateName() );
    }
    else if (workspace.firstWord() == "ureybradley")
    {
        processUreyBradleyLine( workspace, mol_name == workspace.templateName() );
    }
}
    
/** This function reads in a ProtoMS2 solvent template line */
void ProtoMS::readSolventTemplate(ProtoMSWS &workspace)
{
    if (workspace.firstWord() == "atom")
    {
        //format is  atom atmname param
        //       or  atom atmname param0 param1
        
        const QStringList &words = workspace.words();
        QString atmnam = words[1];
        const QString &molnam = workspace.templateName();
        
        workspace.relateAtom( mol_name == molnam and atom_name == atmnam, words[2] );

        if (words.count() > 3 and words[2] != words[3])
        {
            //we have a mutating atom!
            QString param0 = words[2];
            QString param1 = words[3];
            
            workspace.relateAtom( mol_state == "reference" and mol_name == molnam and 
                                  atom_name == atmnam, param0 );
                                  
            workspace.relateAtom( mol_state == "perturbed" and mol_name == molnam and
                                  atom_name == atmnam, param1 );
        }
    }
}
