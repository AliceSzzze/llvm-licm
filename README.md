# llvm-licm

Our implementation of the Loop-invariant code motion pass. 

Build:

    $ cd llvm-pass-skeleton
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make
    $ cd ..

Run:
    $ clang -fpass-plugin=`echo build/licm/LICMPass.*` something.c

