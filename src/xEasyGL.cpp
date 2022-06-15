#include "xEasyGL.h"

#ifdef XEASYGL_PLATFORM_WINDOWS
#ifdef APIENTRY
#undef APIENTRY
#endif // APIENTRY
#include <windows.h>
#endif // XEASYGL_PLATFORM_WINDOWS

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif // STB_IMAGE_IMPLEMENTATION

#include <stb/stb_image.h>

namespace xlonlat
{
	namespace xEasyGL
	{
		xGlobal& xGlobal::m_instance = xGlobal::xGlobal();

		xGlobal::xGlobal()
		{
			wchar_t szFilePath[MAX_PATH+1] = { 0 };
			GetModuleFileNameW(NULL, szFilePath, MAX_PATH);
			(wcsrchr(szFilePath, ('\\')))[1] = 0;
			
			memset(m_resourcePath, 0, sizeof(m_resourcePath));
			wcscpy(m_resourcePath, szFilePath);
			wcscat(m_resourcePath, L"..\\..\\res\\");
		}

		xGlobal::xGlobal(const xGlobal&)
		{
			memset(m_resourcePath, 0, sizeof(m_resourcePath));
		}

		const xGlobal& xGlobal::Instance()
		{
			return m_instance;
		}

		const wchar_t* xGlobal::ResourcePath() const
		{
			return m_resourcePath;
		}

		unsigned char* xGlobal::ReadImage(const wchar_t* file, int& width, int& height, int& channel) const
		{
			int len = WideCharToMultiByte(CP_ACP, 0, file, (int)wcslen(file), NULL, 0, NULL, NULL);
			char* achar = new char[len + 1];
			WideCharToMultiByte(CP_ACP, 0, file, (int)wcslen(file), achar, len, NULL, NULL);
			achar[len] = '\0';
			unsigned char* data = stbi_load(achar, &width, &height, &channel, 0);
			delete[] achar;
			return data;
		}

		int xGlobal::GetCurrentThread() const
		{
			return (int)::GetCurrentThreadId();
		}
	


		void xEventParser::Event(const xEvent& event)
		{
			OnEvent(event);

			switch (event.type)
			{
			case xEventType::MouseUp:
			{
				if (event.tag == 0) OnLButtonUp(event.x, event.y);
				else if (event.tag == 1) OnRButtonUp(event.x, event.y);
				else if (event.tag == 2) OnMButtonUp(event.x, event.y);
			} break;
			case xEventType::MouseDown:
			{
				if (event.tag == 0) OnLButtonDown(event.x, event.y);
				else if (event.tag == 1) OnRButtonDown(event.x, event.y);
				else if (event.tag == 2) OnMButtonDown(event.x, event.y);
			} break;
			case xEventType::MouseWheel:
			{
				OnMouseWheel(event.x, event.y, event.tag);
			} break;
			case xEventType::MouseMove:
			{
				OnMouseMove(event.x, event.y, event.tag);
			} break;
			case xEventType::Resize:
			{
				OnSize(event.x, event.y);
			} break;
			case xEventType::KeyUp:
			{
				OnKeyUp(event.tag);
			} break;
			case xEventType::KeyDown:
			{
				OnKeyDown(event.tag);
			} break;
			}
		}


		xUniformBuffer::xUniformBuffer(uint32_t size, uint32_t binding) :
			m_ID(0), m_Binding(binding), m_Size(size)
		{
			glGenBuffers(1, &m_ID);
			glBindBuffer(GL_UNIFORM_BUFFER, m_ID);
			glBufferData(GL_UNIFORM_BUFFER, m_Size, nullptr, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}

		xUniformBuffer::~xUniformBuffer()
		{
			if (m_ID > 0)
			{
				glDeleteBuffers(1, &m_ID);
				m_ID = m_Binding = m_Size = 0;
			}
		}

		void xUniformBuffer::Bind() const
		{
			glBindBufferBase(GL_UNIFORM_BUFFER, m_Binding, m_ID);
		}

		void xUniformBuffer::Unbind() const
		{
			glBindBufferBase(GL_UNIFORM_BUFFER, m_Binding, 0);
		}

		void xUniformBuffer::Update(const void* data) const
		{
			glBindBuffer(GL_UNIFORM_BUFFER, m_ID);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, m_Size, data);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}

	}
}
