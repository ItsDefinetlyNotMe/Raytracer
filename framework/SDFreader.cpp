#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <glm/vec3.hpp>

#include "SDFreader.hpp"

//mag ich nicht!!
std::shared_ptr<Renderer> sdf_reader(std::string const& path ) {
    //kommentar
    std::vector<std::shared_ptr<Material>> materials;
    std::vector<std::shared_ptr<Point_Light>> lights;
    std::vector<std::shared_ptr<Area_Light>> area_lights;
    std::vector<std::shared_ptr<Camera>> cameras;
    std::vector<std::shared_ptr<Shape>> shapes;
    Color ambient{0.2f,0.2f,0.2f};
    ///////////////////////////
    std::string cam_name;
    std::string filename;
    unsigned x_res = 0;
    unsigned y_res = 0;
    unsigned sample_width = 1;
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

                std::string type = "Diffuse";

                float reflectivity = 0.0f;
                float opacity = 1.0f;
                float refractive_index = 1.0f;

                iss >> name;
                iss >> ka.r >> ka.g >> ka.b;
                iss >> kd.r >> kd.g >> kd.b;
                iss >> ks.r >> ks.g >> ks.b;
                iss >> m;
                iss >> type;

                if ("Diffuse" == type) {
                    materials.push_back(std::make_shared<Material>(Material{name, ka, kd, ks, m, Diffuse, 0.0f, 1.0f, 1.0f}));
                } else if ("Metallic" == type) {
                    iss >> reflectivity;
                    materials.push_back(std::make_shared<Material>(Material{name, ka, kd, ks, m, Metallic, reflectivity, 1.0f, 1.0f}));
                } else if ("Dielectric" == type) {
                    iss >> opacity;
                    iss >> refractive_index;
                    materials.push_back(std::make_shared<Material>(Material{name, ka, kd, ks, m, Dielectric, 0.0f, opacity, refractive_index}));
                } 
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
                else if ("cone" == keyword) {
                    //define shape cone <name> [bottom] [height] [radius] <mat-name>
                    std::string cone_name;
                    glm::vec3 bottom{};
                    float height;
                    float radius;
                    std::string mat_name;

                    iss >> cone_name;
                    iss >> bottom.x >> bottom.y >> bottom.z;
                    iss >> height >> radius;
                    iss >> mat_name;
                    
                    shapes.push_back(std::make_shared<Cone>(Cone{cone_name,find(materials,mat_name), bottom, radius, height}));
                }
                else if ("cylinder" == keyword) {
                    //define shape cylinder <name> [center] [height] [radius] <mat-name>
                    std::string cylinder_name;
                    glm::vec3 bottom;
                    float height;
                    float radius;
                    std::string mat_name;

                    iss >> cylinder_name;
                    iss >> bottom.x >> bottom.y >> bottom.z;
                    iss >> height >> radius;
                    iss >> mat_name;
                    
                    shapes.push_back(std::make_shared<Cylinder>(Cylinder{cylinder_name,find(materials,mat_name), bottom, radius, height}));
                }
                else if ("triangle" == keyword) {
                    //define shape triangle <name> [v1] [v2] [v3] <mat-name>
                    std::string triangle_name;
                    glm::vec3 v1;
                    glm::vec3 v2;
                    glm::vec3 v3;
                    std::string mat_name;

                    iss >> triangle_name;
                    iss >> v1.x >> v1.y >> v1.z;
                    iss >> v2.x >> v2.y >> v2.z;
                    iss >> v3.x >> v3.y >> v3.z;
                    iss >> mat_name;
                    
                    shapes.push_back(std::make_shared<Triangle>(Triangle{triangle_name,find(materials,mat_name), v1, v2, v3}));
                }
                else if ("composite" == keyword) {
                    // define shape composite <name> [shapes ..]
                    std::string composite_name;
                    iss >> composite_name;
                    std::vector<std::string> children_names;
                    std::string name;
                    while(iss>>name) 
                        children_names.push_back(name);
                    std::vector<std::shared_ptr<Shape>> children;
                    for (auto const& name : children_names) {
                        auto find_shape = [&](std::shared_ptr<Shape> const& shape) -> bool {return name == shape->get_name();};
                        auto shape = *std::find_if(shapes.begin(), shapes.end(), find_shape);
                        children.push_back(shape);
                    }

                    shapes.push_back(std::make_shared<Composite>(Composite(composite_name, children)));
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
            else if ("a_light" == keyword) {
                std::string name;
                glm::vec3 pos{};
                glm::vec3 u_vec{};
                glm::vec3 v_vec{};
                unsigned u_steps = 0;
                unsigned v_steps = 0;
                Color col{};
                float brightness = 0.0f;

                iss >> name;
                iss >> pos.x >> pos.y >> pos.z;
                iss >> u_steps;
                iss >> v_steps;
                iss >> u_vec.x >> u_vec.y >> u_vec.z;
                iss >> v_vec.x >> v_vec.y >> v_vec.z;
                iss >> col.r >> col.g >> col.b;
                iss >> brightness;

                area_lights.push_back(std::make_shared<Area_Light>(Area_Light{name, pos, u_vec, v_vec, u_steps, v_steps, col, brightness}));
            }
            else if ("camera" == keyword) {
                //camera <name> <fov-x> (<eye> <dir> <up>)
                std::string camera_name;
                float fov_x;
                glm::vec3 eye{ 0.0f,0.0f,0.0f };
                glm::vec3 dir{ 0.0f,0.0f,-1.0f };
                glm::vec3 up{ 0.0f,1.0f,0.0f };
                
                iss >> camera_name >> fov_x >> eye.x >> eye.y >> eye.z >> dir.x >> dir.y >> dir.z >> up.x >> up.y >> up.z;
                cameras.push_back(std::make_shared<Camera>(Camera{camera_name, fov_x, eye, dir, up}));
            }
            else if ("ambient" == keyword){
                //ambient [ambient]
                iss >> ambient.r >> ambient.g >> ambient.b;
            }
        }
        else if ("transform" == keyword) {
            std::string shape_name;
            iss >> shape_name;
            auto find_shape = [&](std::shared_ptr<Shape> const& shape) -> bool {return shape_name == shape->get_name();};
            auto shape = *std::find_if(shapes.begin(), shapes.end(), find_shape);
            iss >> keyword;
            if ("translate" == keyword) {
                Translation translation;
                iss >> translation.translate.x >> translation.translate.y >> translation.translate.z;
                shape->set_translation(translation);
            }
            else if ("rotate" == keyword) {
                Rotation rot;
                iss >> rot.angle >> rot.vector.x >> rot.vector.y >> rot.vector.z;
                shape->set_rotation(rot);
            }
            else if ("scale" == keyword) {
                Scaling scale;
                iss >> scale.scale;
                shape->set_scaling(scale);
            }
        }
        else if ("set" == keyword) {
            iss >> keyword;
            if ("samples" == keyword) {
                iss >> sample_width;
            }
        }
        else if ("render" == keyword) {
            //render <cam-name> <filename> <x-res> <y-res> //Cameraname ?
            iss >> cam_name >> filename >> x_res >> y_res;
        }
    }

    // Scene s{ shapes,cameras,lights,ambient };
    auto find_shape = [&](std::shared_ptr<Shape> const& shape) -> bool {return "root" == shape->get_name();};
    auto root = *std::find_if(shapes.begin(), shapes.end(), find_shape);
    if (nullptr == root) {
        std::cout<<"No root node"<<std::endl;
        exit(-1);
    }
    root->prepare_for_rendering();
    Scene s{ root, cameras, lights, area_lights, ambient };
    rend = std::make_shared<Renderer>(Renderer{ x_res, y_res, filename, *find(cameras,cam_name), sample_width, s });
    sdf_filestream.close();
    return rend;
}