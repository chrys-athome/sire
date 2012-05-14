/********************************************\
  *
  *  Conspire
  *
  *  Copyright (C) 2012  Christopher Woods
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

#include "Conspire/GUI/optionscommand.h"
#include "Conspire/GUI/configdocument.h"

#include "Conspire/option.h"
#include "Conspire/exceptions.h"

using namespace Conspire;

/////////
///////// Implementation of OptionsCommand
/////////

/** Constructor */
OptionsCommand::OptionsCommand(OptionsCommand *parent) 
               : QUndoCommand(parent), old_state(0)
{}

/** Constructor */
OptionsCommand::OptionsCommand(ConfigDocument *document, OptionsCommand *parent)
               : QUndoCommand(parent), doc(document), old_state(0)
{}

/** Destructor */
OptionsCommand::~OptionsCommand()
{
    delete old_state;
}

/** Return the state of the Options object before this command was applied */
Options OptionsCommand::oldState() const
{
    if (not old_state)
        throw Conspire::program_bug( Conspire::tr(
                "Should not be trying to get the old state of an OptionsCommand "
                "before the command has even been run!"), CODELOC );
                
    return *old_state;
}

/** Undo the effects of this command */
void OptionsCommand::undo()
{
    conspireDebug() << "UNDO" << this->text();

    if (doc and old_state)
    {
        doc->setOptions(*old_state);
        delete old_state;
        old_state = 0;
    }
    else
    {
        delete old_state;
        old_state = 0;
    }
    
    QUndoCommand::undo();
}

/** Run this command */
void OptionsCommand::redo()
{
    conspireDebug() << "REDO" << this->text();

    QUndoCommand::redo();

    if (doc)
    {
        delete old_state;
        old_state = new Options(doc->options());
        Options new_state;

        try
        {
            new_state = this->newState();
        }
        catch(...)
        {
            delete old_state;
            old_state = 0;
            throw;
        }
        
        if (*old_state == new_state)
        {
            delete old_state;
            old_state = 0;
        }
        else
        {
            try
            {
                conspireDebug() << "doc->setOptions( new_state )";
                doc->setOptions(new_state);
            }
            catch(...)
            {
                conspireDebug() << "doc->setOptions() EXCEPTION!";
                delete old_state;
                old_state = 0;
                throw;
            }
        }
    }
}

/////////
///////// Implementation of OptionsUpdateCommand
/////////

/** Constructor */
OptionsUpdateCommand::OptionsUpdateCommand(OptionsCommand *parent)
                     : OptionsCommand(parent)
{}

/** Constructor */
OptionsUpdateCommand::OptionsUpdateCommand(ConfigDocument *document, 
                                           OptionsCommand *parent)
                     : OptionsCommand(document,parent)
{}

/** Construct to set the option with key 'key' to the value 'new_value' */
OptionsUpdateCommand::OptionsUpdateCommand(ConfigDocument *document,
                                           QString key, 
                                           const Value &new_value, 
                                           OptionsCommand *parent)
                     : OptionsCommand(document,parent), k(key), val(new_value)
{
    setText( Conspire::tr("Set option \"%1\" to value \"%2\"")
                    .arg(key, new_value.toString()) );
}

/** Destructor */
OptionsUpdateCommand::~OptionsUpdateCommand()
{}

Options OptionsUpdateCommand::newState() const
{
    if (k.isEmpty())
        return oldState();
    else
        return oldState().setNestedValue(k, val.asA<Value>()).asA<Options>();
}

/////////
///////// Implementation of OptionsAddCommand
/////////

/** Constructor */
OptionsAddCommand::OptionsAddCommand(OptionsCommand *parent)
                  : OptionsCommand(parent)
{}

/** Constructor */
OptionsAddCommand::OptionsAddCommand(ConfigDocument *document, OptionsCommand *parent)
                  : OptionsCommand(document, parent)
{}

/** Construct the command to add the key 'new_key' */
OptionsAddCommand::OptionsAddCommand(ConfigDocument *document,
                                     QString new_key, OptionsCommand *parent)
                  : OptionsCommand(document, parent), k(new_key)
{
    setText( Conspire::tr("Add new option \"%1\"").arg(new_key) );
}
  
/** Destructor */
OptionsAddCommand::~OptionsAddCommand()
{}

Options OptionsAddCommand::newState() const
{
    if (k.isEmpty())
        return oldState();
    else
        return oldState().addDefaultValue(k).asA<Options>();
}

/////////
///////// Implementation of OptionsRemoveCommand
/////////

/** Constructor */
OptionsRemoveCommand::OptionsRemoveCommand(OptionsCommand *parent)
                     : OptionsCommand(parent)
{}

/** Constructor */
OptionsRemoveCommand::OptionsRemoveCommand(ConfigDocument *document, 
                                           OptionsCommand *parent)
                     : OptionsCommand(document, parent)
{}

/** Construct to remove the option with the key 'take_key' */
OptionsRemoveCommand::OptionsRemoveCommand(ConfigDocument *document,
                                           QString take_key, OptionsCommand *parent)
                     : OptionsCommand(document, parent), k(take_key)
{
    setText( Conspire::tr("Remove option \"%1\"").arg(take_key) );
}

/** Destructor */
OptionsRemoveCommand::~OptionsRemoveCommand()
{}

Options OptionsRemoveCommand::newState() const
{
    if (k.isEmpty())
        return oldState();
    else
        return oldState().removeValue(k).asA<Options>();
}
