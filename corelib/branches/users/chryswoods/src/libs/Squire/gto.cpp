/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2009  Christopher Woods
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


//////////
////////// Implementation of GTO
//////////




//////////
////////// Implementation of S_GTO
//////////

S_GTO::S_GTO() : alfa(0), scl(1)
{}

S_GTO::S_GTO(const Vector &center, double alpha, double scale)
      : cent(center), alfa(alpha), scl(scale)
{
    //set scl equal to 'scl' times the normalisation
    //factor for this orbital
    scl *= std::pow(2*alfa/pi, 0.75);
}

S_GTO::S_GTO(const S_GTO &other)
      : cent(other.cent), alfa(other.alfa), scl(other.scl)
{}

S_GTO::~S_GTO()
{}

S_GTO& S_GTO::operator=(const S_GTO &other)
{
    cent = other.cent;
    alfa = other.alfa;
    scl = other.scl;
    return *this;
}

const Vector& S_GTO::center() const
{
    return cent;
}

double S_GTO::alpha() const
{
    return alfa;
}

double S_GTO::scale() const
{
    return scl;
}

int S_GTO::angularMomentum()
{
    return 0;
}

S_GTO S_GTO::operator*(const S_GTO &other)
{
    //the product of two Gaussians is a Gaussian :-)
    const double alpha_times_beta = this->alpha() * other.alpha();
    const double alpha_plus_beta = this->alpha() + other.alpha();
    const double R2 = Vector::distance2(this->center(), other.center());
    
    S_GTO mult;
    
    mult.cent = (this->alpha() * this->center() + 
                 other.alpha() * other.center()) / alpha_plus_beta;
                 
    mult.alfa = alpha_plus_beta;
    
    mult.scl = this->scale() * other.scale() * 
                    std::exp(-alpha_times_beta*R2 / alpha_plus_beta);

    return mult;
}
