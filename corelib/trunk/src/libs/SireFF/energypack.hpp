#ifndef SIREFF_ENERGYPACK_HPP
#define SIREFF_ENERGYPACK_HPP

#include "energy.hpp"

#include <boost/static_assert.hpp>

SIRE_BEGIN_HEADER

namespace SireFF
{
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
class EnergyPack;
}

template<SireFF::EnergyID T1, SireFF::EnergyID T2, SireFF::EnergyID T3,
         SireFF::EnergyID T4, SireFF::EnergyID T5, SireFF::EnergyID T6,
         SireFF::EnergyID T7, SireFF::EnergyID T8, SireFF::EnergyID T9>
QDataStream& operator<<(QDataStream&, const SireFF::EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>&);

template<SireFF::EnergyID T1, SireFF::EnergyID T2, SireFF::EnergyID T3,
         SireFF::EnergyID T4, SireFF::EnergyID T5, SireFF::EnergyID T6,
         SireFF::EnergyID T7, SireFF::EnergyID T8, SireFF::EnergyID T9>
QDataStream& operator>>(QDataStream&, SireFF::EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>&);

namespace SireFF
{

/** This is a null EnergyT type used as the empty default parameter to 
    fill out the unused parameters of the EnergyPack. If is populated
    with dummy functions for assignment and increment, so that it can
    behave like a normal EnergyT. As these functions are empty, they
    should be removed by the compiler at compile time, so should hopefully
    not cost anything at run time. This is one of those C++ cases where
    we have to add code to get things to compile, but it is code that
    should never be run... */
template<>
class EnergyT<0>
{
public:
    EnergyT()
    {}
    
    EnergyT(const EnergyT<0> &)
    {}
    
    ~EnergyT()
    {}
    
    QString toString() const
    {
        return QString::null;
    }
    
    EnergyT<0>& operator=(const EnergyT<0>&)
    {
        return *this;
    }
    
    EnergyT<0>& operator+=(const EnergyT<0>&)
    {
        return *this;
    }
    
    EnergyT<0>& operator-=(const EnergyT<0>&)
    {
        return *this;
    }
    
    EnergyT<0>& operator*=(const EnergyT<0>&)
    {
        return *this;
    }
    
    EnergyT<0>& operator*=(double)
    {
        return *this;
    }
    
    EnergyT<0>& operator/=(const EnergyT<0>&)
    {
        return *this;
    }
    
    EnergyT<0>& operator/=(double)
    {
        return *this;
    }
    
    EnergyT<0> operator+(const EnergyT<0>&) const
    {
        return EnergyT<0>();
    }
    
    EnergyT<0> operator-(const EnergyT<0>&) const
    {
        return EnergyT<0>();
    }
    
    double value() const
    {
        return 0.0;
    }
};

/** Dummy function for dummy EnergyT */
template<>
inline QString getName(const EnergyT<0>&)
{
    return QString::null;
}

/** This is a (I think clever) class that collects different energy components
    together into a single class, with template metaprogramming used to automatically
    combine energy packs together. */
template<EnergyID T1, 
         EnergyID T2,
         EnergyID T3 = 0,
         EnergyID T4 = 0,
         EnergyID T5 = 0,
         EnergyID T6 = 0,
         EnergyID T7 = 0,
         EnergyID T8 = 0,
         EnergyID T9 = 0>
class EnergyPack : public EnergyBase
{

friend QDataStream& ::operator<<<>(QDataStream&, const EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>&);
friend QDataStream& ::operator>><>(QDataStream&, EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>&);

public:
    EnergyPack();

    EnergyPack( EnergyT<T1> t1, 
                EnergyT<T2> t2 = EnergyT<T2>(),
                EnergyT<T3> t3 = EnergyT<T3>(),
                EnergyT<T4> t4 = EnergyT<T4>(),
                EnergyT<T5> t5 = EnergyT<T5>(),
                EnergyT<T6> t6 = EnergyT<T6>(),
                EnergyT<T7> t7 = EnergyT<T7>(),
                EnergyT<T8> t8 = EnergyT<T8>(),
                EnergyT<T9> t9 = EnergyT<T9>()
               );
    
    template<EnergyID U1, EnergyID U2, EnergyID U3, EnergyID U4, EnergyID U5,
             EnergyID U6, EnergyID U7, EnergyID U8, EnergyID U9>
    EnergyPack( const EnergyPack<U1,U2,U3,U4,U5,U6,U7,U8,U9> &other );
    
    EnergyPack( const EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> &other );
    
    virtual ~EnergyPack();
    
    QString toString() const;
    
    template<EnergyID U>
    const EnergyT<U>& get() const;
    
    template<EnergyID U>
    void set(const EnergyT<U> &other);
    
    void set(const EnergyT<0>&);
    
    void set(const EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> &other);
    
    template<EnergyID U1,EnergyID U2,EnergyID U3,
             EnergyID U4,EnergyID U5,EnergyID U6,
             EnergyID U7,EnergyID U8,EnergyID U9>
    void set(const EnergyPack<U1,U2,U3,U4,U5,U6,U7,U8,U9> &other);
    
    template<EnergyID U>
    EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>& operator=(const EnergyT<U> &other);
    
    EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>& operator=(
                      const EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> &other);
    
    template<EnergyID U1,EnergyID U2,EnergyID U3,
             EnergyID U4,EnergyID U5,EnergyID U6,
             EnergyID U7,EnergyID U8,EnergyID U9>
    EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>& operator=(
                      const EnergyPack<U1,U2,U3,U4,U5,U6,U7,U8,U9> &other);
    
    
    template<EnergyID U>
    void add(const EnergyT<U> &other);
    
    void add(const EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> &other);
    
    void add(const EnergyT<0>&);
    
    template<EnergyID U1,EnergyID U2,EnergyID U3,
             EnergyID U4,EnergyID U5,EnergyID U6,
             EnergyID U7,EnergyID U8,EnergyID U9>
    void add(const EnergyPack<U1,U2,U3,U4,U5,U6,U7,U8,U9> &other);
    
    template<EnergyID U>
    EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>& operator+=(const EnergyT<U> &other);
    
    EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>& operator+=(
                      const EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> &other);
    
    template<EnergyID U1,EnergyID U2,EnergyID U3,
             EnergyID U4,EnergyID U5,EnergyID U6,
             EnergyID U7,EnergyID U8,EnergyID U9>
    EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>& operator+=(
                      const EnergyPack<U1,U2,U3,U4,U5,U6,U7,U8,U9> &other);
    

    template<EnergyID U>
    void subtract(const EnergyT<U> &other);
    
    void subtract(const EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> &other);
    
    void subtract(const EnergyT<0>&);
    
    template<EnergyID U1,EnergyID U2,EnergyID U3,
             EnergyID U4,EnergyID U5,EnergyID U6,
             EnergyID U7,EnergyID U8,EnergyID U9>
    void subtract(const EnergyPack<U1,U2,U3,U4,U5,U6,U7,U8,U9> &other);
    
    template<EnergyID U>
    EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>& operator-=(const EnergyT<U> &other);
    
    EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>& operator-=(
                      const EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> &other);
    
    template<EnergyID U1,EnergyID U2,EnergyID U3,
             EnergyID U4,EnergyID U5,EnergyID U6,
             EnergyID U7,EnergyID U8,EnergyID U9>
    EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>& operator-=(
                      const EnergyPack<U1,U2,U3,U4,U5,U6,U7,U8,U9> &other);
    
    
    template<EnergyID U>
    void multiply(const EnergyT<U> &other);
    
    void multiply(const EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> &other);
    
    void multiply(const EnergyT<0>&);
    void multiply(double scl);
    
    template<EnergyID U1,EnergyID U2,EnergyID U3,
             EnergyID U4,EnergyID U5,EnergyID U6,
             EnergyID U7,EnergyID U8,EnergyID U9>
    void multiply(const EnergyPack<U1,U2,U3,U4,U5,U6,U7,U8,U9> &other);
    
    template<EnergyID U>
    EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>& operator*=(const EnergyT<U> &other);
    
    EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>& operator*=(
                      const EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> &other);
    
    template<EnergyID U1,EnergyID U2,EnergyID U3,
             EnergyID U4,EnergyID U5,EnergyID U6,
             EnergyID U7,EnergyID U8,EnergyID U9>
    EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>& operator*=(
                      const EnergyPack<U1,U2,U3,U4,U5,U6,U7,U8,U9> &other);
    
    EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>& operator*=(double scl);    
    
    template<EnergyID U>
    void divide(const EnergyT<U> &other);
    
    void divide(const EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> &other);
    
    void divide(const EnergyT<0>&);
    void divide(double scl);
    
    template<EnergyID U1,EnergyID U2,EnergyID U3,
             EnergyID U4,EnergyID U5,EnergyID U6,
             EnergyID U7,EnergyID U8,EnergyID U9>
    void divide(const EnergyPack<U1,U2,U3,U4,U5,U6,U7,U8,U9> &other);
    
    template<EnergyID U>
    EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>& operator/=(const EnergyT<U> &other);
    
    EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>& operator/=(
                      const EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> &other);
    
    template<EnergyID U1,EnergyID U2,EnergyID U3,
             EnergyID U4,EnergyID U5,EnergyID U6,
             EnergyID U7,EnergyID U8,EnergyID U9>
    EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>& operator/=(
                      const EnergyPack<U1,U2,U3,U4,U5,U6,U7,U8,U9> &other);

    EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>& operator/=(double scl);

    EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> sqrt() const;
    EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> abs() const;
    
protected:
    template<EnergyID U>
    EnergyT<U>& getRef();

    template<EnergyID U>
    const EnergyT<U>& this_EnergyPack_has_no_such_component() const;

    template<EnergyID U>
    void subtractFromTotal(const EnergyT<U> &other);
    void subtractFromTotal(const EnergyT<0>&);
    void subtractFromTotal(const EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> &other);
    
    template<EnergyID U>
    void addToTotal(const EnergyT<U> &other);
    void addToTotal(const EnergyT<0>&);
    void addToTotal(const EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> &other);

    void recalculateTotal();

    /** The first energy component */
    EnergyT<T1> t1;
    /** The second energy component */
    EnergyT<T2> t2;
    /** The third energy component */
    EnergyT<T3> t3;
    /** The fourth energy component */
    EnergyT<T4> t4;
    /** The fifth energy component */
    EnergyT<T5> t5;
    /** The sixth energy component */
    EnergyT<T6> t6;
    /** The seventh energy component */
    EnergyT<T7> t7;
    /** The eighth energy component */
    EnergyT<T8> t8;
    /** The ninth energy component */
    EnergyT<T9> t9;
};

/** This is a dummy function that is used to check that the requested energy component
    is valid. If it is not, then the BOOST_STATIC_ASSERT in this function will return false,
    which will throw an error that includes the name of this function (which is named
    to give more info about the cause of the error */
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
template<EnergyID U>
SIRE_OUTOFLINE_TEMPLATE
const EnergyT<U>& EnergyPack<T1,T2,T3,T4,
                            T5,T6,T7,T8,T9>::this_EnergyPack_has_no_such_component() const
{
    BOOST_STATIC_ASSERT(U == T1 or U == T2 or U == T3 or U == T4 or U == T5 or
                        U == T6 or U == T7 or U == T8 or U == T9 );

    static const EnergyT<U> const_val;
    return const_val;
}

/** Overload the general operator with a specific operator for this EnergyPack type */
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
SIRE_INLINE_TEMPLATE
EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> operator+(
                            const EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> &nrg0, 
                            const EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> &nrg1)
{
    //take a copy of the first EnergyPack.
    EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> ret(nrg0);

    //perform the operation using the self-operator
    ret.add(nrg1);
    
    return ret;
}

/** Overload the general operator with a specific operator for this EnergyPack type */
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
SIRE_INLINE_TEMPLATE
EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> operator-(
                            const EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> &nrg0, 
                            const EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> &nrg1)
{
    //take a copy of the first EnergyPack.
    EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> ret(nrg0);

    //perform the operation using the self-operator
    ret.subtract(nrg1);
    
    return ret;
}

/** Overload the general operator with a specific operator for this EnergyPack type */
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
SIRE_INLINE_TEMPLATE
EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> operator*(
                            const EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> &nrg0, 
                            const EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> &nrg1)
{
    //take a copy of the first EnergyPack.
    EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> ret(nrg0);

    //perform the operation using the self-operator
    ret.multiply(nrg1);
    
    return ret;
}

/** Overload the general operator with a specific operator for this EnergyPack type */
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
SIRE_INLINE_TEMPLATE
EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> operator*(
                            const EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> &nrg, 
                            double scl)
{
    //take a copy of the first EnergyPack.
    EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> ret(nrg);
    
    //perform the operation using the self-operator
    ret.multiply(scl);
    
    return ret;
}

/** Overload the general operator with a specific operator for this EnergyPack type */
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
SIRE_INLINE_TEMPLATE
EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> operator*(
                            double scl,
                            const EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> &nrg)
{
    //take a copy of the first EnergyPack.
    EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> ret(nrg);
    
    //perform the operation using the self-operator
    ret.multiply(scl);
    
    return ret;
}

/** Overload the general operator with a specific operator for this EnergyPack type */
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
SIRE_INLINE_TEMPLATE
EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> operator/(
                            const EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> &nrg, 
                            double scl)
{
    //take a copy of the first EnergyPack.
    EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> ret(nrg);
    
    //perform the operation using the self-operator
    ret.divide(scl);
    
    return ret;
}

/** Overload the general operator with a specific operator for this EnergyPack type */
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
SIRE_INLINE_TEMPLATE
EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> operator/(
                            const EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> &nrg0, 
                            const EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> &nrg1)
{
    //take a copy of the first EnergyPack.
    EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> ret(nrg0);

    //perform the operation using the self-operator
    ret.divide(nrg1);
    
    return ret;
}

/** Recalculate the total energy of this EnergyPack. This shouldn't be too slow as
    it involves only additions within a self-contained object */
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
SIRE_INLINE_TEMPLATE
void EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>::recalculateTotal()
{
    nrg = t1.value() + t2.value() + t3.value() + t4.value() +
          t5.value() + t6.value() + t7.value() + t8.value() + t9.value();
}

/** Construct a zero EnergyPack, with all energy components being zero */
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
SIRE_OUTOFLINE_TEMPLATE
EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>::EnergyPack()
      : EnergyBase(), t1(), t2(), t3(), t4(), t5(), t6(), t7(), t8(), t9()
{}

/** Construct an energy pack from the passed components */
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
SIRE_OUTOFLINE_TEMPLATE
EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>::EnergyPack(
        EnergyT<T1> s1, EnergyT<T2> s2, EnergyT<T3> s3, EnergyT<T4> s4, EnergyT<T5> s5, 
        EnergyT<T6> s6, EnergyT<T7> s7, EnergyT<T8> s8, EnergyT<T9> s9 )
     : EnergyBase(), t1(s1), t2(s2), t3(s3), t4(s4), t5(s5), t6(s6), t7(s7), t8(s8), t9(s9)
{
    //calculate the total energy for this pack
    this->recalculateTotal();
}

/** Copy constructor */
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
SIRE_OUTOFLINE_TEMPLATE
EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>::EnergyPack(
                          const EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> &other)
     : EnergyBase(other), 
       t1(other.t1), t2(other.t2), t3(other.t3), t4(other.t4), t5(other.t5), 
       t6(other.t6), t7(other.t7), t8(other.t8), t9(other.t9)
{}

/** Copy construct this energy pack from another energy pack. Note that this pack must
    contain all of the components that are in 'other' */
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
template<EnergyID U1, EnergyID U2, EnergyID U3, EnergyID U4, EnergyID U5, 
         EnergyID U6, EnergyID U7, EnergyID U8, EnergyID U9>
SIRE_OUTOFLINE_TEMPLATE
EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>::EnergyPack(
                          const EnergyPack<U1,U2,U3,U4,U5,U6,U7,U8,U9> &other)
            : EnergyBase()
{
    this->set(other);
}

template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
SIRE_OUTOFLINE_TEMPLATE
EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>::~EnergyPack()
{}

/** Return the component 'U' from the energy pack, or throw an exception if such a component
    is not contained in this pack. This is a template function, with the resolution of the 
    type made at compile time. This is therefore very fast, and hopefully almost as fast
    as if the energy components were manually coded */
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
template<EnergyID U>
SIRE_INLINE_TEMPLATE
const EnergyT<U>& EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>::get() const
{
    /** Switch based on U. Since U and all of the T's are available at compile
        time I hope that this switch statement will be collapsed down by the compiler
        so that this function is very small and highly efficient 
        (we have to use if-else and not switch-case as a switch cannot have 
        duplicate values, e.g. T* = 0 for unused values)
    */
    if (U == T1)
        return reinterpret_cast<const EnergyT<U>&>(t1);
    else if (U == T2)
        return reinterpret_cast<const EnergyT<U>&>(t2);
    else if (U == T3)
        return reinterpret_cast<const EnergyT<U>&>(t3);
    else if (U == T4)
        return reinterpret_cast<const EnergyT<U>&>(t4);
    else if (U == T5)
        return reinterpret_cast<const EnergyT<U>&>(t5);
    else if (U == T6)
        return reinterpret_cast<const EnergyT<U>&>(t6);
    else if (U == T7)
        return reinterpret_cast<const EnergyT<U>&>(t7);
    else if (U == T8)
        return reinterpret_cast<const EnergyT<U>&>(t8);
    else if (U == T9)
        return reinterpret_cast<const EnergyT<U>&>(t9);
    else
        return this->this_EnergyPack_has_no_such_component<U>();
}  

/** Private, non-const version of EnergyPack::get() const, which is used internally by 
    this class to get a modifiable reference to the components of this EnergyPack.
    This cannot be a public function as the EnergyPack must be informed of all changes
    to components so that it can keep its own total up to date. */
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
template<EnergyID U>
SIRE_INLINE_TEMPLATE
EnergyT<U>& EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>::getRef()
{
    /** Switch based on U. Since U and all of the T's are available at compile
        time I hope that this switch statement will be collapsed down by the compiler
        so that this function is very small and highly efficient */
    if (U == T1)
        return reinterpret_cast<EnergyT<U>&>(t1);
    else if (U == T2)
        return reinterpret_cast<EnergyT<U>&>(t2);
    else if (U == T3)
        return reinterpret_cast<EnergyT<U>&>(t3);
    else if (U == T4)
        return reinterpret_cast<EnergyT<U>&>(t4);
    else if (U == T5)
        return reinterpret_cast<EnergyT<U>&>(t5);
    else if (U == T6)
        return reinterpret_cast<EnergyT<U>&>(t6);
    else if (U == T7)
        return reinterpret_cast<EnergyT<U>&>(t7);
    else if (U == T8)
        return reinterpret_cast<EnergyT<U>&>(t8);
    else if (U == T9)
        return reinterpret_cast<EnergyT<U>&>(t9);
    else
    {
        this->this_EnergyPack_has_no_such_component<U>();
        return reinterpret_cast<EnergyT<U>&>(t1);
    }
}  
    
/** Dummy set function used to ignore 'set' for dummy arguments */
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
SIRE_INLINE_TEMPLATE
void EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>::set(const EnergyT<0>&)
{}
  
/** Assignment operator/function. This assigns the component 'U' from the passed varible
    if that component is held in this pack. If it is not, then a compile time assertation will 
    fail, and you should see a compile error message that includes a line that
    says this_EnergyPack_has_no_such_component */
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
template<EnergyID U>
SIRE_INLINE_TEMPLATE
void EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>::set(const EnergyT<U> &other)
{
    //get the current value of this component
    EnergyT<U> &local = this->getRef<U>();
    
    //subtract the current value from the total
    nrg -= local.value();
    
    //set the local value
    local = other;
    
    //add the value back onto our total
    nrg += local.value();
}
       
/** Assignment operator/function. Assign all components in this EnergyPack from those in
    'other' */
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
SIRE_INLINE_TEMPLATE
void EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>::set(
                          const EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> &other)
{
    //copy the total energies
    nrg = other.nrg;
    
    //copy the components
    t1 = other.t1;
    t2 = other.t2;
    t3 = other.t3;
    t4 = other.t4;
    t5 = other.t5;
    t6 = other.t6;
    t7 = other.t7;
    t8 = other.t8;
    t9 = other.t9;
}

/** Assign the components of this EnergyPack from the components in 'other'. Note that all of the 
    components in other must be present in this EnergyPack or else a BOOST_STATIC_ASSERT will 
    cause a compile fail (see EnergyPack::get) */    
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
template<EnergyID U1,EnergyID U2,EnergyID U3,
         EnergyID U4,EnergyID U5,EnergyID U6,
         EnergyID U7,EnergyID U8,EnergyID U9>
SIRE_INLINE_TEMPLATE
void EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>::set(
                        const EnergyPack<U1,U2,U3,U4,U5,U6,U7,U8,U9> &other)
{
    //set each component from 'other' individually.
    this->set(other.get<U1>());
    this->set(other.get<U2>());
    this->set(other.get<U3>());
    this->set(other.get<U4>());
    this->set(other.get<U5>());
    this->set(other.get<U6>());
    this->set(other.get<U7>());
    this->set(other.get<U8>());
    this->set(other.get<U9>());
}
    
/** Assignment operator/function. This assigns the component 'U' from the passed varible
    if that component is held in this pack. If it is not, then a compile time assertation will 
    fail, and you should see a compile error message that includes a line that
    says this_EnergyPack_has_no_such_component */
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
template<EnergyID U>
SIRE_INLINE_TEMPLATE
EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>& EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>::operator=(
                  const EnergyT<U> &other)
{
    this->set(other);
    return *this;
}
    
/** Assignment operator/function. Assign all components in this EnergyPack from those in
    'other' */
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
SIRE_INLINE_TEMPLATE
EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>& EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>::operator=(
                  const EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> &other)
{
    this->set(other);
    return *this;
}
    
/** Assign the components of this EnergyPack from the components in 'other'. Note that all of the 
    components in other must be present in this EnergyPack or else a BOOST_STATIC_ASSERT will 
    cause a compile fail (see EnergyPack::get) */    
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
template<EnergyID U1,EnergyID U2,EnergyID U3,
         EnergyID U4,EnergyID U5,EnergyID U6,
         EnergyID U7,EnergyID U8,EnergyID U9>
SIRE_INLINE_TEMPLATE
EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>& EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>::operator=(
                   const EnergyPack<U1,U2,U3,U4,U5,U6,U7,U8,U9> &other)
{
    this->set(other);
    return *this;
}

/** Dummy add function used to ignore 'add' for dummy arguments */
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
SIRE_INLINE_TEMPLATE
void EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>::add(const EnergyT<0>&)
{}
  
/** Self-addition operator/function. This adds the component 'U' from the passed varible
    if that component is held in this pack. If it is not, then a compile time assertation will 
    fail, and you should see a compile error message that includes a line that
    says this_EnergyPack_has_no_such_component */
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
template<EnergyID U>
SIRE_INLINE_TEMPLATE
void EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>::add(const EnergyT<U> &other)
{
    //get the current value of this component
    EnergyT<U> &local = this->getRef<U>();
    
    //add to the local value
    local += other;
    
    //add the value onto our total
    nrg += other.value();
}
       
/** Self addition operator/function. Add all of the components of 'other' onto this EnergyPack. */
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
SIRE_INLINE_TEMPLATE
void EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>::add(
                            const EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> &other)
{
    //add the components
    t1 += other.t1;
    t2 += other.t2;
    t3 += other.t3;
    t4 += other.t4;
    t5 += other.t5;
    t6 += other.t6;
    t7 += other.t7;
    t8 += other.t8;
    t9 += other.t9;

    //add to the total
    nrg += other.value();
}

/** Self-addition operator/function of the components in 'other'. Note that all of the 
    components in other must be present in this EnergyPack or else a BOOST_STATIC_ASSERT will 
    cause a compile fail (see EnergyPack::get) */    
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
template<EnergyID U1,EnergyID U2,EnergyID U3,
         EnergyID U4,EnergyID U5,EnergyID U6,
         EnergyID U7,EnergyID U8,EnergyID U9>
SIRE_INLINE_TEMPLATE
void EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>::add(
                            const EnergyPack<U1,U2,U3,U4,U5,U6,U7,U8,U9> &other)
{
    //add each component from 'other' individually.
    this->add(other.get<U1>());
    this->add(other.get<U2>());
    this->add(other.get<U3>());
    this->add(other.get<U4>());
    this->add(other.get<U5>());
    this->add(other.get<U6>());
    this->add(other.get<U7>());
    this->add(other.get<U8>());
    this->add(other.get<U9>());
}
    
/** Self-addition operator/function. This adds the component 'U' from the passed varible
    if that component is held in this pack. If it is not, then a compile time assertation will 
    fail, and you should see a compile error message that includes a line that
    says this_EnergyPack_has_no_such_component */
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
template<EnergyID U>
SIRE_INLINE_TEMPLATE
EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>& EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>::operator+=(
                  const EnergyT<U> &other)
{
    this->add(other);
    return *this;
}
    
/** Self addition operator/function. Add all of the components of 'other' onto this EnergyPack. */
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
SIRE_INLINE_TEMPLATE
EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>& EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>::operator+=(
                  const EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> &other)
{
    this->add(other);
    return *this;
}
    
/** Self-addition operator/function of the components in 'other'. Note that all of the 
    components in other must be present in this EnergyPack or else a BOOST_STATIC_ASSERT will 
    cause a compile fail (see EnergyPack::get) */    
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
template<EnergyID U1,EnergyID U2,EnergyID U3,
         EnergyID U4,EnergyID U5,EnergyID U6,
         EnergyID U7,EnergyID U8,EnergyID U9>
SIRE_INLINE_TEMPLATE
EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>& EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>::operator+=(
                   const EnergyPack<U1,U2,U3,U4,U5,U6,U7,U8,U9> &other)
{
    this->add(other);
    return *this;
}

/** Dummy subtract function used to ignore 'subtract' for dummy arguments */
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
SIRE_INLINE_TEMPLATE
void EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>::subtract(const EnergyT<0>&)
{}
  
/** Self-subtraction operator/function. This subtracts the component 'U' from the passed varible
    if that component is held in this pack. If it is not, then a compile time assertation will 
    fail, and you should see a compile error message that includes a line that
    says this_EnergyPack_has_no_such_component */
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
template<EnergyID U>
SIRE_INLINE_TEMPLATE
void EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>::subtract(const EnergyT<U> &other)
{
    //get the current value of this component
    EnergyT<U> &local = this->getRef<U>();
    
    //subtract from the local value
    local -= other;
    
    //subtract the value from our total
    nrg -= other.value();
}
       
/** Self subtraction operator/function. Subtract all of the components of 'other' from this EnergyPack. */
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
SIRE_INLINE_TEMPLATE
void EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>::subtract(
                        const EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> &other)
{
    //subtract the components
    t1 -= other.t1;
    t2 -= other.t2;
    t3 -= other.t3;
    t4 -= other.t4;
    t5 -= other.t5;
    t6 -= other.t6;
    t7 -= other.t7;
    t8 -= other.t8;
    t9 -= other.t9;
    
    //subtract the value from our total
    nrg -= other.value();
}

/** Self-subtraction operator/function of the components in 'other'. Note that all of the 
    components in other must be present in this EnergyPack or else a BOOST_STATIC_ASSERT will 
    cause a compile fail (see EnergyPack::get) */    
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
template<EnergyID U1,EnergyID U2,EnergyID U3,
         EnergyID U4,EnergyID U5,EnergyID U6,
         EnergyID U7,EnergyID U8,EnergyID U9>
SIRE_INLINE_TEMPLATE
void EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>::subtract(
                              const EnergyPack<U1,U2,U3,U4,U5,U6,U7,U8,U9> &other)
{
    //subtract each component from 'other' individually.
    this->subtract(other.get<U1>());
    this->subtract(other.get<U2>());
    this->subtract(other.get<U3>());
    this->subtract(other.get<U4>());
    this->subtract(other.get<U5>());
    this->subtract(other.get<U6>());
    this->subtract(other.get<U7>());
    this->subtract(other.get<U8>());
    this->subtract(other.get<U9>());
}

/** Self-subtraction operator/function. This subtracts the component 'U' from the passed varible
    if that component is held in this pack. If it is not, then a compile time assertation will 
    fail, and you should see a compile error message that includes a line that
    says this_EnergyPack_has_no_such_component */
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
template<EnergyID U>
SIRE_INLINE_TEMPLATE
EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>& EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>::operator-=(
                  const EnergyT<U> &other)
{
    this->subtract(other);
    return *this;
}
    
/** Self subtraction operator/function. Subtract all of the components of 'other' from this EnergyPack. */
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
SIRE_INLINE_TEMPLATE
EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>& EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>::operator-=(
                  const EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> &other)
{
    this->subtract(other);
    return *this;
}
    
/** Self-subtraction operator/function of the components in 'other'. Note that all of the 
    components in other must be present in this EnergyPack or else a BOOST_STATIC_ASSERT will 
    cause a compile fail (see EnergyPack::get) */    
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
template<EnergyID U1,EnergyID U2,EnergyID U3,
         EnergyID U4,EnergyID U5,EnergyID U6,
         EnergyID U7,EnergyID U8,EnergyID U9>
SIRE_INLINE_TEMPLATE
EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>& EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>::operator-=(
                   const EnergyPack<U1,U2,U3,U4,U5,U6,U7,U8,U9> &other)
{
    this->subtract(other);
    return *this;
}

/** Dummy multiply function used to ignore 'multiply' for dummy arguments */
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
SIRE_INLINE_TEMPLATE
void EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>::multiply(const EnergyT<0>&)
{}
  
/** Self-multiplication operator/function. This multiplies the component 'U' from the passed varible
    if that component is held in this pack. If it is not, then a compile time assertation will 
    fail, and you should see a compile error message that includes a line that
    says this_EnergyPack_has_no_such_component */
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
template<EnergyID U>
SIRE_INLINE_TEMPLATE
void EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>::multiply(const EnergyT<U> &other)
{
    //get the current value of this component
    EnergyT<U> &local = this->getRef<U>();
    
    //multiply the local value
    local *= other;
    
    //recalculate the total of this pack
    this->recalculateTotal();
}
       
/** Self multiplication operator/function. Multiply all of the components 
    of 'other' onto this EnergyPack. */
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
SIRE_INLINE_TEMPLATE
void EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>::multiply(
                      const EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> &other)
{
    //multiply the components
    t1 *= other.t1;
    t2 *= other.t2;
    t3 *= other.t3;
    t4 *= other.t4;
    t5 *= other.t5;
    t6 *= other.t6;
    t7 *= other.t7;
    t8 *= other.t8;
    t9 *= other.t9;
    
    //recalculate the total
    this->recalculateTotal();
}

/** Self-multiplication operator/function of the components in 'other'. Note that all of the 
    components in other must be present in this EnergyPack or else a BOOST_STATIC_ASSERT will 
    cause a compile fail (see EnergyPack::get) */    
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
template<EnergyID U1,EnergyID U2,EnergyID U3,
         EnergyID U4,EnergyID U5,EnergyID U6,
         EnergyID U7,EnergyID U8,EnergyID U9>
SIRE_INLINE_TEMPLATE
void EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>::multiply(
                      const EnergyPack<U1,U2,U3,U4,U5,U6,U7,U8,U9> &other)
{
    //multiply each component from 'other' individually.
    this->multiply(other.get<U1>());
    this->multiply(other.get<U2>());
    this->multiply(other.get<U3>());
    this->multiply(other.get<U4>());
    this->multiply(other.get<U5>());
    this->multiply(other.get<U6>());
    this->multiply(other.get<U7>());
    this->multiply(other.get<U8>());
    this->multiply(other.get<U9>());
}

/** Self-multiplication operator/function. This multiplies all energy components
    in this pack by 'scl' */
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
SIRE_INLINE_TEMPLATE
void EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>::multiply(double scl)
{
    //multiply all components by 'scl'
    t1 *= scl;
    t2 *= scl;
    t3 *= scl;
    t4 *= scl;
    t5 *= scl;
    t6 *= scl;
    t7 *= scl;
    t8 *= scl;
    t9 *= scl;
    
    //multiply the total by 'scl'
    nrg *= scl;
}
           
/** Self-multiplication operator/function. This multiplies the component 'U' from the passed varible
    if that component is held in this pack. If it is not, then a compile time assertation will 
    fail, and you should see a compile error message that includes a line that
    says this_EnergyPack_has_no_such_component */
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
template<EnergyID U>
SIRE_INLINE_TEMPLATE
EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>& EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>::operator*=(
                  const EnergyT<U> &other)
{
    this->multiply(other);
    return *this;
}
    
/** Self multiplication operator/function. Multiply all of the components 
    of 'other' onto this EnergyPack. */
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
SIRE_INLINE_TEMPLATE
EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>& EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>::operator*=(
                  const EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> &other)
{
    this->multiply(other);
    return *this;
}
    
/** Self-multiplication operator/function of the components in 'other'. Note that all of the 
    components in other must be present in this EnergyPack or else a BOOST_STATIC_ASSERT will 
    cause a compile fail (see EnergyPack::get) */    
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
template<EnergyID U1,EnergyID U2,EnergyID U3,
         EnergyID U4,EnergyID U5,EnergyID U6,
         EnergyID U7,EnergyID U8,EnergyID U9>
SIRE_INLINE_TEMPLATE
EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>& EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>::operator*=(
                   const EnergyPack<U1,U2,U3,U4,U5,U6,U7,U8,U9> &other)
{
    this->multiply(other);
    return *this;
}

/** Self-multiplication operator/function. This multiplies all energy components
    in this pack by 'scl' */
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
SIRE_INLINE_TEMPLATE
EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>& EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>::operator*=(
                  double scl)
{
    this->multiply(scl);
    return *this;
}

/** Dummy divide function used to ignore 'divide' for dummy arguments */
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
SIRE_INLINE_TEMPLATE
void EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>::divide(const EnergyT<0>&)
{}
  
/** Self-division operator/function. This divides by the component 'U' from the passed varible
    if that component is held in this pack. If it is not, then a compile time assertation will 
    fail, and you should see a compile error message that includes a line that
    says this_EnergyPack_has_no_such_component */
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
template<EnergyID U>
SIRE_INLINE_TEMPLATE
void EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>::divide(const EnergyT<U> &other)
{
    //get the current value of this component
    EnergyT<U> &local = this->getRef<U>();
    
    //divide the local value
    local /= other;
    
    //recalculate the total
    this->recalculateTotal();
}
       
/** Self division operator/function. Divide by all of the components of 'other'. */
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
SIRE_INLINE_TEMPLATE
void EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>::divide(
                       const EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> &other)
{
    //divide the components
    t1 /= other.t1;
    t2 /= other.t2;
    t3 /= other.t3;
    t4 /= other.t4;
    t5 /= other.t5;
    t6 /= other.t6;
    t7 /= other.t7;
    t8 /= other.t8;
    t9 /= other.t9;
    
    //recalculate the total
    this->recalculateTotal();
}

/** Self-division operator/function of the components in 'other'. Note that all of the 
    components in other must be present in this EnergyPack or else a BOOST_STATIC_ASSERT will 
    cause a compile fail (see EnergyPack::get) */    
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
template<EnergyID U1,EnergyID U2,EnergyID U3,
         EnergyID U4,EnergyID U5,EnergyID U6,
         EnergyID U7,EnergyID U8,EnergyID U9>
SIRE_INLINE_TEMPLATE
void EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>::divide(
                      const EnergyPack<U1,U2,U3,U4,U5,U6,U7,U8,U9> &other)
{
    //divide each component by 'other' individually.
    this->divide(other.get<U1>());
    this->divide(other.get<U2>());
    this->divide(other.get<U3>());
    this->divide(other.get<U4>());
    this->divide(other.get<U5>());
    this->divide(other.get<U6>());
    this->divide(other.get<U7>());
    this->divide(other.get<U8>());
    this->divide(other.get<U9>());
}

/** Self-division operator/function. This divides all energy components
    in this pack by 'scl' */
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
SIRE_INLINE_TEMPLATE
void EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>::divide(double scl)
{
    scl = 1.0 / scl;
    
    //multiply all components by 'scl'
    t1 *= scl;
    t2 *= scl;
    t3 *= scl;
    t4 *= scl;
    t5 *= scl;
    t6 *= scl;
    t7 *= scl;
    t8 *= scl;
    t9 *= scl;
    
    //multiply the total by 'scl'
    nrg *= scl;
}
               
/** Self-division operator/function. This divides by the component 'U' from the passed varible
    if that component is held in this pack. If it is not, then a compile time assertation will 
    fail, and you should see a compile error message that includes a line that
    says this_EnergyPack_has_no_such_component */
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
template<EnergyID U>
SIRE_INLINE_TEMPLATE
EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>& EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>::operator/=(
                  const EnergyT<U> &other)
{
    this->divide(other);
    return *this;
}
    
/** Self division operator/function. Divide by all of the components of 'other'. */
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
SIRE_INLINE_TEMPLATE
EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>& EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>::operator/=(
                  const EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> &other)
{
    this->divide(other);
    return *this;
}
    
/** Self-division operator/function of the components in 'other'. Note that all of the 
    components in other must be present in this EnergyPack or else a BOOST_STATIC_ASSERT will 
    cause a compile fail (see EnergyPack::get) */    
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
template<EnergyID U1,EnergyID U2,EnergyID U3,
         EnergyID U4,EnergyID U5,EnergyID U6,
         EnergyID U7,EnergyID U8,EnergyID U9>
SIRE_INLINE_TEMPLATE
EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>& EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>::operator/=(
                   const EnergyPack<U1,U2,U3,U4,U5,U6,U7,U8,U9> &other)
{
    this->divide(other);
    return *this;
}

/** Self-division operator/function. This divides all energy components
    in this pack by 'scl' */
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
SIRE_INLINE_TEMPLATE
EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>& EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>::operator/=(
                  double scl)
{
    this->divide(scl);
    return *this;
}

/** Return a string representation of this EnergyPack */
template<EnergyID T1, EnergyID T2, EnergyID T3, EnergyID T4, EnergyID T5, 
         EnergyID T6, EnergyID T7, EnergyID T8, EnergyID T9>
SIRE_OUTOFLINE_TEMPLATE
QString EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9>::toString() const
{
    QString ret;
    
    QString space;
    
    if (T1 != 0)
    {
        ret += t1.toString();
        space = " | ";
    }
    
    if (T2 != 0)
    {
        ret += space + t2.toString();
        space = " | ";
    }
    
    if (T3 != 0)
    {
        ret += space + t3.toString();
        space = " | ";
    }
    
    if (T4 != 0)
    {
        ret += space + t4.toString();
        space = " | ";
    }
    
    if (T5 != 0)
    {
        ret += space + t5.toString();
        space = " | ";
    }

    if (T6 != 0)
    {
        ret += space + t6.toString();
        space = " | ";
    }

    if (T7 != 0)
    {
        ret += space + t7.toString();
        space = " | ";
    }

    if (T8 != 0)
    {
        ret += space + t8.toString();
        space = " | ";
    }

    if (T9 != 0)
        ret += space + t9.toString();

    ret += space + QObject::tr("Total: %1 kcal mol-1").arg(nrg);

    return ret;
}

}

/** Function to stream an EnergyPack to a data stream */
template<SireFF::EnergyID T1, SireFF::EnergyID T2, SireFF::EnergyID T3,
         SireFF::EnergyID T4, SireFF::EnergyID T5, SireFF::EnergyID T6,
         SireFF::EnergyID T7, SireFF::EnergyID T8, SireFF::EnergyID T9>
SIRE_OUTOFLINE_TEMPLATE
QDataStream& operator<<(QDataStream &ds, 
                        const SireFF::EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> &energy)
{
    ds << energy.t1 << energy.t2 << energy.t3 << energy.t4 << energy.t5 
       << energy.t6 << energy.t7 << energy.t8 << energy.t9;
    return ds;    
}

/** Function to stream an EnergyPack to a data stream */
template<SireFF::EnergyID T1, SireFF::EnergyID T2, SireFF::EnergyID T3,
         SireFF::EnergyID T4, SireFF::EnergyID T5, SireFF::EnergyID T6,
         SireFF::EnergyID T7, SireFF::EnergyID T8, SireFF::EnergyID T9>
SIRE_OUTOFLINE_TEMPLATE
QDataStream& operator>>(QDataStream &ds, 
                        SireFF::EnergyPack<T1,T2,T3,T4,T5,T6,T7,T8,T9> &energy)
{
    ds >> energy.t1 >> energy.t2 >> energy.t3 >> energy.t4 >> energy.t5 
       >> energy.t6 >> energy.t7 >> energy.t8 >> energy.t9;
    return ds;
}

SIRE_END_HEADER

#endif
