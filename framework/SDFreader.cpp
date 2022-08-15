#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <glm/vec3.hpp>

#include "SDFreader.hpp"
#include "Box.hpp"
#include "Sphere.hpp"


//mag ich nicht!!
std::shared_ptr<Renderer> sdf_reader(std::string const& path ) {
    //kommentar
    std::vector<std::shared_ptr<Material>> materials;
    std::vector<std::shared_ptr<Point_Light>> lights;
    std::vector<std::shared_ptr<Camera>> cameras;
    std::vector<std::shared_ptr<Shape>> shapes;
    Color ambient{0.2f,0.2f,0.2f};
    ///////////////////////////
    std::string cam_name;
    std::string filename;
    unsigned x_res = 0;
    unsigned y_res = 0;
    ///////////////////////////

    std::shared_ptr<Renderer> rend = nullptr;

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
        
        

        if ("define" == keyword) {
            iss >> keyword;
            if ("material" == keyword) {
                
                std::string name = "";
                Color ka{};// Ambient Color
                Color kd{};// Diffuse Color
                Color ks{};// Specular Color
                float m; // Specular Reflection Constant

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
                    //define shape sphere <name> [center] <radius> <mat-name>
                    std::string sphere_name;
                    glm::vec3 center{};
                    float radius;
                    std::string mat_name;

                    iss >> sphere_name;
                    iss >> center.x >> center.y >> center.z;
                    iss >> radius;
                    iss >> mat_name;

                   // find material with name ////Vector?////
                   // !!Annahme!! Material wird vor Sphere deklariert
                    shapes.push_back(std::make_shared<Sphere>(Sphere{ sphere_name,find(materials,mat_name),center,radius }));
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
                    
                    // find material with name ////Vector?////
                    // !!Annahme!! Material wird vor Box deklariert
                    shapes.push_back(std::make_shared<Box>(Box{box_name,find(materials,mat_name),min,max}));
                }
            }
            else if ("light" == keyword) {
                //define light <name> [pos] [color] [brightness]
                std::string light_name;
                glm::vec3 pos{};
                Color color{};
                float brightness;
                
                iss >> light_name;
                iss >> pos.x >> pos.y >> pos.z;
                iss >> color.r >> color.g >> color.b;
                iss >> brightness;
                ///
                lights.push_back(std::make_shared<Point_Light>(Point_Light{light_name,pos,color,brightness}));

            }
            else if ("camera" == keyword) {
                //camera <name> <fov-x> (<eye> <dir> <up>)
                std::string camera_name;
                float fov_x;
                glm::vec3 eye{ 0.0f,0.0f,0.0f };
                glm::vec3 dir{ 0.0f,0.0f,-1.0f };
                glm::vec3 up{ 0.0f,1.0f,0.0f };
                
                iss >> camera_name >> fov_x >> eye.x >> eye.y >> eye.z >> dir.x >> dir.y >> dir.z >> up.x >> up.y >> up.z;
                cameras.push_back(std::make_shared<Camera>(Camera{camera_name,fov_x}));
            }
            else if ("ambient" == keyword){
                //ambient [ambient]
                iss >> ambient.r >> ambient.g >> ambient.b;
            }
        }
        else if ("render" == keyword) {
            //render <cam-name> <filename> <x-res> <y-res> //Cameraname ?
            iss >> cam_name >> filename >> x_res >> y_res;
        }
    }

    Scene s{ shapes,cameras,lights,ambient };
    rend = std::make_shared<Renderer>(Renderer{ x_res,y_res,filename,*find(cameras,cam_name),s});
    sdf_filestream.close();
    return rend;
}