#include <vulkan/vulkan.h>

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>

class Program {
public:
  void run();

private:
  void cleanup();
};
