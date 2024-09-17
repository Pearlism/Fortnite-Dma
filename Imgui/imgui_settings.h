#include "imgui.h"

namespace c {

	inline ImVec4 accent = { 1, 0, 0.5, 1.f };


	namespace bg
	{

		inline ImVec4 background = {0, 0,0.5, 0.4f };
		inline ImVec2 size = ImVec2(960, 610);
		inline float rounding = 9.f;

	}

	namespace chat
	{
		inline ImVec4 background = { 0, 0, 0.5, 0.6f };
		inline ImVec2 size = ImVec2(300, 600);
		inline ImVec2 pos = ImVec2(0, 0);

	}

	namespace child
	{

		inline ImVec4 background = ImColor(17, 17, 19, 145);
		//inline ImVec4 outline = ImColor(18, 20, 18, 255);
		inline ImVec4 outline = { 0.f, 0.f, 0.0f, 1.f };

		inline float rounding = 8.f;

	}

	namespace checkbox
	{
		inline ImVec4 background = ImColor(34, 34, 37, 255);
		inline ImVec4 circle_inactive = ImColor(46, 48, 55, 255);
		inline float rounding = 2.f;
	}

	namespace slider
	{
		inline ImVec4 background = ImColor(34, 34, 37, 255);
		inline ImVec4 circle_inactive = ImColor(46, 48, 55, 255);
		inline float rounding = 2.f;
	}

	namespace tabs
	{
		inline ImVec4 background = ImColor(16, 17, 19, 255);

		inline ImVec4 outline = ImColor(22, 24, 26, 255);
		inline float rounding = 6.f;
	}

	namespace combo
	{
		inline ImVec4 background = ImColor(16, 17, 19, 255);
		inline ImVec4 outline = ImColor(22, 24, 26, 255);
		inline ImVec4 icon = ImColor(26, 27, 31, 255);

		inline float rounding = 2.f;
	}

	namespace picker
	{
		inline ImVec4 background = ImColor(16, 17, 19, 255);
		inline ImVec4 outline = ImColor(22, 24, 26, 255);

		inline float rounding = 2.f;
	}

	namespace button
	{
		inline ImVec4 background_active = ImColor(26, 27, 29, 255);
		inline ImVec4 background_inactive = ImColor(16, 17, 19, 255);

		inline ImVec4 outline = ImColor(22, 24, 26, 255);

		inline float rounding = 2.f;
	}

	namespace keybind
	{
		inline ImVec4 background_active = ImColor(26, 27, 29, 255);
		inline ImVec4 background_inactive = ImColor(16, 17, 19, 255);

		inline ImVec4 outline = ImColor(22, 24, 26, 255);

		inline float rounding = 2.f;
	}

	namespace input
	{
		inline ImVec4 background_active = ImColor(26, 27, 29, 255);
		inline ImVec4 background_inactive = ImColor(16, 17, 19, 255);

		inline ImVec4 outline = ImColor(22, 24, 26, 255);
		inline ImVec4 icon = ImColor(26, 27, 31, 255);

		inline float rounding = 2.f;
	}

	namespace scroll
	{
		inline float rounding = 30.f;
	}

	namespace text
	{
		inline ImVec4 text_active = ImColor(255, 255, 255, 255);
		inline ImVec4 text_hov = ImColor(86 + 40, 88 + 40, 95 + 40, 255);
		inline ImVec4 text = ImColor(46 + 40, 48 + 40, 55 + 40, 255);
		inline ImVec4 bar_text = ImColor(0, 255, 221, 255);

		inline float glow_size = 8.f;
	}
}