// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License


#include "boost/python.hpp"

#include "BoolValue.pypp.hpp"

#include "DirValue.pypp.hpp"

#include "EnumValue.pypp.hpp"

#include "FileValue.pypp.hpp"

#include "FloatValue.pypp.hpp"

#include "IntegerValue.pypp.hpp"

#include "LengthValue.pypp.hpp"

#include "Option.pypp.hpp"

#include "Options.pypp.hpp"

#include "ProteinLigandMC.pypp.hpp"

#include "ProteinLigandMCParams.pypp.hpp"

#include "Sim.pypp.hpp"

#include "SimParams.pypp.hpp"

#include "StringValue.pypp.hpp"

#include "Value.pypp.hpp"

namespace bp = boost::python;

#include "SireSim_containers.h"

#include "SireSim_registrars.h"

BOOST_PYTHON_MODULE(_Sim){
    register_SireSim_objects();

    register_SireSim_containers();

    register_Value_class();

    register_BoolValue_class();

    register_DirValue_class();

    register_EnumValue_class();

    register_FileValue_class();

    register_FloatValue_class();

    register_IntegerValue_class();

    register_LengthValue_class();

    register_Option_class();

    register_Options_class();

    register_Sim_class();

    register_ProteinLigandMC_class();

    register_SimParams_class();

    register_ProteinLigandMCParams_class();

    register_StringValue_class();
}

