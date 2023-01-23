#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Timer.h"

#include "Renderer.h"

using namespace Walnut;

class ExampleLayer : public Walnut::Layer
{
public:
	virtual void OnUIRender() override
	{
		float r = 0.5f;
		float g = 0.78f;
		float b = 0.5f;
		float lightX = -1.0f;
		float lightY = -1.0f;
		float lightZ = -1.0f;
		ImGui::Begin("Settings");
		


		ImGui::Text("Last render: %.3fms", m_LastRenderTime);
		ImGui::SliderFloat("R", &r, 0.0f, 1.0f);
		ImGui::SliderFloat("G", &g, 0.0f, 1.0f);
		ImGui::SliderFloat("B", &b, 0.0f, 1.0f);
		ImGui::SliderFloat("X", &lightX, -1.0f, 1.0f);
		ImGui::SliderFloat("Y", &lightY, -1.0f, 1.0f);
		ImGui::SliderFloat("Z", &lightZ, -1.0f, 1.0f);
		if (ImGui::Button("Render"))
		{
			Render(lightX, lightY, lightZ, r, g, b);
		}
		

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("ViewPort");

		m_ViewportWidth = ImGui::GetContentRegionAvail().x;
		m_ViewportHeight = ImGui::GetContentRegionAvail().y;

		auto image = m_Renderer.GetFinalImage();

		if (image)
			ImGui::Image(image->GetDescriptorSet(), { (float)image->GetWidth(), (float)image->GetHeight() }, ImVec2(0, 1), ImVec2(1,0));


		ImGui::End();
		ImGui::PopStyleVar();

		Render(lightX, lightY, lightZ, r, g, b);
	}
	void Render(float lx, float ly, float lz, float r, float g, float b)
	{
		Timer timer;

		m_Renderer.OnResize(m_ViewportWidth, m_ViewportHeight);
		m_Renderer.Render(lx, ly, lz, r, g, b);

		m_LastRenderTime = timer.ElapsedMillis();
	}

private:
	Renderer m_Renderer;
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

	float m_LastRenderTime = 0.0f;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Ray Tracing";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}