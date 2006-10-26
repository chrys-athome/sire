

void InterCLJFF::recalculateEnergy()
{
    //calculate the total CLJ energy of all molecule pairs...
    double cnrg = 0.0;
    double ljnrg = 0.0;

    int nmols = mols.count();

    const MolCLJInfo *molarray = mols.constData();

    //loop over all molecule pairs
    for (int i=0; i<nmols-1; ++i)
    {
        const MolCLJInfo &mol0 = molarray[i];

        for (int j=i+1; j<nmols; ++j)
        {
            const MolCLJInfo &mol1 = molarray[j];

            double icnrg,iljnrg;

            this->calculateEnergy(mol0,mol1,icnrg,iljnrg);

            cnrg += icnrg;
            ljnrg += iljnrg;
        }
    }

    this->setComponent(ecoulomb, cnrg);
    this->setComponent(elj, ljnrg);
    this->setComponent(etotal, cnrg+ljnrg);
}
