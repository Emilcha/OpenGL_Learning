#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "shader.h"
#include "texture.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
}

int main(int argc, char* argv[])
{
	// Initalisation de GLFW
	glfwInit();
	// OpenGL v3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Mode de compatibilité désactivé
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Fenetre OpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "(!) Erreur durant l'initalisation de la fenetre GLFW" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// Initalisation GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "(!) Erreur durant l'initalisation de GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);
	// Callback appelé quand la fenetre est redimentioné
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	// Création et usage du shader program
	Shader daShader("shaders/shader.vert","shaders/shader.frag");

	// Charger l'image
	Texture container("img/container.jpg");
	Texture awsome("img/awesomeface.png");

	// Coordonées d'Appareil Normalisé (NDC)
	GLfloat vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};

	// Nos deux triangles
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	// Vertex Array Object (Utiliser pour stoquer des atributs de VBO)
	GLuint VAO;
	glGenVertexArrays(1, &VAO);

	// Vertex Buffer Object (Utilisé pour stoquer les coordoné des points)
	GLuint VBO;
	glGenBuffers(1, &VBO);

	// Element Buffer Object (Utilisé pour stoquer les triangle)
	GLuint EBO;
	glGenBuffers(1, &EBO);

	// On dit quel VAO va utiliser le VBO
	glBindVertexArray(VAO);

	// On s'attache au pointeur GL_ARRAY_BUFFER pour le VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// On s'attache au pointeur GL_ELEMENT_ARRAY_BUFFER pour le EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	// On copie nos données dans le VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// On copie nos données dans le EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// On definis nos atributs du VBO (dans le VAO)
	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
	// Couleur
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Position texture
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// On utilise notre shader program
	daShader.use();

	// On utilise notre texture
	container.use(0);
	daShader.setInt("texture1", 0);

	awsome.use(1);
	daShader.setInt("texture2", 1);


	GLint transformLoc = glGetUniformLocation(daShader.ID, "transform");
	

	// Boucle de rendu
	while (!glfwWindowShouldClose(window))
	{
		// Entrée utilisateur
		processInput(window);

		glm::mat4 transform(1.0f);
		transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));
		transform = glm::scale(transform, glm::vec3(glm::sin((float)glfwGetTime())));
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		// Rendu
		glClear(GL_COLOR_BUFFER_BIT);
		// On se bind a notre VAO pour les attribus (et indirectement le VBO)
		glBindVertexArray(VAO);
		// On dessine nos triangles
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// On se casse le lien au VAO
		glBindVertexArray(0);

		// Appel des evenements et changement de buffer
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}