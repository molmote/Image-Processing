/*****************************************************************************/
/*!
\file   ShaderProgram.cpp
\author Yeongki Baek
\par    email: yeongki.baek\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   07/12/2016
\brief
This is the interface file for ShaderProgram features
Copyright 2016, Digipen Institute of Technology
*/
/*****************************************************************************/
#include "ShaderProgram.h"

namespace VK
{
	ShaderProgram::ShaderProgram() :
		linked(false)
	{
		programId = glCreateProgram();
	}
	ShaderProgram::ShaderProgram(std::string &fileName, FlagSet<SHADER_TYPE> &types) :
		linked(false)
	{
		programId = glCreateProgram();
		
		if (types & SHADER_TYPE::GEOMETRY)
		{
			CreateShader(fileName + EXT_GEOM, SHADER_TYPE::GEOMETRY);
		}
		if (types & SHADER_TYPE::VERTEX)
		{
			CreateShader(fileName + EXT_VERT, SHADER_TYPE::VERTEX);
		}
		if (types & SHADER_TYPE::FRAGMENT)
		{
			CreateShader(fileName + EXT_FRAG, SHADER_TYPE::FRAGMENT);
		}
		link();
		
	}

	void ShaderProgram::attachShader(Shader * sh)
	{
		shaders.push_back(sh);
//		glAttachShader(programId, sh->getShaderHandle());
	}


	ShaderProgram::~ShaderProgram()
	{
		unsigned sizeofshaders = shaders.size() + 1;

		while (--sizeofshaders)
			shaders.pop_back();



		unsigned sizeofshaderId = shaders.size() + 1;
		while (--sizeofshaderId)
			shaderId.pop_back();

		/*for (std::vector<Shader*>::reverse_iterator it = shaders.rbegin(); it == shaders.rend(); ++it)
		{
			(*it)->~Shader();
			shaders.pop_back();
		}*/
	}

	/*void ShaderProgram::attachShader(Shader* sh) {
		shaders.push_back(sh);
		glAttachShader(programID, sh->getShaderHandle());
	}*/

	void ShaderProgram::link() {
		glLinkProgram(programId);
		std::cout << "linking Shader\n";

		GLint infologLength = 0;

		glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infologLength);
		std::cout << "Link Log Length " << infologLength << "\n";

		if (infologLength > 0) {
			char *infoLog = new char[infologLength];
			GLint charsWritten = 0;

			glGetProgramInfoLog(programId, infologLength, &charsWritten, infoLog);

			std::cout << infoLog << std::endl;
			delete[] infoLog;
			glGetProgramiv(programId, GL_LINK_STATUS, &infologLength);
			if (infologLength == GL_FALSE) {
				std::cerr << "Program link failed exiting \n";
				exit(EXIT_FAILURE);
			}
		}
		linked = true;

		while (shaderId.size())
		{
			glDeleteShader(shaderId.back());
			shaderId.pop_back();
		}
	}

	GLuint ShaderProgram::getProgramID() const
	{
		return programId;
	}

	void ShaderProgram::use() {
		glUseProgram(programId);
	}

	void ShaderProgram::bindAttrib(std::string name, GLuint index)
	{
	}

	void ShaderProgram::vertexAttribPointer(std::string name, GLint size, GLenum type, GLsizei stride, const GLvoid* data, bool normalize) {
		GLint location;
		auto iter = attribMap.find(name);
		if (iter == attribMap.end()) {
			location = glGetAttribLocation(programId, name.c_str());
			attribMap[name] = location;
		}
		else {
			location = attribMap[name];
		}
		glEnableVertexAttribArray(location);
		glVertexAttribPointer(location, size, type, normalize, stride, data);
	}

	GLuint ShaderProgram::getUniformLocation(std::string name) {
		auto iter = uniformMap.find(name);
		if (iter == uniformMap.end()) {
			GLint loc = glGetUniformLocation(programId, name.c_str());
			uniformMap[name] = loc;
			return loc;
		}
		else {
			return iter->second;
		}
	}

	void ShaderProgram::CreateShader(std::string &fileName, SHADER_TYPE stype)
	{
//		GLint   status;
	//	char      logBuffer[1024];
//		GLsizei	logLength;

		int sID = 0;

		switch (stype) {
		case SHADER_TYPE::VERTEX:
			sID = glCreateShader(GL_VERTEX_SHADER_ARB);
			break;
		case SHADER_TYPE::FRAGMENT:
			sID = glCreateShader(GL_FRAGMENT_SHADER_ARB);
			break;
		case SHADER_TYPE::GEOMETRY:
			sID = glCreateShader(GL_GEOMETRY_SHADER_EXT);
			break;
		}

		shaderId.push_back(sID);

		unsigned& id = shaderId.back();

		/*
		glShaderSource(id, 1, &sourceCS, 0);
		glCompileShader(id);
		glGetShaderiv(id, GL_COMPILE_STATUS, &status);

		if (!status)
		{
			glGetShaderInfoLog(id, 1024, &logLength, logBuffer);
			printf("%d | %s\n", logLength, logBuffer);

			printf("Shader compile error 0!!\n");
			glDeleteShader(id);
			id = 0;

			//return false;
		}
		*/


		std::ifstream shaderSource(fileName.c_str());
		if (!shaderSource.is_open()) {
			std::cerr << "File not found " << fileName.c_str() << "\n";
			shaderId.pop_back();

			//exit(EXIT_FAILURE);
			CreateDefaultShader(stype);
		}

		else
		{		// now read in the data
			std::string source((std::istreambuf_iterator<char>(shaderSource)), std::istreambuf_iterator<char>());
			shaderSource.close();
			source += "\0";

			const char* data = source.c_str();
			glShaderSource(id, 1, &data, nullptr);
			glCompileShader(id);

			GLint isCompiled = 0;
			glGetShaderiv(id, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE) {
				GLint maxLength = 0;
				glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

				//The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(id, maxLength, &maxLength, &infoLog[0]);

				std::cout << &infoLog[0] << std::endl;

				//We don't need the shader anymore.
				//glDeleteShader(vertexShader);

				//Use the infoLog as you see fit.

				//In this simple program, we'll just leave
				//return;
			}
			glAttachShader(programId, id);
		}
	}
	void ShaderProgram::CreateDefaultShader(SHADER_TYPE stype)
	{
		const GLchar *vertex_shader =
			"#version 330\n"
			"	layout(location = 0) in vec4 position;\n"
			"uniform mat4 mvp;\n"
			"void main() {\n"
			"	gl_Position = mvp * position;\n"
			"}\n";


		const GLchar* fragment_shader =
			"#version 330\n"
			"uniform sampler2D positionMap;\n"
			"uniform sampler2D colorMap; \n"
			"uniform sampler2D normalMap; \n"
			"uniform vec3 lightPos; \n"
			"uniform float lightIntensity; \n"
			"uniform float diffuseConstant; \n"
			"uniform vec2 screenSize; \n"
			"uniform vec3 lightColor; \n"
			"out vec4 fragColor; \n"
			"vec2 calcTexCoord() {\n"
			"	return gl_FragCoord.xy / screenSize; \n"
			"}\n"
			"void main() {\n"
			"	vec2 texCoord = calcTexCoord(); \n"
			"	vec3 worldPos = texture(positionMap, texCoord).xyz; \n"
			"	vec3 color = texture(colorMap, texCoord).xyz; \n"
			"	vec3 normal = texture(normalMap, texCoord).xyz; \n"
			"	vec3 lightDir = lightPos - worldPos; \n"
			"	float attenuation = lightIntensity / dot(lightDir, lightDir); \n"
			"	lightDir = normalize(lightDir); \n"
			"	vec3 diffuseComponent = (diffuseConstant * dot(normal, lightDir)) * (color * lightColor); \n"
			"	fragColor = vec4(attenuation * diffuseComponent, 1.0); \n"
			"}\n";

		const GLchar* geometry_shader =
			"#version 330 core\n"
			"layout(points) in;\n"
			"layout(triangle_strip, max_vertices = 4) out;\n"
			"out vec2 texcoord;\n"
			"void main()\n"
			"{\n"
			"gl_Position = vec4(1.0, 1.0, 0.5, 1.0);\n"
			"texcoord = vec2(1.0, 1.0);\n"
			"EmitVertex();\n"
			"gl_Position = vec4(-1.0, 1.0, 0.5, 1.0);\n"
			"texcoord = vec2(0.0, 1.0);\n"
			"EmitVertex();\n"
			"gl_Position = vec4(1.0, -1.0, 0.5, 1.0);\n"
			"texcoord = vec2(1.0, 0.0);\n"
			"EmitVertex();\n"
			"gl_Position = vec4(-1.0, -1.0, 0.5, 1.0);\n"
			"texcoord = vec2(0.0, 0.0);\n"
			"EmitVertex();\n"
			"EndPrimitive();\n"
			"}\n";


		const GLchar * data;

		int sID = 0;

		switch (stype) {
		case SHADER_TYPE::VERTEX:
			sID = glCreateShader(GL_VERTEX_SHADER_ARB);
			data = vertex_shader;
			break;
		case SHADER_TYPE::FRAGMENT:
			sID = glCreateShader(GL_FRAGMENT_SHADER_ARB);
			data = fragment_shader;
			break;
		case SHADER_TYPE::GEOMETRY:
			sID = glCreateShader(GL_GEOMETRY_SHADER_EXT);
			data = geometry_shader;
			break;
		}
		shaderId.push_back(sID);

		unsigned& id = shaderId.back();


		// Backup GL state
		GLint last_texture, last_array_buffer, last_vertex_array;
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);


		glShaderSource(id, 1, &data, nullptr);
		glCompileShader(id);

		GLint isCompiled = 0;
		glGetShaderiv(id, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

			//The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(id, maxLength, &maxLength, &infoLog[0]);

			std::cout << &infoLog[0] << std::endl;

			//We don't need the shader anymore.
			//glDeleteShader(vertexShader);

			//Use the infoLog as you see fit.

			//In this simple program, we'll just leave
			//return;
		}
		glAttachShader(programId, id);
	}

	void ShaderProgram::setUniform1f(const char * varname, GLfloat v0) { glUniform1f(getUniformLocation(varname), v0); }
	void ShaderProgram::setUniform2f(const char* varname, GLfloat v0, GLfloat v1) { glUniform2f(getUniformLocation(varname), v0, v1); }
	void ShaderProgram::setUniform3f(const char* varname, GLfloat v0, GLfloat v1, GLfloat v2) { glUniform3f(getUniformLocation(varname), v0, v1, v2); }
	void ShaderProgram::setUniform4f(const char* varname, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) { glUniform4f(getUniformLocation(varname), v0, v1, v2, v3); }
	void ShaderProgram::setUniform1i(const char* varname, GLint v0) { glUniform1i(getUniformLocation(varname), v0); }
	void ShaderProgram::setUniform2i(const char* varname, GLint v0, GLint v1) { glUniform2i(getUniformLocation(varname), v0, v1); }
	void ShaderProgram::setUniform3i(const char* varname, GLint v0, GLint v1, GLint v2) { glUniform3i(getUniformLocation(varname), v0, v1, v2); }
	void ShaderProgram::setUniform4i(const char* varname, GLint v0, GLint v1, GLint v2, GLint v3) { glUniform4i(getUniformLocation(varname), v0, v1, v2, v3); }
	void ShaderProgram::setUniform1ui(const char* varname, GLuint v0) { glUniform1ui(getUniformLocation(varname), v0); }
	void ShaderProgram::setUniform2ui(const char* varname, GLuint v0, GLuint v1) { glUniform2ui(getUniformLocation(varname), v0, v1); }
	void ShaderProgram::setUniform3ui(const char* varname, GLuint v0, GLuint v1, GLuint v2) { glUniform3ui(getUniformLocation(varname), v0, v1, v2); }
	void ShaderProgram::setUniform4ui(const char* varname, GLuint v0, GLuint v1, GLuint v2, GLuint v3) { glUniform4ui(getUniformLocation(varname), v0, v1, v2, v3); }
	void ShaderProgram::setUniform1fv(const char* varname, GLsizei count, const GLfloat* value) { glUniform1fv(getUniformLocation(varname), count, value); }
	void ShaderProgram::setUniform2fv(const char* varname, GLsizei count, const GLfloat* value) { glUniform2fv(getUniformLocation(varname), count, value); }
	void ShaderProgram::setUniform3fv(const char* varname, GLsizei count, const GLfloat* value) { glUniform3fv(getUniformLocation(varname), count, value); }
	void ShaderProgram::setUniform4fv(const char* varname, GLsizei count, const GLfloat* value) { glUniform4fv(getUniformLocation(varname), count, value); }
	void ShaderProgram::setUniform1iv(const char* varname, GLsizei count, const GLint* value) { glUniform1iv(getUniformLocation(varname), count, value); }
	void ShaderProgram::setUniform2iv(const char* varname, GLsizei count, const GLint* value) { glUniform2iv(getUniformLocation(varname), count, value); }
	void ShaderProgram::setUniform3iv(const char* varname, GLsizei count, const GLint* value) { glUniform3iv(getUniformLocation(varname), count, value); }
	void ShaderProgram::setUniform4iv(const char* varname, GLsizei count, const GLint* value) { glUniform4iv(getUniformLocation(varname), count, value); }
	void ShaderProgram::setUniform1uiv(const char* varname, GLsizei count, const GLuint* value) { glUniform1uiv(getUniformLocation(varname), count, value); }
	void ShaderProgram::setUniform2uiv(const char* varname, GLsizei count, const GLuint* value) { glUniform2uiv(getUniformLocation(varname), count, value); }
	void ShaderProgram::setUniform3uiv(const char* varname, GLsizei count, const GLuint* value) { glUniform3uiv(getUniformLocation(varname), count, value); }
	void ShaderProgram::setUniform4uiv(const char* varname, GLsizei count, const GLuint* value) { glUniform4uiv(getUniformLocation(varname), count, value); }
	void ShaderProgram::setUniformMatrix2fv(const char* varname, GLsizei count, GLboolean transpose, const GLfloat* value) { glUniformMatrix2fv(getUniformLocation(varname), count, transpose, value); }
	void ShaderProgram::setUniformMatrix3fv(const char* varname, GLsizei count, GLboolean transpose, const GLfloat* value) { glUniformMatrix3fv(getUniformLocation(varname), count, transpose, value); }
	void ShaderProgram::setUniformMatrix4fv(const char* varname, GLsizei count, GLboolean transpose, const GLfloat* value) { glUniformMatrix4fv(getUniformLocation(varname), count, transpose, value); }
	void ShaderProgram::setUniformMatrix2x3fv(const char* varname, GLsizei count, GLboolean transpose, const GLfloat* value) { glUniformMatrix2x3fv(getUniformLocation(varname), count, transpose, value); }
	void ShaderProgram::setUniformMatrix3x2fv(const char* varname, GLsizei count, GLboolean transpose, const GLfloat* value) { glUniformMatrix3x2fv(getUniformLocation(varname), count, transpose, value); }
	void ShaderProgram::setUniformMatrix2x4fv(const char* varname, GLsizei count, GLboolean transpose, const GLfloat* value) { glUniformMatrix2x4fv(getUniformLocation(varname), count, transpose, value); }
	void ShaderProgram::setUniformMatrix4x2fv(const char* varname, GLsizei count, GLboolean transpose, const GLfloat* value) { glUniformMatrix4x2fv(getUniformLocation(varname), count, transpose, value); }
	void ShaderProgram::setUniformMatrix3x4fv(const char* varname, GLsizei count, GLboolean transpose, const GLfloat* value) { glUniformMatrix3x4fv(getUniformLocation(varname), count, transpose, value); }
	void ShaderProgram::setUniformMatrix4x3fv(const char* varname, GLsizei count, GLboolean transpose, const GLfloat* value) { glUniformMatrix4x3fv(getUniformLocation(varname), count, transpose, value); }
}