#include <Texture.h>
#include <glad.h>
#include <gtest/gtest.h>
#include <test_resources.h>

namespace oriongl::graphics {

TEST(TextureTest, create_and_initializate) {
    Texture tex{DUMMY_TEXTURE_PATH};

    EXPECT_NE(tex.getTex(), 0);
    EXPECT_TRUE(glIsTexture(tex.getTex()));
};

TEST(TextureTest, create_and_initializate_multiple) {
    Texture array[] = {{DUMMY_TEXTURE_PATH}, {DUMMY_TEXTURE_PATH}, {DUMMY_TEXTURE_PATH}};

    for (auto &tex : array) {
        EXPECT_TRUE(glIsTexture(tex.getTex()));
    }

    for (size_t i = 0; i < 2; i++) {
        for (size_t j = i + 1; j < 3; j++) {
            EXPECT_NE(array[i].getTex(), array[j].getTex());
        }
    }
}

TEST(TextureTest, inexistent_texture_file_must_throw) {
    EXPECT_ANY_THROW(({ Texture tex{"Inexistent_texture_path"}; }));
}

TEST(TextureTest, texture_raii_and_move) {
    unsigned int tex_id = 0;
    {
        Texture t1{DUMMY_TEXTURE_PATH};
        tex_id = t1.getTex();
        EXPECT_TRUE(glIsTexture(tex_id));

        Texture t2 = std::move(t1);
        EXPECT_EQ(t1.getTex(), 0);
        EXPECT_EQ(t2.getTex(), tex_id);
        EXPECT_TRUE(glIsTexture(tex_id));
    }
    EXPECT_FALSE(glIsTexture(tex_id));
}

} // namespace oriongl::graphics
