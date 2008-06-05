/********************************************\
  *
  *  Sire - Molecular Simulation Framework
  *
  *  Copyright (C) 2007  Christopher Woods
  *
  *  This program is free software; you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation; either version 2 of the License, or
  *  (at your option) any later version.
  *
  *  This program is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with this program; if not, write to the Free Software
  *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  *  For full details of the license please see the COPYING file
  *  that should have come with this distribution.
  *
  *  You can contact the authors via the developer's mailing list
  *  at http://siremol.org
  *
\*********************************************/

#include "mtsmc.h"

#include "SireCluster/threadworker.h"

#include "SireSystem/localsimsystem.h"

#include "SireSystem/checkpoint.h"
#include "SireSystem/simsystem.h"

#include "SireStream/datastream.h"
#include "SireStream/shareddatastream.h"

using namespace SireMove;
using namespace SireSystem;
using namespace SireStream;
using namespace SireCluster;

static const RegisterMetaType<MTSMC> r_mtsmc;

/** Serialise to a binary data stream */
QDataStream SIREMOVE_EXPORT &operator<<(QDataStream &ds, const MTSMC &mtsmc)
{
    writeHeader(ds, r_mtsmc, 1);

    SharedDataStream sds(ds);
    sds << mtsmc.fast_moves << mtsmc.nfast
        << mtsmc.nslow
        << mtsmc.fast_component
        << static_cast<const MonteCarlo&>(mtsmc);

    return ds;
}

/** Deserialise from a binary data stream */
QDataStream SIREMOVE_EXPORT &operator>>(QDataStream &ds, MTSMC &mtsmc)
{
    VersionID v = readHeader(ds, r_mtsmc);

    if (v == 1)
    {
        SharedDataStream sds(ds);

        sds >> mtsmc.fast_moves >> mtsmc.nfast
            >> mtsmc.nslow
            >> mtsmc.fast_component
            >> static_cast<MonteCarlo&>(mtsmc);
    }
    else
        throw version_error(v, "1", r_mtsmc, CODELOC);

    return ds;
}

/** Null constructor */
MTSMC::MTSMC()
      : MonteCarlo(),
        fast_component(SireFF::e_total()), nfast(0), nslow(1)
{}

/** Create an MTSMC move that uses 'moves' to perform the 'fast' moves,
    and performs 'nfastmoves' using the 'fast' energy represented by
    'fastcomponent' */
MTSMC::MTSMC(const Moves &moves, const Symbol &fastcomponent, quint32 nfastmoves)
      : MonteCarlo(),
        fast_moves(moves), fast_component(fastcomponent), nfast(nfastmoves), nslow(1)
{
    //make sure that all of the moves use the fast component
    //to calculate the energy
    fast_moves.setEnergyComponent(fast_component);
}

/** Copy constructor */
MTSMC::MTSMC(const MTSMC &other)
      : MonteCarlo(other),
        fast_moves(other.fast_moves), fast_component(other.fast_component),
        nfast(other.nfast), nslow(other.nslow)
{}

/** Destructor */
MTSMC::~MTSMC()
{}

/** Copy assignment operator */
MTSMC& MTSMC::operator=(const MTSMC &other)
{
    if (this != &other)
    {
        fast_moves = other.fast_moves;
        fast_component = other.fast_component;
        nfast = other.nfast;
        nslow = other.nslow;
        MonteCarlo::operator=(other);
    }

    return *this;
}

/** Set the moves that will be performed using the 'fast' energy */
void MTSMC::setMoves(const Moves &moves)
{
    fast_moves = moves;
    fast_moves.setEnergyComponent(fast_component);
}

/** Set the moves that will be performed using the 'fast' energy
    represented by the component 'fastcomponent' */
void MTSMC::setMoves(const Moves &moves, const Symbol &fastcomponent)
{
    fast_moves = moves;
    fast_component = fastcomponent;

    fast_moves.setEnergyComponent(fast_component);
}

/** Set the number of moves that will be performed using the
    fast energy */
void MTSMC::setNumFastMoves(quint32 nmoves)
{
    nfast = nmoves;
}

/** Set the number of moves that are performed with the slow energy.
    By allowing multiple slow moves per MTSMC move, it becomes
    possible to overlay the slow and fast moves so that they are
    run in parallel (thereby allowing the next configuration to be
    generated using the fast forcefield while the previous configuration
    is still being tested using the slow forcefield) */
void MTSMC::setNumSlowMoves(quint32 nmoves)
{
    if (nmoves == 0)
        nslow = 1;
    else
        nslow = nmoves;
}

/** Return the moves that will be performed using the fast
    energy component */
const Moves& MTSMC::moves() const
{
    return fast_moves;
}

/** Return the number of moves that will be performed on the
    fast forcefield */
quint32 MTSMC::numFastMoves() const
{
    return nfast;
}

/** Return the number of slow moves per MTSMC move - if this is
    more than one then the fast and slow moves will be attempted
    in parallel (therefore more than one processor core is required!) */
quint32 MTSMC::numSlowMoves() const
{
    return nslow;
}

/** Return the energy component that represents the fast
    component of the forcefield. */
const Symbol& MTSMC::fastEnergyComponent() const
{
    return fast_component;
}

/** Set the energy component that will be used for the
    'fast' moves. */
void MTSMC::setFastEnergyComponent(const Symbol &component)
{
    if (component != fast_component)
    {
        fast_component = component;
        fast_moves.setEnergyComponent(fast_component);
    }
}

/** Assert that this move is compatible with the passed system */
void MTSMC::assertCompatibleWith(QuerySystem &system) const
{
    fast_moves.assertCompatibleWith(system);
    MonteCarlo::assertCompatibleWith(system);
}

class MTSMC_BG_Runner : public ThreadWorker
{
public:
    MTSMC_BG_Runner(const CheckPoint &checkpoint, const Moves &fast_moves, quint32 nfast);

    ~MTSMC_BG_Runner();

    void start();
    void stop();

    void wait();

    CheckPoint checkPoint();
    Moves moves();

private:
    void calculate();

    /** The system being simulated */
    LocalSimSystem simsystem;

    /** The fast moves run on the system */
    Moves fast_moves;

    /** Mutex to serialise access to this object */
    QMutex mutex;

    /** The number of fast moves to run */
    quint32 nfast;
};

MTSMC_BG_Runner::MTSMC_BG_Runner(const CheckPoint &checkpoint,
                                 const Moves &moves, quint32 nmoves)
                : ThreadWorker(),
                  simsystem(checkpoint),
                  fast_moves(moves), nfast(nmoves)
{}

MTSMC_BG_Runner::~MTSMC_BG_Runner()
{
    qDebug() << "DESTRUCTOR!!!";

    mutex.lock();
    nfast = 0;
    mutex.unlock();

    this->waitUntilReady();
}

void MTSMC_BG_Runner::start()
{
    this->runBG();
}

void MTSMC_BG_Runner::stop()
{
    mutex.lock();
    nfast = 0;
    mutex.unlock();

    this->waitUntilReady();
}

void MTSMC_BG_Runner::wait()
{
    this->waitUntilReady();
}

CheckPoint MTSMC_BG_Runner::checkPoint()
{
    QMutexLocker lkr(&mutex);
    return simsystem.checkPoint();
}

Moves MTSMC_BG_Runner::moves()
{
    QMutexLocker lkr(&mutex);
    return fast_moves;
}

void MTSMC_BG_Runner::calculate()
{
    if (nfast == 0)
        return;

    try
    {
        mutex.lock();

        while (nfast > 0)
        {
            //run the moves in blocks of 50
            if (nfast > 50)
            {
                fast_moves.run(simsystem, 50, false);
                nfast -= 50;
            }
            else
            {
                fast_moves.run(simsystem, nfast, false);
                nfast = 0;
            }

            //give someone waiting the chance to grab hold of the mutex
            //and abort the simulation
            mutex.unlock();

            mutex.lock();
        }

        mutex.unlock();
    }
    catch(...)
    {
        mutex.unlock();
        throw;
    }
}

quint32 MTSMC::parallelMove(SimSystem &system, quint32 nmoves)
{
    qDebug() << "Calculating the energy of the initial configuration....";

    //get the old energy on the slow forcefield
    double V_old = energy(system);   // This is the energy V

    // V2 = V - V1
    double V2_old = V_old - system.energy(fast_component);

    qDebug() << "Complete - checkpointing...";

    //checkpoint the system
    CheckPoint checkpoint = system.checkPoint();
    Moves old_moves = fast_moves;

    //ok, we accept the first configuration, so lets
    //generate the next configuration using a block of fast moves...
    try
    {
        qDebug() << "Running the next block of moves...";
        //perform the moves using the fast forcefield
        fast_moves.run(system, nfast);

        qDebug() << "Complete!";

        while (nmoves > 0)
        {
            qDebug() << "Loop, nmoves ==" << nmoves;

            //ok, now that we have the next configuration, checkpoint it,
            //and then set another block of fast moves running while we work
            //out whether or not to accept it
            double mid_fast_energy = system.energy(fast_component);

            CheckPoint mid_checkpoint = system.checkPoint();
            Moves mid_moves = fast_moves;

            MTSMC_BG_Runner runner(mid_checkpoint, fast_moves, nfast);

            if (nmoves > 1)
            {
                qDebug() << "Running the next block of moves in the background...";
                //only generate the next step if there is a next step!
                runner.start();
            }

            //ok, now we can calculate the energy of this midpointsystem
            qDebug() << "Calculating the energy of this block!";
            ForceFields ffields = mid_checkpoint.forceFields();
            double V_new = ffields.energy(energyComponent());

            double V2_new = V_new - mid_fast_energy;

            if (this->test(V2_new, V2_old))
            {
                qDebug() << "This block passed!";

                if (nmoves > 1)
                {
                    //the move has passed

                    //now wait for the next block of sampling to complete
                    qDebug() << "Waiting for the next block to complete...";
                    runner.wait();
                    qDebug() << "The next block is complete!";

                    //accumulate the statistics for the current system and retake a checkpoint
                    //qDebug() << "***ACCUMULATING STATISTICS***";

                    //system.commit();
                    checkpoint = system.checkPoint();

                    //update the system to the new configuration
                    qDebug() << "Updating to the next new configuration...";
                    system.rollBack(runner.checkPoint());
                    fast_moves = runner.moves();
                }

                --nmoves;
            }
            else
            {
                qDebug() << "The move failed :-(";

                if (nmoves > 1)
                {
                    qDebug() << "Stopping the generation of the next block...";
                    //we might as well stop the generation of the next configuration
                    runner.wait();
                    qDebug() << "...it's been stopped!";
                }

                //the move has been rejected - reset the state (do this after waiting
                //to avoid a segfault)
                system.rollBack(checkpoint);

                //recount the statistics for this old configuration
                //qDebug() << "***ACCUMULATING STATISTICS***";
                //system.commit();

                //reset the moves so that they contain the statistics only up
                //to the generation of this failed configuration
                fast_moves = mid_moves;

                return nmoves - 1;
            }

            qDebug() << "END OF THE LOOP";
        }

        return 0;
    }
    catch(...)
    {
        //restore the system and the moves
        system.rollBack(checkpoint);
        fast_moves = old_moves;

        throw;
    }
}

/** Perform a single MTSMC move on the passed system */
void MTSMC::move(SimSystem &system)
{
    qDebug() << "Running moves..." << nslow;

    if (nslow > 1)
    {
        quint32 nremaining = nslow;

        while (nremaining > 0)
        {
            qDebug() << "calling this->parallelMove(" << nremaining << ")";
            nremaining = this->parallelMove(system, nremaining);
        }

        return;
    }

    // MTSMC algorithm
    //
    // Performs boltzmann sampling on potential V == V1 + V2
    //
    // (1) Start at configuration x_old
    // (2) Evaluate V2(x_old)
    // (3) Set y_old = x_old
    // (4) Perform n steps from y_old to y_new using only potential V1 - reach y_new
    // (5) Set x_new = y_new
    // (6) Accept x_old -> x_new using min[ 1, exp(-beta * {V2(x_new)-V2(x_old)}) ]

    //get the old energy on the slow forcefield
    double V_old = energy(system);   // This is the energy V

    // V2 = V - V1
    double V2_old = V_old - system.energy(fast_component);

    //checkpoint the system
    CheckPoint checkpoint = system.checkPoint();
    Moves old_moves = fast_moves;

    try
    {
        //perform the moves using the fast forcefield
        fast_moves.run(system, nfast);

        //get the new energy after the moves
        double V_new = energy(system);
        double V2_new = V_new - system.energy(fast_component);

        if (not this->test(V2_new, V2_old))
        {
            //the move has been rejected - reset the state,
            //but don't restore the moves, as I don't want to
            //lose their statistics
            system.rollBack(checkpoint);
        }
    }
    catch(...)
    {
        //restore the system and the moves
        system.rollBack(checkpoint);
        fast_moves = old_moves;
        throw;
    }
}
