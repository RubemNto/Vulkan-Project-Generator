#include "../header/presentation.hpp"
#include "../header/setup.hpp"

class Program {
public:
  void run();

private:
  Setup setup;
  Presentation presentation;
  void cleanup();
};
