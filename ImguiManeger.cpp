#include "imguiManeger.h"


ImGuiManager::ImGuiManager()
{

}
void ImGuiManager::Initialize(WinApp* winApp_, DirectXCommon* directX_)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(winApp_->GetHwnd());
	ImGui_ImplDX12_Init(
		directX_->Getdevice(),
		directX_->GetSwapChainDesc_().BufferCount,
		directX_->GetRtvDesc_().Format,
		directX_->GetSrvDescriptorHeap_(),
		directX_->GetSrvDescriptorHeap_()->GetCPUDescriptorHandleForHeapStart(),
		directX_->GetSrvDescriptorHeap_()->GetGPUDescriptorHandleForHeapStart());
}

void ImGuiManager::BeginFlame(DirectXCommon* directX_)
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ID3D12DescriptorHeap* descripterHeap[] = { directX_->GetSrvDescriptorHeap_() };
	directX_->GetcommandList()->SetDescriptorHeaps(1, descripterHeap);
}

void ImGuiManager::EndFlame(DirectXCommon* directX_)
{
	ImGui::ShowDemoWindow();
	ImGui::Render();
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(),directX_->GetcommandList());

}

void ImGuiManager::Release()
{
	ImGui_ImplDX12_Shutdown();
}


