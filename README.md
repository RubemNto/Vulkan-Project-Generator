# Vulkan-Project-Generator
Simple CLI for generating Vulkan Projects with BASH-Script

# Prerequisites<br>
* GLFW Library
  ```
  #Debian
  sudo apt install libglfw3-dev
  
  #Fedora
  sudo dnf install glfw-devel
  
  #Arch
  sudo pacman -S glfw-wayland # glfw-x11 for X11 users
  ```
* GLM Library
  ```
  #Debian
  sudo apt install libglm-dev
  
  #Fedora
  sudo dnf install glm-devel
  
  #Arch
  sudo pacman -S glm
  ```
* GLSLC GLSL/HLSL to SPIR-V compiler
  ```
  #Debian
  download Google's unofficial binaries and copy glslc to your /usr/local/bin
  link to binaries => https://github.com/google/shaderc/blob/main/downloads.md
  
  #Fedora
  sudo dnf install glslc
  
  #Arch
  sudo pacman -S shaderc
  ```
* VULKAN SDK and Tools
  ```
  #Debian
  sudo apt install vulkan-tools
  sudo apt install libvulkan-dev
  sudo apt install vulkan-validationlayers-dev spirv-tools
  
  #Fedora
  sudo dnf install vulkan-tools
  sudo dnf install vulkan-loader-devel
  sudo dnf install mesa-vulkan-devel vulkan-validation-layers-devel
  
  #Arch
  sudo pacman -S vulkan-devel
  ```
Steps
1) Clone repository
   ```
   git clone https://github.com/RubemNto/Vulkan-Project-Generator.git
   ```
2) Execute Vulkan-Templates-Generator.sh in a Terminal Window
    ```
    chmod -x Vulkan-Templates-Generator.sh
    bash ./Vulkan-Templates-Generator.sh
    ```
    2.1) Choose Menu Option
  
    2.2) Define Project Host Directory 
  
    2.3) Define Project Name

3) Generate Project
