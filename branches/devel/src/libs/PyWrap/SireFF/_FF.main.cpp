// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License


#include "boost/python.hpp"

#include "sireff_headers.h"

#include "SireMol/molecule.h"

#include "SireMol/partialmolecule.h"

#include "SireMol/residue.h"

#include "SireMol/newatom.h"

#include "SireMol/atom.h"

#include "SireMol/cgatomid.h"

#include "SireMol/cutgroupnum.h"

#include "SireMol/resnumatomid.h"

#include "SireMol/resid.h"

#include "SireMol/moleculeid.h"

#include "SireBase/property.h"

#include "FFBase.pypp.hpp"

#include "FFCalculator.pypp.hpp"

#include "FFCalculatorBase.pypp.hpp"

#include "FFComponent.pypp.hpp"

#include "FFExpression.pypp.hpp"

#include "FFGroupID.pypp.hpp"

#include "FFLocalCalculator.pypp.hpp"

#include "FFLocalWorker.pypp.hpp"

#include "FFProcessor.pypp.hpp"

#include "FFProcessorBase.pypp.hpp"

#include "FFThreadProcessor.pypp.hpp"

#include "FFThreadWorker.pypp.hpp"

#include "FFWorker.pypp.hpp"

#include "FFWorkerBase.pypp.hpp"

#include "ForceField.pypp.hpp"

#include "ForceFieldID.pypp.hpp"

#include "ForceFields.pypp.hpp"

#include "ForceFieldsBase.pypp.hpp"

#include "ForceTable.pypp.hpp"

#include "ParameterMap.pypp.hpp"

#include "ParameterName.pypp.hpp"

#include "ParameterSource.pypp.hpp"

#include "sireff_containers.h"

namespace bp = boost::python;

BOOST_PYTHON_MODULE(_FF){
    register_SireFF_containers();

    register_ParameterMap_class();

    register_FFBase_class();

    bp::implicitly_convertible< SireFF::FFBase::Group, quint32 >();

    register_FFCalculatorBase_class();

    register_FFLocalCalculator_class();

    register_FFCalculator_class();

    register_FFComponent_class();

    register_FFExpression_class();

    register_FFGroupID_class();

    register_FFWorkerBase_class();

    register_FFLocalWorker_class();

    register_FFProcessorBase_class();

    register_FFProcessor_class();

    register_FFThreadProcessor_class();

    register_FFThreadWorker_class();

    register_FFWorker_class();

    register_ForceField_class();

    register_ForceFieldID_class();

    register_ForceFieldsBase_class();

    register_ForceFields_class();

    register_ForceTable_class();

    register_ParameterName_class();

    bp::implicitly_convertible< QString, SireFF::ParameterName >();

    bp::implicitly_convertible< const SireFF::FFBase&, SireFF::ForceField >();

    bp::implicitly_convertible< SireCAS::Function, SireFF::FFComponent >();

    bp::implicitly_convertible< SireCAS::Function, SireFF::FFExpression >();

    bp::implicitly_convertible< QHash<SireFF::ParameterName,QString>, SireFF::ParameterMap >();

    register_ParameterSource_class();
}

