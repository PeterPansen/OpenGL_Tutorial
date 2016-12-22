#include "HelperFunctions.h"


using namespace std;

void HelperFunctions::printHardwareInfo()
{
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);

	cout << endl << "==== Hardware Infos ====" << endl << "Renderer device is " << renderer << endl;
	cout << "OpenGL Version supported is " << version << endl << "==== Hardware Done ====" << endl << endl;
}

//Needs to be called AFTER making the OpenGL context current
void HelperFunctions::logHardwareInfo() 
{
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);

	gl_log("HARDWARE_INFO:\n");
	gl_log("Renderer device is %s\n", renderer);
	gl_log("OpenGL version supported is %s\n", version);
	gl_log("-----------------\n");
}

void HelperFunctions::checkShaderCompilation(GLuint shade)
{
	GLint compileStat;
	glGetShaderiv(shade, GL_COMPILE_STATUS, &compileStat);

	if (compileStat == GL_FALSE)
	{
		cout << "Shader failed to compile. Sorry :(" << endl;

		GLint infoLogLength;
		glGetShaderiv(shade, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shade, infoLogLength, NULL, strInfoLog);

		cout << strInfoLog << endl;
	}
	else
	{
		cout << "Shader compiled succesfully" << endl;
	}
}

string HelperFunctions::readShaderFile(string path)
{
	string line;
	string result = "";
	ifstream myfile(path);
	if (myfile.is_open())
	{
		cout << "Found and opened " << path << endl;
		while (getline(myfile, line))
		{
			result += line;
			result += "\n";
		}
		myfile.close();
		//cout << result.c_str() << endl;
		return result;
	}
	else cout << "Unable to open file " << path << endl;
	return "UNABLE TO OPEN";
}

bool HelperFunctions::restart_gl_log()
{
	FILE* file = fopen(GL_LOG_FILE, "w");
	if (!file)
	{
		fprintf(stderr, "ERROR: could not open GL_LOG_FILE log file %s for writing\n", GL_LOG_FILE);
		return false;
	}
	time_t now = time(NULL);
	char* date = ctime(&now);
	fprintf(file, "GL_LOG_FILE log. local time %s\n", date);
	fclose(file);
	return true;
}

bool HelperFunctions::gl_log(const char* message,...)
{
	va_list argptr;
	FILE* file = fopen(GL_LOG_FILE, "a");
	if (!file)
	{
		fprintf(stderr, "ERROR: Could not open GL_LOG_FILE %s file for appending\n", GL_LOG_FILE);
		return false;
	}
	va_start(argptr, message);
	vfprintf(file, message, argptr);
	va_end(argptr);
	va_start(argptr, message);
	vfprintf(stderr, message, argptr);
	va_end(argptr);
	fclose(file);
	return true;
}

bool HelperFunctions::gl_log_err(const char* message, ...)
{
	va_list argptr;
	FILE* file = fopen(GL_LOG_FILE, "a");
	if (!file)
	{
		fprintf(stderr, "ERROR: could not open GL_LOG_FILE %s file for appending\n", GL_LOG_FILE);
		return false;
	}
	va_start(argptr, message);
	vfprintf(file, message, argptr);
	va_end(argptr);
	va_start(argptr, message);
	vfprintf(stderr, message, argptr);
	va_end(argptr);
	fclose(file);
	return true;
}

//Needs to be called AFTER making the OpenGL context current
void HelperFunctions::log_gl_params() 
{
	GLenum params[] =
	{
		GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
		GL_MAX_CUBE_MAP_TEXTURE_SIZE,
		GL_MAX_DRAW_BUFFERS,
		GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
		GL_MAX_TEXTURE_IMAGE_UNITS,
		GL_MAX_TEXTURE_SIZE,
		GL_MAX_VARYING_FLOATS,
		GL_MAX_VERTEX_ATTRIBS,
		GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
		GL_MAX_VERTEX_UNIFORM_COMPONENTS,
		GL_MAX_VIEWPORT_DIMS,
		GL_STEREO,
		GL_MAX_SAMPLES,
	};

	const char* names[] =
	{
		"GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS",
		"GL_MAX_CUBE_MAP_TEXTURE_SIZE",
		"GL_MAX_DRAW_BUFFERS",
		"GL_MAX_FRAGMENT_UNIFORM_COMPONENTS",
		"GL_MAX_TEXTURE_IMAGE_UNITS",
		"GL_MAX_TEXTURE_SIZE",
		"GL_MAX_VARYING_FLOATS",
		"GL_MAX_VERTEX_ATTRIBS",
		"GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS",
		"GL_MAX_VERTEX_UNIFORM_COMPONENTS",
		"GL_MAX_VIEWPORT_DIMS",
		"GL_STEREO",
		"GL_MAX_SAMPLES",
	};

	gl_log("GL CONTEXT PARAMETERS:\n");

	for (int i = 0; i < 10; i++)
	{
		int v = 0;
		glGetIntegerv(params[i], &v);
		gl_log("%s %i\n", names[i], v);
	}

	int v[2];
	v[0] = v[1]= 0;
	glGetIntegerv(params[10], v);
	gl_log("%s %i %i\n", names[10], v[0], v[1]);
	unsigned char s = 0;
	glGetBooleanv(params[11], &s);
	gl_log("%s %u\n", names[11], (unsigned int)s);

	int maxSamples = 0;
	glGetIntegerv(params[12], &maxSamples);
	gl_log("%s %i \n", names[12], maxSamples);

	gl_log("---------------------------------------\n");
}







