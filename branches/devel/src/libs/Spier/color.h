#ifndef SPIER_COLOR_H
#define SPIER_COLOR_H

#include "SireMaths/vector.h"

SIRE_BEGIN_HEADER

namespace Spier
{

using SireMaths::Vector;

/**
This class represents a color (32 bit red, green, blue, alpha). Each component has 256 values, from 0 to 255.
 
@author Christopher Woods
*/
class SPIER_EXPORT Color
{
public:
    Color();
    Color(uchar red, uchar green, uchar blue, uchar alpha=255);
    Color(const Vector &colorvec, double opacity=1.0);
    ~Color();

    void set(const Vector &colorvec, double opacity=1.0);
    
    void setRed(const uchar &r);
    void setBlue(const uchar &b);
    void setGreen(const uchar &g);
    void setAlpha(const uchar &a);

    const uchar& red() const;
    const uchar& green() const;
    const uchar& blue() const;
    const uchar& alpha() const;
    
    double r() const;
    double g() const;
    double b() const;
    double a() const;

    operator Vector() const;

private:
    /** The four components of the color (red, green, blue and alpha) */
    uchar rd, gn, bl, al;

};

}

SIRE_END_HEADER

#endif
