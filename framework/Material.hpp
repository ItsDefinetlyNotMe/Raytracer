#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <string>
#include <iostream>

#include <glm/vec3.hpp>

#include "Color.hpp"

struct Material {
    std::string name;

    Color ka; // Ambient Color
    Color kd; // Diffuse Color
    Color ks; // Specular Color

    float m; // Specular Reflection Constant
    Material(std::string n, Color a, Color d, Color s, float spc) :name{ n }, ka{ a }, kd{ d }, ks{ s } {}
    friend std::ostream& operator<<(std::ostream& out, Material const& mat) {
        out << "Material: " << mat.name << "\n"
            << " Ambient Color: " << mat.ka
            << " Diffuse Color: " << mat.kd
            << " Specular Color: " << mat.ks
            << " Specular Reflection Constant: " << mat.m;
        return out;
    }
};

#endif // MATERIAL_HPP