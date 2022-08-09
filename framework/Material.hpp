#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <string>
#include <iostream>

#include <glm/vec3.hpp>

#include "Color.hpp"

struct Material {
    std::string name_;

    Color ka_; // Ambient Color
    Color kd_; // Diffuse Color
    Color ks_; // Specular Color

    float m_; // Specular Reflection Constant

    friend std::ostream& operator<<(std::ostream& out, Material const& mat) {
        out << "Material: " << mat.name_ << "\n"
            << " Ambient Color: " << mat.ka_
            << " Diffuse Color: " << mat.kd_
            << " Specular Color: " << mat.ks_
            << " Specular Reflection Constant: " << mat.m_;
        return out;
    }
};

#endif // MATERIAL_HPP