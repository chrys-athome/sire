#ifndef SIREN_EDITOR_HPP
#define SIREN_EDITOR_HPP
/********************************************\
  *
  *  Siren - C++ metaobject library
  *
  *  Copyright (C) 2011  Christopher Woods
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

#include "Siren/siren.h"
#include "Siren/editor.h"
#include "detail/boostsupport.h"

SIREN_BEGIN_HEADER

namespace Siren
{
    /** Empty constructor */
    template<class EditorClass, class ViewClass>
    SIREN_OUTOFLINE_TEMPLATE
    Editor<EditorClass,ViewClass>::Editor()
    {}
    
    /** Construct to edit the passed view */
    template<class EditorClass, class ViewClass>
    SIREN_OUTOFLINE_TEMPLATE
    Editor<EditorClass,ViewClass>::Editor(const ViewClass &view)
         : orig_view(view)
    {}
    
    /** Copy constructor */
    template<class EditorClass, class ViewClass>
    Editor<EditorClass,ViewClass>::Editor(const Editor<EditorClass,ViewClass> &other)
         : orig_view(other.orig_view)
    {}
    
    /** Destructor */
    template<class EditorClass, class ViewClass>
    Editor<EditorClass,ViewClass>::~Editor()
    {}
    
    /** Default implementation of the "commit" function.
        A compile-time error is raised if you fail to override
        this function in your own class */
    template<class EditorClass, class ViewClass>
    ViewClass Editor<EditorClass,ViewClass>::commit()
    {
        // YOU MUST IMPLEMENT YOUR OWN VERSION OF THIS FUNCTION
        // IN YOUR DERIVED CLASS - A COMPILER ERROR WILL RESULT
        // IF YOU FAIL TO IMPLEMENT THIS FUNCTION
        SIREN_STATIC_ASSERT(sizeof(EditorClass) + sizeof(ViewClass) == 0);
                            
        return View();
    }
    
    /** Set this equal to the passed view */
    template<class EditorClass, class ViewClass>
    EditorClass& Editor<EditorClass,ViewClass>::operator=(const ViewClass &view)
    {
        return static_cast<EditorClass*>(this)->operator=( view.edit() );
    }
    
    /** Revert back to the original version of the object */
    template<class EditorClass, class ViewClass>
    ViewClass Editor<EditorClass,ViewClass>::revert()
    {
        static_cast<EditorClass*>(this)->operator=( orig_view.edit() );
        return orig_view;
    }
    
    /** Copy assignment operator - it is a compile error not to implement
        this in your own class */
    template<class EditorClass, class ViewClass>
    EditorClass& Editor<EditorClass,ViewClass>::operator=(const EditorClass &other)
    {
        // YOU MUST IMPLEMENT YOUR OWN VERSION OF THIS FUNCTION
        // IN YOUR DERIVED CLASS - A COMPILER ERROR WILL RESULT
        // IF YOU FAIL TO IMPLEMENT THIS FUNCTION
        SIREN_STATIC_ASSERT(sizeof(EditorClass) + sizeof(ViewClass) == 0);
    
        return static_cast<EditorClass&>(*this);
    }
    
    /** Comparison operator */
    template<class EditorClass, class ViewClass>
    bool Editor<EditorClass,ViewClass>::operator==(const ViewClass &view) const
    {
        return static_cast<const EditorClass*>(this)->operator==(view.edit());
    }
    
    /** Comparison operator */
    template<class EditorClass, class ViewClass>
    bool Editor<EditorClass,ViewClass>::operator!=(const ViewClass &view) const
    {
        return not static_cast<const EditorClass*>(this)->operator==(view.edit());
    }
    
    /** Comparison operator - it is a compile error not to implement 
        this in your own class */
    template<class EditorClass, class ViewClass>
    bool Editor<EditorClass,ViewClass>::operator==(const EditorClass &other) const
    {
        // YOU MUST IMPLEMENT YOUR OWN VERSION OF THIS FUNCTION
        // IN YOUR DERIVED CLASS - A COMPILER ERROR WILL RESULT
        // IF YOU FAIL TO IMPLEMENT THIS FUNCTION
        SIREN_STATIC_ASSERT(sizeof(EditorClass) + sizeof(ViewClass) == 0);

        return false;
    }
    
    /** Comparison operator */
    template<class EditorClass, class ViewClass>
    bool Editor<EditorClass,ViewClass>::operator!=(const EditorClass &other) const
    {
        return not static_cast<const EditorClass*>(this)->operator==(other);
    }
    
    /** Return the original version of the view being edited. Note
        that this will return a default-constructed object if this
        editor was created using a "quickEdit" call */
    template<class EditorClass, class ViewClass>
    SIREN_OUTOFLINE_TEMPLATE
    const ViewClass& Editor<EditorClass,ViewClass>::originalView() const
    {
        return orig_view;
    }

} // end of namespace Siren


SIREN_END_HEADER

#endif // ifndef SIREN_EDITOR_HPP
