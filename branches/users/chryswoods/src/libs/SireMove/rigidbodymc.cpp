
#include "rigidbodymc.h"

using namespace SireMove;

void RigidBodyMC::move(SimSystem &system)
{
    //update the sampler for this simulation
    sampler.update(system);

    //get the current total energy of the system
    double old_nrg = energy(system);

    //checkpoint the simulation
    RigidBodyMC::CheckPoint ckpt = checkpoint();

    try
    {
        //randomly select a molecule to move
        tuple<Molecule,double> mol_and_bias = sampler().randomMolecule();

        const Molecule &movemol = mol_and_bias.get<0>();
        double oldbias = mol_and_bias.get<1>();

        //now translate and rotate the molecule
        Vector delta = randomVectorOnSphere( maxTranDelta(movemol) );

        Quaternion rotdelta( this->maxRotDelta(movemol) * this->rand(),
                             SireMaths::randomVectorOnSphere(1) );

        movemol.translateAndRotate(delta, rotdelta,
                                   this->centerOfRotation(movemol));

        //update the simulation with the new coordinates
        simulation.change(movemol);

        //calculate the energy of the system
        double new_nrg = this->energy(simulation);

        //get the new bias on this molecule
        double newbias = sampler.probability(simulation, movemol);

        //accept or reject the move based on the change of energy
        //and the biasing factors
        if (not this->accept(new_nrg, old_nrg, new_bias, old_bias))
        {
            //the move has been rejected - reset the state
            rollback(ckpt);
        }
    }
    catch(...)
    {
        //rollback to before the move
        rollback(ckpt);

        //rethrow the exception
        throw;
    }
}
