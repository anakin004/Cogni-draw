#include "Gui.h"

Gui::Gui(GLFWwindow* window) : m_Window(window), m_IsHovered(false)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	m_Io = &ImGui::GetIO();

	// enabling some flags

	m_Io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   
	m_Io->ConfigFlags |= ImGuiConfigFlags_DockingEnable; 


	ImGui::StyleColorsDark();

	// setting up Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
	const char* glsl_version = "#version 330";
	ImGui_ImplOpenGL3_Init(glsl_version);

}

Gui::~Gui()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void Gui::render(DrawDock& dock)
{

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
	ImGui::DockSpaceOverViewport(dockspace_id, ImGui::GetMainViewport());

	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver); 
	ImGui::Begin("CG-Sketch");
	ImGui::Image(dock.get_fbo_scene_ID(), ImVec2(1920, 1080));
	m_IsHovered = ImGui::IsItemHovered();

	ImGui::End();

	ImGui::SetNextWindowSize(ImVec2(250, 100), ImGuiCond_FirstUseEver);  
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver); 
	ImGui::Begin("Stencil Config", nullptr, ImGuiWindowFlags_NoCollapse);

	float stencil_config_size = dock.get_stencil_size_ref();  
	if (ImGui::SliderFloat("Stencil Size", &stencil_config_size, 0.0f, 20.0f, "%.2f"))
	{
		dock.set_stencil_size(stencil_config_size);
	}

	// white
	static ImVec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
	if (ImGui::ColorEdit3("Stencil Color", (float*)&color))
	{
		dock.set_stencil_color(glm::vec3{ color.x, color.y, color.z }); 
	}

	ImGui::End();

	glClear(GL_COLOR_BUFFER_BIT);

	ImGui::Render();
	int display_w, display_h;
	glfwGetFramebufferSize(m_Window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


}
