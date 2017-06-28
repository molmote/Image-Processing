/*****************************************************************************/
/*!
\file   Shader.cpp
\author Yeongki Baek
\par    email: yeongki.baek\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   07/12/2016
\brief
This is the interface file for Shader features
Copyright 2016, Digipen Institute of Technology
*/
/*****************************************************************************/
//#include "Shader.h"
//#include <vector>
//
//Shader::Shader(std::string fileName, ShaderType stype) {
//	switch (stype) {
//		case VERTEX:
//			shaderHandle = glCreateShader(GL_VERTEX_SHADER_ARB);
//			break;
//		case FRAGMENT:
//			shaderHandle = glCreateShader(GL_FRAGMENT_SHADER_ARB);
//			break;
//		case GEOMETRY:
//			shaderHandle = glCreateShader(GL_GEOMETRY_SHADER_EXT);
//			break;
//	}
//
//	std::ifstream shaderSource(fileName.c_str());
//	if (!shaderSource.is_open()) {
//		std::cerr << "File not found " << fileName.c_str() << "\n";
//
//		CreateDefaultShader(shaderHandle, stype);
//		//exit(EXIT_FAILURE);
//	}
//
//	else
//	{
//		// now read in the data
//		std::string source((std::istreambuf_iterator<char>(shaderSource)), std::istreambuf_iterator<char>());
//		shaderSource.close();
//		source += "\0";
//
//		const char* data = source.c_str();
//		glShaderSource(shaderHandle, 1, &data, nullptr);
//		glCompileShader(shaderHandle);
//
//		GLint isCompiled = 0;
//		glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &isCompiled);
//		if (isCompiled == GL_FALSE) {
//			GLint maxLength = 0;
//			glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &maxLength);
//
//			//The maxLength includes the NULL character
//			std::vector<GLchar> infoLog(maxLength);
//			glGetShaderInfoLog(shaderHandle, maxLength, &maxLength, &infoLog[0]);
//
//			std::cout << &infoLog[0] << std::endl;
//
//			//We don't need the shader anymore.
//			//glDeleteShader(vertexShader);
//
//			//Use the infoLog as you see fit.
//
//			//In this simple program, we'll just leave
//			//return;
//		}
//	}
//}
//
//Shader::~Shader()
//{
////	delete this;
//}
//
//void Shader::CreateDefaultShader(GLuint id, ShaderType stype)
//{
//	const GLchar *vertex_shader =
//		"#version 330\n"
//		"	layout(location = 0) in vec4 position;\n"
//		"uniform mat4 mvp;\n"
//		"void main() {\n"
//		"	gl_Position = mvp * position;\n"
//		"}\n";
//
//
//	const GLchar* fragment_shader =
//		"#version 330\n"
//		"uniform sampler2D positionMap;\n"
//		"uniform sampler2D colorMap; \n"
//		"uniform sampler2D normalMap; \n"
//		"uniform vec3 lightPos; \n"
//		"uniform float lightIntensity; \n"
//		"uniform float diffuseConstant; \n"
//		"uniform vec2 screenSize; \n"
//		"uniform vec3 lightColor; \n"
//		"out vec4 fragColor; \n"
//		"vec2 calcTexCoord() {\n"
//		"	return gl_FragCoord.xy / screenSize; \n"
//		"}\n"
//		"void main() {\n"
//		"	vec2 texCoord = calcTexCoord(); \n"
//		"	vec3 worldPos = texture(positionMap, texCoord).xyz; \n"
//		"	vec3 color = texture(colorMap, texCoord).xyz; \n"
//		"	vec3 normal = texture(normalMap, texCoord).xyz; \n"
//		"	vec3 lightDir = lightPos - worldPos; \n"
//		"	float attenuation = lightIntensity / dot(lightDir, lightDir); \n"
//		"	lightDir = normalize(lightDir); \n"
//		"	vec3 diffuseComponent = (diffuseConstant * dot(normal, lightDir)) * (color * lightColor); \n"
//		"	fragColor = vec4(attenuation * diffuseComponent, 1.0); \n"
//		"}\n";
//
//	const GLchar* geometry_shader =
//		"#version 330 core\n"
//		"layout(points) in;\n"
//		"layout(triangle_strip, max_vertices = 4) out;\n"
//		"out vec2 texcoord;\n"
//		"void main()\n"
//		"{\n"
//		"gl_Position = vec4(1.0, 1.0, 0.5, 1.0);\n"
//		"texcoord = vec2(1.0, 1.0);\n"
//		"EmitVertex();\n"
//		"gl_Position = vec4(-1.0, 1.0, 0.5, 1.0);\n"
//		"texcoord = vec2(0.0, 1.0);\n"
//		"EmitVertex();\n"
//		"gl_Position = vec4(1.0, -1.0, 0.5, 1.0);\n"
//		"texcoord = vec2(1.0, 0.0);\n"
//		"EmitVertex();\n"
//		"gl_Position = vec4(-1.0, -1.0, 0.5, 1.0);\n"
//		"texcoord = vec2(0.0, 0.0);\n"
//		"EmitVertex();\n"
//		"EndPrimitive();\n"
//		"}\n";
//
//
//	const GLchar * data;
//
//	int sID = 0;
//
//	switch (stype) {
//	case VERTEX:
//		sID = glCreateShader(GL_VERTEX_SHADER_ARB);
//		data = vertex_shader;
//		break;
//	case FRAGMENT:
//		sID = glCreateShader(GL_FRAGMENT_SHADER_ARB);
//		data = fragment_shader;
//		break;
//	case GEOMETRY:
//		sID = glCreateShader(GL_GEOMETRY_SHADER_EXT);
//		data = geometry_shader;
//		break;
//	}
//
//
//	// Backup GL state
//	GLint last_texture, last_array_buffer, last_vertex_array;
//	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
//	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
//	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);
//
//
//	glShaderSource(id, 1, &data, nullptr);
//	glCompileShader(id);
//
//	GLint isCompiled = 0;
//	glGetShaderiv(id, GL_COMPILE_STATUS, &isCompiled);
//	if (isCompiled == GL_FALSE) {
//		GLint maxLength = 0;
//		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);
//
//		//The maxLength includes the NULL character
//		std::vector<GLchar> infoLog(maxLength);
//		glGetShaderInfoLog(id, maxLength, &maxLength, &infoLog[0]);
//
//		std::cout << &infoLog[0] << std::endl;
//
//		//We don't need the shader anymore.
//		//glDeleteShader(vertexShader);
//
//		//Use the infoLog as you see fit.
//
//		//In this simple program, we'll just leave
//		//return;
//	}
//}
//
//void Shader::load(std::string & fileName)
//{
//}
//
//GLuint Shader::getShaderHandle() const
//{
//	return shaderHandle;
//}
