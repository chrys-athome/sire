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

#include <QVariant>
#include <QHash>
#include <QString>

#include "pluginfactory.h"

#include "Spier/spierview.h"

#include <QDebug>

/** Constructor */
PluginFactory::PluginFactory(QObject *parent) : QWebPluginFactory(parent)
{}

/** Destructor */
PluginFactory::~PluginFactory()
{}

template<class T>
static T getParameter(const QHash<QString,QVariant> &params,
                      const QString &param, const T &default_value)
{
    QVariant value = params.value(param, QVariant(default_value));

    if (not value.canConvert<T>())
        return default_value;
    else
        return value.value<T>();
}

/** Overrides the QWebPluginFactory function */
QObject* PluginFactory::create(const QString &mimeType, const QUrl &url, 
                               const QStringList &argumentNames, 
                               const QStringList &argumentValues) const
{
    QHash<QString,QVariant> params;
    
    for (int i=0; i<argumentNames.count(); ++i)
    {
        params[argumentNames[i]] = argumentValues[i];
    }

    if (mimeType == QLatin1String("application/x-sire_spierview"))
    {
        Spier::SpierView *spierview = new Spier::SpierView();
        
        spierview->setObjectName( getParameter<QString>(params, "name", "unnamed") );
        spierview->resize( getParameter<int>(params, "width", 100),
                           getParameter<int>(params, "height", 100) );
    
        return spierview;
    }
    else
    {
        qDebug() << "Plugins with mimetype" << mimeType << "are not supported...";
        return 0;
    }
}

/** Return the list of supported plugins */
QList<QWebPluginFactory::Plugin> PluginFactory::plugins() const
{
    return QList<QWebPluginFactory::Plugin>();
}
