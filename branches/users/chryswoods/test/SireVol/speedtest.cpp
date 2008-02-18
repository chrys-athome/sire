
#include "SireVol/cartesian.h"
#include "SireVol/coordgroup.h"
#include "SireVol/coordgroup2.h"
#include "SireMaths/vector.h"

#include <QVector>
#include <QTime>

#include <iostream>

using namespace SireVol;
using namespace SireMaths;

using namespace std;

void checkDist(int i, int j, double a, double b)
{
    if (a != b)
	cout << "ERROR: " << i << " " << j << " " << a << " " << b << endl;
}

int main(int argc, const char **argv)
{
    QTime t;

    QVector<Vector> c(4);

    c[0] = Vector(1,0,0);
    c[1] = Vector(2,0,0);
    c[2] = Vector(0,0,1);
    c[3] = Vector(0,0,2);

    CoordGroup2 test(c);

    cout << test.count() << endl;
    cout << qPrintable( test[0].toString() ) << endl;
    cout << qPrintable( test[1].toString() ) << endl;
    cout << qPrintable( test[2].toString() ) << endl;
    cout << qPrintable( test[3].toString() ) << endl;
    cout << qPrintable( test.aaBox().center().toString() ) << endl;

    cerr << "*** TRANSLATE ***\n";
    CoordGroup2 test2 = test.edit().translate( Vector(1,0,0) ).commit();
    cerr << "*** TRANSLATE COMPLETE ***\n";

    cout << "TEST2\n";

    cout << test2.count() << endl;
    cout << qPrintable( test2[0].toString() ) << endl;
    cout << qPrintable( test2[1].toString() ) << endl;
    cout << qPrintable( test2[2].toString() ) << endl;
    cout << qPrintable( test2[3].toString() ) << endl;
    cout << qPrintable( test2.aaBox().center().toString() ) << endl;

    cout << "TEST\n";

    cout << test.count() << endl;
    cout << qPrintable( test[0].toString() ) << endl;
    cout << qPrintable( test[1].toString() ) << endl;
    cout << qPrintable( test[2].toString() ) << endl;
    cout << qPrintable( test[3].toString() ) << endl;
    cout << qPrintable( test.aaBox().center().toString() ) << endl;

    QVector< QVector<CoordGroup> > group0;
    QVector< QVector<CoordGroup> > group1;

    QVector< QVector<CoordGroup2> > group20;
    QVector< QVector<CoordGroup2> > group21;

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

        CoordGroup2 a2(coords);
        CoordGroup2 b2 = a2.edit().translate( Vector(0,2,0) ).commit();

        QVector<CoordGroup2> a2group(1, a2);
        QVector<CoordGroup2> b2group(1, b2);

        group20.append(a2group);
        group21.append(b2group);
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

    const QVector<CoordGroup2> *group20_array = group20.constData();
    const QVector<CoordGroup2> *group21_array = group21.constData();

    //calculate the minimum distance...
    mindist = numeric_limits<double>::max();

    t.start();

    for (int imol=0; imol<group20.count(); ++imol)
    {
       const QVector<CoordGroup2> &mol0 = group20_array[imol];

       int ncg0 = mol0.count();
       const CoordGroup2 *mol0_array = mol0.constData();

       for (int jmol=0; jmol<group21.count(); ++jmol)
       {
           const QVector<CoordGroup2> &mol1 = group21_array[jmol];

           int ncg1 = mol1.count();
           const CoordGroup2 *mol1_array = mol1.constData();
           
           for (int igroup = 0; igroup<ncg0; ++igroup)
           {
               const CoordGroup2 &group0 = mol0_array[igroup];

               for (int jgroup = 0; jgroup<ncg1; ++jgroup)
               {
                   const CoordGroup2 &group1 = mol1_array[jgroup];

                   double this_mindist = space->calcDist(group0, group1, distmat);

                   if (this_mindist < mindist)
                       mindist = this_mindist;
               }
           }
       }
    }

    ms = t.elapsed();

    cout << "Minimum distance = " << mindist << " (took " << ms << " ms)\n";

    //make sure that the calculated distances are the same
    t.start();

    DistMatrix distmat1;
    DistMatrix distmat2;

    for (int imol=0; imol<group0.count(); ++imol)
    {
       const QVector<CoordGroup> &mol10 = group0_array[imol];
       const QVector<CoordGroup2> &mol20 = group20_array[imol];

       int ncg0 = mol10.count();
       const CoordGroup *mol10_array = mol10.constData();
       const CoordGroup2 *mol20_array = mol20.constData();

       for (int jmol=0; jmol<group1.count(); ++jmol)
       {
           const QVector<CoordGroup> &mol11 = group1_array[jmol];
           const QVector<CoordGroup2> &mol21 = group21_array[jmol];

           int ncg1 = mol11.count();

           const CoordGroup *mol11_array = mol11.constData();
           const CoordGroup2 *mol21_array = mol21.constData();
           
           for (int igroup = 0; igroup<ncg0; ++igroup)
           {
               const CoordGroup &group10 = mol10_array[igroup];
               const CoordGroup2 &group20 = mol20_array[igroup];

               const CoordGroup &group11 = mol11_array[igroup];
               const CoordGroup2 &group21 = mol21_array[igroup];

               for (int jgroup = 0; jgroup<ncg1; ++jgroup)
               {
	           const CoordGroup &group11 = mol11_array[jgroup];
                   const CoordGroup2 &group21 = mol21_array[jgroup];

                   double this_mindist1 = space->calcDist(group10, group11, distmat1);
                   double this_mindist2 = space->calcDist(group20, group21, distmat2);

                   checkDist( -1, -1, this_mindist1, this_mindist2 );

                   for (int i=0; i<distmat1.nOuter(); ++i)
                   {
                       for (int j=0; j<distmat1.nInner(); ++j)
                       {
                           checkDist(i, j, distmat1.at(i,j), distmat2.at(i,j));
                       }
                   }
               }
           }
       }
    }

    ms = t.elapsed();

    cout << "Minimum distance = " << mindist << " (took " << ms << " ms)\n";

    return 0;
}
