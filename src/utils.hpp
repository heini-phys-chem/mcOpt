#ifndef SRC_UTILS_HPP
#define SRC_UTILS_HPP
#define HAVE_EIGEN   

namespace McOpt {


struct Option {
  std::string molecule;
  std::string fout;
};

Option get_options (int argc, char **argv) {
  int c;

  Option opts;

  while (1) {
    static struct option long_options[] = {
      {"molecule", required_argument, 0, 'a'},
      {"fout", required_argument, 0, 'b'}
    };

    int option_index = 0;

    c = getopt_long(argc, argv, "a:b:", long_options, &option_index);

    if (c == -1)
      break;

    switch (c) {

      case 0:
        if (long_options[option_index].flag != 0) break;
      break;

      case 'a':
        opts.molecule = optarg;
      case 'b':
        opts.fout = optarg;
    }
  }

  if (optind < argc) {
    printf("non-option ARGV-elements: ");
    while (optind < argc)
      printf("%s ", argv[optind++]);
    putchar ('\n');
  }

  if (opts.molecule.compare("") == 0) {
    printf("ERROR: no molecule specified.\n");
    exit(0);
  }
  if (opts.fout.compare("") == 0) {
    printf("ERROR: no output filename specified.\n");
    exit(0);
  }

  return opts;

}

double optimize_molecule(OpenBabel::OBMol &mol) {

  OpenBabel::OBForceField* pFF = OpenBabel::OBForceField::FindForceField("UFF");
  pFF->Setup(mol);

  double e = pFF->Energy();

  const int steps = 200;
  const double crit = 5.0e-4;

  pFF->ConjugateGradientsInitialize(steps, crit);

  bool done = true;

  while (done) {
    done = pFF->ConjugateGradientsTakeNSteps(1);

    if (pFF->DetectExplosion()) {
      std::cerr << "explosion has occured!" << std::endl;
      exit(1);
    } else {
      pFF->GetCoordinates(mol);
    }
  }

  e = pFF->Energy();
  mol.SetEnergy(e);

  return e;

}

OpenBabel::OBMol readfile(std::string filename) {

  OpenBabel::OBMol mol;

  OpenBabel::OBConversion conv;
  conv.SetInAndOutFormats("sdf", "sdf");

  std::ifstream ifs;

  ifs.open(filename.c_str());
  conv.Read(&mol, &ifs);
  ifs.close();

  return mol;
}

void writefile(OpenBabel::OBMol mol, std::string filename) {

  OpenBabel::OBConversion conv;
  conv.SetInAndOutFormats("xyz", "xyz");

  std::ofstream ofs(filename);

  conv.Write(&mol, &ofs);
  ofs.close();
}



}
#endif
