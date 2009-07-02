/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2009  Christopher Woods
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

#ifndef SPIER_GLDISPLAYLIST_H
#define SPIER_GLDISPLAYLIST_H

#include <boost/shared_ptr.hpp>

#include "glrenderfunction.h"

SIRE_BEGIN_HEADER

namespace Spier
{
class GLDisplayList;
}

QDataStream& operator<<(QDataStream&, const Spier::GLDisplayList&);
QDataStream& operator>>(QDataStream&, Spier::GLDisplayList&);

namespace Spier
{

class GLRenderContext;

/** This class holds an OpenGL display list. This display
    list is used to save the OpenGL commands contained
    in the passed GLRenderFunction
    
    @author Christopher Woods
*/
class SPIER_EXPORT GLDisplayList
{

friend QDataStream& ::operator<<(QDataStream&, const GLDisplayList&);
friend QDataStream& ::operator>>(QDataStream&, GLDisplayList&);

public:
    GLDisplayList();
    GLDisplayList(const GLRenderFunction &renderfunc);
    
    GLDisplayList(const GLDisplayList &other);
    
    ~GLDisplayList();
    
    GLDisplayList& operator=(const GLDisplayList &other);
    
    bool operator==(const GLDisplayList &other) const;
    bool operator!=(const GLDisplayList &other) const;

    static const char* typeName();
    
    const char* what() const
    {
        return GLDisplayList::typeName();
    }
    
    bool isEmpty() const;

    const GLRenderFunction& renderFunction() const;
    
    void render(GLRenderContext &render_contex) const;

private:
    /** Shared global pointer to the render function 
         - this is global so that only one display list is 
           created for the same function */
    GlobalGLRenderFuncPtr render_func;
};

}

Q_DECLARE_METATYPE( Spier::GLDisplayList )

SIRE_EXPOSE_CLASS( Spier::GLDisplayList )

SIRE_END_HEADER

#endif
