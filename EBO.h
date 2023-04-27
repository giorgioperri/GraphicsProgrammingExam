#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <vector>

class EBO {
	public:
		GLuint ID;
		EBO(std::vector<GLuint>& indices);

		void Bind();
		void Unbind();
		void Delete();
};

#endif
