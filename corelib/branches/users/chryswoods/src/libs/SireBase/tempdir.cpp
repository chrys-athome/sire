/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2008  Christopher Woods
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

#include <QUuid>
#include <QMutex>

#include "tempdir.h"

#include "SireError/errors.h"

#include <QDebug>

using namespace SireBase;

QString getUserName()
{
    #ifdef Q_OS_UNIX
        return std::getenv("USER");
    #else
        return "USER";
    #endif
}

static QMutex tmpdir_mutex;

/** Create a new temporary directory in QDir::tempPath() */
void TempDir::createDirectory(const QString &temp_root)
{
    QString dirname = QString("%1/SIRE_%2_%3")
                            .arg(temp_root, getUserName(),
                                 QUuid::createUuid().toString());
                                 
    QMutexLocker lkr(&tmpdir_mutex);
    
    if (tmpdir.exists(dirname))
    {
        //this directory already exists - try again
        lkr.unlock();
        this->createDirectory(temp_root);
    }
    
    qDebug() << "Creating" << dirname;
    
    if (not tmpdir.mkdir(dirname))
        throw SireError::file_error( QObject::tr(
            "Could not create the temporary directory \"%1\". Please "
            "ensure that there is enough disk space and that you have "
            "permission to write to the directory \"%2\".")
                .arg(dirname, temp_root), CODELOC );
                
    tmpdir = QDir(dirname);
}

/** This creates a new temporary directory in QDir::tempPath() */
TempDir::TempDir()
{
    this->createDirectory( QDir::tempPath() );
}

/** This creates a new temporary directory in 'temp_root' */
TempDir::TempDir(const QString &temp_root)
{
    this->createDirectory(temp_root);
}

static void removeDirectory(QDir &dir)
{
    QDir remove_dir(dir.absolutePath());

    qDebug() << remove_dir.absolutePath() << remove_dir.isReadable() << remove_dir.isAbsolute();

    //get the list of all files in this directory
    //(except . and ..)
    QFileInfoList files = remove_dir.entryInfoList( QDir::NoDotAndDotDot );
    
    qDebug() << files.count();
    
    foreach (const QFileInfo &file, files)
    {
        qDebug() << file.absoluteFilePath();
    
        if (file.isDir())
        {
            //remove this directory
            QDir subdir( file.absoluteFilePath() );

            qDebug() << "Recursively removing" << file.absoluteFilePath();
            ::removeDirectory(subdir);
        }
        else
        {
            //remove this file
            QFile f(file.absoluteFilePath());
            
            qDebug() << "Removing file" << file.absoluteFilePath();
            f.remove();
        }
    }
    
    //now remove this directory
    qDebug() << "Directory empty - removing" << dir.absolutePath();
    remove_dir.rmdir(remove_dir.absolutePath());
}

/** The destructor deletes the temporary directory and
    anything that it contains */
TempDir::~TempDir()
{
    ::removeDirectory(tmpdir);
}

/** This returns the complete path to the temporary directory */
QString TempDir::path() const
{
    return tmpdir.absolutePath();
}

/** Return a string representation of this TempDir */
QString TempDir::toString() const
{
    return QString("TempDir(\"%1\")").arg(this->path());
}
