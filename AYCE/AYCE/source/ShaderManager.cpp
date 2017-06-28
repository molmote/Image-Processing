/*****************************************************************************/
/*!
\file   ShaderManager.cpp
\author Yeongki Baek
\par    email: yeongki.baek\@digipen.edu
\par    GAM300
\par    Pre-Production
\date   07/12/2016
\brief
This is the interface file for ShaderManager features
Copyright 2016, Digipen Institute of Technology
*/
/*****************************************************************************/
#include "ShaderManager.h"

namespace VK
{
	// Global static pointer used to ensure a single instance of the class.
	ShaderManager* ShaderManager::m_pInstance = nullptr;

	ShaderManager::ShaderManager()
	{
//		gbuffer = GraphicsRenderer::GetInstance()->getGbuffer();
		//gbuffer = new GBuffer();
	}

	ShaderManager::~ShaderManager()
	{
	}

	std::string	ShaderManager::LoadShaderFromFile(const char* filename) const
	{
		std::ifstream file;
		std::string data;

		char buffer[150];

		file.open(filename);

		if (file.is_open())
		{
			// Read in data
			while (file.getline(buffer, 150))
			{
				data += buffer;
				data += "\n";
			}
		}

		return data;

	}

	ShaderManager* ShaderManager::GetInstance()
	{
		if (!m_pInstance)
			m_pInstance = new ShaderManager();

		return m_pInstance;
	}

	////////////////////////////////////////////////////////////////////////////////////////////

	// Shader ids
	typedef std::deque<unsigned> ShaderIdList;
	static ShaderIdList shaderIds;

	// Program id
	unsigned programId;


	bool ShaderManager::gameCreateVertexShader(const char* fileName)
	{
		GLint   status;
		char      logBuffer[1024];
		GLsizei	logLength;

		int sID = glCreateShader(GL_VERTEX_SHADER);
		shaderIds.push_back(sID);

		unsigned& id = shaderIds.back();

		// Load shader code into one long string
		std::string source = LoadShaderFromFile(fileName);
		const char* sourceCS = source.c_str();

		glShaderSource(id, 1, &sourceCS, nullptr);
		glCompileShader(id);
		glGetShaderiv(id, GL_COMPILE_STATUS, &status);

		if (!status)
		{
			glGetShaderInfoLog(id, 1024, &logLength, logBuffer);
			printf("%d | %s\n", logLength, logBuffer);

			printf("Shader compile error 0!!\n");
			glDeleteShader(id);
			id = 0;

			return false;
		}

		return true;
	}

	// ---------------------------------------------------------------------------

	bool ShaderManager::gameCreatePixelShader(const char* fileName)
	{
		GLint   status;
		char      logBuffer[1024];
		GLsizei	logLength;

		shaderIds.push_back(glCreateShader(GL_FRAGMENT_SHADER));

		unsigned& id = shaderIds.back();

		// Load shader code into one long string
		std::string source = LoadShaderFromFile(fileName);
		const char* sourceCS = source.c_str();

		glShaderSource(id, 1, &sourceCS, nullptr);
		glCompileShader(id);
		glGetShaderiv(id, GL_COMPILE_STATUS, &status);

		if (!status)
		{
			glGetShaderInfoLog(id, 1024, &logLength, logBuffer);
			printf("%d | %s\n", logLength, logBuffer);

			printf("Shader compile error 1!!\n");
			glDeleteShader(id);
			id = 0;

			return false;
		}

		return true;
	}

	// ---------------------------------------------------------------------------

	bool ShaderManager::gameInitGLSL()
	{
		GLint		status;
		char		logBuffer[1024];
		GLsizei		logLength;

		programId = glCreateProgram();

		// Attach all shaders
		for each(unsigned id in shaderIds)
		{
			glAttachShader(programId, id);
		}

		glLinkProgram(programId);
		glGetProgramiv(programId, GL_LINK_STATUS, &status);

		if (!status)
		{
			glGetProgramInfoLog(programId, 1024, &logLength, logBuffer);
			printf("%d | %s\n", logLength, logBuffer);

			printf("Program link error!!\n");
			glDeleteProgram(programId);

			// Delete all shaders
			for each(unsigned id in shaderIds)
			{
				glDeleteShader(id);
			}

			shaderIds.clear();

			return false;
		}

		// Use program
		if (programId)
		{
			glUseProgram(programId);
		}

		return true;
	}

} //namespace VK