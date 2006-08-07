#ifndef SIREMOL_TEMPLATEFUNCTION_H
#define SIREMOL_TEMPLATEFUNCTION_H

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireMol
{

class EditMol;
class EditRes;

/**
This is the base class of all TemplateFunctions. A TemplateFunction is a function that is used to apply a molecule or residue template to an EditMol or EditRes.
 
@author Christopher Woods
*/
class SIREMOL_EXPORT TemplateFunction
{
public:
    TemplateFunction();
    
    virtual ~TemplateFunction();

    /** Apply the molecule template in 'tmpl' to the EditMol 'editmol'. If anything 
        goes wrong then an exception is thrown.
        
        \throw SireMol::template_error
    */
    virtual void applyTemplate(const EditMol &tmpl, EditMol &editmol) const=0;
    
    /** Apply the residue template 'tmpl' to the EditRes 'editres'. If anything
        goes wrong then an exception is thrown.
        
        \throw SireMol::template_error
    */
    virtual void applyTemplate(const EditRes &tmpl, EditRes &editres) const=0;

};

}

SIRE_END_HEADER

#endif
