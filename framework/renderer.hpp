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

#include <string>
#include <atomic>
#include <thread>
#include <glm/glm.hpp>

#include "helper.hpp"
#include "color.hpp"
#include "pixel.hpp"
#include "ppmwriter.hpp"
#include "Scene.hpp"
#include "Shape.hpp"

class Renderer
{
public:
    Renderer(unsigned w, unsigned h, std::string const& file, Camera const& cam, unsigned sample_width, Scene const& sce);
    void render();
    
    inline std::vector<Color> const& color_buffer() const
    {
        return color_buffer_;
    }

    inline void get_size(unsigned int & width, unsigned int & height) const
    {
        width = width_;
        height = height_;
    }
    void ray_thread(std::atomic<int>& i);

private:
    void write(Pixel const& p);
    Color trace_primary(Ray const& prim_ray) const;
    // 0 diffuse | 1 reflection | 2 refraction
    Color trace_secondary(Hitpoint const& h, unsigned int depth) const;
    // float cast_shadow(Ray const& secondary_ray, glm::vec3 const& light_pos) const;
    glm::vec3 cast_shadow(Ray const& secondary_ray, glm::vec3 const& light_pos) const;

    unsigned int width_;
    unsigned int height_;
    std::vector<Color> color_buffer_;
    std::string filename_;
    PpmWriter ppm_;
    Camera camera_;
    unsigned int sample_width_;
    Scene scene_;
};

#endif // #ifndef BUW_RENDERER_HPP
