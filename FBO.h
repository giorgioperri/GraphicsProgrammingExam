#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <glad/glad.h>

class Framebuffer {
    public:
        Framebuffer(int width, int height);
        ~Framebuffer();

        void bind();
        void unbind();

        GLuint getColorAttachment() const;

    private:
        GLuint m_fbo;
        GLuint m_colorAttachment;
        GLuint m_rbo;
        int m_width, m_height;
};

#endif // FRAMEBUFFER_H