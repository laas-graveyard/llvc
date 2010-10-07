llvc
====

llvc stands for low-level vision client. It provides a set of tools
which can start and retrieve results of different algorithm on the
vision server.

Setup
-----

To compile this package, it is recommended to create a separate build
directory:

    mkdir _build
    cd _build
    cmake [OPTIONS] ..
    make install

Please note that CMake produces a `CMakeCache.txt` file which should
be deleted to reconfigure a package from scratch.


### Dependencies

The package depends on several packages which have to be available on
your machine.

 - Libraries:
   - Boost (>= 1.34)
   - ViSP (>= 2.4.5)
   - omniORB4 (>= 4.1.2)
 - System tools:
   - CMake (>=2.6)
   - pkg-config
   - usual compilation tools (GCC/G++, make, etc.)
