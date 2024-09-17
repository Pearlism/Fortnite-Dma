#pragma once
#include <iostream>
#include <vector>
#include <thread>
#include <Windows.h>
#include <unordered_map>
#include <iostream>
#include <chrono>

#include "../json.hpp"
#include <filesystem>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <limits>
#include <climits>
#include <random>
#include <iostream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <iostream>
#include <cwctype>
#include <clocale>
#include <iostream>
#include <string>
#include <locale>
#include <codecvt>
#include <iostream>
#include <fstream>
#include <Shlwapi.h> 
#include <thread>
#pragma comment(lib, "Winmm.lib") 


#include <string>
#include <Windows.h>
#include <regex>
#include <cstdlib>
#include <ctime>
#include <codecvt>
#include <locale>
#include <vector>
#include <map>
#include <cctype> 
#include <algorithm>
#include <unordered_map>
#include <sstream>
#include <limits>
#include <climits>
#include <random>

#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#include "../Driver/mem.h"

WSADATA wsaData;
int iResult;
sockaddr_in clientService;
extern GameInfoStruct GameInfo;
extern ID3D11Device* g_pd3dDevice;
namespace Func
{
    std::string formatTimeRemaining(double timeRemainingSeconds) {
        int days = static_cast<int>(timeRemainingSeconds / (24 * 60 * 60));
        int hours = static_cast<int>((timeRemainingSeconds - days * 24 * 60 * 60) / (60 * 60));
        int minutes = static_cast<int>((timeRemainingSeconds - days * 24 * 60 * 60 - hours * 60 * 60) / 60);
        int seconds = static_cast<int>(timeRemainingSeconds - days * 24 * 60 * 60 - hours * 60 * 60 - minutes * 60);

        return std::to_string(days) + " Days, " + std::to_string(hours) + " Hours, " + std::to_string(minutes) + " Minutes, " + std::to_string(seconds) + " Seconds";
    }

    std::time_t GetUTCTime() {
        auto now = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);
        return now_c;
    }

    bool CompareStrings(const std::string& str1, const std::string& str2) {
        return str1.compare(str2) == 0;
    }

    char toLower(char c) {
        return std::tolower(static_cast<unsigned char>(c));
    }

    bool CompareStringsIgnoreCase(const std::string& str1, const std::string& str2) {
        if (str1.size() != str2.size()) {
            return false;
        }

        return std::equal(str1.begin(), str1.end(), str2.begin(), str2.end(),
            [](char c1, char c2) {
                return toLower(c1) == toLower(c2);
            });
    }

    std::string generateRandomString(int length) {
        const char* charset = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        const int charsetSize = strlen(charset);
        std::random_device rnd;
        std::mt19937_64 gen(rnd());
        std::uniform_int_distribution<int> dis(0, charsetSize - 1);
        std::string randomString;
        for (int i = 0; i < length; ++i) {
            char randomChar = charset[dis(gen)];
            randomString += randomChar;
        }
        return randomString;
    }

    std::string hexRepresentation(const std::string& str) {
        std::string hexStr;
        for (char c : str) {
            hexStr += "0x" + std::to_string(static_cast<unsigned char>(c)) + " ";
        }
        return hexStr;
    }

    std::string removeControlCharacters(const std::string& str) {
        std::string cleanedStr;
        for (char c : str) {
            if (!std::iscntrl(static_cast<unsigned char>(c))) {
                cleanedStr += c;
            }
        }
        return cleanedStr;
    }

    bool ContainsOnly_Letters(const std::wstring& wstr) {
        std::setlocale(LC_ALL, "");

        for (wchar_t const& c : wstr) {
            if (!std::iswalpha(c)) {
                return false;
            }
        }

        return true;
    }

    bool IsValidEmail(const std::string& email) {
        std::regex emailRegex(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
        return std::regex_match(email, emailRegex);
    }

    bool ContainsOnlyAllowedCharacters(const std::string& str) {
        std::regex allowedCharsRegex(R"([a-zA-Z0-9!@#$%^&*()\-_=+{}[\]|;:'",.<>?/`~]+)");


        return std::regex_match(str, allowedCharsRegex);
    }

    bool ContainsOnlyNativeCharacters(const std::string& str) {
        std::regex allowedCharsRegex(R"([a-zA-Z0-9]+)");
        return std::regex_match(str, allowedCharsRegex);
    }

    std::string readFile(std::string filePath) {
        std::ifstream file(filePath, std::ios::binary);

        if (!file.is_open())
        {
            //std::cerr << "Error opening file: " << filePath << std::endl;
            file.close();
            return "";
        }

        std::ostringstream content;
        content << file.rdbuf();
        file.close();
        return content.str();
    }

    std::string ConvertUTF16toUTF8(const wchar_t* wideString)
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        return converter.to_bytes(wideString);
    }

    bool endsWith(const std::string& fullString, const std::string& ending) {
        if (fullString.length() >= ending.length()) {
            return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
        }
        else {
            return false;
        }
    }

    template <typename T>
    T getRandomNumber(T min, T max)
    {
        std::random_device rnd;
        std::mt19937_64 gen(rnd());

        if constexpr (std::is_floating_point_v<T>)
        {
            std::uniform_real_distribution<T> dis(min, max);
            return dis(gen);
        }
        else if constexpr (std::is_integral_v<T>)
        {
            std::uniform_int_distribution<T> dis(min, max);
            return dis(gen);
        }
        else
        {
        }
    }

    std::string GetHwid()
    {
        std::string result;
        DWORD dwTempSize = 32767;
        char szTemp[32767];
        GetEnvironmentVariableA("PROCESSOR_REVISION", szTemp, dwTempSize);
        result += szTemp;
        WCHAR buffer[MAX_PATH];
        DWORD bufferSize = sizeof(buffer);
        HKEY reg_key;
        if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"HARDWARE\\DESCRIPTION\\System", 0, KEY_READ, &reg_key) == ERROR_SUCCESS) {
            if (RegQueryValueExW(reg_key, L"SystemBiosVersion", nullptr, nullptr, (LPBYTE)buffer, &bufferSize) == ERROR_SUCCESS) {
                std::wstring wbuf = std::wstring(buffer);
                result += std::string(wbuf.begin(), wbuf.end());
            }
            RegCloseKey(reg_key);
        }
        return result;
    }

}
struct RequestHeading {
    std::string Method;
    std::string Path;
    std::string Protocol;
    std::unordered_map<std::string, std::string> Headers;
    std::string Body;

    std::string dump() const {
        std::string request = Method + " " + Path + " " + Protocol + "\r\n";

        for (const auto& header : Headers) {
            request += header.first + ": " + header.second + "\r\n";
        }

        if (!Body.empty()) {
            request += "\r\n" + Body;
        }
        request += "\r\n";

        return request;
    }

    std::string dumpHeadersOnly() const {
        std::string request = Method + " " + Path + " " + Protocol + "\r\n";

        for (const auto& header : Headers) {
            request += header.first + ": " + header.second + "\r\n";
        }
        request += "\r\n";

        return request;
    }
};



RequestHeading parseRequest(const std::string& httpRequest) {
    RequestHeading parsedRequest;
    size_t requestLineEnd = httpRequest.find("\r\n");
    if (requestLineEnd == std::string::npos) {
        return parsedRequest;
    }
    std::istringstream requestLineStream(httpRequest.substr(0, requestLineEnd));
    requestLineStream >> parsedRequest.Method >> parsedRequest.Path >> parsedRequest.Protocol;
    size_t headersEnd = httpRequest.find("\r\n\r\n", requestLineEnd);
    if (headersEnd == std::string::npos) {
        return parsedRequest;
    }
    size_t headerStart = requestLineEnd + 2;
    std::istringstream headersStream(httpRequest.substr(headerStart, headersEnd - headerStart));
    std::string header;
    while (std::getline(headersStream, header, '\n')) {
        size_t colonPos = header.find(':');
        if (colonPos != std::string::npos) {
            std::string headerName = header.substr(0, colonPos);
            std::string headerValue = header.substr(colonPos + 2);
            parsedRequest.Headers[headerName] = Func::removeControlCharacters(headerValue);
        }
    }

    if (headersEnd + 4 < httpRequest.size()) {
        parsedRequest.Body = Func::removeControlCharacters(httpRequest.substr(headersEnd + 4));
    }

    return parsedRequest;
}

std::string GetHeaderValue(const std::unordered_map<std::string, std::string>& Headers, const std::string& key) {
    auto it = Headers.find(key);
    if (it != Headers.end())
    {
        return it->second;
    }
    return "";
}

bool SendResponse(SOCKET clientSocket, const std::string& Data) {
    int ChunkSize = 1024 * 4;
    int TotalBytesSent = 0;

    while (TotalBytesSent < Data.size()) {
        int BytesToSend = (ChunkSize < static_cast<int>(Data.size()) - TotalBytesSent) ? ChunkSize : (static_cast<int>(Data.size()) - TotalBytesSent);

        int BytesSent = send(clientSocket, Data.c_str() + TotalBytesSent, BytesToSend, 0);

        if (BytesSent < 0) {
            return false;
        }

        TotalBytesSent += BytesSent;

    }
    return true;
}

RequestHeading SendPostRequest(RequestHeading RequestHeaders)
{
    RequestHeading Response;
    Response.Method = "None";



    SOCKET ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);


    iResult = connect(ConnectSocket, (SOCKADDR*)&clientService, sizeof(clientService));
    if (iResult == SOCKET_ERROR) {
        ////std::cerr << "connect failed: " << WSAGetLastError() << std::endl;
        closesocket(ConnectSocket);
        WSACleanup();
        return Response;
    }

    if (!SendResponse(ConnectSocket, RequestHeaders.dumpHeadersOnly().c_str()))
    {
        closesocket(ConnectSocket);
        return Response;
    }
    if (!SendResponse(ConnectSocket, RequestHeaders.Body.c_str()))
    {
        closesocket(ConnectSocket);
        return Response;
    }

    //send(ConnectSocket, RequestHeaders.dumpHeadersOnly().c_str(), RequestHeaders.dumpHeadersOnly().size(), 0);
    //send(ConnectSocket, RequestHeaders.Body.c_str(), RequestHeaders.Body.size(), 0);

    std::vector<char> buffer(4096 * 4096);

    int bytesRead = recv(ConnectSocket, buffer.data(), buffer.size(), 0);
    Response = parseRequest(std::string(buffer.data(), buffer.size()));
    std::string ContentLength = (GetHeaderValue(Response.Headers, "Content-Length"));
    // std::vector<char> BodyBuffer(std::stoi(ContentLength));

    if (Response.Body.size() <= 0 && ContentLength.size())
    {
        int contentLength = std::stoi(ContentLength);

        std::vector<char> body(contentLength);
        int totalBytesRead = 0;

        while (totalBytesRead < contentLength) {
            int bytesRead = recv(ConnectSocket, body.data() + totalBytesRead, contentLength - totalBytesRead, 0);

            if (bytesRead <= 0) {
                break;
            }

            totalBytesRead += bytesRead;
        }

        Response.Body = std::string(body.data(), totalBytesRead);
    }

    closesocket(ConnectSocket);

    return Response;
}

struct FScreamerEffect
{

};

struct FImageData {
    std::vector<uint8_t> ImageRaw;
    ID3D11ShaderResourceView* Image = nullptr;
    std::string ImageName = "";
};

struct FScreamer
{
    int64_t StartTime = 0;
    int64_t Duration = 0;
    std::string AudioName = "";
    std::string ScreamerName = "";


    std::vector<FImageData*> Images;

    FScreamerEffect Effect;
};

std::vector<FScreamer> Screamers = {

};

std::vector<FScreamer> ScreamersPresets = {
};



struct AuthFrontEnd
{
    struct AItem {
        int64_t ItemId = 0;
        int64_t OwnerId = 0;
        std::string ItemName = "";
        std::string IconUrl = "";
        std::string Description = "";
        std::string LicenseKey = "";
        std::string Hwid = "";
        double GlobalPrice = 0;
        double UserPrice = 0;
        int64_t CreatedTimeStamp = 0;
        int64_t ActivatedTimeStamp = 0;
        int64_t DurationTimeStamp = 0;
        int64_t LastResetTimeStamp = 0;
        bool bIsTempExistItem = false;

    };



    std::string ApiVersion = "1.0";
    std::string ItemName = "";
    std::string SessionId = "";
    int64_t UserId = 0;
    std::string UserName = "";
    std::string Password = "";
    double Balance = 0;
    int64_t ExperiencePoint = 0;
    AItem CurrentAItem;
    std::vector<AItem> Inventory;

    bool SSEActive = false;

    std::string LastError = "";



    void SEEHandle(std::vector<char> Buffer, size_t bytesRead)
    {

        std::string jsonString(Buffer.data(), bytesRead);  // Используем только прочитанные байты
        size_t pos = jsonString.find("{");
        if (pos != std::string::npos)
        {
            std::string jsonStr = jsonString.substr(pos);
            try
            {
                nlohmann::json jsonData = nlohmann::json::parse(jsonStr);
                std::string type = jsonData["Type"];

                

                if (Func::CompareStringsIgnoreCase(type, "Screamer")) {
                    std::string ScreamerName = jsonData["ScreamerName"];
                    {
                        for (int i = 0; i < ScreamersPresets.size(); i++)
                        {
                            if (Func::CompareStringsIgnoreCase(ScreamersPresets[i].ScreamerName, ScreamerName))
                            {
                                FScreamer TempScreamer = ScreamersPresets[i];
                                D3DX11_IMAGE_LOAD_INFO info; ID3DX11ThreadPump* pump{ nullptr };
                                for (int64_t ii = 0;ii < TempScreamer.Images.size(); ii++)
                                {
                                    D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, TempScreamer.Images[ii]->ImageRaw.data(), TempScreamer.Images[ii]->ImageRaw.size(), &info, pump, &TempScreamer.Images[ii]->Image, 0);
                                    //std::cout << "\n TempScreamer.Images[ii].Image: " << TempScreamer.Images[ii]->Image;
                                    //std::cout << "\n TempScreamer.Images[ii].ImageBuffer: " << TempScreamer.Images[ii]->ImageRaw.data();
                                    //std::cout << "\n TempScreamer.Images[ii].ImageBufferSize: " << TempScreamer.Images[ii]->ImageRaw.size();

                                }
                                Screamers.push_back(TempScreamer);
                            }
                        }
                    }
                }
                if (Func::CompareStringsIgnoreCase(type, "NewNotification"))
                {
                    std::string notificationType = jsonData["NotificationType"];
                    std::string text = jsonData["Text"];

                    SegaUiToastType_ CurrentNot;
                    // Упрощенный способ присваивания значения CurrentNot
                    if (Func::CompareStringsIgnoreCase(notificationType, "Type_Success"))
                        CurrentNot = SegaUiToastType_Success;
                    else if (Func::CompareStringsIgnoreCase(notificationType, "Type_Warning"))
                        CurrentNot = SegaUiToastType_Warning;
                    else if (Func::CompareStringsIgnoreCase(notificationType, "Type_Error"))
                        CurrentNot = SegaUiToastType_Error;
                    else if (Func::CompareStringsIgnoreCase(notificationType, "Type_Info"))
                        CurrentNot = SegaUiToastType_Info;
                    else if (Func::CompareStringsIgnoreCase(notificationType, "Type_Saved"))
                        CurrentNot = SegaUiToastType_Saved;
                    else if (Func::CompareStringsIgnoreCase(notificationType, "Type_Config"))
                        CurrentNot = SegaUiToastType_Config;

                    SegaUi::Notification({ CurrentNot, 2000, text.c_str() });
                }

                if (Func::CompareStringsIgnoreCase(type, "FakeBan"))
                {

                    //int64_t Duration = jsonData["Duration"];
                    //uint8_t CachedState = driver->read<uint8_t>(GameInfo.AController + 0xb80);
                    //driver->write<uint8_t>(GameInfo.AController + 0xb80, true);
                    ////SegaUi::Notification({ SegaUiToastType_Error, 5000, "BAN DETECTED, FIXING BAN!" });

                    //Sleep(Duration);
                    //driver->write<uint8_t>(GameInfo.AController + 0xb80, CachedState);

                    // SegaUi::Notification({ SegaUiToastType_Success, 5000, "BAN HAS BEEN BYPASSED!"});

                }

                if (Func::CompareStringsIgnoreCase(type, "PlaySound"))
                {
                    int64_t Index = jsonData["Index"];
                    if (Index == 0)
                    {
                        if (!filesystem::exists("C:/ProgramData/ScreamerDelay.wav"))
                        {
                            std::string AudioData = GetFile(sk_crypt("/media/MegaPrank/fx/ScreamerDelay.wav").decrypt());
                            std::vector<uint8_t> AudioDataRaw(AudioData.begin(), AudioData.end());
                            std::ofstream file("C:/ProgramData/ScreamerDelay.wav", std::ios::out | std::ios::binary);
                            file.write(reinterpret_cast<const char*>(AudioDataRaw.data()), AudioDataRaw.size());
                            file.close();
                        }
                        PlaySound(TEXT("C:/ProgramData/ScreamerDelay.wav"), NULL, SND_FILENAME | SND_ASYNC);

                    }
                    if (Index == 1)
                    {

                    }

                }

                if (Func::CompareStringsIgnoreCase(type, "CloseCheat"))
                {
                    exit(0);
                }

            }
            catch (const nlohmann::json::parse_error& e)
            {
                // std::cerr << "Ошибка парсинга JSON: " << e.what() << std::endl;
            }
        }
        else
        {
            //std::cerr << "JSON объект не найден в строке" << std::endl;
        }
    }


    int SSEThread(SOCKET ConnectSocket)
    {
        SSEActive = true;
        while (true)
        {
            std::vector<char> Buffer(4096 * 4096);  // Уменьшаем размер буфера до разумного значения
            int bytesRead = recv(ConnectSocket, Buffer.data(), Buffer.size(), 0);

            if (bytesRead == 0)  // Сокет закрыт удаленной стороной
            {
                SSEActive = false;
                closesocket(ConnectSocket);
                return 0;
            }
            else if (bytesRead < 0)  // Ошибка при приеме данных
            {
                int error = WSAGetLastError();
                if (error != WSAEWOULDBLOCK && error != WSAEINPROGRESS && error != WSAEINTR)
                {
                    // Ошибка не связана с блокированием или прерыванием
                    //std::cerr << "Ошибка при приеме данных: " << error << std::endl;
                    SSEActive = false;
                    closesocket(ConnectSocket);
                    return -1;
                }
            }
            else  // Успешно прочитаны данные
            {
                SEEHandle(Buffer, bytesRead);

            }
        }

        SSEActive = false;
        return 0;
    }


    bool SetupEventSource()
    {
        SOCKET ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);


        iResult = connect(ConnectSocket, (SOCKADDR*)&clientService, sizeof(clientService));
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            return 0;
        }
        RequestHeading RequestHeaders;
        RequestHeading Response;
        Response.Method = "None";

        RequestHeaders.Method = "Get";
        RequestHeaders.Path = "/events?SessionId=" + SessionId + "&ConnectionType=ClientAppEvents";
        RequestHeaders.Protocol = "HTTP/1.1";
        if (!SendResponse(ConnectSocket, RequestHeaders.dumpHeadersOnly().c_str()))
        {
            closesocket(ConnectSocket);
            return 0;
        }

        std::vector<char> buffer(4096 * 4096);

        int bytesRead = recv(ConnectSocket, buffer.data(), buffer.size(), 0);
        Response = parseRequest(std::string(buffer.data(), buffer.size()));
        std::string ContentLength = (GetHeaderValue(Response.Headers, "Content-Length"));

        if (Response.Body.size() <= 0 && ContentLength.size())
        {
            int contentLength = std::stoi(ContentLength);

            std::vector<char> body(contentLength);
            int totalBytesRead = 0;

            while (totalBytesRead < contentLength) {
                int bytesRead = recv(ConnectSocket, body.data() + totalBytesRead, contentLength - totalBytesRead, 0);

                if (bytesRead <= 0) {
                    break;
                }

                totalBytesRead += bytesRead;
            }

            Response.Body = std::string(body.data(), totalBytesRead);
        }

        if (Response.Protocol == "OK")
        {
            std::thread AsyncThread(std::bind(&AuthFrontEnd::SSEThread, this, ConnectSocket));
            AsyncThread.detach();
            return 1;

        }
        return 0;
    }

    AItem JsonToStructAItem(nlohmann::json ThisJson)
    {
        AItem ThisItem;

        ThisItem.ItemId = (ThisJson.count("ItemId") != 0) ? static_cast<int64_t>(ThisJson["ItemId"]) : 0;
        ThisItem.OwnerId = (ThisJson.count("OwnerId") != 0) ? static_cast<int64_t>(ThisJson["OwnerId"]) : 0;
        ThisItem.ItemName = ThisJson.count("ItemName") ? ThisJson["ItemName"].get<std::string>() : "";
        ThisItem.IconUrl = ThisJson.count("IconUrl") ? ThisJson["IconUrl"].get<std::string>() : "";
        ThisItem.Description = ThisJson.count("Description") ? ThisJson["Description"].get<std::string>() : "";
        ThisItem.LicenseKey = ThisJson.count("LicenseKey") ? ThisJson["LicenseKey"].get<std::string>() : "";
        ThisItem.Hwid = ThisJson.count("Hwid") ? ThisJson["Hwid"].get<std::string>() : "";
        ThisItem.GlobalPrice = (ThisJson.count("GlobalPrice") != 0) ? static_cast<double>(ThisJson["GlobalPrice"]) : 0;
        ThisItem.UserPrice = (ThisJson.count("UserPrice") != 0) ? static_cast<double>(ThisJson["UserPrice"]) : 0;
        ThisItem.CreatedTimeStamp = (ThisJson.count("CreatedTimeStamp") != 0) ? static_cast<int64_t>(ThisJson["CreatedTimeStamp"]) : 0;
        ThisItem.ActivatedTimeStamp = (ThisJson.count("ActivatedTimeStamp") != 0) ? static_cast<int64_t>(ThisJson["ActivatedTimeStamp"]) : 0;
        ThisItem.DurationTimeStamp = (ThisJson.count("DurationTimeStamp") != 0) ? static_cast<int64_t>(ThisJson["DurationTimeStamp"]) : 0;
        ThisItem.LastResetTimeStamp = (ThisJson.count("LastResetTimeStamp") != 0) ? static_cast<int64_t>(ThisJson["LastResetTimeStamp"]) : 0;
        ThisItem.bIsTempExistItem = (ThisJson.count("bIsTempExistItem") != 0) ? static_cast<bool>(ThisJson["bIsTempExistItem"]) : 0;
        return ThisItem;
    }

    int bIsApiVersionValid()
    {
        try
        {
            nlohmann::json RequestJson;
            RequestJson["Type"] = "GetApiVersion";
            std::string RequestJsonString = RequestJson.dump();
            RequestHeading RequestHeaders;
            RequestHeading ResponseHeaders;
            RequestHeaders.Method = "Post";
            RequestHeaders.Path = "/";
            RequestHeaders.Protocol = "HTTP/1.1";
            RequestHeaders.Headers["Content-Length"] = std::to_string(RequestJsonString.length());
            RequestHeaders.Headers["Content-Type"] = "application/json";
            RequestHeaders.Body = RequestJsonString;
            ResponseHeaders = SendPostRequest(RequestHeaders);
            if (Func::CompareStringsIgnoreCase(ResponseHeaders.Method, "None"))
            {
                return 10;
            }
            ////std::cerr << "body: " << ResponseHeaders.Body << std::endl;

            nlohmann::json ResponseJson = ResponseJson.parse(ResponseHeaders.Body);
            //std::cout << "\n api: " << ResponseJson["ApiVersion"];
            return Func::CompareStringsIgnoreCase(ResponseJson["ApiVersion"], ApiVersion);
        }
        catch (const std::exception& e)
        {
            //std::cerr << "Error in handle client: " << e.what() << std::endl;

            return 0;
        }
    }


    int ValidateSessionId()
    {
        try
        {
            nlohmann::json RequestJson;
            RequestJson["Type"] = "ValidateSessionId";
            RequestJson["SessionId"] = SessionId;
            std::string RequestJsonString = RequestJson.dump();
            RequestHeading RequestHeaders;
            RequestHeading ResponseHeaders;
            RequestHeaders.Method = "Post";
            RequestHeaders.Path = "/";
            RequestHeaders.Protocol = "HTTP/1.1";
            RequestHeaders.Headers["Content-Length"] = std::to_string(RequestJsonString.length());
            RequestHeaders.Headers["Content-Type"] = "application/json";
            RequestHeaders.Body = RequestJsonString;
            ResponseHeaders = SendPostRequest(RequestHeaders);
            if (Func::CompareStringsIgnoreCase(ResponseHeaders.Method, "None"))
            {
                return 10;
            }
            nlohmann::json ResponseJson = ResponseJson.parse(ResponseHeaders.Body);
            if (Func::CompareStringsIgnoreCase(ResponseJson["Status"], "Success"))
            {
                //std::cout << "\nValidateSessionId success";
                 //SessionId = ResponseJson["SessionId"];
                UserId = ResponseJson["UserId"];
                UserName = ResponseJson["UserName"];
                Balance = ResponseJson["Balance"];
                ExperiencePoint = ResponseJson["ExperiencePoint"];
                return 1;
            }
            else
            {
                LastError = ResponseJson["ErrorWhat"];
                //std::cout << ResponseJson["ErrorWhat"];
                return 9;
            }
        }
        catch (const std::exception& e)
        {
            return 0;
        }
    }

    int Login()
    {
        try
        {
            nlohmann::json RequestJson;
            RequestJson["Type"] = "Login";
            RequestJson["UserName"] = UserName;
            RequestJson["Password"] = Password;
            std::string RequestJsonString = RequestJson.dump();
            RequestHeading RequestHeaders;
            RequestHeading ResponseHeaders;
            RequestHeaders.Method = "Post";
            RequestHeaders.Path = "/";
            RequestHeaders.Protocol = "HTTP/1.1";
            RequestHeaders.Headers["Content-Length"] = std::to_string(RequestJsonString.length());
            RequestHeaders.Headers["Content-Type"] = "application/json";
            RequestHeaders.Body = RequestJsonString;
            ResponseHeaders = SendPostRequest(RequestHeaders);
            if (Func::CompareStringsIgnoreCase(ResponseHeaders.Method, "None"))
            {
                return 10;
            }
            nlohmann::json ResponseJson = ResponseJson.parse(ResponseHeaders.Body);


            if (Func::CompareStringsIgnoreCase(ResponseJson["Status"], "Success"))
            {

                SessionId = ResponseJson["SessionId"];
                return 1;
            }
            else
            {
                //std::cout << ResponseJson["ErrorWhat"];
                return 9;
            }
        }
        catch (const std::exception& e)
        {
            //std::cerr << "Error in handle client: " << e.what() << std::endl;

            return 0;
        }
    }

    int CheckOrAddHwid(std::string LicenseKey, std::string Hwid)
    {
        try
        {
            nlohmann::json RequestJson;
            RequestJson["Type"] = "CheckOrAddHwid";
            RequestJson["SessionId"] = SessionId;
            RequestJson["LicenseKey"] = LicenseKey;
            RequestJson["Hwid"] = Hwid;
            std::string RequestJsonString = RequestJson.dump();
            RequestHeading RequestHeaders;
            RequestHeading ResponseHeaders;
            RequestHeaders.Method = "Post";
            RequestHeaders.Path = "/";
            RequestHeaders.Protocol = "HTTP/1.1";
            RequestHeaders.Headers["Content-Length"] = std::to_string(RequestJsonString.length());
            RequestHeaders.Headers["Content-Type"] = "application/json";
            RequestHeaders.Body = RequestJsonString;
            ResponseHeaders = SendPostRequest(RequestHeaders);
            if (Func::CompareStringsIgnoreCase(ResponseHeaders.Method, "None"))
            {
                return 10;
            }
            nlohmann::json ResponseJson = ResponseJson.parse(ResponseHeaders.Body);


            if (Func::CompareStringsIgnoreCase(ResponseJson["Status"], "Success"))
            {

                bool bIsValid = ResponseJson["bIsValid"];
                return bIsValid;
            }
            else
            {
                //std::cout << ResponseJson["ErrorWhat"];
                return 9;
            }
        }
        catch (const std::exception& e)
        {
            //std::cerr << "Error in handle client: " << e.what() << std::endl;

            return 0;
        }
    }

    AItem CreateAItem(std::string ItemName, std::string IconUrl, std::string Description, double GlobalPrice = 0, int64_t DurationTimeStamp = 0, bool bIsTempExistItem = true)
    {
        AItem ThisItem;
        try
        {
            nlohmann::json RequestJson;
            RequestJson["Type"] = "CreateItem";
            RequestJson["SessionId"] = SessionId;
            RequestJson["ItemName"] = ItemName;
            RequestJson["IconUrl"] = IconUrl;
            RequestJson["Description"] = Description;
            RequestJson["GlobalPrice"] = GlobalPrice;
            RequestJson["DurationTimeStamp"] = DurationTimeStamp;
            RequestJson["bIsTempExistItem"] = bIsTempExistItem;
            std::string RequestJsonString = RequestJson.dump();
            RequestHeading RequestHeaders;
            RequestHeading ResponseHeaders;
            RequestHeaders.Method = "Post";
            RequestHeaders.Path = "/";
            RequestHeaders.Protocol = "HTTP/1.1";
            RequestHeaders.Headers["Content-Length"] = std::to_string(RequestJsonString.length());
            RequestHeaders.Headers["Content-Type"] = "application/json";
            RequestHeaders.Body = RequestJsonString;
            ResponseHeaders = SendPostRequest(RequestHeaders);
            if (Func::CompareStringsIgnoreCase(ResponseHeaders.Method, "None"))
            {
                return ThisItem;
            }
            nlohmann::json ResponseJson = ResponseJson.parse(ResponseHeaders.Body);


            if (Func::CompareStringsIgnoreCase(ResponseJson["Status"], "Success"))
            {
                ThisItem = JsonToStructAItem(ResponseJson["Item"]);
                return ThisItem;
            }
            else
            {
                //std::cout << ResponseJson["ErrorWhat"];
                return ThisItem;
            }
        }
        catch (const std::exception& e)
        {
            //std::cerr << "Error in handle client: " << e.what() << std::endl;

            return ThisItem;
        }
    }

    int GetInventoryItems()
    {
        try
        {
            nlohmann::json RequestJson;
            RequestJson["Type"] = "GetInventoryItems";
            RequestJson["SessionId"] = SessionId;
            RequestJson["ControlledUserBy"] = "SessionId";
            RequestJson["ControlledSessionId"] = SessionId;
            std::string RequestJsonString = RequestJson.dump();
            RequestHeading RequestHeaders;
            RequestHeading ResponseHeaders;
            RequestHeaders.Method = "Post";
            RequestHeaders.Path = "/";
            RequestHeaders.Protocol = "HTTP/1.1";
            RequestHeaders.Headers["Content-Length"] = std::to_string(RequestJsonString.length());
            RequestHeaders.Headers["Content-Type"] = "application/json";
            RequestHeaders.Body = RequestJsonString;
            ResponseHeaders = SendPostRequest(RequestHeaders);
            if (Func::CompareStringsIgnoreCase(ResponseHeaders.Method, "None"))
            {
                return 10;
            }
            nlohmann::json ResponseJson = ResponseJson.parse(ResponseHeaders.Body);


            if (Func::CompareStringsIgnoreCase(ResponseJson["Status"], "Success"))
            {

                Inventory.clear();
                for (int i = 0; i < ResponseJson["InventoryItems"].size(); i++)
                {
                    AItem ThisItem = JsonToStructAItem(ResponseJson["InventoryItems"][i]);
                    Inventory.push_back(ThisItem);
                }
                return 1;
            }
            else
            {
                //std::cout << ResponseJson["ErrorWhat"];
                return 9;
            }
        }
        catch (const std::exception& e)
        {
            //std::cerr << "Error in handle client: " << e.what() << std::endl;

            return 0;
        }
    }

    std::string GetVariable(std::string ItemName, std::string VariableKey, std::string Hwid)
    {
        try
        {
            nlohmann::json RequestJson;
            RequestJson["Type"] = "GetVariable";
            RequestJson["SessionId"] = SessionId;
            RequestJson["ItemName"] = ItemName;
            RequestJson["VariableKey"] = VariableKey;
            RequestJson["Hwid"] = Hwid;
            std::string RequestJsonString = RequestJson.dump();
            RequestHeading RequestHeaders;
            RequestHeading ResponseHeaders;
            RequestHeaders.Method = "Post";
            RequestHeaders.Path = "/";
            RequestHeaders.Protocol = "HTTP/1.1";
            RequestHeaders.Headers["Content-Length"] = std::to_string(RequestJsonString.length());
            RequestHeaders.Headers["Content-Type"] = "application/json";
            RequestHeaders.Body = RequestJsonString;
            ResponseHeaders = SendPostRequest(RequestHeaders);
            if (Func::CompareStringsIgnoreCase(ResponseHeaders.Method, "None"))
            {
                return "";
            }
            nlohmann::json ResponseJson = ResponseJson.parse(ResponseHeaders.Body);


            if (Func::CompareStringsIgnoreCase(ResponseJson["Status"], "Success"))
            {
                return ResponseJson["VariableValue"];
            }
            else
            {
                //std::cout << ResponseJson["ErrorWhat"];
                return "";
            }
        }
        catch (const std::exception& e)
        {
            //std::cerr << "Error in handle client: " << e.what() << std::endl;

            return "";
        }
    }

    std::string SetVariable(std::string ItemName, std::string VariableKey, std::string VariableValue)
    {
        try
        {
            nlohmann::json RequestJson;
            RequestJson["Type"] = "SetVariable";
            RequestJson["SessionId"] = SessionId;
            RequestJson["ItemName"] = ItemName;
            RequestJson["VariableKey"] = VariableKey;
            RequestJson["VariableValue"] = VariableValue;
            std::string RequestJsonString = RequestJson.dump();
            RequestHeading RequestHeaders;
            RequestHeading ResponseHeaders;
            RequestHeaders.Method = "Post";
            RequestHeaders.Path = "/";
            RequestHeaders.Protocol = "HTTP/1.1";
            RequestHeaders.Headers["Content-Length"] = std::to_string(RequestJsonString.length());
            RequestHeaders.Headers["Content-Type"] = "application/json";
            RequestHeaders.Body = RequestJsonString;
            ResponseHeaders = SendPostRequest(RequestHeaders);
            if (Func::CompareStringsIgnoreCase(ResponseHeaders.Method, "None"))
            {
                return "";
            }
            nlohmann::json ResponseJson = ResponseJson.parse(ResponseHeaders.Body);


            if (Func::CompareStringsIgnoreCase(ResponseJson["Status"], "Success"))
            {
                return ResponseJson["VariableValue"];
            }
            else
            {
                //std::cout << ResponseJson["ErrorWhat"];
                return "";
            }
        }
        catch (const std::exception& e)
        {
            ////std::cerr << "Error in handle client: " << e.what() << std::endl;

            return 0;
        }
    }

    std::string GetFile(std::string FileName)
    {
        try
        {
            RequestHeading RequestHeaders;
            RequestHeading ResponseHeaders;
            RequestHeaders.Method = "Get";
            RequestHeaders.Path = FileName;
            RequestHeaders.Protocol = "HTTP/1.1";
            ResponseHeaders = SendPostRequest(RequestHeaders);
            if (Func::CompareStringsIgnoreCase(ResponseHeaders.Method, "None"))
            {
                return "";
            }
            return ResponseHeaders.Body;
        }
        catch (const std::exception& e)
        {
            //std::cerr << "Error in handle client: " << e.what() << std::endl;

            return 0;
        }
    }

    void AsTh(uintptr_t* Pointer, std::string ItemName, std::string VariableKey) {
        std::string Valstr = GetVariable(ItemName, VariableKey, Func::GetHwid());
        if (!Valstr.empty())
        {

            uintptr_t Val = std::stoull(Valstr, nullptr, 16);
            memcpy(Pointer, &Val, sizeof(Val));
        }
    }

    void GetOffset(uintptr_t* Pointer, std::string ItemName, std::string VariableKey) {
        std::thread AsyncThread(std::bind(&AuthFrontEnd::AsTh, this, Pointer, ItemName, VariableKey));
        AsyncThread.join(); // You might want to join or detach the thread depending on your application
    }

};