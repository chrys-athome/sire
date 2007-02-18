// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License


#include "boost/python.hpp"

#include "siremove_headers.h"

#include "SireMol/molecule.h"

#include "SireMol/residue.h"

#include "SireMol/newatom.h"

#include "SireMol/moleculegroup.h"

#include "SireSystem/simsystem.h"

#include "MonteCarlo.pypp.hpp"

#include "Sampler.pypp.hpp"

#include "SamplerBase.pypp.hpp"

#include "UniformSampler.pypp.hpp"

#include "siremove_containers.h"

namespace bp = boost::python;

BOOST_PYTHON_MODULE(_Move){
    register_SireMove_containers();

    register_MonteCarlo_class();

    register_Sampler_class();

    register_SamplerBase_class();

    bp::implicitly_convertible< const SireMove::SamplerBase&, SireMove::Sampler >();

    register_UniformSampler_class();
}

