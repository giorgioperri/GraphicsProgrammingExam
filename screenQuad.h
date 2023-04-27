#ifndef SCREEN_QUAD_H
#define SCREEN_QUAD_H

#include <glad/glad.h>

class ScreenQuad {
    public:
        ScreenQuad();
        ~ScreenQuad();

        void draw(GLuint textureRef) const;

    private:
        GLuint m_vao;
        GLuint m_vbo;
};

#endif // QUAD_H