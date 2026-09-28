#include <Engine.h>
#include <ResourceSystem.h>
#include <Scene.h>

const char vertexShader[] = {
#embed "../../src/assets/vertex_shader.glsl"
    , '\0'};

const char fragShader[] = {
#embed "../../src/assets/frag_shader.glsl"
    , '\0'};

const char *backpackSrc = "assets/survival_guitar_backpack/scene.gltf";
const char *darkKnightSrc = "assets/dark_knight/scene.gltf";
const char *seaKeepSrc = "assets/sea_keep/scene.gltf";

int main() {
    oriongl::Engine engine;
    oriongl::core::ResourceSystem resourceSystem;

    auto program = resourceSystem.createShader(vertexShader, fragShader);

    auto seaKeep = resourceSystem.createModel("SEA_KEEP", program, seaKeepSrc);
    oriongl::core::Entity seaKeepEnt;
    seaKeepEnt.model = seaKeep;
    seaKeepEnt.instances.emplace_back(0.0F, -300.0F, -500.0F);

    std::vector<oriongl::graphics::PointLight> pointLightPositions = {
        {{0.0F, 0.0F, 0.0F}, {0.7F, 0.7F, 1.0F}, {.constant = 1.0F, .linear = 0.0F, .quadratic = 0.0F}},
    };

    oriongl::core::Lighting sceneLighting{pointLightPositions};

    oriongl::core::Scene scene{.lights = sceneLighting};
    scene.entities.push_back(seaKeepEnt);

    scene.camera = {45.0F, oriongl::ratio::FullHD, 0.1F, 3000.0F};

    engine.setScene(scene);
    engine.run();

    return 0;
}
