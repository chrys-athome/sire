#ifndef PYWRAP_SIREQT_QHASH_REAL_H
#define PYWRAP_SIREQT_QHASH_REAL_H

inline unsigned int qHash(const double &val)
{
    //reinterpret the bits of the double as an unsigned int
    union { double d; unsigned int i; } converter;
    
    //clear the memory of the union - this should
    //prevent any surprises
    converter.i = 0;
    
    //assign the memory of the union to hold the double
    converter.d = val;
    
    //now return the union as an integer - this forces
    //the bit pattern of the double to be interpreted
    //as an integer
    return converter.i;
}

inline unsigned int qHash(const float &val)
{
    //reinterpret the bits of the double as an unsigned int
    union { double d; unsigned int i; } converter;
    
    //clear the memory of the union - this should
    //prevent any surprises
    converter.i = 0;
    
    //assign the memory of the union to hold the double
    converter.d = val;
    
    //now return the union as an integer - this forces
    //the bit pattern of the double to be interpreted
    //as an integer
    return converter.i;
}

#endif
