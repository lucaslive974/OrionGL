#include "Corp.h"
#include "JsonParser.h"
#include <Engine.h>
#include <Entity.h>
#include <Light.h>
#include <ResourceSystem.h>
#include <Scene.h>
#include <memory>
#include <vector>

const char vertexShader[] = {
#embed "../../src/assets/vertex_shader.glsl"
    , '\0'};

const char fragShader[] = {
#embed "../../src/assets/frag_shader.glsl"
    , '\0'};

const char fragLightShader[] = {
#embed "../../src/assets/frag_light_shader.glsl"
    , '\0'};

int main() {
    oriongl::Engine engine;
    oriongl::core::ResourceSystem resourceSystem;

    auto systemData = oriongl::samples::utils::JsonParser::readSystemData();
    auto &planetsData = systemData.first;
    auto &starsData = systemData.second;

    oriongl::core::Scene scene;

    struct CorpEntityRef {
        std::shared_ptr<oriongl::samples::solar_system::Corp> corp;
        size_t entityIndex;
    };
    std::vector<CorpEntityRef> corps;

    // Load Stars (Sun) using emissive light shader
    for (const auto &star : starsData) {
        auto mesh = resourceSystem.createSphereMesh(star.radius);
        auto mat = resourceSystem.createMaterial(star.textures);
        auto program = resourceSystem.createShader(vertexShader, fragLightShader, star.defines);

        auto starModel = std::make_shared<oriongl::samples::solar_system::Corp>(program, mesh, mat, star, true);
        oriongl::core::Entity starEntity;
        starEntity.model = starModel;
        starEntity.instances = {glm::vec3(0.0F)};

        corps.push_back({.corp = starModel, .entityIndex = scene.entities.size()});
        scene.entities.push_back(starEntity);
    }

    // Load Planets using lit shader
    for (const auto &planet : planetsData) {
        auto mesh = resourceSystem.createSphereMesh(planet.radius);
        auto mat = resourceSystem.createMaterial(planet.textures);
        auto program = resourceSystem.createShader(vertexShader, fragShader, planet.defines);

        auto planetModel = std::make_shared<oriongl::samples::solar_system::Corp>(program, mesh, mat, planet, false);
        oriongl::core::Entity planetEntity;
        planetEntity.model = planetModel;
        planetEntity.instances = {glm::vec3(0.0F)};

        corps.push_back({.corp = planetModel, .entityIndex = scene.entities.size()});
        scene.entities.push_back(planetEntity);
    }

    // Configure central point light located at the Sun
    std::vector<oriongl::graphics::PointLight> pointLights = {
        {glm::vec3(0.0F, 0.0F, 0.0F), oriongl::graphics::LightColor{1.0F, 0.98F, 0.92F},
         oriongl::graphics::LightAttenuation{.constant = 1.0F, .linear = 0.0001F, .quadratic = 0.000005F}}};
    scene.lights = oriongl::core::Lighting{pointLights};
    scene.lights.getLightScaling().ambient = 0.08F;
    scene.lights.getLightScaling().diffuse = 0.9F;
    scene.lights.getLightScaling().specular = 0.6F;

    // Configure panoramic camera view
    scene.camera = {45.0F, oriongl::ratio::FullHD, 0.5F, 4000.0F};
    scene.camera.getViewPosition() = glm::vec3(0.0F, 150.0F, 350.0F);

    engine.setScene(scene);

    engine.setUpdateCallback([corps](oriongl::core::Scene &sc, float /*dt*/, float totalTime) {
        for (const auto &ref : corps) {
            if (ref.entityIndex < sc.entities.size() && !sc.entities[ref.entityIndex].instances.empty()) {
                ref.corp->update(sc.entities[ref.entityIndex].instances[0], totalTime);
            }
        }
    });

    engine.run();

    return 0;
}
