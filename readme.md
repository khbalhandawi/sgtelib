# Build and install commands

This is just for SGTELIB if built in and by itself.

We start with:

```bash
mkdir build
cd build
```

In release mode (Linux/Windows):

```bash
cmake -S .. -B Release -DCMAKE_BUILD_TYPE=Release
cmake --build Release --config Release
cmake --install Release --config Release
```

In debug mode:

```bash
cmake -S .. -B Debug -DCMAKE_BUILD_TYPE=Debug
cmake --build Debug --config Debug
cmake --install Debug --config Debug
```

Mostly, the library is only built as part of another project such as fSGTELIB or cSGTELIB.
Testing is handled in cSGTELIB.

# Building the test executable

To build the executable use the following cmake commands

In release mode (Linux/Windows):

```bash
cmake -S .. -B Release -DCMAKE_BUILD_TYPE=Release -DBUILD_MAIN_EXECUTABLE=ON
```

In debug mode:

```bash
cmake -S .. -B Debug -DCMAKE_BUILD_TYPE=Debug -DBUILD_MAIN_EXECUTABLE=ON
```