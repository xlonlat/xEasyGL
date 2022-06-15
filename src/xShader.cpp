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


		xShaderToy::xShaderToy(const wchar_t* file)
		{
			const std::wstring& vs_file = std::wstring(xGlobal::Instance().ResourcePath()) + L"shaders\\canvas.vs";

			m_Shader = new xShader();
			m_Shader->Load(vs_file.c_str(), file);

			m_Block.iResolution = glm::vec3(0);

			m_StartClock = 0;
			m_FrameCount = 0;
		}

		xShaderToy::~xShaderToy()
		{
			m_Shader->Clear();
			delete m_Shader;
		}

		void xShaderToy::Draw(const xViewer* viewer)
		{
			const xCamera& camera = viewer->Camera();

			clock_t curr_clock = clock();
			if (m_StartClock == 0) m_StartClock = curr_clock;

			m_Block.iResolution = glm::vec3(camera.State().vs.w, camera.State().vs.h, 0);
			m_Block.iTime		= (curr_clock - m_StartClock) / 1000.f;
			m_Block.iTimeDelta  = camera.Timespan() / 1000.f;
			m_Block.iFrame		= m_FrameCount++;
			m_Block.iMouse		= glm::vec4(m_MousePosition, 1);

			m_Shader->Enable();
			{
				GLint iResolutionLoc	= glGetUniformLocation(m_Shader->ID(), "iResolution");
				GLint iTimeLoc			= glGetUniformLocation(m_Shader->ID(), "iTime");
				GLint iTimeDeltaLoc		= glGetUniformLocation(m_Shader->ID(), "iTimeDelta");
				GLint iFrameLoc			= glGetUniformLocation(m_Shader->ID(), "iFrame");
				GLint iMouseLoc			= glGetUniformLocation(m_Shader->ID(), "iMouse");

				glUniform3f(iResolutionLoc, m_Block.iResolution.x, m_Block.iResolution.y, m_Block.iResolution.z);
				glUniform1f(iTimeLoc,		m_Block.iTime);
				glUniform1f(iTimeDeltaLoc,	m_Block.iTimeDelta);
				glUniform1i(iFrameLoc,		m_Block.iFrame);
				glUniform4f(iMouseLoc,		m_Block.iMouse.x, m_Block.iMouse.y, m_Block.iMouse.z, m_Block.iMouse.w);
			}
			glDrawArrays(GL_TRIANGLES, 0, 6);
			m_Shader->Disable();
		}

		void xShaderToy::OnEvent(const xEvent& event)
		{
			if (event.type == xEventType::MouseMove)
			{
				m_MousePosition.x = event.x;
				m_MousePosition.y = event.y;
				m_MousePosition.z = (event.tag == 0);
			}
			if (event.type == xEventType::MouseUp)
			{
				m_MousePosition.z = 0;
			}
		}
	}
}
