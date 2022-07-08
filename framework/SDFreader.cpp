#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <glm/vec3.hpp>
#include "SDFreader.hpp"


std::vector<std::shared_ptr<Material>> sdf_reader(std::string path) {
    std::vector<std::shared_ptr<Material>> materials;
    std::ifstream sdf_filestream(path, std::ios::in);
    
    if (!sdf_filestream.is_open()) {
        std::cout << "Could not open file" << std::endl;
        exit(-1);
    }

    std::string line_buffer;
    while (std::getline(sdf_filestream, line_buffer)) {
        std::istringstream iss(line_buffer);
        std::string keyword = "";
        iss >> keyword;
        std::string name = "";

        Color ka{};// Ambient Color
        Color kd{};// Diffuse Color
        Color ks{};// Specular Color
        float m; // Specular Reflection Constant

        if ("define" == keyword) {
            iss >> keyword;
            if ("material" == keyword) {
                iss >> name;
                iss >> ka.r >> ka.g >> ka.b;
                iss >> kd.r >> kd.g >> kd.b;
                iss >> ks.r >> ks.g >> ks.b;
                iss >> m;// Specular Reflection Constant

                materials.push_back(std::make_shared<Material>(Material{ name,ka,kd,ks,m }));
            }
        }
    }
    return materials;
    sdf_filestream.close();
}