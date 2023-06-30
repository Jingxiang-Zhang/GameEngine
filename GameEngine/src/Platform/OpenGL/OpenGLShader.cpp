#include "gepch.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>

namespace GE {

	namespace Utils {

		static GLenum ShaderTypeFromString(const std::string& type)
		{
			if (type == "vertex")
				return GL_VERTEX_SHADER;
			if (type == "fragment" || type == "pixel")
				return GL_FRAGMENT_SHADER;

			GE_CORE_ASSERT(false, "Unknown shader type!");
			return 0;
		}

		//static shaderc_shader_kind GLShaderStageToShaderC(GLenum stage)
		//{
		//	switch (stage)
		//	{
		//	case GL_VERTEX_SHADER:   return shaderc_glsl_vertex_shader;
		//	case GL_FRAGMENT_SHADER: return shaderc_glsl_fragment_shader;
		//	}
		//	HZ_CORE_ASSERT(false);
		//	return (shaderc_shader_kind)0;
		//}

		//static const char* GLShaderStageToString(GLenum stage)
		//{
		//	switch (stage)
		//	{
		//	case GL_VERTEX_SHADER:   return "GL_VERTEX_SHADER";
		//	case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
		//	}
		//	HZ_CORE_ASSERT(false);
		//	return nullptr;
		//}

		//static const char* GetCacheDirectory()
		//{
		//	// TODO: make sure the assets directory is valid
		//	return "assets/cache/shader/opengl";
		//}

		//static void CreateCacheDirectoryIfNeeded()
		//{
		//	std::string cacheDirectory = GetCacheDirectory();
		//	if (!std::filesystem::exists(cacheDirectory))
		//		std::filesystem::create_directories(cacheDirectory);
		//}

		//static const char* GLShaderStageCachedOpenGLFileExtension(uint32_t stage)
		//{
		//	switch (stage)
		//	{
		//	case GL_VERTEX_SHADER:    return ".cached_opengl.vert";
		//	case GL_FRAGMENT_SHADER:  return ".cached_opengl.frag";
		//	}
		//	HZ_CORE_ASSERT(false);
		//	return "";
		//}

		//static const char* GLShaderStageCachedVulkanFileExtension(uint32_t stage)
		//{
		//	switch (stage)
		//	{
		//	case GL_VERTEX_SHADER:    return ".cached_vulkan.vert";
		//	case GL_FRAGMENT_SHADER:  return ".cached_vulkan.frag";
		//	}
		//	HZ_CORE_ASSERT(false);
		//	return "";
		//}

	}

	OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string fragmentSrc)
		:m_RendererID(0)
	{
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(sources);
	}

	OpenGLShader::OpenGLShader(const std::string& filepath)
		: m_FilePath(filepath)
	{
		// Utils::CreateCacheDirectoryIfNeeded();

		std::string source = ReadFile(filepath);
		auto shaderSources = PreProcess(source);
		Compile(shaderSources);
		//{
		//	Timer timer;
		//	CompileOrGetVulkanBinaries(shaderSources);
		//	CompileOrGetOpenGLBinaries();
		//	CreateProgram();
		//	HZ_CORE_WARN("Shader creation took {0} ms", timer.ElapsedMillis());
		//}

		// Extract name from filepath
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.');
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filepath.substr(lastSlash, count);
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string> shaderSource)
	{
		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		
		GLuint program = glCreateProgram();
		std::vector<GLenum> glShaderIDs;

		for (auto& key : shaderSource) 
		{
			GLenum type = key.first;
			const std::string& source = key.second;

			// Create an empty vertex shader handle
			GLuint shader = glCreateShader(type);
			// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
			const GLchar* sourceStr = source.c_str();
			glShaderSource(shader, 1, &sourceStr, 0);

			// Compile the vertex shader
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);

				GE_CORE_ERROR("{0}", infoLog.data());
				GE_CORE_ASSERT(false, "Shader compilation failer!");
				return;
			}
			// Attach our shaders to our program
			glAttachShader(program, shader);
			glShaderIDs.push_back(shader);
		}

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.

			for(auto id: glShaderIDs)
				glDeleteShader(id);

			GE_CORE_ERROR("{0}", infoLog.data());
			GE_CORE_ASSERT(false, "Shader link failer!");
			return;
		}

		// Always detach shaders after a successful link.
		for (auto id : glShaderIDs)
			glDetachShader(program, id);

		// after the shader compilation is successful, assign the shader id
		m_RendererID = program;
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		std::string result;
		// we don't want to read the file with text format, because C++ will try to
		// interpret that file if it text format
		std::ifstream in(filepath, std::ios::in | std::ios::binary); // ifstream closes itself due to RAII
		if (in)
		{
			in.seekg(0, std::ios::end);		// go the the end of the file
			size_t size = in.tellg();		// get the size of the file
			if (size != -1)
			{
				result.resize(size);		// read all the file at once
				in.seekg(0, std::ios::beg);
				in.read(&result[0], size);
				in.close();
			}
			else
			{
				GE_CORE_ERROR("Could not read from file '{0}'", filepath);
			}
		}
		else
		{
			GE_CORE_ERROR("Could not open file '{0}'", filepath);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0); //Start of shader type declaration line
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos); //End of shader type declaration line
			GE_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1; //Start of shader type name (after "#type " keyword)
			std::string type = source.substr(begin, eol - begin);
			GE_CORE_ASSERT(Utils::ShaderTypeFromString(type), "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol); //Start of shader code after shader type declaration line
			GE_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
			pos = source.find(typeToken, nextLinePos); //Start of next shader type declaration line

			shaderSources[Utils::ShaderTypeFromString(type)] = (pos == std::string::npos) ? 
				source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}
		return shaderSources;
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, int value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformIntArray(const std::string& name, int* values, uint32_t count)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1iv(location, count, values);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}


}