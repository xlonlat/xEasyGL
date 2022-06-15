#include "xEasyGL.h"

namespace xlonlat
{
	namespace xEasyGL
	{
		xViewer::xViewer(xCamera* camera/* = nullptr */) : 
			m_RenderThreadID(-1)
		{
			camera == nullptr ? m_camera = new xDefaultCamera() : m_camera = camera;

			char* aaa = new char[4];
		}

		xViewer::~xViewer(void)
		{
			delete m_camera;
		}

		xCamera& xViewer::Camera() const
		{
			return *m_camera;
		}

		void xViewer::AttachLayer(xLayer* layer)
		{
			if (layer == nullptr) return;
			layer->OnAttach();
			m_Layers.push_back(layer);
		}

		void xViewer::DetachLayer(xLayer* layer)
		{
			if (layer == nullptr) return;
			for (auto itor = m_Layers.begin(); itor != m_Layers.end(); itor++)
			{
				if (*itor == layer)
				{
					layer->OnDetach();
					m_Layers.erase(itor);
					break;
				}
			}
		}

		void xViewer::Initialize()
		{
			m_RenderThreadID = xGlobal::Instance().GetCurrentThread();

			const std::wstring& img = std::wstring(xGlobal::Instance().ResourcePath()) + L"images\\opengl_logo.jpeg";
			m_logoImg.Load(img.c_str());

			const std::wstring& vs_file = std::wstring(xGlobal::Instance().ResourcePath()) + L"shaders\\sample.vs";
			const std::wstring& fs_file = std::wstring(xGlobal::Instance().ResourcePath()) + L"shaders\\sample.fs";
			m_sampleShader.Load(vs_file.c_str(), fs_file.c_str());
		}

		void xViewer::Render()
		{
			m_camera->Update();

			int width = (int)m_camera->State().vs.w;
			int height = (int)m_camera->State().vs.h;

			glClearDepth(1.0f);
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glEnable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			Begin3D();
			{
				m_sampleShader.Enable();
				glDrawArrays(GL_TRIANGLES, 0, 3);
				m_sampleShader.Disable();

				glBegin(GL_LINES);
				for (int i = -100; i <= 100; i += 2)
				{
					glColor3f(0.4f, 0.4f, 0.4f); glVertex3f(i * 1.0f, 100.0f, 0.0f);
					glColor3f(0.4f, 0.4f, 0.4f); glVertex3f(i * 1.0f, -100.0f, 0.0f);

					glColor3f(0.4f, 0.4f, 0.4f); glVertex3f(100.0f, i * 1.0f, 0.0f);
					glColor3f(0.4f, 0.4f, 0.4f); glVertex3f(-100.0f, i * 1.0f, 0.0f);
				}
				glEnd();

				for (auto layer : m_Layers)
				{
					layer->OnDraw3D();
				}
			}

			Begin2D();
			{
				glColor4d(0.0, 1.0, 0.0, 1.0);
				glBegin(GL_LINES);
				glVertex2f(width / 2.f - 10.f, height / 2.f);
				glVertex2f(width / 2.f + 10.f, height / 2.f);
				glVertex2f(width / 2.f, height / 2.f - 10.f);
				glVertex2f(width / 2.f, height / 2.f + 10.f);
				glEnd();

				if (m_logoImg.Available())
				{
					float img_w = 128.f;
					float img_h = img_w * m_logoImg.Height() / m_logoImg.Width();
					float pos_x = width - img_w - 10.f;
					float pos_y = height - img_h - 10.f;
					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, m_logoImg.ID());
					glFrontFace(GL_CCW);
					glColor4d(1.0, 1.0, 1.0, 1.0);
					glBegin(GL_QUADS);
					glTexCoord2f(0.0f, 0.0f); glVertex2f(pos_x + 0.0f,  pos_y + 0.0f);
					glTexCoord2f(0.0f, 1.0f); glVertex2f(pos_x + 0.0f,  pos_y + img_h);
					glTexCoord2f(1.0f, 1.0f); glVertex2f(pos_x + img_w, pos_y + img_h);
					glTexCoord2f(1.0f, 0.0f); glVertex2f(pos_x + img_w, pos_y + 0.0f);
					glEnd();
					glDisable(GL_TEXTURE_2D);
				}

				for (auto layer : m_Layers)
				{
					layer->OnDraw2D();
				}
			}
		}

		void xViewer::Clear()
		{
			for (auto layer : m_Layers)
			{
				layer->OnDetach();
				delete layer;
			}
			m_Layers.clear();
			m_sampleShader.Clear();
			m_logoImg.Clear();
		}

		void xViewer::OnEvent(const xEvent& event)
		{
			if (m_camera != nullptr)
			{
				m_camera->Event(event);
			}
			for (auto layer : m_Layers)
			{
				layer->Event(event);
			}
		}

		void xViewer::Begin2D()
		{
			const xViewportState& vs = m_camera->State().vs;

			// Set viewport.
			glViewport(0, 0, vs.w, vs.h);

			// Set projection matrix.
			glm::mat4 proj = glm::ortho(0.0f, (float)vs.w, (float)vs.h, 0.0f);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glLoadMatrixf(glm::value_ptr(proj));

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
		}

		void xViewer::Begin3D()
		{
			const xViewportState& vs = m_camera->State().vs;
			const xProjState& ps = m_camera->State().ps;

			// Set viewport .
			glViewport(0, 0, vs.w, vs.h);

			// Set projection matrix.
			const glm::mat4& proj = m_camera->ProjMat();
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glLoadMatrixf(glm::value_ptr(proj));

			const xCameraState& cam = m_camera->State();
			const glm::mat4& model = glm::lookAtRH(cam.eye, cam.tar, cam.up);
			const glm::mat4& model2 = m_camera->ViewMat();
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glLoadMatrixf(glm::value_ptr(model2));

			//static int print = 0;
			//if (!print++)
			//{
			//    xCommon::PrintMatrix4x4f(glm::value_ptr(model));
			//}
		}
	}
}
