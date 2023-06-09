#include "EBO.h"

// Constructor that generates a Vertex Buffer Object and links it to vertices
EBO::EBO(std::vector<GLuint>& indices) {
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}

// Binds the VBO
void EBO::Bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

// Unbinds the VBO
void EBO::Unbind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// Deletes the VBO
void EBO::Delete() {
	glDeleteBuffers(1, &ID);
}