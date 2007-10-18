/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007  Christopher Woods
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

#include "pythonscript.h"

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDebug>

#include "SireError/errors.h"

using namespace SirePy;

PythonScript::PythonScript() : stat(SCRIPT_EMPTY)
{}

PythonScript::~PythonScript()
{}

void PythonScript::addFile(const QString &filename)
{
    //open the file and read its contents into a QString
    QFile file(filename);

    if (not file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        throw SireError::file_error(file, CODELOC);
    }
    else
    {
        qDebug() << QObject::tr("Running file '%1'").arg(filename);
        //read the entire contents of the file
        QTextStream ts(&file);
        QString filecontents = ts.readAll();

        this->addText(filecontents);
    }
}

void PythonScript::addText(const QString &text)
{
    if (text.isEmpty())
      return;

    //split the text into lines...
    QStringList lines = text.split("\n",QString::KeepEmptyParts);

    QString line;
    foreach (line,lines)
    {
        if (incompleteblock.isEmpty())
        {
            //if this is a comment, then just straight add it onto the pending blocks
            //(or if it is just an empty line!)
            if (line.trimmed().startsWith("#") or line.simplified() == " " or line.isEmpty())
            {
                this->addPendingBlock(line);
            }
            else
            {
                //there is no current block - this line must either be a whole
                //block, or the start of a new block. Either way, it should start
                //on the first column
                line = line.trimmed();

                //if the line ends in a ':' then it is the start of a new block
                if (line.endsWith(":"))
                    incompleteblock = line;
                else
                    //this is a complete block in itself - append it to the
                    //list of idle blocks
                    this->addPendingBlock(line);
            }
        }
        else
        {
            //if this is a comment line then just add it onto the incomplete block
            if (line.trimmed().startsWith("#") or line.simplified() == " " or line.isEmpty())
            {
                incompleteblock += "\n" + line;
            }
            //if this line does not start on column 0 then it must be
            //a continuation of the previous block
            else if (line.isEmpty() or line.startsWith(" ") or line.startsWith("\t"))
            {
                incompleteblock += "\n" + line;
            }
            else
            {
                //remove excess whitespace at either end of the string
                line = line.trimmed();

                //the old block is now complete - add it to the pending block queue
                this->addPendingBlock(incompleteblock);
                incompleteblock = QString();

                //now see whether this is a whole block in itself
                if (line.endsWith(":"))
                    incompleteblock = line;
                else
                    this->addPendingBlock(line);
            }
        }
    }
}

void PythonScript::addPendingBlock(const QString &block)
{
    QWriteLocker lkr(&lock);
    
    //add the missing(?) newline from the end of each block
    if (block.isEmpty())
      pendingblocks << "\n";
    else if (block.endsWith("\n"))
      pendingblocks << block;
    else
      pendingblocks << block + "\n";

    stat = SCRIPT_INCOMPLETE;
}

ScriptStatus PythonScript::status()
{
    QReadLocker lkr(&lock);
    return stat;
}

QString PythonScript::nextBlock()
{
    QWriteLocker lkr(&lock);

    //return an empty string if there are no pending blocks
    //left or there was an error with the current block
    if (stat == SCRIPT_COMPLETE or stat == SCRIPT_ERROR)
        return QString();

    if (pendingblocks.isEmpty())
    {
        //pending blocks is empty despite stat being SCRIPT_PENDING - bug!
        qDebug() << QObject::tr("pendingblocks.isEmpty() despite SCRIPT_PENDING! BUG!");
        stat = SCRIPT_COMPLETE;
        return QString();
    }

    //save the current block to the run blocks list
    if (not currentblock.isEmpty())
        runblocks << currentblock;

    //save the top block as the current block
    currentblock = pendingblocks.first();

    //remove the top block from the pending blocks
    pendingblocks.pop_front();

    //now set the status if necessary
    if (pendingblocks.isEmpty())
        stat = SCRIPT_COMPLETE;

    //return the new currentblock
    return currentblock;
}

void PythonScript::setError(const PythonError &error)
{
    QWriteLocker lkr(&lock);
    qDebug() << QObject::tr("Setting a python error!");

    err = error;
}

PythonError PythonScript::error()
{
    QReadLocker lkr(&lock);
    return err;
}

void PythonScript::correctError(const QString &newblock)
{
    QWriteLocker lkr(&lock);

    qDebug() << QObject::tr("Correcting an error in\n%1\nwith\n%2")
                             .arg(currentblock,newblock);

    //clear the broken current block
    currentblock = QString();

    //add the new block to the top of the string queue
    pendingblocks.push_front(newblock);

    //reset the status and clear the error
    err = PythonError();

    stat = SCRIPT_INCOMPLETE;
}

void PythonScript::ignoreError()
{
    QWriteLocker lkr(&lock);

    //clear the error and current block
    err = PythonError();
    currentblock = QString();

    //reset the status of the script
    if (pendingblocks.isEmpty())
        stat = SCRIPT_COMPLETE;
    else
        stat = SCRIPT_INCOMPLETE;
}

void PythonScript::debugScript()
{
    QReadLocker lkr(&lock);

    //counters for the number of printed blocks and number of printed lines
    int nblock(0),nline(0);

    qDebug() << QObject::tr("Completed blocks-------------------");

    QString block;
    foreach (block,runblocks)
    {
        printDebugBlock(block,nblock,nline);
    }

    qDebug() << QObject::tr("Current block----------------------");
    printDebugBlock(currentblock,nblock,nline);

    qDebug() << QObject::tr("Pending blocks---------------------");
    foreach (block,pendingblocks)
    {
        printDebugBlock(block,nblock,nline);
    }
}

void PythonScript::printDebugBlock(const QString &block, int &i, int &linenum)
{
    QStringList lines = block.split("\n",QString::KeepEmptyParts);

    //this split will add one extra empty string on the end unnecessarily! 
    //we will have to remove it!
    lines.pop_back();
    
    //increment the number of blocks printed
    i++;

    bool firstline(true);

    QString line;
    int blockline(0);
    foreach(line,lines)
    {
        //increment the number of lines printed
        linenum++;
        blockline++;
        
        if (firstline)
        {
            qDebug() << QString("%1 - %2(%3): %4").arg(linenum,5).arg(i,5)
                                                  .arg(blockline,5).arg(line);
            firstline = false;
        }
        else
            qDebug() << QString("%1 -      (%2): %3").arg(linenum,5)
                                                .arg(blockline,5).arg(line);
    }
}

void PythonScript::setStatus(ScriptStatus s)
{
    QWriteLocker lkr(&lock);
    stat = s;
}
