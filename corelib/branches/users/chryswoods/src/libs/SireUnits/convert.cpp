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

#include <QString>
#include <QStringList>

#include "convert.h"
#include "dimensions.h"

namespace SireUnits
{

namespace Dimension
{

static void appendString(int M, QString rep, QStringList &pos, QStringList &neg)
{
    if (M > 1)
    {
        pos.append( QString("%1^%2").arg(rep).arg(M) );
    }
    else if (M == 1)
    {
        pos.append(rep);
    }
    else if (M < 0)
    {
        neg.append( QString("%1%2").arg(rep).arg(M) );
    }
} 

/** Return a string representing the unit with specified dimensions */
QString SIREUNITS_EXPORT getUnitString(int M, int L, int T, int C, int t, int Q, int A)
{
    QStringList pos;
    QStringList neg;
    
    appendString(M, "M", pos, neg);
    appendString(L, "L", pos, neg);
    appendString(T, "T", pos, neg);
    appendString(C, "C", pos, neg);
    appendString(t, "t", pos, neg);
    appendString(Q, "Q", pos, neg);
    appendString(A, "A", pos, neg);
    
    if (pos.isEmpty() and neg.isEmpty())
        return "";
    else if (neg.isEmpty())
        return pos.join(" ");
    else if (pos.isEmpty())
        return neg.join(" ");
    else
        return QString("%1 %2").arg(pos.join(" "), neg.join(" "));
}

}

}

