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

/** Return the indicies of all of the residues in this molecule that
    are called 'name' - this returns the ResIdx indicies of the 
    residues sorted in the order that they appear in the molecule.
    This raises an exception if there are no residues with this
    name in this molecule.
    
    \throw SireMol::missing_residue
*/
QList<ResIdx> MoleculeInfoData::map(const ResName &name) const
{
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
    QList<ResIdx> residxs;
    residxs.append( idx.map(res_by_index.count()) );
    
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
    QList<ChainIdx> chainidxs = chain_by_name.values(name);
    
    if (chainidxs.isEmpty())
}

/** Obvious function that maps the ChainIdx to a list of ChainIdx
    objects
    
    \throw SireError::invalid_index
*/
QList<ChainIdx> MoleculeInfoData::map(const ChainIdx &idx) const
{
    QList<ChainIdx> chainidxs;
    chainidxs.append( idx.map(chains_by_index.count()) );
    
    return chainidxs;
}

/** Return the indicies of chains that match the ChainID 'chainid'

    \throw SireMol::missing_chain
    \throw SireError::invalid_index
*/
QList<ChainIdx> MoleculeInfoData::map(const ChainID &chainid) const
{
    return chainid.map(*this);
}

/** Return the indicies of the segments that have the name 'name'

    \throw SireMol::missing_segment
*/
QList<SegIdx> MoleculeInfoData::map(const SegName &name) const
{
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
    QList<SegIdx> segidxs;
    segidxs.append( idx.map(seg_by_index.count()) );
    
    return segidxs;
}

/** Return the indicies of segments that match the ID 'segid'

    \throw SireMol::missing_segment
    \throw SireError::invalid_index
*/
QList<SegIdx> MoleculeInfoData::map(const SegID &segid) const
{
    return segid.map(*this);
}

/** Return the indicies of CutGroups that have the name 'name'

    \throw SireMol::missing_cutgroup
*/
QList<CGIdx> MoleculeInfoData::map(const CGName &name) const
{
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
    QList<CGIdx> cgidxs;
    cgidxs.append( idx.map(cg_by_index.count()) );
    
    return cgidxs;
}

/** Return the indicies of CutGroups that match the ID 'cgid'

    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
QList<CGIdx> MoleculeInfoData::map(const CGID &cgid) const
{
    return cgid.map(*this);
}
    
/** Return the indicies of atoms that are called 'name'
 
    \throw SireMol::missing_atom
*/
QList<AtomIdx> MoleculeInfoData::map(const AtomName &name) const
{
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
    QList<AtomIdx> atomidxs;
    
    //loop through all of the atoms
    int nats = atoms_by_index.count();
    const AtomInfo *atoms_by_index_array = atoms_by_index.constData();
    
    for (int i=0; i<nats; ++i)
    {
        if (atoms_by_index_array[i].number = num)
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
    QList<AtomIdx> atomidxs;
    atomidxs.append( idx.map(atoms_by_index.count()) );
    
    return atomidxs;
}

/** Return the indicies of atoms that match the ID 'atomid'

    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
QList<AtomIdx> MoleculeInfoData::map(const AtomID &atomid) const
{
    return atomid.map(*this);
}

/** Return the indicies of residues that are in the chains that match
    the specified ID
    
    \throw SireMol::missing_chain
    \throw SireError::invalid_index
*/
QList<ResIdx> MoleculeInfoData::getResiduesIn(const ChainID &chainid) const
{
    QList<ChainIdx> chainidxs = chainid.map(*this);
    
    QList<ResIdx> residxs;
    
    foreach (ChainIdx chainidx, chainidxs)
    {
        residxs.append( chains_by_index.at(chainidx).res_indicies.toList() );
    }

    if (chainidxs.count() > 1)
        qSort(residxs);
    
    return residxs;
}

void MoleculeInfoData::_pvt_getAtomsIn(const QList<ResIdx> &residx, 
                                       QList<AtomIdx> &atomidxs) const
{
    const ResInfo *res_by_index_array = res_by_index.constData();

    foreach (ResIdx residx, residxs)
    {
        atomidxs.append( res_by_index_array[residx].atom_indicies.toList() );
    }
}

/** Return the indicies of all of the atoms that are in the 
    residues identified by the ID 'resid'
    
    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
QList<AtomIdx> MoleculeInfoData::getAtomsIn(const ResID &resid) const
{
    //get the indicies of matching residues
    QList<ResIdx> residxs = resid.map(*this);
    
    //add all of the atomidxs together
    QList<AtomIdx> atomidxs;
    
    this->_pvt_getAtomsIn(residxs, atomidxs);
    
    if (residxs.count() > 1)
        qSort(atomidxs);
    
    return atomidxs;
}

void MoleculeInfoData::_pvt_getAtomsIn(const QList<ResIdx> residxs, AtomName name,
                                       QList<AtomIdx> &atomidxs) const
{
    const ResInfo *res_by_index_array = res_by_index.constData();
    
    foreach (ResIdx residx, residxs)
    {
        atomidxs.append( res_by_index_array[residx].atoms_by_name.values(name) );
    }
}

/** Return the indicies of all of the atoms that have the name
    'name' that are in the residues identified by the ID 'resid'
    
    \throw SireMol::missing_atom
    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
QList<AtomIdx> MoleculeInfoData::getAtomsIn(const ResID &resid,
                                            const AtomName &name) const
{
    //get the indicies of matching residues
    QList<ResIdx> residxs = this->map(resid);
    
    //now only search these residues for the atom with the 
    //specified name
    QList<AtomIdx> atomidxs;

    this->_pvt_getAtomsIn(residxs, name, atomidxs);
    
    if (atomidxs.isEmpty())
        throw SireMol::missing_atom( QObject::tr(
            "There is no atom called \"%1\" in the residue %2 in the "
            "molecule %3.").arg(name, resid.toString(), molname), CODELOC );

    qSort(atomidxs);

    return atomidxs;
}

/** Return the indicies of all of the atoms that are in the chains
    with ID 'chainid'
    
    \throw SireMol::missing_chain
    \throw SireError::invalid_index
*/
QList<AtomIdx> MoleculeInfoData::getAtomsIn(const ChainID &chainid) const
{
    QList<ResIdx> residxs = this->getResiduesIn(chainid);
    
    QList<AtomIdx> atomidxs;

    this->_pvt_getAtomsIn(residxs, atomidxs);

    if (residxs.count() > 1)
        qSort(atomidxs);
    
    return atomidxs;
}

/** Return the indicies of all atoms that are in the chain identified
    by 'chainid' and are also called 'name'
    
    \throw SireMol::missing_chain
    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
QList<AtomIdx> MoleculeInfoData::getAtomsIn(const ChainID &chainid, 
                                            const AtomName &name) const
{
    QList<ResIdx> residxs = this->getResiduesIn(chainid);
    
    QList<AtomIdx> atomidxs;
    
    this->_pvt_getAtomsIn(residxs, name, atomidxs);
    
    if (atomidxs.isEmpty())
        throw SireMol::missing_molecule( QObject::tr(
            "There are no atoms called \"%1\" in the chains identified "
            "by %2 in the molecule \"%3\".")
                .arg(name, chainid.toString(), molname), CODELOC );
    
    qSort(atomidxs);
    
    return atomidxs;
}

/** Return the indicies of the atoms in the segment identified by 
    the segment with ID 'segid'
    
    \throw SireMol::missing_segment
    \throw SireError::invalid_index
*/
QList<AtomIdx> MoleculeInfoData::getAtomsIn(const SegID &segid) const
{
    QList<SegIdx> segidxs = segid.map(*this);
    
    QList<AtomIdx> atomidxs;
    
    foreach (SegIdx segidx, segidxs)
    {
        atomidxs.append( seg_by_index[segidx].atom_indicies.toList() );
    }
    
    if (segidx.count() > 1)
        qSort(atomidxs);
    
    return atomidxs;
}

/** Return the indicies of the atoms in the CutGroups identified
    by the ID 'cgid'
    
    \throw SireMol::missing_cutgroup
    \throw SireError::invalid_index
*/
QList<AtomIdx> MoleculeInfoData::getAtomsIn(const CGID &cgid) const
{
    QList<CGIdx> cgidxs = cgid.map(*this);
    
    QList<AtomIdx> atomidxs;
    
    foreach (CGIdx cgidx, cgidxs)
    {
        atomidxs.append( cg_by_index[cgidx].atom_indicies.toList() );
    }
    
    if (cgidxs.count() > 1)
        qSort(atomidxs);
        
    return atomidxs;
}
