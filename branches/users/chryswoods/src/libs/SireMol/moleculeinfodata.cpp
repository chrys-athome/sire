/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007  Christopher Woods
  *
  *  This program is free software; you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation; either version 2 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program; if not, write to the Free Software
  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  *  For full details of the license please see the COPYING file
  *  that should have come with this distribution.
  *
  *  You can contact the authors via the developer's mailing list
  *  at http://siremol.org
  *
\*********************************************/

#include "moleculeinfodata.h"

#include "SireMol/errors.h"

using namespace SireMol;
using namespace SireID;

/** Null constructor */
MoleculeInfoData::MoleculeInfoData()
                 : QSharedData()
{}
    
/** Copy constructor */
MoleculeInfoData::MoleculeInfoData(const MoleculeInfoData &other)
                 : QSharedData(),
                   molname(other.molname),
                   molnum(other.molnum),
                   atoms_by_index(other.atoms_by_index),
                   res_by_index(other.res_by_index),
                   res_by_name(other.res_by_name),
                   res_by_num(other.res_by_num),
                   chains_by_index(other.chains_by_index),
                   chains_by_name(other.chains_by_name),
                   seg_by_index(other.seg_by_index),
                   seg_by_name(other.seg_by_name),
                   cg_by_index(other.cg_by_index),
                   cg_by_name(other.cg_by_name)
{}
  
/** Destructor */  
MoleculeInfoData::~MoleculeInfoData()
{}

/** Copy assignment operator */
MoleculeInfoData& MoleculeInfoData::operator=(const MoleculeInfoData &other)
{
    if (&other != this)
    {
        molname = other.molname;
        molnum = other.molnum;
        atoms_by_index = other.atoms_by_index;
        res_by_index = other.res_by_index;
        res_by_name = other.res_by_name;
        res_by_num = other.res_by_num;
        chains_by_index = other.chains_by_index;
        chains_by_name = other.chains_by_name;
        seg_by_index = other.seg_by_index;
        seg_by_name = other.seg_by_name;
        cg_by_index = other.cg_by_index;
        cg_by_name = other.cg_by_name;
    }
    
    return *this;
}

/** Return the name of the molecule */
const MolName& MoleculeInfoData::name() const
{
    return molname;
}

/** Return the name of the chain that matches the ID 'chainid' */
const ChainName& MoleculeInfoData::name(const ChainID &chainid) const
{
    return chains_by_index[ this->chainIdx(chainid) ].name;
}

/** Return the name of the chain that has the index 'chainidx' */
const ChainName& MoleculeInfoData::name(ChainIdx chainidx) const
{
    chainidx = chainidx.map(chains_by_index.count());
    return chains_by_index[chainidx].name;
}

/** Return the name of the segment with ID 'segid' */
const SegName& MoleculeInfoData::name(const SegID &segid) const
{
    return seg_by_index[ this->segIdx(segid) ].name;
}

/** Return the name of the segment with index 'segidx' */
const SegName& MoleculeInfoData::name(SegIdx segidx) const
{
    segidx = segidx.map(seg_by_index.count());
    return seg_by_index[segidx].name;
}

/** Return the name of the residue with ID 'resid' */
const ResName& MoleculeInfoData::name(const ResID &resid) const
{
    return res_by_index[ this->resIdx(resid) ].name;
}

/** Return the name of the residue with index 'residx' */
const ResName& MoleculeInfoData::name(ResIdx residx) const
{
    residx = residx.map(res_by_index.count());
    return res_by_index[residx].name;
}

/** Return the name of the CutGroup with ID 'cgid' */
const CGName& MoleculeInfoData::name(const CGID &cgid) const
{
    return cg_by_index[ this->cgIdx(cgid) ].name;
}

/** Return the name of the CutGroup with index 'cgidx' */
const CGName& MoleculeInfoData::name(CGIdx cgidx) const
{
    cgidx = cgidx.map(cg_by_index.count());
    return cg_by_index[cgidx].name;
}

/** Return the name of the atom with ID 'atomid' */
const AtomName& MoleculeInfoData::name(const AtomID &atomid) const
{
    return atoms_by_index[ this->atomIdx(atomid) ].name;
}

/** Return the name of the atom at index 'atomidx' */
const AtomName& MoleculeInfoData::name(AtomIdx atomidx) const
{
    atomidx = atomidx.map(atoms_by_index.count());
    return atoms_by_index[atomidx].name;
}

/** Return the number of the residue at ID 'resid' */
ResNum MoleculeInfoData::number(const ResID &resid) const
{
    return res_by_index[ this->resIdx(resid) ].number;
}

/** Return the number of the residue at index 'residx' */
ResNum MoleculeInfoData::number(ResIdx residx) const
{
    residx = residx.map(res_by_index.count());
    return res_by_index[residx].number;
}

/** Return the number of the atom with ID 'atomid' */
AtomNum MoleculeInfoData::number(const AtomID &atomid) const
{
    return atoms_by_index[ this->atomIdx(atomid) ].number;
}

/** Return the number of the atom at index 'atomidx' */
AtomNum MoleculeInfoData::number(AtomIdx atomidx) const
{
    atomidx = atomidx.map(atoms_by_index.count());
    return atoms_by_index[atomidx].number;
}

/** Return the CGAtomIdx of the atom at index 'atomidx' */
CGAtomIdx MoleculeInfoData::cgAtomIdx(AtomIdx atomidx) const
{
    atomidx = atomidx.map(atoms_by_index.count());
    return atoms_by_index[atomidx].cgatomidx;
}

/** Return the CGAtomIdx of the atom with ID 'atomid' */
CGAtomIdx MoleculeInfoData::cgAtomIdx(const AtomID &atomid) const
{
    return atoms_by_index[ this->atomIdx(atomid) ].cgatomidx;
}

/** Return the CGAtomIdxs of all of the atoms that match the ID 'atomid' */
QList<CGAtomIdx> MoleculeInfoData::cgAtomIdxs(const AtomID &atomid) const
{
    QList<AtomIdx> atomidxs = atomid.map(*this);
    
    QList<CGAtomIdx> cgatomidxs;
    
    foreach (AtomIdx atomidx, atomidxs)
    {
        cgatomidxs.append( atoms_by_index[atomidx].cgatomidx );
    }
    
    return cgatomidxs;
}

/** Assert that there is the index for just one atom 
   
    \throw SireMol::missing_atom
    \throw SireMol::duplicate_atom
*/
void MoleculeInfoData::assertSingleAtom(const QList<AtomIdx> &atomidxs) const
{
    if (atomidxs.isEmpty())
        throw SireMol::missing_atom( QObject::tr(
            "Could not find a matching atom in the molecule \"%1\".")
                .arg(molname), CODELOC );
                
    else if (atomidxs.count() > 1)
        throw SireMol::duplicate_atom( QObject::tr(
            "Too many atoms have matched (%1) in the molecule \"%2\".")
                .arg(atomidxs.count()).arg(molname), CODELOC );
}

/** Assert that there is the index for just one residue
   
    \throw SireMol::missing_residue
    \throw SireMol::duplicate_residue
*/
void MoleculeInfoData::assertSingleResidue(const QList<ResIdx> &residxs) const
{
    if (residxs.isEmpty())
        throw SireMol::missing_residue( QObject::tr(
            "Could not find a matching residue in the molecule \"%1\".")
                .arg(molname), CODELOC );
                
    else if (residxs.count() > 1)
        throw SireMol::duplicate_residue( QObject::tr(
            "Too many residues have matched (%1) in the molecule \"%2\".")
                .arg(residxs.count()).arg(molname), CODELOC );
}

/** Assert that there is the index for just one chain 
   
    \throw SireMol::missing_chain
    \throw SireMol::duplicate_chain
*/
void MoleculeInfoData::assertSingleChain(const QList<ChainIdx> &chainidxs) const
{
    if (chainidxs.isEmpty())
        throw SireMol::missing_chain( QObject::tr(
            "Could not find a matching chain in the molecule \"%1\".")
                .arg(molname), CODELOC );
                
    else if (chainidxs.count() > 1)
        throw SireMol::duplicate_chain( QObject::tr(
            "Too many chains have matched (%1) in the molecule \"%2\".")
                .arg(chainidxs.count()).arg(molname), CODELOC );
}

/** Assert that there is the index for just one CutGroup 
   
    \throw SireMol::missing_cutgroup
    \throw SireMol::duplicate_cutgroup
*/
void MoleculeInfoData::assertSingleCutGroup(const QList<CGIdx> &cgidxs) const
{
    if (cgidxs.isEmpty())
        throw SireMol::missing_cutgroup( QObject::tr(
            "Could not find a matching CutGroup in the molecule \"%1\".")
                .arg(molname), CODELOC );
                
    else if (cgidxs.count() > 1)
        throw SireMol::duplicate_cutgroup( QObject::tr(
            "Too many CutGroups have matched (%1) in the molecule \"%2\".")
                .arg(cgidxs.count()).arg(molname), CODELOC );
}

/** Assert that there is the index for just one segment 
   
    \throw SireMol::missing_segment
    \throw SireMol::duplicate_segment
*/
void MoleculeInfoData::assertSingleSegment(const QList<SegIdx> &segidxs) const
{
    if (segidxs.isEmpty())
        throw SireMol::missing_segment( QObject::tr(
            "Could not find a matching segment in the molecule \"%1\".")
                .arg(molname), CODELOC );
                
    else if (segidxs.count() > 1)
        throw SireMol::duplicate_segment( QObject::tr(
            "Too many segments have matched (%1) in the molecule \"%2\".")
                .arg(segidxs.count()).arg(molname), CODELOC );
}

/** Return the index of the atom with ID 'atomid' */
AtomIdx MoleculeInfoData::atomIdx(const AtomID &atomid) const
{
    QList<AtomIdx> atomidxs = atomid.map(*this);
    
    this->assertSingleAtom(atomidxs);
    
    return atomidxs.first();
}

/** Return the index of the atom with CGAtomIdx 'cgatomidx' */
AtomIdx MoleculeInfoData::atomIdx(const CGAtomIdx &cgatomidx) const
{
    const QVector<AtomIdx> &atomidxs 
            = cg_by_index[cgatomidx.cutGroup().map(cg_by_index.count())].atom_indicies;
    
    return atomidxs[cgatomidx.atom().map(atomidxs.count())];
}

/** Return the index of the residue that matches the ID 'resid' */
ResIdx MoleculeInfoData::resIdx(const ResID &resid) const
{
    QList<ResIdx> residxs = resid.map(*this);
    
    this->assertSingleResidue(residxs);
    
    return residxs.first();
}

/** Return the index of the chain that matches the ID 'chainid' */
ChainIdx MoleculeInfoData::chainIdx(const ChainID &chainid) const
{
    QList<ChainIdx> chainidxs = chainid.map(*this);
    
    this->assertSingleChain(chainidxs);
    
    return chainidxs.first();
}

/** Return the index of the segment that matches the ID 'segid' */
SegIdx MoleculeInfoData::segIdx(const SegID &segid) const
{
    QList<SegIdx> segidxs = segid.map(*this);
    
    this->assertSingleSegment(segidxs);
    
    return segidxs.first();
}

/** Return the index of the CutGroup that matches the ID 'cgid' */
CGIdx MoleculeInfoData::cgIdx(const CGID &cgid) const
{
    QList<CGIdx> cgidxs = cgid.map(*this);
    
    this->assertSingleCutGroup(cgidxs);
    
    return cgidxs.first();
}

/** Return the indicies of all of the segments in this molecule */
QList<SegIdx> MoleculeInfoData::getSegments() const
{
    QList<SegIdx> segidxs;
    
    for (int i=0; i<seg_by_index.count(); ++i)
    {
        segidxs.append(SegIdx(i));
    }
    
    return segidxs;
}

/** Return the indicies of all of the CutGroups in this molecule */
QList<CGIdx> MoleculeInfoData::getCutGroups() const
{
    QList<CGIdx> cgidxs;
    
    for (int i=0; i<cg_by_index.count(); ++i)
    {
        cgidxs.append(CGIdx(i));
    }
    
    return cgidxs;
}

/** Return the indicies of all of the chains in this molecule */
QList<ChainIdx> MoleculeInfoData::getChains() const
{
    QList<ChainIdx> chainidxs;
    
    for (int i=0; i<chains_by_index.count(); ++i)
    {
        chainidxs.append(ChainIdx(i));
    }
    
    return chainidxs;
}

/** Return the indicies of all of the residues in this molecule */
QList<ResIdx> MoleculeInfoData::getResidues() const
{
    QList<ResIdx> residxs;
    
    for (int i=0; i<res_by_index.count(); ++i)
    {
        residxs.append(ResIdx(i));
    }
    
    return residxs;
}

/** Return the indicies of all of the residues in the chain at index 'chainidx' */
QList<ResIdx> MoleculeInfoData::getResiduesIn(ChainIdx chainidx) const
{
    return chains_by_index[chainidx.map(chains_by_index.count())]
                    .res_indicies.toList();
}

/** Return the indicies of all of the residues in the chains that match
    the ID 'chainid' */
QList<ResIdx> MoleculeInfoData::getResiduesIn(const ChainID &chainid) const
{
    QList<ChainIdx> chainidxs = chainid.map(*this);
    
    QList<ResIdx> residxs;
    
    foreach (ChainIdx chainidx, chainidxs)
    {
        residxs += chains_by_index[chainidx].res_indicies.toList();
    }
    
    if (chainidxs.count() > 1)
        qSort(residxs);
        
    return residxs;
}

/** Return the indicies of all of the atoms in this molecule */
QList<AtomIdx> MoleculeInfoData::getAtoms() const
{
    QList<AtomIdx> atomidxs;
    
    for (int i=0; i<atoms_by_index.count(); ++i)
    {
        atomidxs.append( AtomIdx(i) );
    }
    
    return atomidxs;
}

/** Return the indicies of all of the atoms in the residue at index 'residx' */
QList<AtomIdx> MoleculeInfoData::getAtomsIn(ResIdx residx) const
{
    return res_by_index[residx.map(res_by_index.count())].atom_indicies.toList();
}

QList<AtomIdx> MoleculeInfoData::_pvt_getAtomsIn(const QList<ResIdx> &residxs) const
{
    QList<AtomIdx> atomidxs;
    
    foreach (ResIdx residx, residxs)
    {
        atomidxs += res_by_index[residx].atom_indicies.toList();
    }
    
    if (residxs.count() > 1)
        qSort(atomidxs);
       
    return atomidxs;
}

/** Return the indicies of all of the atoms in the residues that match
    the ID 'resid' */
QList<AtomIdx> MoleculeInfoData::getAtomsIn(const ResID &resid) const
{
    QList<ResIdx> residxs = resid.map(*this);
    QList<AtomIdx> atomidxs = this->_pvt_getAtomsIn(residxs);
    
    BOOST_ASSERT( not atomidxs.isEmpty() );
    
    return atomidxs;
}

/** Return the indicies of the atoms in the residue at index 'residx'
    that are called 'name' */
QList<AtomIdx> MoleculeInfoData::getAtomsIn(ResIdx residx, 
                                            const AtomName &name) const
{
    QList<AtomIdx> atomidxs 
            = res_by_index[residx.map(res_by_index.count())].atoms_by_name.values(name);
    
    if (atomidxs.isEmpty())
        throw SireMol::missing_atom( QObject::tr(
            "There is no atom called \"%1\" in the residue at index %2 "
            "in the molecule \"%3\".")
                .arg(name).arg(residx).arg(molname), CODELOC );
                
    if (atomidxs.count() > 1)
        qSort(atomidxs);
        
    return atomidxs;
}

QList<AtomIdx> MoleculeInfoData::_pvt_getAtomsIn(const QList<ResIdx> &residxs,
                                                 const AtomName &name) const
{
    QList<AtomIdx> atomidxs;
    
    foreach (ResIdx residx, residxs)
    {
        atomidxs += res_by_index[residx].atoms_by_name.values(name);
    }
                
    if (atomidxs.count() > 1)
        qSort(atomidxs);
    
    return atomidxs;
}

/** Return the indicies of the atoms in the residues that match the ID 'resid' 
    that are called 'name' */
QList<AtomIdx> MoleculeInfoData::getAtomsIn(const ResID &resid,
                                            const AtomName &name) const
{
    QList<ResIdx> residxs = resid.map(*this);

    QList<AtomIdx> atomidxs = this->_pvt_getAtomsIn(residxs,name);
    
    if (atomidxs.isEmpty())
        throw SireMol::missing_atom( QObject::tr(
            "There are no atoms called \"%1\" in the residues matching "
            "%2 in the molecule \"%3\".")
                .arg(name, resid.toString(), molname), CODELOC );

    return atomidxs;
}                                            

/** Return the list of atoms in the chain at index 'chainidx' */
QList<AtomIdx> MoleculeInfoData::getAtomsIn(ChainIdx chainidx) const
{
    QList<ResIdx> residxs = this->getResiduesIn(chainidx);
    QList<AtomIdx> atomidxs = this->_pvt_getAtomsIn(residxs);

    BOOST_ASSERT( not atomidxs.isEmpty() );
    
    return atomidxs;
}

/** Return the list of atoms in the chains identified by 'chainid' */
QList<AtomIdx> MoleculeInfoData::getAtomsIn(const ChainID &chainid) const
{
    QList<ResIdx> residxs = this->getResiduesIn(chainid);
    QList<AtomIdx> atomidxs = this->_pvt_getAtomsIn(residxs);

    BOOST_ASSERT( not atomidxs.isEmpty() );
    
    return atomidxs;
}

/** Return the list of atoms in the chain at index 'chainidx' that
    are also called 'name' */
QList<AtomIdx> MoleculeInfoData::getAtomsIn(ChainIdx chainidx, 
                                            const AtomName &name) const
{
    QList<ResIdx> residxs = this->getResiduesIn(chainidx);
    QList<AtomIdx> atomidxs = this->_pvt_getAtomsIn(residxs, name);
    
    if (atomidxs.isEmpty())
        throw SireMol::missing_atom( QObject::tr(
            "There are no atoms called \"%1\" in the chain at index "
            "%2 in the molecule \"%3\".")
                .arg(name).arg(chainidx).arg(molname), CODELOC );
                
    return atomidxs;
}
                                            
/** Return the list of atoms in the chains identified by the ID 'chainid'
    that are also called 'name' */
QList<AtomIdx> MoleculeInfoData::getAtomsIn(const ChainID &chainid,
                                            const AtomName &name) const
{
    QList<ResIdx> residxs = this->getResiduesIn(chainid);
    QList<AtomIdx> atomidxs = this->_pvt_getAtomsIn(residxs, name);
    
    if (atomidxs.isEmpty())
        throw SireMol::missing_atom( QObject::tr(
            "There are no atoms called \"%1\" in the chains matching "
            "%2 in the molecule \"%3\".")
                .arg(name, chainid.toString(), molname), CODELOC );
    
    return atomidxs;
}
                          
/** Return the indicies of atoms in the CutGroup at index 'cgidx' */
QList<AtomIdx> MoleculeInfoData::getAtomsIn(CGIdx cgidx) const
{
    return cg_by_index[cgidx.map(cg_by_index.count())].atom_indicies.toList();
}

/** Return the indicies of atoms in the CutGroups identified by ID 'cgid' */
QList<AtomIdx> MoleculeInfoData::getAtomsIn(const CGID &cgid) const
{
    QList<CGIdx> cgidxs = cgid.map(*this);
    
    QList<AtomIdx> atomidxs;
    
    foreach (CGIdx cgidx, cgidxs)
    {
        atomidxs += cg_by_index[cgidx].atom_indicies.toList();
    }
    
    BOOST_ASSERT( not atomidxs.isEmpty() );
    
    if (cgidxs.count() > 1)
        qSort(atomidxs);
        
    return atomidxs;
}

/** Return the indicies of the atoms in the segment at index 'segidx' */
QList<AtomIdx> MoleculeInfoData::getAtomsIn(SegIdx segidx) const
{
    return seg_by_index[segidx.map(seg_by_index.count())].atom_indicies.toList();
}

/** Return the indicies of atoms in the segments that match the ID 'segid' */
QList<AtomIdx> MoleculeInfoData::getAtomsIn(const SegID &segid) const
{
    QList<SegIdx> segidxs = segid.map(*this);
    
    QList<AtomIdx> atomidxs;
    
    foreach (SegIdx segidx, segidxs)
    {
        atomidxs += seg_by_index[segidx].atom_indicies.toList();
    }
    
    BOOST_ASSERT( not atomidxs.isEmpty() );
    
    if (segidxs.count() > 1)
        qSort(atomidxs);
        
    return atomidxs;
}

/** Return the index of the chain that contains the residue with index 'residx'

    \throw SireError::invalid_index
*/
ChainIdx MoleculeInfoData::parentChain(ResIdx residx) const
{
    return res_by_index[residx.map(res_by_index.count())].chainidx;
}

/** Return the index of the chain that contains the residue
    identified by the ID 'resid' 
    
    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
ChainIdx MoleculeInfoData::parentChain(const ResID &resid) const
{
    return res_by_index[ this->resIdx(resid) ].chainidx;
}

/** Return the parent chain of the atom at index 'atomidx'

    \throw SireError::invalid_index
*/
ChainIdx MoleculeInfoData::parentChain(AtomIdx atomidx) const
{
    return res_by_index[ atoms_by_index[
                           atomidx.map(atoms_by_index.count())].residx ].chainidx;
}

/** Return the parent chain of the atom identified by the ID 'atomid'

    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
ChainIdx MoleculeInfoData::parentChain(const AtomID &atomid) const
{
    return res_by_index[ atoms_by_index[this->atomIdx(atomid)].residx ].chainidx;
}

/** Return the parent residue of the atom at index 'atomidx'

    \throw SireError::invalid_index
*/
ResIdx MoleculeInfoData::parentResidue(AtomIdx atomidx) const
{
    return atoms_by_index[atomidx.map(atoms_by_index.count())].residx;
}

/** Return the parent residue of the atom identified by 'atomid'

    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
ResIdx MoleculeInfoData::parentResidue(const AtomID &atomid) const
{
    return atoms_by_index[ this->atomIdx(atomid) ].residx;
}

/** Return the parent segment of the atom at index 'atomidx' 

    \throw SireError::invalid_index 
*/
SegIdx MoleculeInfoData::parentSegment(AtomIdx atomidx) const
{
    return atoms_by_index[atomidx.map(atoms_by_index.count())].segidx;
}

/** Return the parent segment of the atom identified by the ID 'atomid'

    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
SegIdx MoleculeInfoData::parentSegment(const AtomID &atomid) const
{
    return atoms_by_index[ this->atomIdx(atomid) ].segidx;
}

/** Return the parent CutGroup of the atom at index 'atomid'

    \throw SireError::invalid_index
*/
CGIdx MoleculeInfoData::parentCutGroup(AtomIdx atomidx) const
{
    return atoms_by_index[atomidx.map(atoms_by_index.count())].cgatomidx.cutGroup();
}

/** Return the parent CutGroup of the atom identified by 'atomid'

    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
CGIdx MoleculeInfoData::parentCutGroup(const AtomID &atomid) const
{
    return atoms_by_index[this->atomIdx(atomid)].cgatomidx.cutGroup();
}

/** Return the number of atoms in the molecule */
int MoleculeInfoData::nAtoms() const
{
    return atoms_by_index.count();
}

int MoleculeInfoData::_pvt_nAtoms(const QVector<ResIdx> &residxs) const
{
    int nats = 0;
    
    foreach (ResIdx residx, residxs)
    {
        nats += res_by_index[residx].atom_indicies.count();
    }
    
    return nats;
}

int MoleculeInfoData::_pvt_nAtoms(const QList<ResIdx> &residxs) const
{
    int nats = 0;
    
    foreach (ResIdx residx, residxs)
    {
        nats += res_by_index[residx].atom_indicies.count();
    }
    
    return nats;
}

int MoleculeInfoData::_pvt_nAtoms(ChainIdx chainidx) const
{
    return this->_pvt_nAtoms( chains_by_index[chainidx].res_indicies );
}

/** Return the number of atoms in the chains identified by 'chainid'

    \throw SireMol::missing_chain
    \throw SireError::invalid_index
*/
int MoleculeInfoData::nAtoms(const ChainID &chainid) const
{
    QList<ChainIdx> chainidxs = chainid.map(*this);
    
    int nats = 0;
    
    foreach (ChainIdx chainidx, chainidxs)
    {
        nats += this->_pvt_nAtoms(chainidx);
    }
    
    return nats;
}

/** Return the number of atoms in the chain at index 'chainidx' 

    \throw SireError::invalid_index
*/
int MoleculeInfoData::nAtoms(ChainIdx chainidx) const
{
    if (chainidx.isNull())
        return this->nAtoms();

    chainidx = ChainIdx(chainidx.map(chains_by_index.count()));
    
    return this->_pvt_nAtoms(chainidx);
}

/** Return the number of atoms in the residue(s) identified by 'resid' 

    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
int MoleculeInfoData::nAtoms(const ResID &resid) const
{
    return this->_pvt_nAtoms( resid.map(*this) );
}

/** Return the number of atoms in the residue at index 'residx'

    \throw SireError::invalid_index
*/
int MoleculeInfoData::nAtoms(ResIdx residx) const
{
    if (residx.isNull())
        return this->nAtoms();

    return res_by_index[residx.map(res_by_index.count())].atom_indicies.count();
}

/** Return the number of residues in the segment identified by 'segid'

    \throw SireMol::missing_segment
    \throw SireError::invalid_index
*/
int MoleculeInfoData::nAtoms(const SegID &segid) const
{
    QList<SegIdx> segidxs = segid.map(*this);
    
    int nats = 0;
    
    foreach (SegIdx segidx, segidxs)
    {
        nats += seg_by_index[segidx].atom_indicies.count();
    }
    
    return nats;
}

/** Return the number of atoms in the segment at index 'segidx' 

    \throw SireError::invalid_index
*/
int MoleculeInfoData::nAtoms(SegIdx segidx) const
{
    if (segidx.isNull())
        return this->nAtoms();
        
    return seg_by_index[segidx.map(seg_by_index.count())].atom_indicies.count();
}

/** Return the number of atoms in the CutGroup(s) that are 
    identified by 'cgid'
    
    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
int MoleculeInfoData::nAtoms(const CGID &cgid) const
{
    QList<CGIdx> cgidxs = cgid.map(*this);
    
    int nats = 0;
    
    foreach (CGIdx cgidx, cgidxs)
    {
        nats += cg_by_index[cgidx].atom_indicies.count();
    }
    
    return nats;
}

/** Return the number of atoms in the CutGroup at index 'cgidx'

    \throw SireError::invalid_index
*/
int MoleculeInfoData::nAtoms(CGIdx cgidx) const
{
    if (cgidx.isNull())
        return this->nAtoms();
        
    return cg_by_index[cgidx.map(cg_by_index.count())].atom_indicies.count();
}

/** Return the number of residues in this molecule */
int MoleculeInfoData::nResidues() const
{
    return res_by_index.count();
}

/** Return the number of residues in the chain identified by 'chainid'

    \throw SireMol::missing_chain
    \throw SireError::invalid_index
*/
int MoleculeInfoData::nResidues(const ChainID &chainid) const
{
    QList<ChainIdx> chainidxs = chainid.map(*this);
    
    int nres = 0;
    
    foreach (ChainIdx chainidx, chainidxs)
    {
        nres += chains_by_index[chainidx].res_indicies.count();
    }
    
    return nres;
}

/** Return the number of residues in the chain at index 'chainidx'

    \throw SireError::invalid_index
*/
int MoleculeInfoData::nResidues(ChainIdx chainidx) const
{
    if (chainidx.isNull())
        return this->nResidues();
    
    return chains_by_index[chainidx.map(chains_by_index.count())].res_indicies.count();
}

/** Return the number of chains in this molecule */
int MoleculeInfoData::nChains() const
{
    return chains_by_index.count();
}

/** Return the number of CutGroups in this molecule */
int MoleculeInfoData::nCutGroups() const
{
    return cg_by_index.count();
}

/** Return the number of segments in this molecule */
int MoleculeInfoData::nSegments() const
{
    return seg_by_index.count();
}

/** Return the indicies of all of the residues in this molecule that
    are called 'name' - this returns the ResIdx indicies of the 
    residues sorted in the order that they appear in the molecule.
    This raises an exception if there are no residues with this
    name in this molecule.
    
    \throw SireMol::missing_residue
*/
QList<ResIdx> MoleculeInfoData::map(const ResName &name) const
{
    if (name.isNull())
        return this->getResidues();
    
    QList<ResIdx> residxs = res_by_name.values(name);
    
    if (residxs.isEmpty())
        throw SireMol::missing_residue( QObject::tr(
            "There is no residue called \"%1\" in the molecule \"%2\".")
                .arg(name, molname), CODELOC );
                
    qSort(residxs);
    return residxs;
}

/** Return the indicies of all of the residues in this molecule that
    have the number 'num' - this returns the ResIdx indicies of the 
    residues sorted in the order that they appear in the molecule.
    This raises an exception if there are no residues with this
    number in this molecule.
    
    \throw SireMol::missing_residue
*/
QList<ResIdx> MoleculeInfoData::map(const ResNum &num) const
{
    if (num.isNull())
        return this->getResidues();

    QList<ResIdx> residxs = res_by_num.values(num);
    
    if (residxs.isEmpty())
        throw SireMol::missing_residue( QObject::tr(
            "There is no residue with the number \"%1\" in the molecule \"%2\".")
                .arg(num).arg(molname), CODELOC );
                
    qSort(residxs);
    return residxs;
}

/** Obvious overload function that allows the trivial mapping of  
    a ResIdx to a list of ResIdxs (that just contains the passed ResIdx.
    This does map the ResIdx into this molecule, and raises an exception
    if this in an invalid ResIdx
    
    \throw SireError::invalid_index
*/
QList<ResIdx> MoleculeInfoData::map(const ResIdx &idx) const
{
    if (idx.isNull())
        return this->getResidues();

    QList<ResIdx> residxs;
    residxs.append( ResIdx(idx.map(res_by_index.count())) );
    
    return residxs;
}

/** Return the indicies of residues that match the generic residue ID 'resid'.
    This returns the indicies sorted in the order the residues appear
    in the molecule, and raises an exception if there is no residue
    in the molecule that matches this ID.
    
    \throw SireMol::missing_residue
*/
QList<ResIdx> MoleculeInfoData::map(const ResID &resid) const
{
    if (resid.isNull())
        return this->getResidues();
        
    return resid.map(*this);
}
    
/** Return the indicies of chains that match the name 'name'. This
    returns the indicies sorted in the order the chains appear
    in the molecule, and raises an exception if there is no chain
    with this name.
    
    \throw SireMol::missing_chain
*/
QList<ChainIdx> MoleculeInfoData::map(const ChainName &name) const
{
    if (name.isNull())
        return this->getChains();

    QList<ChainIdx> chainidxs = chains_by_name.values(name);
    
    if (chainidxs.isEmpty())
        throw SireMol::missing_chain( QObject::tr(
            "There is no chain called \"%1\" in the molecule \"%2\".")
                .arg(name, molname), CODELOC );
                
    qSort(chainidxs);
    return chainidxs;
}

/** Obvious function that maps the ChainIdx to a list of ChainIdx
    objects
    
    \throw SireError::invalid_index
*/
QList<ChainIdx> MoleculeInfoData::map(const ChainIdx &idx) const
{
    if (idx.isNull())
        return this->getChains();

    QList<ChainIdx> chainidxs;
    chainidxs.append( ChainIdx(idx.map(chains_by_index.count())) );
    
    return chainidxs;
}

/** Return the indicies of chains that match the ChainID 'chainid'

    \throw SireMol::missing_chain
    \throw SireError::invalid_index
*/
QList<ChainIdx> MoleculeInfoData::map(const ChainID &chainid) const
{
    if (chainid.isNull())
        return this->getChains();

    return chainid.map(*this);
}

/** Return the indicies of the segments that have the name 'name'

    \throw SireMol::missing_segment
*/
QList<SegIdx> MoleculeInfoData::map(const SegName &name) const
{
    if (name.isNull())
        return this->getSegments();

    QList<SegIdx> segidxs = seg_by_name.values(name);
    
    if (segidxs.isEmpty())
        throw SireMol::missing_segment( QObject::tr(
            "There is no segment called \"%1\" in the molecule \"%2\".")
                .arg(name, molname), CODELOC );
                
    qSort(segidxs);
    return segidxs;
}

/** Obvious function that maps the SegIdx to a list of SegIdx
    objects
    
    \throw SireError::invalid_index
*/
QList<SegIdx> MoleculeInfoData::map(const SegIdx &idx) const
{
    if (idx.isNull())
        return this->getSegments();

    QList<SegIdx> segidxs;
    segidxs.append( SegIdx(idx.map(seg_by_index.count())) );
    
    return segidxs;
}

/** Return the indicies of segments that match the ID 'segid'

    \throw SireMol::missing_segment
    \throw SireError::invalid_index
*/
QList<SegIdx> MoleculeInfoData::map(const SegID &segid) const
{
    if (segid.isNull())
        return this->getSegments();

    return segid.map(*this);
}

/** Return the indicies of CutGroups that have the name 'name'

    \throw SireMol::missing_cutgroup
*/
QList<CGIdx> MoleculeInfoData::map(const CGName &name) const
{
    if (name.isNull())
        return this->getCutGroups();
    
    QList<CGIdx> cgidxs = cg_by_name.values(name);
    
    if (cgidxs.isEmpty())
        throw SireMol::missing_cutgroup( QObject::tr(
            "There is no CutGroup called \"%1\" in the molecule \"%2\".")
                .arg(name, molname), CODELOC );
                
    qSort(cgidxs);
    return cgidxs;
}

/** Obvious function that maps the CGIdx to a list of CGIdx
    objects
    
    \throw SireError::invalid_index
*/
QList<CGIdx> MoleculeInfoData::map(const CGIdx &idx) const
{
    if (idx.isNull())
        return this->getCutGroups();

    QList<CGIdx> cgidxs;
    cgidxs.append( CGIdx(idx.map(cg_by_index.count())) );
    
    return cgidxs;
}

/** Return the indicies of CutGroups that match the ID 'cgid'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
QList<CGIdx> MoleculeInfoData::map(const CGID &cgid) const
{
    if (cgid.isNull())
        return this->getCutGroups();

    return cgid.map(*this);
}
    
/** Return the indicies of atoms that are called 'name'
 
    \throw SireMol::missing_atom
*/
QList<AtomIdx> MoleculeInfoData::map(const AtomName &name) const
{
    if (name.isNull())
        return this->getAtoms();

    QList<AtomIdx> atomidxs;
    
    //loop through all of the atoms...
    int nats = atoms_by_index.count();
    const AtomInfo *atoms_by_index_array = atoms_by_index.constData();
    
    for (int i=0; i<nats; ++i)
    {
        if (atoms_by_index_array[i].name == name)
            atomidxs.append( AtomIdx(i) );
    }
    
    if (atomidxs.isEmpty())
        throw SireMol::missing_atom( QObject::tr(
            "There is no atom called \"%1\" in the molecule \"%2\".")
                .arg(name, molname), CODELOC );
                
    //no need to sort as added in sorted order
    return atomidxs;
}

/** Return the indicies of atoms that have the number 'num'.

    \throw SireMol::missing_atom
*/
QList<AtomIdx> MoleculeInfoData::map(const AtomNum &num) const
{
    if (num.isNull())
        return this->getAtoms();

    QList<AtomIdx> atomidxs;
    
    //loop through all of the atoms
    int nats = atoms_by_index.count();
    const AtomInfo *atoms_by_index_array = atoms_by_index.constData();
    
    for (int i=0; i<nats; ++i)
    {
        if (atoms_by_index_array[i].number == num)
            atomidxs.append( AtomIdx(i) );
    }
    
    //no need to sort as added in sorted order
    return atomidxs;
}

/** Obvious function that maps the AtomIdx to a list of AtomIdx
    objects
    
    \throw SireError::invalid_index
*/
QList<AtomIdx> MoleculeInfoData::map(const AtomIdx &idx) const
{
    if (idx.isNull())
        return this->getAtoms();

    QList<AtomIdx> atomidxs;
    atomidxs.append( AtomIdx(idx.map(atoms_by_index.count())) );
    
    return atomidxs;
}

/** Return the indicies of atoms that match the ID 'atomid'

    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
QList<AtomIdx> MoleculeInfoData::map(const AtomID &atomid) const
{
    if (atomid.isNull())
        return this->getAtoms();

    return atomid.map(*this);
}
