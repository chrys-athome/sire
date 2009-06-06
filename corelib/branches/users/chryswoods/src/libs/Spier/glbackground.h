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

#ifndef SPIER_GLBACKGROUND_H
#define SPIER_GLBACKGROUND_H

#include "glrenderfunction.h"

SIRE_BEGIN_HEADER

namespace Spier
{
class GLBackground;
class GradientBackground;
}

QDataStream& operator<<(QDataStream&, const Spier::GLBackground&);
QDataStream& operator>>(QDataStream&, Spier::GLBackground&);

QDataStream& operator<<(QDataStream&, const Spier::GradientBackground&);
QDataStream& operator>>(QDataStream&, Spier::GradientBackground&);

namespace Spier
{

/** This is the base class of all objects used to render the background
    of the RenderView
    
    @author Christopher Woods
*/
class SPIER_EXPORT GLBackground : public GLRenderFunction
{

friend QDataStream& ::operator<<(QDataStream&, const GLBackground&);
friend QDataStream& ::operator>>(QDataStream&, GLBackground&);

public:
    GLBackground();
    GLBackground(const GLBackground &other);
    
    virtual ~GLBackground();
    
    static const char* typeName()
    {
        return "Spier::GLBackground";
    }
    
    virtual GLBackground* clone() const=0;
};

/** This is a simple gradient background */
class SPIER_EXPORT GradientBackground 
            : public SireBase::ConcreteProperty<GradientBackground,GLBackground>
{

friend QDataStream& ::operator<<(QDataStream&, const GradientBackground&);
friend QDataStream& ::operator>>(QDataStream&, GradientBackground&);

public:
    GradientBackground();
    GradientBackground(const GradientBackground &other);
    
    ~GradientBackground();
    
    GradientBackground& operator=(const GradientBackground &other);
    
    bool operator==(const GradientBackground &other) const;
    bool operator!=(const GradientBackground &other) const;
    
    static const char* typeName();
    
    GradientBackground* clone() const
    {
        return new GradientBackground(*this);
    }
    
    void operator()() const;
};

}

Q_DECLARE_METATYPE( Spier::GradientBackground )

SIRE_EXPOSE_CLASS( Spier::GLBackground )
SIRE_EXPOSE_CLASS( Spier::GradientBackground )

SIRE_END_HEADER

#endif
