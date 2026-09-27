#include <Engine.h>
#include <string>
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

// clang-format off
const std::vector<glm::vec3> cube_positions = {
    {  35.0f,   0.0f,   0.0f },
    {  24.7f,   8.0f,  24.7f },
    {   0.0f,  15.0f,  35.0f },
    { -24.7f,   8.0f,  24.7f },
    { -35.0f,   0.0f,   0.0f },

    { -24.7f,  -8.0f, -24.7f },
    {   0.0f, -15.0f, -35.0f },
    {  24.7f,  -8.0f, -24.7f },

    {  17.5f,  20.0f,  30.3f },
    { -17.5f, -20.0f, -30.3f }
};
// clang-format on

std::vector<std::string> box_material{
    "assets/container.png",
    "assets/container_specular.png",
};

std::vector<std::string> lights_uniform_material{"assets/white_pixel.png"};

int main() {
    oriongl::Engine engine;
    oriongl::core::ResourceSystem resource_system;

    auto cube_mesh = resource_system.createCubeMesh(3.0f);
    auto cube_program = resource_system.createShader(vertex_shader, frag_shader, {});
    auto cube_material = resource_system.createMaterial(box_material);
    auto cube_model = resource_system.createModel("CUBE_MODEL_1", cube_program, cube_mesh, cube_material);

    oriongl::core::Entity cube_ent;
    cube_ent.model = cube_model;
    cube_ent.instances.assign(cube_positions.begin(), cube_positions.end());

    auto light_program = resource_system.createShader(vertex_shader, frag_light_shader, {});
    auto sphere_mesh = resource_system.createSphereMesh(5.0f);
    auto sphere_material_green = resource_system.createMaterial(lights_uniform_material);
    sphere_material_green->setColor({0.0f, 1.0f, 0.1f});

    auto sphere_material_blue = resource_system.createMaterial(lights_uniform_material);
    sphere_material_blue->setColor({0.0f, 0.7f, 1.0f});

    auto sphere_model_green =
        resource_system.createModel("SPHERE_MODEL_GREEN", light_program, sphere_mesh, sphere_material_green);
    auto sphere_model_blue =
        resource_system.createModel("SPHERE_MODEL_BLUE", light_program, sphere_mesh, sphere_material_blue);

    oriongl::core::Entity sphere_ent_green;
    sphere_ent_green.model = sphere_model_green;
    sphere_ent_green.instances = {{0.0f, 0.0f, -100.0f}};

    oriongl::core::Entity sphere_ent_blue;
    sphere_ent_blue.model = sphere_model_blue;
    sphere_ent_blue.instances = {{-50.0f, 20.0f, 40.0f}};

    std::vector<oriongl::graphics::PointLight> point_light_positions = {{{0.0f, 0.0f, -100.0f}, {0.5f, 1.0f, 0.5f}},
                                                                        {{-50.0f, 20.0f, 40.0f}, {0.5f, 0.5f, 1.0f}}};
    oriongl::core::Lighting scene_lighting{point_light_positions};

    oriongl::core::Scene scene{.lights = scene_lighting};
    scene.entities.insert(scene.entities.end(), {cube_ent, sphere_ent_green, sphere_ent_blue});

    engine.setScene(scene);
    engine.run();

    return 0;
}
