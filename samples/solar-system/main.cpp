#include <Engine.h>
#include <Entity.h>
#include <Light.h>
#include <ResourceSystem.h>
#include <Scene.h>
#include "Corp.h"
#include "JsonParser.h"
#include <memory>
#include <vector>

const char vertex_shader[] = {
#embed "../../src/assets/vertex_shader.glsl"
    , '\0'};

const char frag_shader[] = {
#embed "../../src/assets/frag_shader.glsl"
    , '\0'};

const char frag_light_shader[] = {
#embed "../../src/assets/frag_light_shader.glsl"
    , '\0'};

int main() {
    oriongl::Engine engine;
    oriongl::core::ResourceSystem resource_system;

    auto system_data = oriongl::samples::utils::JsonParser::readSystemData();
    auto &planets_data = system_data.first;
    auto &stars_data = system_data.second;

    oriongl::core::Scene scene;

    // Load Stars (Sun) using emissive light shader
    for (const auto &star : stars_data) {
        auto mesh = resource_system.createSphereMesh(star.radius);
        auto mat = resource_system.createMaterial(star.textures);
        auto program = resource_system.createShader(vertex_shader, frag_light_shader, star.defines);

        auto star_model = std::make_shared<oriongl::samples::solar_system::Corp>(program, mesh, mat, star, true);
        oriongl::core::Entity star_entity;
        star_entity.model = star_model;
        star_entity.instances = {glm::vec3(0.0f)};

        scene.entities.push_back(star_entity);
    }

    // Load Planets using lit shader
    for (const auto &planet : planets_data) {
        auto mesh = resource_system.createSphereMesh(planet.radius);
        auto mat = resource_system.createMaterial(planet.textures);
        auto program = resource_system.createShader(vertex_shader, frag_shader, planet.defines);

        auto planet_model = std::make_shared<oriongl::samples::solar_system::Corp>(program, mesh, mat, planet, false);
        oriongl::core::Entity planet_entity;
        planet_entity.model = planet_model;
        planet_entity.instances = {glm::vec3(0.0f)};

        scene.entities.push_back(planet_entity);
    }

    // Configure central point light located at the Sun
    std::vector<oriongl::graphics::PointLight> point_lights = {
        {
            glm::vec3(0.0f, 0.0f, 0.0f),
            oriongl::graphics::LightColor{1.0f, 0.98f, 0.92f},
            oriongl::graphics::LightAttenuation{1.0f, 0.0001f, 0.000005f}
        }
    };
    scene.lights = oriongl::core::Lighting{point_lights};
    scene.lights.getLightScaling().ambient = 0.08f;
    scene.lights.getLightScaling().diffuse = 0.9f;
    scene.lights.getLightScaling().specular = 0.6f;

    // Configure panoramic camera view
    scene.camera = {45.0f, oriongl::ratio::FullHD, 0.5f, 4000.0f};
    scene.camera.getViewPosition() = glm::vec3(0.0f, 150.0f, 350.0f);

    engine.setScene(scene);
    engine.run();

    return 0;
}

