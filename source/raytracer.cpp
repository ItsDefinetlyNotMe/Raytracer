#include "SDFreader.hpp"
#include "Scene.hpp"

#include <renderer.hpp>
#include <window.hpp>

#include <GLFW/glfw3.h>
#include <thread>
#include <utility>
#include <cmath>

//now single threaded again
/*
int main(int argc, char* argv[])
{
  unsigned const image_width = 800;
  unsigned const image_height = 600;
  std::string const filename = "./checkerboard.ppm";

  Renderer renderer{ image_width, image_height, filename };
  renderer.render();

  Window window{{image_width, image_height}};

  while (!window.should_close()) {
    if (window.get_key(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      window.close();
    }
    window.show(renderer.color_buffer());
  }

  return 0;
}*/
int main(int argc, char* argv[]) {
    unsigned const image_width = 800;//480;
    unsigned const image_height = 600;//320;

    std::string const filename = "./../../../../source/examplescene.sdf";
    auto s = sdf_reader(filename);
    s->render();

    Window window{ {image_width, image_height} };

    while (!window.should_close()) {
        if (window.get_key(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            window.close();
        }
        auto b = s->color_buffer();
        window.show(s->color_buffer());
    }

    return 0;
}

