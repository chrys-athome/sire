
//including python headers must be done first!
#include <Python.h>
//Include the boost.python header
#include <boost/python.hpp>

#include <QDebug>

#include "python2qt.h"
#include "pythonerror.h"

//using python namespace - must be after including other headers!
using namespace boost::python;
using namespace SirePy;

namespace SirePy
{

/** First define the PythonError_pvt class */
class PythonError_pvt
{
public:
    PythonError_pvt();
    PythonError_pvt(const PythonError_pvt &err);
    ~PythonError_pvt();

    object typ;
    object val;
    object traceback;

    static QString currentFrame(object &tback);
};

}

PythonError_pvt::PythonError_pvt()
{}

PythonError_pvt::PythonError_pvt(const PythonError_pvt &err)
{
    typ = err.typ;
    val = err.val;
    traceback = err.traceback;
}

PythonError_pvt::~PythonError_pvt()
{}

/** Now define the functions of PythonError */
PythonError::PythonError() : pvterror(new PythonError_pvt()), isempty(true)
{}

PythonError::PythonError(const PythonError &err)
        : pvterror(new PythonError_pvt(*(err.pvterror))),
        isempty(err.isempty)
{}

PythonError::~PythonError()
{
    delete pvterror;
}

const PythonError& PythonError::operator=(const PythonError &err)
{
    if (*this == err)
        return *this;

    delete pvterror;
    pvterror = new PythonError_pvt(*(err.pvterror));
    isempty = err.isempty;
    return *this;
}

bool PythonError::operator==(const PythonError &err)
{
    //equal if both point to same pvterror object
    return (pvterror == err.pvterror);
}

bool PythonError::operator!=(const PythonError &err)
{
    return (pvterror != err.pvterror);
}

PythonError PythonError::fromPythonAPI()
{
    PythonError err;

    //fetch raw pointers to the Python objects reported by the exception
    PyObject *typ_ptr, *val_ptr, *traceback_ptr;
    PyErr_Fetch(&typ_ptr,&val_ptr,&traceback_ptr);

    //note that we now own the references to these error object...

    //make sure that there are some Python objects reported by the exception!
    if (typ_ptr == 0)
    {
        qDebug() << QObject::tr("Creating PythonError from API when there is no error?");
        
        qDebug() << QObject::tr("%1 %2 %3").arg(toUInt(typ_ptr)).arg(toUInt(val_ptr))
                      .arg(toUInt(traceback_ptr));
        
        return err;
    }

    //the new error is not empty
    err.isempty = false;

    //now lets make some boost::python::objects out of these pointers. This
    //will increment the reference count of these objects (we will own them twice!)
    err.pvterror->typ = Python2Qt::toBoostObject(typ_ptr);
    err.pvterror->val = Python2Qt::toBoostObject(val_ptr);
    err.pvterror->traceback = Python2Qt::toBoostObject(traceback_ptr);

    //now restore the pointers to the Python error indicator - this will
    //remove one of our references to these objects (so now we only own them once)
    PyErr_Restore(typ_ptr,val_ptr,traceback_ptr);

    //Tell python to print the exception to stderr so that we get a Python-eye-view
    //of the error. This will clear the error indicator
    PyErr_Print();

    //we can now return 'err' knowing that all of the detail of the
    //python exception is now available in the boost::python::objects held
    //in its 'pvterror'.
    return err;
}

void PythonError::debug()
{
    //if this is an empty error then return
    if (isempty)
    {
        qDebug() << QObject::tr("There is no Python error. Everything is fine, move along...");
        return;
    }

    //print the type of error
    qDebug() << QObject::tr("Python error of type '%1'").arg(this->type());
    qDebug() << QObject::tr("Python error value = '%1'").arg(this->value());
    qDebug() << QObject::tr("Traceback\n%1").arg(this->traceback());
}

QString PythonError::type()
{
    if (isempty)
        return QObject::tr("NoError");
    else
        return Python2Qt::str(pvterror->typ);
}

QString PythonError::value()
{
    if (isempty)
        return QObject::tr("NoError");
    else
        return Python2Qt::str(pvterror->val);
}

QString PythonError::traceback()
{
#warning Will replace this function with a class that returns a 'traceback' object!

    if (isempty)
        return QObject::tr("NoError");
    else
    {
        //The Python TracebackObject consists of a number of FrameObjects, each of which
        //consists of CodeObjects...

        QString tbackstr;

        //copy the traceback into a temporary object representing the current level
        //of this traceback
        object tlevel = pvterror->traceback;

        try
        {
            //keep going while the level is not None
            while(not Python2Qt::isNone(tlevel))
            {
                //append the information about the current level
                tbackstr += PythonError_pvt::currentFrame(tlevel) + "\n";

                //advance to the next frame
                tlevel = tlevel.attr("tb_next");
            }
        }
        catch(...)
        {
            return QObject::tr("Traceback unavailable. The Python API may have changed...");
        }

        //return the constructed string
        return tbackstr;
    }
}

QString PythonError_pvt::currentFrame(object &tback)
{
    object frame = tback.attr("tb_frame");
    
    //make sure that the frame exists, if not only return the line number
    if (Python2Qt::isNone(frame))
    {
        return QObject::tr("??? on line ??? (no frame information available!)");
    }
    
    //get the line number of this frame
    int frameline = extract<int>(frame.attr("f_lineno"));
        
    //get the code object referred to by this frame
    object codeobj = frame.attr("f_code");
    
    //get the name of this function
    const char* codename = extract<const char*>(codeobj.attr("co_name"));
    
    //get the name of the file containing this code object
    const char* filename = extract<const char*>(codeobj.attr("co_filename"));
    
    return QObject::tr("File \"%1\", line %2, in %3")
                .arg(filename).arg(frameline).arg(codename);
}

bool PythonError::isEmpty() const
{
    return isempty;
}

