#include "glew.h"

#include <GLFW/glfw3.h>
#include "glm.hpp"
#include "ext.hpp"
#include <iostream>
#include <cmath>

#include "ex_6_1.hpp"





int main(int argc, char** argv)
{
	// inicjalizacja glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// tworzenie okna za pomoca glfw
	GLFWwindow* window = glfwCreateWindow(1000, 1000, "FirstWindow", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// ladowanie OpenGL za pomoca glew
	glewInit();
	glViewport(0, 0, 1000, 1000);

	init(window);

	// uruchomienie glownej petli
	renderLoop(window);

	shutdown(window);
	glfwTerminate();
	return 0;
}
