#ifndef SPIER_GLWIDGETSET_H
#define SPIER_GLWIDGETSET_H

#include "SireBase/set.hpp"

#include "glwidget.h"

/** Indexing function to allow widgets to be placed into a set. As GLWidget 
    is a shared pointer, we can uniquely identify each widget by using its 
    memory location. */
template<>
inline Spier::GLWidgetBase* set_indexer(const Spier::GLWidget &widget)
{
    return const_cast<Spier::GLWidgetBase*>(widget.get());
}

namespace Spier
{

class GLWidgetBase;

typedef SireBase::Set<GLWidgetBase*, GLWidget> GLWidgetSet;

}

#endif
