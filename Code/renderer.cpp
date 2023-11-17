// Taken from Raytracing in One Weekend, modified from there
#include "color.h"
#include "ray.h"
#include "vec3.h"
#include "sphere.h"
#include "triangle.h"
#include "cylinder.h"
#include "camera.h"
#include <vector>
#include "shape.h"
#include "scene.h"
#include "json.hpp"
using json = nlohmann::json;
#include <fstream>

#include <iostream>

std::string renderMode;
camera cam;
scene sce;

color ray_color(const ray& r) {
    // seems dependent on the order of the shapes in the scene object, rather than which is actually closest to the camera

    for (int i = 0; i < sce.getShapes().size(); ++i) {
        shape* currentShape = sce.getShapes()[i];
        double intersect = currentShape->intersection(r);
        if (intersect != -1) {
            if (renderMode == "binary") {
                return color(1,0,0);
            }
            else if (renderMode == "phong") {
                // return currentShape->get_material().get_diffuse_color(); // just to check
                auto pointOnSurface = r.at(intersect);
                color ambient = color(0,0,0);
                for (int j = 0; j < sce.getLights().size(); ++j) {
                    auto currentLight = sce.getLights()[j];
                    // std::cerr << "currentLight: " << currentLight->getPosition() << "\n";
                    color lightIntensity = currentLight->getIntensity();

                    auto kd = currentShape->get_material().get_kd();
                    auto ks = currentShape->get_material().get_ks();
                    auto specularExponent = currentShape->get_material().get_specular_exponent();

                    color materialDiffuseColor = currentShape->get_material().get_diffuse_color();
                    color materialSpecularColor = currentShape->get_material().get_specular_color();

                    vec3 lightVector = unit_vector(currentLight->getPosition() - pointOnSurface);
                    vec3 normalVector = currentShape->get_normal(pointOnSurface);
                    vec3 viewVector = unit_vector(cam.getPosition() - pointOnSurface);

                    // vec3 reflectionVector = unit_vector(2 * (dot(lightVector, normalVector)) * normalVector - lightVector);

                    // auto lightNormalDot = std::max(dot(lightVector, normalVector), 0.0);
                    auto lightNormalDot = dot(lightVector, normalVector);

                    // Term used in phong
                    // auto viewReflectionDot = std::max(dot(viewVector, reflectionVector), 0.0);
                    // auto viewReflectionDot = dot(reflectionVector, viewVector);

                    // Term used in blinn-phong
                    vec3 halfwayVector = unit_vector(lightVector + viewVector);
                    auto normalHalfwayDot = std::max(dot(normalVector, halfwayVector), 0.0);

                    auto diffuseColor = kd * lightNormalDot * materialDiffuseColor * lightIntensity;

                    // phong term
                    // auto specularColor = ks * pow(viewReflectionDot, specularExponent) * materialSpecularColor * lightIntensity;

                    // blinn-phong term
                    auto specularColor = ks * pow(normalHalfwayDot, specularExponent) * materialSpecularColor * lightIntensity;
                    ambient = ambient + diffuseColor + specularColor;

                    // std::vector<double> colour(3);
                    // for (int k = 0; k < 3; ++k) {
                    //     auto diffuseTerm = kd * materialDiffuse[k] * lightIntensity[k] * lightNormalDot;
                    //     std::cerr << "diffuseTerm: " << diffuseTerm << "\n";

                    //     // phong term
                    //     auto specularTerm = ks * materialSpecular[k] * lightIntensity[k] * pow(viewReflectionDot, specularExponent);

                    //     // blinn-phong term
                    //     // auto specularTerm = ks * materialSpecular[k] * lightIntensity[k] * pow(viewHalfwayDot, specularExponent);
                    //     colour[k] = diffuseTerm + specularTerm;
                    // }
                    // color phongColor(colour[0], colour[1], colour[2]);
                    // ambient += phongColor;
                }
                return ambient;
            }
        } 
    }
    return sce.getBackgroundColor();
}

material parse_material(const json& j) {
    // auto sphereMaterial = j["material"];

    auto ks = j["ks"].get<double>();
    auto kd = j["kd"].get<double>();

    auto specularExponent = j["specularexponent"].get<int>();

    auto isReflective = j["isreflective"].get<bool>();
    auto reflectivity = j["reflectivity"].get<double>();

    auto isRefractive = j["isrefractive"].get<bool>();
    auto refractiveIndex = j["refractiveindex"].get<double>();

    auto diffuse = j["diffusecolor"].get<std::vector<double>>();
    color diffuseColor = color(diffuse[0], diffuse[1], diffuse[2]);

    auto specular = j["specularcolor"].get<std::vector<double>>();
    color specularColor = color(specular[0], specular[1], specular[2]);

    material mat = material(ks, kd, reflectivity, refractiveIndex, specularExponent, 
                            diffuseColor, specularColor, isReflective, isRefractive);\
    return mat;
}

void parse_camera_params(const json& j) {
    // generated by copilot line by line
    auto cameraPos = j["position"].get<std::vector<double>>();
    point3 cameraPosition(cameraPos[0], cameraPos[1], cameraPos[2]);

    auto cameraLookAt = j["lookAt"].get<std::vector<double>>();
    vec3 cameraLookAtVector(cameraLookAt[0], cameraLookAt[1], cameraLookAt[2]);

    auto cameraUpVec = j["upVector"].get<std::vector<double>>();
    vec3 cameraUpVector(cameraUpVec[0], cameraUpVec[1], cameraUpVec[2]);

    auto cameraWidth = j["width"].get<int>();
    auto cameraHeight = j["height"].get<int>();
    auto cameraFov = j["fov"].get<double>();

    // implement the exposure once that's something you actually have to care about lol

    cam = camera(cameraWidth, cameraHeight, cameraPosition, cameraLookAtVector, cameraUpVector, cameraFov, 1.0);
    // return cam;
}

void parse_scene_params(const json& j) {
    // Parse scene parameters
    // following 2 lines generated by copilot
    auto sceneBackgroundColorData = j["backgroundcolor"].get<std::vector<double>>();
    color sceneBackgroundColor(sceneBackgroundColorData[0], sceneBackgroundColorData[1], sceneBackgroundColorData[2]);

    std::vector<pointlight*> lightsources;
    if (renderMode != "binary") {
        auto sceneLightSources = j["lightsources"];
        std::cerr << "sceneLightSources: " << sceneLightSources << "\n";
        if (!j["lightsources"].is_null()) {
            for (int i = 0; i < sceneLightSources.size(); ++i) {
                auto light = sceneLightSources[i];
                
                auto lightPosition = light["position"].get<std::vector<double>>();
                point3 lightPositionPoint(lightPosition[0], lightPosition[1], lightPosition[2]);

                auto lightIntensity = light["intensity"].get<std::vector<double>>();
                color lightIntensityColor(lightIntensity[0], lightIntensity[1], lightIntensity[2]);

                pointlight* l = new pointlight(lightPositionPoint, lightIntensityColor);
                
                lightsources.push_back(l);
            }
        }
    }

    auto sceneShapes = j["shapes"];
    std::vector<shape*> shapes;

    for (int i = 0; i < sceneShapes.size(); ++i) {
        auto shapeType = sceneShapes[i]["type"].get<std::string>();

        material mat;
        if (!sceneShapes[i]["material"].is_null()) {
            mat = parse_material(sceneShapes[i]["material"]);
        }

        // generated by copilot
        if (shapeType == "sphere") {
            sphere* s;
            auto sphereCenter = sceneShapes[i]["center"].get<std::vector<double>>();
            point3 sphereCenterPoint(sphereCenter[0], sphereCenter[1], sphereCenter[2]);

            auto sphereRadius = sceneShapes[i]["radius"].get<double>();

            if (!sceneShapes[i]["material"].is_null()) {
                s = new sphere(sphereCenterPoint, mat, sphereRadius);
            }
            else {
                s = new sphere(sphereCenterPoint, sphereRadius);
            }
            shapes.push_back(s);
        }
        else if (shapeType == "triangle") {
            auto v0 = sceneShapes[i]["v0"].get<std::vector<double>>();
            point3 v0Point(v0[0], v0[1], v0[2]);

            auto v1 = sceneShapes[i]["v1"].get<std::vector<double>>();
            point3 v1Point(v1[0], v1[1], v1[2]);

            auto v2 = sceneShapes[i]["v2"].get<std::vector<double>>();
            point3 v2Point(v2[0], v2[1], v2[2]);

            triangle* tri;
            if (!sceneShapes[i]["material"].is_null()) {
                tri = new triangle(v0Point, v1Point, v2Point, mat);
            }
            else {
                tri = new triangle(v0Point, v1Point, v2Point);
            }
            shapes.push_back(tri);
        }
        else if (shapeType == "cylinder") {
            auto cylinderCenter = sceneShapes[i]["center"].get<std::vector<double>>();
            point3 cylinderCenterPoint(cylinderCenter[0], cylinderCenter[1], cylinderCenter[2]);

            auto cylinderAxis = sceneShapes[i]["axis"].get<std::vector<double>>();
            vec3 cylinderAxisVector(cylinderAxis[0], cylinderAxis[1], cylinderAxis[2]);

            auto cylinderRadius = sceneShapes[i]["radius"].get<double>();
            auto cylinderHeight = sceneShapes[i]["height"].get<double>();

            cylinder* cyl;
            if (!sceneShapes[i]["material"].is_null()) {
                cyl = new cylinder(cylinderCenterPoint, cylinderAxisVector, mat, cylinderRadius, cylinderHeight);
            }
            else {
                cyl = new cylinder(cylinderCenterPoint, cylinderAxisVector, cylinderRadius, cylinderHeight);
            }
            shapes.push_back(cyl);
        }
    }

    sce = scene(sceneBackgroundColor, shapes, lightsources);
}


int main(int argc, char *argv[]) {
    // assuming the file name of the JSON file is passed as the first argument
    // retrieve data from the JSON file
    std::string filepath = __FILE__;
    std::cerr << filepath << "\n";
    std::string base_filename = "Code/" + filepath.substr(filepath.find_last_of("/") + 1);
    filepath.erase(filepath.length() - base_filename.length());

    std::ifstream jsonFile(filepath + argv[1]);
    json j = json::parse(jsonFile);

    renderMode = j["rendermode"].get<std::string>();

    // Parse camera parameters
    parse_camera_params(j["camera"]);

    // Parse scene parameters
    parse_scene_params(j["scene"]);

    // Render
    std::cout << "P3\n" << cam.getWidth() << ' ' << cam.getHeight() << "\n255\n";
    for (int j = 0; j < cam.getHeight(); ++j) { 
        std::clog << "\rScanlines remaining: " << (cam.getHeight() - j) << ' ' << std::flush;

        for (int i = 0; i < cam.getWidth(); ++i) {
            auto pixel_center = cam.getPixel00Loc() + (i * cam.getPixelDeltaU()) + (j * cam.getPixelDeltaV());
            auto ray_direction = pixel_center - cam.getPosition();
            ray r(cam.getPosition(), ray_direction);
            
            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\rDone.                 \n";
}