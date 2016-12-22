#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "HelperFunctions.h"

using namespace std;


//Reported Window Size
int g_win_width = 640;
int g_win_height = 480;

//Framebuffer Size
int g_fb_width = 640;
int g_fb_height = 480;

double previous_seconds;
int frame_count;


void glfw_error_callback(int error, const char* description)
{
	HelperFunctions::gl_log_err("GLFW ERROR: code %i msg: %s\n", error, description);
}

void glfw_window_size_callback(GLFWwindow* window, int width, int height)
{
	g_win_width = width;
	g_win_height = height;
}

void glfw_framebuffer_resize_callback(GLFWwindow* window, int width, int height)
{
	g_fb_width = width;
	g_fb_height = height;
}

void _update_fps_counter(GLFWwindow* window) 
{
	double current_seconds;
	double elapsed_seconds;
	current_seconds = glfwGetTime();
	elapsed_seconds = current_seconds - previous_seconds;
	//Limit Text Updates to 4 per seconds
	if (elapsed_seconds > 0.25) 
	{
		previous_seconds = current_seconds;
		char tmp[128];
		double fps = (double)frame_count / elapsed_seconds;
		sprintf(tmp, "opengl @ fps: %.2f", fps);
		glfwSetWindowTitle(window, tmp);
		frame_count = 0;
	}
	frame_count++;
}

int main()
{
	if (!HelperFunctions::restart_gl_log())
	{
		/* QUIT? */
	}

	if (!glfwInit())
	{
		cout << "ERROR: could not start GLFW3" << endl;
		return 1;
	}

	//Set Hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//Enable 4x AntiAlising
	glfwWindowHint(GLFW_SAMPLES, 4);



	GLFWmonitor* mon = glfwGetPrimaryMonitor();
	const GLFWvidmode* vmode = glfwGetVideoMode(mon);
	GLFWwindow* window = glfwCreateWindow(vmode->width, vmode->height, "Extended GL Init", NULL, NULL);//mon, NULL);


	if (!window)
	{
		cout << "ERROR: could not open window with GLFW" << endl;
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(window);

	//These won't work if called BEFORE making the OpenGL context current
	HelperFunctions::logHardwareInfo();
	HelperFunctions::log_gl_params();


	HelperFunctions::gl_log("Starting GLFW\n%s\n", glfwGetVersionString());
	//Register the error call-back function
	glfwSetErrorCallback(glfw_error_callback);
	glfwSetWindowSizeCallback(window,glfw_window_size_callback);
	glfwSetFramebufferSizeCallback(window, glfw_framebuffer_resize_callback);


	glewExperimental = GL_TRUE;
	if (glewInit() == GLEW_OK)
	{
		cout << endl << "==== GLEW Initialized successfully ====" << endl;
	}

	//Prints OpenGL and Hardware-Info for this machine
	HelperFunctions::printHardwareInfo();

	glEnable(GL_DEPTH_TEST);
	//Smaller value is closer
	glDepthFunc(GL_LESS);

	//Begin Triangle Stuff
	GLfloat points[] =
	{
		//Up Right
		0.5, 0.5, 0,
		//Low Right
		0.5, -0.5, 0,
		//Low Left
		-0.5, -0.5, 0,
		//SECOND TRIANGLE
		//Up Left
		-0.5, 0.5, 0,
		//Up Right
		0.5, 0.5, 0,
		//Low Left
		-0.5, -0.5, 0
	};

	//Bind Triangle to VertexBufferObject
	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	//Create Vertex Attribute Object
	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	//Bind Vertex Attribute Object
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	string vertex_shader = HelperFunctions::readShaderFile("vertex.shader");
	GLint lenVS = vertex_shader.length();
	const char* vShader = vertex_shader.c_str();
	string fragment_shader = HelperFunctions::readShaderFile("fragment.shader");
	GLint lenFS = fragment_shader.length();
	const char* fShader = fragment_shader.c_str();

	//Load shaders into GL_SHADERS
	//Vertex Shader
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vShader, &lenVS);
	glCompileShader(vs);

	//Fragment Shader
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fShader, &lenFS);
	glCompileShader(fs);

	//Check if Shaders compiled correctly
	cout << "Checking Vertex Shader Compilation" << endl;
	HelperFunctions::checkShaderCompilation(vs);
	cout << "Checking Fragment Shader Compilation" << endl;
	HelperFunctions::checkShaderCompilation(fs);

	//Combine Shaders into GPU shader program
	GLuint shader_programme = glCreateProgram();
	glAttachShader(shader_programme, fs);
	glAttachShader(shader_programme, vs);
	glLinkProgram(shader_programme);

	while (!glfwWindowShouldClose(window))
	{
		_update_fps_counter(window);
		//wipe the drawing surface clear
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Update viewport size
		glViewport(0, 0, g_fb_width, g_fb_height);
		glClearColor(0.5, 0.5, 0.5, 1.0);
		glUseProgram(shader_programme);
		glBindVertexArray(vao);
		//Draw points 0 to 3 from the currently bound VAO with our shaders
		glDrawArrays(GL_TRIANGLES, 0, 6);
		//update input handling
		glfwPollEvents();
		//Put everything on our display
		glfwSwapBuffers(window);

		if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE))
		{
			glfwSetWindowShouldClose(window, 1);
		}
	}

	return 0;
}