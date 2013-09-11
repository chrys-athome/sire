#ifndef SIREN_EDITOR_H
#define SIREN_EDITOR_H
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

#include "siren.h"

SIREN_BEGIN_HEADER

namespace Siren
{
    /** This class provides a simple template for "editor" style   
        classes. An editor class is one that is used to edit
        a Siren::Object, and is returned by calling object.edit().
        Editor classes need to provide certain functionality. This
        template provides that functionality, thereby simplifying
        the process of writing editors. Note that use of this
        template is optional - you can write your own editor class
        that provides the required functionality manually
        
        @author Christopher Woods
    */
    template<class EditorClass, class ViewClass>
    class SIREN_EXPORT Editor
    {
    public:
        typedef ViewClass View;
        
        Editor();
        Editor(const View &view);
        
        Editor(const Editor &other);
        
        ~Editor();
        
        View commit();
        View revert();
        
        EditorClass& operator=(const View &view);
        EditorClass& operator=(const EditorClass &other);
        
        bool operator==(const View &view) const;
        bool operator!=(const View &view) const;
        
        bool operator==(const EditorClass &other) const;
        bool operator!=(const EditorClass &other) const;
        
    protected:
        void updateOriginal(const View &new_version);
    
        const View& originalView() const;
    
    private:
        /** The original copy of the View, so that it can
            be returned by the "revert" function. */
        View orig_view;
    
    }; // end of class Editor
        

} // end of namespace Siren


SIREN_END_HEADER

#endif // ifndef SIREN_EDITOR_H
