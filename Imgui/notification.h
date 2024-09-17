#define SEGAUI_NOTIFY

#pragma once
#include <vector>
#include <string>
#include "imgui.h"
#include <iostream>
#include <sysinfoapi.h>

#include "imgui_settings.h"

//using namespace SegaUi;

extern std::string ProductName;

#include <thread>
#include <chrono>
#include <random>
using namespace std;
using namespace std::chrono;

static int dismiss = 0;

namespace font
{
	extern ImFont* inter_bold;
	extern ImFont* inter_default;
	extern ImFont* icon;
}

#define NOTIFY_MAX_MSG_LENGTH			4096		
#define NOTIFY_PADDING_X				15.f		
#define NOTIFY_PADDING_Y				15.f		
#define NOTIFY_PADDING_MESSAGE_Y		15.f		
#define NOTIFY_FADE_IN_OUT_TIME			500
#define NOTIFY_DEFAULT_DISMISS			3000		
#define NOTIFY_OPACITY					1.0f		
#define NOTIFY_TOAST_FLAGS				SegaUiWindowFlags_AlwaysAutoResize | SegaUiWindowFlags_NoDecoration | SegaUiWindowFlags_NoInputs | SegaUiWindowFlags_NoNav | SegaUiWindowFlags_NoBringToFrontOnFocus | SegaUiWindowFlags_NoFocusOnAppearing

#define NOTIFY_INLINE					inline
#define NOTIFY_NULL_OR_EMPTY(str)		(!str ||! strlen(str))
#define NOTIFY_FORMAT(fn, format, ...)	if (format) { va_list args; va_start(args, format); fn(format, args, __VA_ARGS__); va_end(args); }

typedef int SegaUiToastType;
typedef int SegaUiToastPhase;
typedef int SegaUiToastPos;

enum SegaUiToastType_
{
	SegaUiToastType_None,
	SegaUiToastType_Success,
	SegaUiToastType_Warning,
	SegaUiToastType_Error,
	SegaUiToastType_Info,
	SegaUiToastType_Saved,
	SegaUiToastType_Config,
	SegaUiToastType_COUNT
};

enum SegaUiToastPhase_
{
	SegaUiToastPhase_FadeIn,
	SegaUiToastPhase_Wait,
	SegaUiToastPhase_FadeOut,
	SegaUiToastPhase_Expired,
	SegaUiToastPhase_COUNT
};

enum SegaUiToastPos_
{
	SegaUiToastPos_TopLeft,
	SegaUiToastPos_TopCenter,
	SegaUiToastPos_TopRight,
	SegaUiToastPos_BottomLeft,
	SegaUiToastPos_BottomCenter,
	SegaUiToastPos_BottomRight,
	SegaUiToastPos_Center,
	SegaUiToastPos_COUNT
};

class SegaUiToast
{
public:
	SegaUiToastType	type = SegaUiToastType_None;
	char			title[NOTIFY_MAX_MSG_LENGTH];
	char			content[NOTIFY_MAX_MSG_LENGTH];
	int				dismiss_time = NOTIFY_DEFAULT_DISMISS;
	uint64_t		creation_time = 0;

private:
	// Setters

	NOTIFY_INLINE auto set_title(const char* format, va_list args) { vsnprintf(this->title, sizeof(this->title), format, args); }

	NOTIFY_INLINE auto set_content(const char* format, va_list args) { vsnprintf(this->content, sizeof(this->content), format, args); }

public:

	NOTIFY_INLINE auto set_title(const char* format, ...) -> void { NOTIFY_FORMAT(this->set_title, format); }

	NOTIFY_INLINE auto set_content(const char* format, ...) -> void { NOTIFY_FORMAT(this->set_content, format); }

	NOTIFY_INLINE auto set_type(const SegaUiToastType& type) -> void { IM_ASSERT(type < SegaUiToastType_COUNT); this->type = type; };

public:
	// Getters

	NOTIFY_INLINE auto get_title() -> char* { return this->title; };

	NOTIFY_INLINE auto get_default_title() -> char*
	{
		if (!strlen(this->title))
		{
			switch (this->type)
			{
			case SegaUiToastType_None:
				return NULL;
			case SegaUiToastType_Success:
				return "Success";
			case SegaUiToastType_Warning:
				return "Warning";
			case SegaUiToastType_Error:
				return "Error";
			case SegaUiToastType_Info:
				return "Info";
			case SegaUiToastType_Config:
				return "Config";
			}
		}

		return this->title;
	};


	NOTIFY_INLINE auto get_type() -> const SegaUiToastType& { return this->type; };

	NOTIFY_INLINE auto get_color() -> const ImVec4&
	{
		switch (this->type)
		{
		case SegaUiToastType_None:
			return { 255, 255, 255, 255 };
		case SegaUiToastType_Success:
			return { 0, 255, 0, 255 };
		case SegaUiToastType_Warning:
			return { 255, 255, 0, 255 };
		case SegaUiToastType_Error:
			return { 255, 0, 0, 255 };
		case SegaUiToastType_Info:
			return { 0, 157, 255, 255 };
		case SegaUiToastType_Saved:
			return { 255, 100, 0, 255 };
		case SegaUiToastType_Config:
			return { 100, 0, 100, 255 };
		}
	}

	NOTIFY_INLINE auto get_content() -> char* { return this->content; };

	NOTIFY_INLINE auto get_elapsed_time() { return GetTickCount64() - this->creation_time; }

	NOTIFY_INLINE auto get_phase() ->  SegaUiToastPhase
	{
		const auto elapsed = get_elapsed_time();
		//std::cout << "\n elapsed: " << elapsed;
		//std::cout << "\n dismiss_time: " << this->dismiss_time;

		if (elapsed > NOTIFY_FADE_IN_OUT_TIME + this->dismiss_time + NOTIFY_FADE_IN_OUT_TIME)
		{
			//std::cout << "\n SegaUiToastPhase_Expired";

			return SegaUiToastPhase_Expired;
		}
		else if (elapsed > NOTIFY_FADE_IN_OUT_TIME + this->dismiss_time)
		{
			//std::cout << "\n SegaUiToastPhase_FadeOut";

			return SegaUiToastPhase_FadeOut;
		}
		else if (elapsed > NOTIFY_FADE_IN_OUT_TIME)
		{
			//std::cout << "\n SegaUiToastPhase_Wait";

			return SegaUiToastPhase_Wait;
		}
		else
		{
			//std::cout << "\n SegaUiToastPhase_FadeIn";

			return SegaUiToastPhase_FadeIn;
		}
	}

	NOTIFY_INLINE auto get_fade_percent() -> const float
	{
		SegaUiToastPhase phase = this->get_phase();
		const auto elapsed = this->get_elapsed_time();
		//std::cout << "\n phase:" << phase;
		//std::cout << "\n elapsed:" << elapsed;

		if (phase == SegaUiToastPhase_FadeIn)
		{
			//std::cout << "\n fade in";
			return ((float)elapsed / (float)NOTIFY_FADE_IN_OUT_TIME) * NOTIFY_OPACITY;
		}
		else if (phase == SegaUiToastPhase_FadeOut)
		{
			//std::cout << "\n fade out";

			return (1.f - (((float)elapsed - (float)NOTIFY_FADE_IN_OUT_TIME - (float)this->dismiss_time) / (float)NOTIFY_FADE_IN_OUT_TIME)) * NOTIFY_OPACITY;
		}
		//std::cout << "\n gay";

		return 1.f * NOTIFY_OPACITY;
	}

public:

	SegaUiToast(SegaUiToastType type, int dismiss_time = NOTIFY_DEFAULT_DISMISS)
	{
		IM_ASSERT(type < SegaUiToastType_COUNT);

		this->type = type;
		this->dismiss_time = dismiss_time;
		this->creation_time = GetTickCount64();

		memset(this->title, 0, sizeof(this->title));
		memset(this->content, 0, sizeof(this->content));

		dismiss = dismiss_time;

	}

	SegaUiToast(SegaUiToastType type, const char* format, ...) : SegaUiToast(type) { NOTIFY_FORMAT(this->set_content, format); }

	SegaUiToast(SegaUiToastType type, int dismiss_time, const char* format, ...) : SegaUiToast(type, dismiss_time) { NOTIFY_FORMAT(this->set_content, format); }
};

namespace SegaUi
{
	NOTIFY_INLINE std::vector<SegaUiToast> notifications;

	NOTIFY_INLINE VOID Notification(const SegaUiToast& toast)
	{
		notifications.push_back(toast);
	}

	NOTIFY_INLINE VOID RemoveNotification(int index)
	{
		notifications.erase(notifications.begin() + index);
	}

	NOTIFY_INLINE VOID RenderNotifications()
	{
		const auto vp_size = GetMainViewport()->Size;

		float height = 0.f;

		for (auto i = 0; i < notifications.size(); i++)
		{
			auto* current_toast = &notifications[i];

			if (current_toast->get_phase() == SegaUiToastPhase_Expired)
			{
				RemoveNotification(i);
				continue;
			}

			const char* title = current_toast->get_title();
			const auto content = current_toast->get_content();
			const auto default_title = current_toast->get_default_title();
			const auto opacity = current_toast->get_fade_percent();
			//std::cout << "\n gay: " << current_toast->get_elapsed_time();
			const auto delay = current_toast->get_elapsed_time();

			auto text_color = current_toast->get_color();
			text_color.w = opacity;

			char window_name[50];
			sprintf_s(window_name, "##TOAST%d", i);

			PushStyleVar(SegaUiStyleVar_Alpha, opacity);

			SetNextWindowPos(ImVec2(vp_size.x - NOTIFY_PADDING_X, vp_size.y - NOTIFY_PADDING_Y - height), SegaUiCond_Always, ImVec2(1.f, 1.0f));

			PushStyleVar(SegaUiStyleVar_WindowPadding, ImVec2(20, 10));
			PushStyleVar(SegaUiStyleVar_WindowRounding, c::button::rounding);
			PushStyleVar(SegaUiStyleVar_ItemSpacing, ImVec2(10, 10));
			PushStyleVar(SegaUiStyleVar_WindowBorderSize, 1.f);

			PushStyleColor(SegaUiCol_WindowBg, GetColorU32(c::child::background));
			PushStyleColor(SegaUiCol_Border, GetColorU32(c::child::outline));

			Begin(window_name, NULL, NOTIFY_TOAST_FLAGS);
			{
				const ImVec2& pos = SegaUi::GetWindowPos(), spacing = GetStyle().ItemSpacing, region = SegaUi::GetContentRegionMax();

				GetForegroundDrawList()->AddRectFilled(pos + ImVec2(20, region.y - 16), pos + ImVec2(region.x, region.y - 10), GetColorU32(c::child::outline), 30.f);

				GetForegroundDrawList()->AddShadowRect(pos + ImVec2(20, region.y - 16), pos + ImVec2(opacity * region.x, region.y - 10), GetColorU32(text_color), 10.f, ImVec2(0, 0), 30.f);
				GetForegroundDrawList()->AddRectFilled(pos + ImVec2(20, region.y - 16), pos + ImVec2(opacity * region.x, region.y - 10), GetColorU32(text_color), 30.f);

				SetCursorPosY(GetCursorPosY() + 10);

				PushTextWrapPos(vp_size.x / 3.f);

				PushFont(font::icon);

				if (default_title == "Success") TextColored(text_color, "f");

				if (default_title == "Warning") TextColored(text_color, "d");

				if (default_title == "Error") TextColored(text_color, "e");

				if (default_title == "Info") TextColored(text_color, "g");

				if (default_title == "Config") TextColored(text_color, "a");

				PopFont();

				SegaUi::SameLine();

				PushFont(font::inter_bold);
				//TextColored(text_color, ProductName.c_str());
				TextColored(text_color, ProductName.c_str());
				PopFont();

				if (!NOTIFY_NULL_OR_EMPTY(content))
				{
					TextColored(ImColor(GetColorU32(c::text::text_active)), content);
				}

				PopTextWrapPos();
			}

			height += GetWindowHeight() + NOTIFY_PADDING_MESSAGE_Y;

			SetCursorPosY(GetCursorPosY() + 21);

			End();
			PopStyleVar(5);
			PopStyleColor(2);
		}
	}
}

