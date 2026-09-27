#include <gtest/gtest.h>
#include <Transform.h>
#include <Entity.h>
#include <glm/gtc/matrix_transform.hpp>

TEST(TransformTest, default_values) {
    oriongl::core::Transform t;
    EXPECT_EQ(t.position, glm::vec3(0.0f, 0.0f, 0.0f));
    EXPECT_EQ(t.rotation, glm::vec3(0.0f, 0.0f, 0.0f));
    EXPECT_EQ(t.scale, glm::vec3(1.0f, 1.0f, 1.0f));
    EXPECT_EQ(t.getMatrix(), glm::mat4(1.0f));
}

TEST(TransformTest, position_constructors) {
    oriongl::core::Transform t1(glm::vec3(1.0f, 2.0f, 3.0f));
    EXPECT_EQ(t1.position, glm::vec3(1.0f, 2.0f, 3.0f));
    EXPECT_EQ(t1.scale, glm::vec3(1.0f, 1.0f, 1.0f));

    oriongl::core::Transform t2(4.0f, 5.0f, 6.0f);
    EXPECT_EQ(t2.position, glm::vec3(4.0f, 5.0f, 6.0f));
    EXPECT_EQ(t2.scale, glm::vec3(1.0f, 1.0f, 1.0f));
}

TEST(TransformTest, matrix_calculation) {
    glm::vec3 pos(10.0f, -5.0f, 2.0f);
    glm::vec3 rot(0.0f, 90.0f, 0.0f);
    glm::vec3 scl(2.0f, 2.0f, 2.0f);

    oriongl::core::Transform t(pos, rot, scl);

    glm::mat4 expected = glm::mat4(1.0f);
    expected = glm::translate(expected, pos);
    expected = glm::rotate(expected, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    expected = glm::scale(expected, scl);

    glm::mat4 actual = t.getMatrix();
    for (int col = 0; col < 4; ++col) {
        for (int row = 0; row < 4; ++row) {
            EXPECT_NEAR(actual[col][row], expected[col][row], 1e-4f);
        }
    }
}

TEST(TransformTest, entity_instances_compatibility) {
    oriongl::core::Entity entity;
    entity.instances.push_back({1.0f, 2.0f, 3.0f});
    entity.instances.emplace_back(glm::vec3(4.0f, 5.0f, 6.0f));

    EXPECT_EQ(entity.instances.size(), 2);
    EXPECT_EQ(entity.instances[0].position, glm::vec3(1.0f, 2.0f, 3.0f));
    EXPECT_EQ(entity.instances[1].position, glm::vec3(4.0f, 5.0f, 6.0f));
}
