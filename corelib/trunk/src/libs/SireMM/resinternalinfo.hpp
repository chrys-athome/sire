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
