#include "xEasyGL.h"

using namespace xlonlat;
using namespace xlonlat::xEasyGL;

class xLayerToy : public xLayer
{
public:
	xLayerToy(const xViewer* viewer) : xLayer(viewer)
	{

	}

	virtual void OnAttach() override
	{
		const std::wstring& vs_file = std::wstring(xGlobal::Instance().ResourcePath()) + L"shaders\\sample_toy.fs";
		m_ShaderToy = new xShaderToy(vs_file.c_str());
	}

	virtual void OnDraw3D() override
	{
		m_ShaderToy->Draw(m_pViewer);
	}

	virtual void OnDetach() override
	{
		if (m_ShaderToy != nullptr)
		{
			delete m_ShaderToy;
			m_ShaderToy = nullptr;
		}
	}

	virtual void OnEvent(const xEvent& event) override
	{
		m_ShaderToy->Event(event);
	}
private:
	xShaderToy*	m_ShaderToy;
};

int main()
{
	std::wcout.imbue(std::locale("chs"));

	{
		xViewer* viewer = new xViewer(nullptr);
		xWindow window("xApp", 1280, 720, viewer);
		viewer->AttachLayer(new xLayerToy(viewer));
		window.Run();
	}

    _CrtDumpMemoryLeaks();
    return 0;
}