#include "shaderClass.h"

std::string get_file_contents(const char* filename) {

	std::ifstream in(filename, std::ios::binary);
	
	if (in) {
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

Shader::Shader(const char* vertexFile, const char* fragmentFile) {
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	const char* vertexShaderSource = vertexCode.c_str();
	const char* fragmentShaderSource = fragmentCode.c_str();

	// Define a reference to a vertexShader (all OpenGL objects are accessed by a handle (aka reference))
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Set the shader source of the above defined reference to our string at file start. The one is the amount of strings.
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// Compile the shader into machine code before running.
	glCompileShader(vertexShader);
	compileErrors(vertexShader, "VERTEX");

	// Same
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	compileErrors(fragmentShader, "FRAGMENT");

	// Define a reference to a Shader Program
	ID = glCreateProgram();

	// Attach both shaders to the program
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);

	// Link the program
	glLinkProgram(ID);
	compileErrors(ID, "PROGRAM");

	// Once inserted into the program, shader references can be dumped
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Activate() {
	glUseProgram(ID);
}

void Shader::Delete() {
	glDeleteProgram(ID);
}

void Shader::compileErrors(unsigned int shader, const char* type) {
	GLint hasCompiled;
	char infoLog[1024];

	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_COMPILATION_ERROR for: " << type << "\n" << infoLog << "\n";
		}
	}
	else {
		glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE) {
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "PROGRAM_LINKING_ERROR for: " << type << "\n" << infoLog << "\n";
		}
	}
}