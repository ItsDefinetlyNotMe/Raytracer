// -----------------------------------------------------------------------------
// Copyright  : (C) 2014-2017 Andreas-C. Bernstein
// License    : MIT (see the file LICENSE)
// Maintainer : Andreas-C. Bernstein <andreas.bernstein@uni-weimar.de>
// Stability  : experimental
//
// Renderer
// -----------------------------------------------------------------------------

#include "renderer.hpp"

Renderer::Renderer(unsigned w, unsigned h, std::string const& file, std::vector<std::shared_ptr<Shape>> const& objects)
  : width_(w)
  , height_(h)
  , color_buffer_(w*h, Color{0.0, 0.0, 0.0})
  , filename_(file)
  , objects_(objects)
  , ppm_(width_, height_)
{}

void Renderer::render(float fov)
{
  std::size_t const checker_pattern_size = 20;

  for (unsigned y = 0; y < height_; ++y) {
    for (unsigned x = 0; x < width_; ++x) {
      Pixel p(x,y);
      // if ( ((x/checker_pattern_size)%2) != ((y/checker_pattern_size)%2)) {
      //   p.color = Color{0.0f, 1.0f, float(x)/height_};
      // } else {
      //   p.color = Color{1.0f, 0.0f, float(y)/width_};
      // }

      // origin {0, 0, 0}
      // eye-dir {0, 0, -1}
      // fov-x 90°

      float screen_x_normalized = 2.0f * (((float) x + 0.5f) / (float) width_) - 1.0f;
      float screen_y_normalized = 2.0f * (((float) y + 0.5f) / (float) height_) - 1.0f;

      float aspect = (float) width_ / (float) height_;

      float fov_rad = fov * (3.1415f / 180.0f); // fov-x in radians

      float camera_x = screen_x_normalized * tanf(fov_rad/2.0f) * aspect;
      float camera_y = screen_y_normalized * tanf(fov_rad/2.0f);

      glm::vec3 ray_dir {camera_x, camera_y, -1.0f};
      ray_dir = glm::normalize(ray_dir);

      Ray cam_ray{{0.0f, 0.0f, 0.0f}, ray_dir};

      p.color = Color{0.0f, 0.0f, 0.0f};

      float closest_t = INFINITY;

      for (auto const& obj : objects_) {
        Hitpoint hitp = obj->intersect(cam_ray);
        if (hitp.hit && hitp.t < closest_t) {
          // p.color = Color{(ray_dir.r + 1.0f) / 2.0f, (ray_dir.g + 1.0f) / 2.0f, hitp.t / 50.f};
          p.color = Color{0.0f, 0.0f, hitp.t / 50.f};
          closest_t = hitp.t;
        }
      }

      write(p);
    }
  }
  ppm_.save(filename_);
}

void Renderer::write(Pixel const& p)
{
  // flip pixels, because of opengl glDrawPixels
  size_t buf_pos = (width_*p.y + p.x);
  if (buf_pos >= color_buffer_.size() || (int)buf_pos < 0) {
    std::cerr << "Fatal Error Renderer::write(Pixel p) : "
      << "pixel out of ppm_ : "
      << (int)p.x << "," << (int)p.y
      << std::endl;
  } else {
    color_buffer_.at(buf_pos) = p.color;
  }

  ppm_.write(p);
}
