#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers

class VBO {
	public:
		GLuint ID;
		VBO(GLfloat* vertices, GLsizeiptr size);

		void Bind();
		void Unbind();
		void Delete();
};

#endif
