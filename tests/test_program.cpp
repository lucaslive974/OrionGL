#include <Program.h>
#include <Shader.h>
#include <glad.h>
#include <gtest/gtest.h>
#include <memory.h>
#include <test_resources.h>

namespace oriongl::graphics {

class ProgramTest : public testing::Test {
  protected:
    static std::shared_ptr<Shader> vertex_shader;
    static std::shared_ptr<Shader> fragment_shader;

    void SetUp() override {
        vertex_shader = std::make_shared<Shader>(ShaderType::VERTEX, vertex_src);
        fragment_shader = std::make_shared<Shader>(ShaderType::FRAGMENT, fragment_src);
    };

    void TearDown() override {
        vertex_shader = nullptr;
        fragment_shader = nullptr;
    };
};

std::shared_ptr<Shader> ProgramTest::vertex_shader = nullptr;
std::shared_ptr<Shader> ProgramTest::fragment_shader = nullptr;

TEST_F(ProgramTest, create_and_link) {
    Program program{vertex_shader, fragment_shader};
    EXPECT_NE(program.getId(), 0);
    EXPECT_TRUE(glIsProgram(program.getId()));
};

TEST_F(ProgramTest, program_raii_and_move) {
    unsigned int id = 0;
    {
        Program p1{vertex_shader, fragment_shader};
        id = p1.getId();
        EXPECT_TRUE(glIsProgram(id));

        Program p2 = std::move(p1);
        EXPECT_EQ(p1.getId(), 0);
        EXPECT_EQ(p2.getId(), id);
        EXPECT_TRUE(glIsProgram(id));
    }
    EXPECT_FALSE(glIsProgram(id));
}

} // namespace oriongl::graphics
