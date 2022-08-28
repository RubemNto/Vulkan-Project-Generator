CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi
all:
	g++ $(CFLAGS) -o build/VulkanTest.exe src/*.cpp $(LDFLAGS) 
	./build/VulkanTest.exe
