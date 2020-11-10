#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <gmpxx.h>

// counting two-step paths in the quarter-plane
// this will output separate files for all walks, walks ending on x-axis, walks ending on y-axis, and walks ending at origin

// USAGE
// ./a.out T nmax allfile xfile yfile ofile
// where T is the 16-digit bitstring for the matrix
// nmax is the number of series terms
// allfile (xfile, yfile, ofile) is the name of the file for walks ending anywhere (x-axis, y-axis, origin)

class ValMap {
public:
  std::vector<std::map<int, std::map<int, std::map<int, mpz_class>>>> data;

  ValMap () {
    data.assign(2, std::map<int, std::map<int, std::map<int, mpz_class>>> ());
  }

  void add (int const id, int const x, int const y, int const d, mpz_class const v) {
    data[id][x][y][d] += v;
  }

  void clear (int const id) {
    data[id].clear();
  }

};

void count (int const maxlen, std::vector<std::vector<int>> const & T, ValMap & data, std::vector<mpz_class> & outall, std::vector<mpz_class> & outx, std::vector<mpz_class> & outy, std::vector<mpz_class> & outo) {
  int id = 1;
  int idn = 0;
  data.add(1, 1, 0, 0, 1); // initial east step
  data.add(1, 0, 1, 1, 1); // initial north step
  for (int n = 1; n <= maxlen; ++n) {
    for (auto const & xv : data.data[id]) {
      for (auto const & yv : xv.second) {
        for (auto const & dv : yv.second) {
          if (n < maxlen) {
            // step east if possible
            if (T[dv.first][0]) data.add(idn, xv.first+1, yv.first, 0, dv.second);
            // step north if possible
            if (T[dv.first][1]) data.add(idn, xv.first, yv.first+1, 1, dv.second);
            // step west if possible
            if (xv.first > 0 && T[dv.first][2]) data.add(idn, xv.first-1, yv.first, 2, dv.second);
            // step south if possible
            if (yv.first > 0 && T[dv.first][3]) data.add(idn, xv.first, yv.first-1, 3, dv.second);
          }
          // dump the data
          outall[n] += dv.second;
          if (yv.first == 0) outx[n] += dv.second;
          if (xv.first == 0) outy[n] += dv.second;
          if (xv.first == 0 && yv.first == 0) outo[n] += dv.second;
        }
      }
    }
    // then clear the stuff we just used
    data.clear(id);
    // and switch id and idn
    if (id == 0) { id = 1; idn = 0; }
    else { id = 0; idn = 1; }
    // std::cout << n << std::endl;
  }
}

int main (int argc, char* argv[]) {
  int maxlen = std::stoi(argv[1]);
  std::vector<std::vector<int>> T (4, std::vector<int> (4, 0));
  char* Tstring = argv[2];
  for (int i = 0; i < 16; ++i) {
    T[i/4][i%4] = Tstring[i] - '0';
  }
  ValMap data;
  std::vector<mpz_class> outall (maxlen+1, 0);
  std::vector<mpz_class> outx (maxlen+1, 0);
  std::vector<mpz_class> outy (maxlen+1, 0);
  std::vector<mpz_class> outo (maxlen+1, 0);
  count (maxlen, T, data, outall, outx, outy, outo);
  std::ofstream outallfs (argv[3], std::ofstream::out);
  std::ofstream outxfs (argv[4], std::ofstream::out);
  std::ofstream outyfs (argv[5], std::ofstream::out);
  std::ofstream outofs (argv[6], std::ofstream::out);
  for (int n = 0; n <=maxlen; ++n) {
    outallfs << outall[n] << std::endl;
    outxfs << outx[n] << std::endl;
    outyfs << outy[n] << std::endl;
    outofs << outo[n] << std::endl;
  }
  outallfs.close();
  outxfs.close();
  outyfs.close();
  outofs.close();
  return 0;
}
