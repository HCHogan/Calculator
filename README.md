# Matrix Calculator

A simple matrix calculator written in modern c++.

Only tested under homebrew clang 17.0.4

## Build Steps:

First, you need to install llvm and cmake using homebrew,
don't use the default clang because it's too old.
```bash
brew install llvm cmake
```

Next, build the project:
```bash
git clone git@github.com:HCHogan/Calculator.git
cd Calculator
mkdir build
cd build
cmake -DCMAKE_CXX_COMPILER=/opt/homebrew/opt/llvm/bin/clang++ ..
make -j8
```

Finally, run the calculator:
```bash
./matrix_calc_test
```
