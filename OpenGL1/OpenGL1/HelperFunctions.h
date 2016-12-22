#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdarg.h>
#include <time.h>
#define GL_LOG_FILE "gl.log"

using namespace std;

class HelperFunctions
{
public:
	static void printHardwareInfo();
	static void logHardwareInfo();
	static void checkShaderCompilation(GLuint shade);
	static string readShaderFile(string path);
	static bool restart_gl_log();
	static bool gl_log(const char* message,...);
	static bool gl_log_err(const char* message, ...);
	static void log_gl_params();
private:
};