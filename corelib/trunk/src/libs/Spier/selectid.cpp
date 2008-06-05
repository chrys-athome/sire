
#include "selectid.h"
#include "molselectid.h"

#include <QObject>

using namespace Spier;

SelectID::SelectID() : boost::shared_ptr<SelectIDBase>()
{}

SelectID::SelectID( SelectIDBase *base ) : boost::shared_ptr<SelectIDBase>(base)
{}

SelectID::SelectID( const SelectID &other ) : boost::shared_ptr<SelectIDBase>(other)
{}

SelectID::~SelectID()
{}

SelectID& SelectID::operator=(const SelectID &other)
{
    boost::shared_ptr<SelectIDBase>::operator=(other);
    return *this;
}

bool SelectID::operator==(const SelectID &other) const
{
    if (this->get() != 0 and other.get() != 0)
        return *(this->get()) == *(other.get());
    else
        return this->get() == other.get();
}

bool SelectID::operator!=(const SelectID &other) const
{
    return not (*this == other);
}

bool SelectID::isNull() const
{
    return (this->get() == 0) or this->get()->isNull();
}

ViewObjPtr SelectID::object() const
{
    if (this->get() != 0)
        return ViewObjPtr();
    else
        return this->get()->object();
}
    
void SelectID::select()
{
    if (this->get() != 0)
        this->get()->select();
}

void SelectID::deselect()
{
    if (this->get() != 0)
        this->get()->deselect();
}

void SelectID::toggle()
{
    if (this->get() != 0)
        this->get()->toggle();
}

void SelectID::highlight()
{
    if (this->get() != 0)
        this->get()->highlight();
}

bool SelectID::selected() const
{
    return this->get() != 0 and this->get()->selected();
}

QString SelectID::toString() const
{
    if (this->get() == 0)
        return QObject::tr("Nothing");
    else
        return this->get()->toString();
}

const MolSelectID* SelectID::atomSelection() const
{
    if (this->get() == 0)
        return 0;
    else
        return this->get()->atomSelection();
}

//////////////////
////////////////// Implementation of SelectIDBase
//////////////////

SelectIDBase::SelectIDBase()
{}

SelectIDBase::SelectIDBase(const ViewObjPtr &obj) : selectobj(obj)
{}

SelectIDBase::~SelectIDBase()
{}

/** Return this SelectID cast as an atom selection. 0 is returned if 
    this is not an atom selection */
const MolSelectID* SelectIDBase::atomSelection() const
{
    return dynamic_cast<const MolSelectID*>(this);
}

