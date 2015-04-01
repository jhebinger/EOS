# Credits

The original project, authored by Christian Wacker, Danny van Dyk, Frederik Beaujean, and Ismo Toijala, can be found at http://project.het.physik.tu-dortmund.de/source/eos. This repository is a fork of it setup for convenience.


# How to build

At the moment, this can only build on Linux using the g++ compiler.

Here is one way to build EOS in the source directory. From a directory of your choosing, issue the following commands. I am assuming a machine with 4 cores. Hence the `-j4` options passed to `make`. Please adjust for your machine.
```bash
~% wget http://www.cern.ch/mathlibs/sw/5_34_14/Minuit2/Minuit2-5.34.14.tar.gz
~% tar xfz Minuit2-5.34.14.tar.gz
~% cd Minuit2-5.34.14/
~% ./configure
~% make -j4
~% sudo make install
~% cd ..
~% git clone http://project.het.physik.tu-dortmund.de/source/eos
~% cd eos
 ~% sudo yum install libtool gsl-devel hdf5-devel
~% ./autogen.bash
~% CXXFLAGS=-fopenmp ./configure
~% make -j4
```

# Documentation

All classes are documented with Doxygen. The documentation can be generated from the top directory with
```bash
make doxygen
```

Then open `doc/html/index.html`.