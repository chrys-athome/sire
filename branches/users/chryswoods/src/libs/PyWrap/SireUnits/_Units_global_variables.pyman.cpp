
#include "_Units_global_variables.pyman.hpp"
#include <boost/python.hpp>
#include "sireunits_headers.h"

using namespace boost::python;
using namespace SireUnits;
using namespace SireUnits::Dimension;

void register_man_global_variables()
{
    scope().attr("mole") = mole;

    scope().attr("dozen") = dozen;

    scope().attr("angstrom") = angstrom;

    scope().attr("picometer") = picometer;

    scope().attr("nanometer") = nanometer;

    scope().attr("micrometer") = micrometer;

    scope().attr("millimeter") = millimeter;

    scope().attr("centimeter") = centimeter;

    scope().attr("meter") = meter;

    scope().attr("kilometer") = kilometer;

    scope().attr("bohr_radii") = bohr_radii;

    scope().attr("inch") = inch;

    scope().attr("foot") = foot;

    scope().attr("yard") = yard;

    scope().attr("mile") = mile;

    scope().attr("g_per_mol") = g_per_mol;

    scope().attr("gram") = gram;

    scope().attr("kilogram") = kilogram;

    scope().attr("tonne") = tonne;

    scope().attr("milligram") = milligram;

    scope().attr("microgram") = microgram;

    scope().attr("nanogram") = nanogram;

    scope().attr("picogram") = picogram;

    scope().attr("femtogram") = femtogram;

    scope().attr("kg_per_mol") = kg_per_mol;

    scope().attr("tonne_per_mol") = tonne_per_mol;

    scope().attr("mg_per_mol") = mg_per_mol;

    scope().attr("ug_per_mol") = ug_per_mol;

    scope().attr("ng_per_mol") = ng_per_mol;

    scope().attr("pg_per_mol") = pg_per_mol;

    scope().attr("fg_per_mol") = fg_per_mol;

    scope().attr("mod_electron") = mod_electron;

    scope().attr("faraday") = faraday;

    scope().attr("coulomb") = coulomb;

    scope().attr("coulomb_per_mol") = coulomb_per_mol;

    scope().attr("e_charge") = e_charge;

    scope().attr("kcal_per_mol") = kcal_per_mol;

    scope().attr("kcal") = kcal;

    scope().attr("cal_per_mol") = cal_per_mol;

    scope().attr("cal") = cal;

    scope().attr("kJ_per_mol") = kJ_per_mol;

    scope().attr("kilojoule") = kilojoule;

    scope().attr("MJ_per_mol") = MJ_per_mol;

    scope().attr("megajoule") = megajoule;

    scope().attr("J_per_mol") = J_per_mol;

    scope().attr("joule") = joule;

    scope().attr("int_kcal_per_mol") = int_kcal_per_mol;

    scope().attr("int_cal_per_mol") = int_cal_per_mol;

    scope().attr("int_kcal") = int_kcal;

    scope().attr("int_cal") = int_cal;

    scope().attr("hartree") = hartree;

    scope().attr("akma_time") = akma_time;

    scope().attr("second") = second;

    scope().attr("millisecond") = millisecond;

    scope().attr("microsecond") = microsecond;

    scope().attr("nanosecond") = nanosecond;

    scope().attr("picosecond") = picosecond;

    scope().attr("femtosecond") = femtosecond;

    scope().attr("minute") = minute;

    scope().attr("hour") = hour;

    scope().attr("day") = day;

    scope().attr("newton") = newton;

    scope().attr("ounce") = ounce;

    scope().attr("pound") = pound;

    scope().attr("stone") = stone;

    scope().attr("hundredweight") = hundredweight;

    scope().attr("pascal") = pascal;

    scope().attr("bar") = bar;

    scope().attr("atm") = atm;

    scope().attr("psi") = psi;

    scope().attr("mmHg") = mmHg;

    scope().attr("kelvin") = kelvin;

    scope().attr("amp") = amp;

    scope().attr("volt") = volt;

    scope().attr("farad") = farad;

    scope().attr("watt") = watt;

    scope().attr("watt_per_mol") = watt_per_mol;

    scope().attr("c") = c;

    scope().attr("g_accel") = g_accel;

    scope().attr("electron_mass") = electron_mass;

    scope().attr("proton_mass") = proton_mass;

    scope().attr("neutron_mass") = neutron_mass;

    scope().attr("atomic_mass_constant") = atomic_mass_constant;

    scope().attr("molar_volume") = molar_volume;

}

