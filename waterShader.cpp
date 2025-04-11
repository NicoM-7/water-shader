#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "PlaneMesh.hpp"
#include "ShaderUtils.hpp"
#include "TextureLoader.hpp"
#include "Camera.hpp"

using namespace glm;

GLFWwindow *window;

Camera camera;

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
	camera.processMouseButton(button, action);
}

void cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{
	camera.processMouseMovement(xpos, ypos);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	camera.processKey(key, action);
}

int main(int argc, char *argv[])
{
	float screenW = 1500;
	float screenH = 1500;
	float stepsize = 1.0f;
	float xmin = -10;
	float xmax = 10;

	if (argc > 1)
	{
		screenW = atof(argv[1]);
	}
	if (argc > 2)
	{
		screenH = atof(argv[2]);
	}
	if (argc > 3)
	{
		stepsize = atof(argv[3]);
	}
	if (argc > 4)
	{
		xmin = atof(argv[4]);
	}
	if (argc > 5)
	{
		xmax = atof(argv[5]);
	}

	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	window = glfwCreateWindow(screenW, screenH, "Water Shader", NULL, NULL);
	if (window == NULL)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetKeyCallback(window, key_callback);

	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		glfwTerminate();
		return -1;
	}

	glPatchParameteri(GL_PATCH_VERTICES, 4);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	PlaneMesh plane(xmin, xmax, stepsize);
	plane.init();

	GLuint waterTexture = loadBMP("./Assets/water.bmp");
	GLuint dispTexture = loadBMP("./Assets/displacement-map1.bmp");

	mat4 Projection = perspective(radians(45.0f), screenW / screenH, 0.001f, 1000.0f);
	mat4 View;
	mat4 MVP;

	vec3 lightPos(30.0f, 30.0f, 30.0f);
	vec3 ambientLight(1.0f);
	float shininess = 8.0f;
	vec3 specularColor(1.0f, 1.0f, 1.0f);

	float startTime = (float)glfwGetTime();

	do
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		View = camera.getViewMatrix();
		MVP = Projection * View;
		float time = (float)glfwGetTime() - startTime;

		plane.useShader();
		setUniform(plane.programID, "time", time);
		setUniform(plane.programID, "MVP", MVP);
		setUniform(plane.programID, "eyePos", camera.getPosition());
		setUniform(plane.programID, "lightPos", lightPos);
		setUniform(plane.programID, "tessLevelInner", 16.0f);
		setUniform(plane.programID, "tessLevelOuter", 16.0f);
		setUniform(plane.programID, "texOffset", vec2(0.0f, 0.0f));
		setUniform(plane.programID, "texScale", 10.0f);
		setUniform(plane.programID, "dispScale", 1.0f);
		setUniform(plane.programID, "ambientLight", ambientLight);
		setUniform(plane.programID, "specularColor", glm::vec3(0.7f));
		setUniform(plane.programID, "shininess", shininess);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, waterTexture);
		setUniform(plane.programID, "waterTex", 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, dispTexture);
		setUniform(plane.programID, "dispMap", 1);

		plane.draw();

		glfwSwapBuffers(window);
		glfwPollEvents();

	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
			 !glfwWindowShouldClose(window));

	glfwTerminate();
	return 0;
}