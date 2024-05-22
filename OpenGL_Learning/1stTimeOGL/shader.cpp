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

	GLint sucessVert;
	GLint sucessFrag;
	GLint sucessProg;
	GLint tempLogLenth;
	GLchar* infoLog;

	// Compilation du shader vertex
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertShaderCode, NULL);
	glCompileShader(vertexShader);
	// Recuperation des erreurs de la compilation du shader
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &sucessVert);
	if (!sucessVert)
	{
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &tempLogLenth);
		infoLog = new GLchar[tempLogLenth];
		glGetShaderInfoLog(vertexShader, tempLogLenth, NULL, infoLog);
		std::cout << "(!) Erreur durant la compilation du vertex shader :\n" << infoLog << std::endl;
		delete [] infoLog;
	}
	// Compilation du shader fragment
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragShaderCode, NULL);
	glCompileShader(fragmentShader);
	// Recuperation des erreurs de la compilation du shader
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &sucessFrag);
	if (!sucessFrag)
	{
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &tempLogLenth);
		infoLog = new GLchar[tempLogLenth];
		glGetShaderInfoLog(fragmentShader, tempLogLenth, NULL, infoLog);
		std::cout << "(!) Erreur durant la compilation du fragment shader :\n" << infoLog << std::endl;
		delete[] infoLog;
	}
	// Linking des shader en un programme
	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);
	// Recuperation des erreurs du linking du programme
	glGetProgramiv(ID, GL_LINK_STATUS, &sucessProg);
	if (!sucessProg)
	{
		glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &tempLogLenth);
		infoLog = new GLchar[tempLogLenth];
		glGetProgramInfoLog(ID, tempLogLenth, NULL, infoLog);
		std::cout << "(!) Erreur durant le linkage du programme :\n" << infoLog << std::endl;
		delete[] infoLog;
	}
	// Cleanup, on suprimme les shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool val) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)val);
}
void Shader::setInt(const std::string& name, int val) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), val);
}
void Shader::setFloat(const std::string& name, float val) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), val);
}
