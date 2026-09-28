#include <Engine.h>
#include <string>
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

// clang-format off
const std::vector<glm::vec3> cubePositions = {
    {  35.0F,   0.0F,   0.0F },
    {  24.7F,   8.0F,  24.7F },
    {   0.0F,  15.0F,  35.0F },
    { -24.7F,   8.0F,  24.7F },
    { -35.0F,   0.0F,   0.0F },

    { -24.7F,  -8.0F, -24.7F },
    {   0.0F, -15.0F, -35.0F },
    {  24.7F,  -8.0F, -24.7F },

    {  17.5F,  20.0F,  30.3F },
    { -17.5F, -20.0F, -30.3F }
};
// clang-format on

std::vector<std::string> boxMaterial{
    "assets/container.png",
    "assets/container_specular.png",
};

std::vector<std::string> lightsUniformMaterial{"assets/white_pixel.png"};

int main() {
    oriongl::Engine engine;
    oriongl::core::ResourceSystem resourceSystem;

    auto cubeMesh = resourceSystem.createCubeMesh(3.0F);
    auto cubeProgram = resourceSystem.createShader(vertexShader, fragShader, {});
    auto cubeMaterial = resourceSystem.createMaterial(boxMaterial);
    auto cubeModel = resourceSystem.createModel("CUBE_MODEL_1", cubeProgram, cubeMesh, cubeMaterial);

    oriongl::core::Entity cubeEnt;
    cubeEnt.model = cubeModel;
    cubeEnt.instances.assign(cubePositions.begin(), cubePositions.end());

    auto lightProgram = resourceSystem.createShader(vertexShader, fragLightShader, {});
    auto sphereMesh = resourceSystem.createSphereMesh(5.0F);
    auto sphereMaterialGreen = resourceSystem.createMaterial(lightsUniformMaterial);
    sphereMaterialGreen->setColor({0.0F, 1.0F, 0.1F});

    auto sphereMaterialBlue = resourceSystem.createMaterial(lightsUniformMaterial);
    sphereMaterialBlue->setColor({0.0F, 0.7F, 1.0F});

    auto sphereModelGreen = resourceSystem.createModel("SPHERE_MODEL_GREEN", lightProgram, sphereMesh, sphereMaterialGreen);
    auto sphereModelBlue = resourceSystem.createModel("SPHERE_MODEL_BLUE", lightProgram, sphereMesh, sphereMaterialBlue);

    oriongl::core::Entity sphereEntGreen;
    sphereEntGreen.model = sphereModelGreen;
    sphereEntGreen.instances = {{0.0F, 0.0F, -100.0F}};

    oriongl::core::Entity sphereEntBlue;
    sphereEntBlue.model = sphereModelBlue;
    sphereEntBlue.instances = {{-50.0F, 20.0F, 40.0F}};

    std::vector<oriongl::graphics::PointLight> pointLightPositions = {{{0.0F, 0.0F, -100.0F}, {0.5F, 1.0F, 0.5F}},
                                                                      {{-50.0F, 20.0F, 40.0F}, {0.5F, 0.5F, 1.0F}}};
    oriongl::core::Lighting sceneLighting{pointLightPositions};

    oriongl::core::Scene scene{.lights = sceneLighting};
    scene.entities.insert(scene.entities.end(), {cubeEnt, sphereEntGreen, sphereEntBlue});

    engine.setScene(scene);
    engine.run();

    return 0;
}
