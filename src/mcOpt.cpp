#define HAVE_EIGEN


#include <stdio.h>

#include <iostream>
//#include <sstream>
#include <vector>
#include <math.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <random>
#include <getopt.h>

#include <openbabel/babelconfig.h>
#include <openbabel/base.h>
#include <openbabel/mol.h>
#include <openbabel/obconversion.h>
#include <openbabel/forcefield.h>
#include <openbabel/obutil.h>

#include "utils.hpp"


int main(int argc, char *argv[]) {

  std::cout << "\n[-] Optimize Molecule" << std::endl;

  McOpt::Option opts = McOpt::get_options(argc, argv);

  OpenBabel::OBMol mol = McOpt::readfile(opts.molecule);

  double energy;
  energy = McOpt::optimize_molecule(mol);

  McOpt::writefile(mol, opts.fout);

  std::cout << "[+] Optimize Molecule, Energy = " << energy << "\n" << std::endl;

}


