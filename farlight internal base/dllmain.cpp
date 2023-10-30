#include "cheat/cheat.h"
#include "include.h"

#define BYTEn(x, n)   (*((_BYTE*)&(x)+n))
#define BYTE1(x)   BYTEn(x,  1)         

ID3D11RenderTargetView* RenderTarget;
ID3D11DeviceContext* Context;
ID3D11Device* Device;
HWND hwnd;
int X;
int Y;
uintptr_t origPresentPtr_content;
bool ShowMenu = true;
ImVec4 fovcol = ImColor(255, 255, 255);






void menustyle() {
    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
    colors[ImGuiCol_Border] = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    colors[ImGuiCol_Button] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
    colors[ImGuiCol_Separator] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
    colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);

    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowPadding = ImVec2(8.00f, 8.00f);
    style.FramePadding = ImVec2(5.00f, 2.00f);
    style.ItemSpacing = ImVec2(6.00f, 6.00f);
    style.ItemInnerSpacing = ImVec2(6.00f, 6.00f);
    style.TouchExtraPadding = ImVec2(0.00f, 0.00f);
    style.IndentSpacing = 25;
    style.ScrollbarSize = 15;
    style.GrabMinSize = 10;
    style.WindowBorderSize = 1;
    style.ChildBorderSize = 1;
    style.PopupBorderSize = 1;
    style.FrameBorderSize = 1;
    style.TabBorderSize = 1;
    style.WindowRounding = 7;
    style.ChildRounding = 4;
    style.FrameRounding = 3;
    style.PopupRounding = 4;
    style.ScrollbarRounding = 9;
    style.GrabRounding = 3;
    style.TabRounding = 4;
}




void menu() {
    menustyle();
    if (ShowMenu)
    {
        ImGuiIO& io = ImGui::GetIO();
        POINT p;
        GetCursorPos(&p);
        io.MousePos.x = p.x;
        io.MousePos.y = p.y;
        if (steam_keybind(VK_LBUTTON)) {
            io.MouseDown[0] = true;
            io.MouseClicked[0] = true;
            io.MouseClickedPos[0].x = io.MousePos.x;
            io.MouseClickedPos[0].y = io.MousePos.y;
        }
        else {
            io.MouseDown[0] = false;
        }
        ImGui::SetNextWindowSize({ 350, 350 });
        ImGui::Begin(_("refo internal"));
        ImGui::Checkbox(_("cornerbox"), &cornerbox);
        ImGui::Checkbox(_("box"), &box);
        ImGui::Checkbox(_("snapline"), &draw_line);
        ImGui::Checkbox(_("aimbot"), &aimbot);
        ImGui::SliderInt(_("aimbone"), &boneidselect, 1, 3);
        ImGui::SliderFloat(_("fov"), &aimfov, 10.f, 100.f);        
        ImGui::SliderFloat(_("smooth"), &smooth_, 1, 6);

        ImGui::End();
    }


}
__int64 main_hook(IDXGISwapChain* swapchain, __int64 interval, __int64 flags)
{
    static bool first = false;
    if (!first)
    {
        if (SUCCEEDED(swapchain->GetDevice(__uuidof(ID3D11Device), (void**)&Device)))
        {
            Device->GetImmediateContext(&Context);
            DXGI_SWAP_CHAIN_DESC sd;
            swapchain->GetDesc(&sd);
            hwnd = sd.OutputWindow;

            ID3D11Texture2D* pBackBuffer;

            swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
            Device->CreateRenderTargetView(pBackBuffer, NULL, &RenderTarget);
            pBackBuffer->Release();


            ImGui::CreateContext();
            ImGui::GetIO().Fonts->AddFontFromFileTTF(_("C:\\Windows\\Fonts\\Arial.ttf"), 15.0f);
            ImGui_ImplDX11_Init(hwnd, Device, Context);
            ImGui_ImplDX11_CreateDeviceObjects();

            first = true;
        }
        else return ((__int64 (*)(IDXGISwapChain*, __int64, __int64))(origPresentPtr_content))(swapchain, interval, flags);
    }

    if (Device || Context)
    {
        ID3D11Texture2D* renderTargetTexture = nullptr;
        if (!RenderTarget)
        {
            if (SUCCEEDED(swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&renderTargetTexture))))
            {
                Device->CreateRenderTargetView(renderTargetTexture, nullptr, &RenderTarget);
                renderTargetTexture->Release();
            }
        }
    }
    if (steam_keybind(VK_HOME))
    {
        Sleep(300);
        ShowMenu = !ShowMenu;
    }
    if (steam_keybind(VK_END))
    {
        Sleep(300);
        exit(1);
    }

    ImGui_ImplDX11_NewFrame();
    menu();

    if (aimbot)
    {
        ImGui::GetOverlayDrawList()->AddCircle(ImVec2(width / 2, height / 2), aimfov, ImGui::ColorConvertFloat4ToU32(fovcol), 360);
    }

    if (aimbot || draw_line || cornerbox || box)
    {
        initcheat();
    }

    ImGui::EndFrame();
    Context->OMSetRenderTargets(1, &RenderTarget, NULL);
    ImGui::Render();

    if (RenderTarget)
    {
        RenderTarget->Release();
        RenderTarget = nullptr;
    }

    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    return ((__int64 (*)(IDXGISwapChain*, __int64, __int64))(origPresentPtr_content))(swapchain, interval, flags);
}



bool init() {
    auto module_ = find_base_address(_("GameOverlayRenderer64.dll"));
    auto steamPresentPtr_address = PatternScan(_("\x48\xFF\x25\x00\x00\x00\x00\x48\x89\x5C\x24\x30"), _("xxx????xxxxx"), module_, 0xFFFFF);
    steamPresentPtr_address = *(unsigned int*)(steamPresentPtr_address + 0x3) + steamPresentPtr_address + 0x7;
    origPresentPtr_content = *(uintptr_t*)(steamPresentPtr_address);
    *(uintptr_t*)(steamPresentPtr_address) = (uintptr_t)&main_hook;
    return true;
}




bool __stdcall DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
    UNREFERENCED_PARAMETER(hModule);
    UNREFERENCED_PARAMETER(lpReserved);

    if (dwReason == 1)
    {
       // show_console();
        init();
    }


    return 1;
}

