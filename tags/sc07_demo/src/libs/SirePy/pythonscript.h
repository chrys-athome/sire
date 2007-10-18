/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2006  Christopher Woods
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

#ifndef SIREPY_PYTHONSCRIPT_H
#define SIREPY_PYTHONSCRIPT_H

#include <QStringList>
#include <QReadWriteLock>

#include "pythonerror.h"

SIRE_BEGIN_HEADER

namespace SirePy
{

/** The status of the Python Script */
enum ScriptStatus{ SCRIPT_INCOMPLETE, //there are still parts of the script that need to be run
                   SCRIPT_ERROR,      //there is an error with part of the script
                   SCRIPT_EMPTY,      //there is nothing contained in the script
                   SCRIPT_COMPLETE};  //all the blocks of the script have been run

/**
This is a complete cache and representation of a Python script. Text can be added onto the end of the script while a parser tries to read text from the top of the script. The PythonScript organisers the script into blocks, where each block is a complete, compilable block of Python code. The class keeps a record of all blocks of text that have been parsed, the current block being parsed, and a list of all of the text that has yet to be parsed. The status (run, error, pending) is available for each block, as is a PythonError if there is an error. It is possible to edit any part of the script that has not yet been parsed.

Note that this class is thread-safe. This means that you can add python to the end of the script while it is still running! You can also edit any parts of the script that have not already been run.
 
@author Christopher Woods
*/
class SIREPY_EXPORT PythonScript
{
public:
    PythonScript();
    virtual ~PythonScript();

    /** Add some text to the end of the script. This assumes that the text
        should end on a newline (a newline will be added if one does not exist) */
    void addText(const QString &text);
    
    /** Add some text to the end of the script from 'file'. This asssumes that the
        file should end on a newline (one will be added if one does not exist) */
    void addFile(const QString &file);
    
    /** Returns the status of the script (EMPTY, INCOMPLETE, COMPLETE, ERROR) */
    ScriptStatus status();
    
    /** Take an idle block to be parsed - returns an empty QString() if there are
        no idle blocks waiting to be parsed, or there was an error with the current
        block */
    QString nextBlock();

    /** Tell the script that there was an error with the current statement.
        This will need to be edited or cleared before the script can continue. */
    void setError(const PythonError &err);

    /** Replace the current block with the provided block. This function
        will only work if there was an error with the current statement */
    void correctError(const QString &newblock);
    
    /** Clear the error (ignore it), so that we can continue with the rest
        of the script. Note that this will discard the broken block, which
        will not be added to the history of run blocks. */
    void ignoreError();
        
    /** For debugging, print out the entire script, with the status of
        each block */
    void debugScript();

    /** Return the current Python error (which is empty if there is no current error) */
    PythonError error();
            
protected:
    /** Set the status of the script */
    void setStatus(ScriptStatus s);

    /** Print block 'i', starting from line 'line'  (note that both 'i' and
        'line' will be incremented as each line is printed) */
    void printDebugBlock(const QString &block, int &i, int &line);
        
    /** Append a statement block onto the list of pending blocks */
    void addPendingBlock(const QString &block);
    
    /** A list of all of the blocks that have been correctly
        run by the parser */
    QStringList runblocks;
    
    /** A list of all of the blocks that have yet to be run
        by the parser */
    QStringList pendingblocks;
    
    /** Part of an incomplete block (a block may be added bit 
        by bit */
    QString incompleteblock;
    
    /** The block that is currently being parsed */
    QString currentblock;
    
    /** A lock to protect access to this script */
    QReadWriteLock lock;

    /** The status of the script */
    ScriptStatus stat;
    
    /** The current Python error (holds an empty error if there
        is no current error) */
    PythonError err;
};

}

SIRE_END_HEADER

#endif
