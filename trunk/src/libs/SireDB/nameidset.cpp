
#include "nameidset.h"

#include <QString>
#include <QStringList>

namespace SireDB
{

/** Convert the set to a string (space separated list of numbers) */
QString convertToString(const NameIDSet &set)
{
    if (set.isEmpty())
        return QString();
    else
    {
        QStringList parts;
      
        for (NameIDSet::const_iterator it = set.begin();
            it != set.end();
            ++it)
        {
            parts.append( QString::number( *it ) );
        }
        
        return parts.join(" ");
    }
}

/** Convert a string to a set (need to pass space separated list of numbers) */
NameIDSet convertToNameIDSet(const QString &string)
{
    QStringList parts = string.split(" ", QString::SkipEmptyParts);
    
    NameIDSet set;
    
    foreach ( QString part, parts )
        set.insert( part.toUInt() );
        
    return set;
}

}
