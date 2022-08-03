#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <glm/vec3.hpp>

#include "Sphere.hpp"
#include "SDFreader.hpp"
#include "Box.hpp"


std::vector<std::shared_ptr<Material>> sdf_reader(std::string path) {
    std::vector<std::shared_ptr<Material>> materials;
    std::vector<std::shared_ptr<Shape>> shapes;

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
        
        //kommt noch ins material if

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
                iss >> m;

                materials.push_back(std::make_shared<Material>(Material{ name,ka,kd,ks,m }));
            }
            else if ("shape" == keyword) {
                iss >> keyword;
                if ("sphere" == keyword) {
                    //define shape sphere <name>[center] <radius> <mat - name>
                    std::string sphere_name;
                    glm::vec3 center{};
                    float radius;
                    std::string mat_name;
                    iss >> sphere_name;
                    iss >> center.x >> center.y >> center.z;
                    iss >> radius;
                    iss >> mat_name;

                   // find material with name
                   //shapes.push_back(std::make_shared<Sphere>(Sphere{ sphere_name,center,radius,mat_name }));
                }
                else if ("box" == keyword) {
                    //define shape box <name> [p1] [p2] <mat-name>
                    std::string box_name;
                    glm::vec3 min{};
                    glm::vec3 max{};
                    std::string mat_name;

                    iss >> box_name;
                    iss >> min.x >> min.y >> min.z;
                    iss >> max.x >> max.y >> max.z;
                    iss >> mat_name;
                    
                    // find material with name
                    //shapes.push_back(std::make_shared<Box>(Box{}));
                }
            }
            else if ("light" == keyword) {

            }
            else if ("camera" == keyword) {

            }
        }
        else if ("render" == keyword) {

        }
    }
    return materials;
    sdf_filestream.close();
}