# Pong++

Pong in C++ using Raylib
This is a training project for me to learn C++ and game dev

# Build

The project is setup for Visual Studio 2022
There are 2 absolute paths that need to point to the build output of raylib (static linking)

```
Project Configuration > C/C++ > General > Additional Include Directories
Project Configuration > Linker > General > Additional Library Directories
```

The rest *should* work, unless you're not on Windows, in which case I hope you know more than me about C++ project building on Mac/Linux