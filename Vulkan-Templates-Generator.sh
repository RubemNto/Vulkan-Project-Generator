#!/bin/bash
clear

echo "

██╗░░░██╗██╗░░░██╗██╗░░░░░██╗░░██╗░█████╗░███╗░░██╗
██║░░░██║██║░░░██║██║░░░░░██║░██╔╝██╔══██╗████╗░██║
╚██╗░██╔╝██║░░░██║██║░░░░░█████═╝░███████║██╔██╗██║
░╚████╔╝░██║░░░██║██║░░░░░██╔═██╗░██╔══██║██║╚████║
░░╚██╔╝░░╚██████╔╝███████╗██║░╚██╗██║░░██║██║░╚███║
░░░╚═╝░░░░╚═════╝░╚══════╝╚═╝░░╚═╝╚═╝░░╚═╝╚═╝░░╚══╝

██████╗░██████╗░░█████╗░░░░░░██╗███████╗░█████╗░████████╗
██╔══██╗██╔══██╗██╔══██╗░░░░░██║██╔════╝██╔══██╗╚══██╔══╝
██████╔╝██████╔╝██║░░██║░░░░░██║█████╗░░██║░░╚═╝░░░██║░░░
██╔═══╝░██╔══██╗██║░░██║██╗░░██║██╔══╝░░██║░░██╗░░░██║░░░
██║░░░░░██║░░██║╚█████╔╝╚█████╔╝███████╗╚█████╔╝░░░██║░░░
╚═╝░░░░░╚═╝░░╚═╝░╚════╝░░╚════╝░╚══════╝░╚════╝░░░░╚═╝░░░

░██████╗░███████╗███╗░░██╗███████╗██████╗░░█████╗░████████╗░█████╗░██████╗░
██╔════╝░██╔════╝████╗░██║██╔════╝██╔══██╗██╔══██╗╚══██╔══╝██╔══██╗██╔══██╗
██║░░██╗░█████╗░░██╔██╗██║█████╗░░██████╔╝███████║░░░██║░░░██║░░██║██████╔╝
██║░░╚██╗██╔══╝░░██║╚████║██╔══╝░░██╔══██╗██╔══██║░░░██║░░░██║░░██║██╔══██╗
╚██████╔╝███████╗██║░╚███║███████╗██║░░██║██║░░██║░░░██║░░░╚█████╔╝██║░░██║
░╚═════╝░╚══════╝╚═╝░░╚══╝╚══════╝╚═╝░░╚═╝╚═╝░░╚═╝░░░╚═╝░░░░╚════╝░╚═╝░░╚═╝"

echo "
* - * - * - * - * - * - * - * - *
| PROJECTS MENU                 |
*                               *
| (1) - CONSOLE VULKAN PROGRAM  |
*                               *
| (0) - EXIT                    |
* - * - * - * - * - * - * - * - *
"

read -p "Choose an option: " option

if [ $option != '1' ]; 
	then
		echo "Exiting Generator"
		exit 0
	else 
		read -p "Choose a Host Directory: " hostDir
		read -p "Project name: " projectName
		mkdir -p $hostDir/$projectName
		cd $hostDir/$projectName
		mkdir build
		mkdir src
		mkdir header
		touch Makefile
		echo 'CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi
all:
	g++ $(CFLAGS) -o build/VulkanTest.exe src/*.cpp $(LDFLAGS) 
	./build/VulkanTest.exe' >> Makefile
		
		touch src/main.cpp src/program.cpp header/program.hpp
		
		if [ $option == '1' ];
		then
			echo 'Generating Console Program'
			echo '#include <vulkan/vulkan.h>

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>

class Program {
public:
  void run();

private:
  void cleanup();
};' >> header/program.hpp
		echo '#include "../header/program.hpp"

void Program::run() 
{
    std::cout<<"Hello Vulkan-Console-Program!"<<std::endl;
    cleanup();
}

void Program::cleanup() {}' >> src/program.cpp
		echo '#include "../header/program.hpp"

int main() {
  Program app;

  try 
  {
    app.run();
  } catch (const std::exception &e) 
  {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}' >> src/main.cpp
fi
fi
