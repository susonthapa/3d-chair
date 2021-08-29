# 3d-chair
OpenGL implementation of 3D chair in c++ using GLFW, Glad and GLM. This is one of my college project for computer graphics. I just wanted to share this project
to help anyone learning graphics. This project was done long time ago and I have forgot many stuffs about graphics. So I might be unable to help anyone who face
issues while compiling and running this project. I myself spent a whole day setting this project up and running and I can tell you it wasn't fun. 

### Demo
![3d-chair](https://user-images.githubusercontent.com/33973551/131256160-b72c8f57-1713-4a6c-9c2a-a46752c52011.gif)

### Install
I have used **cmake** to generate the build and **make** to build the project. First you have to install [cmake](https://cmake.org/download/) and [make](https://www.gnu.org/software/make/).
If you are using linux you can use your package manager to install the tools. Most probably it would be installed by default.

### Compiling and Running
I will be using linux, for other operation system please google search for equivalent operations.

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
### Concept
If the compilation is successful, then run the generated executable
```
./3DChair
```

The chair is rendered using concept of cube. It consists of 20 cubes which are generated programmatically. 
![image](https://user-images.githubusercontent.com/33973551/131254803-805d4a65-090b-4a3d-8c28-20826e9a963b.png)

In the figure above leg with `1` is a cube with small length and breadth but large height. And the arm rest with `2` is another cube with small length and height
but large breadth.

This project uses `vertex_shader.glsl` for computing the position of the chair. The light effects are computed using `fragment_shader.glsl`.

### Implemented operations
This project implements the phong model for illuminations. You can look into `fragment_shader.glsl` to understand how specular, diffuse and ambient light reflections are computed. 

The project supports basic inputs from keyboard to manipulate the chair. The keys and their functions are given below.

| Keys | Operation |
| ---- | ---- |
| LEFT and RIGHT | Rotation about X-axis |
| UP and DOWN | Rotation about Z-axis |
| W and S | Translation about Z-axis |
| A and D | Translation about X-axis |
| + and - | Ambient light control |
