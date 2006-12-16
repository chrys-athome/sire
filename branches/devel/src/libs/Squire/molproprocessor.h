#ifndef SQUIRE_MOLPROPROCESSOR_H
#define SQUIRE_MOLPROPROCESSOR_H

#include "SireFF/ffthreadprocessor.h"

#include <QDir>

SIRE_BEGIN_HEADER

namespace Squire
{

using SireFF::ForceField;

namespace detail
{

/** Private implementation holding the data for MolproProcessor

    @author Christopher Woods
*/
class MolproProcessorPvt : public SireFF::detail::FFThreadProcessorPvt
{
public:
    MolproProcessorPvt();
    MolproProcessorPvt(const ForceField &forcefield,
                       const QString &molpro_exe,
                       const QDir &tmpdir);

    ~MolproProcessorPvt();

    void setMolpro(const QString &molpro_executable);
    void setTempDir(const QDir &temp_dir);

protected:
    boost::shared_ptr<SireCluster::WorkerBase> _pvt_activate();

private:
    /** The full name and path to the molpro executable */
    QString molpro_exe;

    /** The temp directory in which to run molpro jobs */
    QDir tmpdir;
};

}

/** This is the procesor that connects to an external Molpro process with
    which QM calculations may be performed. The connection is handled by
    a background thread, so will not block the main thread.

    @author Christopher Woods
*/
class SQUIRE_EXPORT MolproProcessor : public SireFF::FFThreadProcessor
{
public:
    MolproProcessor();
    MolproProcessor(const ForceField &forcefield,
                    const QString &molpro_exe = "molpro",
                    const QDir &temp_dir = QDir::temp());

    MolproProcessor(const MolproProcessor &other);

    ~MolproProcessor();

    void setMolpro(const QString &molpro_exe);
    void setTempDir(const QDir &tmpdir);

    static const char* typeName()
    {
        return "Squire::MolproProcessor";
    }

    const char* what() const
    {
        return MolproProcessor::typeName();
    }

protected:
    MolproProcessor(const boost::shared_ptr<detail::MolproProcessorPvt> &data);

    detail::MolproProcessorPvt& data();
    const detail::MolproProcessorPvt& data() const;
};

}

SIRE_END_HEADER

#endif
