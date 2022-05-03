#include "xEasyGL.h"

namespace xlonlat
{
	namespace xEasyGL
	{
		xTexture::xTexture() :
			m_id(0), m_width(0), m_height(0), m_channel(0)
		{

		}

		xTexture::~xTexture()
		{
			Clear();
		}

		void xTexture::Clear()
		{
			if (m_id > 0) glDeleteTextures(1, &m_id);
			m_id = m_width = m_height = m_channel = 0;
		}

		bool xTexture::Load(const wchar_t* file)
		{
			unsigned char* data = xGlobal::Instance().ReadImage(file, m_width, m_height, m_channel);
			if (data)
			{
				GLenum format = GL_RGB;
				if (m_channel == 3) 
				{
					format = GL_RGB;
				}
				else if (m_channel == 4)
				{
					format = GL_RGBA;
				}
				else
				{
					free(data);
					return false;
				}

				Load(data, format);
				free(data);
				return true;
			}

			Clear();
			return false;
		}

		void xTexture::Load(const unsigned char* data, GLenum format)
		{
			glGenTextures(1, &m_id);
			glBindTexture(GL_TEXTURE_2D, m_id);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				m_channel,
				m_width,
				m_height,
				0,
				format,
				GL_UNSIGNED_BYTE,
				data);
		}

		GLuint xTexture::ID() const
		{
			return m_id;
		}

		size_t xTexture::Width() const
		{
			return m_width;
		}

		size_t xTexture::Height() const
		{
			return m_height;
		}

		size_t xTexture::Channel() const
		{
			return m_channel;
		}

		bool xTexture::Available() const
		{
			return (m_id > 0 && m_width > 0 && m_height > 0 && m_channel > 0);
		}
	}
}
