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

#include "Siren/block.h"
#include "Siren/block.hpp"
#include "Siren/exceptions.h"

#include "Siren/detail/blockdata.h"

using namespace Siren;
using namespace Siren::detail;

//////////
////////// Implementation of BlockData
//////////

/** Constructor */
BlockData::BlockData() : noncopyable()
{}

/** Destructor */
BlockData::~BlockData()
{}

//////////
////////// Implementation of Block
//////////

/** Null constructor */
Block::Block()
{}

/** Function called by the different Block constructors to set the
    pointer to the actual block data */
void Block::setData(BlockData *ptr)
{
    d.reset(ptr);

    if (d.get() == 0)
        throw Siren::program_bug( String::tr(
                "A programming error has resulted in a Block "
                "being initialised with a null pointer!"), CODELOC );
}

/** Constructor */
Block::Block(const exp_shared_ptr<BlockData>::Type &ptr) : d(ptr)
{
    if (d.get() == 0)
        throw Siren::program_bug( String::tr(
                "A programming error has resulted in a Block "
                "being initialised with a null pointer!"), CODELOC );
}

/** Copy constructor */
Block::Block(const Block &other) : d(other.d)
{}

/** Destructor */
Block::~Block()
{}

/** Copy assignment operator */
Block& Block::operator=(const Block &other)
{
    d = other.d;
    return *this;
}

/** Return a string representation of this Block */
String Block::toString() const
{
    return d->toString();
}

/** Return the type of block */
const char* Block::what() const
{
    return d->blockType();
}

//////////
////////// Implementation of BlockRef
//////////

/** Null constructor */
BlockRef::BlockRef()
{}

/** Construct a reference to the passed block */
BlockRef::BlockRef(const Block &block) : d(block.d)
{}

/** Copy constructor*/
BlockRef::BlockRef(const BlockRef &other) : d(other.d)
{}

/** Destructor */
BlockRef::~BlockRef()
{}

/** Copy assignment operator */
BlockRef& BlockRef::operator=(const BlockRef &other)
{
    d = other.d;
    return *this;
}

/** Comparison operator */
bool BlockRef::operator==(const Block &other) const
{
    return d.lock().get() == other.d.get();
}

/** Comparison operator */
bool BlockRef::operator!=(const Block &other) const
{
    return not operator==(other);
}

/** Comparison operator */
bool BlockRef::operator==(const BlockRef &other) const
{
    return d.lock().get() == other.d.lock().get();
}

/** Comparison operator */
bool BlockRef::operator!=(const BlockRef &other) const
{
    return not operator==(other);
}

/** Return the type of Block held in this reference - remember
    that the reference could be set to null at any time after 
    calling this function (including immediately after!).
    A null BlockRef will return '0' */
const char* BlockRef::what() const
{
    exp_shared_ptr<BlockData>::Type ptr = d.lock();
    
    if (ptr)
        return ptr->blockType();
    else
        return 0;
}

/** Return a string representation of the block */
String BlockRef::toString() const
{
    exp_shared_ptr<BlockData>::Type ptr = d.lock();
    
    if (ptr)
        return ptr->toString();
    else
        return String::tr("BlockRef::null");
}

/** Internal function used by for_ages to tell the block to check
    for the end of for_ages */
void BlockRef::checkEndForAges() const
{
    exp_shared_ptr<BlockData>::Type ptr = d.lock();
    
    if (ptr)
        ptr->checkEndForAges();
}

/** Internal function used to throw an exception if an attempt 
    is made to cast a null reference to a block */
void BlockRef::throwNullRefError(const char *type_name) const
{
    throw Siren::invalid_state( String::tr(
            "You cannot cast a null block reference to a Block of type %1.")
                .arg(type_name), CODELOC );
}
