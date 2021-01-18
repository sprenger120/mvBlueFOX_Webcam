#include <iostream>
#include <unistd.h>

#include "CameraManager.hpp"

using namespace std;

int main() {
    webcam::Driver::CameraManager mgr;

  bool shouldRun = true;
  while (shouldRun) {
    usleep(10'000);
  }

  return 0;
}