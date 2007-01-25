/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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

#ifndef SIREMM_RESINTERNALINFO_HPP
#define SIREMM_RESINTERNALINFO_HPP

#include "detail/internalinfo.hpp"

SIRE_BEGIN_HEADER

namespace SireMM
{

template<class T>
class MolInternalInfo;

/** This is the base class of all classes that hold the metainformation
    about internals within a residue (e.g. ResBondInfo, ResAngleInfo etc.)
    
    @author Christopher Woods
*/
template<class T>
class SIREMM_EXPORT ResInternalInfo : public detail::InternalInfo<T>
{
public:
    ResInternalInfo();
    ResInternalInfo(const MolInternalInfo<T> &molinfo, ResNum resnum);
    
    ResInternalInfo(const detail::InternalInfo<T> &other);
    
    ~ResInternalInfo();
        
private:
    /** ResInternalInfo cannot be edited, so hide all of the 
        editing functionality of InternalInfo<T> */
    void addInternal(const T&){}
    void removeInternal(const T&){}
    
    /** Cannot extract a residue from a residue! */
    detail::InternalInfo<T> extractResidue(ResNum){ return detail::InternalInfo<T>(); }
};

/** Empty constructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
ResInternalInfo<T>::ResInternalInfo() : detail::InternalInfo<T>()
{}

/** Copy constructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
ResInternalInfo<T>::ResInternalInfo(const detail::InternalInfo<T> &other)
                   : detail::InternalInfo<T>(other)
{}

/** Destructor */
template<class T>
SIRE_OUTOFLINE_TEMPLATE
ResInternalInfo<T>::~ResInternalInfo()
{}

}

SIRE_END_HEADER

#endif
