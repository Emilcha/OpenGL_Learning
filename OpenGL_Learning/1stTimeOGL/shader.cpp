#include "shader.h"

Shader::Shader(const char* vertPath, const char* fragPath)
{
    std::string vertCode;
    std::string fragCode;
    std::ifstream vertShaderFile;
    std::ifstream fragShaderFile;
    vertShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    
    try
    {
        vertShaderFile.open(vertPath);
        fragShaderFile.open(fragPath);
        std::stringstream vertShaderStream, fragShaderStream;
        vertShaderStream << vertShaderFile.rdbuf();
        fragShaderStream << fragShaderFile.rdbuf();
        vertShaderFile.close();
        fragShaderFile.close();
        vertCode = vertShaderStream.str();
        fragCode = fragShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "(!) Erreur durant l'ouverture des fichiers !" << std::endl;
    }
	
	const char* vertShaderCode = vertCode.c_str();
	const char* fragShaderCode = fragCode.c_str();

	// Compilation du shader vertex
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertShaderCode, NULL);
	glCompileShader(vertexShader);
	// Recuperation des erreurs de la compilation du shader
	printErrorLog(vertexShader, "vertex");
	// Compilation du shader fragment
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragShaderCode, NULL);
	glCompileShader(fragmentShader);
	// Recuperation des erreurs de la compilation du shader
	printErrorLog(fragmentShader, "fragment");
	// Linking des shader en un programme
	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
	// Recuperation des erreurs du linking du programme
	printErrorLog(ID, "programme");
	// Cleanup, on suprimme les shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::printErrorLog(GLuint element, std::string elemType)
{
	GLint succes;
	GLint logLenth;
	GLchar* log;
	if (elemType == "programme")
	{
		glGetProgramiv(element, GL_LINK_STATUS, &succes);
		if (!succes)
		{
			glGetProgramiv(element, GL_INFO_LOG_LENGTH, &logLenth);
			log = new GLchar[logLenth];
			glGetProgramInfoLog(element, logLenth, NULL, log);
			std::cout << "(!) Erreur durant le linkage du programme :\n" << log << std::endl;
			delete[] log;
		} else {
			std::cout << "(+) Le linkage du programme a reussi !" << std::endl;
		}
	} else {
		glGetShaderiv(element, GL_COMPILE_STATUS, &succes);
		if (!succes)
		{
			glGetShaderiv(element, GL_INFO_LOG_LENGTH, &logLenth);
			log = new GLchar[logLenth];
			glGetShaderInfoLog(element, logLenth, NULL, log);
			std::cout << "(!) Erreur durant la compilation du " << elemType << " shader :\n" << log << std::endl;
			delete[] log;
		} else {
			std::cout << "(+) Le linkage du " << elemType << " shader a reussi !" << std::endl;
		}
	}
}

// ------------------------------------------------------------------------
void Shader::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
// ------------------------------------------------------------------------
void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
// ------------------------------------------------------------------------
void Shader::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
// ------------------------------------------------------------------------
void Shader::setVec2(const std::string& name, const glm::vec2& value) const
{
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setVec2(const std::string& name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}
// ------------------------------------------------------------------------
void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setVec3(const std::string& name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}
// ------------------------------------------------------------------------
void Shader::setVec4(const std::string& name, const glm::vec4& value) const
{
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setVec4(const std::string& name, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}
// ------------------------------------------------------------------------
void Shader::setMat2(const std::string& name, const glm::mat2& mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::setMat3(const std::string& name, const glm::mat3& mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------