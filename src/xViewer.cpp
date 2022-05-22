#include "xEasyGL.h"

namespace xlonlat
{
	namespace xEasyGL
	{
		xViewer::xViewer(int width, int height, xCamera* camera/* = nullptr */)
		{
			xCameraState cam{};
			cam.eye = glm::vec3(0.f, 0.f, 300.f);
			cam.tar = glm::vec3(0.f, 0.f, 0.f);
			cam.up = glm::vec3(0.f, 1.f, 0.f);
			cam.vs.x = 0;
			cam.vs.y = 0;
			cam.vs.w = width;
			cam.vs.h = height;
			cam.ps.zNear = 0.1f;
			cam.ps.zFar = 1000.0f;
			cam.ps.fovy = 45.0f;
			cam.ps.aspect = 1.0f * width / height;

			camera == nullptr ? m_camera = new xFirstPersonCamera() : m_camera = camera;
			m_camera->Link(this);
			m_camera->SetState(cam);
			m_camera->Update();

			m_lastLDown.x = m_lastLDown.y = -1;
			m_lastRDown.x = m_lastRDown.y = -1;
		}

		xViewer::~xViewer(void)
		{
			delete m_camera;
		}

		xCamera& xViewer::Camera() const
		{
			return *m_camera;
		}

		void xViewer::Initialize()
		{
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
			}

			Begin3D();
			{
				m_sampleShader.Enable();
				glPushMatrix();
				glScaled(10.0, 10.0, 10.0);
				glBegin(GL_TRIANGLES);
				glColor3f(1.0f, 0.0f, 0.0f); glVertex3f( 0.0f,  1.0f, 0.0f);
				glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
				glColor3f(0.0f, 0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, 0.0f);
				glEnd();
				glPopMatrix();
				m_sampleShader.Disable();

				glBegin(GL_LINES);
				for (int i=-100; i<=100; i+=2)
				{
					glColor3f(0.4f, 0.4f, 0.4f); glVertex3f(i * 1.0f,  100.0f, 0.0f);
					glColor3f(0.4f, 0.4f, 0.4f); glVertex3f(i * 1.0f, -100.0f, 0.0f);

					glColor3f(0.4f, 0.4f, 0.4f); glVertex3f( 100.0f, i * 1.0f, 0.0f);
					glColor3f(0.4f, 0.4f, 0.4f); glVertex3f(-100.0f, i * 1.0f, 0.0f);
				}
				glEnd();
			}
		}

		void xViewer::Clear()
		{
			m_sampleShader.Clear();
			m_logoImg.Clear();
		}

		void xViewer::Event(const xEvent& event)
		{
			switch (event.type)
			{
			case xEasyGL::Resize:
			{
				xViewportState vs = m_camera->State().vs;
				vs.w = event.x;
				vs.h = event.y;
				m_camera->SetViewport(vs);

				xProjState ps = m_camera->State().ps;
				ps.aspect = 1.0f * vs.w / vs.h;
				m_camera->SetProjection(ps);
			}break;
			default:
				break;
			}
		}

		void xViewer::OnLButtonUp(const xMousePos& eye)
		{
			m_lastLDown.x = m_lastLDown.y = -1;
		}

		void xViewer::OnRButtonUp(const xMousePos& eye)
		{
			m_lastRDown.x = m_lastRDown.y = -1;
		}

		void xViewer::OnMouseMove(const xMousePos& eye, int button)
		{
			if (button == 0)	// Left button.
			{
				if (m_lastLDown.x == -1 && m_lastLDown.y == -1)
				{
					m_lastLDown = eye;
				}

				m_camera->Pan(m_lastLDown, eye);

				m_lastLDown = eye;
			}
			else if (button == 1)	// Right button.
			{
				if (m_lastRDown.x == -1 && m_lastRDown.y == -1)
				{
					m_lastRDown = eye;
				}

				m_camera->Rotate(m_lastRDown, eye);

				m_lastRDown = eye;
			}
		}

		void xViewer::OnMouseWheel(const xMousePos& eye, bool zoomin)
		{
			m_camera->Zoom(eye, zoomin);
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
			glm::mat4 proj = glm::perspectiveFovRH(glm::radians(ps.fovy), (float)vs.w, (float)vs.h, ps.zNear, ps.zFar);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glLoadMatrixf(glm::value_ptr(proj));

			const xCameraState& cam = m_camera->State();
			glm::mat4 model = glm::lookAtRH(cam.eye, cam.tar, cam.up);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glLoadMatrixf(glm::value_ptr(model));

			//static int print = 0;
			//if (!print++)
			//{
			//    xCommon::PrintMatrix4x4f(glm::value_ptr(model));
			//}
		}
	}
}
