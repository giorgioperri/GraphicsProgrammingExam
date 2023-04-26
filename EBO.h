#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers

class EBO {
public:
	GLuint ID;
	EBO(GLuint* indices, GLsizeiptr size);

	void Bind();
	void Unbind();
	void Delete();
};

#endif
