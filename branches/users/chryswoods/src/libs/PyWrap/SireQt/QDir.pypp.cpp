// This file has been generated by Py++.

// (C) Christopher Woods, GPL >=2 License

#include "QDir.pypp.hpp"
#include "boost/python.hpp"
#include "sireqt_headers.h"

namespace bp = boost::python;

void register_QDir_class(){

    { //::QDir
        typedef bp::class_< QDir > QDir_exposer_t;
        QDir_exposer_t QDir_exposer = QDir_exposer_t( "QDir", bp::init< bp::optional< QString const & > >(( bp::arg("path")=::QString( ) )) );
        bp::scope QDir_scope( QDir_exposer );
        bp::enum_< QDir::Filter>("Filter")
            .value("Dirs", QDir::Dirs)
            .value("Files", QDir::Files)
            .value("Drives", QDir::Drives)
            .value("NoSymLinks", QDir::NoSymLinks)
            .value("AllEntries", QDir::AllEntries)
            .value("TypeMask", QDir::TypeMask)
            .value("Readable", QDir::Readable)
            .value("Writable", QDir::Writable)
            .value("Executable", QDir::Executable)
            .value("PermissionMask", QDir::PermissionMask)
            .value("Modified", QDir::Modified)
            .value("Hidden", QDir::Hidden)
            .value("System", QDir::System)
            .value("AccessMask", QDir::AccessMask)
            .value("AllDirs", QDir::AllDirs)
            .value("CaseSensitive", QDir::CaseSensitive)
            .value("NoDotAndDotDot", QDir::NoDotAndDotDot)
            .value("NoFilter", QDir::NoFilter)
            .export_values()
            ;
        bp::enum_< QDir::SortFlag>("SortFlag")
            .value("Name", QDir::Name)
            .value("Time", QDir::Time)
            .value("Size", QDir::Size)
            .value("Unsorted", QDir::Unsorted)
            .value("SortByMask", QDir::SortByMask)
            .value("DirsFirst", QDir::DirsFirst)
            .value("Reversed", QDir::Reversed)
            .value("IgnoreCase", QDir::IgnoreCase)
            .value("DirsLast", QDir::DirsLast)
            .value("LocaleAware", QDir::LocaleAware)
            .value("Type", QDir::Type)
            .value("NoSort", QDir::NoSort)
            .export_values()
            ;
        QDir_exposer.def( bp::init< QString const &, QString const &,
        bp::optional< QFlags<QDir::SortFlag>, QFlags<QDir::Filter> > >((
        bp::arg("path"), bp::arg("nameFilter"),
        bp::arg("sort")=::QFlags<QDir::SortFlag>( QFlag(16) ),
        bp::arg("filter")=QDir::AllEntries )) );
        { //::QDir::absoluteFilePath
        
            typedef ::QString ( ::QDir::*absoluteFilePath_function_type )( ::QString const & ) const;
            
            QDir_exposer.def( 
                "absoluteFilePath"
                , absoluteFilePath_function_type( &::QDir::absoluteFilePath )
                , ( bp::arg("fileName") ) );
        
        }
        { //::QDir::absolutePath
        
            typedef ::QString ( ::QDir::*absolutePath_function_type )(  ) const;
            
            QDir_exposer.def( 
                "absolutePath"
                , absolutePath_function_type( &::QDir::absolutePath ) );
        
        }
        { //::QDir::addResourceSearchPath
        
            typedef void ( *addResourceSearchPath_function_type )( ::QString const & );
            
            QDir_exposer.def( 
                "addResourceSearchPath"
                , addResourceSearchPath_function_type( &::QDir::addResourceSearchPath )
                , ( bp::arg("path") ) );
        
        }
        { //::QDir::canonicalPath
        
            typedef ::QString ( ::QDir::*canonicalPath_function_type )(  ) const;
            
            QDir_exposer.def( 
                "canonicalPath"
                , canonicalPath_function_type( &::QDir::canonicalPath ) );
        
        }
        { //::QDir::cd
        
            typedef bool ( ::QDir::*cd_function_type )( ::QString const & ) ;
            
            QDir_exposer.def( 
                "cd"
                , cd_function_type( &::QDir::cd )
                , ( bp::arg("dirName") ) );
        
        }
        { //::QDir::cdUp
        
            typedef bool ( ::QDir::*cdUp_function_type )(  ) ;
            
            QDir_exposer.def( 
                "cdUp"
                , cdUp_function_type( &::QDir::cdUp ) );
        
        }
        { //::QDir::cleanPath
        
            typedef ::QString ( *cleanPath_function_type )( ::QString const & );
            
            QDir_exposer.def( 
                "cleanPath"
                , cleanPath_function_type( &::QDir::cleanPath )
                , ( bp::arg("path") ) );
        
        }
        { //::QDir::count
        
            typedef ::uint ( ::QDir::*count_function_type )(  ) const;
            
            QDir_exposer.def( 
                "count"
                , count_function_type( &::QDir::count ) );
        
        }
        { //::QDir::current
        
            typedef ::QDir ( *current_function_type )(  );
            
            QDir_exposer.def( 
                "current"
                , current_function_type( &::QDir::current ) );
        
        }
        { //::QDir::currentPath
        
            typedef ::QString ( *currentPath_function_type )(  );
            
            QDir_exposer.def( 
                "currentPath"
                , currentPath_function_type( &::QDir::currentPath ) );
        
        }
        { //::QDir::dirName
        
            typedef ::QString ( ::QDir::*dirName_function_type )(  ) const;
            
            QDir_exposer.def( 
                "dirName"
                , dirName_function_type( &::QDir::dirName ) );
        
        }
        { //::QDir::drives
        
            typedef ::QFileInfoList ( *drives_function_type )(  );
            
            QDir_exposer.def( 
                "drives"
                , drives_function_type( &::QDir::drives ) );
        
        }
        { //::QDir::entryInfoList
        
            typedef ::QFileInfoList ( ::QDir::*entryInfoList_function_type )( ::QFlags<QDir::Filter>,::QFlags<QDir::SortFlag> ) const;
            
            QDir_exposer.def( 
                "entryInfoList"
                , entryInfoList_function_type( &::QDir::entryInfoList )
                , ( bp::arg("filters")=QDir::NoFilter, bp::arg("sort")=QDir::NoSort ) );
        
        }
        { //::QDir::entryInfoList
        
            typedef ::QFileInfoList ( ::QDir::*entryInfoList_function_type )( ::QStringList const &,::QFlags<QDir::Filter>,::QFlags<QDir::SortFlag> ) const;
            
            QDir_exposer.def( 
                "entryInfoList"
                , entryInfoList_function_type( &::QDir::entryInfoList )
                , ( bp::arg("nameFilters"), bp::arg("filters")=QDir::NoFilter,
                bp::arg("sort")=QDir::NoSort ) );
        
        }
        { //::QDir::entryList
        
            typedef ::QStringList ( ::QDir::*entryList_function_type )( ::QFlags<QDir::Filter>,::QFlags<QDir::SortFlag> ) const;
            
            QDir_exposer.def( 
                "entryList"
                , entryList_function_type( &::QDir::entryList )
                , ( bp::arg("filters")=QDir::NoFilter, bp::arg("sort")=QDir::NoSort ) );
        
        }
        { //::QDir::entryList
        
            typedef ::QStringList ( ::QDir::*entryList_function_type )( ::QStringList const &,::QFlags<QDir::Filter>,::QFlags<QDir::SortFlag> ) const;
            
            QDir_exposer.def( 
                "entryList"
                , entryList_function_type( &::QDir::entryList )
                , ( bp::arg("nameFilters"), bp::arg("filters")=QDir::NoFilter,
                    bp::arg("sort")=QDir::NoSort ) );
        
        }
        { //::QDir::exists
        
            typedef bool ( ::QDir::*exists_function_type )(  ) const;
            
            QDir_exposer.def( 
                "exists"
                , exists_function_type( &::QDir::exists ) );
        
        }
        { //::QDir::exists
        
            typedef bool ( ::QDir::*exists_function_type )( ::QString const & ) const;
            
            QDir_exposer.def( 
                "exists"
                , exists_function_type( &::QDir::exists )
                , ( bp::arg("name") ) );
        
        }
        { //::QDir::filePath
        
            typedef ::QString ( ::QDir::*filePath_function_type )( ::QString const & ) const;
            
            QDir_exposer.def( 
                "filePath"
                , filePath_function_type( &::QDir::filePath )
                , ( bp::arg("fileName") ) );
        
        }
        { //::QDir::filter
        
            typedef ::QFlags<QDir::Filter> ( ::QDir::*filter_function_type )(  ) const;
            
            QDir_exposer.def( 
                "filter"
                , filter_function_type( &::QDir::filter ) );
        
        }
        { //::QDir::fromNativeSeparators
        
            typedef ::QString ( *fromNativeSeparators_function_type )( ::QString const & );
            
            QDir_exposer.def( 
                "fromNativeSeparators"
                , fromNativeSeparators_function_type( &::QDir::fromNativeSeparators )
                , ( bp::arg("pathName") ) );
        
        }
        { //::QDir::home
        
            typedef ::QDir ( *home_function_type )(  );
            
            QDir_exposer.def( 
                "home"
                , home_function_type( &::QDir::home ) );
        
        }
        { //::QDir::homePath
        
            typedef ::QString ( *homePath_function_type )(  );
            
            QDir_exposer.def( 
                "homePath"
                , homePath_function_type( &::QDir::homePath ) );
        
        }
        { //::QDir::isAbsolute
        
            typedef bool ( ::QDir::*isAbsolute_function_type )(  ) const;
            
            QDir_exposer.def( 
                "isAbsolute"
                , isAbsolute_function_type( &::QDir::isAbsolute ) );
        
        }
        { //::QDir::isAbsolutePath
        
            typedef bool ( *isAbsolutePath_function_type )( ::QString const & );
            
            QDir_exposer.def( 
                "isAbsolutePath"
                , isAbsolutePath_function_type( &::QDir::isAbsolutePath )
                , ( bp::arg("path") ) );
        
        }
        { //::QDir::isReadable
        
            typedef bool ( ::QDir::*isReadable_function_type )(  ) const;
            
            QDir_exposer.def( 
                "isReadable"
                , isReadable_function_type( &::QDir::isReadable ) );
        
        }
        { //::QDir::isRelative
        
            typedef bool ( ::QDir::*isRelative_function_type )(  ) const;
            
            QDir_exposer.def( 
                "isRelative"
                , isRelative_function_type( &::QDir::isRelative ) );
        
        }
        { //::QDir::isRelativePath
        
            typedef bool ( *isRelativePath_function_type )( ::QString const & );
            
            QDir_exposer.def( 
                "isRelativePath"
                , isRelativePath_function_type( &::QDir::isRelativePath )
                , ( bp::arg("path") ) );
        
        }
        { //::QDir::isRoot
        
            typedef bool ( ::QDir::*isRoot_function_type )(  ) const;
            
            QDir_exposer.def( 
                "isRoot"
                , isRoot_function_type( &::QDir::isRoot ) );
        
        }
        { //::QDir::makeAbsolute
        
            typedef bool ( ::QDir::*makeAbsolute_function_type )(  ) ;
            
            QDir_exposer.def( 
                "makeAbsolute"
                , makeAbsolute_function_type( &::QDir::makeAbsolute ) );
        
        }
        { //::QDir::match
        
            typedef bool ( *match_function_type )( ::QStringList const &,::QString const & );
            
            QDir_exposer.def( 
                "match"
                , match_function_type( &::QDir::match )
                , ( bp::arg("filters"), bp::arg("fileName") ) );
        
        }
        { //::QDir::match
        
            typedef bool ( *match_function_type )( ::QString const &,::QString const & );
            
            QDir_exposer.def( 
                "match"
                , match_function_type( &::QDir::match )
                , ( bp::arg("filter"), bp::arg("fileName") ) );
        
        }
        { //::QDir::mkdir
        
            typedef bool ( ::QDir::*mkdir_function_type )( ::QString const & ) const;
            
            QDir_exposer.def( 
                "mkdir"
                , mkdir_function_type( &::QDir::mkdir )
                , ( bp::arg("dirName") ) );
        
        }
        { //::QDir::mkpath
        
            typedef bool ( ::QDir::*mkpath_function_type )( ::QString const & ) const;
            
            QDir_exposer.def( 
                "mkpath"
                , mkpath_function_type( &::QDir::mkpath )
                , ( bp::arg("dirPath") ) );
        
        }
        { //::QDir::nameFilters
        
            typedef ::QStringList ( ::QDir::*nameFilters_function_type )(  ) const;
            
            QDir_exposer.def( 
                "nameFilters"
                , nameFilters_function_type( &::QDir::nameFilters ) );
        
        }
        QDir_exposer.def( bp::self != bp::self );
        QDir_exposer.def( bp::self == bp::self );
        { //::QDir::operator[]
        
            typedef ::QString ( ::QDir::*__getitem___function_type )( int ) const;
            
            QDir_exposer.def( 
                "__getitem__"
                , __getitem___function_type( &::QDir::operator[] )
                , ( bp::arg("arg0") ) );
        
        }
        { //::QDir::path
        
            typedef ::QString ( ::QDir::*path_function_type )(  ) const;
            
            QDir_exposer.def( 
                "path"
                , path_function_type( &::QDir::path ) );
        
        }
        { //::QDir::refresh
        
            typedef void ( ::QDir::*refresh_function_type )(  ) const;
            
            QDir_exposer.def( 
                "refresh"
                , refresh_function_type( &::QDir::refresh ) );
        
        }
        { //::QDir::relativeFilePath
        
            typedef ::QString ( ::QDir::*relativeFilePath_function_type )( ::QString const & ) const;
            
            QDir_exposer.def( 
                "relativeFilePath"
                , relativeFilePath_function_type( &::QDir::relativeFilePath )
                , ( bp::arg("fileName") ) );
        
        }
        { //::QDir::remove
        
            typedef bool ( ::QDir::*remove_function_type )( ::QString const & ) ;
            
            QDir_exposer.def( 
                "remove"
                , remove_function_type( &::QDir::remove )
                , ( bp::arg("fileName") ) );
        
        }
        { //::QDir::rename
        
            typedef bool ( ::QDir::*rename_function_type )( ::QString const &,::QString const & ) ;
            
            QDir_exposer.def( 
                "rename"
                , rename_function_type( &::QDir::rename )
                , ( bp::arg("oldName"), bp::arg("newName") ) );
        
        }
        { //::QDir::rmdir
        
            typedef bool ( ::QDir::*rmdir_function_type )( ::QString const & ) const;
            
            QDir_exposer.def( 
                "rmdir"
                , rmdir_function_type( &::QDir::rmdir )
                , ( bp::arg("dirName") ) );
        
        }
        { //::QDir::rmpath
        
            typedef bool ( ::QDir::*rmpath_function_type )( ::QString const & ) const;
            
            QDir_exposer.def( 
                "rmpath"
                , rmpath_function_type( &::QDir::rmpath )
                , ( bp::arg("dirPath") ) );
        
        }
        { //::QDir::root
        
            typedef ::QDir ( *root_function_type )(  );
            
            QDir_exposer.def( 
                "root"
                , root_function_type( &::QDir::root ) );
        
        }
        { //::QDir::rootPath
        
            typedef ::QString ( *rootPath_function_type )(  );
            
            QDir_exposer.def( 
                "rootPath"
                , rootPath_function_type( &::QDir::rootPath ) );
        
        }
        { //::QDir::separator
        
            typedef ::QChar ( *separator_function_type )(  );
            
            QDir_exposer.def( 
                "separator"
                , separator_function_type( &::QDir::separator ) );
        
        }
        { //::QDir::setCurrent
        
            typedef bool ( *setCurrent_function_type )( ::QString const & );
            
            QDir_exposer.def( 
                "setCurrent"
                , setCurrent_function_type( &::QDir::setCurrent )
                , ( bp::arg("path") ) );
        
        }
        { //::QDir::setFilter
        
            typedef void ( ::QDir::*setFilter_function_type )( ::QFlags<QDir::Filter> ) ;
            
            QDir_exposer.def( 
                "setFilter"
                , setFilter_function_type( &::QDir::setFilter )
                , ( bp::arg("filter") ) );
        
        }
        { //::QDir::setNameFilters
        
            typedef void ( ::QDir::*setNameFilters_function_type )( ::QStringList const & ) ;
            
            QDir_exposer.def( 
                "setNameFilters"
                , setNameFilters_function_type( &::QDir::setNameFilters )
                , ( bp::arg("nameFilters") ) );
        
        }
        { //::QDir::setPath
        
            typedef void ( ::QDir::*setPath_function_type )( ::QString const & ) ;
            
            QDir_exposer.def( 
                "setPath"
                , setPath_function_type( &::QDir::setPath )
                , ( bp::arg("path") ) );
        
        }
        { //::QDir::setSorting
        
            typedef void ( ::QDir::*setSorting_function_type )( ::QFlags<QDir::SortFlag> ) ;
            
            QDir_exposer.def( 
                "setSorting"
                , setSorting_function_type( &::QDir::setSorting )
                , ( bp::arg("sort") ) );
        
        }
        { //::QDir::sorting
        
            typedef ::QFlags<QDir::SortFlag> ( ::QDir::*sorting_function_type )(  ) const;
            
            QDir_exposer.def( 
                "sorting"
                , sorting_function_type( &::QDir::sorting ) );
        
        }
        { //::QDir::temp
        
            typedef ::QDir ( *temp_function_type )(  );
            
            QDir_exposer.def( 
                "temp"
                , temp_function_type( &::QDir::temp ) );
        
        }
        { //::QDir::tempPath
        
            typedef ::QString ( *tempPath_function_type )(  );
            
            QDir_exposer.def( 
                "tempPath"
                , tempPath_function_type( &::QDir::tempPath ) );
        
        }
        { //::QDir::toNativeSeparators
        
            typedef ::QString ( *toNativeSeparators_function_type )( ::QString const & );
            
            QDir_exposer.def( 
                "toNativeSeparators"
                , toNativeSeparators_function_type( &::QDir::toNativeSeparators )
                , ( bp::arg("pathName") ) );
        
        }
        QDir_exposer.staticmethod( "addResourceSearchPath" );
        QDir_exposer.staticmethod( "cleanPath" );
        QDir_exposer.staticmethod( "current" );
        QDir_exposer.staticmethod( "currentPath" );
        QDir_exposer.staticmethod( "drives" );
        QDir_exposer.staticmethod( "fromNativeSeparators" );
        QDir_exposer.staticmethod( "home" );
        QDir_exposer.staticmethod( "homePath" );
        QDir_exposer.staticmethod( "isAbsolutePath" );
        QDir_exposer.staticmethod( "isRelativePath" );
        QDir_exposer.staticmethod( "match" );
        QDir_exposer.staticmethod( "root" );
        QDir_exposer.staticmethod( "rootPath" );
        QDir_exposer.staticmethod( "separator" );
        QDir_exposer.staticmethod( "setCurrent" );
        QDir_exposer.staticmethod( "temp" );
        QDir_exposer.staticmethod( "tempPath" );
        QDir_exposer.staticmethod( "toNativeSeparators" );
    }

}
