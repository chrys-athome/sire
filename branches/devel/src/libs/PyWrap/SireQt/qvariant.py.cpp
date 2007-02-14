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

#include <boost/python.hpp>

#include <QVariant>
#include <QDataStream>
#include <QByteArray>
#include <QBitArray>
#include <QString>
#include <QStringList>
#include <QDate>
#include <QTime>
#include <QDateTime>
#include <QList>
#include <QMap>
#include <QSize>
#include <QSizeF>
#include <QPoint>
#include <QPointF>
#include <QLine>
#include <QLineF>
#include <QRect>
#include <QRectF>
#include <QUrl>
#include <QLocale>
#include <QRegExp>

#include "sireglobal.h"

using namespace boost::python;

void SIREQT_EXPORT export_QVariant()
{
    class_<QVariant>( "QVariant", init<>() )

        .def( init<const QVariant&>() )
        .def( init<QDataStream&>() )
        .def( init<int>() )
        .def( init<uint>() )
        .def( init<qlonglong>() )
        .def( init<qulonglong>() )
        .def( init<bool>() )
        .def( init<double>() )
        .def( init<const QByteArray&>() )
        .def( init<const QBitArray&>() )
        .def( init<const QString&>() )
        .def( init<const QStringList&>() )
        .def( init<const QDate&>() )
        .def( init<const QTime&>() )
        .def( init<const QDateTime&>() )
        .def( init<const QList<QVariant>&>() )
        .def( init<const QMap<QString,QVariant>&>() )
        .def( init<const QSize&>() )
        .def( init<const QSizeF&>() )
        .def( init<const QPoint&>() )
        .def( init<const QPointF&>() )
        .def( init<const QLine&>() )
        .def( init<const QLineF&>() )
        .def( init<const QRect&>() )
        .def( init<const QRectF&>() )
        .def( init<const QUrl&>() )
        .def( init<const QLocale&>() )
        .def( init<const QRegExp&>() )

        .def( "clear", &QVariant::clear )
        .def( "isNull", &QVariant::isNull )
        .def( "isValid", &QVariant::isValid )
        .def( "toBitArray", &QVariant::toBitArray )
        .def( "toBool", &QVariant::toBool )
        .def( "toByteArray", &QVariant::toByteArray )
        .def( "toChar", &QVariant::toChar )
        .def( "toDate", &QVariant::toDate )
        .def( "toDateTime", &QVariant::toDateTime )
        .def( "toDouble", &QVariant::toDouble )
        .def( "toInt", &QVariant::toInt )
        .def( "toLine", &QVariant::toLine )
        .def( "toLineF", &QVariant::toLineF )
        .def( "toList", &QVariant::toList )
        .def( "toLocale", &QVariant::toLocale )
        .def( "toLongLong", &QVariant::toLongLong )
        .def( "toMap", &QVariant::toMap )
        .def( "toPoint", &QVariant::toPoint )
        .def( "toPointF", &QVariant::toPointF )
        .def( "toRect", &QVariant::toRect )
        .def( "toRectF", &QVariant::toRectF )
        .def( "toRegExp", &QVariant::toRegExp )
        .def( "toSize", &QVariant::toSize )
        .def( "toSizeF", &QVariant::toSizeF )
        .def( "toString", &QVariant::toString )
        .def( "toStringList", &QVariant::toStringList )
        .def( "toTime", &QVariant::toTime )
        .def( "toUInt", &QVariant::toUInt )
        .def( "toULongLong", &QVariant::toULongLong )
        .def( "toUrl", &QVariant::toUrl )
        .def( "typeName", &QVariant::typeName )
        .def( "userType", &QVariant::userType )
        .def( self != self )
        .def( self == self )
    ;
}
