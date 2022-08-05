#include <renderer.hpp>
#include <window.hpp>

#include <GLFW/glfw3.h>
#include <thread>
#include <utility>
#include <cmath>
#include <vector>

#include "Sphere.hpp"
#include "Box.hpp"

//now single threaded again
int main(int argc, char* argv[])
{
  unsigned const image_width = 800;
  unsigned const image_height = 600;
  std::string const filename = "./raycaster_test.ppm";

  std::vector<std::shared_ptr<Shape>> scene_mockup {std::make_shared<Box>(Box{{-20.0f, -15.0f, -20.0f}, {2.0f, -5.0f, -15.0f}}),
                                                    std::make_shared<Sphere>(Sphere{{2.0f, 0.0f, -8.0f}, 2.0f}),
                                                    std::make_shared<Sphere>(Sphere{{15.0f, -5.0f, -30.0f}, 15.0f})};

  Renderer renderer{image_width, image_height, filename, scene_mockup};

  float fov = 90.0f;

  renderer.render(fov);

  Window window{{image_width, image_height}};

  while (!window.should_close()) {
    if (window.get_key(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      window.close();
    }
    if (window.get_key(GLFW_KEY_A) == GLFW_PRESS) {
      fov += 15;
      std::cout<<fov<<" "<<fov * (3.1415f / 180.0f)<<" "<<tanf((fov * (3.1415f / 180.0f))/2.0f)<<std::endl;
      renderer.render(fov);
    }
    if (window.get_key(GLFW_KEY_S) == GLFW_PRESS) {
      fov -= 15;
      std::cout<<fov<<" "<<fov * (3.1415f / 180.0f)<<" "<<tanf((fov * (3.1415f / 180.0f))/2.0f)<<std::endl;
      renderer.render(fov);
    }

    window.show(renderer.color_buffer());
  }

  return 0;
}
