#ifndef SIRECAS_COMPLEXVALUES_H
#define SIRECAS_COMPLEXVALUES_H

#include <QHash>

#include "symbolcomplex.h"
#include "values.h"
#include "symbols.h"

#include "sireglobal.h"

SIRE_BEGIN_HEADER

namespace SireCAS
{
class ComplexValues;
}

class QDataStream;
QDataStream& operator<<(QDataStream&, const SireCAS::ComplexValues&);
QDataStream& operator>>(QDataStream&, SireCAS::ComplexValues&);

namespace SireCAS
{

class Symbol;

/**
This class holds a set of Symbols and their associated complex values. This is used when numerically evaluating an equation using complex maths.

@author Christopher Woods
*/
class SIRECAS_EXPORT ComplexValues
{

friend QDataStream& ::operator<<(QDataStream&, const ComplexValues&);
friend QDataStream& ::operator>>(QDataStream&, ComplexValues&);

public:
    ComplexValues();
    ComplexValues(const SymbolComplex &symval0);
    ComplexValues(const SymbolComplex &symval0, const SymbolComplex &symval1);
    ComplexValues(const SymbolComplex &symval0, const SymbolComplex &symval1, 
                  const SymbolComplex &symval2);
    ComplexValues(const SymbolComplex &symval0, const SymbolComplex &symval1, 
                  const SymbolComplex &symval2, const SymbolComplex &symval3);
    ComplexValues(const SymbolComplex &symval0, const SymbolComplex &symval1, 
                  const SymbolComplex &symval2, const SymbolComplex &symval3, 
                  const SymbolComplex &symval4);
    ComplexValues(const SymbolComplex &symval0, const SymbolComplex &symval1, 
                  const SymbolComplex &symval2, const SymbolComplex &symval3, 
                  const SymbolComplex &symval4, const SymbolComplex &symval5);
    ComplexValues(const SymbolComplex &symval0, const SymbolComplex &symval1, 
                  const SymbolComplex &symval2, const SymbolComplex &symval3, 
                  const SymbolComplex &symval4, const SymbolComplex &symval5,
                  const SymbolComplex &symval6);
    ComplexValues(const SymbolComplex &symval0, const SymbolComplex &symval1, 
                  const SymbolComplex &symval2, const SymbolComplex &symval3, 
                  const SymbolComplex &symval4, const SymbolComplex &symval5,
                  const SymbolComplex &symval6, const SymbolComplex &symval7);
    ComplexValues(const SymbolComplex &symval0, const SymbolComplex &symval1, 
                  const SymbolComplex &symval2, const SymbolComplex &symval3, 
                  const SymbolComplex &symval4, const SymbolComplex &symval5,
                  const SymbolComplex &symval6, const SymbolComplex &symval7, 
                  const SymbolComplex &symval8);
    ComplexValues(const SymbolComplex &symval0, const SymbolComplex &symval1, 
                  const SymbolComplex &symval2, const SymbolComplex &symval3, 
                  const SymbolComplex &symval4, const SymbolComplex &symval5,
                  const SymbolComplex &symval6, const SymbolComplex &symval7, 
                  const SymbolComplex &symval8, const SymbolComplex &symval9);
    
    ComplexValues(const Values &other);
    
    ComplexValues(const ComplexValues &other);

    ~ComplexValues();

    void set(const Symbol &symbol, const Complex &value);

    void add(const SymbolComplex &symval0);
    void add(const SymbolComplex &symval0, const SymbolComplex &symval1);
    void add(const SymbolComplex &symval0, const SymbolComplex &symval1, 
             const SymbolComplex &symval2);
    void add(const SymbolComplex &symval0, const SymbolComplex &symval1, 
             const SymbolComplex &symval2, const SymbolComplex &symval3);
    void add(const SymbolComplex &symval0, const SymbolComplex &symval1, 
             const SymbolComplex &symval2, const SymbolComplex &symval3, 
             const SymbolComplex &symval4);
    void add(const SymbolComplex &symval0, const SymbolComplex &symval1, 
             const SymbolComplex &symval2, const SymbolComplex &symval3, 
             const SymbolComplex &symval4, const SymbolComplex &symval5);
    void add(const SymbolComplex &symval0, const SymbolComplex &symval1, 
             const SymbolComplex &symval2, const SymbolComplex &symval3, 
             const SymbolComplex &symval4, const SymbolComplex &symval5,
             const SymbolComplex &symval6);
    void add(const SymbolComplex &symval0, const SymbolComplex &symval1, 
             const SymbolComplex &symval2, const SymbolComplex &symval3, 
             const SymbolComplex &symval4, const SymbolComplex &symval5,
             const SymbolComplex &symval6, const SymbolComplex &symval7);
    void add(const SymbolComplex &symval0, const SymbolComplex &symval1, 
             const SymbolComplex &symval2, const SymbolComplex &symval3, 
             const SymbolComplex &symval4, const SymbolComplex &symval5,
             const SymbolComplex &symval6, const SymbolComplex &symval7, 
             const SymbolComplex &symval8);
    void add(const SymbolComplex &symval0, const SymbolComplex &symval1, 
             const SymbolComplex &symval2, const SymbolComplex &symval3, 
             const SymbolComplex &symval4, const SymbolComplex &symval5,
             const SymbolComplex &symval6, const SymbolComplex &symval7, 
             const SymbolComplex &symval8, const SymbolComplex &symval9);

    Complex value(const Symbol &sym) const;

    const QHash<SymbolID,Complex>& values() const;

private:

    /** Hash mapping Symbol IDs to actual numerical values */
    QHash<SymbolID, Complex> vals;

};

/** Add a SymbolComplex to the set of values */
inline void ComplexValues::add(const SymbolComplex &val0)
{
    vals.insert(val0.ID(), val0.value());
}

/** Set the Symbol 'symbol' equal to 'value' */
inline void ComplexValues::set(const Symbol &symbol, const Complex &value)
{
    vals.insert(symbol.ID(), value);
}

/** Return the hash mapping Symbol IDs to complex values */
inline const QHash<SymbolID,Complex>& ComplexValues::values() const
{
    return vals;
}

}

Q_DECLARE_METATYPE(SireCAS::ComplexValues)

SIRE_END_HEADER

#endif
