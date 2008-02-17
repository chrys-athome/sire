
#include "SireVol/cartesian.h"
#include "SireVol/coordgroup.h"
#include "SireMaths/vector.h"

#include <QVector>
#include <QTime>

#include <iostream>

using namespace SireVol;
using namespace SireMaths;

using namespace std;

int main(int argc, const char **argv)
{
    QTime t;

    QVector< QVector<CoordGroup> > group0;
    QVector< QVector<CoordGroup> > group1;

    QVector<Vector> coords(4);

    coords[0] = Vector(1.0, 0.0, 0.0);
    coords[1] = Vector(1.0, 1.0, 0.0);
    coords[2] = Vector(1.0, 0.0, 1.0);
    coords[3] = Vector(1.0, 1.0, 1.0);

    Space space;
    space = Cartesian();

    DistMatrix distmat;

    t.start();

    for (int i=0; i<1000; ++i)
    {
	for (int j=0; j<4; ++j)
        {
            coords[j] += Vector(2,0,0);
        }

        CoordGroup a(coords);

        CoordGroup b = a.edit().translate( Vector(0,2,0) );

        QVector<CoordGroup> agroup(1, a);
        QVector<CoordGroup> bgroup(1, b);

        group0.append(agroup);
        group1.append(bgroup);
    }

    int ms = t.elapsed();
    cout << "Building the molecules took " << ms << " ms\n";

    const QVector<CoordGroup> *group0_array = group0.constData();
    const QVector<CoordGroup> *group1_array = group1.constData();

    //calculate the minimum distance...
    double mindist = numeric_limits<double>::max();

    t.start();

    for (int imol=0; imol<group0.count(); ++imol)
    {
       const QVector<CoordGroup> &mol0 = group0_array[imol];

       int ncg0 = mol0.count();
       const CoordGroup *mol0_array = mol0.constData();

       for (int jmol=0; jmol<group1.count(); ++jmol)
       {
           const QVector<CoordGroup> &mol1 = group1_array[jmol];

           int ncg1 = mol1.count();
           const CoordGroup *mol1_array = mol1.constData();
           
           for (int igroup = 0; igroup<ncg0; ++igroup)
           {
               const CoordGroup &group0 = mol0_array[igroup];

               for (int jgroup = 0; jgroup<ncg1; ++jgroup)
               {
                   const CoordGroup &group1 = mol1_array[jgroup];

                   double this_mindist = space->calcDist(group0, group1, distmat);

                   if (this_mindist < mindist)
                       mindist = this_mindist;
               }
           }
       }
    }

    ms = t.elapsed();

    cout << "Minimum distance = " << mindist << " (took " << ms << " ms)\n";

    return 0;
}
