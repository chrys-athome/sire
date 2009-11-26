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

#ifndef SIREBASE_GLRENDERFUNCTION_H
#define SIREBASE_GLRENDERFUNCTION_H

#include "SireBase/property.h"

SIRE_BEGIN_HEADER

namespace Spier
{
class GLRenderFunction;
class NullGLRenderFunction;
}

QDataStream& operator<<(QDataStream&, const Spier::GLRenderFunction&);
QDataStream& operator>>(QDataStream&, Spier::GLRenderFunction&);

QDataStream& operator<<(QDataStream&, const Spier::NullGLRenderFunction&);
QDataStream& operator>>(QDataStream&, Spier::NullGLRenderFunction&);

namespace Spier
{

/** This is the base class of all functions that render
    OpenGL content
    
    @author Christopher Woods
*/
class SPIER_EXPORT GLRenderFunction : public SireBase::Property
{

friend QDataStream& ::operator<<(QDataStream&, const Spier::GLRenderFunction&);
friend QDataStream& ::operator>>(QDataStream&, Spier::GLRenderFunction&);

public:
    GLRenderFunction();
    GLRenderFunction(const GLRenderFunction &other);
    
    virtual ~GLRenderFunction();

    static const char* typeName()
    {
        return "Spier::GLRenderFunction";
    }

    virtual GLRenderFunction* clone() const=0;
    
    virtual void operator()() const=0;

    static const NullGLRenderFunction& null();

protected:
    GLRenderFunction& operator=(const GLRenderFunction &other);
    
    bool operator==(const GLRenderFunction &other) const;
    bool operator!=(const GLRenderFunction &other) const;
};

/** This is a null render function that does nothing */
class SPIER_EXPORT NullGLRenderFunction 
         : public SireBase::ConcreteProperty<NullGLRenderFunction,GLRenderFunction>
{

friend QDataStream& ::operator<<(QDataStream&, const NullGLRenderFunction&);
friend QDataStream& ::operator>>(QDataStream&, NullGLRenderFunction&);

public:
    NullGLRenderFunction();
    
    NullGLRenderFunction(const NullGLRenderFunction &other);
    
    ~NullGLRenderFunction();
    
    NullGLRenderFunction& operator=(const NullGLRenderFunction &other);
    
    bool operator==(const NullGLRenderFunction &other) const;
    bool operator!=(const NullGLRenderFunction &other) const;
    
    static const char* typeName();
    
    void operator()() const;
};

typedef SireBase::PropPtr<GLRenderFunction> GLRenderFuncPtr;

typedef SireBase::GlobalPropPtr<GLRenderFunction> GlobalGLRenderFuncPtr;

}

Q_DECLARE_METATYPE(Spier::NullGLRenderFunction)

SIRE_EXPOSE_CLASS( Spier::GLRenderFunction )
SIRE_EXPOSE_CLASS( Spier::NullGLRenderFunction )

SIRE_EXPOSE_PROPERTY( Spier::GLRenderFuncPtr, Spier::GLRenderFunction )

SIRE_END_HEADER

#endif
