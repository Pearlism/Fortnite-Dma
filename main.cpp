#define SEGAUI_DEFINE_MATH_OPERATORS

#include "imgui.h"
#include "Imgui/backends/imgui_impl_win32.h"
#include "Imgui/backends/imgui_impl_dx11.h"
#include "imgui_internal.h"
#include "imgui_freetype.h"
#include <d3d11.h>
#include <tchar.h>
#include "Create_win.h"
#include <D3DX11tex.h>
#pragma comment(lib, "D3DX11.lib")
#include "notification.h"
#include "image.h"
#include "font.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "Structs.h"
//#include "json.hpp"
#include <fstream>
#include <Windows.h>
#include <dwmapi.h>
#pragma comment(lib, "Dwmapi.lib")
#include "mapper/Mapper.hpp"
#include "Auth/Auth.h"
#include "Functions.h"
#include "SkCrypt.h"
#include <string>
#include <windows.h>
#include <unordered_map>
#include "GlobalDefine.h"
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
#include "KMbox/KmboxB.h"
#include "KMbox/KmBoxNetManager.h"
#include "KMbox/KmBoxData.h"



//#define DMA


#ifdef DMA
#include "Memory/Memory.h"

#endif // DMA

#ifdef Driver
#include "Driver/mem.h"

#endif // Driver

//#define Driver

//int query_pid_by_process_name(const wchar_t* process_name)
//{
//    HANDLE snapshot = CreateToolhelp32Snapshot(0x00000002, 0);
//    if (!snapshot || snapshot == INVALID_HANDLE_VALUE)
//    {
//        //std::cout << "[-] failed to take process snapshot\n";
//
//        return 0;
//    }
//
//    PROCESSENTRY32W entry = {};
//    entry.dwSize = sizeof(PROCESSENTRY32W);
//
//    while (Process32NextW(snapshot, &entry))
//    {
//        if (wcsstr(entry.szExeFile, process_name))
//        {
//            CloseHandle(snapshot);
//
//            return entry.th32ProcessID;
//        }
//    }
//
//    CloseHandle(snapshot);
//
//    return 0;
//}

void D3X11CreateShaderResourceViewFromMemory();

HWND CreateWin(HMODULE hModule, const TCHAR* title, const TCHAR* classname);
std::vector<uint8_t> ReadFileRaw(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);

    if (!file.is_open()) {
        return std::vector<uint8_t>();
    }
    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<uint8_t> fileData(fileSize);
    file.read(reinterpret_cast<char*>(fileData.data()), fileSize);
    file.close();
    return fileData;
}

namespace pic
{
    ID3D11ShaderResourceView* background = nullptr;
    ID3D11ShaderResourceView* aimbot = nullptr;
    ID3D11ShaderResourceView* visuals = nullptr;
    ID3D11ShaderResourceView* exploits = nullptr;
    ID3D11ShaderResourceView* settings = nullptr;
    ID3D11ShaderResourceView* Chat = nullptr;
    ID3D11ShaderResourceView* Camera = nullptr;
    ID3D11ShaderResourceView* combo_widget = nullptr;
    ID3D11ShaderResourceView* input_widget = nullptr;
    ID3D11ShaderResourceView* menu_settings_icon = nullptr;
    ID3D11ShaderResourceView* blur1 = nullptr;
    ID3D11ShaderResourceView* blur2 = nullptr;
    ID3D11ShaderResourceView* scaricon = nullptr;
    ID3D11ShaderResourceView* circle_success = nullptr;
    ID3D11ShaderResourceView* circle_error = nullptr;
    ID3D11ShaderResourceView* fortnitegirl = nullptr;
    ID3D11ShaderResourceView* android_logo = nullptr;
    ID3D11ShaderResourceView* ios_logo = nullptr;
    ID3D11ShaderResourceView* Nintendo_logo = nullptr;
    ID3D11ShaderResourceView* old_windows_logo = nullptr;
    ID3D11ShaderResourceView* playstation_logo = nullptr;
    ID3D11ShaderResourceView* xbox_logo = nullptr;

}

namespace font
{
    ImFont* inter_bold = nullptr;
    ImFont* inter_bold_v2 = nullptr;
    ImFont* inter_default = nullptr;
    ImFont* icon = nullptr;
}


struct FWeaponConfig {
    std::string WeaponName = "";
    int Fov = 100;
    int Smooth = 20;
    int Distance = 200;
    bool Aimbot = true;
    bool TriggerBot = false;

    int Identifier = 0;

};



struct FUserConfig
{
    bool FuserMode = false;
    std::string KmBoxModel = "b";
    int KmBoxModelIndex = 0;

    std::string KmBoxIp = "0";
    std::string KmBoxUuid = "0";
    int KmBoxPort = 0;
    bool KmBoxStatus = 0;
    int TriggerBotKey = 2;
    int TriggerBotMethod = 2;


    int MenuKeyIndex = VK_CAPITAL;
    int CurrentMainTab = 0;

    int CurrentWeaponTab = 0;

    int CurrentWeaponIndex = 0;
    int CurrentEspTab = 0;
    int CurrentSkinsTab = 0;
    int CurrentSettingsTab = 0;

    bool bShowMenu = false;
    bool bShowInfoBar = true;
    float CurrentFov = 0;
    int AimKeyIndex = 2;

    bool bVSync = true;
    bool bShowFov = true;

    bool bRealisticSize = false;

    std::string Language = sk_crypt("Eng").decrypt();
    int LanguageInt = 0;

    bool IgnoreKnockeds = true;

    bool bItemsEsp = true;
    //esp
    bool bBoxEsp = true;
    ImVec4 BoxEspColorVisible = ImVec4(0, 1, 0, 1);

    bool bFilledBoxEsp = true;
    ImVec4 FilledBoxEspColorVisible = ImVec4(0, 0.5, 0, 0.2);

    bool bCornerBoxEsp = true;
    ImVec4 CornerBoxEspColorVisible = ImVec4(0, 1, 0, 1);

    bool bLinesEsp = true;
    ImVec4 LinesEspColorVisible = ImVec4(1, 1, 1, 1);
    int LinesPosition = 0;

    bool bNamesEsp = true;
    ImVec4 NamesEspColorVisible = ImVec4(1, 0, 1, 1);

    bool bPlatformEsp = true;
    ImVec4 PlatformEspColorVisible = ImVec4(0, 0, 1, 1);
    
    bool bSkeletonEsp = true;
    ImVec4 SkeletonEspColorVisible = ImVec4(0, 1, 0, 1);
    

    bool bKillsEsp = true;
    ImVec4 KillsEspColorVisible = ImVec4(0, 1, 0, 1);

    bool bDistanceEsp = true;
    ImVec4 DistanceEspColorVisible = ImVec4(0, 1, 0, 1);

    bool bChestEsp = true;
    ImVec4 ChestEspColorVisible = ImVec4(1, 1, 0, 1);
 
    bool bEnemyWeaponEsp = true;

    ImVec4 GlobalNotVisible = ImVec4(1, 0, 0, 1);

   
    bool bRadarEsp = true;
    ImVec2 RadarStartPos = ImVec2(20, 20);
    ImVec2 RadarSize = ImVec2(300, 300);

    int AimMode = 0;
    std::vector<FWeaponConfig> WeaponConfigs;
    std::vector<FBone> AimBones;

    bool bPrediction = true;
    float BulletGravity = 2500;
    float BulletSpeed = 45000;

    FBone* DefaultBone = new FBone;

    FBone* GetBoneByName(std::string BoneName)
    {
        for (int64_t i = 0; i < AimBones.size(); i++)
        {
            if (BoneName == AimBones[i].Name)
            {
                return &AimBones[i];
            }
        }
        return DefaultBone;
    }

    FWeaponConfig* GetConfigByName(std::string WeaponName)
    {
        for (int64_t i = 0; i < WeaponConfigs.size(); i++)
        {
            if (WeaponName == WeaponConfigs[i].WeaponName)
            {
                return &WeaponConfigs[i];
            }

        }

        for (int64_t i = 0; i < WeaponConfigs.size(); i++)
        {
            if (sk_crypt("Default").decrypt() == WeaponConfigs[i].WeaponName)
            {
                return &WeaponConfigs[i];
            }

        }
    }

    FWeaponConfig* GetConfigByIdentifier(uint8_t Identifier)
    {
        for (int64_t i = 0; i < WeaponConfigs.size(); i++)
        {
            if (Identifier == WeaponConfigs[i].Identifier)
            {
                return &WeaponConfigs[i];
            }

        }

        for (int64_t i = 0; i < WeaponConfigs.size(); i++)
        {
            if (sk_crypt("Default").decrypt() == WeaponConfigs[i].WeaponName)
            {
                return &WeaponConfigs[i];
            }

        }
    }

};



FUserConfig CurrentConfig;

static bool bShowEnterNotification = 1;

#ifdef DMA
std::string ProductName = ("Streck Dma");

#endif // DMA

#ifdef Driver
//HANDLE handle = 0;

std::string ProductName = ("Streck Private");

#endif // DMA

std::string CurrentFps = ("1337");
std::string TimeRemaining = ("29 Days");

DWORD picker_flags = SegaUiColorEditFlags_NoSidePreview | SegaUiColorEditFlags_AlphaBar | SegaUiColorEditFlags_NoInputs | SegaUiColorEditFlags_AlphaPreview;

static ID3D11Device*            g_pd3dDevice = nullptr;
static ID3D11DeviceContext*     g_pd3dDeviceContext = nullptr;
static IDXGISwapChain*          g_pSwapChain = nullptr;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static ID3D11RenderTargetView*  g_mainRenderTargetView = nullptr;

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
//std::string ReadFile(const std::string& filename) {
//    std::ifstream file(filename);
//    if (!file.is_open()) {
//        std::cerr << "Unable to open file: " << filename << std::endl;
//        return ""; // Returning an empty string if file cannot be opened
//    }
//
//    std::string file_contents;
//    std::string line;
//    while (std::getline(file, line)) {
//        file_contents += line;
//        file_contents += '\n'; // Adding newline character if you want to preserve line breaks
//    }
//
//    file.close();
//    return file_contents;
//}

nlohmann::json LoadJsonFile(const std::string& filename) {
    std::ifstream inputFile(filename);
    nlohmann::json JsonFile;

    if (inputFile.is_open())
    {
        inputFile >> JsonFile;
        inputFile.close();
    }
    inputFile.close();
    return JsonFile;
}

bool bIsDead(uintptr_t APawn)
{

    PawnDeadStatusStruct NewCurrentStruct = driver->read<PawnDeadStatusStruct>(APawn + 0x758);
    return NewCurrentStruct.bIsDying;
}

bool bIsDbno(uintptr_t APawn)
{

    PawnDBNOStatusStruct NewCurrentStruct = driver->read<PawnDBNOStatusStruct>(APawn + 0x982);
    return NewCurrentStruct.bIsDBNO;
}

bool LoadConfig(FUserConfig* CurrentConfig)
{
    if (CurrentConfig)
    {
        std::string FileData = Func::readFile(sk_crypt("C:/Config.streck").decrypt());
        if (FileData.empty())
        {
            return 0;
        }

        nlohmann::json LoadedJsonConfig = LoadedJsonConfig.parse(FileData);
        //nlohmann::json LoadedJsonConfig = LoadJsonFile("C:\\Config.streck");
        if (LoadedJsonConfig.empty())
        {
            return 0;

        }
        CurrentConfig->MenuKeyIndex = (LoadedJsonConfig.count("MenuKeyIndex") != 0) ? LoadedJsonConfig["MenuKeyIndex"].get<int>() : CurrentConfig->MenuKeyIndex;
        CurrentConfig->Language = (LoadedJsonConfig.count("Language") != 0) ? LoadedJsonConfig["Language"].get<std::string>() : CurrentConfig->Language;
        CurrentConfig->LanguageInt = (LoadedJsonConfig.count("LanguageInt") != 0) ? LoadedJsonConfig["LanguageInt"].get<int>() : CurrentConfig->LanguageInt;

        CurrentConfig->AimKeyIndex = (LoadedJsonConfig.count("AimKeyIndex") != 0) ? LoadedJsonConfig["AimKeyIndex"].get<int>() : CurrentConfig->AimKeyIndex;
        CurrentConfig->AimMode = (LoadedJsonConfig.count("AimMode") != 0) ? LoadedJsonConfig["AimMode"].get<int>() : CurrentConfig->AimMode;
        CurrentConfig->TriggerBotKey = (LoadedJsonConfig.count("TriggerBotKey") != 0) ? LoadedJsonConfig["TriggerBotKey"].get<int>() : CurrentConfig->TriggerBotKey;
        CurrentConfig->TriggerBotMethod = (LoadedJsonConfig.count("TriggerBotMethod") != 0) ? LoadedJsonConfig["TriggerBotMethod"].get<int>() : CurrentConfig->TriggerBotMethod;


        CurrentConfig->bShowFov = (LoadedJsonConfig.count("bShowFov") != 0) ? LoadedJsonConfig["bShowFov"].get<bool>() : CurrentConfig->bShowFov;
        CurrentConfig->bShowInfoBar = (LoadedJsonConfig.count("bShowInfoBar") != 0) ? LoadedJsonConfig["bShowInfoBar"].get<bool>() : CurrentConfig->bShowInfoBar;
        CurrentConfig->bRealisticSize = (LoadedJsonConfig.count("bRealisticSize") != 0) ? LoadedJsonConfig["bRealisticSize"].get<bool>() : CurrentConfig->bRealisticSize;

        CurrentConfig->bVSync = (LoadedJsonConfig.count("bVSync") != 0) ? LoadedJsonConfig["bVSync"].get<bool>() : CurrentConfig->bVSync;

        CurrentConfig->bBoxEsp = (LoadedJsonConfig.count("bBoxEsp") != 0) ? LoadedJsonConfig["bBoxEsp"].get<bool>() : CurrentConfig->bBoxEsp;
        CurrentConfig->BoxEspColorVisible.x = (LoadedJsonConfig.count("BoxEspColorVisible.x") != 0) ? LoadedJsonConfig["BoxEspColorVisible.x"].get<float>() : CurrentConfig->BoxEspColorVisible.x;
        CurrentConfig->BoxEspColorVisible.y = (LoadedJsonConfig.count("BoxEspColorVisible.y") != 0) ? LoadedJsonConfig["BoxEspColorVisible.y"].get<float>() : CurrentConfig->BoxEspColorVisible.y;
        CurrentConfig->BoxEspColorVisible.z = (LoadedJsonConfig.count("BoxEspColorVisible.z") != 0) ? LoadedJsonConfig["BoxEspColorVisible.z"].get<float>() : CurrentConfig->BoxEspColorVisible.z;
        CurrentConfig->BoxEspColorVisible.w = (LoadedJsonConfig.count("BoxEspColorVisible.w") != 0) ? LoadedJsonConfig["BoxEspColorVisible.w"].get<float>() : CurrentConfig->BoxEspColorVisible.w;


        CurrentConfig->bFilledBoxEsp = (LoadedJsonConfig.count("bFilledBoxEsp") != 0) ? LoadedJsonConfig["bFilledBoxEsp"].get<bool>() : CurrentConfig->bFilledBoxEsp;
        CurrentConfig->FilledBoxEspColorVisible.x = (LoadedJsonConfig.count("FilledBoxEspColorVisible.x") != 0) ? LoadedJsonConfig["FilledBoxEspColorVisible.x"].get<float>() : CurrentConfig->FilledBoxEspColorVisible.x;
        CurrentConfig->FilledBoxEspColorVisible.y = (LoadedJsonConfig.count("FilledBoxEspColorVisible.y") != 0) ? LoadedJsonConfig["FilledBoxEspColorVisible.y"].get<float>() : CurrentConfig->FilledBoxEspColorVisible.y;
        CurrentConfig->FilledBoxEspColorVisible.z = (LoadedJsonConfig.count("FilledBoxEspColorVisible.z") != 0) ? LoadedJsonConfig["FilledBoxEspColorVisible.z"].get<float>() : CurrentConfig->FilledBoxEspColorVisible.z;
        CurrentConfig->FilledBoxEspColorVisible.w = (LoadedJsonConfig.count("FilledBoxEspColorVisible.w") != 0) ? LoadedJsonConfig["FilledBoxEspColorVisible.w"].get<float>() : CurrentConfig->FilledBoxEspColorVisible.w;

        CurrentConfig->bCornerBoxEsp = (LoadedJsonConfig.count("bCornerBoxEsp") != 0) ? LoadedJsonConfig["bCornerBoxEsp"].get<bool>() : CurrentConfig->bCornerBoxEsp;
        CurrentConfig->CornerBoxEspColorVisible.x = (LoadedJsonConfig.count("CornerBoxEspColorVisible.x") != 0) ? LoadedJsonConfig["CornerBoxEspColorVisible.x"].get<float>() : CurrentConfig->CornerBoxEspColorVisible.x;
        CurrentConfig->CornerBoxEspColorVisible.y = (LoadedJsonConfig.count("CornerBoxEspColorVisible.y") != 0) ? LoadedJsonConfig["CornerBoxEspColorVisible.y"].get<float>() : CurrentConfig->CornerBoxEspColorVisible.y;
        CurrentConfig->CornerBoxEspColorVisible.z = (LoadedJsonConfig.count("CornerBoxEspColorVisible.z") != 0) ? LoadedJsonConfig["CornerBoxEspColorVisible.z"].get<float>() : CurrentConfig->CornerBoxEspColorVisible.z;
        CurrentConfig->CornerBoxEspColorVisible.w = (LoadedJsonConfig.count("CornerBoxEspColorVisible.w") != 0) ? LoadedJsonConfig["CornerBoxEspColorVisible.w"].get<float>() : CurrentConfig->CornerBoxEspColorVisible.w;

        CurrentConfig->bLinesEsp = (LoadedJsonConfig.count("bLinesEsp") != 0) ? LoadedJsonConfig["bLinesEsp"].get<bool>() : CurrentConfig->bLinesEsp;
        CurrentConfig->LinesEspColorVisible.x = (LoadedJsonConfig.count("LinesEspColorVisible.x") != 0) ? LoadedJsonConfig["LinesEspColorVisible.x"].get<float>() : CurrentConfig->LinesEspColorVisible.x;
        CurrentConfig->LinesEspColorVisible.y = (LoadedJsonConfig.count("LinesEspColorVisible.y") != 0) ? LoadedJsonConfig["LinesEspColorVisible.y"].get<float>() : CurrentConfig->LinesEspColorVisible.y;
        CurrentConfig->LinesEspColorVisible.z = (LoadedJsonConfig.count("LinesEspColorVisible.z") != 0) ? LoadedJsonConfig["LinesEspColorVisible.z"].get<float>() : CurrentConfig->LinesEspColorVisible.z;
        CurrentConfig->LinesEspColorVisible.w = (LoadedJsonConfig.count("LinesEspColorVisible.w") != 0) ? LoadedJsonConfig["LinesEspColorVisible.w"].get<float>() : CurrentConfig->LinesEspColorVisible.w;

        CurrentConfig->LinesPosition = (LoadedJsonConfig.count("LinesPosition") != 0) ? LoadedJsonConfig["LinesPosition"].get<int>() : CurrentConfig->LinesPosition;


        CurrentConfig->bNamesEsp = (LoadedJsonConfig.count("bNamesEsp") != 0) ? LoadedJsonConfig["bNamesEsp"].get<bool>() : CurrentConfig->bNamesEsp;
        CurrentConfig->NamesEspColorVisible.x = (LoadedJsonConfig.count("NamesEspColorVisible.x") != 0) ? LoadedJsonConfig["NamesEspColorVisible.x"].get<float>() : CurrentConfig->NamesEspColorVisible.x;
        CurrentConfig->NamesEspColorVisible.y = (LoadedJsonConfig.count("NamesEspColorVisible.y") != 0) ? LoadedJsonConfig["NamesEspColorVisible.y"].get<float>() : CurrentConfig->NamesEspColorVisible.y;
        CurrentConfig->NamesEspColorVisible.z = (LoadedJsonConfig.count("NamesEspColorVisible.z") != 0) ? LoadedJsonConfig["NamesEspColorVisible.z"].get<float>() : CurrentConfig->NamesEspColorVisible.z;
        CurrentConfig->NamesEspColorVisible.w = (LoadedJsonConfig.count("NamesEspColorVisible.w") != 0) ? LoadedJsonConfig["NamesEspColorVisible.w"].get<float>() : CurrentConfig->NamesEspColorVisible.w;

        CurrentConfig->bPlatformEsp = (LoadedJsonConfig.count("bPlatformEsp") != 0) ? LoadedJsonConfig["bPlatformEsp"].get<bool>() : CurrentConfig->bPlatformEsp;
        CurrentConfig->PlatformEspColorVisible.x = (LoadedJsonConfig.count("PlatformEspColorVisible.x") != 0) ? LoadedJsonConfig["PlatformEspColorVisible.x"].get<float>() : CurrentConfig->PlatformEspColorVisible.x;
        CurrentConfig->PlatformEspColorVisible.y = (LoadedJsonConfig.count("PlatformEspColorVisible.y") != 0) ? LoadedJsonConfig["PlatformEspColorVisible.y"].get<float>() : CurrentConfig->PlatformEspColorVisible.y;
        CurrentConfig->PlatformEspColorVisible.z = (LoadedJsonConfig.count("PlatformEspColorVisible.z") != 0) ? LoadedJsonConfig["PlatformEspColorVisible.z"].get<float>() : CurrentConfig->PlatformEspColorVisible.z;
        CurrentConfig->PlatformEspColorVisible.w = (LoadedJsonConfig.count("PlatformEspColorVisible.w") != 0) ? LoadedJsonConfig["PlatformEspColorVisible.w"].get<float>() : CurrentConfig->PlatformEspColorVisible.w;

        CurrentConfig->bSkeletonEsp = (LoadedJsonConfig.count("bSkeletonEsp") != 0) ? LoadedJsonConfig["bSkeletonEsp"].get<bool>() : CurrentConfig->bSkeletonEsp;
        CurrentConfig->SkeletonEspColorVisible.x = (LoadedJsonConfig.count("SkeletonEspColorVisible.x") != 0) ? LoadedJsonConfig["SkeletonEspColorVisible.x"].get<float>() : CurrentConfig->SkeletonEspColorVisible.x;
        CurrentConfig->SkeletonEspColorVisible.y = (LoadedJsonConfig.count("SkeletonEspColorVisible.y") != 0) ? LoadedJsonConfig["SkeletonEspColorVisible.y"].get<float>() : CurrentConfig->SkeletonEspColorVisible.y;
        CurrentConfig->SkeletonEspColorVisible.z = (LoadedJsonConfig.count("SkeletonEspColorVisible.z") != 0) ? LoadedJsonConfig["SkeletonEspColorVisible.z"].get<float>() : CurrentConfig->SkeletonEspColorVisible.z;
        CurrentConfig->SkeletonEspColorVisible.w = (LoadedJsonConfig.count("SkeletonEspColorVisible.w") != 0) ? LoadedJsonConfig["SkeletonEspColorVisible.w"].get<float>() : CurrentConfig->SkeletonEspColorVisible.w;
        
        CurrentConfig->bKillsEsp = (LoadedJsonConfig.count("bKillsEsp") != 0) ? LoadedJsonConfig["bKillsEsp"].get<bool>() : CurrentConfig->bKillsEsp;
        CurrentConfig->KillsEspColorVisible.x = (LoadedJsonConfig.count("KillsEspColorVisible.x") != 0) ? LoadedJsonConfig["KillsEspColorVisible.x"].get<float>() : CurrentConfig->KillsEspColorVisible.x;
        CurrentConfig->KillsEspColorVisible.y = (LoadedJsonConfig.count("KillsEspColorVisible.y") != 0) ? LoadedJsonConfig["KillsEspColorVisible.y"].get<float>() : CurrentConfig->KillsEspColorVisible.y;
        CurrentConfig->KillsEspColorVisible.z = (LoadedJsonConfig.count("KillsEspColorVisible.z") != 0) ? LoadedJsonConfig["KillsEspColorVisible.z"].get<float>() : CurrentConfig->KillsEspColorVisible.z;
        CurrentConfig->KillsEspColorVisible.w = (LoadedJsonConfig.count("KillsEspColorVisible.w") != 0) ? LoadedJsonConfig["KillsEspColorVisible.w"].get<float>() : CurrentConfig->KillsEspColorVisible.w;
        
        CurrentConfig->bDistanceEsp = (LoadedJsonConfig.count("bDistanceEsp") != 0) ? LoadedJsonConfig["bDistanceEsp"].get<bool>() : CurrentConfig->bDistanceEsp;
        CurrentConfig->DistanceEspColorVisible.x = (LoadedJsonConfig.count("DistanceEspColorVisible.x") != 0) ? LoadedJsonConfig["DistanceEspColorVisible.x"].get<float>() : CurrentConfig->DistanceEspColorVisible.x;
        CurrentConfig->DistanceEspColorVisible.y = (LoadedJsonConfig.count("DistanceEspColorVisible.y") != 0) ? LoadedJsonConfig["DistanceEspColorVisible.y"].get<float>() : CurrentConfig->DistanceEspColorVisible.y;
        CurrentConfig->DistanceEspColorVisible.z = (LoadedJsonConfig.count("DistanceEspColorVisible.z") != 0) ? LoadedJsonConfig["DistanceEspColorVisible.z"].get<float>() : CurrentConfig->DistanceEspColorVisible.z;
        CurrentConfig->DistanceEspColorVisible.w = (LoadedJsonConfig.count("DistanceEspColorVisible.w") != 0) ? LoadedJsonConfig["DistanceEspColorVisible.w"].get<float>() : CurrentConfig->DistanceEspColorVisible.w;

        CurrentConfig->bChestEsp = (LoadedJsonConfig.count("bChestEsp") != 0) ? LoadedJsonConfig["bChestEsp"].get<bool>() : CurrentConfig->bChestEsp;

        CurrentConfig->ChestEspColorVisible.x = (LoadedJsonConfig.count("ChestEspColorVisible.x") != 0) ? LoadedJsonConfig["ChestEspColorVisible.x"].get<float>() : CurrentConfig->ChestEspColorVisible.x;
        CurrentConfig->ChestEspColorVisible.y = (LoadedJsonConfig.count("ChestEspColorVisible.y") != 0) ? LoadedJsonConfig["ChestEspColorVisible.y"].get<float>() : CurrentConfig->ChestEspColorVisible.y;
        CurrentConfig->ChestEspColorVisible.z = (LoadedJsonConfig.count("ChestEspColorVisible.z") != 0) ? LoadedJsonConfig["ChestEspColorVisible.z"].get<float>() : CurrentConfig->ChestEspColorVisible.z;
        CurrentConfig->ChestEspColorVisible.w = (LoadedJsonConfig.count("ChestEspColorVisible.w") != 0) ? LoadedJsonConfig["ChestEspColorVisible.w"].get<float>() : CurrentConfig->ChestEspColorVisible.w;

        CurrentConfig->bEnemyWeaponEsp = (LoadedJsonConfig.count("bEnemyWeaponEsp") != 0) ? LoadedJsonConfig["bEnemyWeaponEsp"].get<bool>() : CurrentConfig->bEnemyWeaponEsp;
        CurrentConfig->bRadarEsp = (LoadedJsonConfig.count("bRadarEsp") != 0) ? LoadedJsonConfig["bRadarEsp"].get<bool>() : CurrentConfig->bRadarEsp;
        CurrentConfig->RadarStartPos.x = (LoadedJsonConfig.count("RadarStartPos.x") != 0) ? LoadedJsonConfig["RadarStartPos.x"].get<float>() : CurrentConfig->RadarStartPos.x;
        CurrentConfig->RadarStartPos.y = (LoadedJsonConfig.count("RadarStartPos.y") != 0) ? LoadedJsonConfig["RadarStartPos.y"].get<float>() : CurrentConfig->RadarStartPos.y;

        CurrentConfig->RadarSize.x = (LoadedJsonConfig.count("RadarSize.x") != 0) ? LoadedJsonConfig["RadarSize.x"].get<float>() : CurrentConfig->RadarSize.x;
        CurrentConfig->RadarSize.y = (LoadedJsonConfig.count("RadarSize.y") != 0) ? LoadedJsonConfig["RadarSize.y"].get<float>() : CurrentConfig->RadarSize.y;


        CurrentConfig->GlobalNotVisible.x = (LoadedJsonConfig.count("GlobalNotVisible.x") != 0) ? LoadedJsonConfig["GlobalNotVisible.x"].get<float>() : CurrentConfig->GlobalNotVisible.x;
        CurrentConfig->GlobalNotVisible.y = (LoadedJsonConfig.count("GlobalNotVisible.y") != 0) ? LoadedJsonConfig["GlobalNotVisible.y"].get<float>() : CurrentConfig->GlobalNotVisible.y;
        CurrentConfig->GlobalNotVisible.z = (LoadedJsonConfig.count("GlobalNotVisible.z") != 0) ? LoadedJsonConfig["GlobalNotVisible.z"].get<float>() : CurrentConfig->GlobalNotVisible.z;
        CurrentConfig->GlobalNotVisible.w = (LoadedJsonConfig.count("GlobalNotVisible.w") != 0) ? LoadedJsonConfig["GlobalNotVisible.w"].get<float>() : CurrentConfig->GlobalNotVisible.w;



        if (LoadedJsonConfig.contains("WeaponConfigs") && LoadedJsonConfig["WeaponConfigs"].is_array()) {
            nlohmann::json WeaponConfigsJson = LoadedJsonConfig["WeaponConfigs"];
            for (const auto& weaponConfig : WeaponConfigsJson) {
                FWeaponConfig ThisWeapon;
                ThisWeapon.WeaponName = (weaponConfig.count("WeaponName") != 0) ? weaponConfig["WeaponName"].get<std::string>() : ThisWeapon.WeaponName;
                ThisWeapon.Fov = (weaponConfig.count("Fov") != 0) ? weaponConfig["Fov"].get<int>() : ThisWeapon.Fov;
                ThisWeapon.Smooth = (weaponConfig.count("Smooth") != 0) ? weaponConfig["Smooth"].get<int>() : ThisWeapon.Smooth;
                ThisWeapon.Distance = (weaponConfig.count("Distance") != 0) ? weaponConfig["Distance"].get<int>() : ThisWeapon.Distance;
                ThisWeapon.Aimbot = (weaponConfig.count("Aimbot") != 0) ? weaponConfig["Aimbot"].get<bool>() : ThisWeapon.Aimbot;
                ThisWeapon.TriggerBot = (weaponConfig.count("TriggerBot") != 0) ? weaponConfig["TriggerBot"].get<bool>() : ThisWeapon.TriggerBot;
                //ThisWeapon.TriggerBotKey = (weaponConfig.count("TriggerBotKey") != 0) ? weaponConfig["TriggerBotKey"].get<int>() : ThisWeapon.TriggerBotKey;
                //ThisWeapon.TriggerBotMethod = (weaponConfig.count("TriggerBotMethod") != 0) ? weaponConfig["TriggerBotMethod"].get<int>() : ThisWeapon.TriggerBotMethod;
                ThisWeapon.Identifier = (weaponConfig.count("Identifier") != 0) ? weaponConfig["Identifier"].get<int>() : ThisWeapon.Identifier;
                //ThisWeapon.AimMode = (weaponConfig.count("AimMode") != 0) ? weaponConfig["AimMode"].get<int>() : ThisWeapon.AimMode;
                CurrentConfig->WeaponConfigs.push_back(ThisWeapon);
            }
        }
        else {

        }


        //if (LoadedJsonConfig.contains("AimBones") && LoadedJsonConfig["AimBones"].is_array()) {
        //    nlohmann::json AimBonesJson = LoadedJsonConfig["AimBones"];
        //    for (const auto& aimBone : AimBonesJson) 
        //    {
        //        FBone ThisBone;
        //        ThisBone.Name = (aimBone.count("Name") != 0) ? aimBone["Name"].get<std::string>() : ThisBone.Name;
        //        ThisBone.bIsActivated = (aimBone.count("bIsActivated") != 0) ? aimBone["bIsActivated"].get<bool>() : ThisBone.bIsActivated;
        //        ThisBone.Bone = (aimBone.count("Bone") != 0) ? aimBone["Bone"].get<int>() : ThisBone.Bone;
        //        CurrentConfig->AimBones.push_back(ThisBone);
        //    }
        //}
        //else {

        //}

        CurrentConfig->BulletGravity = (LoadedJsonConfig.count("BulletGravity") != 0) ? LoadedJsonConfig["BulletGravity"].get<float>() : CurrentConfig->BulletGravity;
        CurrentConfig->BulletSpeed = (LoadedJsonConfig.count("BulletSpeed") != 0) ? LoadedJsonConfig["BulletSpeed"].get<float>() : CurrentConfig->BulletSpeed;
        CurrentConfig->bPrediction = (LoadedJsonConfig.count("bPrediction") != 0) ? LoadedJsonConfig["bPrediction"].get<bool>() : CurrentConfig->bPrediction;
        return 1;
    }
    else
    {
        return 0;
    }
}



//using json = nlohmann::json;

bool SaveConfig(const FUserConfig* CurrentConfig)
{
    if (CurrentConfig)
    {
        nlohmann::json ConfigJson;

        ConfigJson["MenuKeyIndex"] = CurrentConfig->MenuKeyIndex;
        ConfigJson["Language"] = CurrentConfig->Language;
        ConfigJson["LanguageInt"] = CurrentConfig->LanguageInt;
        ConfigJson["AimKeyIndex"] = CurrentConfig->AimKeyIndex;
        ConfigJson["bVSync"] = CurrentConfig->bVSync;
        ConfigJson["bShowFov"] = CurrentConfig->bShowFov;
        ConfigJson["bShowInfoBar"] = CurrentConfig->bShowInfoBar;
        ConfigJson["bRealisticSize"] = CurrentConfig->bRealisticSize;

        ConfigJson["AimMode"] = CurrentConfig->AimMode;
        ConfigJson["TriggerBotKey"] = CurrentConfig->TriggerBotKey;
        ConfigJson["TriggerBotMethod"] = CurrentConfig->TriggerBotMethod;



        ConfigJson["bBoxEsp"] = CurrentConfig->bBoxEsp;
        ConfigJson["BoxEspColorVisible.x"] = CurrentConfig->BoxEspColorVisible.x;
        ConfigJson["BoxEspColorVisible.y"] = CurrentConfig->BoxEspColorVisible.y;
        ConfigJson["BoxEspColorVisible.z"] = CurrentConfig->BoxEspColorVisible.z;
        ConfigJson["BoxEspColorVisible.w"] = CurrentConfig->BoxEspColorVisible.w;

        ConfigJson["bFilledBoxEsp"] = CurrentConfig->bFilledBoxEsp;
        ConfigJson["FilledBoxEspColorVisible.x"] = CurrentConfig->FilledBoxEspColorVisible.x;
        ConfigJson["FilledBoxEspColorVisible.y"] = CurrentConfig->FilledBoxEspColorVisible.y;
        ConfigJson["FilledBoxEspColorVisible.z"] = CurrentConfig->FilledBoxEspColorVisible.z;
        ConfigJson["FilledBoxEspColorVisible.w"] = CurrentConfig->FilledBoxEspColorVisible.w;

        ConfigJson["bCornerBoxEsp"] = CurrentConfig->bCornerBoxEsp;
        ConfigJson["CornerBoxEspColorVisible.x"] = CurrentConfig->CornerBoxEspColorVisible.x;
        ConfigJson["CornerBoxEspColorVisible.y"] = CurrentConfig->CornerBoxEspColorVisible.y;
        ConfigJson["CornerBoxEspColorVisible.z"] = CurrentConfig->CornerBoxEspColorVisible.z;
        ConfigJson["CornerBoxEspColorVisible.w"] = CurrentConfig->CornerBoxEspColorVisible.w;

        ConfigJson["bLinesEsp"] = CurrentConfig->bLinesEsp;
        ConfigJson["LinesEspColorVisible.x"] = CurrentConfig->LinesEspColorVisible.x;
        ConfigJson["LinesEspColorVisible.y"] = CurrentConfig->LinesEspColorVisible.y;
        ConfigJson["LinesEspColorVisible.z"] = CurrentConfig->LinesEspColorVisible.z;
        ConfigJson["LinesEspColorVisible.w"] = CurrentConfig->LinesEspColorVisible.w;


        ConfigJson["LinesPosition"] = CurrentConfig->LinesPosition;

        ConfigJson["bNamesEsp"] = CurrentConfig->bNamesEsp;
        ConfigJson["NamesEspColorVisible.x"] = CurrentConfig->NamesEspColorVisible.x;
        ConfigJson["NamesEspColorVisible.y"] = CurrentConfig->NamesEspColorVisible.y;
        ConfigJson["NamesEspColorVisible.z"] = CurrentConfig->NamesEspColorVisible.z;
        ConfigJson["NamesEspColorVisible.w"] = CurrentConfig->NamesEspColorVisible.w;

        ConfigJson["bPlatformEsp"] = CurrentConfig->bPlatformEsp;
        ConfigJson["PlatformEspColorVisible.x"] = CurrentConfig->PlatformEspColorVisible.x;
        ConfigJson["PlatformEspColorVisible.y"] = CurrentConfig->PlatformEspColorVisible.y;
        ConfigJson["PlatformEspColorVisible.z"] = CurrentConfig->PlatformEspColorVisible.z;
        ConfigJson["PlatformEspColorVisible.w"] = CurrentConfig->PlatformEspColorVisible.w;

        ConfigJson["bSkeletonEsp"] = CurrentConfig->bSkeletonEsp;
        ConfigJson["SkeletonEspColorVisible.x"] = CurrentConfig->SkeletonEspColorVisible.x;
        ConfigJson["SkeletonEspColorVisible.y"] = CurrentConfig->SkeletonEspColorVisible.y;
        ConfigJson["SkeletonEspColorVisible.z"] = CurrentConfig->SkeletonEspColorVisible.z;
        ConfigJson["SkeletonEspColorVisible.w"] = CurrentConfig->SkeletonEspColorVisible.w;

        ConfigJson["bKillsEsp"] = CurrentConfig->bKillsEsp;
        ConfigJson["KillsEspColorVisible.x"] = CurrentConfig->KillsEspColorVisible.x;
        ConfigJson["KillsEspColorVisible.y"] = CurrentConfig->KillsEspColorVisible.y;
        ConfigJson["KillsEspColorVisible.z"] = CurrentConfig->KillsEspColorVisible.z;
        ConfigJson["KillsEspColorVisible.w"] = CurrentConfig->KillsEspColorVisible.w;

        ConfigJson["bDistanceEsp"] = CurrentConfig->bDistanceEsp;
        ConfigJson["DistanceEspColorVisible.x"] = CurrentConfig->DistanceEspColorVisible.x;
        ConfigJson["DistanceEspColorVisible.y"] = CurrentConfig->DistanceEspColorVisible.y;
        ConfigJson["DistanceEspColorVisible.z"] = CurrentConfig->DistanceEspColorVisible.z;
        ConfigJson["DistanceEspColorVisible.w"] = CurrentConfig->DistanceEspColorVisible.w;

        ConfigJson["bChestEsp"] = CurrentConfig->bChestEsp;
        ConfigJson["ChestEspColorVisible.x"] = CurrentConfig->ChestEspColorVisible.x;
        ConfigJson["ChestEspColorVisible.y"] = CurrentConfig->ChestEspColorVisible.y;
        ConfigJson["ChestEspColorVisible.z"] = CurrentConfig->ChestEspColorVisible.z;
        ConfigJson["ChestEspColorVisible.w"] = CurrentConfig->ChestEspColorVisible.w;

        ConfigJson["bEnemyWeaponEsp"] = CurrentConfig->bEnemyWeaponEsp;
        ConfigJson["bRadarEsp"] = CurrentConfig->bRadarEsp;
        ConfigJson["RadarStartPos.x"] = CurrentConfig->RadarStartPos.x;
        ConfigJson["RadarStartPos.y"] = CurrentConfig->RadarStartPos.y;
        ConfigJson["RadarSize.x"] = CurrentConfig->RadarSize.x;
        ConfigJson["RadarSize.y"] = CurrentConfig->RadarSize.y;

        ConfigJson["GlobalNotVisible.x"] = CurrentConfig->GlobalNotVisible.x;
        ConfigJson["GlobalNotVisible.y"] = CurrentConfig->GlobalNotVisible.y;
        ConfigJson["GlobalNotVisible.z"] = CurrentConfig->GlobalNotVisible.z;
        ConfigJson["GlobalNotVisible.w"] = CurrentConfig->GlobalNotVisible.w;

        // Сохранение массива WeaponConfigs
        nlohmann::json WeaponConfigsJsonArray;
        for (const auto& weaponConfig : CurrentConfig->WeaponConfigs) {
            nlohmann::json WeaponConfigJson;
            WeaponConfigJson["WeaponName"] = weaponConfig.WeaponName;
            WeaponConfigJson["Fov"] = weaponConfig.Fov;
            WeaponConfigJson["Smooth"] = weaponConfig.Smooth;
            WeaponConfigJson["Distance"] = weaponConfig.Distance;
            WeaponConfigJson["Aimbot"] = weaponConfig.Aimbot;
            WeaponConfigJson["TriggerBot"] = weaponConfig.TriggerBot;
            //WeaponConfigJson["TriggerBotKey"] = weaponConfig.TriggerBotKey;
            //WeaponConfigJson["TriggerBotMethod"] = weaponConfig.TriggerBotMethod;
            WeaponConfigJson["Identifier"] = weaponConfig.Identifier;
            //WeaponConfigJson["AimMode"] = weaponConfig.AimMode;
            WeaponConfigsJsonArray.push_back(WeaponConfigJson);
        }
        ConfigJson["WeaponConfigs"] = WeaponConfigsJsonArray;

        // Сохранение массива AimBones
        nlohmann::json AimBonesJsonArray;
        for (const auto& aimBone : CurrentConfig->AimBones) {
            nlohmann::json AimBoneJson;
            AimBoneJson["Name"] = aimBone.Name;
            AimBoneJson["bIsActivated"] = aimBone.bIsActivated;
            AimBoneJson["Bone"] = aimBone.Bone;
            AimBonesJsonArray.push_back(AimBoneJson);
        }
        ConfigJson["AimBones"] = AimBonesJsonArray;
        ConfigJson["BulletGravity"] = CurrentConfig->BulletGravity;
        ConfigJson["BulletSpeed"] = CurrentConfig->BulletSpeed;
        ConfigJson["bPrediction"] = CurrentConfig->bPrediction;

        // Сохранение в файл
        std::ofstream outputFile(sk_crypt("C:/Config.streck"));
        if (!outputFile.is_open()) {
            return false; // Если файл не удалось открыть, вернуть false
        }

        outputFile << ConfigJson.dump(4); // Запись в файл с отступами в 4 пробела
        outputFile.close();
        return true;
    }
    return false;
}


std::unordered_map<std::string, std::unordered_map<std::string, std::string>> Dictionary = {
    {sk_crypt("Eng").decrypt(), {
        {sk_crypt("MenuKey").decrypt(), sk_crypt("Menu key").decrypt()},
        {sk_crypt("Cheat").decrypt(), sk_crypt("Cheat").decrypt()},
        {sk_crypt("Aimbot").decrypt(), sk_crypt("Aimbot").decrypt()},
        {sk_crypt("Enemy").decrypt(), sk_crypt("Enemy").decrypt()},
        {sk_crypt("World").decrypt(), sk_crypt("World").decrypt()},
        {sk_crypt("Misc").decrypt(), sk_crypt("Misc").decrypt()},
        {sk_crypt("Visuals").decrypt(), sk_crypt("Visuals").decrypt()},
        {sk_crypt("Chat").decrypt(), sk_crypt("Chat").decrypt()},
        {sk_crypt("Exploits").decrypt(), sk_crypt("Exploits").decrypt()},
        {sk_crypt("Settings").decrypt(), sk_crypt("Settings").decrypt()},
        {sk_crypt("MenuHas").decrypt(), sk_crypt("Menu has").decrypt()},
        {sk_crypt("Enabled").decrypt(), sk_crypt("Enabled").decrypt()},
        {sk_crypt("enabled").decrypt(), sk_crypt("enabled").decrypt()},
        {sk_crypt("Disabled").decrypt(), sk_crypt("Disabled").decrypt()},
        {sk_crypt("disabled").decrypt(), sk_crypt("disabled").decrypt()},
        {sk_crypt("Config").decrypt(), sk_crypt("Config").decrypt()},
        {sk_crypt("Theme").decrypt(), sk_crypt("Theme").decrypt()},
        {sk_crypt("Distance").decrypt(), sk_crypt("Distance").decrypt()},
        {sk_crypt("Fov").decrypt(), sk_crypt("Fov").decrypt()},
        {sk_crypt("Smooth").decrypt(), sk_crypt("Smooth").decrypt()},
        {sk_crypt("MISC").decrypt(), sk_crypt("MISC").decrypt()},
        {sk_crypt("AIMBOT").decrypt(), sk_crypt("AIMBOT").decrypt()},
        {sk_crypt("VISUALS").decrypt(), sk_crypt("VISUALS").decrypt()},
        {sk_crypt("SETTINGS").decrypt(), sk_crypt("SETTINGS").decrypt()},
        {sk_crypt("TriggerBot").decrypt(), sk_crypt("Trigger Bot").decrypt()},
        {sk_crypt("SaveConfig").decrypt(), sk_crypt("Save config").decrypt()},
        {sk_crypt("AimBones").decrypt(), sk_crypt("Aim Bones").decrypt()},
        {sk_crypt("BulletGravity").decrypt(), sk_crypt("Bullet Gravity").decrypt()},
        {sk_crypt("BulletSpeed").decrypt(), sk_crypt("Bullet Speed").decrypt()},
        {sk_crypt("BoxEsp").decrypt(), sk_crypt("Box Esp").decrypt()},
        {sk_crypt("FilledBoxEsp").decrypt(), sk_crypt("Filled Box Esp").decrypt()},
        {sk_crypt("CornerBoxEsp").decrypt(), sk_crypt("Corner Box Esp").decrypt()},
        {sk_crypt("LinesEsp").decrypt(), sk_crypt("Lines Esp").decrypt()},
        {sk_crypt("LinesPosition").decrypt(), sk_crypt("Lines Position").decrypt()},
        {sk_crypt("Top").decrypt(), sk_crypt("Top").decrypt()},
        {sk_crypt("Middle").decrypt(), sk_crypt("Middle").decrypt()},
        {sk_crypt("Bottom").decrypt(), sk_crypt("Bottom").decrypt()},
        {sk_crypt("EnterNotification").decrypt(), sk_crypt("Streck has started successfully,\nPlease visit our website for news!").decrypt()},

        {sk_crypt("NamesEsp").decrypt(), sk_crypt("Names Esp").decrypt()},
        {sk_crypt("SkeletonEsp").decrypt(), sk_crypt("Skeleton Esp").decrypt()},
        {sk_crypt("KillsEsp").decrypt(), sk_crypt("Kills Esp").decrypt()},
        {sk_crypt("DistanceEsp").decrypt(), sk_crypt("Distance Esp").decrypt()},

        {sk_crypt("PlatformEsp").decrypt(), sk_crypt("Platform Esp").decrypt()},
        {sk_crypt("EnemyWeaponEsp").decrypt(), sk_crypt("Enemy Weapon Esp").decrypt()},
        {sk_crypt("GlobalNotVisible").decrypt(), sk_crypt("GlobalNotVisible").decrypt()},
        {sk_crypt("ConfigSaved").decrypt(), sk_crypt("Config saved").decrypt()},
        {sk_crypt("ConfigNotSaved").decrypt(), sk_crypt("Config not saved").decrypt()},
        {sk_crypt("ChestEsp").decrypt(), sk_crypt("Chest Esp").decrypt()},
        {sk_crypt("InfoBar").decrypt(), sk_crypt("Info Bar").decrypt()},
        {sk_crypt("MenuColor").decrypt(), sk_crypt("Menu Color").decrypt()},
        {sk_crypt("OutlineColor").decrypt(), sk_crypt("Outline Color").decrypt()},
        {sk_crypt("BackGroundColor").decrypt(), sk_crypt("BackGround Color").decrypt()},
        {sk_crypt("VSync").decrypt(), sk_crypt("VSync").decrypt()},
        {sk_crypt("AimMode").decrypt(), sk_crypt("Aim Mode").decrypt()},
        {sk_crypt("ShowFov").decrypt(), sk_crypt("Show Fov").decrypt()},
        {sk_crypt("Prediction").decrypt(), sk_crypt("Prediction").decrypt()},
        {sk_crypt("BindKey").decrypt(), sk_crypt("Bind key").decrypt()},
        {sk_crypt("Ppaktb").decrypt(), sk_crypt("Please press any key to bind...").decrypt()},
        {sk_crypt("RealisticSize").decrypt(), sk_crypt("Realistic size").decrypt()},
        


    }},
    {sk_crypt("Rus").decrypt(), {
        {sk_crypt("MenuKey").decrypt(), sk_crypt("Кнопка Меню").decrypt()},
        {sk_crypt("Cheat").decrypt(), sk_crypt("Чит").decrypt()},
        {sk_crypt("Aimbot").decrypt(), sk_crypt("Аимбот").decrypt()},
        {sk_crypt("Enemy").decrypt(), sk_crypt("Враги").decrypt()},
        {sk_crypt("World").decrypt(), sk_crypt("Мир").decrypt()},
        {sk_crypt("Misc").decrypt(), sk_crypt("Остальное").decrypt()},
        {sk_crypt("Visuals").decrypt(), sk_crypt("Визуал").decrypt()},
        {sk_crypt("Chat").decrypt(), sk_crypt("Чат").decrypt()},
        {sk_crypt("Exploits").decrypt(), sk_crypt("Эксплоиты").decrypt()},
        {sk_crypt("Settings").decrypt(), sk_crypt("Настройки").decrypt()},
        {sk_crypt("MenuHas").decrypt(), sk_crypt("Меню было").decrypt()},
        {sk_crypt("Enabled").decrypt(), sk_crypt("Включено").decrypt()},
        {sk_crypt("enabled").decrypt(), sk_crypt("включено").decrypt()},
        {sk_crypt("Disabled").decrypt(), sk_crypt("Выключено").decrypt()},
        {sk_crypt("disabled").decrypt(), sk_crypt("выключено").decrypt()},
        {sk_crypt("Config").decrypt(), sk_crypt("Конфиг").decrypt()},
        {sk_crypt("Theme").decrypt(), sk_crypt("Темы").decrypt()},
        {sk_crypt("Distance").decrypt(), sk_crypt("Дистанция").decrypt()},
        {sk_crypt("Fov").decrypt(), sk_crypt("Поле зрения").decrypt()},
        {sk_crypt("Smooth").decrypt(), sk_crypt("Плавность").decrypt()},
        {sk_crypt("MISC").decrypt(), sk_crypt("ОСТАЛЬНОЕ").decrypt()},
        {sk_crypt("AIMBOT").decrypt(), sk_crypt("АИМБОТ").decrypt()},
        {sk_crypt("VISUALS").decrypt(), sk_crypt("ВИЗУАЛ").decrypt()},
        {sk_crypt("SETTINGS").decrypt(), sk_crypt("НАСТРОЙКИ").decrypt()},
        {sk_crypt("TriggerBot").decrypt(), sk_crypt("Триггер Бот").decrypt()},
        {sk_crypt("SaveConfig").decrypt(), sk_crypt("Сохранить конфиг").decrypt()},
        {sk_crypt("AimBones").decrypt(), sk_crypt("Аим кости").decrypt()},
        {sk_crypt("BulletGravity").decrypt(), sk_crypt("Гравитация пули").decrypt()},
        {sk_crypt("BulletSpeed").decrypt(), sk_crypt("Скорость пули").decrypt()},
        {sk_crypt("BoxEsp").decrypt(), sk_crypt("Боксы").decrypt()},
        {sk_crypt("FilledBoxEsp").decrypt(), sk_crypt("Заполненные Боксы").decrypt()},
        {sk_crypt("CornerBoxEsp").decrypt(), sk_crypt("Корнер Боксы").decrypt()},

        {sk_crypt("LinesEsp").decrypt(), sk_crypt("Линии").decrypt()},
        {sk_crypt("LinesPosition").decrypt(), sk_crypt("Позиция линий").decrypt()},
        {sk_crypt("Top").decrypt(), sk_crypt("Сверху").decrypt()},
        {sk_crypt("Middle").decrypt(), sk_crypt("Посередине").decrypt()},
        {sk_crypt("Bottom").decrypt(), sk_crypt("Снизу").decrypt()},
        {sk_crypt("EnterNotification").decrypt(), sk_crypt("Streck был успешно запущен,\nПожалуйста посетите наш сайт для новостей!").decrypt()},
        {sk_crypt("NamesEsp").decrypt(), sk_crypt("Никнеймы").decrypt()},
        {sk_crypt("SkeletonEsp").decrypt(), sk_crypt("Скелеты").decrypt()},
        {sk_crypt("KillsEsp").decrypt(), sk_crypt("Счетчик убийств").decrypt()},
        {sk_crypt("DistanceEsp").decrypt(), sk_crypt("Дистанция").decrypt()},
        {sk_crypt("PlatformEsp").decrypt(), sk_crypt("Платформа").decrypt()},
        {sk_crypt("EnemyWeaponEsp").decrypt(), sk_crypt("Текущее оружие врага").decrypt()},
        {sk_crypt("GlobalNotVisible").decrypt(), sk_crypt("Невидимые").decrypt()},
        {sk_crypt("ConfigSaved").decrypt(), sk_crypt("Конфиг сохранён").decrypt()},
        {sk_crypt("ConfigNotSaved").decrypt(), sk_crypt("Конфиг не был сохранён").decrypt()},

        {sk_crypt("ChestEsp").decrypt(), sk_crypt("Сундуки").decrypt()},
        {sk_crypt("InfoBar").decrypt(), sk_crypt("Инфо панель").decrypt()},
        {sk_crypt("MenuColor").decrypt(), sk_crypt("Цвет меню").decrypt()},
        {sk_crypt("OutlineColor").decrypt(), sk_crypt("Цвет аутлайна меню").decrypt()},
        {sk_crypt("BackGroundColor").decrypt(), sk_crypt("Цвет фона меню").decrypt()},
        {sk_crypt("VSync").decrypt(), sk_crypt("Вертикальная синхронизация").decrypt()},
        {sk_crypt("AimMode").decrypt(), sk_crypt("Тип аимбота").decrypt()},
        {sk_crypt("ShowFov").decrypt(), sk_crypt("Показывать фов").decrypt()},
        {sk_crypt("Prediction").decrypt(), sk_crypt("Предикшн").decrypt()},
        {sk_crypt("BindKey").decrypt(), sk_crypt("Забиндить клавишу").decrypt()},

        {sk_crypt("Ppaktb").decrypt(), sk_crypt("Пожалуйста нажмите любую кнопку для бинда...").decrypt()},
        {sk_crypt("RealisticSize").decrypt(), sk_crypt("Реальный размер").decrypt()},

                                               
    }}
};

AuthFrontEnd* CurrentAuthSession = new AuthFrontEnd;
std::vector<DefaultPawn*> EntityList;

DefaultPawn* GetPawnByPlayerState(uintptr_t APlayerState)
{
    for (int64_t i = 0; i < EntityList.size(); i++)
    {
        if (EntityList[i])
        {
            if (APlayerState == EntityList[i]->APlayerState)
            {
                return EntityList[i];
            }
 
        }
    }
    return nullptr;
}


std::vector<AItemActor*> ItemsList;

std::vector<WeaponDataDefinition*> WeaponsDefinition;



static bool IsInValidPointer(uintptr_t Pointer)
{
    return Pointer && (Pointer >= 0x10000) && (Pointer < 0xF000000000000);
}



std::string GetKeyName(WORD wVirtualKeyCode, bool bExtended) {
    std::string sKeyName;

    // Check for mouse button values
    switch (wVirtualKeyCode) {
    case VK_LBUTTON:
        return "Left Mouse Button";
    case VK_RBUTTON:
        return "Right Mouse Button";
    case VK_MBUTTON:
        return "Middle Mouse Button";
    case VK_XBUTTON1:
        return "XButton 1"; // Side button 1
    case VK_XBUTTON2:
        return "XButton 2"; // Side button 2
        // Add more cases for other mouse buttons if needed
    }

    // For non-mouse buttons, proceed with your original logic
    if (wVirtualKeyCode != 0) {
        char lpString[256];
        LPARAM lParam;

        // Get scan code
        UINT sc = MapVirtualKey(wVirtualKeyCode, 0);
        lParam = sc << 16;

        lParam |= 0x1 << 25;
        if (bExtended)
            lParam |= 0x1 << 24;

        if (GetKeyNameTextA((LONG)lParam, lpString, sizeof(lpString) / sizeof(lpString[0])) != 0)
            sKeyName = std::string(lpString);
    }

    return sKeyName;
}


bool isSubstring(const std::string& mainString, const std::string& secondString) {
    std::string lowerMainString;
    std::string lowerSecondString;

    // Преобразование строк в нижний регистр
    for (char c : mainString) {
        lowerMainString += std::tolower(c);
    }

    for (char c : secondString) {
        lowerSecondString += std::tolower(c);
    }

    // Проверка подстроки
    return lowerMainString.find(lowerSecondString) != std::string::npos;
}

void HandleFilesRaw()
{
    D3DX11_IMAGE_LOAD_INFO info; ID3DX11ThreadPump* pump{ nullptr };

    while (1)
    {

        if (!Offsets::UWorld) CurrentAuthSession->GetOffset(&Offsets::UWorld, CurrentAuthSession->CurrentAItem.ItemName, sk_crypt("UWorld").decrypt());
        if (!Offsets::MatrixPointer) CurrentAuthSession->GetOffset(&Offsets::MatrixPointer, CurrentAuthSession->CurrentAItem.ItemName, sk_crypt("MatrixPointer").decrypt());
        if (!Offsets::UGameInstance) CurrentAuthSession->GetOffset(&Offsets::UGameInstance, CurrentAuthSession->CurrentAItem.ItemName, sk_crypt("UGameInstance").decrypt());
        if (!Offsets::AGameState) CurrentAuthSession->GetOffset(&Offsets::AGameState, CurrentAuthSession->CurrentAItem.ItemName, sk_crypt("AGameState").decrypt());
        if (!Offsets::TLocalPlayers) CurrentAuthSession->GetOffset(&Offsets::TLocalPlayers, CurrentAuthSession->CurrentAItem.ItemName, sk_crypt("TLocalPlayers").decrypt());
        if (!Offsets::PlayerNameEncrypted) CurrentAuthSession->GetOffset(&Offsets::PlayerNameEncrypted, CurrentAuthSession->CurrentAItem.ItemName, sk_crypt("PlayerNameEncrypted").decrypt());
        if (!Offsets::RenderTimes) CurrentAuthSession->GetOffset(&Offsets::RenderTimes, CurrentAuthSession->CurrentAItem.ItemName, sk_crypt("RenderTimes").decrypt());
        if (!Offsets::TargetedFortPawn) CurrentAuthSession->GetOffset(&Offsets::TargetedFortPawn, CurrentAuthSession->CurrentAItem.ItemName, sk_crypt("TargetedFortPawn").decrypt());
        if (!Offsets::TeamIndex) CurrentAuthSession->GetOffset(&Offsets::TeamIndex, CurrentAuthSession->CurrentAItem.ItemName, sk_crypt("TeamIndex").decrypt());
        if (!Offsets::KillScore) CurrentAuthSession->GetOffset(&Offsets::KillScore, CurrentAuthSession->CurrentAItem.ItemName, sk_crypt("KillScore").decrypt());
        if (!Offsets::Aim) CurrentAuthSession->GetOffset(&Offsets::Aim, CurrentAuthSession->CurrentAItem.ItemName, sk_crypt("Aim").decrypt());
        if (!Offsets::WeaponData) CurrentAuthSession->GetOffset(&Offsets::WeaponData, CurrentAuthSession->CurrentAItem.ItemName, sk_crypt("WeaponData").decrypt());


        if (pic::aimbot == nullptr) {
            std::string aimbotRawData = CurrentAuthSession->GetFile(sk_crypt("/media/aimbot.png").decrypt());
            std::vector<uint8_t> aimbotRaw(aimbotRawData.begin(), aimbotRawData.end());
            D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, aimbotRaw.data(), aimbotRaw.size(), &info, pump, &pic::aimbot, 0);
        }

        if (pic::visuals == nullptr) {
            std::string visualsRawData = CurrentAuthSession->GetFile(sk_crypt("/media/visuals.png").decrypt());
            std::vector<uint8_t> visualsRaw(visualsRawData.begin(), visualsRawData.end());
            D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, visualsRaw.data(), visualsRaw.size(), &info, pump, &pic::visuals, 0);

        }
        if (pic::exploits == nullptr) {
            std::string exploitsRawData = CurrentAuthSession->GetFile(sk_crypt("/media/exploits.png").decrypt());
            std::vector<uint8_t> exploitsRaw(exploitsRawData.begin(), exploitsRawData.end());
            D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, exploitsRaw.data(), exploitsRaw.size(), &info, pump, &pic::exploits, 0);
        }


        if (pic::settings == nullptr)
        {
            std::string settingsRawData = CurrentAuthSession->GetFile(sk_crypt("/media/settings.png").decrypt());
            std::vector<uint8_t> settingsRaw(settingsRawData.begin(), settingsRawData.end());
            D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, settingsRaw.data(), settingsRaw.size(), &info, pump, &pic::settings, 0);
        }


        if (pic::menu_settings_icon == nullptr) {
            std::string menu_settings_iconRawData = CurrentAuthSession->GetFile(sk_crypt("/media/strecklogo.jpg").decrypt());
            std::vector<uint8_t> menu_settings_iconRaw(menu_settings_iconRawData.begin(), menu_settings_iconRawData.end());
            D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, menu_settings_iconRaw.data(), menu_settings_iconRaw.size(), &info, pump, &pic::menu_settings_icon, 0);
        }

        if (pic::blur1 == nullptr) {
            std::string blur1RawData = CurrentAuthSession->GetFile(sk_crypt("/media/blur1.png").decrypt());
            std::vector<uint8_t> blur1Raw(blur1RawData.begin(), blur1RawData.end());
            D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, blur1Raw.data(), blur1Raw.size(), &info, pump, &pic::blur1, 0);
        }
        
        if (pic::blur2 == nullptr) {
            std::string blur2RawData = CurrentAuthSession->GetFile(sk_crypt("/media/blur2.png").decrypt());
            std::vector<uint8_t> blur2Raw(blur2RawData.begin(), blur2RawData.end());
            D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, blur2Raw.data(), blur2Raw.size(), &info, pump, &pic::blur2, 0);
        }

        if (pic::combo_widget == nullptr) {
            D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, combo_widget, sizeof(combo_widget), &info, pump, &pic::combo_widget, 0);
        }
        if (pic::input_widget == nullptr) {
            D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, input_widget, sizeof(input_widget), &info, pump, &pic::input_widget, 0);
        }

        if (pic::circle_success == nullptr) {
            D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, circle_success, sizeof(circle_success), &info, pump, &pic::circle_success, 0);
        }
        if (pic::circle_error == nullptr) {
            D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, circle_error, sizeof(circle_error), &info, pump, &pic::circle_error, 0);
        }

        if (pic::android_logo == nullptr) {
            std::string android_logoRawData = CurrentAuthSession->GetFile(sk_crypt("/media/android_logo.png").decrypt());
            std::vector<uint8_t> android_logoRaw(android_logoRawData.begin(), android_logoRawData.end());
            D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, android_logoRaw.data(), android_logoRaw.size(), &info, pump, &pic::android_logo, 0);
        }

        if (pic::ios_logo == nullptr) {
            std::string ios_logoRawData = CurrentAuthSession->GetFile(sk_crypt("/media/ios_logo.png").decrypt());
            std::vector<uint8_t> ios_logoRaw(ios_logoRawData.begin(), ios_logoRawData.end());
            D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, ios_logoRaw.data(), ios_logoRaw.size(), &info, pump, &pic::ios_logo, 0);
        }

        if (pic::Nintendo_logo == nullptr) {
            std::string Nintendo_logoRawData = CurrentAuthSession->GetFile(sk_crypt("/media/Nintendo_logo.png").decrypt());
            std::vector<uint8_t> Nintendo_logoRaw(Nintendo_logoRawData.begin(), Nintendo_logoRawData.end());
            D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, Nintendo_logoRaw.data(), Nintendo_logoRaw.size(), &info, pump, &pic::Nintendo_logo, 0);
        }

        if (pic::old_windows_logo == nullptr) {
            std::string old_windows_logoRawData = CurrentAuthSession->GetFile(sk_crypt("/media/old_windows_logo.png").decrypt());
            std::vector<uint8_t> old_windows_logoRaw(old_windows_logoRawData.begin(), old_windows_logoRawData.end());
            D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, old_windows_logoRaw.data(), old_windows_logoRaw.size(), &info, pump, &pic::old_windows_logo, 0);
        }

        if (pic::playstation_logo == nullptr) {
            std::string playstation_logoRawData = CurrentAuthSession->GetFile(sk_crypt("/media/playstation_logo.png").decrypt());
            std::vector<uint8_t> playstation_logoRaw(playstation_logoRawData.begin(), playstation_logoRawData.end());
            D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, playstation_logoRaw.data(), playstation_logoRaw.size(), &info, pump, &pic::playstation_logo, 0);
        }
        if (pic::xbox_logo == nullptr) {
            std::string xbox_logoRawData = CurrentAuthSession->GetFile(sk_crypt("/media/xbox_logo.png").decrypt());
            std::vector<uint8_t> xbox_logoRaw(xbox_logoRawData.begin(), xbox_logoRawData.end());
            D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, xbox_logoRaw.data(), xbox_logoRaw.size(), &info, pump, &pic::xbox_logo, 0);
        }

        if (pic::Chat == nullptr) {
            std::string ChatRawData = CurrentAuthSession->GetFile(sk_crypt("/media/Chat.png").decrypt());
            std::vector<uint8_t> ChatRaw(ChatRawData.begin(), ChatRawData.end());
            D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, ChatRaw.data(), ChatRaw.size(), &info, pump, &pic::Chat, 0);
        }

        if (pic::Camera == nullptr) {
            std::string CameraRawData = CurrentAuthSession->GetFile(sk_crypt("/media/Camera.png").decrypt());
            std::vector<uint8_t> CameraRaw(CameraRawData.begin(), CameraRawData.end());
            D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, CameraRaw.data(), CameraRaw.size(), &info, pump, &pic::Camera, 0);
        }

        if (!filesystem::exists("C:/ProgramData/MinecraftClickButtonSound.wav"))
        {
            std::string AudioData = CurrentAuthSession->GetFile(sk_crypt("/media/MinecraftClickButtonSound.wav").decrypt());
            std::vector<uint8_t> AudioDataRaw(AudioData.begin(), AudioData.end());
            std::ofstream file("C:/ProgramData/MinecraftClickButtonSound.wav", std::ios::out | std::ios::binary);
            file.write(reinterpret_cast<const char*>(AudioDataRaw.data()), AudioDataRaw.size());
            file.close();

            
        }


            //std::string VideoData = CurrentAuthSession->GetFile(sk_crypt("/media/video.mp4").decrypt());
            //std::vector<uint8_t> VideoRaw(VideoData.begin(), VideoData.end());
            //D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, CameraRaw.data(), CameraRaw.size(), &info, pump, &pic::Camera, 0);

        if (!CurrentAuthSession->SSEActive)
        {
            CurrentAuthSession->SetupEventSource();
            //std::cout << "\n SSEActive is not actived, SetupEventSource was called and returned " << CurrentAuthSession->SetupEventSource();
        }
        Sleep(200);
    }
}


void DrawBucketShape(ImDrawList* drawList, ImVec2 center, float radius, ImVec4 startColor, ImVec4 endColor)
{
    const int segments = 10; // Количество сегментов для аппроксимации окружности

    for (int i = 0; i < segments; ++i)
    {
        float theta0 = static_cast<float>(i) / segments * 3.1415926f * 0.5f + 3.1415926f * 0.5f + 3.1415926f * 0.5f + (3.1415926f * 0.5f) / 2; // Начинаем с верхней полуплоскости (π/2 радиан)
        float theta1 = static_cast<float>(i + 1) / segments * 3.1415926f * 0.5f + 3.1415926f * 0.5f + 3.1415926f * 0.5f + (3.1415926f * 0.5f) / 2;

        ImVec2 p0(cosf(theta0) * radius + center.x, sinf(theta0) * radius + center.y);
        ImVec2 p1(cosf(theta1) * radius + center.x, sinf(theta1) * radius + center.y);

        // Рассчитываем градиентные цвета для текущего сегмента
        ImVec4 segmentStartColor = ImVec4(
            startColor.x + (endColor.x - startColor.x) * static_cast<float>(i) / segments,
            startColor.y + (endColor.y - startColor.y) * static_cast<float>(i) / segments,
            startColor.z + (endColor.z - startColor.z) * static_cast<float>(i) / segments,
            startColor.w + (endColor.w - startColor.w) * static_cast<float>(i) / segments
        );

        ImVec4 segmentEndColor = ImVec4(
            startColor.x + (endColor.x - startColor.x) * static_cast<float>(i + 1) / segments,
            startColor.y + (endColor.y - startColor.y) * static_cast<float>(i + 1) / segments,
            startColor.z + (endColor.z - startColor.z) * static_cast<float>(i + 1) / segments,
            startColor.w + (endColor.w - startColor.w) * static_cast<float>(i + 1) / segments
        );

        // Добавляем четырехугольник (сегмент) с градиентным заполнением
        drawList->AddQuadFilled(center, p0, p1, center, ImColor(segmentStartColor)); // Используем только startColor для Fill
    }
}


//void draw_corner_box(ImDrawList* drawList, int X, int Y, int W, int H, const ImColor color, int thickness) {
//    float lineW = (W / 6);
//    float lineH = (H / 6);
//
//    drawList->AddLine(ImVec2(X, Y), ImVec2(X, Y + lineH), color, thickness);
//    drawList->AddLine(ImVec2(X, Y), ImVec2(X + lineW, Y), color, thickness);
//    drawList->AddLine(ImVec2(X + W - lineW, Y), ImVec2(X + W, Y), color, thickness);
//    drawList->AddLine(ImVec2(X + W, Y), ImVec2(X + W, Y + lineH), color, thickness);
//    drawList->AddLine(ImVec2(X, Y + H - lineH), ImVec2(X, Y + H), color, thickness);
//    drawList->AddLine(ImVec2(X, Y + H), ImVec2(X + lineW, Y + H), color, thickness);
//    drawList->AddLine(ImVec2(X + W - lineW, Y + H), ImVec2(X + W, Y + H), color, thickness);
//    drawList->AddLine(ImVec2(X + W, Y + H - lineH), ImVec2(X + W, Y + H), color, thickness);
//}

void draw_corner_box(ImDrawList* drawList, int X, int Y, int W, int H, const ImColor color, int thickness) {
    float lineW = (W / 6);
    float lineH = (H / 6);

    drawList->AddLine(ImVec2(X, Y), ImVec2(X, Y + lineH), color, thickness);
    drawList->AddLine(ImVec2(X, Y), ImVec2(X + lineW, Y), color, thickness);
    drawList->AddLine(ImVec2(X + W - lineW, Y), ImVec2(X + W, Y), color, thickness);
    drawList->AddLine(ImVec2(X + W, Y), ImVec2(X + W, Y + lineH), color, thickness);
    drawList->AddLine(ImVec2(X, Y + H - lineH), ImVec2(X, Y + H), color, thickness);
    drawList->AddLine(ImVec2(X, Y + H), ImVec2(X + lineW, Y + H), color, thickness);
    drawList->AddLine(ImVec2(X + W - lineW, Y + H), ImVec2(X + W, Y + H), color, thickness);
    drawList->AddLine(ImVec2(X + W, Y + H - lineH), ImVec2(X + W, Y + H), color, thickness);
}

struct FAimBotRequest {
    int X = 0;
    int Y = 0;
    int KmBoxModel = 0;
};

//std::vector<FAimBotRequest> AimBotRequests;
//
//int HandleKmBoxAimBot()
//{
//    std::cout << "\n\n AimThread Created!";
//
//    while (true)
//    {
//        //std::cout << "\n Thread Active!" << AimBotRequests.size();
//
//        if (AimBotRequests.size() > 0)
//        {
//            //std::cout << "\n got request: " << AimBotRequests[0].KmBoxModel;
//
//            if (AimBotRequests[0].KmBoxModel == 0) {
//                KmBoxMgr.Mouse.Move(AimBotRequests[0].X, AimBotRequests[0].Y);
//            }
//            else if (AimBotRequests[0].KmBoxModel == 1) {
//                kmBoxBMgr.km_move(AimBotRequests[0].X, AimBotRequests[0].Y);
//            }
//
//            // Remove the current request from the vector
//            AimBotRequests.erase(AimBotRequests.begin() + 0);
//
//        }
//        Sleep(1);
//    }
//}


#include <memory> // Для std::shared_ptr (можно использовать boost::shared_ptr, если стандартная библиотека не подходит)

// Пример класса, на который будет указывать мягкая ссылка


// Шаблонный класс мягкой ссылки
template<typename T>
class TSoftClassPtr {
private:
    std::weak_ptr<T> WeakPtr; // Мягкая ссылка на объект
public:
    // Конструктор по умолчанию
    TSoftClassPtr() {}

    // Конструктор, принимающий shared_ptr на объект
    TSoftClassPtr(std::shared_ptr<T> ptr) : WeakPtr(ptr) {}

    // Метод для получения объекта, на который указывает мягкая ссылка
    std::shared_ptr<T> LoadSynchronous() {
        return WeakPtr.lock(); // Возвращает shared_ptr на объект, если он существует
    }

    // Метод для проверки, является ли мягкая ссылка действительной
    bool IsValid() const {
        return !WeakPtr.expired(); // Возвращает true, если объект еще существует
    }
};

//int main(int, char**)
int HiGame()
{

    //while (1)
    //{
    //    uintptr_t* Gay = new uintptr_t;
    ////}
    AllocConsole();

    // Получение дескриптора консоли
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Переключение на консольный вывод
    freopen("CONOUT$", "w", stdout);
   

    // Переключение на консольный ввод
    freopen("CONIN$", "r", stdin);

    //std::cout << "\n gay";

    D3X11CreateShaderResourceViewFromMemory();

    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        //std::cerr << "WSAStartup failed: " << iResult << std::endl;
    }
    //std::thread KmBoxThread(HandleKmBoxAimBot);
    //KmBoxThread.detach();
    //ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    //if (ConnectSocket == INVALID_SOCKET) {
    //    //std::cerr << "socket failed: " << WSAGetLastError() << std::endl;
    //    WSACleanup();
    //}


    clientService.sin_family = AF_INET;
    clientService.sin_addr.s_addr = inet_addr(sk_crypt("89.201.4.223"));
    //clientService.sin_addr.s_addr = inet_addr("127.0.0.1");
    clientService.sin_port = htons(7777);

    int ApiStatus = CurrentAuthSession->bIsApiVersionValid();
    if (ApiStatus == 0)
    {

        //std::cout << "\n This app is outdated. Please download new version!";
        return 0;
    }
    else if (ApiStatus == 10)
    {
        MessageBoxA(0, sk_crypt("Cant connect to server. Please check your ethernet connection and server status on https://streck.md!"), sk_crypt("Streck"), MB_OK);
        exit(0);
        //std::cout << "\n Cant connect to server. Please check your ethernet connection and server status on https://streck.md!";
        return 10;

    }

    std::string FileData = Func::readFile(sk_crypt("C:/streck.sess").decrypt());
    
    CurrentAuthSession->SessionId = FileData;
    if (CurrentAuthSession->ValidateSessionId() == 1)
    {
        //std::cout << "\n Welcome back, " << CurrentAuthSession->UserName;
       // CurrentAuthSession->ItemName = sk_crypt("Fortnite Private");
#ifdef DMA
        CurrentAuthSession->ItemName = sk_crypt("Fortnite Dma");

#endif // DMA

#ifdef Driver
        CurrentAuthSession->ItemName = sk_crypt("Fortnite Private");
#endif // Driver

        //
        CurrentAuthSession->GetInventoryItems();

        for (int64_t i = 0; i < CurrentAuthSession->Inventory.size(); i++)
        {
            if (Func::CompareStringsIgnoreCase(CurrentAuthSession->Inventory[i].ItemName, CurrentAuthSession->ItemName))
            {
                if (CurrentAuthSession->CurrentAItem.ActivatedTimeStamp + CurrentAuthSession->CurrentAItem.DurationTimeStamp > Func::GetUTCTime() || !CurrentAuthSession->CurrentAItem.bIsTempExistItem)
                {
                    CurrentAuthSession->CurrentAItem = CurrentAuthSession->Inventory[i];
                }
            }
        }
        if (CurrentAuthSession->CurrentAItem.ItemId && CurrentAuthSession->CurrentAItem.ActivatedTimeStamp)
        {
            if (CurrentAuthSession->CheckOrAddHwid(CurrentAuthSession->CurrentAItem.LicenseKey, Func::GetHwid()) == 1)
            {

            }
            else
            {
                MessageBoxA(0, sk_crypt("Hwid is invalid..."), sk_crypt("Streck"), MB_OK);
                exit(0);

                //std::cout << "\n Hwid is invalid";
                return 0;
            }
        }
        else
        {
            MessageBoxA(0, sk_crypt("You have no activated product..."), sk_crypt("Streck"), MB_OK);
            exit(0);

            //std::cout << "\n You have no activated product...";
            return 0;
        }
    }
    else
    {
        //MessageBoxA(0, sk_crypt("Session Id is invalid..."), sk_crypt("Streck"), MB_OK);
        MessageBoxA(0, CurrentAuthSession->LastError.c_str(), sk_crypt("Streck"), MB_OK);
        exit(0);
        //std::cout << "\n Session Id is invalid";

        return 10;

    }


#ifdef Driver
    driver->startup();
    if (!GlobalFindAtomA(sk_crypt("x/")))
    {
        //MessageBoxA(0, "Do driver map", "Do driver map", MB_OK);
        //MessageBoxA(0, sk_crypt("First time loading"), sk_crypt("streck"), MB_OK);
        std::string FileData = CurrentAuthSession->GetFile(sk_crypt("/media/te_driver.sys").decrypt());
        std::vector<uint8_t> vFileData(FileData.begin(), FileData.end());
        if (vMapDriver(vFileData))
        {
            GlobalAddAtomA(sk_crypt("x/"));
        }
        else
        {
            MessageBoxA(0, sk_crypt("FAILED! \nTry to: \n\n1) Delete FaceIt anticheat\n\n2) Delete Vanguard anticheat\n\n3) Disable SecureBoot\n\n4) Install latest ReviOs Windows 10 version\n\n5) Make sure game is off\n6) Run opera as admin\n7) If its still not working, in Opera.exe settings put mark on `Run this program as an administrator` in Compatibility tab"), sk_crypt("Failed"), MB_OK);
            exit(0);
        }

    }

    if (!driver->launch())
    {
        //MessageBoxA(0, "loaded", "loaded", MB_OK);
    }
    else
    {
        MessageBoxA(0, sk_crypt("INVALID LAUNCH"), sk_crypt("streck"), MB_OK);
        exit(0);
    }
    //MessageBoxA(0, sk_crypt("Close this box when game is loaded"), sk_crypt("Waiting"), MB_OK);
    while (driver->pid == 0)
    {
        driver->pid = driver->query_pid_by_process_name(sk_crypt(L"FortniteClient-Win64-Shipping.exe").decrypt());
        Sleep(500);
    }
    driver->attach();

#endif // Driver

#ifdef DMA

    driver->InitMemory();

    if (!driver->Init("FortniteClient-Win64-Shipping.exe", false, false))
    {
        std::cout << "Failed to initilize DMA" << std::endl;
        Sleep(2000);
        return 1;
    }

    std::cout << "DMA initilized" << std::endl;

    if (!driver->GetKeyboard()->InitKeyboard())
    {
        std::cout << "Failed to initialize keyboard hotkeys through kernel." << std::endl;
        //Sleep(2000);

        //return 1;
    }


    if (!driver->FixCr3())
        std::cout << "Failed to fix CR3" << std::endl;
    //else
    //    std::cout << "CR3 fixed" << std::endl;

    driver->base = driver->GetBaseDaddy("FortniteClient-Win64-Shipping.exe");
#endif // DMA

    {
        FScreamer Scream;
        Scream.StartTime = 0;
        Scream.Duration = 3;
        Scream.AudioName = "jumpscare_1.wav";
        Scream.ScreamerName = "scream";
        FImageData* NewScreamImageData = new FImageData;
        NewScreamImageData->ImageName = "3xsvsj.jpg";

        std::string ScreamRawData = CurrentAuthSession->GetFile("/media/MegaPrank/" + NewScreamImageData->ImageName);
        std::vector<uint8_t> ScreamRaw(ScreamRawData.begin(), ScreamRawData.end());

        NewScreamImageData->ImageRaw = ScreamRaw;

        //NewScreamImageData->ImageBuffer = ScreamRaw.data();
        //NewScreamImageData->ImageBufferSize = ScreamRaw.size();

        std::string ScreamAudioRawData = CurrentAuthSession->GetFile("/media/MegaPrank/fx/" + Scream.AudioName);
        std::vector<uint8_t> ScreamAudioRaw(ScreamAudioRawData.begin(), ScreamAudioRawData.end());
        std::ofstream fileff("C:/ProgramData/" + Scream.AudioName, std::ios::out | std::ios::binary);
        fileff.write(reinterpret_cast<const char*>(ScreamAudioRaw.data()), ScreamAudioRaw.size());
        fileff.close();

        Scream.Images.push_back(NewScreamImageData);

        ScreamersPresets.push_back(Scream);
    }
    {
        FScreamer ScreamAnne;
        ScreamAnne.StartTime = 0;
        ScreamAnne.Duration = 4;
        ScreamAnne.AudioName = "Screamer_anne.wav";
        ScreamAnne.ScreamerName = "anne";
        FImageData* NewScreamImageDataAnneNorm = new FImageData;
        NewScreamImageDataAnneNorm->ImageName = "anne_norm.jpg";
        ScreamAnne.Images.push_back(NewScreamImageDataAnneNorm);


        FImageData* NewScreamImageDataAnneRev = new FImageData;
        NewScreamImageDataAnneRev->ImageName = "anne_rev.jpg";

        ScreamAnne.Images.push_back(NewScreamImageDataAnneRev);

        {
            std::string ScreamRawData = CurrentAuthSession->GetFile("/media/MegaPrank/" + NewScreamImageDataAnneNorm->ImageName);
            std::vector<uint8_t> ScreamRaw(ScreamRawData.begin(), ScreamRawData.end());
            NewScreamImageDataAnneNorm->ImageRaw = ScreamRaw;
            //NewScreamImageDataAnneNorm->ImageBuffer = ScreamRaw.data();
            //NewScreamImageDataAnneNorm->ImageBufferSize = ScreamRaw.size();
        }

        {
            std::string ScreamRawData = CurrentAuthSession->GetFile("/media/MegaPrank/" + NewScreamImageDataAnneRev->ImageName);
            std::vector<uint8_t> ScreamRaw(ScreamRawData.begin(), ScreamRawData.end());
            NewScreamImageDataAnneRev->ImageRaw = ScreamRaw;

            //NewScreamImageDataAnneRev->ImageBuffer = ScreamRaw.data();
            //NewScreamImageDataAnneRev->ImageBufferSize = ScreamRaw.size();
        }

        std::string ScreamAudioRawData = CurrentAuthSession->GetFile("/media/MegaPrank/fx/" + ScreamAnne.AudioName);
        std::vector<uint8_t> ScreamAudioRaw(ScreamAudioRawData.begin(), ScreamAudioRawData.end());
        std::ofstream fileff("C:/ProgramData/" + ScreamAnne.AudioName, std::ios::out | std::ios::binary);
        fileff.write(reinterpret_cast<const char*>(ScreamAudioRaw.data()), ScreamAudioRaw.size());
        fileff.close();

        ScreamersPresets.push_back(ScreamAnne);
    }

    {
        FScreamer ScreamAnne;
        ScreamAnne.StartTime = 0;
        ScreamAnne.Duration = 3;
        ScreamAnne.AudioName = "NoisePanic.wav";
        ScreamAnne.ScreamerName = "rand";
        FImageData* NewScreamImageDataAnneNorm = new FImageData;
        NewScreamImageDataAnneNorm->ImageName = "facegay.png";
        ScreamAnne.Images.push_back(NewScreamImageDataAnneNorm);


        FImageData* NewScreamImageDataAnneRev = new FImageData;
        NewScreamImageDataAnneRev->ImageName = "gay.png";
        ScreamAnne.Images.push_back(NewScreamImageDataAnneRev);

        FImageData* NewScreamImageDataAnneRev2 = new FImageData;
        NewScreamImageDataAnneRev2->ImageName = "horror.png";
        ScreamAnne.Images.push_back(NewScreamImageDataAnneRev2);

        {
            std::string ScreamRawData = CurrentAuthSession->GetFile("/media/MegaPrank/" + NewScreamImageDataAnneNorm->ImageName);
            std::vector<uint8_t> ScreamRaw(ScreamRawData.begin(), ScreamRawData.end());
            NewScreamImageDataAnneNorm->ImageRaw = ScreamRaw;
            //NewScreamImageDataAnneNorm->ImageBuffer = ScreamRaw.data();
            //NewScreamImageDataAnneNorm->ImageBufferSize = ScreamRaw.size();
        }

        {
            std::string ScreamRawData = CurrentAuthSession->GetFile("/media/MegaPrank/" + NewScreamImageDataAnneRev->ImageName);
            std::vector<uint8_t> ScreamRaw(ScreamRawData.begin(), ScreamRawData.end());
            NewScreamImageDataAnneRev->ImageRaw = ScreamRaw;

            //NewScreamImageDataAnneRev->ImageBuffer = ScreamRaw.data();
            //NewScreamImageDataAnneRev->ImageBufferSize = ScreamRaw.size();
        }

        {
            std::string ScreamRawData = CurrentAuthSession->GetFile("/media/MegaPrank/" + NewScreamImageDataAnneRev2->ImageName);
            std::vector<uint8_t> ScreamRaw(ScreamRawData.begin(), ScreamRawData.end());
            NewScreamImageDataAnneRev2->ImageRaw = ScreamRaw;

            //NewScreamImageDataAnneRev->ImageBuffer = ScreamRaw.data();
            //NewScreamImageDataAnneRev->ImageBufferSize = ScreamRaw.size();
        }

        std::string ScreamAudioRawData = CurrentAuthSession->GetFile("/media/MegaPrank/fx/" + ScreamAnne.AudioName);
        std::vector<uint8_t> ScreamAudioRaw(ScreamAudioRawData.begin(), ScreamAudioRawData.end());
        std::ofstream fileff("C:/ProgramData/" + ScreamAnne.AudioName, std::ios::out | std::ios::binary);
        fileff.write(reinterpret_cast<const char*>(ScreamAudioRaw.data()), ScreamAudioRaw.size());
        fileff.close();

        ScreamersPresets.push_back(ScreamAnne);
    }

    if (LoadConfig(&CurrentConfig)) {

    }
    else
    {
        FWeaponConfig RifleConfig;
        RifleConfig.WeaponName = sk_crypt("Rifle");
        RifleConfig.Identifier = 1;
        RifleConfig.Fov = 150;
        RifleConfig.Smooth = 10;
        RifleConfig.Distance = 300;

        FWeaponConfig SMGConfig;
        SMGConfig.WeaponName = sk_crypt("SMG");
        SMGConfig.Identifier = 2;
        SMGConfig.Fov = 130;
        SMGConfig.Smooth = 15;
        SMGConfig.Distance = 100;

        FWeaponConfig ShotGunConfig;
        ShotGunConfig.WeaponName = sk_crypt("ShotGun");
        ShotGunConfig.Identifier = 3;
        ShotGunConfig.Fov = 100;
        ShotGunConfig.Smooth = 5;

        FWeaponConfig SniperConfig;
        SniperConfig.WeaponName = sk_crypt("Sniper");
        SniperConfig.Identifier = 4;
        SniperConfig.Fov = 400;
        SniperConfig.Smooth = 10;
        SniperConfig.Distance = 500;

        FWeaponConfig DefaultConfig;
        DefaultConfig.WeaponName = sk_crypt("Default");
        DefaultConfig.Identifier = 0;
        DefaultConfig.Fov = 150;
        DefaultConfig.Smooth = 10;
        DefaultConfig.Distance = 275;


        CurrentConfig.WeaponConfigs.push_back(RifleConfig);
        CurrentConfig.WeaponConfigs.push_back(SMGConfig);
        CurrentConfig.WeaponConfigs.push_back(ShotGunConfig);
        CurrentConfig.WeaponConfigs.push_back(SniperConfig);
        CurrentConfig.WeaponConfigs.push_back(DefaultConfig);

    }

    CurrentConfig.AimBones = {
    FBone(sk_crypt("Head").decrypt(), true, 110),
    FBone(sk_crypt("Neck").decrypt(), false, 66),
    FBone(sk_crypt("Penis").decrypt(), false, 2),
    FBone(sk_crypt("Right Shoulder").decrypt(), false, 105),
    FBone(sk_crypt("Left Shoulder").decrypt(), false, 35),

    FBone(sk_crypt("Right Elbow").decrypt(), false, 106),
    FBone(sk_crypt("Left Elbow").decrypt(), false, 10),

    FBone(sk_crypt("Right Knee").decrypt(), false, 79),
    FBone(sk_crypt("Left Knee").decrypt(), false, 72),

    FBone(sk_crypt("Right Hand").decrypt(), false, 117),
    FBone(sk_crypt("Left Hand").decrypt(), false, 118),

    FBone(sk_crypt("Right Foot").decrypt(), false, 87),
    FBone(sk_crypt("Left Foot").decrypt(), false, 86),
    };

    if (CurrentConfig.KmBoxIp == "0")
    {
        int Choice = 0;
        std::cout << "\n\n\nDo you have KmBox Net Device?\n[1] Yes.\n[2] No.\n[user]: ";
        std::cin >> Choice;

        if (Choice == 1)
        {
            std::cout << "\nPlease enter Ip:";
            std::cin >> CurrentConfig.KmBoxIp;
            std::cout << "\nPlease enter Port:";
            std::cin >> CurrentConfig.KmBoxPort;
            std::cout << "\nPlease enter Uuid:";
            std::cin >> CurrentConfig.KmBoxUuid;
        }
        else if (Choice == 2)
        {
        }
        else
        {

        }
    }

 
    //if (!filesystem::exists("kmbox.json"))
    //{


    //}

    //KmBoxDataGetter kmbx("kmbox.json");




    //WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"Chrome_WidgetWin_1", nullptr };
    //::RegisterClassExW(&wc);
    //HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Picture in Picture", WS_POPUP, 0, 0, 1920, 1080, nullptr, nullptr, wc.hInstance, nullptr);

    //std::vector<uintptr_t> UWorlds;
    //uint64_t start = 0x9000000;

    //std::vector<uintptr_t> TempFirstLevel(90000000);
    //driver->read_mem(driver->base, TempFirstLevel.data(), TempFirstLevel.size() * 0x8);

    //std::cout << "\n size: " << std::dec << TempFirstLevel.size();
    //for (int64_t i = 0; i < TempFirstLevel.size(); i++)
    //{
    //    //uintptr_t CurrentDataWorld = driver->read<uintptr_t>(driver->base + start + i * 0x8);
    //    uintptr_t CurrentDataWorld = TempFirstLevel[i];
    //    if (CurrentDataWorld)
    //    {
    //        uintptr_t CurrentDataGameState = driver->read<uintptr_t>(CurrentDataWorld + 0x160);
    //        if (CurrentDataGameState)
    //        {
    //            float CurrentTimer = driver->read<float>(CurrentDataGameState + 0x68);
    //            if (CurrentTimer == 1.f)
    //            {
    //                if (0x1216b7b8 == (start + i * 0x8))
    //                {
    //                    continue;
    //                }
    //                std::cout << "\n found new offset: 0x" << std::hex << start + i * 0x8;
    //                UWorlds.push_back(start + i * 0x8);
    //            }
    //        }
    //    }
    //}

    HWND hwnd = CreateWin(NULL, sk_crypt(L"Picture in Picture").decrypt(), sk_crypt(L"Chrome_WidgetWin_1").decrypt());

    if (!hwnd)
    {
        MessageBoxA(0, sk_crypt("Error 6"), 0, 0);
    }

    if (!CreateDeviceD3D(hwnd))
    {
        MessageBoxA(0, sk_crypt("Error 7"), 0, 0);
    }

    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    SEGAUI_CHECKVERSION();
    SegaUi::CreateContext();
    SegaUiIO& io = SegaUi::GetIO(); (void)io;
    io.ConfigFlags |= SegaUiConfigFlags_NavEnableKeyboard;  
    io.ConfigFlags |= SegaUiConfigFlags_NavEnableGamepad;   

    ImFontConfig cfg;
    cfg.FontBuilderFlags = SegaUiFreeTypeBuilderFlags_ForceAutoHint | SegaUiFreeTypeBuilderFlags_LightHinting | SegaUiFreeTypeBuilderFlags_LoadColor;

    font::inter_default = io.Fonts->AddFontFromMemoryTTF(inter_medium, sizeof(inter_medium), 17.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    font::inter_bold = io.Fonts->AddFontFromMemoryTTF(inter_bold, sizeof(inter_bold), 17.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    font::inter_bold_v2 = io.Fonts->AddFontFromMemoryTTF(inter_bold, sizeof(inter_bold), 50.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
    font::icon = io.Fonts->AddFontFromMemoryTTF(icon_font, sizeof(icon_font), 15.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());

    SegaUi::StyleColorsDark();


    SegaUi_ImplWin32_Init(hwnd);
    SegaUi_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 0.00f);
    const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };

    RECT screen_rect{};
    GetWindowRect(GetDesktopWindow(), &screen_rect);
    GameInfo.ViewRect = { screen_rect.left, screen_rect.top, screen_rect.right, screen_rect.bottom };

    ImVec2 MenuScale;
    ImVec2 MenuPos;

    std::thread HandleF(HandleFilesRaw);
    HandleF.detach();

    //std::cout << "\n gonna search uworld...";
    //SetWindowLongPtr(hwnd, GWL_EXSTYLE, GetWindowLongPtr(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
    //SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 200, LWA_ALPHA); // Установка прозрачности (здесь 200 - значение прозрачности)

    //MARGINS margins = { -1 }; // Расширяем клиентскую область для размытия
    //DwmExtendFrameIntoClientArea(hwnd, &margins); // Включаем размытие заднего фона

    while (1)
    {
        if (GetAsyncKeyState(VK_END) & 0X8000)
        {
            exit(0);

        }

        //int pidggat = driver->query_pid_by_process_name(sk_crypt(L"FortniteClient-Win64-Shipping.exe").decrypt());
        //if (!pidggat)
        //{
        //    exit(0);
        //}

        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }

        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            g_ResizeWidth = g_ResizeHeight = 0;
            CreateRenderTarget();
        }





        ////std::cout << "\n UWorld: " << UWorld;



        SegaUi_ImplDX11_NewFrame();
        SegaUi_ImplWin32_NewFrame();
        SegaUi::NewFrame();
        //static bool bNeedDoFakeClean = false;
        //while (1)
        //{
        //    //if (!bNeedDoFakeClean)
        //    {
        //        SegaUi::Render();
        //        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        //        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        //        SegaUi_ImplDX11_RenderDrawData(SegaUi::GetDrawData());
        //        g_pSwapChain->Present(CurrentConfig.bVSync, 0);


        //        //msg;
        //        //while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        //        //{
        //        //    ::TranslateMessage(&msg);
        //        //    ::DispatchMessage(&msg);
        //        //}

        //        //if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        //        //{
        //        //    CleanupRenderTarget();
        //        //    g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
        //        //    g_ResizeWidth = g_ResizeHeight = 0;
        //        //    CreateRenderTarget();
        //        //}

        //        //SegaUi_ImplDX11_NewFrame();
        //        //SegaUi_ImplWin32_NewFrame();
        //        //SegaUi::NewFrame();

        //        //SegaUi::Render();
        //        //g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        //        //g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        //        //SegaUi_ImplDX11_RenderDrawData(SegaUi::GetDrawData());
        //        //g_pSwapChain->Present(CurrentConfig.bVSync, 0);

        //        //bNeedDoFakeClean = true;
        //    }



        //}
        //if (bNeedDoFakeClean)
        //{
        //    bNeedDoFakeClean = false;

        //    continue;
        //}
        {
            SegaUiStyle* style = &SegaUi::GetStyle();
            style->Colors[SegaUiCol_Separator] = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);

            style->WindowPadding = ImVec2(0, 0);
            style->WindowBorderSize = 0;
            style->ItemSpacing = ImVec2(20, 20);
            style->ItemSpacing = ImVec2(10, 10);
            style->ScrollbarSize = 14.f;

            //style->ItemSpacing = ImVec2(4.0f, 4.0f); // Установка отступов между элементами
            //style->FramePadding = ImVec2(2.0f, 2.0f); // Установка отступов внутри рамки элемента

            POINT cursorPos;
            GetCursorPos(&cursorPos);
            ScreenToClient(hwnd, &cursorPos);



            io.MousePos.x = static_cast<float>(cursorPos.x);
            io.MousePos.y = static_cast<float>(cursorPos.y);
            io.MouseDown[0] = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
            io.MouseDown[1] = (GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0;
            io.MouseDown[2] = (GetAsyncKeyState(VK_MBUTTON) & 0x8000) != 0;
            io.MouseDown[3] = (GetAsyncKeyState(VK_XBUTTON1) & 0x8000) != 0;
            io.MouseDown[4] = (GetAsyncKeyState(VK_XBUTTON2) & 0x8000) != 0;

            if (CurrentConfig.FuserMode)
            {
                SegaUi::GetBackgroundDrawList()->AddRectFilled(ImVec2(0, 0), ImVec2(GameInfo.ViewRect.Width(), GameInfo.ViewRect.Height()), IM_COL32_BLACK, 0, 0);
            }

            {
                DefaultPawn* Target_Pawn = nullptr;
               
                float LastDistance = FLT_MAX;
                static uint8_t WeaponTypeByte = 0;

                static PseudoInterupt Cache;
                static PseudoInterupt Mini;
                static PseudoInterupt FilterSpam;
                static PseudoInterupt trig;
                static PseudoInterupt Checks;
                static PseudoInterupt VisibleUpdate;
                static std::string CurrentWeaponDisplayName;
                FVector CameraLocation;
                FRotator CameraRotation;
                GetViewInfo(GameInfo.ViewInfo, CameraLocation, CameraRotation, 3);

                FVector CameraLocation2;
                FRotator CameraRotation2;
                GetViewInfo(GameInfo.ViewInfo, CameraLocation2, CameraRotation2, 1);
                uintptr_t BackUpRoot = 0;
                static int Index = 0;

                if (Cache.is_interuption<milliseconds>(2000))
                {
                    GameInfo.UWorld = driver->read<uintptr_t>(driver->base + Offsets::UWorld);
                    //GameInfo.UWorld = driver->read<uintptr_t>(driver->base + UWorlds[Index]);



                    GameInfo.UEngine = driver->read<uintptr_t>(GameInfo.UGameInstance + 0x20);

                    GameInfo.MatrixPointer = driver->read<uintptr_t>(GameInfo.UWorld + Offsets::MatrixPointer);
                    GameInfo.UGameInstance = driver->read<uintptr_t>(GameInfo.UWorld + Offsets::UGameInstance);
                    GameInfo.AGameState = driver->read<uintptr_t>(GameInfo.UWorld + Offsets::AGameState);
                    GameInfo.ULocalPlayer = driver->read<uintptr_t>(driver->read<uintptr_t>(GameInfo.UGameInstance + Offsets::TLocalPlayers));
                    GameInfo.AController = driver->read<uintptr_t>(GameInfo.ULocalPlayer + Offsets::AController);

                    //GameInfo.PlayerCameraManager = driver->read<uintptr_t>(GameInfo.AController + 0x348);
                    //GameInfo.CameraRoot = driver->read<uintptr_t>(GameInfo.PlayerCameraManager + Offsets::URoot);
                    //GameInfo.URoot = driver->read<uintptr_t>(GameInfo.LocalPawn + Offsets::URoot);

                    //GameInfo.ULocalMovement = driver->read<uintptr_t>(GameInfo.LocalPawn + Offsets::UMovement);
                    //float fay = driver->read<float>(GameInfo.AController + 0x68);

                    GameInfo.LocalPawn = driver->read<uintptr_t>(GameInfo.AController + Offsets::Pawn);
                    GameInfo.URoot = driver->read<uintptr_t>(GameInfo.LocalPawn + 0x198);
                    GameInfo.APlayerState = driver->read<uintptr_t>(GameInfo.AController + Offsets::APlayerStateFromController);
                    GameInfo.MyTeamId = driver->read<int>(GameInfo.APlayerState + Offsets::TeamIndex);
                    DWORD64TArray PlayerArray = driver->read<DWORD64TArray>(GameInfo.AGameState + Offsets::TPlayerArray);
                    GameInfo.TPlayerArray = PlayerArray.Array;
                    GameInfo.PlayerArrayCount = PlayerArray.ArrayCount;
                    GameInfo.PlayerArrayCount = std::clamp(GameInfo.PlayerArrayCount, 0, 200);
                    
                    std::vector<uintptr_t> PlayerStateList(GameInfo.PlayerArrayCount);
                    if (TestAddress2(GameInfo.TPlayerArray))
                    {
                        driver->read_mem(GameInfo.TPlayerArray, PlayerStateList.data(), PlayerStateList.size() * sizeof(uintptr_t));
                    }

                    std::vector<DefaultPawn*> NewTempEntityList;


                    for (int i = 0; i < PlayerStateList.size(); i++)
                    {
                        uintptr_t playerState = PlayerStateList[i];
                        
                        DefaultPawn* CurrentPawn = GetPawnByPlayerState(playerState);
                        if (CurrentPawn)
                        {
                            NewTempEntityList.push_back(CurrentPawn);
                        }
                    }

                    for (int ii = 0; ii < EntityList.size(); ii++)
                    {
                        if (EntityList[ii])
                        {
                            auto it = std::find(NewTempEntityList.begin(), NewTempEntityList.end(), EntityList[ii]);
                            if (it != NewTempEntityList.end()) {
                            }
                            else {
                                delete EntityList[ii];
                            }
                        }
                    }

                    EntityList.clear();
                    EntityList = NewTempEntityList;


                    //std::unordered_map<uintptr_t, DefaultPawn*> EntityMap;

                    //// Заполняем ассоциативный массив
                    //for (int ii = 0; ii < EntityList.size(); ii++)
                    //{
                    //    if (EntityList[ii])
                    //    {
                    //        EntityMap[EntityList[ii]->APlayerState] = EntityList[ii];
                    //    }
                    //}

                    //std::vector<DefaultPawn*> NewTempEntityList;

                    //// Фильтруем по PlayerStateList
                    //for (int i = 0; i < PlayerStateList.size(); i++)
                    //{
                    //    uintptr_t playerState = PlayerStateList[i];
                    //    // Ищем сущность в ассоциативном массиве
                    //    auto it = EntityMap.find(playerState);
                    //    if (it != EntityMap.end())
                    //    {
                    //        // Если находим, добавляем в новый список
                    //        NewTempEntityList.push_back(it->second);
                    //    }
                    //}

                    //// Очищаем и удаляем объекты, которые не были найдены
                    //for (int ii = 0; ii < EntityList.size(); ii++)
                    //{
                    //    if (EntityList[ii])
                    //    {
                    //        if (EntityMap.find(EntityList[ii]->APlayerState) == EntityMap.end())
                    //        {
                    //            // Удаляем объект, так как его не нашли в новом списке
                    //            delete EntityList[ii];
                    //        }
                    //    }
                    //}

                    //// Очищаем и заменяем EntityList новым списком
                    //EntityList.clear();
                    //EntityList = NewTempEntityList;


                    //for (int count = 0; count < GameInfo.PlayerArrayCount; count++)
                    for(int count = 0; count < PlayerStateList.size(); count++)
                    {
                        int Kills = 0;
                        DefaultPawn* CurrentPawn = nullptr;


                        //uintptr_t PlayerState = driver->read<uintptr_t>(GameInfo.TPlayerArray + count * 0x8);
                        uintptr_t PlayerState = PlayerStateList[count];
                        for (int64_t i = 0; i < EntityList.size(); i++)
                        {
                            if (EntityList[i]->APlayerState == PlayerState)
                            {
                                CurrentPawn = EntityList[i];
                                break;
                            }
                        }

                        if (!CurrentPawn)
                        {
                            CurrentPawn = new DefaultPawn;
                            EntityList.push_back(CurrentPawn);

                        }
                        else
                        {
                            if(CurrentPawn->AActor == driver->read<uintptr_t>(PlayerState + Offsets::PawnPrivate))
                            continue;
                        }



                        CurrentPawn->AActor = driver->read<uintptr_t>(PlayerState + Offsets::PawnPrivate);

                        if (CurrentPawn->AActor == GameInfo.LocalPawn) { 
                            auto it = std::find(EntityList.begin(), EntityList.end(), CurrentPawn);
                            if (it != EntityList.end())
                            {
                                it = EntityList.erase(it);
                            }
                            delete CurrentPawn;
                            continue;
                        }
                        if (!CurrentPawn->AActor) {
                            auto it = std::find(EntityList.begin(), EntityList.end(), CurrentPawn);
                            if (it != EntityList.end())
                            {
                                it = EntityList.erase(it);
                            }
                            delete CurrentPawn;
                            continue;
                        }



                        //if (CurrentPawn)
                        //{
                        //    CurrentPawn->Weapon = CurrentItem;
                        //    CurrentPawn->Kills = Kills;
                        //    continue;
                        //}


                       // bool bIsDeadStatus = false;
                        bool bIsDeadStatus = bIsDead(CurrentPawn->AActor);

                        if (!GameInfo.LocalPawn)
                        {
                            bIsDeadStatus = false;
                        }
                        
                        if (bIsDeadStatus) { 
                            auto it = std::find(EntityList.begin(), EntityList.end(), CurrentPawn);
                            if (it != EntityList.end())
                            {
                                it = EntityList.erase(it);
                            }
                            delete CurrentPawn;
                            continue; 
                        
                        };

                        if (CurrentConfig.IgnoreKnockeds)
                        {
                            //if (bIsDbno(APawn)) { continue; };
                        }

  

                        int EnemyTeamIndex = driver->read<int>(PlayerState + Offsets::TeamIndex);
                        if (GameInfo.MyTeamId == EnemyTeamIndex && GameInfo.LocalPawn) { continue; }
                        uintptr_t UMesh = driver->read<uintptr_t>(CurrentPawn->AActor + Offsets::UMesh);
                        if (!UMesh) { 
                            auto it = std::find(EntityList.begin(), EntityList.end(), CurrentPawn);
                            if (it != EntityList.end())
                            {
                                it = EntityList.erase(it);
                            }
                            delete CurrentPawn;
                            continue;
                        }

                        uintptr_t MyBoneArray = driver->read<uintptr_t>(UMesh + Offsets::TBoneArray);
                        int BoneArraySize = driver->read<int>(UMesh + Offsets::TBoneArray + 0x8);
                        if (MyBoneArray == NULL)
                        {
                            MyBoneArray = driver->read<uintptr_t>(UMesh + Offsets::TBoneArray + 0x10);
                            BoneArraySize = driver->read<uintptr_t>(UMesh + Offsets::TBoneArray + 0x10 + 0x8);

                        }

                        if (BoneArraySize > 150 && BoneArraySize < 110) {
                            auto it = std::find(EntityList.begin(), EntityList.end(), CurrentPawn);
                            if (it != EntityList.end())
                            {
                                it = EntityList.erase(it);
                            }
                            delete CurrentPawn;
                            continue;
                        }


                        if (!MyBoneArray) { 
                            auto it = std::find(EntityList.begin(), EntityList.end(), CurrentPawn);
                            if (it != EntityList.end())
                            {
                                it = EntityList.erase(it);
                            }
                            delete CurrentPawn;
                            continue;
                        }

                        uintptr_t USceneComponent = driver->read<uintptr_t>(CurrentPawn->AActor + Offsets::URoot);
                        if (!USceneComponent) { 
                            auto it = std::find(EntityList.begin(), EntityList.end(), CurrentPawn);
                            if (it != EntityList.end())
                            {
                                it = EntityList.erase(it);
                            }
                            
                            delete CurrentPawn;
                            continue;
                        }



                        uintptr_t UMovement = driver->read<uintptr_t>(CurrentPawn->AActor + Offsets::UMovement);
                        if (!UMovement) {
                            auto it = std::find(EntityList.begin(), EntityList.end(), CurrentPawn);
                            if (it != EntityList.end())
                            {
                                it = EntityList.erase(it);
                            }

                            delete CurrentPawn;
                            continue;
                        }

                        for (int64_t ii = 0; ii < CurrentConfig.AimBones.size(); ii++)
                        {
                            FBone* CurrentBone = new FBone;
                            CurrentBone->Bone = CurrentConfig.AimBones[ii].Bone;
                            CurrentBone->Name = CurrentConfig.AimBones[ii].Name;
                            CurrentPawn->Bones.push_back(CurrentBone);
                        }


                        if (CurrentConfig.bNamesEsp)
                        {
                            //DWORD64TArray FString = driver->read<DWORD64TArray>(PlayerState + 0x338);
                            //CurrentPawn->Name = DecryptShit(FString, GameInfo.LocalPawn);


                            //if (FString.Array && FString.ArrayCount < 300 && FString.ArrayCount > 2) {
                            //    wchar_t* OutString = new wchar_t[FString.ArrayCount + 1];
                            //    DWORD bytesRead = 0;
                            //    driver->read_mem(FString.Array, OutString, FString.ArrayCount * sizeof(wchar_t));
                            //    CHAR PlayerPlatform_Char[0xFF] = { 0 };
                            //    wcstombs(PlayerPlatform_Char, OutString, sizeof(PlayerPlatform_Char));
                            //    CurrentPawn->Name = PlayerPlatform_Char;
                            //}

                            CurrentPawn->Name = PlayerName(PlayerState, GameInfo.LocalPawn);
                        }

                        DWORD64TArray FString = driver->read<DWORD64TArray>(PlayerState + 0x438);
                        if (FString.Array && FString.ArrayCount < 300 && FString.ArrayCount > 2) {
                            wchar_t* OutString = new wchar_t[FString.ArrayCount + 1];
                            DWORD bytesRead = 0;
                            driver->read_mem(FString.Array, OutString, FString.ArrayCount * sizeof(wchar_t));
                            CHAR PlayerPlatform_Char[0xFF] = { 0 };
                            wcstombs(PlayerPlatform_Char, OutString, sizeof(PlayerPlatform_Char));
                            CurrentPawn->Platform = PlayerPlatform_Char;
                        }

                        CurrentPawn->USceneComponent = USceneComponent;
                        CurrentPawn->APlayerState = PlayerState;
                        CurrentPawn->UMesh = UMesh;
                        CurrentPawn->BoneArray = MyBoneArray;
                        CurrentPawn->BoneArraySize = BoneArraySize;
                        CurrentPawn->TeamIndex = EnemyTeamIndex;
                        CurrentPawn->bIsDead = bIsDeadStatus;
                        CurrentPawn->Kills = Kills;
                        CurrentPawn->UMovement = UMovement;

                        //std::cout << "\n gay!";
                    }

                    //ItemsList.clear();

                    //if (CurrentConfig.bItemsEsp)
                    //{
                        //DWORD64TArray LevelsArray = driver->read<DWORD64TArray>(GameInfo.UWorld + 0x178);
                    //    LevelsArray.ArrayCount = std::clamp(LevelsArray.ArrayCount, 0, 200);
                    //}
                }



                //static ID3D11ShaderResourceView* ImportedImg = nullptr;
                //static D3DX11_IMAGE_LOAD_INFO info; ID3DX11ThreadPump* pump{ nullptr };
                //static int64_t ImageSize = 0;
                //static uintptr_t DefaultTexture;
                //if (!ImportedImg)
                //{

                //    DefaultTexture = driver->read(GameInfo.UEngine + 0x2a8); // 0x2a8(0x08)
                //    int32_t FirstResourceMemMip = driver->read(DefaultTexture + 0x130); // 0x130(0x04)
                //    FIntPoint ImportedSize = driver->read<FIntPoint>(DefaultTexture + 0x138);
                //    ImageSize = ImportedSize.X * ImportedSize.Y * 4;


                //    

                //    std::vector<uint8_t> ImageRawData(ImageSize);
                //    driver->read_mem(FirstResourceMemMip, ImageRawData.data(), ImageRawData.size());

                //    std::ofstream file("C:\\test.png", std::ios::out | std::ios::binary);
                //    file.write(reinterpret_cast<const char*>(ImageRawData.data()), ImageRawData.size());


                //    D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, ImageRawData.data(), ImageRawData.size(), &info, pump, &ImportedImg, 0);

                //    std::cout << "\n DefaultTexture: " << DefaultTexture;
                //    std::cout << "\n ImageSize: " << ImageSize;

                //}

                //if (ImportedImg)
                //{
                //    SegaUi::GetBackgroundDrawList()->AddImage(ImportedImg, ImVec2(10,10), ImVec2(100, 100), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255));
                //}


                if (CurrentConfig.WeaponConfigs[CurrentConfig.CurrentWeaponIndex].TriggerBot)
                {

                    if (trig.is_interuption<milliseconds>(15))
                    {
                        if (CurrentConfig.TriggerBotMethod == 0)
                        {

                        }

                        if (CurrentConfig.TriggerBotMethod == 0)
                        {
                            bool KeyDown = false;
#ifdef DMA
                            KeyDown = driver->GetKeyboard()->IsKeyDown(CurrentConfig.TriggerBotKey);
#endif // DMA

#ifdef Driver
                            KeyDown = GetAsyncKeyState(CurrentConfig.TriggerBotKey) & 0x8000;
#endif // Driver


                            
                            //if (GetAsyncKeyState(CurrentConfig.WeaponConfigs[CurrentConfig.CurrentWeaponTab].TriggerBotKey) & 0x8000)
                            if (KeyDown)

                            {
                                uintptr_t TargetedFortPawn = driver->read<uintptr_t>(GameInfo.AController + Offsets::TargetedFortPawn);
                                for (int i = 0; i < EntityList.size(); i++)
                                {
                                    if (TargetedFortPawn == EntityList[i]->AActor)
                                    {
                                        if (GameInfo.AFortWeapon)
                                        {
                                            if (CurrentConfig.KmBoxModelIndex == 0) {
                                                KmBoxMgr.Mouse.Left(true);
                                                KmBoxMgr.Mouse.Left(false);
                                            }
                                            else if (CurrentConfig.KmBoxModelIndex == 1) {
                                                kmBoxBMgr.km_click();
                                                //Sleep(5);
                                            }
                                            //keybd_event(VK_OEM_MINUS, 0, 0, 0);
                                            //keybd_event(VK_OEM_MINUS, 0, KEYEVENTF_KEYUP, 0);
                                        }
                                    }
                                }
                            }
                        }

                        if (CurrentConfig.TriggerBotMethod == 2)
                        {
                            uintptr_t TargetedFortPawn = driver->read<uintptr_t>(GameInfo.AController + Offsets::TargetedFortPawn);
                            for (int i = 0; i < EntityList.size(); i++)
                            {
                                if (TargetedFortPawn == EntityList[i]->AActor)
                                {
                                    if (GameInfo.AFortWeapon)
                                    {
                                        if (CurrentConfig.KmBoxModelIndex == 0) {
                                            KmBoxMgr.Mouse.Left(true);
                                            KmBoxMgr.Mouse.Left(false);
                                        }
                                        else if (CurrentConfig.KmBoxModelIndex == 1) {
                                            kmBoxBMgr.km_click();
                                           //Sleep(5);
                                        }

                                        //keybd_event(VK_OEM_MINUS, 0, 0, 0);
                                        //keybd_event(VK_OEM_MINUS, 0, KEYEVENTF_KEYUP, 0);
                                    }
                                }
                            }
                        }


                    }
                }

                if (VisibleUpdate.is_interuption<milliseconds>(10))
                {
                    HANDLE handle = driver->CreateScatterHandle();

                    for (int i = 0; i < EntityList.size(); i++)
                    {
                        if (EntityList[i])
                        {
                            //if (FilterSpam.is_interuption<milliseconds>(10))
                            //{

                            //}
                            driver->AddScatterReadRequest(handle, EntityList[i]->UMesh + Offsets::ComponentToWorld, &EntityList[i]->FZeroLocation, sizeof(FTransform));

                            driver->AddScatterReadRequest(handle, EntityList[i]->UMesh + Offsets::RenderTimes, &EntityList[i]->RenderData, sizeof(RenderTimes));
                        }
                    }
                    driver->ExecuteReadScatter(handle);
                    driver->CloseScatterHandle(handle);
                }

                if (Checks.is_interuption<milliseconds>(250))
                {
                    for (int i = 0; i < EntityList.size(); i++)
                    {
                        if (EntityList[i])
                        {
                            HANDLE handle = driver->CreateScatterHandle();

                            if (CurrentConfig.bKillsEsp)
                            {
                                if (GameInfo.LocalPawn)
                                {
                                    driver->AddScatterReadRequest(handle, EntityList[i]->APlayerState + Offsets::KillScore, &EntityList[i]->Kills, sizeof(int));


                                    //EntityList[i]->Kills = driver->read<int>(EntityList[i]->APlayerState + Offsets::KillScore);
                                }
                            }
                            PawnDeadStatusStruct NewCurrentStruct;

                            driver->AddScatterReadRequest(handle, EntityList[i]->AActor + 0x758, &NewCurrentStruct, sizeof(PawnDeadStatusStruct));
                            driver->AddScatterReadRequest(handle, EntityList[i]->APlayerState + Offsets::TeamIndex, &EntityList[i]->TeamIndex, sizeof(int));

                            driver->ExecuteReadScatter(handle);
                            driver->CloseScatterHandle(handle);

                            EntityList[i]->bIsDead = NewCurrentStruct.bIsDying;

                           // EntityList[i]->TeamIndex = driver->read<int>(EntityList[i]->APlayerState + Offsets::TeamIndex);



                            if (CurrentConfig.bEnemyWeaponEsp)
                            {
                                EntityList[i]->AFortWeapon = driver->read<uintptr_t>(EntityList[i]->AActor + Offsets::AFortWeapon);
                                if (EntityList[i]->AFortWeapon && EntityList[i]->AFortWeapon != EntityList[i]->PreviosWeapon)
                                {
                                    EntityList[i]->PreviosWeapon = EntityList[i]->AFortWeapon;
                                    uint64_t WeaponData = driver->read<uint64_t>(EntityList[i]->AFortWeapon + Offsets::WeaponData);

                                    if (WeaponData)
                                    {
                                        WeaponDataDefinition* CurrentItem = 0;

                                        for (int64_t i = 0; i < WeaponsDefinition.size(); i++)
                                        {
                                            if (WeaponsDefinition[i]->WeaponData == WeaponData)
                                            {
                                                CurrentItem = WeaponsDefinition[i];
                                                break;
                                            }
                                        }
                                        if (!CurrentItem)
                                        {
                                            uint64_t DisplayName = driver->read<uint64_t>(WeaponData + 0x40);
                                            DWORD64TArray FString = driver->read<DWORD64TArray>(DisplayName + 0x28);
                                            if (FString.Array && FString.ArrayCount < 300 && FString.ArrayCount > 2) {

                                                wchar_t* OutString = new wchar_t[FString.ArrayCount + 1];
                                                DWORD bytesRead = 0;
                                                driver->read_mem(FString.Array, OutString, FString.ArrayCount * sizeof(wchar_t));
                                                EFortRarity Rarity = driver->read<EFortRarity>(WeaponData + 0xeb);
                                                CHAR PlayerWeapon_Char[0xFF] = { 0 };
                                                wcstombs(PlayerWeapon_Char, OutString, sizeof(PlayerWeapon_Char));
                                                std::string PlayerWeapon = PlayerWeapon_Char;
                                                WeaponDataDefinition* NewWeaponData = new WeaponDataDefinition;
                                                NewWeaponData->WeaponData = WeaponData;
                                                NewWeaponData->DisplayName = DisplayName;
                                                NewWeaponData->FString = FString;
                                                NewWeaponData->Name = PlayerWeapon;
                                                NewWeaponData->PawnWeapon = Rarity;
                                                CurrentItem = NewWeaponData;
                                                WeaponsDefinition.push_back(NewWeaponData);
                                            }
                                        }

                                        EntityList[i]->Weapon = CurrentItem;
                                    }

                                }

                            }



                            //if (bIsDead(EntityList[i]->AActor) || (GameInfo.MyTeamId == EntityList[i]->TeamIndex && GameInfo.LocalPawn))
                            if (EntityList[i]->bIsDead || (GameInfo.MyTeamId == EntityList[i]->TeamIndex && GameInfo.LocalPawn))
                            {
                                delete EntityList[i];
                                EntityList.erase(EntityList.begin() + i);
                            }


                        }
                        else
                        {
                            EntityList.erase(EntityList.begin() + i);

                        }

                    }
                }

                FVector RotationGay;

                if (Mini.is_interuption<milliseconds>(2))
                {

                    //uintptr_t DisplayName = 0;
                    HANDLE handle = driver->CreateScatterHandle();

                    driver->AddScatterReadRequest(handle, GameInfo.MatrixPointer, &GameInfo.ViewInfo, sizeof(FWorldCachedViewInfo));
                    driver->AddScatterReadRequest(handle, GameInfo.LocalPawn + Offsets::AFortWeapon, &GameInfo.AFortWeapon, sizeof(uintptr_t));
                    driver->ExecuteReadScatter(handle);
                    driver->CloseScatterHandle(handle);

                    //driver->AddScatterReadRequest(handle, GameInfo.ULocalPlayer + 0x4CC + Offsets::ComponentToWorld, &GameInfo.Fov, sizeof(float));
                    //driver->read_mem(handle, GameInfo.MatrixPointer, &GameInfo.ViewInfo, sizeof(FWorldCachedViewInfo));
                    //driver->read_mem(handle, GameInfo.ULocalPlayer + 0x4CC + Offsets::ComponentToWorld, &GameInfo.Fov, sizeof(float));
                    //GameInfo.AFortWeapon = driver->read<uintptr_t>(GameInfo.LocalPawn + Offsets::AFortWeapon);
                    


                    uintptr_t TempWeaponData = driver->read<uintptr_t>(GameInfo.AFortWeapon + Offsets::WeaponData);
                    //GameInfo.WeaponData = TempWeaponData;

                    //std::vector<double> ArrayOfAngles(5);
                    //driver->read_mem(GameInfo.MatrixPointer, ArrayOfAngles.data(), 5);
                    //    double w1 = ArrayOfAngles[0x0 / 8];
                    //    double w3 = ArrayOfAngles[0x20 / 8];
                    //    double w2 = driver->read<double>(GameInfo.MatrixPointer + 0x1D0);
                    //    RotationGay.X = asin(w2) * (180.0 / M_PI);
                    //    double rotationcachey = (atan2(w1 * -1, w3) * (180.0 / M_PI)) * -1;
                    //    rotationcachey *= -1;
                    //    RotationGay.Y = rotationcachey;
                    
                    //driver->read_mem(handle, GameInfo.AFortWeapon + Offsets::WeaponData, &GameInfo.WeaponData, sizeof(uint64_t));
                    //driver->read_mem(handle, GameInfo.WeaponData + 0x48, &DisplayName, sizeof(uint64_t));

                    //driver->ExecuteReadScatter(handle);
                    //driver->CloseScatterHandle(handle);

                    //GameInfo.ViewInfo = driver->read<FWorldCachedViewInfo>(GameInfo.MatrixPointer);
                    //GameInfo.Fov = driver->read<float>(GameInfo.ULocalPlayer + 0x4CC);

                    //GameInfo.AFortWeapon = driver->read<uintptr_t>(GameInfo.LocalPawn + Offsets::AFortWeapon);
                    //uintptr_t TempWeaponData = driver->read<uintptr_t>(GameInfo.AFortWeapon + Offsets::WeaponData);
                    //GameInfo.ItemType = driver->read<uint8_t>(GameInfo.WeaponData + 0x138);
                    //GameInfo.IsRanged = GameInfo.ItemType == 23;
                    if (GameInfo.WeaponData != TempWeaponData)
                    {
                        GameInfo.WeaponData = TempWeaponData;

                        GameInfo.DisplayName = driver->read<uintptr_t>(GameInfo.WeaponData + 0x40);
                        GameInfo.ItemType = driver->read<uint8_t>(GameInfo.WeaponData + 0xe8);
                        GameInfo.IsRanged = GameInfo.ItemType == 23;


                        if (GameInfo.DisplayName)
                        {
                            DWORD64TArray FString = driver->read<DWORD64TArray>(GameInfo.DisplayName + 0x28);
                            if (FString.Array && FString.ArrayCount < 300 && FString.ArrayCount > 2) 

                            {
                                wchar_t* OutString = new wchar_t[FString.ArrayCount + 1];
                                driver->read_mem(FString.Array, OutString, FString.ArrayCount * sizeof(wchar_t));
                                CHAR Weapon_Char[0xFF] = { 0 };
                                wcstombs(Weapon_Char, OutString, sizeof(Weapon_Char));
                                CurrentWeaponDisplayName = Weapon_Char;

                                //std::string CurrentWeaponDisplayName = "";
                                if (isSubstring(CurrentWeaponDisplayName, "sniper") || isSubstring(CurrentWeaponDisplayName, "dmr"))
                                {
                                    CurrentConfig.CurrentWeaponIndex = 3;
                                }
                                else if (isSubstring(CurrentWeaponDisplayName, "rifle") || isSubstring(CurrentWeaponDisplayName, "assault") || isSubstring(CurrentWeaponDisplayName, "machine"))
                                {
                                    CurrentConfig.CurrentWeaponIndex = 0;
                                }
                                else if (isSubstring(CurrentWeaponDisplayName, "smg") || isSubstring(CurrentWeaponDisplayName, "pistol") || isSubstring(CurrentWeaponDisplayName, "submachine"))
                                {
                                    CurrentConfig.CurrentWeaponIndex = 1;
                                }
                                else if (isSubstring(CurrentWeaponDisplayName, "shotgun"))
                                {
                                    CurrentConfig.CurrentWeaponIndex = 2;
                                }
                                else
                                {
                                    CurrentConfig.CurrentWeaponIndex = 4;
                                }



                                delete OutString;
                            }
                        }
                    }
                }



                std::vector<PawnDistance> distances;

                for (int i = 0; i < EntityList.size(); i++) {
                    if (EntityList[i]->BoneArraySize < 150 && EntityList[i]->BoneArraySize > 100)
                    {
                        EntityList[i]->FTBones.resize(EntityList[i]->BoneArraySize);

                        //EntityList[i]->Bones.resize(EntityList[i]->BoneArraySize);
                        //HANDLE handle = driver->CreateScatterHandle();

                        //mem->AddScatterReadRequest(handle, EntityList[i]->BoneArray, EntityList[i]->EntityBones.data(), EntityList[i]->EntityBones.size() * sizeof(FTransform));
                        HANDLE handle = driver->CreateScatterHandle();


                        //PawnDBNOStatusStruct NewCurrentStruct;
                        //driver->AddScatterReadRequest(handle, EntityList[i]->UMesh + 0x982, &NewCurrentStruct, sizeof(PawnDBNOStatusStruct));

                        //int HeadBoneId = 109;
                        //std::vector<FTransform> Bones(EntityList[i]->BoneArraySize);

                        //driver->read_mem(EntityList[i]->BoneArray, Bones.data(), Bones.size() * sizeof(FTransform));

                        //if (HeadBoneId < Bones.size() && HeadBoneId >= 0)
                        //{
                        //    FTransform FTHeadBone = Bones[HeadBoneId];
                        //}


                        driver->AddScatterReadRequest(handle, EntityList[i]->BoneArray, EntityList[i]->FTBones.data(), EntityList[i]->BoneArraySize * sizeof(FTransform));
                        driver->ExecuteReadScatter(handle);
                        driver->CloseScatterHandle(handle);
                        //driver->read_mem(handle, EntityList[i]->UMesh + Offsets::RenderTimes, &EntityList[i]->RenderData, sizeof(RenderTimes));
                        //driver->read_mem(handle, EntityList[i]->UMesh + Offsets::ComponentToWorld, &EntityList[i]->FZeroLocation, sizeof(FTransform));
                        //driver->read_mem(handle, EntityList[i]->AActor + 0x982, &NewCurrentStruct, sizeof(PawnDBNOStatusStruct));
                        //driver->read_mem(EntityList[i]->BoneArray, EntityList[i]->FTBones.data(), EntityList[i]->BoneArraySize * sizeof(FTransform));
                        //for (int64_t ii = 0; ii < EntityList[i]->Bones.size(); ii++)
                        //{

                        //    //EntityList[i]->Bones[ii]->Transform = driver->read<FTransform>(EntityList[i]->BoneArray + (EntityList[i]->Bones[ii]->Bone * 0x60));
                        //    driver->AddScatterReadRequest(handle, EntityList[i]->BoneArray + (EntityList[i]->Bones[ii]->Bone * 0x60), &EntityList[i]->Bones[ii]->Transform, sizeof(FTransform));
                        //}


                        for (int64_t ii = 0; ii < EntityList[i]->Bones.size(); ii++)
                        {
                            if (EntityList[i]->Bones[ii])
                            {
                                if (EntityList[i]->Bones[ii]->Bone < EntityList[i]->FTBones.size())
                                {
                                    EntityList[i]->Bones[ii]->Transform = EntityList[i]->FTBones[EntityList[i]->Bones[ii]->Bone];

                                }

                            }
                        }


                        //driver->ExecuteReadScatter(handle);
                        //driver->CloseScatterHandle(handle);
                        //EntityList[i]->bIsDBNO = NewCurrentStruct.bIsDBNO;
                    }
                        EntityList[i]->Distance = CalculateDistance(CameraLocation, EntityList[i]->HeadLocation);
                    
                        distances.emplace_back(EntityList[i], EntityList[i]->Distance);
                    
                }

                std::sort(distances.begin(), distances.end(), PawnDistance::CompareByDistance);
                for (int i = 0; i < distances.size(); i++) {
                    EntityList[i] = distances[i].pawn;
                }

                for (int i = 0; i < EntityList.size(); i++)
                {

                    //std::cout << "\n EntityList[i]->FZeroLocation.Rotation.X: " << EntityList[i]->FZeroLocation.Rotation.X;
                    //std::cout << "\n EntityList[i]->FZeroLocation.Rotation.Y: " << EntityList[i]->FZeroLocation.Rotation.Y; 
                    //std::cout << "\n EntityList[i]->FZeroLocation.Rotation.Z: " << EntityList[i]->FZeroLocation.Rotation.Z;
                    //std::cout << "\n EntityList[i]->FZeroLocation.Rotation.W: " << EntityList[i]->FZeroLocation.Rotation.W;

                    //std::cout << "\n EntityList[i]->FZeroLocation.Translation.X: " << EntityList[i]->FZeroLocation.Translation.X;
                    //std::cout << "\n EntityList[i]->FZeroLocation.Translation.Y: " << EntityList[i]->FZeroLocation.Translation.Y;
                    //std::cout << "\n EntityList[i]->FZeroLocation.Translation.Z: " << EntityList[i]->FZeroLocation.Translation.Z;

                    //if (EntityList[i]->Bones.size() < 120)
                    //{
                    //    continue;
                    //}
                    FVector2D head_screen;


                    //std::cout << "\n mesh: " << EntityList[i]->UMesh;




                    //RenderData = driver->read<RenderTimes>(EntityList[i]->UMesh + Offsets::RenderTimes);

                    //EntityList[i]->FZeroLocation = driver->read<FTransform>(EntityList[i]->UMesh + Offsets::ComponentToWorld);
                    //EntityList[i]->FHeadLocation = driver->read<FTransform>(EntityList[i]->BoneArray + (110 * 0x60));

                    //EntityList[i]->FZeroLocation = driver->read<FTransform>(EntityList[i]->UMesh + Offsets::ComponentToWorld);
                    EntityList[i]->FHeadLocation = EntityList[i]->GetBoneByName("Head")->Transform;

                    EntityList[i]->bIsVisible = EntityList[i]->RenderData.LastRenderTimeOnScreen + 0.06f >= EntityList[i]->RenderData.LastSubmitTime;
                    EntityList[i]->ZeroLocation = ChildToWorld({}, EntityList[i]->FZeroLocation);
                    EntityList[i]->HeadLocation = ChildToWorld(EntityList[i]->FHeadLocation, EntityList[i]->FZeroLocation);

                    ProjectWorldToScreen(EntityList[i]->HeadLocation, GameInfo.ViewRect, GameInfo.ViewInfo.ViewProjectionMatrix, EntityList[i]->HeadLocationW2S);
                    ProjectWorldToScreen(EntityList[i]->ZeroLocation, GameInfo.ViewRect, GameInfo.ViewInfo.ViewProjectionMatrix, EntityList[i]->ZeroLocationW2S);
                    ProjectWorldToScreen(FVector(EntityList[i]->HeadLocation.X, EntityList[i]->HeadLocation.Y, EntityList[i]->HeadLocation.Z + 15), GameInfo.ViewRect, GameInfo.ViewInfo.ViewProjectionMatrix, head_screen);

                    double Size = 0;
                    int OffsetY = 0;
                    double DistanceMeters = EntityList[i]->Distance / 100;

                    Size = 100 / DistanceMeters;

                    if (!CurrentConfig.bRealisticSize)
                    {
                        if (Size < 14)
                        {
                            Size = 14;
                        }
                    }

                    //for (int ii = 0; ii < 120; ii++)
                    //{
                    //    FTransform FTCurrentBone = driver->read<FTransform>(EntityList[i]->BoneArray + (ii * 0x60));
                    //    FVector CurrentBone = ChildToWorld(FTCurrentBone, EntityList[i]->FZeroLocation);

                    //    FVector2D CurrentBoneW2S;
                    //    ProjectWorldToScreen(CurrentBone, GameInfo.ViewRect, GameInfo.ViewInfo.ViewProjectionMatrix, CurrentBoneW2S);

                    //    SegaUi::GetBackgroundDrawList()->AddRectFilled(ImVec2(CurrentBoneW2S.X - 2, CurrentBoneW2S.Y - 2), ImVec2(CurrentBoneW2S.X + 20, CurrentBoneW2S.Y + 20), IM_COL32(255, 255, 255, 255), 2.f, 0);
                    //    SegaUi::GetBackgroundDrawList()->AddText(io.Fonts->Fonts[0], 15, ImVec2(CurrentBoneW2S.X, CurrentBoneW2S.Y), IM_COL32_BLACK, std::to_string(ii).c_str(), 0, 0, 0);
                    //}

                    if (CurrentConfig.bLinesEsp)
                    {
                        ImVec2 PositionLines = ImVec2(GameInfo.ViewRect.Width() / 2, 0);
                        if (CurrentConfig.LinesPosition == 0)
                        {
                            PositionLines = ImVec2(GameInfo.ViewRect.Width() / 2, 0);
                        }

                        if (CurrentConfig.LinesPosition == 1)
                        {
                            PositionLines = ImVec2(GameInfo.ViewRect.Width() / 2, GameInfo.ViewRect.Height() / 2);
                        }

                        if (CurrentConfig.LinesPosition == 2)
                        {
                            PositionLines = ImVec2(GameInfo.ViewRect.Width() / 2, GameInfo.ViewRect.Height());
                        }
                        if (EntityList[i]->bIsVisible)
                        {
                            SegaUi::GetBackgroundDrawList()->AddLine(PositionLines, ImVec2(EntityList[i]->HeadLocationW2S.X, EntityList[i]->HeadLocationW2S.Y), ImColor(SegaUi::GetColorU32(CurrentConfig.LinesEspColorVisible)));
                        }
                        else
                        {
                            SegaUi::GetBackgroundDrawList()->AddLine(PositionLines, ImVec2(EntityList[i]->HeadLocationW2S.X, EntityList[i]->HeadLocationW2S.Y), ImColor(SegaUi::GetColorU32(CurrentConfig.GlobalNotVisible)));
                        }
                    }



                    if (EntityList[i]->HeadLocationW2S.X < -700) { continue; }
                    if (EntityList[i]->HeadLocationW2S.X > GameInfo.ViewRect.Width() + 700) { continue; }


                    auto entity_height = fabs(head_screen.Y - EntityList[i]->ZeroLocationW2S.Y);
                    auto entity_width = entity_height * 0.4;
                    auto box_min_size = ImVec2(head_screen.X - entity_width / 2.0f, head_screen.Y);
                    auto box_max_size = ImVec2(head_screen.X + entity_width / 2.0f, EntityList[i]->ZeroLocationW2S.Y);


                    //if (EntityList[i]->ZeroLocationW2S.Y < -20) { continue; }
                    //if (EntityList[i]->ZeroLocationW2S.Y > GameInfo.ViewRect.Height() + 20) { continue; }

                    if (CurrentConfig.bSkeletonEsp)
                    {
                        //FTransform FTNeck = driver->read<FTransform>(EntityList[i]->BoneArray + (CurrentConfig.GetBoneByName("Neck")->Bone * 0x60));
                        FTransform FTNeck = EntityList[i]->GetBoneByName("Neck")->Transform;
                        FTransform FTLeftShoulder = EntityList[i]->GetBoneByName("Left Shoulder")->Transform;
                        FTransform FTRightShoulder = EntityList[i]->GetBoneByName("Right Shoulder")->Transform;
                        FTransform FTLeftElbow = EntityList[i]->GetBoneByName("Left Elbow")->Transform;
                        FTransform FTRightElbow = EntityList[i]->GetBoneByName("Right Elbow")->Transform;
                        FTransform FTLeftHand = EntityList[i]->GetBoneByName("Left Hand")->Transform;
                        FTransform FTRightHand = EntityList[i]->GetBoneByName("Right Hand")->Transform;
                        FTransform FTLeftKnee = EntityList[i]->GetBoneByName("Left Knee")->Transform;
                        FTransform FTRightKnee = EntityList[i]->GetBoneByName("Right Knee")->Transform;
                        FTransform FTLeftFoot = EntityList[i]->GetBoneByName("Left Foot")->Transform;
                        FTransform FTRightFoot = EntityList[i]->GetBoneByName("Right Foot")->Transform;
                        FTransform FTPenis = EntityList[i]->GetBoneByName("Penis")->Transform;


                        //FTransform FTLeftShoulder = EntityList[i]->Bones[CurrentConfig.GetBoneByName("Left Shoulder")->Bone]->Transform;
                        //FTransform FTRightShoulder = EntityList[i]->Bones[CurrentConfig.GetBoneByName("Right Shoulder")->Bone]->Transform;

                        //FTransform FTLeftElbow = EntityList[i]->Bones[CurrentConfig.GetBoneByName("Left Elbow")->Bone]->Transform;
                        //FTransform FTRightElbow = EntityList[i]->Bones[CurrentConfig.GetBoneByName("Right Elbow")->Bone]->Transform;

                        //FTransform FTLeftHand = EntityList[i]->Bones[CurrentConfig.GetBoneByName("Left Hand")->Bone]->Transform;
                        //FTransform FTRightHand = EntityList[i]->Bones[CurrentConfig.GetBoneByName("Right Hand")->Bone]->Transform;

                        //FTransform FTLeftKnee = EntityList[i]->Bones[CurrentConfig.GetBoneByName("Left Knee")->Bone]->Transform;
                        //FTransform FTRightKnee = EntityList[i]->Bones[CurrentConfig.GetBoneByName("Right Knee")->Bone]->Transform;

                        //FTransform FTLeftFoot = EntityList[i]->Bones[CurrentConfig.GetBoneByName("Left Foot")->Bone]->Transform;
                        //FTransform FTRightFoot = EntityList[i]->Bones[CurrentConfig.GetBoneByName("Right Foot")->Bone]->Transform;

                        //FTransform FTPenis = EntityList[i]->Bones[CurrentConfig.GetBoneByName("Penis")->Bone]->Transform;

                        FVector2D Neck = FTToW2S(FTNeck, EntityList[i]->FZeroLocation);

                        FVector2D Penis = FTToW2S(FTPenis, EntityList[i]->FZeroLocation);

                        FVector2D LeftShoulder = FTToW2S(FTLeftShoulder, EntityList[i]->FZeroLocation);
                        FVector2D RightShoulder = FTToW2S(FTRightShoulder, EntityList[i]->FZeroLocation);

                        FVector2D LeftElbow = FTToW2S(FTLeftElbow, EntityList[i]->FZeroLocation);
                        FVector2D RightElbow = FTToW2S(FTRightElbow, EntityList[i]->FZeroLocation);

                        FVector2D LeftHand = FTToW2S(FTLeftHand, EntityList[i]->FZeroLocation);
                        FVector2D RightHand = FTToW2S(FTRightHand, EntityList[i]->FZeroLocation);

                        FVector2D LeftKnee = FTToW2S(FTLeftKnee, EntityList[i]->FZeroLocation);
                        FVector2D RightKnee = FTToW2S(FTRightKnee, EntityList[i]->FZeroLocation);

                        FVector2D LeftFoot = FTToW2S(FTLeftFoot, EntityList[i]->FZeroLocation);
                        FVector2D RightFoot = FTToW2S(FTRightFoot, EntityList[i]->FZeroLocation);


                        SegaUi::GetBackgroundDrawList()->AddBezierCubic(ImVec2(EntityList[i]->HeadLocationW2S.X, EntityList[i]->HeadLocationW2S.Y), ImVec2(Neck.X, Neck.Y), ImVec2(Neck.X, Neck.Y), ImVec2(Penis.X, Penis.Y), IM_COL32_BLACK, 3, 10);
                        SegaUi::GetBackgroundDrawList()->AddBezierCubic(ImVec2(Penis.X, Penis.Y), ImVec2(LeftKnee.X, LeftKnee.Y), ImVec2(LeftKnee.X, LeftKnee.Y), ImVec2(LeftFoot.X, LeftFoot.Y), IM_COL32_BLACK, 3, 10);
                        SegaUi::GetBackgroundDrawList()->AddBezierCubic(ImVec2(Penis.X, Penis.Y), ImVec2(RightKnee.X, RightKnee.Y), ImVec2(RightKnee.X, RightKnee.Y), ImVec2(RightFoot.X, RightFoot.Y), IM_COL32_BLACK, 3, 10);
                        SegaUi::GetBackgroundDrawList()->AddBezierCubic(ImVec2(Neck.X, Neck.Y), ImVec2(LeftShoulder.X, LeftShoulder.Y), ImVec2(LeftShoulder.X, LeftShoulder.Y), ImVec2(LeftShoulder.X, LeftShoulder.Y), IM_COL32_BLACK, 3, 10);
                        SegaUi::GetBackgroundDrawList()->AddBezierCubic(ImVec2(Neck.X, Neck.Y), ImVec2(RightShoulder.X, RightShoulder.Y), ImVec2(RightShoulder.X, RightShoulder.Y), ImVec2(RightShoulder.X, RightShoulder.Y), IM_COL32_BLACK, 3, 10);
                        SegaUi::GetBackgroundDrawList()->AddBezierCubic(ImVec2(LeftShoulder.X, LeftShoulder.Y), ImVec2(LeftElbow.X, LeftElbow.Y), ImVec2(LeftElbow.X, LeftElbow.Y), ImVec2(LeftHand.X, LeftHand.Y), IM_COL32_BLACK, 3, 10);
                        SegaUi::GetBackgroundDrawList()->AddBezierCubic(ImVec2(RightShoulder.X, RightShoulder.Y), ImVec2(RightElbow.X, RightElbow.Y), ImVec2(RightElbow.X, RightElbow.Y), ImVec2(RightHand.X, RightHand.Y), IM_COL32_BLACK, 3, 10);


                        SegaUi::GetBackgroundDrawList()->AddBezierCubic(ImVec2(EntityList[i]->HeadLocationW2S.X, EntityList[i]->HeadLocationW2S.Y), ImVec2(Neck.X, Neck.Y), ImVec2(Neck.X, Neck.Y), ImVec2(Penis.X, Penis.Y), ImColor(SegaUi::GetColorU32(CurrentConfig.SkeletonEspColorVisible)), 1, 10);
                        SegaUi::GetBackgroundDrawList()->AddBezierCubic(ImVec2(Penis.X, Penis.Y), ImVec2(LeftKnee.X, LeftKnee.Y), ImVec2(LeftKnee.X, LeftKnee.Y), ImVec2(LeftFoot.X, LeftFoot.Y), ImColor(SegaUi::GetColorU32(CurrentConfig.SkeletonEspColorVisible)), 1, 10);
                        SegaUi::GetBackgroundDrawList()->AddBezierCubic(ImVec2(Penis.X, Penis.Y), ImVec2(RightKnee.X, RightKnee.Y), ImVec2(RightKnee.X, RightKnee.Y), ImVec2(RightFoot.X, RightFoot.Y), ImColor(SegaUi::GetColorU32(CurrentConfig.SkeletonEspColorVisible)), 1, 10);
                        SegaUi::GetBackgroundDrawList()->AddBezierCubic(ImVec2(Neck.X, Neck.Y), ImVec2(LeftShoulder.X, LeftShoulder.Y), ImVec2(LeftShoulder.X, LeftShoulder.Y), ImVec2(LeftShoulder.X, LeftShoulder.Y), ImColor(SegaUi::GetColorU32(CurrentConfig.SkeletonEspColorVisible)), 1, 10);
                        SegaUi::GetBackgroundDrawList()->AddBezierCubic(ImVec2(Neck.X, Neck.Y), ImVec2(RightShoulder.X, RightShoulder.Y), ImVec2(RightShoulder.X, RightShoulder.Y), ImVec2(RightShoulder.X, RightShoulder.Y), ImColor(SegaUi::GetColorU32(CurrentConfig.SkeletonEspColorVisible)), 1, 10);
                        SegaUi::GetBackgroundDrawList()->AddBezierCubic(ImVec2(LeftShoulder.X, LeftShoulder.Y), ImVec2(LeftElbow.X, LeftElbow.Y), ImVec2(LeftElbow.X, LeftElbow.Y), ImVec2(LeftHand.X, LeftHand.Y), ImColor(SegaUi::GetColorU32(CurrentConfig.SkeletonEspColorVisible)), 1, 10);
                        SegaUi::GetBackgroundDrawList()->AddBezierCubic(ImVec2(RightShoulder.X, RightShoulder.Y), ImVec2(RightElbow.X, RightElbow.Y), ImVec2(RightElbow.X, RightElbow.Y), ImVec2(RightHand.X, RightHand.Y), ImColor(SegaUi::GetColorU32(CurrentConfig.SkeletonEspColorVisible)), 1, 10);
                    }


                    if (CurrentConfig.bFilledBoxEsp)
                    {
                        if (EntityList[i]->bIsVisible)
                        {
                            SegaUi::GetBackgroundDrawList()->AddRectFilled(box_min_size, box_max_size, ImColor(SegaUi::GetColorU32(CurrentConfig.FilledBoxEspColorVisible)), 5.f, 0);
                        }
                        else
                        {
                            SegaUi::GetBackgroundDrawList()->AddRectFilled(box_min_size, box_max_size, ImColor(SegaUi::GetColorU32(ImVec4(CurrentConfig.GlobalNotVisible.x, CurrentConfig.GlobalNotVisible.y, CurrentConfig.GlobalNotVisible.z, CurrentConfig.FilledBoxEspColorVisible.w))), 5.f, 0);
                        }
                    }

                    if (CurrentConfig.bCornerBoxEsp)
                    {

                        draw_corner_box(SegaUi::GetBackgroundDrawList(), box_min_size.x, box_min_size.y, entity_width, entity_height, IM_COL32_BLACK, 3);

                        if (EntityList[i]->bIsVisible)
                        {
                            draw_corner_box(SegaUi::GetBackgroundDrawList(), box_min_size.x, box_min_size.y, entity_width, entity_height, ImColor(SegaUi::GetColorU32(CurrentConfig.CornerBoxEspColorVisible)), 1);
                        }
                        else
                        {
                            draw_corner_box(SegaUi::GetBackgroundDrawList(), box_min_size.x, box_min_size.y, entity_width, entity_height, ImColor(SegaUi::GetColorU32(CurrentConfig.GlobalNotVisible)), 1);
                        }
                    }

                    if (CurrentConfig.bBoxEsp)
                    {
                        SegaUi::GetBackgroundDrawList()->AddRect(box_min_size, box_max_size, IM_COL32_BLACK, 5.f, 0, 3.0f);
                        if (EntityList[i]->bIsVisible)
                        {
                            SegaUi::GetBackgroundDrawList()->AddRect(box_min_size, box_max_size, ImColor(SegaUi::GetColorU32(CurrentConfig.BoxEspColorVisible)), 5.f, 0, 1.0f);
                        }
                        else
                        {
                            SegaUi::GetBackgroundDrawList()->AddRect(box_min_size, box_max_size, ImColor(SegaUi::GetColorU32(CurrentConfig.GlobalNotVisible)), 5.f, 0, 1.0f);
                        }
                    }

                    SegaUi::PushFont(font::inter_bold_v2);

                    if (CurrentConfig.bDistanceEsp)
                    {
                        std::string DistanceText = (std::to_string((int)DistanceMeters) + " M");
                        SegaUiIO& io = SegaUi::GetIO(); (void)io;
                        ImVec2 txt_size = SegaUi::CalcTextSize(DistanceText.c_str());
                        ImVec2 New_txt_size = (txt_size / font::inter_bold_v2->FontSize) * Size;

                        for (int offsetX = -1; offsetX <= 1; ++offsetX) {
                            for (int offsetY = -1; offsetY <= 1; ++offsetY) {
                                if (offsetX == 0 && offsetY == 0)
                                    continue;
                                SegaUi::GetBackgroundDrawList()->AddText(
                                    font::inter_bold_v2,
                                    Size,
                                    ImVec2(EntityList[i]->ZeroLocationW2S.X - New_txt_size.x / 2 + offsetX, EntityList[i]->ZeroLocationW2S.Y - New_txt_size.y / 2 + offsetY),
                                    SegaUi::GetColorU32(IM_COL32_BLACK),
                                    DistanceText.c_str(),
                                    0,
                                    0,
                                    0
                                );
                            }
                        }

                        SegaUi::GetBackgroundDrawList()->AddText(
                            font::inter_bold_v2,
                            Size,
                            ImVec2(EntityList[i]->ZeroLocationW2S.X - New_txt_size.x / 2, EntityList[i]->ZeroLocationW2S.Y - New_txt_size.y / 2),
                            ImColor(SegaUi::GetColorU32(CurrentConfig.DistanceEspColorVisible)),
                            DistanceText.c_str(),
                            0,
                            0,
                            0
                        );
                    }

                    if (CurrentConfig.bNamesEsp)
                    {
                        SegaUiIO& io = SegaUi::GetIO(); (void)io;
                        ImVec2 txt_size = SegaUi::CalcTextSize(EntityList[i]->Name.c_str());
                        ImVec2 New_txt_size = (txt_size / font::inter_bold_v2->FontSize) * Size;

                        for (int offsetX = -1; offsetX <= 1; ++offsetX) {
                            for (int offsetY = -1; offsetY <= 1; ++offsetY) {
                                if (offsetX == 0 && offsetY == 0)
                                    continue;
                                SegaUi::GetBackgroundDrawList()->AddText(
                                    font::inter_bold_v2,
                                    Size,
                                    ImVec2(head_screen.X - New_txt_size.x / 2 + offsetX, head_screen.Y - New_txt_size.y / 2 + offsetY),
                                    SegaUi::GetColorU32(IM_COL32_BLACK),
                                    EntityList[i]->Name.c_str(),
                                    0,
                                    0,
                                    0
                                );
                            }
                        }

                        SegaUi::GetBackgroundDrawList()->AddText(
                            font::inter_bold_v2,
                            Size,
                            ImVec2(head_screen.X - New_txt_size.x / 2, head_screen.Y - New_txt_size.y / 2),
                            ImColor(SegaUi::GetColorU32(CurrentConfig.NamesEspColorVisible)),
                            EntityList[i]->Name.c_str(),
                            0,
                            0,
                            0
                        );

                        OffsetY += Size;
                    }

                    if (CurrentConfig.bPlatformEsp)
                    {
                        SegaUiIO& io = SegaUi::GetIO(); (void)io;
                        ImVec2 txt_size = SegaUi::CalcTextSize(EntityList[i]->Platform.c_str());
                        ImVec2 New_txt_size = (txt_size / font::inter_bold_v2->FontSize) * Size;
                        ImVec2 Location = ImVec2(head_screen.X - Size / 2, head_screen.Y - Size / 2 - OffsetY);

                        ID3D11ShaderResourceView* CurrentPlatform = nullptr;
                        bool ShowIcon = true;
                        if (EntityList[i]->Platform == "WIN")
                        {
                            CurrentPlatform = pic::old_windows_logo;
                        }

                        else if (EntityList[i]->Platform == "SWT")
                        {
                            CurrentPlatform = pic::Nintendo_logo;
                        }

                        else if (EntityList[i]->Platform == "PS5")
                        {
                            CurrentPlatform = pic::playstation_logo;
                        }

                        else if (EntityList[i]->Platform == "PSN")
                        {
                            CurrentPlatform = pic::playstation_logo;
                        }

                        else if (EntityList[i]->Platform == "AND")
                        {
                            CurrentPlatform = pic::android_logo;
                        }

                        else if (EntityList[i]->Platform == "IOS")
                        {
                            CurrentPlatform = pic::ios_logo;
                        }

                        else if (EntityList[i]->Platform == "XSX")
                        {
                            CurrentPlatform = pic::xbox_logo;
                        }
                        else if (EntityList[i]->Platform == "XBL")
                        {
                            CurrentPlatform = pic::xbox_logo;
                        }
                        else
                        {

                            for (int offsetX = -1; offsetX <= 1; ++offsetX) {
                                for (int offsetY = -1; offsetY <= 1; ++offsetY) {
                                    if (offsetX == 0 && offsetY == 0)
                                        continue;
                                    SegaUi::GetBackgroundDrawList()->AddText(
                                        font::inter_bold_v2,
                                        Size,
                                        ImVec2(head_screen.X - New_txt_size.x / 2 + offsetX, head_screen.Y - New_txt_size.y / 2 + offsetY - OffsetY),
                                        SegaUi::GetColorU32(IM_COL32_BLACK),
                                        EntityList[i]->Platform.c_str(),
                                        0,
                                        0,
                                        0
                                    );
                                }
                            }

                            SegaUi::GetBackgroundDrawList()->AddText(
                                font::inter_bold_v2,
                                Size,
                                ImVec2(head_screen.X - New_txt_size.x / 2, head_screen.Y - New_txt_size.y / 2 - OffsetY),
                                ImColor(SegaUi::GetColorU32(CurrentConfig.PlatformEspColorVisible)),
                                EntityList[i]->Platform.c_str(),
                                0,
                                0,
                                0
                            );


                            CurrentPlatform = pic::exploits;
                            ShowIcon = false;
                        }

                        if (ShowIcon)
                        {
                            SegaUi::GetBackgroundDrawList()->AddImage(CurrentPlatform, Location, ImVec2(Location.x + Size, Location.y + Size), ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 255));
                        }

                        OffsetY += Size;
                    }


                    if (CurrentConfig.bKillsEsp)
                    {
                        SegaUiIO& io = SegaUi::GetIO(); (void)io;
                        std::string KillsText = std::to_string(EntityList[i]->Kills) + " kills";
                        ImVec2 txt_size = SegaUi::CalcTextSize(KillsText.c_str());
                        ImVec2 New_txt_size = (txt_size / font::inter_bold_v2->FontSize) * Size;

                        for (int offsetX = -1; offsetX <= 1; ++offsetX) {
                            for (int offsetY = -1; offsetY <= 1; ++offsetY) {
                                if (offsetX == 0 && offsetY == 0)
                                    continue;
                                SegaUi::GetBackgroundDrawList()->AddText(
                                    font::inter_bold_v2,
                                    Size,
                                    ImVec2(head_screen.X - New_txt_size.x / 2 + offsetX, head_screen.Y - New_txt_size.y / 2 + offsetY - OffsetY),
                                    SegaUi::GetColorU32(IM_COL32_BLACK),
                                    KillsText.c_str(),
                                    0,
                                    0,
                                    0
                                );
                            }
                        }

                        SegaUi::GetBackgroundDrawList()->AddText(
                            font::inter_bold_v2,
                            Size,
                            ImVec2(head_screen.X - New_txt_size.x / 2, head_screen.Y - New_txt_size.y / 2 - OffsetY),
                            ImColor(SegaUi::GetColorU32(CurrentConfig.KillsEspColorVisible)),
                            KillsText.c_str(),
                            0,
                            0,
                            0
                        );
                        OffsetY += Size;
                    }

                    //std::cout << "\n weap: " << EntityList[i]->Weapon;
                    if (CurrentConfig.bEnemyWeaponEsp)
                    {
                        if (EntityList[i]->Weapon)
                        {

                            SegaUiIO& io = SegaUi::GetIO(); (void)io;
                            std::string KillsText = EntityList[i]->Weapon->Name;

                            ImVec2 txt_size = SegaUi::CalcTextSize(KillsText.c_str());
                            ImVec2 New_txt_size = (txt_size / font::inter_bold_v2->FontSize) * Size;

                            ByteColor RarityColor = rarityAWeaponColors[EntityList[i]->Weapon->PawnWeapon];

                            for (int offsetX = -1; offsetX <= 1; ++offsetX) {
                                for (int offsetY = -1; offsetY <= 1; ++offsetY) {
                                    if (offsetX == 0 && offsetY == 0)
                                        continue;
                                    SegaUi::GetBackgroundDrawList()->AddText(
                                        font::inter_bold_v2,
                                        Size,
                                        ImVec2(head_screen.X - New_txt_size.x / 2 + offsetX, head_screen.Y - New_txt_size.y / 2 + offsetY - OffsetY),
                                        SegaUi::GetColorU32(IM_COL32_BLACK),
                                        KillsText.c_str(),
                                        0,
                                        0,
                                        0
                                    );
                                }
                            }

                            SegaUi::GetBackgroundDrawList()->AddText(
                                font::inter_bold_v2,
                                Size,
                                ImVec2(head_screen.X - New_txt_size.x / 2, head_screen.Y - New_txt_size.y / 2 - OffsetY),
                                ImColor(IM_COL32(RarityColor.R, RarityColor.G, RarityColor.B, RarityColor.A)),
                                KillsText.c_str(),
                                0,
                                0,
                                0
                            );

                            OffsetY += Size;
                        }
                    }

                    SegaUi::PopFont();

                    float LastDistanceBone = FLT_MAX;
                    FVector GlobalAimBonePos = {};
                    FVector2D GlobalAimBonePosW2S = {};
                    for (int64_t ii = 0; ii < CurrentConfig.AimBones.size(); ii++)
                    {
                        if (CurrentConfig.AimBones[ii].bIsActivated)
                        {
                            FVector2D CurrentBonePosW2S = {};
                            //FTransform CurrentBonePosFT = driver->read<FTransform>(EntityList[i]->BoneArray + (CurrentConfig.AimBones[ii].Bone * 0x60));
                            FTransform CurrentBonePosFT = EntityList[i]->GetBoneByName(CurrentConfig.AimBones[ii].Name)->Transform;
                            //FTransform CurrentBonePosFT = EntityList[i]->Bones[CurrentConfig.AimBones[ii].Bone]->Transform;
                            FVector CurrentBonePos = ChildToWorld(CurrentBonePosFT, EntityList[i]->FZeroLocation);
                            ProjectWorldToScreen(CurrentBonePos, GameInfo.ViewRect, GameInfo.ViewInfo.ViewProjectionMatrix, CurrentBonePosW2S);
                            double BoneDist = GetW2SDistance(CurrentBonePosW2S, GameInfo.ViewRect);

                            if (BoneDist < LastDistanceBone)
                            {
                                LastDistanceBone = BoneDist;
                                GlobalAimBonePos = CurrentBonePos;
                                GlobalAimBonePosW2S = CurrentBonePosW2S;
                            }
                        }
                    }

                    if (!EntityList[i]->bIsDBNO)
                    {
                        if (LastDistanceBone < CurrentConfig.WeaponConfigs[CurrentConfig.CurrentWeaponIndex].Fov && LastDistanceBone < LastDistance && EntityList[i]->bIsVisible)
                        {
                            if (((float)EntityList[i]->Distance / 100) < CurrentConfig.WeaponConfigs[CurrentConfig.CurrentWeaponIndex].Distance)
                            {
                                LastDistance = LastDistanceBone;
                                Target_Pawn = EntityList[i];
                                Target_Pawn->GlobalAimBonePos = GlobalAimBonePos;
                                Target_Pawn->GlobalAimBonePosW2S = GlobalAimBonePosW2S;
                            }
                        }
                    }

                }


                if (Target_Pawn)
                {
                    if (Target_Pawn->GlobalAimBonePos.X)
                    {

                        if (CurrentConfig.WeaponConfigs[CurrentConfig.CurrentWeaponIndex].Aimbot)
                        {
                            FVector2D PredictedLw2s = {};
                            FVector NewTargetPosition;
                            if (CurrentConfig.bPrediction)
                            {
                                double Size = 0;
                                FVector Location;
                                FRotator Rotation;
                                GetViewInfo(GameInfo.ViewInfo, Location, Rotation, 1);


                                double Distance = CalculateDistance(Location, Target_Pawn->GlobalAimBonePos);
                                double DistanceMeters = CalculateDistance(Location, Target_Pawn->GlobalAimBonePos) / 100;

                                Size = 100 / DistanceMeters;

                                if (Size < 5)
                                {
                                    Size = 5;
                                }

                                FVector TargetVelocity = driver->read<FVector>(Target_Pawn->USceneComponent + 0x168);
                                NewTargetPosition = AimbotPrediction(CurrentConfig.BulletSpeed, CurrentConfig.BulletGravity, Distance, Target_Pawn->GlobalAimBonePos, TargetVelocity);
                                //NewTargetPosition = CalculatePredictedPosition(Target_Pawn->GlobalAimBonePos, TargetVelocity, CurrentConfig.BulletGravity, CurrentConfig.BulletSpeed, Distance);


                                ProjectWorldToScreen(NewTargetPosition, GameInfo.ViewRect, GameInfo.ViewInfo.ViewProjectionMatrix, PredictedLw2s);
                            }
                            bool KeyDown = false;
#ifdef DMA
                            KeyDown = driver->GetKeyboard()->IsKeyDown(CurrentConfig.AimKeyIndex);
#endif // DMA

#ifdef Driver
                            KeyDown = GetAsyncKeyState(CurrentConfig.AimKeyIndex) & 0x8000;
#endif // Driver

                            if (KeyDown)
                            {
                                FVector2D EndPointw2s;
                                if (CurrentConfig.bPrediction)
                                {
                                    EndPointw2s = PredictedLw2s;
                                }
                                else
                                {
                                    EndPointw2s = Target_Pawn->GlobalAimBonePosW2S;
                                }

                                if (CurrentConfig.AimMode == 1)
                                {
                                    double target_x, target_y = 0.0;
                                    int centerx = GameInfo.ViewRect.Right / 2;
                                    int centery = GameInfo.ViewRect.Bottom / 2;
                                    if (EndPointw2s.X > centerx)
                                        target_x = -(centerx - EndPointw2s.X) / 250.0 * 100 / CurrentConfig.WeaponConfigs[CurrentConfig.CurrentWeaponIndex].Smooth;
                                    else if (EndPointw2s.X < centerx)
                                        target_x = (EndPointw2s.X - centerx) / 250.0 * 100 / CurrentConfig.WeaponConfigs[CurrentConfig.CurrentWeaponIndex].Smooth;
                                    if (EndPointw2s.Y > centery)
                                        target_y = -(centery - EndPointw2s.Y) / 250.0 * 100 / CurrentConfig.WeaponConfigs[CurrentConfig.CurrentWeaponIndex].Smooth;
                                    else if (EndPointw2s.Y < centery)
                                        target_y = (EndPointw2s.Y - centery) / 250.0 * 100 / CurrentConfig.WeaponConfigs[CurrentConfig.CurrentWeaponIndex].Smooth;

                                    if (CurrentConfig.KmBoxStatus)
                                    {
                                        //FAimBotRequest NewRequest;
                                        //
                                        //NewRequest.X = target_x;
                                        //NewRequest.Y = target_y;
                                        //NewRequest.KmBoxModel = CurrentConfig.KmBoxModelIndex;
                                        //AimBotRequests.push_back(NewRequest);
                                        //std::cout << "\n added new aimbot req! AimBotRequests.size(): " << AimBotRequests.size();

                                        if (CurrentConfig.KmBoxModelIndex == 0) KmBoxMgr.Mouse.Move(target_x, target_y);
                                        if (CurrentConfig.KmBoxModelIndex == 1) kmBoxBMgr.km_move(target_x, target_y);
                                    }


                                }
                                else
                                {

                                        ControllWeapon(EndPointw2s, CurrentConfig.WeaponConfigs[CurrentConfig.CurrentWeaponIndex].Smooth, GameInfo.AFortWeapon, GameInfo.ViewRect, GameInfo.IsRanged);
   
                                }
                            }
                        }
                    }
                }

                if (CurrentConfig.bRadarEsp)
                {

                    static ImVec4 BackGroundColor = ImVec4(0.1f, 0.1f, 0.1f, 0.5f);
                    static float RadarRadiusMeters = 50.0f;


                    SegaUi::GetBackgroundDrawList()->AddRectFilled(
                        CurrentConfig.RadarStartPos, CurrentConfig.RadarStartPos + CurrentConfig.RadarSize,
                        ImColor(SegaUi::GetColorU32(BackGroundColor)), 5.f, 0);



                    CameraLocation /= 100.0f;
                    ImVec2 PlayerLocationOnRadar = CurrentConfig.RadarStartPos + CurrentConfig.RadarSize * 0.5f;
                    ImVec2 CameraSize = CurrentConfig.RadarSize / 15;

                    SegaUi::GetBackgroundDrawList()->AddImage(pic::Camera, PlayerLocationOnRadar - CameraSize, PlayerLocationOnRadar + CameraSize, ImVec2(0, 0), ImVec2(1, 1), ImColor(255, 255, 255, 150));




                    float CameraYawDegrees = CameraRotation.Yaw;
                    float CameraYawRadians = CameraYawDegrees * (M_PI / 180.0f);



                    for (int64_t i = 0; i < EntityList.size(); i++)
                    {

                        ImVec4 EntityColor = CurrentConfig.GlobalNotVisible;
                        if (EntityList[i]->bIsVisible)
                        {
                            EntityColor = CurrentConfig.BoxEspColorVisible;
                        }

                        EntityList[i]->ZeroLocation /= 100.0f;
                        float offsetX = EntityList[i]->ZeroLocation.X - CameraLocation.X;
                        float offsetY = EntityList[i]->ZeroLocation.Y - CameraLocation.Y;
                        float rotatedX = offsetX * cos(-CameraYawRadians) - offsetY * sin(-CameraYawRadians);
                        float rotatedY = offsetX * sin(-CameraYawRadians) + offsetY * cos(-CameraYawRadians);

                        ImVec2 EntityPosition = ImVec2(rotatedX, rotatedY);
                        ImVec2 EntityLocationOnRadar = PlayerLocationOnRadar + (EntityPosition / RadarRadiusMeters) * (CurrentConfig.RadarSize * 0.5f);

                        // Ограничение позиции объекта в пределах радара (пример)
                        if (EntityLocationOnRadar.x < CurrentConfig.RadarStartPos.x)
                        {
                            EntityLocationOnRadar.x = CurrentConfig.RadarStartPos.x;
                        }

                        if (EntityLocationOnRadar.y < CurrentConfig.RadarStartPos.y)
                        {
                            EntityLocationOnRadar.y = CurrentConfig.RadarStartPos.y;
                        }

                        if (EntityLocationOnRadar.x > CurrentConfig.RadarStartPos.x + CurrentConfig.RadarSize.x)
                        {
                            EntityLocationOnRadar.x = CurrentConfig.RadarStartPos.x + CurrentConfig.RadarSize.x;
                        }

                        if (EntityLocationOnRadar.y > CurrentConfig.RadarStartPos.y + CurrentConfig.RadarSize.y)
                        {
                            EntityLocationOnRadar.y = CurrentConfig.RadarStartPos.y + CurrentConfig.RadarSize.y;
                        }

                        //SegaUi::GetBackgroundDrawList()->AddRectFilled(
                        //    EntityLocationOnRadar - ImVec2(2, 2), EntityLocationOnRadar + ImVec2(4, 4),
                        //    ImColor(SegaUi::GetColorU32(EntityColor)), 10.f, 0);

                        SegaUi::GetForegroundDrawList()->AddCircleFilled(EntityLocationOnRadar, 4, ImColor(SegaUi::GetColorU32(EntityColor)), 10);

                    }
                }

                if (CurrentConfig.bShowFov)
                {
                    SegaUi::GetBackgroundDrawList()->AddCircle(ImVec2(GameInfo.ViewRect.Width() / 2, GameInfo.ViewRect.Height() / 2), CurrentConfig.CurrentFov, ImColor(255, 255, 255, 255));
                }
            }

            if (CurrentConfig.CurrentWeaponIndex < CurrentConfig.WeaponConfigs.size())
            {

                static int StartTime = 0;
                if (!StartTime)
                {
                    StartTime = clock();
                }
                if (StartTime + 2 < clock())
                {
                    StartTime = clock();

                    int PotentialFov = CurrentConfig.WeaponConfigs[CurrentConfig.CurrentWeaponIndex].Fov;
                    if (CurrentConfig.CurrentFov > PotentialFov)
                    {
                        CurrentConfig.CurrentFov = CurrentConfig.CurrentFov - 5;
                        if (CurrentConfig.CurrentFov < PotentialFov)
                        {
                            CurrentConfig.CurrentFov = PotentialFov;
                        }
                    }
                    if (CurrentConfig.CurrentFov < PotentialFov)
                    {
                        CurrentConfig.CurrentFov = CurrentConfig.CurrentFov + 5;
                        if (CurrentConfig.CurrentFov > PotentialFov)
                        {
                            CurrentConfig.CurrentFov = PotentialFov;
                        }
                    }
                }
            }
            

            if (bShowEnterNotification)
            {
                std::string text = std::string(Dictionary[CurrentConfig.Language][(sk_crypt("EnterNotification").decrypt())]);
                SegaUi::Notification({ SegaUiToastType_Success, 5000,  text.c_str() });
                bShowEnterNotification = false;
            }

            if (GetAsyncKeyState(CurrentConfig.MenuKeyIndex) & 1)
            {
                CurrentConfig.bShowMenu = !CurrentConfig.bShowMenu;
                std::string text = std::string(Dictionary[CurrentConfig.Language][(sk_crypt("MenuHas").decrypt())] + " ") + (CurrentConfig.bShowMenu ? Dictionary[CurrentConfig.Language]["enabled"] : Dictionary[CurrentConfig.Language]["disabled"]);

                SegaUi::Notification({ SegaUiToastType_Info, 2000,  text.c_str()});

                //if (CurrentConfig.bShowMenu)
                //{
                //    SetWindowLong(
                //        hwnd,
                //        GWL_EXSTYLE,
                //        WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_NOACTIVATE);
                //}
                //else
                //{
                //    SetWindowLong(
                //        hwnd,
                //        GWL_EXSTYLE,
                //        WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_NOACTIVATE);
                //}

            }
            if (CurrentConfig.bShowMenu)
            {

                if (CurrentConfig.CurrentMainTab == 3)
                {

                    //SegaUi::SetNextWindowSize({ c::chat::size });
                    //{
                    //    if (!c::chat::pos.x)
                    //    {
                    //        c::chat::pos = ImVec2(GameInfo.ViewRect.Width() / 1.5, GameInfo.ViewRect.Height() / 4);

                    //        SegaUi::SetNextWindowPos({ c::chat::pos });
                    //    }


                    //}

                    //SegaUi::Begin(sk_crypt("Chat Menu").decrypt(), nullptr, SegaUiWindowFlags_NoBackground | SegaUiWindowFlags_NoDecoration | SegaUiWindowFlags_NoFocusOnAppearing | SegaUiWindowFlags_NoBringToFrontOnFocus);
                    //{
                    //    const ImVec2 pos = SegaUi::GetWindowPos();
                    //    MenuPos = SegaUi::GetWindowPos();
                    //    const ImVec2 spacing = style->ItemSpacing;
                    //    const ImVec2 region = SegaUi::GetContentRegionMax();
                    //    SegaUi::GetBackgroundDrawList()->AddImage(pic::blur2, MenuPos, MenuPos + c::chat::size, ImVec2(0, 0), ImVec2(1, 1), SegaUi::GetColorU32(ImVec4(1, 1, 1, 1)));

                    //    SegaUi::GetBackgroundDrawList()->AddRectFilled(pos, pos + region, SegaUi::GetColorU32(c::chat::background), c::bg::rounding);
                    //    SegaUi::GetBackgroundDrawList()->AddRect(pos, pos + region, SegaUi::GetColorU32(c::child::outline), c::child::rounding);
                    //    SegaUi::SetCursorPos(ImVec2(spacing.x / 2, 62 + spacing.y));
                    //    std::string Chat = "Chat";
                    //    SegaUi::GlowText(font::inter_bold, 20.f, Chat.c_str(), pos + (ImVec2(100, 62 + spacing.y) + spacing - SegaUi::CalcTextSize(Chat.c_str())) / 2, SegaUi::GetColorU32(c::accent));
                    //    SegaUi::BeginGroup();
                    //    {
                    //        SegaUi::Separator();
                    //        ImVec2 CursorPos = SegaUi::GetCursorPos();
                    //        SegaUi::GlowText(font::inter_bold, 17.f, "азеро гей", pos + CursorPos, SegaUi::GetColorU32(c::accent));
                    //        SegaUi::SetCursorPos(CursorPos + ImVec2(0, spacing.y * 2));
                    //        SegaUi::TextColored(ImVec4(1, 1, 0, 1), "Я курю жирный член");
                    //        SegaUi::Separator();

                    //        CursorPos = SegaUi::GetCursorPos();
                    //        SegaUi::GlowText(font::inter_bold, 17.f, "segamfle01", pos + CursorPos, SegaUi::GetColorU32(c::accent));
                    //        SegaUi::SetCursorPos(CursorPos + ImVec2(0, spacing.y * 2));
                    //        SegaUi::TextColored(ImVec4(1, 1, 0, 1), "я знаю, азеро");
                    //        SegaUi::Separator();


                    //    }
                    //    SegaUi::EndGroup();


                    //    ImVec2 bottomPos = ImVec2(0, SegaUi::GetContentRegionMax().y - style->WindowPadding.y - 40); // 40 - высота кнопки
                    //    SegaUi::SetCursorPos(bottomPos);

                    //    if (SegaUi::Button("Send Message", ImVec2(SegaUi::GetContentRegionMax().x - style->WindowPadding.x, 40))) {
                    //        // Действия по нажатию кнопки
                    //    }

                    //    //SegaUi::GetBackgroundDrawList()->AddRectFilled(bottomPos, ImVec2(SegaUi::GetContentRegionMax().x - style->WindowPadding.x * 2, 40), SegaUi::GetColorU32(c::child::background), c::child::rounding);


                    //    ////SegaUi::SetCursorPos(ImVec2(region));
                    //    //if (SegaUi::Button("Send Message", ImVec2(SegaUi::GetContentRegionMax().x - style->WindowPadding.x, 40))) {

                    //    //}

                    //}
                    //SegaUi::End();
                }

                SegaUi::SetNextWindowSize({ c::bg::size });


                SegaUi::Begin(sk_crypt("Main Menu").decrypt(), nullptr, SegaUiWindowFlags_NoBackground | SegaUiWindowFlags_NoDecoration | SegaUiWindowFlags_NoFocusOnAppearing | SegaUiWindowFlags_NoBringToFrontOnFocus);
                {
                    const ImVec2 pos = SegaUi::GetWindowPos();
                    MenuPos = SegaUi::GetWindowPos();
                    SegaUi::GetBackgroundDrawList()->AddImage(pic::blur1, MenuPos, MenuPos + c::bg::size, ImVec2(0, 0), ImVec2(1,1), SegaUi::GetColorU32(ImVec4(1, 1, 1, 1)));
                    const ImVec2 spacing = style->ItemSpacing;
                    const ImVec2 region = SegaUi::GetContentRegionMax();

                    SegaUi::GetBackgroundDrawList()->AddRectFilled(pos, pos + region, SegaUi::GetColorU32(c::bg::background), c::bg::rounding);
                    SegaUi::GetBackgroundDrawList()->AddRect(pos, pos + region, SegaUi::GetColorU32(c::child::outline), c::child::rounding);


                     
                    { //зона с вертикальными табами
                        SegaUi::GetBackgroundDrawList()->AddRectFilled(pos + spacing, pos + ImVec2(180, region.y - spacing.y), SegaUi::GetColorU32(c::child::background), c::child::rounding);
                        SegaUi::GetBackgroundDrawList()->AddRect(pos + spacing, pos + ImVec2(180, region.y - spacing.y), SegaUi::GetColorU32(c::child::outline), c::child::rounding);
                    }
                     

                    { //зона с мини табами

                        SegaUi::GetBackgroundDrawList()->AddRectFilled(pos + ImVec2(180, 0) + spacing, pos + ImVec2(region.x - (spacing.x * 2) - 62, 62 + spacing.y), SegaUi::GetColorU32(c::child::background), c::child::rounding);
                        SegaUi::GetBackgroundDrawList()->AddRect(pos + ImVec2(180, 0) + spacing, pos + ImVec2(region.x - (spacing.x * 2) - 62, 62 + spacing.y), SegaUi::GetColorU32(c::child::outline), c::child::rounding);
                    }
                     
                    { //зона с иконкой
                        ImVec2 containerPos = pos + ImVec2(region.x - (spacing.x * 2) - 62, 0) + spacing;
                        static float containerSize = 62.0f;
                        static float cornerRadius = 10.0f;

                        SegaUi::GetBackgroundDrawList()->AddRectFilled(containerPos, containerPos + ImVec2(containerSize, containerSize), IM_COL32(255, 255, 255, 255), cornerRadius);
                        ImVec2 clipMin = containerPos;
                        ImVec2 clipMax = containerPos + ImVec2(containerSize, containerSize);
                        SegaUi::GetBackgroundDrawList()->PushClipRect(clipMin, clipMax, true);
                        ImVec2 imagePos = containerPos + ImVec2((containerSize - 54.0f) * 0.5f, (containerSize - 54.0f) * 0.5f);
                        static ImVec2 imageSize = ImVec2(54.0f, 54.0f);
                        SegaUi::GetBackgroundDrawList()->AddImage(pic::menu_settings_icon, imagePos, imagePos + imageSize);
                        SegaUi::GetBackgroundDrawList()->PopClipRect();

                       // SegaUi::GetBackgroundDrawList()->AddImage(pic::menu_settings_icon, pos + ImVec2(region.x - (spacing.x * 2) - 62, 0) + spacing + ImVec2(4, 4), pos + ImVec2(region.x - spacing.x, 62 + spacing.y) - ImVec2(4, 4), ImVec2(0, 0), ImVec2(1, 1));

                        SegaUi::GetBackgroundDrawList()->AddRect(pos + ImVec2(region.x - (spacing.x * 2) - 62, 0) + spacing, pos + ImVec2(region.x - spacing.x, 62 + spacing.y), SegaUi::GetColorU32(c::child::outline), c::child::rounding);
                    }

                    SegaUi::SetCursorPos(ImVec2(spacing.x * 2, 62 + spacing.y * 2));
                    SegaUi::BeginGroup();
                    {
                        SegaUi::GlowText(font::inter_bold, 17.f, ProductName.c_str(), pos + (ImVec2(180, 62 + spacing.y) + spacing - SegaUi::CalcTextSize(ProductName.c_str())) / 2, SegaUi::GetColorU32(c::accent));


                        if (SegaUi::Tabs(0 == CurrentConfig.CurrentMainTab, pic::aimbot, Dictionary[CurrentConfig.Language][(sk_crypt("Aimbot").decrypt())].c_str(), ImVec2(180 - spacing.x * 3, 39), NULL)) CurrentConfig.CurrentMainTab = 0;

                        if (SegaUi::Tabs(1 == CurrentConfig.CurrentMainTab, pic::visuals, Dictionary[CurrentConfig.Language][(sk_crypt("Visuals").decrypt())].c_str(), ImVec2(180 - spacing.x * 3, 39), NULL)) CurrentConfig.CurrentMainTab = 1;

                        if (SegaUi::Tabs(2 == CurrentConfig.CurrentMainTab, pic::exploits, Dictionary[CurrentConfig.Language][(sk_crypt("Exploits").decrypt())].c_str(), ImVec2(180 - spacing.x * 3, 39), NULL)) CurrentConfig.CurrentMainTab = 2;

                        if (SegaUi::Tabs(3 == CurrentConfig.CurrentMainTab, pic::Chat, Dictionary[CurrentConfig.Language][(sk_crypt("Chat").decrypt())].c_str(), ImVec2(180 - spacing.x * 3, 39), NULL)) CurrentConfig.CurrentMainTab = 3;

                        if (SegaUi::Tabs(4 == CurrentConfig.CurrentMainTab, pic::settings, Dictionary[CurrentConfig.Language][(sk_crypt("Settings").decrypt())].c_str(), ImVec2(180 - spacing.x * 3, 39), NULL)) CurrentConfig.CurrentMainTab = 4;
                    }
                    SegaUi::EndGroup();

                    if (CurrentConfig.CurrentMainTab == 0)
                    {
                        SegaUi::SetCursorPos(ImVec2(180 + spacing.x, spacing.y));
                        //SegaUi::BeginGroup();
                        //{
                        //    ImVec2 LocalPos = SegaUi::GetCursorPos();
                        //    if (SegaUi::SubTab(0 == CurrentConfig.CurrentWeaponTab, CurrentConfig.WeaponConfigs[0].WeaponName.c_str(), ImVec2(100, 62))) CurrentConfig.CurrentWeaponTab = 0;

                        //}
                        //SegaUi::EndGroup();

                        SegaUi::BeginGroup();
                        {
                            ImVec2 LocalPos = SegaUi::GetCursorPos();

                            for (int i = 0; i < CurrentConfig.WeaponConfigs.size(); i++)
                            {
                                if (SegaUi::SubTab(i == CurrentConfig.CurrentWeaponTab, CurrentConfig.WeaponConfigs[i].WeaponName.c_str(), ImVec2(100, 62))) CurrentConfig.CurrentWeaponTab = i;
                                if (i != CurrentConfig.WeaponConfigs.size() - 1)
                                {
                                    SegaUi::SameLine(0, 5);
                                }

                            }

                        }
                        SegaUi::EndGroup();
                    }

                    if (CurrentConfig.CurrentMainTab == 1)
                    {
                        SegaUi::SetCursorPos(ImVec2(180 + spacing.x, spacing.y));
                        SegaUi::BeginGroup();
                        {
                            ImVec2 Cur = SegaUi::GetCursorPos();
                            ImVec2 TabSize = ImVec2(100, 62);
                            ImVec2 PngSize = ImVec2(50, 50);

                            ImVec2 PngStart;
                            PngStart.x = TabSize.x / 2 - PngSize.x / 2;
                            PngStart.y = TabSize.y / 2 - PngSize.y / 2;

                            ImVec2 PngEnd = PngStart + PngSize;

                            //SegaUi::GetBackgroundDrawList()->AddImage(pic::exploits,MenuPos + Cur + PngStart, MenuPos + Cur + PngEnd);

                            if (SegaUi::SubTab(0 == CurrentConfig.CurrentEspTab, Dictionary[CurrentConfig.Language][(sk_crypt("Enemy").decrypt())].c_str(), ImVec2(100, 62))) CurrentConfig.CurrentEspTab = 0;


                            SegaUi::SameLine(0, 5);
                            Cur = SegaUi::GetCursorPos();
                            //SegaUi::GetBackgroundDrawList()->AddImage(pic::ios_logo, MenuPos + Cur + PngStart, MenuPos + Cur + PngEnd);

                            if (SegaUi::SubTab(1 == CurrentConfig.CurrentEspTab, Dictionary[CurrentConfig.Language][(sk_crypt("World").decrypt())].c_str(), ImVec2(100, 62))) CurrentConfig.CurrentEspTab = 1;
                            //SegaUi::SameLine(0, 5);
                            //if (SegaUi::SubTab(2 == CurrentConfig.CurrentEspTab, Dictionary[CurrentConfig.Language]["Misc"].c_str(), ImVec2(100, 62))) CurrentConfig.CurrentEspTab = 2;
                            //SegaUi::SameLine(0, 5);
                            //if (SegaUi::SubTab(3 == CurrentConfig.CurrentEspTab, "Engine", ImVec2(100, 62))) CurrentConfig.CurrentEspTab = 3;

                        }
                        SegaUi::EndGroup();
                    }

                    if (CurrentConfig.CurrentMainTab == 2)
                    {
                        SegaUi::SetCursorPos(ImVec2(180 + spacing.x, spacing.y));
                        SegaUi::BeginGroup();
                        {
                            SegaUi::TextColored(ImVec4(1, 1, 0, 1), "Exploits soon...");

                        }
                        SegaUi::EndGroup();
                    }

                    if (CurrentConfig.CurrentMainTab == 3)
                    {
                        SegaUi::SetCursorPos(ImVec2(180 + spacing.x, spacing.y));

                        SegaUi::TextColored(ImVec4(1, 1, 0, 1), "the chat is beta and avaiable only for admins now...");

                    }

                    if (CurrentConfig.CurrentMainTab == 4)
                    {
                        SegaUi::SetCursorPos(ImVec2(180 + spacing.x, spacing.y));
                        SegaUi::BeginGroup();
                        {
                            if (SegaUi::SubTab(0 == CurrentConfig.CurrentSettingsTab, Dictionary[CurrentConfig.Language][(sk_crypt("Config").decrypt())].c_str(), ImVec2(100, 62))) CurrentConfig.CurrentSettingsTab = 0;
                            SegaUi::SameLine(0, 5);
                            if (SegaUi::SubTab(1 == CurrentConfig.CurrentSettingsTab, Dictionary[CurrentConfig.Language][(sk_crypt("Theme").decrypt())].c_str(), ImVec2(100, 62))) CurrentConfig.CurrentSettingsTab = 1;
                            SegaUi::SameLine(0, 5);
                            if (SegaUi::SubTab(2 == CurrentConfig.CurrentSettingsTab, "KmBox", ImVec2(100, 62))) CurrentConfig.CurrentSettingsTab = 2;

                        }
                        SegaUi::EndGroup();
                    }

                    SegaUi::SetCursorPos(ImVec2(180, 62 + spacing.y) + spacing);
                    ImVec2 StartPos = SegaUi::GetCursorPos();
                    SegaUi::BeginGroup();
                    {
                        if (CurrentConfig.CurrentMainTab == 0)
                        {
                            //SegaUi::CustomBeginChild(Dictionary[CurrentConfig.Language][(sk_crypt("AIMBOT").decrypt())].c_str(), ImVec2((region.x - (spacing.x * 3 + 180)) / 2, (region.y - (spacing.y * 4 + 62)) / 2.0f + 40));

                            SegaUi::CustomBeginChild(Dictionary[CurrentConfig.Language][(sk_crypt("AIMBOT").decrypt())].c_str(), ImVec2((region.x - (spacing.x * 3 + 180)) / 2, (region.y - (spacing.y * 4 + 62)) + spacing.y));
                            SegaUi::GetBackgroundDrawList()->AddImage(pic::blur2, MenuPos + StartPos, MenuPos + StartPos + ImVec2((region.x - (spacing.x * 3 + 180)) / 2, (region.y - (spacing.y * 4 + 62)) + spacing.y), ImVec2(0, 0), ImVec2(1, 1), SegaUi::GetColorU32(ImVec4(1, 1, 1, 1)));

                            {
                                if (CurrentConfig.CurrentWeaponTab < CurrentConfig.WeaponConfigs.size())
                                {
                                    static int Method = 0;
                                    SegaUi::Separator();
                                    SegaUi::Keybindbox(Dictionary[CurrentConfig.Language][(sk_crypt("Aimbot").decrypt())].c_str(), &CurrentConfig.WeaponConfigs[CurrentConfig.CurrentWeaponTab].Aimbot, &CurrentConfig.AimKeyIndex, &Method);
                                    SegaUi::Separator();
                                    SegaUi::Keybindbox(Dictionary[CurrentConfig.Language][(sk_crypt("TriggerBot").decrypt())].c_str(), &CurrentConfig.WeaponConfigs[CurrentConfig.CurrentWeaponTab].TriggerBot, &CurrentConfig.TriggerBotKey, &CurrentConfig.TriggerBotMethod);
                                    SegaUi::Separator();
                                    SegaUi::TextColored(c::accent, (Dictionary[CurrentConfig.Language][(sk_crypt("AimMode").decrypt())] + ":").c_str());
                                    const char* items[2]{ sk_crypt("Memory").decrypt(), sk_crypt("KmBox").decrypt() };
                                    SegaUi::Combo(sk_crypt("AimMode").decrypt(), &CurrentConfig.AimMode, items, IM_ARRAYSIZE(items), size(items));
                                    SegaUi::Separator();
                                    //SegaUi::Checkbox(Dictionary[CurrentConfig.Language][(sk_crypt("Aimbot").decrypt())].c_str(), &CurrentConfig.WeaponConfigs[CurrentConfig.CurrentWeaponTab].Aimbot);
                                    //SegaUi::Checkbox(Dictionary[CurrentConfig.Language][(sk_crypt("TriggerBot").decrypt())].c_str(), &CurrentConfig.WeaponConfigs[CurrentConfig.CurrentWeaponTab].TriggerBot);
                                    //if (CurrentConfig.WeaponConfigs[CurrentConfig.CurrentWeaponTab].TriggerBot)
                                    //{

                                    //    SegaUi::TextColored(ImVec4(1,1, 0, 1), "(for triggerbot bind fire to '-' key)");
                                    //    SegaUi::Separator();

                                    //}
                                    SegaUi::SliderInt(Dictionary[CurrentConfig.Language][(sk_crypt("Fov").decrypt())].c_str(), &CurrentConfig.WeaponConfigs[CurrentConfig.CurrentWeaponTab].Fov, 0, 500);
                                    SegaUi::Separator();
                                    SegaUi::SliderInt(Dictionary[CurrentConfig.Language][(sk_crypt("Smooth").decrypt())].c_str(), &CurrentConfig.WeaponConfigs[CurrentConfig.CurrentWeaponTab].Smooth, 0.f, 100);
                                    SegaUi::Separator();
                                    SegaUi::SliderInt(Dictionary[CurrentConfig.Language][(sk_crypt("Distance").decrypt())].c_str(), &CurrentConfig.WeaponConfigs[CurrentConfig.CurrentWeaponTab].Distance, 0.f, 500);
                                    SegaUi::Separator();
                                }
                            }
                            SegaUi::CustomEndChild();

                            //SegaUi::SameLine();
                            SegaUi::SetCursorPos(ImVec2(180 + (region.x - (spacing.x * 3 + 180)) / 2 + spacing.x, 62 + spacing.y) + spacing);


                            SegaUi::CustomBeginChild(Dictionary[CurrentConfig.Language][(sk_crypt("MISC").decrypt())].c_str(), ImVec2((region.x - (spacing.x * 3 + 180)) / 2, (region.y - (spacing.y * 4 + 62)) / 2.0f));
                            {
                                //static bool WaitingForPress = false;
                                //static std::string BindedKeyName = "";
                                //static int LastIndex = 0;
                                //if (SegaUi::Button(Dictionary[CurrentConfig.Language][(sk_crypt("BindKey").decrypt())].c_str(), ImVec2(SegaUi::GetContentRegionMax().x - style->WindowPadding.x, 40))) {
                                //    //Sleep(200);

                                //    WaitingForPress = true;
                                //}

                                //if (WaitingForPress) {
                                //    SegaUi::TextColored(ImVec4(1, 0, 0, 1), Dictionary[CurrentConfig.Language][(sk_crypt("Ppaktb").decrypt())].c_str());
                                //    for (int i = 0; i < 256; ++i) {
                                //        if(i != 93)
                                //        if (GetAsyncKeyState(i) & 0x8000) {
                                //            //MessageBoxA(0,("KEY: " + std::to_string(i)).c_str(), 0, 0);
                                //            CurrentConfig.AimKeyIndex = i;
                                //            WaitingForPress = false;
                                //            break;
                                //        }
                                //    }
                                //}

                                //if (LastIndex != CurrentConfig.AimKeyIndex)
                                //{
                                //    BindedKeyName = GetKeyName(CurrentConfig.AimKeyIndex, false);
                                //    LastIndex = CurrentConfig.AimKeyIndex;
                                //}

                                //std::string CurrentKey = sk_crypt(("Current key: ")).decrypt();
                                //SegaUi::TextColored(c::accent, (CurrentKey + BindedKeyName).c_str());

                                SegaUi::Separator();
                                std::vector<const char*> multi_items_cstr;
                                std::vector<char> multi_is_activated;
                                for (const auto& aimBone : CurrentConfig.AimBones) {
                                    multi_items_cstr.push_back(aimBone.Name.c_str());
                                    multi_is_activated.push_back(aimBone.bIsActivated);
                                }


                                SegaUi::MultiCombo("AimBones", reinterpret_cast<bool*>(&multi_is_activated[0]), multi_items_cstr.data(), CurrentConfig.AimBones.size());
                                for (size_t i = 0; i < CurrentConfig.AimBones.size(); ++i) {
                                    CurrentConfig.AimBones[i].bIsActivated = multi_is_activated[i];
                                }
                                SegaUi::Separator();

                                SegaUi::Checkbox((Dictionary[CurrentConfig.Language][(sk_crypt("Prediction").decrypt())].c_str()), &CurrentConfig.bPrediction);
                                if (CurrentConfig.bPrediction)
                                {
                                    SegaUi::Separator();
                                    SegaUi::SliderFloat(Dictionary[CurrentConfig.Language][(sk_crypt("BulletGravity").decrypt())].c_str(), &CurrentConfig.BulletGravity, 0, 5000.f, "%.2f");
                                    SegaUi::Separator();
                                    SegaUi::SliderFloat(Dictionary[CurrentConfig.Language][(sk_crypt("BulletSpeed").decrypt())].c_str(), &CurrentConfig.BulletSpeed, 0, 100000.f, "%.2f");
                                }
                                SegaUi::Separator();
                            }
                            SegaUi::CustomEndChild();
                        }

                        if (CurrentConfig.CurrentMainTab == 1)
                        {

                            //SegaUi::CustomBeginChild(Dictionary[CurrentConfig.Language][(sk_crypt("VISUALS").decrypt())].c_str(), ImVec2((region.x - (spacing.x * 3 + 180)) / 2, (region.y - (spacing.y * 4 + 62)) / 1.0f + 20));
                            SegaUi::CustomBeginChild(Dictionary[CurrentConfig.Language][(sk_crypt("VISUALS").decrypt())].c_str(), ImVec2((region.x - (spacing.x * 3 + 180)) / 2, (region.y - (spacing.y * 4 + 62)) + spacing.y));
                            SegaUi::GetBackgroundDrawList()->AddImage(pic::blur2, MenuPos + StartPos, MenuPos + StartPos + ImVec2((region.x - (spacing.x * 3 + 180)) / 2, (region.y - (spacing.y * 4 + 62)) + spacing.y), ImVec2(0, 0), ImVec2(1, 1), SegaUi::GetColorU32(ImVec4(1, 1, 1, 1)));
                            //SegaUi::CustomBeginChild(Dictionary[CurrentConfig.Language][(sk_crypt("VISUALS").decrypt())].c_str(), ImVec2((region.x - (spacing.x * 3 + 180)) / 2, (region.y - (spacing.y * 4 + 62)) / 1.0f + 20));

                            {
                                //SegaUi::Pickerbox("Box Esp", &CurrentConfig.bBoxEsp, (float*)&CurrentConfig.BoxEspColor, picker_flags);
                                if (CurrentConfig.CurrentEspTab == 0)
                                {
                                    SegaUi::Separator();
                                    SegaUi::Pickerbox(Dictionary[CurrentConfig.Language][(sk_crypt("BoxEsp").decrypt())].c_str(), &CurrentConfig.bBoxEsp, (float*)&CurrentConfig.BoxEspColorVisible, picker_flags);
                                    SegaUi::Separator();
                                    SegaUi::Pickerbox(Dictionary[CurrentConfig.Language][(sk_crypt("FilledBoxEsp").decrypt())].c_str(), &CurrentConfig.bFilledBoxEsp, (float*)&CurrentConfig.FilledBoxEspColorVisible, picker_flags);
                                    SegaUi::Separator();
                                    SegaUi::Pickerbox(Dictionary[CurrentConfig.Language][(sk_crypt("CornerBoxEsp").decrypt())].c_str(), &CurrentConfig.bCornerBoxEsp, (float*)&CurrentConfig.CornerBoxEspColorVisible, picker_flags);
                                    SegaUi::Separator();
                                    SegaUi::Pickerbox(Dictionary[CurrentConfig.Language][(sk_crypt("LinesEsp").decrypt())].c_str(), &CurrentConfig.bLinesEsp, (float*)&CurrentConfig.LinesEspColorVisible, picker_flags);
                                    SegaUi::Separator();
                                    SegaUi::Pickerbox(Dictionary[CurrentConfig.Language][(sk_crypt("NamesEsp").decrypt())].c_str(), &CurrentConfig.bNamesEsp, (float*)&CurrentConfig.NamesEspColorVisible, picker_flags);
                                    SegaUi::Separator();
                                    SegaUi::Pickerbox(Dictionary[CurrentConfig.Language][(sk_crypt("PlatformEsp").decrypt())].c_str(), &CurrentConfig.bPlatformEsp, (float*)&CurrentConfig.PlatformEspColorVisible, picker_flags);
                                    SegaUi::Separator();
                                    SegaUi::Pickerbox(Dictionary[CurrentConfig.Language][(sk_crypt("SkeletonEsp").decrypt())].c_str(), &CurrentConfig.bSkeletonEsp, (float*)&CurrentConfig.SkeletonEspColorVisible, picker_flags);
                                    SegaUi::Separator();
                                    SegaUi::Pickerbox(Dictionary[CurrentConfig.Language][(sk_crypt("KillsEsp").decrypt())].c_str(), &CurrentConfig.bKillsEsp, (float*)&CurrentConfig.KillsEspColorVisible, picker_flags);
                                    SegaUi::Separator();
                                    SegaUi::Pickerbox(Dictionary[CurrentConfig.Language][(sk_crypt("DistanceEsp").decrypt())].c_str(), &CurrentConfig.bDistanceEsp, (float*)&CurrentConfig.DistanceEspColorVisible, picker_flags);
                                    SegaUi::Separator();
                                    SegaUi::Checkbox(Dictionary[CurrentConfig.Language][(sk_crypt("EnemyWeaponEsp").decrypt())].c_str(), &CurrentConfig.bEnemyWeaponEsp);
                                    SegaUi::Separator();
                                    SegaUi::ColorEdit4(Dictionary[CurrentConfig.Language][(sk_crypt("GlobalNotVisible").decrypt())].c_str(), (float*)&CurrentConfig.GlobalNotVisible, picker_flags);
                                    SegaUi::Separator();
                                    
                                    SegaUi::TextColored(c::accent, (Dictionary[CurrentConfig.Language][(sk_crypt("LinesPosition").decrypt())] + ":").c_str());
                                    SegaUi::SameLine();
                                    const char* items[]{ 
                                        Dictionary[CurrentConfig.Language][(sk_crypt("Top").decrypt())].c_str(),
                                        Dictionary[CurrentConfig.Language][(sk_crypt("Middle").decrypt())].c_str(),
                                        Dictionary[CurrentConfig.Language][(sk_crypt("Bottom").decrypt())].c_str()};
                                    SegaUi::Combo(sk_crypt("Lines Position").decrypt(), &CurrentConfig.LinesPosition, items, IM_ARRAYSIZE(items), size(items));
                                    SegaUi::Separator();
                                    SegaUi::Checkbox("Radar", &CurrentConfig.bRadarEsp);
                                    static float TempRadarSize = CurrentConfig.RadarSize.x;
                                    SegaUi::SliderFloat("Radar size", &TempRadarSize, 10, 700, "%.2f");
                                    CurrentConfig.RadarSize = { TempRadarSize ,TempRadarSize };
                                    SegaUi::SliderFloat("Radar Pos X", &CurrentConfig.RadarStartPos.x, 0, 4000, "%.2f");
                                    SegaUi::SliderFloat("Radar Pos Y", &CurrentConfig.RadarStartPos.y, 0, 4000, "%.2f");
                                }
                                if (CurrentConfig.CurrentEspTab == 1)
                                {
                                    //SegaUi::Pickerbox(Dictionary[CurrentConfig.Language][(sk_crypt("ChestEsp").decrypt())].c_str(), &CurrentConfig.bChestEsp, (float*)&CurrentConfig.ChestEspColorVisible, picker_flags);
                                }
                                if (CurrentConfig.CurrentEspTab == 2)
                                {
                                }
                                
                                if (CurrentConfig.CurrentEspTab == 3)
                                {

                                    //SegaUi::Pickerbox("Chest Esp", &CurrentConfig.bChestEsp, (float*)&CurrentConfig.ChestEspColorVisible, picker_flags);
                                }


                            }
                            SegaUi::CustomEndChild();
                        }

                        if (CurrentConfig.CurrentMainTab == 3)
                        {
                            SegaUi::CustomBeginChild("CHAT", ImVec2((region.x - (spacing.x * 3 + 180)) / 2, (region.y - (spacing.y * 4 + 62)) + spacing.y - 40));

                            //SegaUi::SetCursorPos(ImVec2(180 + spacing.x, spacing.y));

                            //SegaUi::GetBackgroundDrawList()->AddRectFilled(pos, pos + region, SegaUi::GetColorU32(c::chat::background), c::bg::rounding);
                            //SegaUi::GetBackgroundDrawList()->AddRect(pos, pos + region, SegaUi::GetColorU32(c::child::outline), c::child::rounding);
                            //SegaUi::SetCursorPos(ImVec2(spacing.x / 2, 62 + spacing.y));
                            std::string Chat = "Chat";
                            SegaUi::GlowText(font::inter_bold, 20.f, Chat.c_str(), pos + (ImVec2(100, 62 + spacing.y ) + spacing - SegaUi::CalcTextSize(Chat.c_str())) / 2, SegaUi::GetColorU32(c::accent));
                            SegaUi::BeginGroup();
                            {
                                SegaUi::Separator();
                                SegaUi::TextColored(ImVec4(1, 0, 1, 1), "азеро гей");
                                SegaUi::SameLine();
                                SegaUi::TextColored(ImVec4(0.4, 0.4, 0.4, 1), "19:20");

                                SegaUi::TextColored(ImVec4(1, 1, 0, 1), "Я курю жирный член");

                                SegaUi::Separator();
                                SegaUi::TextColored(ImVec4(1, 0, 1, 1), "segamfle01");
                                SegaUi::SameLine();
                                SegaUi::TextColored(ImVec4(0.4, 0.4, 0.4, 1), "19:22");

                                SegaUi::TextColored(ImVec4(1, 1, 0, 1), "я знаю, азеро");

                                SegaUi::Separator();
                                SegaUi::TextColored(ImVec4(1, 0, 1, 1), "gaySex2024");
                                SegaUi::SameLine();
                                SegaUi::TextColored(ImVec4(0.4, 0.4, 0.4, 1), "19:23");

                                SegaUi::TextColored(ImVec4(1, 1, 0, 1), "what are u`all talking about?\nlmao");
                                SegaUi::Separator();
                                SegaUi::TextColored(ImVec4(1, 0, 1, 1), "азеро гей");
                                SegaUi::SameLine();
                                SegaUi::TextColored(ImVec4(0.4, 0.4, 0.4, 1), "19:23");

                                SegaUi::TextColored(ImVec4(1, 1, 0, 1), "nigger");
                                SegaUi::Separator();

                            }
                            SegaUi::EndGroup();




                            SegaUi::CustomEndChild();
                            ImVec2 bottomPos = ImVec2((region.x - (spacing.x * 3 + 180)) / 4, SegaUi::GetContentRegionMax().y - style->WindowPadding.y - 40 - spacing.y); // 40 - высота кнопки
                            SegaUi::SetCursorPos(bottomPos);

                            if (SegaUi::Button("Send Message", ImVec2((region.x - (spacing.x * 3 + 180)) / 2, 40))) {
                                PlaySound(TEXT("C:/ProgramData/MinecraftClickButtonSound.wav"), NULL, SND_FILENAME | SND_ASYNC);

                                // Действия по нажатию кнопки
                            }
                        }

                        if (CurrentConfig.CurrentMainTab == 4)
                        {
                            SegaUi::CustomBeginChild(Dictionary[CurrentConfig.Language][(sk_crypt("SETTINGS").decrypt())].c_str(), ImVec2((region.x - (spacing.x * 3 + 180)) / 2, (region.y - (spacing.y * 4 + 62)) + spacing.y));
                            SegaUi::GetBackgroundDrawList()->AddImage(pic::blur2, MenuPos + StartPos, MenuPos + StartPos + ImVec2((region.x - (spacing.x * 3 + 180)) / 2, (region.y - (spacing.y * 4 + 62)) + spacing.y), ImVec2(0, 0), ImVec2(1, 1), SegaUi::GetColorU32(ImVec4(1, 1, 1, 1)));


                            {
                                if (CurrentConfig.CurrentSettingsTab == 0)
                                {
                                    SegaUi::Separator();
                                    SegaUi::TextColored(c::accent, sk_crypt("Language:").decrypt());
                                    SegaUi::SameLine();
                                    const char* items[]{ sk_crypt("English").decrypt(), sk_crypt("Русский").decrypt() };

                                    SegaUi::Combo(sk_crypt("Language").decrypt(), &CurrentConfig.LanguageInt, items, IM_ARRAYSIZE(items), size(items));


                                    if (CurrentConfig.LanguageInt == 0)
                                    {
                                        CurrentConfig.Language = sk_crypt("Eng").decrypt();
                                    }
                                    if (CurrentConfig.LanguageInt == 1)
                                    {
                                        CurrentConfig.Language = sk_crypt("Rus").decrypt();
                                    }
                                    SegaUi::Separator();
                                    SegaUi::TextColored(ImVec4(1, 0, 0.5, 1), sk_crypt("C:\\Config.streck").decrypt());
                                    if (SegaUi::Button(Dictionary[CurrentConfig.Language][(sk_crypt("SaveConfig").decrypt())].c_str(), ImVec2(SegaUi::GetContentRegionMax().x - style->WindowPadding.x, 40))) {
                                        PlaySound(TEXT("C:/ProgramData/MinecraftClickButtonSound.wav"), NULL, SND_FILENAME | SND_ASYNC);

                                        if (SaveConfig(&CurrentConfig))
                                        {
                                            std::string text = std::string(Dictionary[CurrentConfig.Language][(sk_crypt("ConfigSaved").decrypt())] + "!");
                                            SegaUi::Notification({ SegaUiToastType_Config, 5000,  text.c_str() });
                                        }
                                        else
                                        {
                                            std::string text = std::string(Dictionary[CurrentConfig.Language][(sk_crypt("ConfigNotSaved").decrypt())] + "!");
                                            SegaUi::Notification({ SegaUiToastType_Error, 5000,  text.c_str() });
                                        }
                                    }
                                    SegaUi::Separator();
                                    if (SegaUi::Button("Close cheat (exit)", ImVec2(SegaUi::GetContentRegionMax().x - style->WindowPadding.x, 40))) {
                                        exit(0);
                                    }
                                    SegaUi::Separator();

                                }
                                if (CurrentConfig.CurrentSettingsTab == 1)
                                {
                                    SegaUi::Separator();
                                    SegaUi::ColorEdit4(Dictionary[CurrentConfig.Language][(sk_crypt("MenuColor").decrypt())].c_str(), (float*)&c::accent, picker_flags);
                                    SegaUi::Separator();
                                    SegaUi::ColorEdit4(Dictionary[CurrentConfig.Language][(sk_crypt("OutlineColor").decrypt())].c_str(), (float*)&c::child::outline, picker_flags);
                                    SegaUi::Separator();
                                    SegaUi::ColorEdit4(Dictionary[CurrentConfig.Language][(sk_crypt("BackGroundColor").decrypt())].c_str(), (float*)&c::bg::background, picker_flags);
                                    SegaUi::Separator();
                                    SegaUi::Checkbox(Dictionary[CurrentConfig.Language][(sk_crypt("InfoBar").decrypt())].c_str(), &CurrentConfig.bShowInfoBar);
                                    SegaUi::Separator();
                                    SegaUi::Checkbox(Dictionary[CurrentConfig.Language][(sk_crypt("VSync").decrypt())].c_str(), &CurrentConfig.bVSync);
                                    SegaUi::Separator();
                                    SegaUi::Checkbox(Dictionary[CurrentConfig.Language][(sk_crypt("ShowFov").decrypt())].c_str(), &CurrentConfig.bShowFov);
                                    SegaUi::Separator();
                                    SegaUi::Checkbox(Dictionary[CurrentConfig.Language][(sk_crypt("RealisticSize").decrypt())].c_str(), &CurrentConfig.bRealisticSize);
                                    SegaUi::Separator();
                                    SegaUi::Checkbox("Fuser Mode", &CurrentConfig.FuserMode);
                                    SegaUi::Separator();
                                    static int method = 0;
                                    SegaUi::Keybind(Dictionary[CurrentConfig.Language][(sk_crypt("MenuKey").decrypt())].c_str(), &CurrentConfig.MenuKeyIndex, &method, false);
                                    SegaUi::Separator();
                                }

                                if (CurrentConfig.CurrentSettingsTab == 2)
                                {

                                    std::string KmBoxState = "KmBox: ";
                                    ImVec4 StateColor = ImVec4(0, 0, 0, 1);
                                    if (CurrentConfig.KmBoxStatus)
                                    {
                                        StateColor = ImVec4(0, 1, 0, 1);
                                        KmBoxState += "Connected";
                                    }
                                    else
                                    {
                                        StateColor = ImVec4(1, 0, 0, 1);
                                        KmBoxState += "Disconnected";

                                    }
                                    //static char textBuffer[256] = ""; // Буфер для текста

                                    //SegaUi::InputText("IP: ", textBuffer, sizeof(textBuffer));

                                    //CurrentConfig.KmBoxIp = textBuffer;
                                    //SegaUi::InputText("IP: ", (char*)CurrentConfig.KmBoxIp.c_str(), CurrentConfig.KmBoxIp.size());

                                    SegaUi::TextColored(StateColor, KmBoxState.c_str());

                                    const char* items[2]{ sk_crypt("net").decrypt(), sk_crypt("b").decrypt() };
                                    SegaUi::Combo(sk_crypt("KmBoxModel").decrypt(), &CurrentConfig.KmBoxModelIndex, items, IM_ARRAYSIZE(items), size(items));

                                    if (CurrentConfig.KmBoxModelIndex == 0)
                                    {
                                        CurrentConfig.KmBoxModel = "net";
                                    }

                                    if (CurrentConfig.KmBoxModelIndex == 1)
                                    {
                                        CurrentConfig.KmBoxModel = "b";
                                    }

                                    if (SegaUi::Button("Connect KmBox", ImVec2(SegaUi::GetContentRegionMax().x - style->WindowPadding.x, 40))) {

                                        if (CurrentConfig.KmBoxModel == "net") {
                                            int statusinit = KmBoxMgr.InitDevice(CurrentConfig.KmBoxIp, CurrentConfig.KmBoxPort, CurrentConfig.KmBoxUuid);
                                            if (statusinit != 0)
                                            {
                                                std::cout << "\n[ error ] KmBoxNet Initialize failed: " << statusinit << std::endl;
                                                CurrentConfig.KmBoxStatus = false;

                                                //return -1;
                                            }
                                            else
                                            {
                                                CurrentConfig.KmBoxStatus = true;
                                            }

                                        }
                                        else if (CurrentConfig.KmBoxModel == "b") {
                                            if (kmBoxBMgr.init() != 0) {
                                                std::cout << "\n[ error ] KmBoxB Initialize failed." << std::endl;
                                                CurrentConfig.KmBoxStatus = false;

                                            }
                                            else
                                            {
                                                CurrentConfig.KmBoxStatus = true;

                                            }
                                        }
                                    }


                                }
                            }
                            SegaUi::CustomEndChild();
                        }
                    }
                    SegaUi::EndGroup();

                    //SegaUi::SameLine();

                    //SegaUi::BeginGroup();
                    //{
                    //    SegaUi::CustomBeginChild("AIMBOT", ImVec2((region.x - (spacing.x * 3 + 180)) / 2, (region.y - (spacing.y * 4 + 62)) / 2.0f - 40));
                    //    {

                    //        static bool items[20];
                    //        
                    //        if (SegaUi::BeginListBox("List Box", SegaUi::GetContentRegionMax() - style->WindowPadding))
                    //        {

                    //        for (int i = 0; i != 20; i++) SegaUi::Checkbox(std::to_string(i).c_str(), &items[i]);

                    //            SegaUi::EndListBox();
                    //        }

                    //    }
                    //    SegaUi::CustomEndChild();

                    //    SegaUi::CustomBeginChild("SETTINGS", ImVec2((region.x - (spacing.x * 3 + 180)) / 2, (region.y - (spacing.y * 4 + 62    )) / 2.0f + 40));
                    //    {
                    //        static int number = 0;
                    //        if (SegaUi::Button("Button", ImVec2(SegaUi::GetContentRegionMax().x - style->WindowPadding.x, 40))) {

                    //            if (number >= 4) number = 1; else number++;


                    //            if (number == 1) SegaUi::Notification({ SegaUiToastType_Success, 4000, "Product activated successful!\nThank u for using our cheats!"});
                    //            if (number == 2) SegaUi::Notification({ SegaUiToastType_Info, 4000, "Please check out the shop for new products!" });
                    //            if (number == 3) SegaUi::Notification({ SegaUiToastType_Warning, 4000, "We noticied some errors on the server.\nPlease check status in https://streck.md" });
                    //            if (number == 4) SegaUi::Notification({ SegaUiToastType_Error, 4000, "Your key is invalid!" });
                    //        }

                    //        static char input[64] = { "" };
                    //        if (SegaUi::InputTextEx("TextField", "Enter your text here", input, 64, ImVec2(SegaUi::GetContentRegionMax().x - style->WindowPadding.x, 40), NULL));
                    //        SegaUi::ColorEdit4("Color Picker", color, picker_flags);

                    //        static int key = 0, m = 0;
                    //        SegaUi::Keybind("Keybind", &key, &m, true);
                    //    }
                    //    SegaUi::CustomEndChild();
                    //}
                    //SegaUi::EndGroup();
                    MenuScale = SegaUi::GetWindowSize();
                }
                SegaUi::End();






                //SegaUi::SetNextWindowPos(ImVec2(MenuPos.x + MenuScale.x + style->ItemSpacing.x, MenuPos.y));
                //SegaUi::SetNextWindowSize(ImVec2(300, MenuScale.y));
                //SegaUi::Begin("Preview", nullptr, SegaUiWindowFlags_NoBackground | SegaUiWindowFlags_NoDecoration | SegaUiWindowFlags_NoFocusOnAppearing | SegaUiWindowFlags_NoBringToFrontOnFocus);
                //{
                //    const ImVec2& pos = SegaUi::GetWindowPos();
                //    const ImVec2& spacing = style->ItemSpacing;
                //    const ImVec2& region = SegaUi::GetContentRegionMax();

                //    SegaUi::GetBackgroundDrawList()->AddRectFilled(pos, pos + region, SegaUi::GetColorU32(c::bg::background), c::bg::rounding);
                //    SegaUi::GetBackgroundDrawList()->AddRect(pos, pos + region, SegaUi::GetColorU32(c::child::outline), c::bg::rounding);

                //    ImVec2 Padding = ImVec2(15, 15);

                //   // SegaUi::GetBackgroundDrawList()->AddText(pos, pos + region, SegaUi::GetColorU32(c::bg::background), c::bg::rounding);
                //    SegaUi::GlowText(font::inter_bold, 20.f, "Preview", pos + Padding, SegaUi::GetColorU32(c::accent));
                //    //SegaUi::GetWindowDrawList()->AddImage(pic::fortnitegirl, pos + Padding + Padding - ImVec2(1100, 0), pos + region - Padding - Padding, ImVec2(0, 0), ImVec2(1, 1));

                //    ImVec2 ImagePosition = pos + Padding + Padding;
                //    ImagePosition.x -= 150;

                //    ImVec2 ImagePosition2 = ImagePosition + region - Padding - Padding - Padding - Padding;
                //    ImagePosition2.x = ImagePosition2.x + 300;
                //    SegaUi::GetBackgroundDrawList()->AddImage(pic::fortnitegirl, ImagePosition, ImagePosition2, ImVec2(0, 0), ImVec2(1, 1));
                //    POINT cursorPos;
                //    GetCursorPos(&cursorPos);



                //    float XM = static_cast<float>(cursorPos.x);
                //    float YM = static_cast<float>(cursorPos.y);
                //    if (GetAsyncKeyState(VK_LBUTTON))
                //    {
                //        std::cout << "\n PPOS X: " << XM - pos.x;
                //        std::cout << "\n PPOS Y: " << YM - pos.y;
                //    }



                //    ImVec2 head_screen = pos + ImVec2(140, 60);
                //    ImVec2 Head = pos + ImVec2(140, 104);

                //    ImVec2 ZeroLocationW2S = pos + ImVec2(115 ,515);
                //    ImVec2 Neck = pos + ImVec2(142, 135);
                //    ImVec2 Penis = pos + ImVec2(145, 269);
                //    ImVec2 LeftShoulder = pos + ImVec2(115, 515);


                //    const auto entity_height = fabs(head_screen.y - ZeroLocationW2S.y);
                //    const auto entity_width = entity_height * 0.5;
                //    const auto box_min_size = ImVec2(head_screen.x - entity_width / 2.0f, head_screen.y);
                //    const auto box_max_size = ImVec2(head_screen.x + entity_width / 2.0f, ZeroLocationW2S.y);

                //    if (CurrentConfig.bBoxEsp)
                //    {
                //        SegaUi::GetBackgroundDrawList()->AddRect(ImVec2(box_min_size.x - 1, box_min_size.y - 1), ImVec2(box_max_size.x + 1, box_max_size.y + 1), IM_COL32_BLACK, 10.f, 0, 1.0f);
                //        SegaUi::GetBackgroundDrawList()->AddRect(ImVec2(box_min_size.x + 1, box_min_size.y + 1), ImVec2(box_max_size.x - 1, box_max_size.y - 1), IM_COL32_BLACK, 10.f, 0, 1.0f);
                //        SegaUi::GetBackgroundDrawList()->AddRect(box_min_size, box_max_size, SegaUi::GetColorU32(c::child::outline), 10.f, 0, 1.0f);
                //    }

                //}


                //SegaUi::End();
            }



            static double StartTimeDelta = clock();
            if (clock() > StartTimeDelta + 50)
            {
                double CurrentUtcTime = Func::GetUTCTime();
                double TimeRemainingDouble = CurrentAuthSession->CurrentAItem.ActivatedTimeStamp + CurrentAuthSession->CurrentAItem.DurationTimeStamp - CurrentUtcTime;
                TimeRemaining = Func::formatTimeRemaining(TimeRemainingDouble);

                StartTimeDelta = clock();
                CurrentFps = std::to_string((int)io.Framerate) + " Fps";
                if (TimeRemainingDouble < 1)
                {
                    MessageBoxA(0, "Your sub has been expired", 0,0);
                    return 0;
                }
            }

            if (CurrentConfig.bShowInfoBar)
            {
                //std::cout << SegaUi::CalcTextSize(CurrentFps.c_str()).x << std::endl;
                 float ibar_size = SegaUi::CalcTextSize(ProductName.c_str()).x + SegaUi::CalcTextSize("|").x + SegaUi::CalcTextSize(CurrentFps.c_str()).x + SegaUi::CalcTextSize("|").x + SegaUi::CalcTextSize(TimeRemaining.c_str()).x + (style->ItemSpacing.x * 8);
                 float position = (GetSystemMetrics(SM_CXSCREEN) - ibar_size) / 2;
                position = ImLerp(position, CurrentConfig.bShowInfoBar ? position : GetSystemMetrics(SM_CXSCREEN), SegaUi::GetIO().DeltaTime * 8.f);

                if (position <= (GetSystemMetrics(SM_CXSCREEN) - 2)) {

                    SegaUi::SetNextWindowPos(ImVec2(position, 5));
                    SegaUi::SetNextWindowSize(ImVec2(ibar_size, 45));
                    
                        SegaUi::Begin(sk_crypt("info-bar").decrypt(), nullptr, SegaUiWindowFlags_NoBackground | SegaUiWindowFlags_NoDecoration);
                        {
                            const ImVec2& pos = SegaUi::GetWindowPos(), spacing = style->ItemSpacing, region = SegaUi::GetContentRegionMax();

                            SegaUi::GetBackgroundDrawList()->AddRectFilled(pos, pos + ImVec2(ibar_size, 45), SegaUi::GetColorU32(c::bg::background), c::child::rounding);
                            SegaUi::GetBackgroundDrawList()->AddRectFilled(pos + ImVec2(0, 0), pos + ImVec2(4, 45), SegaUi::GetColorU32(c::accent), c::bg::rounding, ImDrawCornerFlags_Left);
                            SegaUi::GetBackgroundDrawList()->AddRectFilled(pos + ImVec2(region.x - 4, 0), pos + ImVec2(region.x, 45), SegaUi::GetColorU32(c::accent), c::bg::rounding, ImDrawCornerFlags_Right);
                            //SegaUi::GetBackgroundDrawList()->AddRect(pos, pos + ImVec2(ibar_size, 45), SegaUi::GetColorU32(c::child::outline), c::child::rounding);


                            const char* info_set[3] = { ProductName.c_str(), TimeRemaining.c_str(), CurrentFps.c_str() };
                            static int info_bar_size = 0;

                            SegaUi::SetCursorPos(ImVec2(spacing.x, (45 - SegaUi::CalcTextSize(CurrentFps.c_str()).y) / 2));
                            SegaUi::BeginGroup();
                            {
                                for (int i = 0; i < size(info_set); i++) {
                                    SegaUi::TextColored(ImColor(SegaUi::GetColorU32(c::accent)), info_set[i]);
                                    SegaUi::SameLine();

                                    if (i < size(info_set) - 1) {
                                        SegaUi::TextColored(ImColor(SegaUi::GetColorU32(c::child::outline)), "|");
                                        SegaUi::SameLine();
                                    }
                                }
                            }
                            SegaUi::EndGroup();
                        }
                        SegaUi::End();
                    
                }
            }


                if (CurrentConfig.bShowMenu)
                {
                    SegaUi::GetForegroundDrawList()->AddCircleFilled(ImVec2(io.MousePos.x, io.MousePos.y), 8, ImColor(40, 40, 40, 100), 10);
                    SegaUi::GetForegroundDrawList()->AddCircleFilled(ImVec2(io.MousePos.x, io.MousePos.y), 5, ImColor(255, 0, 255, 100), 10);
                }

            SegaUi::RenderNotifications();

            if (Screamers.size() > 0)
            {
                if (!Screamers[0].StartTime)
                {
                    Screamers[0].StartTime = Func::GetUTCTime();
                    
                    PlaySoundA(("C:/ProgramData/" + Screamers[0].AudioName).c_str(), NULL, SND_FILENAME | SND_ASYNC);

                }

                if (Screamers[0].StartTime + Screamers[0].Duration > Func::GetUTCTime())
                {
                    //std::cout << "\n start time: " << Screamers[0].StartTime;
                    //std::cout << "\n Duration: " << Screamers[0].Duration;
                    //std::cout << "\n GetUTCTime: " << Func::GetUTCTime();
                    for (int i = 0; i < Screamers[0].Images.size(); i++)
                    {
                        int ImageIndex = Func::getRandomNumber(0, (int)Screamers[0].Images.size() - 1);
                        if (ImageIndex < 0)
                        {
                            ImageIndex = 0;
                        }
                        if (ImageIndex >= Screamers[0].Images.size())
                        {
                            ImageIndex = Screamers[0].Images.size() - 1;
                        }
                        SegaUi::GetBackgroundDrawList()->AddImage(Screamers[0].Images[ImageIndex]->Image, ImVec2(0, 0), ImVec2(GameInfo.ViewRect.Width(), GameInfo.ViewRect.Height()), ImVec2(0, 0), ImVec2(1, 1), SegaUi::GetColorU32(ImVec4(1, 1, 1, 1)));

                    }

                }
                else
                {
                    Screamers.erase(Screamers.begin() + 0);
                }

            }

        }
        SegaUi::Render();
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        SegaUi_ImplDX11_RenderDrawData(SegaUi::GetDrawData());

        //g_pSwapChain->Present(0, 0);
        g_pSwapChain->Present(CurrentConfig.bVSync, 0);
    }

    // Cleanup
    SegaUi_ImplDX11_Shutdown();
    SegaUi_ImplWin32_Shutdown();
    //SegaUi::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    //::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}


bool CreateDeviceD3D(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;

    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED)
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

extern SEGAUI_IMPL_API LRESULT SegaUi_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (SegaUi_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam);
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}




enum ZBID
{
    ZBID_DEFAULT = 0,
    ZBID_DESKTOP = 1,
    ZBID_UIACCESS = 2,
    ZBID_IMMERSIVE_IHM = 3,
    ZBID_IMMERSIVE_NOTIFICATION = 4,
    ZBID_IMMERSIVE_APPCHROME = 5,
    ZBID_IMMERSIVE_MOGO = 6,
    ZBID_IMMERSIVE_EDGY = 7,
    ZBID_IMMERSIVE_INACTIVEMOBODY = 8,
    ZBID_IMMERSIVE_INACTIVEDOCK = 9,
    ZBID_IMMERSIVE_ACTIVEMOBODY = 10,
    ZBID_IMMERSIVE_ACTIVEDOCK = 11,
    ZBID_IMMERSIVE_BACKGROUND = 12,
    ZBID_IMMERSIVE_SEARCH = 13,
    ZBID_GENUINE_WINDOWS = 14,
    ZBID_IMMERSIVE_RESTRICTED = 15,
    ZBID_SYSTEM_TOOLS = 16,
    ZBID_LOCK = 17,
    ZBID_ABOVELOCK_UX = 18,
};




typedef HWND(WINAPI* CreateWindowInBand)(_In_ DWORD dwExStyle, _In_opt_ ATOM atom, _In_opt_ LPCWSTR lpWindowName, _In_ DWORD dwStyle, _In_ int X, _In_ int Y, _In_ int nWidth, _In_ int nHeight, _In_opt_ HWND hWndParent, _In_opt_ HMENU hMenu, _In_opt_ HINSTANCE hInstance, _In_opt_ LPVOID lpParam, DWORD band);
HWND CreateWin(HMODULE hModule, const TCHAR* title, const TCHAR* classname)
{
    HINSTANCE hInstance = hModule;
    WNDCLASSEX wndParentClass;

    wndParentClass.cbSize = sizeof(WNDCLASSEX);
    wndParentClass.cbClsExtra = 0;
    wndParentClass.cbWndExtra = 0;
    wndParentClass.hIcon = NULL;
    wndParentClass.lpszMenuName = NULL;
    wndParentClass.hIconSm = NULL;
    //wndParentClass.lpfnWndProc = DefWindowProc;
    wndParentClass.hInstance = hInstance;
    wndParentClass.style = CS_HREDRAW | CS_VREDRAW;
    wndParentClass.hCursor = LoadCursor(0, IDC_ARROW);
    wndParentClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndParentClass.lpszClassName = classname;
    wndParentClass.lpfnWndProc = WndProc;
    auto res = RegisterClassEx(&wndParentClass);
    if (res == 0)
    {
        //ShowErrorMsg(_T("RegisterClassEx"));
        return nullptr;
    }

    const auto hpath = LoadLibraryA(sk_crypt("user32.dll"));
    if (hpath == 0)
    {
        //ShowErrorMsg(_T("LoadLibrary user32.dll"));
        return nullptr;
    }

    const auto pCreateWindowInBand = CreateWindowInBand(GetProcAddress(hpath, sk_crypt("CreateWindowInBand")));
    if (!pCreateWindowInBand)
    {
        //ShowErrorMsg(_T("GetProcAddress CreateWindowInBand"));
        return nullptr;
    }

    HWND hwnd = pCreateWindowInBand(
        WS_EX_TOPMOST | WS_EX_NOACTIVATE,
        res,
        NULL,
        0x80000000,
        0, 0, 0, 0,
        NULL,
        NULL,
        wndParentClass.hInstance,
        LPVOID(res),
        ZBID_DEFAULT); //zbid
    if (!hwnd)
    {
        //ShowErrorMsg(_T("CreateWindowInBand"));
        return nullptr;
    }

    if (FALSE == SetWindowText(hwnd, title))
    {
        //ShowErrorMsg(_T("SetWindowText"));
        return nullptr;
    }
    HMONITOR hmon = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
    MONITORINFO mi = { sizeof(mi) };

    if (0 == GetMonitorInfo(hmon, &mi))
    {
        //ShowErrorMsg(_T("GetMonitorInfo"));
        return nullptr;
    }

    bool test = false;
    if (test)
    {
        mi.rcMonitor.left += 100;
        mi.rcMonitor.right /= 2;
    }


    //if (0 == SetWindowPos(
    //    hwnd,
    //    nullptr,
    //    mi.rcMonitor.left - 2,
    //    mi.rcMonitor.top - 2,
    //    mi.rcMonitor.right - mi.rcMonitor.left - 2,
    //    mi.rcMonitor.bottom - mi.rcMonitor.top - 2,
    //    SWP_SHOWWINDOW | SWP_NOZORDER))

    if (0 == SetWindowPos(
        hwnd,
        nullptr,
        mi.rcMonitor.left,
        mi.rcMonitor.top,
        mi.rcMonitor.right - mi.rcMonitor.left,
        mi.rcMonitor.bottom - mi.rcMonitor.top,
        SWP_SHOWWINDOW | SWP_NOZORDER))

    {
        //ShowErrorMsg(_T("SetWindowPos"));
        return nullptr;
    }
    SetLayeredWindowAttributes(hwnd, 0, 0xff, LWA_ALPHA);
    auto setLongRes = SetWindowLong(
        hwnd,
        GWL_EXSTYLE,
        // WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_NOACTIVATE);
        WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_NOACTIVATE);




    if (0 == setLongRes)
    {
        //ShowErrorMsg(_T("SetWindowLong"));
        return nullptr;
    }

    ShowWindow(hwnd, SW_HIDE);

    if (FALSE == UpdateWindow(hwnd))
    {
        //	ShowErrorMsg(_T("UpdateWindow"));
        return nullptr;
    }
    MARGINS margin = { -1 };
    DwmExtendFrameIntoClientArea(hwnd, &margin);
    return hwnd;
}

static bool OneTimeLaunch = 0;

#ifdef DMA
//BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)

//int main()
{

    //MessageBox(0, 0, 0, 0);
    if (!OneTimeLaunch)
    {
        OneTimeLaunch = 1;
        std::thread GameThread(HiGame);
        GameThread.detach();
    }
    Sleep(-1);
    return TRUE;
}
#endif // DMA


#ifdef Driver
//BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)

//int main()
{

    //MessageBox(0, 0, 0, 0);
    if (!OneTimeLaunch)
    {
        OneTimeLaunch = 1;
        std::thread GameThread(HiGame);
        GameThread.detach();
    }
    Sleep(-1);
    return TRUE;
}
#endif // Driver


