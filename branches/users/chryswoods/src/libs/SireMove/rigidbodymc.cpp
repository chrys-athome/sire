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

#include "rigidbodymc.h"

using namespace SireMove;

// void RigidBodyMC::move(SimSystem &system)
// {
//     //update the sampler for this simulation
//     smplr.update(system);
// 
//     //get the current total energy of the system
//     double old_nrg = energy(system);
// 
//     //checkpoint the simulation
//     RigidBodyMC::CheckPoint ckpt = checkpoint();
// 
//     try
//     {
//         //randomly select a molecule to move
//         tuple<Molecule,double> mol_and_bias = sampler().randomMolecule();
// 
//         const Molecule &movemol = mol_and_bias.get<0>();
//         double oldbias = mol_and_bias.get<1>();
// 
//         //now translate and rotate the molecule
//         Vector delta = randomVectorOnSphere( maxTranDelta(movemol) );
// 
//         Quaternion rotdelta( this->maxRotDelta(movemol) * this->rand(),
//                              SireMaths::randomVectorOnSphere(1) );
// 
//         movemol.translateAndRotate(delta, rotdelta,
//                                    this->centerOfRotation(movemol));
// 
//         //update the simulation with the new coordinates
//         simulation.change(movemol);
// 
//         //calculate the energy of the system
//         double new_nrg = this->energy(simulation);
// 
//         //get the new bias on this molecule
//         double newbias = sampler.probability(simulation, movemol);
// 
//         //accept or reject the move based on the change of energy
//         //and the biasing factors
//         if (not this->accept(new_nrg, old_nrg, new_bias, old_bias))
//         {
//             //the move has been rejected - reset the state
//             rollback(ckpt);
//         }
//     }
//     catch(...)
//     {
//         //rollback to before the move
//         rollback(ckpt);
// 
//         //rethrow the exception
//         throw;
//     }
// }
