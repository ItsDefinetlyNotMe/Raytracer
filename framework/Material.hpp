#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <string>
#include <iostream>
#include <memory>

#include <glm/vec3.hpp>

#include "Color.hpp"

enum Mat_Type {Diffuse, Metallic, Dielectric};

struct Material {
    std::string name_;
    
    Color ka_; // Ambient Color
    Color kd_; // Diffuse Color
    Color ks_; // Specular Color
    
    float m_; // Specular Reflection Constant

    Mat_Type type_ = Diffuse;

    float reflectivity_ = 0.0f;

    float opacity_ = 1.0f;
    float refractive_index_ = 1.0f;

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