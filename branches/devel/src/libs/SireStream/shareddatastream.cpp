/**
  * @file
  *
  * C++ Implementation: SharedDataStream
  *
  * Description: Implementation of SharedDataStream
  *
  *
  * @author Christopher Woods, (C) 2006
  *
  * Copyright: See COPYING file that comes with this distribution
  *
  */

#include "shareddatastream.h"

using namespace SireStream;

/** Construct a SharedDataStream that uses the QDataStream 'datastream'
    to serialise and unserialise the objects. */
SharedDataStream::SharedDataStream(QDataStream &qdatastream)
                 : ds(qdatastream)
{}

/** Destructor */
SharedDataStream::~SharedDataStream()
{}
