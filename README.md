# Water Shader

A real-time water simulation demo built with OpenGL, using tessellation and Gerstner wave modeling to create a dynamic, animated water surface. This project leverages advanced shaders to simulate wave motion, lighting, and texture-based reflections, offering interactive 3D camera controls for user exploration.

## Preview

![Water Shader](WaterShader.gif)

## Features

- Animated wave simulation using Gerstner wave equations
- Tessellation Control and Evaluation Shaders for mesh subdivision
- Geometry Shader displaces mesh with wave height and displacement map
- Realistic lighting using a Phong-inspired reflection model
- Textured surface with dynamic specular and normal-based highlights
- Interactive globe-style camera: click & drag to rotate, arrow keys to zoom

## Environment Setup

Follow these steps to configure your environment on Windows using MSYS2:

1. [**Install MSYS2**](https://www.msys2.org/) and install it in `C:\msys64`

2. **Open the MSYS2 MINGW64 terminal**, then run:

   ```bash
   pacman -Syu
   ```

   _(The terminal will close after updating. Reopen the MINGW64 terminal to continue.)_

3. **Install required packages**:

   ```bash
   pacman -S mingw-w64-x86_64-toolchain \
               mingw-w64-x86_64-freeglut \
               mingw-w64-x86_64-glew \
               mingw-w64-x86_64-glfw \
               mingw-w64-x86_64-glm
   ```

4. **Update your PATH**  
   Add the following to your user environment variables:
   ```
   C:\msys64\mingw64\bin
   ```

## Build and Run

1. Clone the repository
   ```bash
   git clone https://github.com/NicoM-7/water-shader.git
   cd water-shader
   ```
2. Open `WaterShader.cpp` in [Visual Studio Code](https://code.visualstudio.com/)
3. From the top menu, go to: `Terminal` → `Run Build Task`
4. Locate and double-click the built executable (`waterShader.exe`) to launch the simulation
