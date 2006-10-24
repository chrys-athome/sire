

void InterCLJFF::calculateEnergy(const CoordGroup &group0, 
                                 const QVector<CLJParameter> &clj0,
                                 const CoordGroup &group1,
                                 const QVector<CLJParameter> &clj1,
                                 double &cnrg, double &ljnrg)
{
    if (space.withinCutoff(group0,group1,distmat))
    {
        this->combineParameters(clj0,clj1,cljmat);
    
        CLJFF::calculateEnergy(distmat, cljmat, cnrg, ljnrg);
    }
}

void InterCLJFF::calculateEnergy(const MolCLJInfo &mol0,
                                 const MolCLJInfo &mol1,
                                 double &cnrg, double &ljnrg)
{
    cnrg = 0.0;
    ljnrg = 0.0;

    int ncg0 = mol0.coords.count();
    int ncg1 = mol1.coords.count();
    
    CoordGroup *cg0array = mol0.coords.constData();
    CoordGroup *cg1array = mol1.coords.constData();
    
    QVector<CLJParameter> *clj0array = mol0.params.constData();
    QVector<CLJParameter> *clj1array = mol1.params.constData();
    
    double sclfac;
    
    for (int i=0; i<ncg0; ++i)
    {
        const CoordGroup &group0 = cg0array[i];
        const QVector<CLJParameter> &clj0 = clj0array[i];
    
        for (int j=0; j<ncg1; ++j)
        {
            const CoordGroup &group1 = cg1array[j];
            const QVector<CLJParameter> &clj1 = clj1array[j];
        
            double icnrg,iljnrg;
            
            this->calculateEnergy(group0,clj0, group1,clj1, icnrg,iljnrg);
            
            cnrg += icnrg;
            iljnrg += iljnrg;
        }
    }
}

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
