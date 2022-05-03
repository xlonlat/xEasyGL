#include "xEasyGL.h"

namespace xlonlat
{
	namespace xEasyGL
	{
		xShader::xShader() :
			m_id(0), m_vs(0), m_fs(0)
		{

		}

		xShader::~xShader()
		{
			Clear();
		}

		void xShader::Clear()
		{
			if (m_vs > 0) glDeleteShader(m_vs);
			if (m_fs > 0) glDeleteShader(m_fs);
			if (m_id > 0) glDeleteProgram(m_id);

			m_vs = m_fs = m_id = 0;
		}

		bool xShader::Load(const wchar_t* vs_file, const wchar_t* fs_file)
		{
			std::ifstream vsFile;
			std::ifstream fsFile;

			std::stringstream vsStream;
			std::stringstream fsStream;
			std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;

			vsFile.open(conv.to_bytes(vs_file).c_str(), std::ios_base::in);
			if (vsFile.is_open())
			{
				vsStream << vsFile.rdbuf();
				vsFile.close();
			}

			fsFile.open(conv.to_bytes(fs_file).c_str(), std::ios_base::in);
			if (fsFile.is_open())
			{
				fsStream << fsFile.rdbuf();
				fsFile.close();
			}

			return Load(vsStream.str().c_str(), fsStream.str().c_str());
		}

		bool xShader::Load(const char* vs_code, const char* fs_code)
		{
			GLint result = GL_FALSE;
			char arrInfo[512] = { 0 };

			/* Initialize the vertex shader. */
			if (vs_code)
			{
				m_vs = glCreateShader(GL_VERTEX_SHADER);
				glShaderSource(m_vs, 1, &vs_code, NULL);
				glCompileShader(m_vs);

				glGetShaderiv(m_vs, GL_COMPILE_STATUS, &result);
				if (result == GL_FALSE)
				{
					glGetShaderInfoLog(m_vs, sizeof(arrInfo), NULL, arrInfo);
					goto FAILED;
				}
			}

			/* Initialize the fragment shader. */
			if (fs_code)
			{
				m_fs = glCreateShader(GL_FRAGMENT_SHADER);
				glShaderSource(m_fs, 1, &fs_code, NULL);
				glCompileShader(m_fs);

				glGetShaderiv(m_fs, GL_COMPILE_STATUS, &result);
				if (result == GL_FALSE)
				{
					glGetShaderInfoLog(m_fs, sizeof(arrInfo), NULL, arrInfo);
					goto FAILED;
				}
			}

			/* Initialize the shader program. */
			m_id = glCreateProgram();
			if (vs_code) glAttachShader(m_id, m_vs);
			if (fs_code) glAttachShader(m_id, m_fs);
			glLinkProgram(m_id);

			glGetProgramiv(m_id, GL_LINK_STATUS, &result);
			if (result == GL_FALSE)
			{
				glGetProgramInfoLog(m_id, sizeof(arrInfo), NULL, arrInfo);
				goto FAILED;
			}

			/* Release the shader resource. */
			glDeleteShader(m_vs);
			glDeleteShader(m_fs);

			return true;

		FAILED:
			Clear();
			return false;
		}

		void xShader::GetInfo(char info[512]) const
		{
			glGetShaderInfoLog(m_id, (GLsizei)strlen(info), NULL, info);
		}

		GLuint xShader::ID() const
		{
			return m_id;
		}

		void xShader::Enable() const
		{
			glUseProgram(m_id);
		}

		void xShader::Disable() const
		{
			glUseProgram(0);
		}
	}
}
