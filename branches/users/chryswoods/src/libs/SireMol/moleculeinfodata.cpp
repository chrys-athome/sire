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

QList<SegIdx> MoleculeInfoData::map(const SegName &name) const;
QList<SegIdx> MoleculeInfoData::map(const SegID &segid) const;
   
QList<CGIdx> MoleculeInfoData::map(const CGName &name) const;
QList<CGIdx> MoleculeInfoData::map(const CGID &cgid) const;
    
QList<AtomIdx> MoleculeInfoData::map(const AtomName &name) const;
QList<AtomIdx> MoleculeInfoData::map(const AtomNum &num) const;
QList<AtomIdx> MoleculeInfoData::map(const AtomID &atomid) const;

/** Return the indicies of residues that simultaneously match both 
    the chain ID and residue IDs supplied. This throws an exception
    if there are no residues or chains that match these IDs
    
    \throw SireMol::missing_chain
    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
QList<ResIdx> MoleculeInfoData::getResiduesIn(const ChainID &chainid, 
                                              const ResID &resid) const
{
    //get a list of all of the residues in the chains that match the
    //ID 'chainid'
    QList<ResIdx> res_in_chains = this->getResiduesIn(chainid);
    
    //now match the residue, limited by 'res_in_chains'
    return resid.map(*this, res_in_chains);
}

/** Return the indicies of all of the atoms that are in the 
    residues identified by the ID 'resid'
    
    \throw SireMol::missing_residue
    \throw SireError::invalid_index
*/
QList<AtomIdx> MoleculeInfoData::getAtomsIn(const ResID &resid) const
{
    //get the indicies of matching residues
    QList<ResIdx> residxs = this->map(resid);
    
    //add all of the atomidxs together
    QList<AtomIdx> atomidxs;
    
    const ResInfo *res_by_index_array = res_by_index.constData();
    
    foreach (ResIdx residx, residxs)
    {
        atomidxs.append( res_by_index_array[residx].atom_indicies.toList() );
    }
    
    qSort(atomidxs);
    
    return atomidxs;
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
    
    const ResInfo *res_by_index_array = res_by_index.constData();
    
    foreach (ResIdx residx, residxs)
    {
        atomidxs.append( res_by_index_array[residx].atoms_by_name.values(name) );
    }
    
    if (atomidxs.isEmpty())
        throw SireMol::missing_atom( QObject::tr(
            "There is no atom called \"%1\" in the residue %2 in the "
            "molecule %3.").arg(name, resid.toString(), molname), CODELOC );

    qSort(atomidxs);

    return atomidxs;
}

/** Return the indicies of all of the atoms in the residues that match
    the ID 'resid' that match the atom ID 'atomid'
    
    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
    \throw SireError::invalid_index
*/
QList<AtomIdx> MoleculeInfoData::getAtomsIn(const ResID &resid,
                                            const AtomID &atomid) const
{
    QList<AtomIdx> atomidxs = this->getAtomsIn(resid);
    return atomid.map(*this, atomidxs);
}

////////// ResIDCombo ///////////////////////////////

/** Return the indicies of residues that match this combined 
    chain-residue ID 'resid' 
    
    \throw SireMol::missing_chain
    \throw SireMol::missing_residue
    \throw SireError:invalid_index
*/
QList<ResIdx> ResIDCombo::map(const MoleculeInfo &molinfo) const
{
    this->assertNotNull();

    if (_chain.isNull())
        return molinfo.map(_res);
    else if (_res.isNull())
        return molinfo.map(_chain);
    else
    {
        QList<ResIdx> res_in_chains = molinfo.getResiduesIn(_chain);
        return _res.map(molinfo, res_in_chains);
    }
}

////////////////////////////////////////////////////////

//////// ResAtomCombo //////////////////////////////////

/** Return the indicies of atoms that match this combined
    res-atom ID 'atomid'
    
    \throw SireMol::missing_residue
    \throw SireMol::missing_atom
*/
QList<AtomIdx> AtomIDCombo::map(const MoleculeInfo &molinfo) const
{
    this->assertNotNull();
    
    if (_res.isNull())
        return molinfo.map(_atom);
    else if (_atom.isNull())
        return molinfo.map(_res);
    else
    {
        return _atom.map(molinfo, _res);
    }
}
