// -----------------------------------------------------------------------------
// Copyright  : (C) 2014-2017 Andreas-C. Bernstein
// License    : MIT (see the file LICENSE)
// Maintainer : Andreas-C. Bernstein <andreas.bernstein@uni-weimar.de>
// Stability  : experimental
//
// Renderer
// -----------------------------------------------------------------------------

#ifndef BUW_RENDERER_HPP
#define BUW_RENDERER_HPP

#include "Camera.hpp"
#include "color.hpp"
#include "pixel.hpp"
#include "ppmwriter.hpp"
#include <string>
#include <glm/glm.hpp>
#include "Scene.hpp"
#include "Shape.hpp"
class Renderer
{
public:
  Renderer(unsigned w, unsigned h, std::string const& file, Camera const& cam,Scene const& sce);

  void render();
  void write(Pixel const& p);

  Color trace_ray(float x, float y) const;
  Color Renderer::lightning(Hitpoint const& h, std::shared_ptr<Shape> const& obj_h) const;
  Color Renderer::trace_ray_second(Ray const& prim_ray) const;


  inline std::vector<Color> const& color_buffer() const
  {
    return color_buffer_;
  }

private:
  unsigned width_;
  unsigned height_;
  std::vector<Color> color_buffer_;
  std::string filename_;
  PpmWriter ppm_;
  Camera camera_;
  Scene scene_;
};

#endif // #ifndef BUW_RENDERER_HPP
