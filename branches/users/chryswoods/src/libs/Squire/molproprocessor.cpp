
#include "molproprocessor.h"
#include "molproff.h"

#include <QProcess>
#include <QThread>

#include <MolproClient/molproclient.h>

using namespace Squire;

namespace Squire
{
namespace detail
{

class MolproProcessorPvt : public QThread
{
public:
    MolproProcessorPvt();

    ~MolproProcessorPvt();

    void setForceField(const ForceField &ffield);

    double energy();

protected:
    void run();

private:
    /** The name of the molpro executable */
    QString molpro_exe;

    /** Local copy of the MolproFF being simulated */
    MolproFF molproff;

    /** The Molpro client used to connect to the Molpro server */
    MolproConnection molpro_client;

    /** The QProcess that is used to run the Molpro server */
    QProcess molpro_process;

    /** A mutex used to serialise access to this processor */
    QMutex thread_mutex;
};

}
}

///////////
/////////// Implementation of MolproProcessorPvt
///////////

using namespace Squire::detail;
/*
double MolproProcessorPvt::calculateEnergy(const MolproFF &ff)
{
    //can only calculate the energy of the local FF
    BOOST_ASSERT( &ff == &molproff );

    //off we go...

    return 0;
}
*/
