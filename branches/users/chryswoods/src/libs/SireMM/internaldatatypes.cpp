
#include "qhash_siremm.h"
#include "qhash_siremol.h"

#include <QDataStream>

//needed so that qHash(QString) is found
#include <QHash>
#include <QString>

#include "internaldatatypes.h"

using namespace SireMM;

QDataStream& operator<<(QDataStream &ds, const BondResID &id)
{
    ds << id.resNum0() << id.resNum1();
    return ds;
}

QDataStream& operator>>(QDataStream &ds, BondResID &id)
{
    ds >> id.id[0] >> id.id[1];
    return ds;
}

uint SIREMM_EXPORT qHash(const BondResID &id)
{
    //return the lowest 16 bits of each type, as
    //    33222222222211111111110000000000
    //    10987654321098765432109876543210
    //    00000000000000001111111111111111  (0 = ResNum0 bit, 1 = ResNum1 bit)
    return (qHash(id.resNum0()) << 16) | (qHash(id.resNum1()) & 0x0000FFFF);
}

QDataStream& operator<<(QDataStream &ds, const BondAtomID &id)
{
    ds << id.atom0() << id.atom1();
    return ds;
}

QDataStream& operator>>(QDataStream &ds, BondAtomID &id)
{
    ds >> id.atms[0] >> id.atms[1];
    return ds;
}

uint SIREMM_EXPORT qHash(const BondAtomID &id)
{
    //return the lowest 16 bits of the has of each type, as
    //    33222222222211111111110000000000
    //    10987654321098765432109876543210
    //    00000000000000001111111111111111  (0 = qHash(atom0) bit, 1 = qHash(atom1) bit)
    return ( qHash(id.atom0()) << 16 ) | ( qHash(id.atom1()) & 0x0000FFFF );
}

QDataStream& operator<<(QDataStream &ds, const AngleResID &id)
{
    ds << id.resNum0() << id.resNum1() << id.resNum2();
    return ds;
}

QDataStream& operator>>(QDataStream &ds, AngleResID &id)
{
    ds >> id.id[0] >> id.id[1] >> id.id[2];
    return ds;
}

uint SIREMM_EXPORT qHash(const AngleResID &id)
{
    //return the lowest 12 bits of res0, and 10 bits of the other residues, as
    //    33222222222211111111110000000000
    //    10987654321098765432109876543210
    //    00000000000011111111112222222222  (0 = ResNum0, 1 = ResNum1, 2 = ResNum2)
    return (qHash(id.resNum0()) << 20) | 
            ((qHash(id.resNum1()) << 10) & 0x000FFC00) |
              (qHash(id.resNum2()) & 0x000003FF);
}

QDataStream& operator<<(QDataStream &ds, const AngleAtomID &id)
{
    ds << id.atom0() << id.atom1() << id.atom2();
    return ds;
}

QDataStream& operator>>(QDataStream &ds, AngleAtomID &id)
{
    ds >> id.atms[0] >> id.atms[1] >> id.atms[2];
    return ds;
}

uint SIREMM_EXPORT qHash(const AngleAtomID &id)
{
    //return the lowest 12 bits of res0, and 10 bits of the other residues, as
    //    33222222222211111111110000000000
    //    10987654321098765432109876543210
    //    00000000000011111111112222222222  (0 = atm0, 1 = atm1, 2 = atm2)
    return (qHash(id.atom0()) << 20) | ((qHash(id.atom1()) << 10) & 0x000FFC00) |
                    (qHash(id.atom2()) & 0x000003FF);
}

QDataStream& operator<<(QDataStream &ds, const DihedralResID &id)
{
    ds << id.resNum0() << id.resNum1() << id.resNum2() << id.resNum3();
    return ds;
}

QDataStream& operator>>(QDataStream &ds, DihedralResID &id)
{
    ds >> id.id[0] >> id.id[1] >> id.id[2] >> id.id[3];
    return ds;
}

uint SIREMM_EXPORT qHash(const DihedralResID &id)
{
    //return the lowest 8 bits of each res
    //    33222222222211111111110000000000
    //    10987654321098765432109876543210
    //    00000000111111112222222233333333  (0 = ResNum0, 1 = ResNum1, 
    //                                       2 = ResNum2, 3 = ResNum3)
    return (qHash(id.resNum0()) << 24) | 
              ((qHash(id.resNum1()) << 16) & 0x00FF0000) |
                ((qHash(id.resNum2()) << 8) & 0x0000FF00) | 
                    (qHash(id.resNum3()) & 0x000000FF);
}

QDataStream& operator<<(QDataStream &ds, const DihedralAtomID &id)
{
    ds << id.atom0() << id.atom1() << id.atom2() << id.atom3();
    return ds;
}

QDataStream& operator>>(QDataStream &ds, DihedralAtomID &id)
{
    ds >> id.atms[0] >> id.atms[1] >> id.atms[2] >> id.atms[3];
    return ds;
}

uint SIREMM_EXPORT qHash(const DihedralAtomID &id)
{
    //return the lowest 12 bits of res0, and 10 bits of the other residues, as
    //    33222222222211111111110000000000
    //    10987654321098765432109876543210
    //    00000000000011111111112222222222  (0 = atm0, 1 = atm1, 2 = atm2)
    return (qHash(id.atom0()) << 24) | ((qHash(id.atom1()) << 16) & 0x00FF0000) |
           ((qHash(id.atom2()) << 8) & 0x0000FF00) | (qHash(id.atom3()) & 0x000000FF);
}
