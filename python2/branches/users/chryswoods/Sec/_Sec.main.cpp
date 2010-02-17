// This file has been generated by Py++.

// (C) Christopher Woods, GPL >= 2 License


#include "boost/python.hpp"

#include "Key.pypp.hpp"

#include "Lock.pypp.hpp"

#include "Password.pypp.hpp"

#include "PasswordLock.pypp.hpp"

#include "PrivateKey.pypp.hpp"

#include "PubPriLock.pypp.hpp"

#include "PublicKey.pypp.hpp"

#include "_Sec_enumerations.pypp.hpp"

namespace bp = boost::python;

#include "SireSec_containers.h"

#include "SireSec_properties.h"

#include "SireSec/key.h"

BOOST_PYTHON_MODULE(_Sec){
    register_enumerations();

    register_SireSec_containers();

    register_Key_class();

    register_Lock_class();

    register_Password_class();

    register_PasswordLock_class();

    register_PrivateKey_class();

    register_PubPriLock_class();

    register_SireSec_properties();

    bp::implicitly_convertible< SireSec::Key::Option, SireSec::Key::Options >();

    register_PublicKey_class();
}

