
params = db.assign( mol, [assign_atoms( atms, using_dbs(ChargeDB,LJDB) ),
                          assign_bonds( bonds, using_dbs(BondDB) ),
                          using_relationships(RelateMRADB),
                          assign_dihedrals( dihs, using_dbs(DihedralDB),
                                                  using_relationships(RelateAtomTypeDB) )
                         ]
                  )

params = db.assign_parameters( mol, assign_atoms( AtomIndexSet(),
                                                  from<ChargeDB>(),
                                                  from<AtomTypeDB>(),
                                                  from<LJDB>(),
                                                  using<RelateMRADB>(),
                                                  using<RelateAtomTypeDB>() ),
                                    
                                    assign_bonds( BondSet(),
                                                  from<BondDB>(),
                                                  using<RelateMRADB>() ),
                                               
                                    assign_angles( AngleSet(),
                                                   from< sum_of<AngleDB,UreyBradleyDB> >(),
                                                   using<RelateMRADB>() ),
                                                  
                                    assign_dihedrals( DihedralSet(),
                                                      from<DihedralDB>(),
                                                      using<RelateMRADB>() )
                             )

params = db.assignParameters( mol, assign_atoms( AtomIndexSet(),
                                                 params_from(ChargeDB(),LJDB(),AtomTypeDB()),
                                                 using_relationships( ) ...
                                                 
                                             
params = ParameterTable(mol);

params.assign( db, assign_atoms( AtomIndexSet(),
                                 [using<ChargeDB>(), using<LJDB>(), using<AtomTypeDB>()],
                                 [relationships<RelateMRADB>(), relationships<RelateAtomTypeDB>()] )

                   assign_bonds( BondSet(),
                                 params_from(BondDB()),
                                 using_relationships(RelateMRADB()),
                                 using_relationships(RelateAtomTypeDB()) ),
                                 
                   assign_angles( AngleSet(),
                                  [using_sum<AngleDB,UreyBradleyDB>()]
                                  using_relationships(RelateMRADB()),
                                  using_relationships(RelateAtomTypeDB()) ),
                                  
                   assign_dihedrals( DihedralSet(), 
                                     params_from(DihedralDB()),
                                     using_relationships(RelateMRADB()),
                                     using_relationships(RelateAtomTypeDB()) ) );
                   
                                                 
class ParameterTable
{
public:

    template<class T>
    bool contains(const QString&) const;

    AtomTables& atoms() const;

private:
    QHash<QString, TablePtr> tables;
};

ChargeTable charges = params["charge"];

params["charge"].asA<ChargeTable>().setParameter( ("O00",1), 0.15 );

params["charge"].asA<ChargeTable>().setCharge( ("O00",1), 0.15 );


in python..

params["charge"].setCharge( ("O00",1), 0.15 )
params["bond"].setParameter( (.....) )
