# 3d-chair
OpenGL implementation of 3D chair in c++ using GLFW, Glad and GLM. This is one of my college project for computer graphics. I just wanted to share the project
to help anyone learning graphics. This project was done long time ago and I have forgot many stuffs about graphics. So I might be unable to help anyone who face
issues while compiling and running this project. I myself spent a whole day setting this project up and running and I can tell you it wasn't fun.

### Install
I have used cmake to generate the build and make to build the project. First you have to install [cmake](https://cmake.org/download/) and [make](https://www.gnu.org/software/make/).
If you are using linux you can use your package manager to install the tools. Most probably it would be installed by default.

### Compiling and Running
I will be using linux for other operation system please google search.

Clone the project and change directory to `build`
```
cd build
```

Run `cmake` to generate the build and then use `make` to compile the project
```
# cmake to build the project build
cmake ..
# compile project
make
```

If the compilation is successful, then run the generated executable
```
./3DChair
```


