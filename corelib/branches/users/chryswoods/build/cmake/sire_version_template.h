#ifndef SIRE_VERSION_H
#define SIRE_VERSION_H

#include <QString>

static const QString svn_repository_url = "@SVN_REPOSITORY_URL@";
static const QString svn_version_number = "@SVN_VERSION_NUMBER@";

static const int SIRE_VERSION_MAJOR = @SIRE_VERSION_MAJOR@;
static const int SIRE_VERSION_MINOR = @SIRE_VERSION_MINOR@;
static const int SIRE_VERSION_PATCH = @SIRE_VERSION_PATCH@;

static const QString compile_flags = "@CMAKE_CXX_FLAGS@";
static const QString link_flags = "@CMAKE_SHARED_LINKER_FLAGS@";

#endif
