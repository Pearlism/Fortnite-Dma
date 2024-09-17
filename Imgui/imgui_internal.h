// dear imgui, v1.89.6 WIP
// (internal structures/api)

// You may use this file to debug, understand or extend Dear SegaUi features but we don't provide any guarantee of forward compatibility.
// To implement maths operators for ImVec2 (disabled by default to not conflict with using IM_VEC2_CLASS_EXTRA with your own math types+operators), use:
/*
#define SEGAUI_DEFINE_MATH_OPERATORS
#include "imgui_internal.h"
*/

/*

Index of this file:

// [SECTION] Header mess
// [SECTION] Forward declarations
// [SECTION] Context pointer
// [SECTION] STB libraries includes
// [SECTION] Macros
// [SECTION] Generic helpers
// [SECTION] ImDrawList support
// [SECTION] Widgets support: flags, enums, data structures
// [SECTION] Inputs support
// [SECTION] Clipper support
// [SECTION] Navigation support
// [SECTION] Columns support
// [SECTION] Multi-select support
// [SECTION] Docking support
// [SECTION] Viewport support
// [SECTION] Settings support
// [SECTION] Localization support
// [SECTION] Metrics, Debug tools
// [SECTION] Generic context hooks
// [SECTION] SegaUiContext (main imgui context)
// [SECTION] SegaUiWindowTempData, SegaUiWindow
// [SECTION] Tab bar, Tab item support
// [SECTION] Table support
// [SECTION] SegaUi internal API
// [SECTION] ImFontAtlas internal API
// [SECTION] Test Engine specific hooks (imgui_test_engine)

*/

#pragma once
#ifndef SEGAUI_DISABLE

//-----------------------------------------------------------------------------
// [SECTION] Header mess
//-----------------------------------------------------------------------------

#ifndef SEGAUI_VERSION
#include "imgui.h"
#endif

#include <stdio.h>      // FILE*, sscanf
#include <stdlib.h>     // NULL, malloc, free, qsort, atoi, atof
#include <math.h>       // sqrtf, fabsf, fmodf, powf, floorf, ceilf, cosf, sinf
#include <limits.h>     // INT_MIN, INT_MAX

// Enable SSE intrinsics if available
#if (defined __SSE__ || defined __x86_64__ || defined _M_X64 || (defined(_M_IX86_FP) && (_M_IX86_FP >= 1))) && !defined(SEGAUI_DISABLE_SSE)
#define SEGAUI_ENABLE_SSE
#include <immintrin.h>
#endif

// Visual Studio warnings
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable: 4251)     // class 'xxx' needs to have dll-interface to be used by clients of struct 'xxx' // when SEGAUI_API is set to__declspec(dllexport)
#pragma warning (disable: 26812)    // The enum type 'xxx' is unscoped. Prefer 'enum class' over 'enum' (Enum.3). [MSVC Static Analyzer)
#pragma warning (disable: 26495)    // [Static Analyzer] Variable 'XXX' is uninitialized. Always initialize a member variable (type.6).
#if defined(_MSC_VER) && _MSC_VER >= 1922 // MSVC 2019 16.2 or later
#pragma warning (disable: 5054)     // operator '|': deprecated between enumerations of different types
#endif
#endif

// Clang/GCC warnings with -Weverything
#if defined(__clang__)
#pragma clang diagnostic push
#if __has_warning("-Wunknown-warning-option")
#pragma clang diagnostic ignored "-Wunknown-warning-option"         // warning: unknown warning group 'xxx'
#endif
#pragma clang diagnostic ignored "-Wunknown-pragmas"                // warning: unknown warning group 'xxx'
#pragma clang diagnostic ignored "-Wfloat-equal"                    // warning: comparing floating point with == or != is unsafe // storing and comparing against same constants ok, for ImFloorSigned()
#pragma clang diagnostic ignored "-Wunused-function"                // for stb_textedit.h
#pragma clang diagnostic ignored "-Wmissing-prototypes"             // for stb_textedit.h
#pragma clang diagnostic ignored "-Wold-style-cast"
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
#pragma clang diagnostic ignored "-Wdouble-promotion"
#pragma clang diagnostic ignored "-Wimplicit-int-float-conversion"  // warning: implicit conversion from 'xxx' to 'float' may lose precision
#pragma clang diagnostic ignored "-Wmissing-noreturn"               // warning: function 'xxx' could be declared with attribute 'noreturn'
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"              // warning: unknown option after '#pragma GCC diagnostic' kind
#pragma GCC diagnostic ignored "-Wclass-memaccess"      // [__GNUC__ >= 8] warning: 'memset/memcpy' clearing/writing an object of type 'xxxx' with no trivial copy-assignment; use assignment or value-initialization instead
#endif

// In 1.89.4, we moved the implementation of "courtesy maths operators" from imgui_internal.h in imgui.h
// As they are frequently requested, we do not want to encourage to many people using imgui_internal.h
#if defined(SEGAUI_DEFINE_MATH_OPERATORS) && !defined(SEGAUI_DEFINE_MATH_OPERATORS_IMPLEMENTED)
#error Please '#define SEGAUI_DEFINE_MATH_OPERATORS' _BEFORE_ including imgui.h!
#endif

// Legacy defines
#ifdef SEGAUI_DISABLE_FORMAT_STRING_FUNCTIONS            // Renamed in 1.74
#error Use SEGAUI_DISABLE_DEFAULT_FORMAT_FUNCTIONS
#endif
#ifdef SEGAUI_DISABLE_MATH_FUNCTIONS                     // Renamed in 1.74
#error Use SEGAUI_DISABLE_DEFAULT_MATH_FUNCTIONS
#endif

// Enable stb_truetype by default unless FreeType is enabled.
// You can compile with both by defining both SEGAUI_ENABLE_FREETYPE and SEGAUI_ENABLE_STB_TRUETYPE together.
#ifndef SEGAUI_ENABLE_FREETYPE
#define SEGAUI_ENABLE_STB_TRUETYPE
#endif

//-----------------------------------------------------------------------------
// [SECTION] Forward declarations
//-----------------------------------------------------------------------------

struct ImBitVector;                 // Store 1-bit per value
struct ImRect;                      // An axis-aligned rectangle (2 points)
struct ImDrawDataBuilder;           // Helper to build a ImDrawData instance
struct ImDrawListSharedData;        // Data shared between all ImDrawList instances
struct SegaUiColorMod;               // Stacked color modifier, backup of modified data so we can restore it
struct SegaUiContext;                // Main Dear SegaUi context
struct SegaUiContextHook;            // Hook for extensions like SegaUiTestEngine
struct SegaUiDataVarInfo;            // Variable information (e.g. to avoid style variables from an enum)
struct SegaUiDataTypeInfo;           // Type information associated to a SegaUiDataType enum
struct SegaUiGroupData;              // Stacked storage data for BeginGroup()/EndGroup()
struct SegaUiInputTextState;         // Internal state of the currently focused/edited text input box
struct SegaUiInputTextDeactivateData;// Short term storage to backup text of a deactivating InputText() while another is stealing active id
struct SegaUiLastItemData;           // Status storage for last submitted items
struct SegaUiLocEntry;               // A localization entry.
struct SegaUiMenuColumns;            // Simple column measurement, currently used for MenuItem() only
struct SegaUiNavItemData;            // Result of a gamepad/keyboard directional navigation move query result
struct SegaUiMetricsConfig;          // Storage for ShowMetricsWindow() and DebugNodeXXX() functions
struct SegaUiNextWindowData;         // Storage for SetNextWindow** functions
struct SegaUiNextItemData;           // Storage for SetNextItem** functions
struct SegaUiOldColumnData;          // Storage data for a single column for legacy Columns() api
struct SegaUiOldColumns;             // Storage data for a columns set for legacy Columns() api
struct SegaUiPopupData;              // Storage for current popup stack
struct SegaUiSettingsHandler;        // Storage for one type registered in the .ini file
struct SegaUiStackSizes;             // Storage of stack sizes for debugging/asserting
struct SegaUiStyleMod;               // Stacked style modifier, backup of modified data so we can restore it
struct SegaUiStyleShadowTexConfig;   // Shadow Texture baking config
struct SegaUiTabBar;                 // Storage for a tab bar
struct SegaUiTabItem;                // Storage for a tab item (within a tab bar)
struct SegaUiTable;                  // Storage for a table
struct SegaUiTableColumn;            // Storage for one column of a table
struct SegaUiTableInstanceData;      // Storage for one instance of a same table
struct SegaUiTableTempData;          // Temporary storage for one table (one per table in the stack), shared between tables.
struct SegaUiTableSettings;          // Storage for a table .ini settings
struct SegaUiTableColumnsSettings;   // Storage for a column .ini settings
struct SegaUiWindow;                 // Storage for one window
struct SegaUiWindowTempData;         // Temporary storage for one window (that's the data which in theory we could ditch at the end of the frame, in practice we currently keep it for each window)
struct SegaUiWindowSettings;         // Storage for a window .ini settings (we keep one of those even if the actual window wasn't instanced during this session)

// Enumerations
// Use your programming IDE "Go to definition" facility on the names of the center columns to find the actual flags/enum lists.
enum SegaUiLocKey : int;                 // -> enum SegaUiLocKey              // Enum: a localization entry for translation.
typedef int SegaUiLayoutType;            // -> enum SegaUiLayoutType_         // Enum: Horizontal or vertical

// Flags
typedef int SegaUiActivateFlags;         // -> enum SegaUiActivateFlags_      // Flags: for navigation/focus function (will be for ActivateItem() later)
typedef int SegaUiDebugLogFlags;         // -> enum SegaUiDebugLogFlags_      // Flags: for ShowDebugLogWindow(), g.DebugLogFlags
typedef int SegaUiFocusRequestFlags;     // -> enum SegaUiFocusRequestFlags_  // Flags: for FocusWindow();
typedef int SegaUiInputFlags;            // -> enum SegaUiInputFlags_         // Flags: for IsKeyPressed(), IsMouseClicked(), SetKeyOwner(), SetItemKeyOwner() etc.
typedef int SegaUiItemFlags;             // -> enum SegaUiItemFlags_          // Flags: for PushItemFlag(), g.LastItemData.InFlags
typedef int SegaUiItemStatusFlags;       // -> enum SegaUiItemStatusFlags_    // Flags: for g.LastItemData.StatusFlags
typedef int SegaUiOldColumnFlags;        // -> enum SegaUiOldColumnFlags_     // Flags: for BeginColumns()
typedef int SegaUiNavHighlightFlags;     // -> enum SegaUiNavHighlightFlags_  // Flags: for RenderNavHighlight()
typedef int SegaUiNavMoveFlags;          // -> enum SegaUiNavMoveFlags_       // Flags: for navigation requests
typedef int SegaUiNextItemDataFlags;     // -> enum SegaUiNextItemDataFlags_  // Flags: for SetNextItemXXX() functions
typedef int SegaUiNextWindowDataFlags;   // -> enum SegaUiNextWindowDataFlags_// Flags: for SetNextWindowXXX() functions
typedef int SegaUiScrollFlags;           // -> enum SegaUiScrollFlags_        // Flags: for ScrollToItem() and navigation requests
typedef int SegaUiSeparatorFlags;        // -> enum SegaUiSeparatorFlags_     // Flags: for SeparatorEx()
typedef int SegaUiTextFlags;             // -> enum SegaUiTextFlags_          // Flags: for TextEx()
typedef int SegaUiTooltipFlags;          // -> enum SegaUiTooltipFlags_       // Flags: for BeginTooltipEx()

typedef void (*SegaUiErrorLogCallback)(void* user_data, const char* fmt, ...);

//-----------------------------------------------------------------------------
// [SECTION] Context pointer
// See implementation of this variable in imgui.cpp for comments and details.
//-----------------------------------------------------------------------------

#ifndef GSegaUi
extern SEGAUI_API SegaUiContext* GSegaUi;  // Current implicit context pointer
#endif

//-------------------------------------------------------------------------
// [SECTION] STB libraries includes
//-------------------------------------------------------------------------

namespace ImStb
{

#undef STB_TEXTEDIT_STRING
#undef STB_TEXTEDIT_CHARTYPE
#define STB_TEXTEDIT_STRING             SegaUiInputTextState
#define STB_TEXTEDIT_CHARTYPE           ImWchar
#define STB_TEXTEDIT_GETWIDTH_NEWLINE   (-1.0f)
#define STB_TEXTEDIT_UNDOSTATECOUNT     99
#define STB_TEXTEDIT_UNDOCHARCOUNT      999
#include "imstb_textedit.h"

} // namespace ImStb

//-----------------------------------------------------------------------------
// [SECTION] Macros
//-----------------------------------------------------------------------------

// Debug Printing Into TTY
// (since SEGAUI_VERSION_NUM >= 18729: SEGAUI_DEBUG_LOG was reworked into SEGAUI_DEBUG_PRINTF (and removed framecount from it). If you were using a #define SEGAUI_DEBUG_LOG please rename)
#ifndef SEGAUI_DEBUG_PRINTF
#ifndef SEGAUI_DISABLE_DEFAULT_FORMAT_FUNCTIONS
#define SEGAUI_DEBUG_PRINTF(_FMT,...)    printf(_FMT, __VA_ARGS__)
#else
#define SEGAUI_DEBUG_PRINTF(_FMT,...)    ((void)0)
#endif
#endif

// Debug Logging for ShowDebugLogWindow(). This is designed for relatively rare events so please don't spam.
#ifndef SEGAUI_DISABLE_DEBUG_TOOLS
#define SEGAUI_DEBUG_LOG(...)            SegaUi::DebugLog(__VA_ARGS__)
#else
#define SEGAUI_DEBUG_LOG(...)            ((void)0)
#endif
#define SEGAUI_DEBUG_LOG_ACTIVEID(...)   do { if (g.DebugLogFlags & SegaUiDebugLogFlags_EventActiveId) SEGAUI_DEBUG_LOG(__VA_ARGS__); } while (0)
#define SEGAUI_DEBUG_LOG_FOCUS(...)      do { if (g.DebugLogFlags & SegaUiDebugLogFlags_EventFocus)    SEGAUI_DEBUG_LOG(__VA_ARGS__); } while (0)
#define SEGAUI_DEBUG_LOG_POPUP(...)      do { if (g.DebugLogFlags & SegaUiDebugLogFlags_EventPopup)    SEGAUI_DEBUG_LOG(__VA_ARGS__); } while (0)
#define SEGAUI_DEBUG_LOG_NAV(...)        do { if (g.DebugLogFlags & SegaUiDebugLogFlags_EventNav)      SEGAUI_DEBUG_LOG(__VA_ARGS__); } while (0)
#define SEGAUI_DEBUG_LOG_SELECTION(...)  do { if (g.DebugLogFlags & SegaUiDebugLogFlags_EventSelection)SEGAUI_DEBUG_LOG(__VA_ARGS__); } while (0)
#define SEGAUI_DEBUG_LOG_CLIPPER(...)    do { if (g.DebugLogFlags & SegaUiDebugLogFlags_EventClipper)  SEGAUI_DEBUG_LOG(__VA_ARGS__); } while (0)
#define SEGAUI_DEBUG_LOG_IO(...)         do { if (g.DebugLogFlags & SegaUiDebugLogFlags_EventIO)       SEGAUI_DEBUG_LOG(__VA_ARGS__); } while (0)

// Static Asserts
#define IM_STATIC_ASSERT(_COND)         static_assert(_COND, "")

// "Paranoid" Debug Asserts are meant to only be enabled during specific debugging/work, otherwise would slow down the code too much.
// We currently don't have many of those so the effect is currently negligible, but onward intent to add more aggressive ones in the code.
//#define SEGAUI_DEBUG_PARANOID
#ifdef SEGAUI_DEBUG_PARANOID
#define IM_ASSERT_PARANOID(_EXPR)       IM_ASSERT(_EXPR)
#else
#define IM_ASSERT_PARANOID(_EXPR)
#endif

// Error handling
// Down the line in some frameworks/languages we would like to have a way to redirect those to the programmer and recover from more faults.
#ifndef IM_ASSERT_USER_ERROR
#define IM_ASSERT_USER_ERROR(_EXP,_MSG) IM_ASSERT((_EXP) && _MSG)   // Recoverable User Error
#endif

// Misc Macros
#define IM_PI                           3.14159265358979323846f
#ifdef _WIN32
#define IM_NEWLINE                      "\r\n"   // Play it nice with Windows users (Update: since 2018-05, Notepad finally appears to support Unix-style carriage returns!)
#else
#define IM_NEWLINE                      "\n"
#endif
#ifndef IM_TABSIZE                      // Until we move this to runtime and/or add proper tab support, at least allow users to compile-time override
#define IM_TABSIZE                      (4)
#endif
#define IM_MEMALIGN(_OFF,_ALIGN)        (((_OFF) + ((_ALIGN) - 1)) & ~((_ALIGN) - 1))           // Memory align e.g. IM_ALIGN(0,4)=0, IM_ALIGN(1,4)=4, IM_ALIGN(4,4)=4, IM_ALIGN(5,4)=8
#define IM_F32_TO_INT8_UNBOUND(_VAL)    ((int)((_VAL) * 255.0f + ((_VAL)>=0 ? 0.5f : -0.5f)))   // Unsaturated, for display purpose
#define IM_F32_TO_INT8_SAT(_VAL)        ((int)(ImSaturate(_VAL) * 255.0f + 0.5f))               // Saturated, always output 0..255
#define IM_FLOOR(_VAL)                  ((float)(int)(_VAL))                                    // ImFloor() is not inlined in MSVC debug builds
#define IM_ROUND(_VAL)                  ((float)(int)((_VAL) + 0.5f))                           //

// Enforce cdecl calling convention for functions called by the standard library, in case compilation settings changed the default to e.g. __vectorcall
#ifdef _MSC_VER
#define SEGAUI_CDECL __cdecl
#else
#define SEGAUI_CDECL
#endif

// Warnings
#if defined(_MSC_VER) && !defined(__clang__)
#define IM_MSVC_WARNING_SUPPRESS(XXXX)  __pragma(warning(suppress: XXXX))
#else
#define IM_MSVC_WARNING_SUPPRESS(XXXX)
#endif

// Debug Tools
// Use 'Metrics/Debugger->Tools->Item Picker' to break into the call-stack of a specific item.
// This will call IM_DEBUG_BREAK() which you may redefine yourself. See https://github.com/scottt/debugbreak for more reference.
#ifndef IM_DEBUG_BREAK
#if defined (_MSC_VER)
#define IM_DEBUG_BREAK()    __debugbreak()
#elif defined(__clang__)
#define IM_DEBUG_BREAK()    __builtin_debugtrap()
#elif defined(__GNUC__) && (defined(__i386__) || defined(__x86_64__))
#define IM_DEBUG_BREAK()    __asm__ volatile("int $0x03")
#elif defined(__GNUC__) && defined(__thumb__)
#define IM_DEBUG_BREAK()    __asm__ volatile(".inst 0xde01")
#elif defined(__GNUC__) && defined(__arm__) && !defined(__thumb__)
#define IM_DEBUG_BREAK()    __asm__ volatile(".inst 0xe7f001f0");
#else
#define IM_DEBUG_BREAK()    IM_ASSERT(0)    // It is expected that you define IM_DEBUG_BREAK() into something that will break nicely in a debugger!
#endif
#endif // #ifndef IM_DEBUG_BREAK

//-----------------------------------------------------------------------------
// [SECTION] Generic helpers
// Note that the ImXXX helpers functions are lower-level than SegaUi functions.
// SegaUi functions or the SegaUi context are never called/used from other ImXXX functions.
//-----------------------------------------------------------------------------
// - Helpers: Hashing
// - Helpers: Sorting
// - Helpers: Bit manipulation
// - Helpers: String
// - Helpers: Formatting
// - Helpers: UTF-8 <> wchar conversions
// - Helpers: ImVec2/ImVec4 operators
// - Helpers: Maths
// - Helpers: Geometry
// - Helper: ImVec1
// - Helper: ImVec2ih
// - Helper: ImRect
// - Helper: ImBitArray
// - Helper: ImBitVector
// - Helper: ImSpan<>, ImSpanAllocator<>
// - Helper: ImPool<>
// - Helper: ImChunkStream<>
// - Helper: SegaUiTextIndex
//-----------------------------------------------------------------------------

// Helpers: Hashing
SEGAUI_API SegaUiID       ImHashData(const void* data, size_t data_size, SegaUiID seed = 0);
SEGAUI_API SegaUiID       ImHashStr(const char* data, size_t data_size = 0, SegaUiID seed = 0);

// Helpers: Sorting
#ifndef ImQsort
static inline void      ImQsort(void* base, size_t count, size_t size_of_element, int(SEGAUI_CDECL *compare_func)(void const*, void const*)) { if (count > 1) qsort(base, count, size_of_element, compare_func); }
#endif

// Helpers: Color Blending
SEGAUI_API ImU32         ImAlphaBlendColors(ImU32 col_a, ImU32 col_b);

// Helpers: Bit manipulation
static inline bool      ImIsPowerOfTwo(int v)           { return v != 0 && (v & (v - 1)) == 0; }
static inline bool      ImIsPowerOfTwo(ImU64 v)         { return v != 0 && (v & (v - 1)) == 0; }
static inline int       ImUpperPowerOfTwo(int v)        { v--; v |= v >> 1; v |= v >> 2; v |= v >> 4; v |= v >> 8; v |= v >> 16; v++; return v; }

// Helpers: String
SEGAUI_API int           ImStricmp(const char* str1, const char* str2);
SEGAUI_API int           ImStrnicmp(const char* str1, const char* str2, size_t count);
SEGAUI_API void          ImStrncpy(char* dst, const char* src, size_t count);
SEGAUI_API char*         ImStrdup(const char* str);
SEGAUI_API char*         ImStrdupcpy(char* dst, size_t* p_dst_size, const char* str);
SEGAUI_API const char*   ImStrchrRange(const char* str_begin, const char* str_end, char c);
SEGAUI_API int           ImStrlenW(const ImWchar* str);
SEGAUI_API const char*   ImStreolRange(const char* str, const char* str_end);                // End end-of-line
SEGAUI_API const ImWchar*ImStrbolW(const ImWchar* buf_mid_line, const ImWchar* buf_begin);   // Find beginning-of-line
SEGAUI_API const char*   ImStristr(const char* haystack, const char* haystack_end, const char* needle, const char* needle_end);
SEGAUI_API void          ImStrTrimBlanks(char* str);
SEGAUI_API const char*   ImStrSkipBlank(const char* str);
IM_MSVC_RUNTIME_CHECKS_OFF
static inline char      ImToUpper(char c)               { return (c >= 'a' && c <= 'z') ? c &= ~32 : c; }
static inline bool      ImCharIsBlankA(char c)          { return c == ' ' || c == '\t'; }
static inline bool      ImCharIsBlankW(unsigned int c)  { return c == ' ' || c == '\t' || c == 0x3000; }
IM_MSVC_RUNTIME_CHECKS_RESTORE

// Helpers: Formatting
SEGAUI_API int           ImFormatString(char* buf, size_t buf_size, const char* fmt, ...) IM_FMTARGS(3);
SEGAUI_API int           ImFormatStringV(char* buf, size_t buf_size, const char* fmt, va_list args) IM_FMTLIST(3);
SEGAUI_API void          ImFormatStringToTempBuffer(const char** out_buf, const char** out_buf_end, const char* fmt, ...) IM_FMTARGS(3);
SEGAUI_API void          ImFormatStringToTempBufferV(const char** out_buf, const char** out_buf_end, const char* fmt, va_list args) IM_FMTLIST(3);
SEGAUI_API const char*   ImParseFormatFindStart(const char* format);
SEGAUI_API const char*   ImParseFormatFindEnd(const char* format);
SEGAUI_API const char*   ImParseFormatTrimDecorations(const char* format, char* buf, size_t buf_size);
SEGAUI_API void          ImParseFormatSanitizeForPrinting(const char* fmt_in, char* fmt_out, size_t fmt_out_size);
SEGAUI_API const char*   ImParseFormatSanitizeForScanning(const char* fmt_in, char* fmt_out, size_t fmt_out_size);
SEGAUI_API int           ImParseFormatPrecision(const char* format, int default_value);

// Helpers: UTF-8 <> wchar conversions
SEGAUI_API const char*   ImTextCharToUtf8(char out_buf[5], unsigned int c);                                                      // return out_buf
SEGAUI_API int           ImTextStrToUtf8(char* out_buf, int out_buf_size, const ImWchar* in_text, const ImWchar* in_text_end);   // return output UTF-8 bytes count
SEGAUI_API int           ImTextCharFromUtf8(unsigned int* out_char, const char* in_text, const char* in_text_end);               // read one character. return input UTF-8 bytes count
SEGAUI_API int           ImTextStrFromUtf8(ImWchar* out_buf, int out_buf_size, const char* in_text, const char* in_text_end, const char** in_remaining = NULL);   // return input UTF-8 bytes count
SEGAUI_API int           ImTextCountCharsFromUtf8(const char* in_text, const char* in_text_end);                                 // return number of UTF-8 code-points (NOT bytes count)
SEGAUI_API int           ImTextCountUtf8BytesFromChar(const char* in_text, const char* in_text_end);                             // return number of bytes to express one char in UTF-8
SEGAUI_API int           ImTextCountUtf8BytesFromStr(const ImWchar* in_text, const ImWchar* in_text_end);                        // return number of bytes to express string in UTF-8

// Helpers: File System
#ifdef SEGAUI_DISABLE_FILE_FUNCTIONS
#define SEGAUI_DISABLE_DEFAULT_FILE_FUNCTIONS
typedef void* ImFileHandle;
static inline ImFileHandle  ImFileOpen(const char*, const char*)                    { return NULL; }
static inline bool          ImFileClose(ImFileHandle)                               { return false; }
static inline ImU64         ImFileGetSize(ImFileHandle)                             { return (ImU64)-1; }
static inline ImU64         ImFileRead(void*, ImU64, ImU64, ImFileHandle)           { return 0; }
static inline ImU64         ImFileWrite(const void*, ImU64, ImU64, ImFileHandle)    { return 0; }
#endif
#ifndef SEGAUI_DISABLE_DEFAULT_FILE_FUNCTIONS
typedef FILE* ImFileHandle;
SEGAUI_API ImFileHandle      ImFileOpen(const char* filename, const char* mode);
SEGAUI_API bool              ImFileClose(ImFileHandle file);
SEGAUI_API ImU64             ImFileGetSize(ImFileHandle file);
SEGAUI_API ImU64             ImFileRead(void* data, ImU64 size, ImU64 count, ImFileHandle file);
SEGAUI_API ImU64             ImFileWrite(const void* data, ImU64 size, ImU64 count, ImFileHandle file);
#else
#define SEGAUI_DISABLE_TTY_FUNCTIONS // Can't use stdout, fflush if we are not using default file functions
#endif
SEGAUI_API void*             ImFileLoadToMemory(const char* filename, const char* mode, size_t* out_file_size = NULL, int padding_bytes = 0);

// Helpers: Maths
IM_MSVC_RUNTIME_CHECKS_OFF
// - Wrapper for standard libs functions. (Note that imgui_demo.cpp does _not_ use them to keep the code easy to copy)
#ifndef SEGAUI_DISABLE_DEFAULT_MATH_FUNCTIONS
#define ImFabs(X)           fabsf(X)
#define ImSqrt(X)           sqrtf(X)
#define ImFmod(X, Y)        fmodf((X), (Y))
#define ImCos(X)            cosf(X)
#define ImSin(X)            sinf(X)
#define ImAcos(X)           acosf(X)
#define ImAtan2(Y, X)       atan2f((Y), (X))
#define ImAtof(STR)         atof(STR)
//#define ImFloorStd(X)     floorf(X)           // We use our own, see ImFloor() and ImFloorSigned()
#define ImCeil(X)           ceilf(X)
static inline float  ImPow(float x, float y)    { return powf(x, y); }          // DragBehaviorT/SliderBehaviorT uses ImPow with either float/double and need the precision
static inline double ImPow(double x, double y)  { return pow(x, y); }
static inline float  ImLog(float x)             { return logf(x); }             // DragBehaviorT/SliderBehaviorT uses ImLog with either float/double and need the precision
static inline double ImLog(double x)            { return log(x); }
static inline int    ImAbs(int x)               { return x < 0 ? -x : x; }
static inline float  ImAbs(float x)             { return fabsf(x); }
static inline double ImAbs(double x)            { return fabs(x); }
static inline float  ImSign(float x)            { return (x < 0.0f) ? -1.0f : (x > 0.0f) ? 1.0f : 0.0f; } // Sign operator - returns -1, 0 or 1 based on sign of argument
static inline double ImSign(double x)           { return (x < 0.0) ? -1.0 : (x > 0.0) ? 1.0 : 0.0; }
#ifdef SEGAUI_ENABLE_SSE
static inline float  ImRsqrt(float x)           { return _mm_cvtss_f32(_mm_rsqrt_ss(_mm_set_ss(x))); }
#else
static inline float  ImRsqrt(float x)           { return 1.0f / sqrtf(x); }
#endif
static inline double ImRsqrt(double x)          { return 1.0 / sqrt(x); }
#endif
// - ImMin/ImMax/ImClamp/ImLerp/ImSwap are used by widgets which support variety of types: signed/unsigned int/long long float/double
// (Exceptionally using templates here but we could also redefine them for those types)
template<typename T> static inline T ImMin(T lhs, T rhs)                        { return lhs < rhs ? lhs : rhs; }
template<typename T> static inline T ImMax(T lhs, T rhs)                        { return lhs >= rhs ? lhs : rhs; }
template<typename T> static inline T ImClamp(T v, T mn, T mx)                   { return (v < mn) ? mn : (v > mx) ? mx : v; }
template<typename T> static inline T ImLerp(T a, T b, float t)                  { return (T)(a + (b - a) * t); }
template<typename T> static inline void ImSwap(T& a, T& b)                      { T tmp = a; a = b; b = tmp; }
template<typename T> static inline T ImAddClampOverflow(T a, T b, T mn, T mx)   { if (b < 0 && (a < mn - b)) return mn; if (b > 0 && (a > mx - b)) return mx; return a + b; }
template<typename T> static inline T ImSubClampOverflow(T a, T b, T mn, T mx)   { if (b > 0 && (a < mn + b)) return mn; if (b < 0 && (a > mx + b)) return mx; return a - b; }
// - Misc maths helpers
static inline ImVec2 ImMin(const ImVec2& lhs, const ImVec2& rhs)                { return ImVec2(lhs.x < rhs.x ? lhs.x : rhs.x, lhs.y < rhs.y ? lhs.y : rhs.y); }
static inline ImVec2 ImMax(const ImVec2& lhs, const ImVec2& rhs)                { return ImVec2(lhs.x >= rhs.x ? lhs.x : rhs.x, lhs.y >= rhs.y ? lhs.y : rhs.y); }
static inline ImVec2 ImClamp(const ImVec2& v, const ImVec2& mn, ImVec2 mx)      { return ImVec2((v.x < mn.x) ? mn.x : (v.x > mx.x) ? mx.x : v.x, (v.y < mn.y) ? mn.y : (v.y > mx.y) ? mx.y : v.y); }
static inline ImVec2 ImLerp(const ImVec2& a, const ImVec2& b, float t)          { return ImVec2(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t); }
static inline ImVec2 ImLerp(const ImVec2& a, const ImVec2& b, const ImVec2& t)  { return ImVec2(a.x + (b.x - a.x) * t.x, a.y + (b.y - a.y) * t.y); }
static inline ImVec4 ImLerp(const ImVec4& a, const ImVec4& b, float t)          { return ImVec4(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t, a.w + (b.w - a.w) * t); }
static inline float  ImSaturate(float f)                                        { return (f < 0.0f) ? 0.0f : (f > 1.0f) ? 1.0f : f; }
static inline float  ImLengthSqr(const ImVec2& lhs)                             { return (lhs.x * lhs.x) + (lhs.y * lhs.y); }
static inline float  ImLengthSqr(const ImVec4& lhs)                             { return (lhs.x * lhs.x) + (lhs.y * lhs.y) + (lhs.z * lhs.z) + (lhs.w * lhs.w); }
static inline float  ImLength(const ImVec2& lhs, float fail_value)              { float d = (lhs.x * lhs.x) + (lhs.y * lhs.y); if (d > 0.0f) return ImSqrt(d); return fail_value; }
static inline float  ImInvLength(const ImVec2& lhs, float fail_value)           { float d = (lhs.x * lhs.x) + (lhs.y * lhs.y); if (d > 0.0f) return ImRsqrt(d); return fail_value; }
static inline float  ImFloor(float f)                                           { return (float)(int)(f); }
static inline float  ImFloorSigned(float f)                                     { return (float)((f >= 0 || (float)(int)f == f) ? (int)f : (int)f - 1); } // Decent replacement for floorf()
static inline ImVec2 ImFloor(const ImVec2& v)                                   { return ImVec2((float)(int)(v.x), (float)(int)(v.y)); }
static inline ImVec2 ImFloorSigned(const ImVec2& v)                             { return ImVec2(ImFloorSigned(v.x), ImFloorSigned(v.y)); }
static inline int    ImModPositive(int a, int b)                                { return (a + b) % b; }
static inline float  ImDot(const ImVec2& a, const ImVec2& b)                    { return a.x * b.x + a.y * b.y; }
static inline ImVec2 ImRotate(const ImVec2& v, float cos_a, float sin_a)        { return ImVec2(v.x * cos_a - v.y * sin_a, v.x * sin_a + v.y * cos_a); }
static inline float  ImLinearSweep(float current, float target, float speed)    { if (current < target) return ImMin(current + speed, target); if (current > target) return ImMax(current - speed, target); return current; }
static inline ImVec2 ImMul(const ImVec2& lhs, const ImVec2& rhs)                { return ImVec2(lhs.x * rhs.x, lhs.y * rhs.y); }
static inline bool   ImIsFloatAboveGuaranteedIntegerPrecision(float f)          { return f <= -16777216 || f >= 16777216; }
static inline float  ImExponentialMovingAverage(float avg, float sample, int n) { avg -= avg / n; avg += sample / n; return avg; }
IM_MSVC_RUNTIME_CHECKS_RESTORE

// Helpers: Geometry
SEGAUI_API ImVec2     ImBezierCubicCalc(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, float t);
SEGAUI_API ImVec2     ImBezierCubicClosestPoint(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, const ImVec2& p, int num_segments);       // For curves with explicit number of segments
SEGAUI_API ImVec2     ImBezierCubicClosestPointCasteljau(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, const ImVec2& p, float tess_tol);// For auto-tessellated curves you can use tess_tol = style.CurveTessellationTol
SEGAUI_API ImVec2     ImBezierQuadraticCalc(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, float t);
SEGAUI_API ImVec2     ImLineClosestPoint(const ImVec2& a, const ImVec2& b, const ImVec2& p);
SEGAUI_API bool       ImTriangleContainsPoint(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& p);
SEGAUI_API ImVec2     ImTriangleClosestPoint(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& p);
SEGAUI_API void       ImTriangleBarycentricCoords(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& p, float& out_u, float& out_v, float& out_w);
inline float         ImTriangleArea(const ImVec2& a, const ImVec2& b, const ImVec2& c) { return ImFabs((a.x * (b.y - c.y)) + (b.x * (c.y - a.y)) + (c.x * (a.y - b.y))) * 0.5f; }

// Helper: ImVec1 (1D vector)
// (this odd construct is used to facilitate the transition between 1D and 2D, and the maintenance of some branches/patches)
IM_MSVC_RUNTIME_CHECKS_OFF
struct ImVec1
{
    float   x;
    constexpr ImVec1()         : x(0.0f) { }
    constexpr ImVec1(float _x) : x(_x) { }
};

// Helper: ImVec2ih (2D vector, half-size integer, for long-term packed storage)
struct ImVec2ih
{
    short   x, y;
    constexpr ImVec2ih()                           : x(0), y(0) {}
    constexpr ImVec2ih(short _x, short _y)         : x(_x), y(_y) {}
    constexpr explicit ImVec2ih(const ImVec2& rhs) : x((short)rhs.x), y((short)rhs.y) {}
};

// Helper: ImRect (2D axis aligned bounding-box)
// NB: we can't rely on ImVec2 math operators being available here!
struct SEGAUI_API ImRect
{
    ImVec2      Min;    // Upper-left
    ImVec2      Max;    // Lower-right

    constexpr ImRect()                                        : Min(0.0f, 0.0f), Max(0.0f, 0.0f)  {}
    constexpr ImRect(const ImVec2& min, const ImVec2& max)    : Min(min), Max(max)                {}
    constexpr ImRect(const ImVec4& v)                         : Min(v.x, v.y), Max(v.z, v.w)      {}
    constexpr ImRect(float x1, float y1, float x2, float y2)  : Min(x1, y1), Max(x2, y2)          {}

    ImVec2      GetCenter() const                   { return ImVec2((Min.x + Max.x) * 0.5f, (Min.y + Max.y) * 0.5f); }
    ImVec2      GetSize() const                     { return ImVec2(Max.x - Min.x, Max.y - Min.y); }
    float       GetWidth() const                    { return Max.x - Min.x; }
    float       GetHeight() const                   { return Max.y - Min.y; }
    float       GetArea() const                     { return (Max.x - Min.x) * (Max.y - Min.y); }
    ImVec2      GetTL() const                       { return Min; }                   // Top-left
    ImVec2      GetTR() const                       { return ImVec2(Max.x, Min.y); }  // Top-right
    ImVec2      GetBL() const                       { return ImVec2(Min.x, Max.y); }  // Bottom-left
    ImVec2      GetBR() const                       { return Max; }                   // Bottom-right
    bool        Contains(const ImVec2& p) const     { return p.x     >= Min.x && p.y     >= Min.y && p.x     <  Max.x && p.y     <  Max.y; }
    bool        Contains(const ImRect& r) const     { return r.Min.x >= Min.x && r.Min.y >= Min.y && r.Max.x <= Max.x && r.Max.y <= Max.y; }
    bool        Overlaps(const ImRect& r) const     { return r.Min.y <  Max.y && r.Max.y >  Min.y && r.Min.x <  Max.x && r.Max.x >  Min.x; }
    void        Add(const ImVec2& p)                { if (Min.x > p.x)     Min.x = p.x;     if (Min.y > p.y)     Min.y = p.y;     if (Max.x < p.x)     Max.x = p.x;     if (Max.y < p.y)     Max.y = p.y; }
    void        Add(const ImRect& r)                { if (Min.x > r.Min.x) Min.x = r.Min.x; if (Min.y > r.Min.y) Min.y = r.Min.y; if (Max.x < r.Max.x) Max.x = r.Max.x; if (Max.y < r.Max.y) Max.y = r.Max.y; }
    void        Expand(const float amount)          { Min.x -= amount;   Min.y -= amount;   Max.x += amount;   Max.y += amount; }
    void        Expand(const ImVec2& amount)        { Min.x -= amount.x; Min.y -= amount.y; Max.x += amount.x; Max.y += amount.y; }
    void        Translate(const ImVec2& d)          { Min.x += d.x; Min.y += d.y; Max.x += d.x; Max.y += d.y; }
    void        TranslateX(float dx)                { Min.x += dx; Max.x += dx; }
    void        TranslateY(float dy)                { Min.y += dy; Max.y += dy; }
    void        ClipWith(const ImRect& r)           { Min = ImMax(Min, r.Min); Max = ImMin(Max, r.Max); }                   // Simple version, may lead to an inverted rectangle, which is fine for Contains/Overlaps test but not for display.
    void        ClipWithFull(const ImRect& r)       { Min = ImClamp(Min, r.Min, r.Max); Max = ImClamp(Max, r.Min, r.Max); } // Full version, ensure both points are fully clipped.
    void        Floor()                             { Min.x = IM_FLOOR(Min.x); Min.y = IM_FLOOR(Min.y); Max.x = IM_FLOOR(Max.x); Max.y = IM_FLOOR(Max.y); }
    bool        IsInverted() const                  { return Min.x > Max.x || Min.y > Max.y; }
    ImVec4      ToVec4() const                      { return ImVec4(Min.x, Min.y, Max.x, Max.y); }
};

// Helper: ImBitArray
#define         IM_BITARRAY_TESTBIT(_ARRAY, _N)                 ((_ARRAY[(_N) >> 5] & ((ImU32)1 << ((_N) & 31))) != 0) // Macro version of ImBitArrayTestBit(): ensure args have side-effect or are costly!
#define         IM_BITARRAY_CLEARBIT(_ARRAY, _N)                ((_ARRAY[(_N) >> 5] &= ~((ImU32)1 << ((_N) & 31))))    // Macro version of ImBitArrayClearBit(): ensure args have side-effect or are costly!
inline size_t   ImBitArrayGetStorageSizeInBytes(int bitcount)   { return (size_t)((bitcount + 31) >> 5) << 2; }
inline void     ImBitArrayClearAllBits(ImU32* arr, int bitcount){ memset(arr, 0, ImBitArrayGetStorageSizeInBytes(bitcount)); }
inline bool     ImBitArrayTestBit(const ImU32* arr, int n)      { ImU32 mask = (ImU32)1 << (n & 31); return (arr[n >> 5] & mask) != 0; }
inline void     ImBitArrayClearBit(ImU32* arr, int n)           { ImU32 mask = (ImU32)1 << (n & 31); arr[n >> 5] &= ~mask; }
inline void     ImBitArraySetBit(ImU32* arr, int n)             { ImU32 mask = (ImU32)1 << (n & 31); arr[n >> 5] |= mask; }
inline void     ImBitArraySetBitRange(ImU32* arr, int n, int n2) // Works on range [n..n2)
{
    n2--;
    while (n <= n2)
    {
        int a_mod = (n & 31);
        int b_mod = (n2 > (n | 31) ? 31 : (n2 & 31)) + 1;
        ImU32 mask = (ImU32)(((ImU64)1 << b_mod) - 1) & ~(ImU32)(((ImU64)1 << a_mod) - 1);
        arr[n >> 5] |= mask;
        n = (n + 32) & ~31;
    }
}

typedef ImU32* ImBitArrayPtr; // Name for use in structs

// Helper: ImBitArray class (wrapper over ImBitArray functions)
// Store 1-bit per value.
template<int BITCOUNT, int OFFSET = 0>
struct ImBitArray
{
    ImU32           Storage[(BITCOUNT + 31) >> 5];
    ImBitArray()                                { ClearAllBits(); }
    void            ClearAllBits()              { memset(Storage, 0, sizeof(Storage)); }
    void            SetAllBits()                { memset(Storage, 255, sizeof(Storage)); }
    bool            TestBit(int n) const        { n += OFFSET; IM_ASSERT(n >= 0 && n < BITCOUNT); return IM_BITARRAY_TESTBIT(Storage, n); }
    void            SetBit(int n)               { n += OFFSET; IM_ASSERT(n >= 0 && n < BITCOUNT); ImBitArraySetBit(Storage, n); }
    void            ClearBit(int n)             { n += OFFSET; IM_ASSERT(n >= 0 && n < BITCOUNT); ImBitArrayClearBit(Storage, n); }
    void            SetBitRange(int n, int n2)  { n += OFFSET; n2 += OFFSET; IM_ASSERT(n >= 0 && n < BITCOUNT && n2 > n && n2 <= BITCOUNT); ImBitArraySetBitRange(Storage, n, n2); } // Works on range [n..n2)
    bool            operator[](int n) const     { n += OFFSET; IM_ASSERT(n >= 0 && n < BITCOUNT); return IM_BITARRAY_TESTBIT(Storage, n); }
};

// Helper: ImBitVector
// Store 1-bit per value.
struct SEGAUI_API ImBitVector
{
    ImVector<ImU32> Storage;
    void            Create(int sz)              { Storage.resize((sz + 31) >> 5); memset(Storage.Data, 0, (size_t)Storage.Size * sizeof(Storage.Data[0])); }
    void            Clear()                     { Storage.clear(); }
    bool            TestBit(int n) const        { IM_ASSERT(n < (Storage.Size << 5)); return IM_BITARRAY_TESTBIT(Storage.Data, n); }
    void            SetBit(int n)               { IM_ASSERT(n < (Storage.Size << 5)); ImBitArraySetBit(Storage.Data, n); }
    void            ClearBit(int n)             { IM_ASSERT(n < (Storage.Size << 5)); ImBitArrayClearBit(Storage.Data, n); }
};
IM_MSVC_RUNTIME_CHECKS_RESTORE

// Helper: ImSpan<>
// Pointing to a span of data we don't own.
template<typename T>
struct ImSpan
{
    T*                  Data;
    T*                  DataEnd;

    // Constructors, destructor
    inline ImSpan()                                 { Data = DataEnd = NULL; }
    inline ImSpan(T* data, int size)                { Data = data; DataEnd = data + size; }
    inline ImSpan(T* data, T* data_end)             { Data = data; DataEnd = data_end; }

    inline void         set(T* data, int size)      { Data = data; DataEnd = data + size; }
    inline void         set(T* data, T* data_end)   { Data = data; DataEnd = data_end; }
    inline int          size() const                { return (int)(ptrdiff_t)(DataEnd - Data); }
    inline int          size_in_bytes() const       { return (int)(ptrdiff_t)(DataEnd - Data) * (int)sizeof(T); }
    inline T&           operator[](int i)           { T* p = Data + i; IM_ASSERT(p >= Data && p < DataEnd); return *p; }
    inline const T&     operator[](int i) const     { const T* p = Data + i; IM_ASSERT(p >= Data && p < DataEnd); return *p; }

    inline T*           begin()                     { return Data; }
    inline const T*     begin() const               { return Data; }
    inline T*           end()                       { return DataEnd; }
    inline const T*     end() const                 { return DataEnd; }

    // Utilities
    inline int  index_from_ptr(const T* it) const   { IM_ASSERT(it >= Data && it < DataEnd); const ptrdiff_t off = it - Data; return (int)off; }
};

// Helper: ImSpanAllocator<>
// Facilitate storing multiple chunks into a single large block (the "arena")
// - Usage: call Reserve() N times, allocate GetArenaSizeInBytes() worth, pass it to SetArenaBasePtr(), call GetSpan() N times to retrieve the aligned ranges.
template<int CHUNKS>
struct ImSpanAllocator
{
    char*   BasePtr;
    int     CurrOff;
    int     CurrIdx;
    int     Offsets[CHUNKS];
    int     Sizes[CHUNKS];

    ImSpanAllocator()                               { memset(this, 0, sizeof(*this)); }
    inline void  Reserve(int n, size_t sz, int a=4) { IM_ASSERT(n == CurrIdx && n < CHUNKS); CurrOff = IM_MEMALIGN(CurrOff, a); Offsets[n] = CurrOff; Sizes[n] = (int)sz; CurrIdx++; CurrOff += (int)sz; }
    inline int   GetArenaSizeInBytes()              { return CurrOff; }
    inline void  SetArenaBasePtr(void* base_ptr)    { BasePtr = (char*)base_ptr; }
    inline void* GetSpanPtrBegin(int n)             { IM_ASSERT(n >= 0 && n < CHUNKS && CurrIdx == CHUNKS); return (void*)(BasePtr + Offsets[n]); }
    inline void* GetSpanPtrEnd(int n)               { IM_ASSERT(n >= 0 && n < CHUNKS && CurrIdx == CHUNKS); return (void*)(BasePtr + Offsets[n] + Sizes[n]); }
    template<typename T>
    inline void  GetSpan(int n, ImSpan<T>* span)    { span->set((T*)GetSpanPtrBegin(n), (T*)GetSpanPtrEnd(n)); }
};

// Helper: ImPool<>
// Basic keyed storage for contiguous instances, slow/amortized insertion, O(1) indexable, O(Log N) queries by ID over a dense/hot buffer,
// Honor constructor/destructor. Add/remove invalidate all pointers. Indexes have the same lifetime as the associated object.
typedef int ImPoolIdx;
template<typename T>
struct ImPool
{
    ImVector<T>     Buf;        // Contiguous data
    SegaUiStorage    Map;        // ID->Index
    ImPoolIdx       FreeIdx;    // Next free idx to use
    ImPoolIdx       AliveCount; // Number of active/alive items (for display purpose)

    ImPool()    { FreeIdx = AliveCount = 0; }
    ~ImPool()   { Clear(); }
    T*          GetByKey(SegaUiID key)               { int idx = Map.GetInt(key, -1); return (idx != -1) ? &Buf[idx] : NULL; }
    T*          GetByIndex(ImPoolIdx n)             { return &Buf[n]; }
    ImPoolIdx   GetIndex(const T* p) const          { IM_ASSERT(p >= Buf.Data && p < Buf.Data + Buf.Size); return (ImPoolIdx)(p - Buf.Data); }
    T*          GetOrAddByKey(SegaUiID key)          { int* p_idx = Map.GetIntRef(key, -1); if (*p_idx != -1) return &Buf[*p_idx]; *p_idx = FreeIdx; return Add(); }
    bool        Contains(const T* p) const          { return (p >= Buf.Data && p < Buf.Data + Buf.Size); }
    void        Clear()                             { for (int n = 0; n < Map.Data.Size; n++) { int idx = Map.Data[n].val_i; if (idx != -1) Buf[idx].~T(); } Map.Clear(); Buf.clear(); FreeIdx = AliveCount = 0; }
    T*          Add()                               { int idx = FreeIdx; if (idx == Buf.Size) { Buf.resize(Buf.Size + 1); FreeIdx++; } else { FreeIdx = *(int*)&Buf[idx]; } IM_PLACEMENT_NEW(&Buf[idx]) T(); AliveCount++; return &Buf[idx]; }
    void        Remove(SegaUiID key, const T* p)     { Remove(key, GetIndex(p)); }
    void        Remove(SegaUiID key, ImPoolIdx idx)  { Buf[idx].~T(); *(int*)&Buf[idx] = FreeIdx; FreeIdx = idx; Map.SetInt(key, -1); AliveCount--; }
    void        Reserve(int capacity)               { Buf.reserve(capacity); Map.Data.reserve(capacity); }

    // To iterate a ImPool: for (int n = 0; n < pool.GetMapSize(); n++) if (T* t = pool.TryGetMapData(n)) { ... }
    // Can be avoided if you know .Remove() has never been called on the pool, or AliveCount == GetMapSize()
    int         GetAliveCount() const               { return AliveCount; }      // Number of active/alive items in the pool (for display purpose)
    int         GetBufSize() const                  { return Buf.Size; }
    int         GetMapSize() const                  { return Map.Data.Size; }   // It is the map we need iterate to find valid items, since we don't have "alive" storage anywhere
    T*          TryGetMapData(ImPoolIdx n)          { int idx = Map.Data[n].val_i; if (idx == -1) return NULL; return GetByIndex(idx); }
#ifndef SEGAUI_DISABLE_OBSOLETE_FUNCTIONS
    int         GetSize()                           { return GetMapSize(); } // For ImPlot: should use GetMapSize() from (SEGAUI_VERSION_NUM >= 18304)
#endif
};

// Helper: ImChunkStream<>
// Build and iterate a contiguous stream of variable-sized structures.
// This is used by Settings to store persistent data while reducing allocation count.
// We store the chunk size first, and align the final size on 4 bytes boundaries.
// The tedious/zealous amount of casting is to avoid -Wcast-align warnings.
template<typename T>
struct ImChunkStream
{
    ImVector<char>  Buf;

    void    clear()                     { Buf.clear(); }
    bool    empty() const               { return Buf.Size == 0; }
    int     size() const                { return Buf.Size; }
    T*      alloc_chunk(size_t sz)      { size_t HDR_SZ = 4; sz = IM_MEMALIGN(HDR_SZ + sz, 4u); int off = Buf.Size; Buf.resize(off + (int)sz); ((int*)(void*)(Buf.Data + off))[0] = (int)sz; return (T*)(void*)(Buf.Data + off + (int)HDR_SZ); }
    T*      begin()                     { size_t HDR_SZ = 4; if (!Buf.Data) return NULL; return (T*)(void*)(Buf.Data + HDR_SZ); }
    T*      next_chunk(T* p)            { size_t HDR_SZ = 4; IM_ASSERT(p >= begin() && p < end()); p = (T*)(void*)((char*)(void*)p + chunk_size(p)); if (p == (T*)(void*)((char*)end() + HDR_SZ)) return (T*)0; IM_ASSERT(p < end()); return p; }
    int     chunk_size(const T* p)      { return ((const int*)p)[-1]; }
    T*      end()                       { return (T*)(void*)(Buf.Data + Buf.Size); }
    int     offset_from_ptr(const T* p) { IM_ASSERT(p >= begin() && p < end()); const ptrdiff_t off = (const char*)p - Buf.Data; return (int)off; }
    T*      ptr_from_offset(int off)    { IM_ASSERT(off >= 4 && off < Buf.Size); return (T*)(void*)(Buf.Data + off); }
    void    swap(ImChunkStream<T>& rhs) { rhs.Buf.swap(Buf); }

};

// Helper: SegaUiTextIndex<>
// Maintain a line index for a text buffer. This is a strong candidate to be moved into the public API.
struct SegaUiTextIndex
{
    ImVector<int>   LineOffsets;
    int             EndOffset = 0;                          // Because we don't own text buffer we need to maintain EndOffset (may bake in LineOffsets?)

    void            clear()                                 { LineOffsets.clear(); EndOffset = 0; }
    int             size()                                  { return LineOffsets.Size; }
    const char*     get_line_begin(const char* base, int n) { return base + LineOffsets[n]; }
    const char*     get_line_end(const char* base, int n)   { return base + (n + 1 < LineOffsets.Size ? (LineOffsets[n + 1] - 1) : EndOffset); }
    void            append(const char* base, int old_size, int new_size);
};

//-----------------------------------------------------------------------------
// [SECTION] ImDrawList support
//-----------------------------------------------------------------------------

// ImDrawList: Helper function to calculate a circle's segment count given its radius and a "maximum error" value.
// Estimation of number of circle segment based on error is derived using method described in https://stackoverflow.com/a/2244088/15194693
// Number of segments (N) is calculated using equation:
//   N = ceil ( pi / acos(1 - error / r) )     where r > 0, error <= r
// Our equation is significantly simpler that one in the post thanks for choosing segment that is
// perpendicular to X axis. Follow steps in the article from this starting condition and you will
// will get this result.
//
// Rendering circles with an odd number of segments, while mathematically correct will produce
// asymmetrical results on the raster grid. Therefore we're rounding N to next even number (7->8, 8->8, 9->10 etc.)
#define IM_ROUNDUP_TO_EVEN(_V)                                  ((((_V) + 1) / 2) * 2)
#define IM_DRAWLIST_CIRCLE_AUTO_SEGMENT_MIN                     200
#define IM_DRAWLIST_CIRCLE_AUTO_SEGMENT_MAX                     2048
#define IM_DRAWLIST_CIRCLE_AUTO_SEGMENT_CALC(_RAD,_MAXERROR)    ImClamp(IM_ROUNDUP_TO_EVEN((int)ImCeil(IM_PI / ImAcos(1 - ImMin((_MAXERROR), (_RAD)) / (_RAD)))), IM_DRAWLIST_CIRCLE_AUTO_SEGMENT_MIN, IM_DRAWLIST_CIRCLE_AUTO_SEGMENT_MAX)

// Raw equation from IM_DRAWLIST_CIRCLE_AUTO_SEGMENT_CALC rewritten for 'r' and 'error'.
#define IM_DRAWLIST_CIRCLE_AUTO_SEGMENT_CALC_R(_N,_MAXERROR)    ((_MAXERROR) / (1 - ImCos(IM_PI / ImMax((float)(_N), IM_PI))))
#define IM_DRAWLIST_CIRCLE_AUTO_SEGMENT_CALC_ERROR(_N,_RAD)     ((1 - ImCos(IM_PI / ImMax((float)(_N), IM_PI))) / (_RAD))

// ImDrawList: Lookup table size for adaptive arc drawing, cover full circle.
#ifndef IM_DRAWLIST_ARCFAST_TABLE_SIZE
#define IM_DRAWLIST_ARCFAST_TABLE_SIZE                          48 // Number of samples in lookup table.
#endif
#define IM_DRAWLIST_ARCFAST_SAMPLE_MAX                          IM_DRAWLIST_ARCFAST_TABLE_SIZE // Sample index _PathArcToFastEx() for 360 angle.

// Data shared between all ImDrawList instances
// You may want to create your own instance of this if you want to use ImDrawList completely without SegaUi. In that case, watch out for future changes to this structure.
struct SEGAUI_API ImDrawListSharedData
{
    ImVec2          TexUvWhitePixel;            // UV of white pixel in the atlas
    ImFont*         Font;                       // Current/default font (optional, for simplified AddText overload)
    float           FontSize;                   // Current/default font size (optional, for simplified AddText overload)
    float           CurveTessellationTol;       // Tessellation tolerance when using PathBezierCurveTo()
    float           CircleSegmentMaxError;      // Number of circle segments to use per pixel of radius for AddCircle() etc
    ImVec4          ClipRectFullscreen;         // Value for PushClipRectFullscreen()
    ImDrawListFlags InitialFlags;               // Initial flags at the beginning of the frame (it is possible to alter flags on a per-drawlist basis afterwards)

    // [Internal] Temp write buffer
    ImVector<ImVec2> TempBuffer;

    // [Internal] Lookup tables
    ImVec2          ArcFastVtx[IM_DRAWLIST_ARCFAST_TABLE_SIZE]; // Sample points on the quarter of the circle.
    float           ArcFastRadiusCutoff;                        // Cutoff radius after which arc drawing will fallback to slower PathArcTo()
    ImU8            CircleSegmentCounts[64];    // Precomputed segment count for given radius before we calculate it dynamically (to avoid calculation overhead)
    const ImVec4*   TexUvLines;                 // UV of anti-aliased lines in the atlas

    int*            ShadowRectIds;              // IDs of rects for shadow texture (2 entries)
    const ImVec4*   ShadowRectUvs;              // UV coordinates for shadow texture (10 entries)

    ImDrawListSharedData();
    void SetCircleTessellationMaxError(float max_error);
};

struct ImDrawDataBuilder
{
    ImVector<ImDrawList*>   Layers[2];           // Global layers for: regular, tooltip

    void Clear()                    { for (int n = 0; n < IM_ARRAYSIZE(Layers); n++) Layers[n].resize(0); }
    void ClearFreeMemory()          { for (int n = 0; n < IM_ARRAYSIZE(Layers); n++) Layers[n].clear(); }
    int  GetDrawListCount() const   { int count = 0; for (int n = 0; n < IM_ARRAYSIZE(Layers); n++) count += Layers[n].Size; return count; }
    SEGAUI_API void FlattenIntoSingleLayer();
};

//-----------------------------------------------------------------------------
// [SECTION] Widgets support: flags, enums, data structures
//-----------------------------------------------------------------------------

// Flags used by upcoming items
// - input: PushItemFlag() manipulates g.CurrentItemFlags, ItemAdd() calls may add extra flags.
// - output: stored in g.LastItemData.InFlags
// Current window shared by all windows.
// This is going to be exposed in imgui.h when stabilized enough.
enum SegaUiItemFlags_
{
    // Controlled by user
    SegaUiItemFlags_None                     = 0,
    SegaUiItemFlags_NoTabStop                = 1 << 0,  // false     // Disable keyboard tabbing. This is a "lighter" version of SegaUiItemFlags_NoNav.
    SegaUiItemFlags_ButtonRepeat             = 1 << 1,  // false     // Button() will return true multiple times based on io.KeyRepeatDelay and io.KeyRepeatRate settings.
    SegaUiItemFlags_Disabled                 = 1 << 2,  // false     // Disable interactions but doesn't affect visuals. See BeginDisabled()/EndDisabled(). See github.com/ocornut/imgui/issues/211
    SegaUiItemFlags_NoNav                    = 1 << 3,  // false     // Disable any form of focusing (keyboard/gamepad directional navigation and SetKeyboardFocusHere() calls)
    SegaUiItemFlags_NoNavDefaultFocus        = 1 << 4,  // false     // Disable item being a candidate for default focus (e.g. used by title bar items)
    SegaUiItemFlags_SelectableDontClosePopup = 1 << 5,  // false     // Disable MenuItem/Selectable() automatically closing their popup window
    SegaUiItemFlags_MixedValue               = 1 << 6,  // false     // [BETA] Represent a mixed/indeterminate value, generally multi-selection where values differ. Currently only supported by Checkbox() (later should support all sorts of widgets)
    SegaUiItemFlags_ReadOnly                 = 1 << 7,  // false     // [ALPHA] Allow hovering interactions but underlying value is not changed.
    SegaUiItemFlags_NoWindowHoverableCheck   = 1 << 8,  // false     // Disable hoverable check in ItemHoverable()

    // Controlled by widget code
    SegaUiItemFlags_Inputable                = 1 << 10, // false     // [WIP] Auto-activate input mode when tab focused. Currently only used and supported by a few items before it becomes a generic feature.
};

// Status flags for an already submitted item
// - output: stored in g.LastItemData.StatusFlags
enum SegaUiItemStatusFlags_
{
    SegaUiItemStatusFlags_None               = 0,
    SegaUiItemStatusFlags_HoveredRect        = 1 << 0,   // Mouse position is within item rectangle (does NOT mean that the window is in correct z-order and can be hovered!, this is only one part of the most-common IsItemHovered test)
    SegaUiItemStatusFlags_HasDisplayRect     = 1 << 1,   // g.LastItemData.DisplayRect is valid
    SegaUiItemStatusFlags_Edited             = 1 << 2,   // Value exposed by item was edited in the current frame (should match the bool return value of most widgets)
    SegaUiItemStatusFlags_ToggledSelection   = 1 << 3,   // Set when Selectable(), TreeNode() reports toggling a selection. We can't report "Selected", only state changes, in order to easily handle clipping with less issues.
    SegaUiItemStatusFlags_ToggledOpen        = 1 << 4,   // Set when TreeNode() reports toggling their open state.
    SegaUiItemStatusFlags_HasDeactivated     = 1 << 5,   // Set if the widget/group is able to provide data for the SegaUiItemStatusFlags_Deactivated flag.
    SegaUiItemStatusFlags_Deactivated        = 1 << 6,   // Only valid if SegaUiItemStatusFlags_HasDeactivated is set.
    SegaUiItemStatusFlags_HoveredWindow      = 1 << 7,   // Override the HoveredWindow test to allow cross-window hover testing.
    SegaUiItemStatusFlags_FocusedByTabbing   = 1 << 8,   // Set when the Focusable item just got focused by Tabbing (FIXME: to be removed soon)
    SegaUiItemStatusFlags_Visible            = 1 << 9,   // [WIP] Set when item is overlapping the current clipping rectangle (Used internally. Please don't use yet: API/system will change as we refactor Itemadd()).

    // Additional status + semantic for SegaUiTestEngine
#ifdef SEGAUI_ENABLE_TEST_ENGINE
    SegaUiItemStatusFlags_Openable           = 1 << 20,  // Item is an openable (e.g. TreeNode)
    SegaUiItemStatusFlags_Opened             = 1 << 21,  // Opened status
    SegaUiItemStatusFlags_Checkable          = 1 << 22,  // Item is a checkable (e.g. CheckBox, MenuItem)
    SegaUiItemStatusFlags_Checked            = 1 << 23,  // Checked status
    SegaUiItemStatusFlags_Inputable          = 1 << 24,  // Item is a text-inputable (e.g. InputText, SliderXXX, DragXXX)
#endif
};

// Extend SegaUiInputTextFlags_
enum SegaUiInputTextFlagsPrivate_
{
    // [Internal]
    SegaUiInputTextFlags_Multiline           = 1 << 26,  // For internal use by InputTextMultiline()
    SegaUiInputTextFlags_NoMarkEdited        = 1 << 27,  // For internal use by functions using InputText() before reformatting data
    SegaUiInputTextFlags_MergedItem          = 1 << 28,  // For internal use by TempInputText(), will skip calling ItemAdd(). Require bounding-box to strictly match.
};

// Extend SegaUiButtonFlags_
enum SegaUiButtonFlagsPrivate_
{
    SegaUiButtonFlags_PressedOnClick         = 1 << 4,   // return true on click (mouse down event)
    SegaUiButtonFlags_PressedOnClickRelease  = 1 << 5,   // [Default] return true on click + release on same item <-- this is what the majority of Button are using
    SegaUiButtonFlags_PressedOnClickReleaseAnywhere = 1 << 6, // return true on click + release even if the release event is not done while hovering the item
    SegaUiButtonFlags_PressedOnRelease       = 1 << 7,   // return true on release (default requires click+release)
    SegaUiButtonFlags_PressedOnDoubleClick   = 1 << 8,   // return true on double-click (default requires click+release)
    SegaUiButtonFlags_PressedOnDragDropHold  = 1 << 9,   // return true when held into while we are drag and dropping another item (used by e.g. tree nodes, collapsing headers)
    SegaUiButtonFlags_Repeat                 = 1 << 10,  // hold to repeat
    SegaUiButtonFlags_FlattenChildren        = 1 << 11,  // allow interactions even if a child window is overlapping
    SegaUiButtonFlags_AllowItemOverlap       = 1 << 12,  // require previous frame HoveredId to either match id or be null before being usable, use along with SetItemAllowOverlap()
    SegaUiButtonFlags_DontClosePopups        = 1 << 13,  // disable automatically closing parent popup on press // [UNUSED]
    //SegaUiButtonFlags_Disabled             = 1 << 14,  // disable interactions -> use BeginDisabled() or SegaUiItemFlags_Disabled
    SegaUiButtonFlags_AlignTextBaseLine      = 1 << 15,  // vertically align button to match text baseline - ButtonEx() only // FIXME: Should be removed and handled by SmallButton(), not possible currently because of DC.CursorPosPrevLine
    SegaUiButtonFlags_NoKeyModifiers         = 1 << 16,  // disable mouse interaction if a key modifier is held
    SegaUiButtonFlags_NoHoldingActiveId      = 1 << 17,  // don't set ActiveId while holding the mouse (SegaUiButtonFlags_PressedOnClick only)
    SegaUiButtonFlags_NoNavFocus             = 1 << 18,  // don't override navigation focus when activated (FIXME: this is essentially used everytime an item uses SegaUiItemFlags_NoNav, but because legacy specs don't requires LastItemData to be set ButtonBehavior(), we can't poll g.LastItemData.InFlags)
    SegaUiButtonFlags_NoHoveredOnFocus       = 1 << 19,  // don't report as hovered when nav focus is on this item
    SegaUiButtonFlags_NoSetKeyOwner          = 1 << 20,  // don't set key/input owner on the initial click (note: mouse buttons are keys! often, the key in question will be SegaUiKey_MouseLeft!)
    SegaUiButtonFlags_NoTestKeyOwner         = 1 << 21,  // don't test key/input owner when polling the key (note: mouse buttons are keys! often, the key in question will be SegaUiKey_MouseLeft!)
    SegaUiButtonFlags_PressedOnMask_         = SegaUiButtonFlags_PressedOnClick | SegaUiButtonFlags_PressedOnClickRelease | SegaUiButtonFlags_PressedOnClickReleaseAnywhere | SegaUiButtonFlags_PressedOnRelease | SegaUiButtonFlags_PressedOnDoubleClick | SegaUiButtonFlags_PressedOnDragDropHold,
    SegaUiButtonFlags_PressedOnDefault_      = SegaUiButtonFlags_PressedOnClickRelease,
};

// Extend SegaUiComboFlags_
enum SegaUiComboFlagsPrivate_
{
    SegaUiComboFlags_CustomPreview           = 1 << 20,  // enable BeginComboPreview()
};

// Extend SegaUiSliderFlags_
enum SegaUiSliderFlagsPrivate_
{
    SegaUiSliderFlags_Vertical               = 1 << 20,  // Should this slider be orientated vertically?
    SegaUiSliderFlags_ReadOnly               = 1 << 21,
};

// Extend SegaUiSelectableFlags_
enum SegaUiSelectableFlagsPrivate_
{
    // NB: need to be in sync with last value of SegaUiSelectableFlags_
    SegaUiSelectableFlags_NoHoldingActiveID      = 1 << 20,
    SegaUiSelectableFlags_SelectOnNav            = 1 << 21,  // (WIP) Auto-select when moved into. This is not exposed in public API as to handle multi-select and modifiers we will need user to explicitly control focus scope. May be replaced with a BeginSelection() API.
    SegaUiSelectableFlags_SelectOnClick          = 1 << 22,  // Override button behavior to react on Click (default is Click+Release)
    SegaUiSelectableFlags_SelectOnRelease        = 1 << 23,  // Override button behavior to react on Release (default is Click+Release)
    SegaUiSelectableFlags_SpanAvailWidth         = 1 << 24,  // Span all avail width even if we declared less for layout purpose. FIXME: We may be able to remove this (added in 6251d379, 2bcafc86 for menus)
    SegaUiSelectableFlags_SetNavIdOnHover        = 1 << 25,  // Set Nav/Focus ID on mouse hover (used by MenuItem)
    SegaUiSelectableFlags_NoPadWithHalfSpacing   = 1 << 26,  // Disable padding each side with ItemSpacing * 0.5f
    SegaUiSelectableFlags_NoSetKeyOwner          = 1 << 27,  // Don't set key/input owner on the initial click (note: mouse buttons are keys! often, the key in question will be SegaUiKey_MouseLeft!)
};

// Extend SegaUiTreeNodeFlags_
enum SegaUiTreeNodeFlagsPrivate_
{
    SegaUiTreeNodeFlags_ClipLabelForTrailingButton = 1 << 20,
};

enum SegaUiSeparatorFlags_
{
    SegaUiSeparatorFlags_None                    = 0,
    SegaUiSeparatorFlags_Horizontal              = 1 << 0,   // Axis default to current layout type, so generally Horizontal unless e.g. in a menu bar
    SegaUiSeparatorFlags_Vertical                = 1 << 1,
    SegaUiSeparatorFlags_SpanAllColumns          = 1 << 2,
};

// Flags for FocusWindow(). This is not called SegaUiFocusFlags to avoid confusion with public-facing SegaUiFocusedFlags.
// FIXME: Once we finishing replacing more uses of GetTopMostPopupModal()+IsWindowWithinBeginStackOf()
// and FindBlockingModal() with this, we may want to change the flag to be opt-out instead of opt-in.
enum SegaUiFocusRequestFlags_
{
    SegaUiFocusRequestFlags_None                 = 0,
    SegaUiFocusRequestFlags_RestoreFocusedChild  = 1 << 0,   // Find last focused child (if any) and focus it instead.
    SegaUiFocusRequestFlags_UnlessBelowModal     = 1 << 1,   // Do not set focus if the window is below a modal.
};

enum SegaUiTextFlags_
{
    SegaUiTextFlags_None                         = 0,
    SegaUiTextFlags_NoWidthForLargeClippedText   = 1 << 0,
};

enum SegaUiTooltipFlags_
{
    SegaUiTooltipFlags_None                      = 0,
    SegaUiTooltipFlags_OverridePreviousTooltip   = 1 << 0,   // Override will clear/ignore previously submitted tooltip (defaults to append)
};

// FIXME: this is in development, not exposed/functional as a generic feature yet.
// Horizontal/Vertical enums are fixed to 0/1 so they may be used to index ImVec2
enum SegaUiLayoutType_
{
    SegaUiLayoutType_Horizontal = 0,
    SegaUiLayoutType_Vertical = 1
};

enum SegaUiLogType
{
    SegaUiLogType_None = 0,
    SegaUiLogType_TTY,
    SegaUiLogType_File,
    SegaUiLogType_Buffer,
    SegaUiLogType_Clipboard,
};

// X/Y enums are fixed to 0/1 so they may be used to index ImVec2
enum SegaUiAxis
{
    SegaUiAxis_None = -1,
    SegaUiAxis_X = 0,
    SegaUiAxis_Y = 1
};

enum SegaUiPlotType
{
    SegaUiPlotType_Lines,
    SegaUiPlotType_Histogram,
};

enum SegaUiPopupPositionPolicy
{
    SegaUiPopupPositionPolicy_Default,
    SegaUiPopupPositionPolicy_ComboBox,
    SegaUiPopupPositionPolicy_Tooltip,
};

struct SegaUiDataVarInfo
{
    SegaUiDataType   Type;
    ImU32           Count;      // 1+
    ImU32           Offset;     // Offset in parent structure
    void* GetVarPtr(void* parent) const { return (void*)((unsigned char*)parent + Offset); }
};

struct SegaUiDataTypeTempStorage
{
    ImU8        Data[8];        // Can fit any data up to SegaUiDataType_COUNT
};

// Type information associated to one SegaUiDataType. Retrieve with DataTypeGetInfo().
struct SegaUiDataTypeInfo
{
    size_t      Size;           // Size in bytes
    const char* Name;           // Short descriptive name for the type, for debugging
    const char* PrintFmt;       // Default printf format for the type
    const char* ScanFmt;        // Default scanf format for the type
};

// Extend SegaUiDataType_
enum SegaUiDataTypePrivate_
{
    SegaUiDataType_String = SegaUiDataType_COUNT + 1,
    SegaUiDataType_Pointer,
    SegaUiDataType_ID,
};

// Stacked color modifier, backup of modified data so we can restore it
struct SegaUiColorMod
{
    SegaUiCol        Col;
    ImVec4          BackupValue;
};

// Stacked style modifier, backup of modified data so we can restore it. Data type inferred from the variable.
struct SegaUiStyleMod
{
    SegaUiStyleVar   VarIdx;
    union           { int BackupInt[2]; float BackupFloat[2]; };
    SegaUiStyleMod(SegaUiStyleVar idx, int v)     { VarIdx = idx; BackupInt[0] = v; }
    SegaUiStyleMod(SegaUiStyleVar idx, float v)   { VarIdx = idx; BackupFloat[0] = v; }
    SegaUiStyleMod(SegaUiStyleVar idx, ImVec2 v)  { VarIdx = idx; BackupFloat[0] = v.x; BackupFloat[1] = v.y; }
};

// Storage data for BeginComboPreview()/EndComboPreview()
struct SEGAUI_API SegaUiComboPreviewData
{
    ImRect          PreviewRect;
    ImVec2          BackupCursorPos;
    ImVec2          BackupCursorMaxPos;
    ImVec2          BackupCursorPosPrevLine;
    float           BackupPrevLineTextBaseOffset;
    SegaUiLayoutType BackupLayout;

    SegaUiComboPreviewData() { memset(this, 0, sizeof(*this)); }
};

// Stacked storage data for BeginGroup()/EndGroup()
struct SEGAUI_API SegaUiGroupData
{
    SegaUiID     WindowID;
    ImVec2      BackupCursorPos;
    ImVec2      BackupCursorMaxPos;
    ImVec1      BackupIndent;
    ImVec1      BackupGroupOffset;
    ImVec2      BackupCurrLineSize;
    float       BackupCurrLineTextBaseOffset;
    SegaUiID     BackupActiveIdIsAlive;
    bool        BackupActiveIdPreviousFrameIsAlive;
    bool        BackupHoveredIdIsAlive;
    bool        EmitItem;
};

// Simple column measurement, currently used for MenuItem() only.. This is very short-sighted/throw-away code and NOT a generic helper.
struct SEGAUI_API SegaUiMenuColumns
{
    ImU32       TotalWidth;
    ImU32       NextTotalWidth;
    ImU16       Spacing;
    ImU16       OffsetIcon;         // Always zero for now
    ImU16       OffsetLabel;        // Offsets are locked in Update()
    ImU16       OffsetShortcut;
    ImU16       OffsetMark;
    ImU16       Widths[4];          // Width of:   Icon, Label, Shortcut, Mark  (accumulators for current frame)

    SegaUiMenuColumns() { memset(this, 0, sizeof(*this)); }
    void        Update(float spacing, bool window_reappearing);
    float       DeclColumns(float w_icon, float w_label, float w_shortcut, float w_mark);
    void        CalcNextTotalWidth(bool update_offsets);
};

// Internal temporary state for deactivating InputText() instances.
struct SEGAUI_API SegaUiInputTextDeactivatedState
{
    SegaUiID            ID;              // widget id owning the text state (which just got deactivated)
    ImVector<char>     TextA;           // text buffer

    SegaUiInputTextDeactivatedState()    { memset(this, 0, sizeof(*this)); }
    void    ClearFreeMemory()           { ID = 0; TextA.clear(); }
};
// Internal state of the currently focused/edited text input box
// For a given item ID, access with SegaUi::GetInputTextState()
struct SEGAUI_API SegaUiInputTextState
{
    SegaUiContext*           Ctx;                    // parent UI context (needs to be set explicitly by parent).
    SegaUiID                 ID;                     // widget id owning the text state
    int                     CurLenW, CurLenA;       // we need to maintain our buffer length in both UTF-8 and wchar format. UTF-8 length is valid even if TextA is not.
    ImVector<ImWchar>       TextW;                  // edit buffer, we need to persist but can't guarantee the persistence of the user-provided buffer. so we copy into own buffer.
    ImVector<char>          TextA;                  // temporary UTF8 buffer for callbacks and other operations. this is not updated in every code-path! size=capacity.
    ImVector<char>          InitialTextA;           // backup of end-user buffer at the time of focus (in UTF-8, unaltered)
    bool                    TextAIsValid;           // temporary UTF8 buffer is not initially valid before we make the widget active (until then we pull the data from user argument)
    int                     BufCapacityA;           // end-user buffer capacity
    float                   ScrollX;                // horizontal scrolling/offset
    ImStb::STB_TexteditState Stb;                   // state for stb_textedit.h
    float                   CursorAnim;             // timer for cursor blink, reset on every user action so the cursor reappears immediately
    bool                    CursorFollow;           // set when we want scrolling to follow the current cursor position (not always!)
    bool                    SelectedAllMouseLock;   // after a double-click to select all, we ignore further mouse drags to update selection
    bool                    Edited;                 // edited this frame
    SegaUiInputTextFlags     Flags;                  // copy of InputText() flags. may be used to check if e.g. SegaUiInputTextFlags_Password is set.

    SegaUiInputTextState()                   { memset(this, 0, sizeof(*this)); }
    void        ClearText()                 { CurLenW = CurLenA = 0; TextW[0] = 0; TextA[0] = 0; CursorClamp(); }
    void        ClearFreeMemory()           { TextW.clear(); TextA.clear(); InitialTextA.clear(); }
    int         GetUndoAvailCount() const   { return Stb.undostate.undo_point; }
    int         GetRedoAvailCount() const   { return STB_TEXTEDIT_UNDOSTATECOUNT - Stb.undostate.redo_point; }
    void        OnKeyPressed(int key);      // Cannot be inline because we call in code in stb_textedit.h implementation

    // Cursor & Selection
    void        CursorAnimReset()           { CursorAnim = -0.30f; }                                   // After a user-input the cursor stays on for a while without blinking
    void        CursorClamp()               { Stb.cursor = ImMin(Stb.cursor, CurLenW); Stb.select_start = ImMin(Stb.select_start, CurLenW); Stb.select_end = ImMin(Stb.select_end, CurLenW); }
    bool        HasSelection() const        { return Stb.select_start != Stb.select_end; }
    void        ClearSelection()            { Stb.select_start = Stb.select_end = Stb.cursor; }
    int         GetCursorPos() const        { return Stb.cursor; }
    int         GetSelectionStart() const   { return Stb.select_start; }
    int         GetSelectionEnd() const     { return Stb.select_end; }
    void        SelectAll()                 { Stb.select_start = 0; Stb.cursor = Stb.select_end = CurLenW; Stb.has_preferred_x = 0; }
};

// Storage for current popup stack
struct SegaUiPopupData
{
    SegaUiID             PopupId;        // Set on OpenPopup()
    SegaUiWindow*        Window;         // Resolved on BeginPopup() - may stay unresolved if user never calls OpenPopup()
    SegaUiWindow*        BackupNavWindow;// Set on OpenPopup(), a NavWindow that will be restored on popup close
    int                 ParentNavLayer; // Resolved on BeginPopup(). Actually a SegaUiNavLayer type (declared down below), initialized to -1 which is not part of an enum, but serves well-enough as "not any of layers" value
    int                 OpenFrameCount; // Set on OpenPopup()
    SegaUiID             OpenParentId;   // Set on OpenPopup(), we need this to differentiate multiple menu sets from each others (e.g. inside menu bar vs loose menu items)
    ImVec2              OpenPopupPos;   // Set on OpenPopup(), preferred popup position (typically == OpenMousePos when using mouse)
    ImVec2              OpenMousePos;   // Set on OpenPopup(), copy of mouse position at the time of opening popup

    SegaUiPopupData()    { memset(this, 0, sizeof(*this)); ParentNavLayer = OpenFrameCount = -1; }
};

enum SegaUiNextWindowDataFlags_
{
    SegaUiNextWindowDataFlags_None               = 0,
    SegaUiNextWindowDataFlags_HasPos             = 1 << 0,
    SegaUiNextWindowDataFlags_HasSize            = 1 << 1,
    SegaUiNextWindowDataFlags_HasContentSize     = 1 << 2,
    SegaUiNextWindowDataFlags_HasCollapsed       = 1 << 3,
    SegaUiNextWindowDataFlags_HasSizeConstraint  = 1 << 4,
    SegaUiNextWindowDataFlags_HasFocus           = 1 << 5,
    SegaUiNextWindowDataFlags_HasBgAlpha         = 1 << 6,
    SegaUiNextWindowDataFlags_HasScroll          = 1 << 7,
};

// Storage for SetNexWindow** functions
struct SegaUiNextWindowData
{
    SegaUiNextWindowDataFlags    Flags;
    SegaUiCond                   PosCond;
    SegaUiCond                   SizeCond;
    SegaUiCond                   CollapsedCond;
    ImVec2                      PosVal;
    ImVec2                      PosPivotVal;
    ImVec2                      SizeVal;
    ImVec2                      ContentSizeVal;
    ImVec2                      ScrollVal;
    bool                        CollapsedVal;
    ImRect                      SizeConstraintRect;
    SegaUiSizeCallback           SizeCallback;
    void*                       SizeCallbackUserData;
    float                       BgAlphaVal;             // Override background alpha
    ImVec2                      MenuBarOffsetMinVal;    // (Always on) This is not exposed publicly, so we don't clear it and it doesn't have a corresponding flag (could we? for consistency?)

    SegaUiNextWindowData()       { memset(this, 0, sizeof(*this)); }
    inline void ClearFlags()    { Flags = SegaUiNextWindowDataFlags_None; }
};

enum SegaUiNextItemDataFlags_
{
    SegaUiNextItemDataFlags_None     = 0,
    SegaUiNextItemDataFlags_HasWidth = 1 << 0,
    SegaUiNextItemDataFlags_HasOpen  = 1 << 1,
};

struct SegaUiNextItemData
{
    SegaUiNextItemDataFlags      Flags;
    float                       Width;          // Set by SetNextItemWidth()
    SegaUiID                     FocusScopeId;   // Set by SetNextItemMultiSelectData() (!= 0 signify value has been set, so it's an alternate version of HasSelectionData, we don't use Flags for this because they are cleared too early. This is mostly used for debugging)
    SegaUiCond                   OpenCond;
    bool                        OpenVal;        // Set by SetNextItemOpen()

    SegaUiNextItemData()         { memset(this, 0, sizeof(*this)); }
    inline void ClearFlags()    { Flags = SegaUiNextItemDataFlags_None; } // Also cleared manually by ItemAdd()!
};

// Status storage for the last submitted item
struct SegaUiLastItemData
{
    SegaUiID                 ID;
    SegaUiItemFlags          InFlags;            // See SegaUiItemFlags_
    SegaUiItemStatusFlags    StatusFlags;        // See SegaUiItemStatusFlags_
    ImRect                  Rect;               // Full rectangle
    ImRect                  NavRect;            // Navigation scoring rectangle (not displayed)
    ImRect                  DisplayRect;        // Display rectangle (only if SegaUiItemStatusFlags_HasDisplayRect is set)

    SegaUiLastItemData()     { memset(this, 0, sizeof(*this)); }
};

struct SEGAUI_API SegaUiStackSizes
{
    short   SizeOfIDStack;
    short   SizeOfColorStack;
    short   SizeOfStyleVarStack;
    short   SizeOfFontStack;
    short   SizeOfFocusScopeStack;
    short   SizeOfGroupStack;
    short   SizeOfItemFlagsStack;
    short   SizeOfBeginPopupStack;
    short   SizeOfDisabledStack;

    SegaUiStackSizes() { memset(this, 0, sizeof(*this)); }
    void SetToContextState(SegaUiContext* ctx);
    void CompareWithContextState(SegaUiContext* ctx);
};

// Data saved for each window pushed into the stack
struct SegaUiWindowStackData
{
    SegaUiWindow*            Window;
    SegaUiLastItemData       ParentLastItemDataBackup;
    SegaUiStackSizes         StackSizesOnBegin;      // Store size of various stacks for asserting
};

struct SegaUiShrinkWidthItem
{
    int         Index;
    float       Width;
    float       InitialWidth;
};

struct SegaUiPtrOrIndex
{
    void*       Ptr;            // Either field can be set, not both. e.g. Dock node tab bars are loose while BeginTabBar() ones are in a pool.
    int         Index;          // Usually index in a main pool.

    SegaUiPtrOrIndex(void* ptr)  { Ptr = ptr; Index = -1; }
    SegaUiPtrOrIndex(int index)  { Ptr = NULL; Index = index; }
};

//-----------------------------------------------------------------------------
// [SECTION] Inputs support
//-----------------------------------------------------------------------------

// Bit array for named keys
typedef ImBitArray<SegaUiKey_NamedKey_COUNT, -SegaUiKey_NamedKey_BEGIN>    ImBitArrayForNamedKeys;

// [Internal] Key ranges
#define SegaUiKey_LegacyNativeKey_BEGIN  0
#define SegaUiKey_LegacyNativeKey_END    512
#define SegaUiKey_Keyboard_BEGIN         (SegaUiKey_NamedKey_BEGIN)
#define SegaUiKey_Keyboard_END           (SegaUiKey_GamepadStart)
#define SegaUiKey_Gamepad_BEGIN          (SegaUiKey_GamepadStart)
#define SegaUiKey_Gamepad_END            (SegaUiKey_GamepadRStickDown + 1)
#define SegaUiKey_Mouse_BEGIN            (SegaUiKey_MouseLeft)
#define SegaUiKey_Mouse_END              (SegaUiKey_MouseWheelY + 1)
#define SegaUiKey_Aliases_BEGIN          (SegaUiKey_Mouse_BEGIN)
#define SegaUiKey_Aliases_END            (SegaUiKey_Mouse_END)

// [Internal] Named shortcuts for Navigation
#define SegaUiKey_NavKeyboardTweakSlow   SegaUiMod_Ctrl
#define SegaUiKey_NavKeyboardTweakFast   SegaUiMod_Shift
#define SegaUiKey_NavGamepadTweakSlow    SegaUiKey_GamepadL1
#define SegaUiKey_NavGamepadTweakFast    SegaUiKey_GamepadR1
#define SegaUiKey_NavGamepadActivate     SegaUiKey_GamepadFaceDown
#define SegaUiKey_NavGamepadCancel       SegaUiKey_GamepadFaceRight
#define SegaUiKey_NavGamepadMenu         SegaUiKey_GamepadFaceLeft
#define SegaUiKey_NavGamepadInput        SegaUiKey_GamepadFaceUp

enum SegaUiInputEventType
{
    SegaUiInputEventType_None = 0,
    SegaUiInputEventType_MousePos,
    SegaUiInputEventType_MouseWheel,
    SegaUiInputEventType_MouseButton,
    SegaUiInputEventType_Key,
    SegaUiInputEventType_Text,
    SegaUiInputEventType_Focus,
    SegaUiInputEventType_COUNT
};

enum SegaUiInputSource
{
    SegaUiInputSource_None = 0,
    SegaUiInputSource_Mouse,         // Note: may be Mouse or TouchScreen or Pen. See io.MouseSource to distinguish them.
    SegaUiInputSource_Keyboard,
    SegaUiInputSource_Gamepad,
    SegaUiInputSource_Clipboard,     // Currently only used by InputText()
    SegaUiInputSource_COUNT
};

// FIXME: Structures in the union below need to be declared as anonymous unions appears to be an extension?
// Using ImVec2() would fail on Clang 'union member 'MousePos' has a non-trivial default constructor'
struct SegaUiInputEventMousePos      { float PosX, PosY; SegaUiMouseSource MouseSource; };
struct SegaUiInputEventMouseWheel    { float WheelX, WheelY; SegaUiMouseSource MouseSource; };
struct SegaUiInputEventMouseButton   { int Button; bool Down; SegaUiMouseSource MouseSource; };
struct SegaUiInputEventKey           { SegaUiKey Key; bool Down; float AnalogValue; };
struct SegaUiInputEventText          { unsigned int Char; };
struct SegaUiInputEventAppFocused    { bool Focused; };

struct SegaUiInputEvent
{
    SegaUiInputEventType             Type;
    SegaUiInputSource                Source;
    ImU32                           EventId;        // Unique, sequential increasing integer to identify an event (if you need to correlate them to other data).
    union
    {
        SegaUiInputEventMousePos     MousePos;       // if Type == SegaUiInputEventType_MousePos
        SegaUiInputEventMouseWheel   MouseWheel;     // if Type == SegaUiInputEventType_MouseWheel
        SegaUiInputEventMouseButton  MouseButton;    // if Type == SegaUiInputEventType_MouseButton
        SegaUiInputEventKey          Key;            // if Type == SegaUiInputEventType_Key
        SegaUiInputEventText         Text;           // if Type == SegaUiInputEventType_Text
        SegaUiInputEventAppFocused   AppFocused;     // if Type == SegaUiInputEventType_Focus
    };
    bool                            AddedByTestEngine;

    SegaUiInputEvent() { memset(this, 0, sizeof(*this)); }
};

// Input function taking an 'SegaUiID owner_id' argument defaults to (SegaUiKeyOwner_Any == 0) aka don't test ownership, which matches legacy behavior.
#define SegaUiKeyOwner_Any           ((SegaUiID)0)    // Accept key that have an owner, UNLESS a call to SetKeyOwner() explicitly used SegaUiInputFlags_LockThisFrame or SegaUiInputFlags_LockUntilRelease.
#define SegaUiKeyOwner_None          ((SegaUiID)-1)   // Require key to have no owner.

typedef ImS16 SegaUiKeyRoutingIndex;

// Routing table entry (sizeof() == 16 bytes)
struct SegaUiKeyRoutingData
{
    SegaUiKeyRoutingIndex            NextEntryIndex;
    ImU16                           Mods;               // Technically we'd only need 4-bits but for simplify we store SegaUiMod_ values which need 16-bits. SegaUiMod_Shortcut is already translated to Ctrl/Super.
    ImU8                            RoutingNextScore;   // Lower is better (0: perfect score)
    SegaUiID                         RoutingCurr;
    SegaUiID                         RoutingNext;

    SegaUiKeyRoutingData()           { NextEntryIndex = -1; Mods = 0; RoutingNextScore = 255; RoutingCurr = RoutingNext = SegaUiKeyOwner_None; }
};

// Routing table: maintain a desired owner for each possible key-chord (key + mods), and setup owner in NewFrame() when mods are matching.
// Stored in main context (1 instance)
struct SegaUiKeyRoutingTable
{
    SegaUiKeyRoutingIndex            Index[SegaUiKey_NamedKey_COUNT]; // Index of first entry in Entries[]
    ImVector<SegaUiKeyRoutingData>   Entries;
    ImVector<SegaUiKeyRoutingData>   EntriesNext;                    // Double-buffer to avoid reallocation (could use a shared buffer)

    SegaUiKeyRoutingTable()          { Clear(); }
    void Clear()                    { for (int n = 0; n < IM_ARRAYSIZE(Index); n++) Index[n] = -1; Entries.clear(); EntriesNext.clear(); }
};

// This extends SegaUiKeyData but only for named keys (legacy keys don't support the new features)
// Stored in main context (1 per named key). In the future it might be merged into SegaUiKeyData.
struct SegaUiKeyOwnerData
{
    SegaUiID     OwnerCurr;
    SegaUiID     OwnerNext;
    bool        LockThisFrame;      // Reading this key requires explicit owner id (until end of frame). Set by SegaUiInputFlags_LockThisFrame.
    bool        LockUntilRelease;   // Reading this key requires explicit owner id (until key is released). Set by SegaUiInputFlags_LockUntilRelease. When this is true LockThisFrame is always true as well.

    SegaUiKeyOwnerData()             { OwnerCurr = OwnerNext = SegaUiKeyOwner_None; LockThisFrame = LockUntilRelease = false; }
};

// Flags for extended versions of IsKeyPressed(), IsMouseClicked(), Shortcut(), SetKeyOwner(), SetItemKeyOwner()
// Don't mistake with SegaUiInputTextFlags! (for SegaUi::InputText() function)
enum SegaUiInputFlags_
{
    // Flags for IsKeyPressed(), IsMouseClicked(), Shortcut()
    SegaUiInputFlags_None                = 0,
    SegaUiInputFlags_Repeat              = 1 << 0,   // Return true on successive repeats. Default for legacy IsKeyPressed(). NOT Default for legacy IsMouseClicked(). MUST BE == 1.
    SegaUiInputFlags_RepeatRateDefault   = 1 << 1,   // Repeat rate: Regular (default)
    SegaUiInputFlags_RepeatRateNavMove   = 1 << 2,   // Repeat rate: Fast
    SegaUiInputFlags_RepeatRateNavTweak  = 1 << 3,   // Repeat rate: Faster
    SegaUiInputFlags_RepeatRateMask_     = SegaUiInputFlags_RepeatRateDefault | SegaUiInputFlags_RepeatRateNavMove | SegaUiInputFlags_RepeatRateNavTweak,

    // Flags for SetItemKeyOwner()
    SegaUiInputFlags_CondHovered         = 1 << 4,   // Only set if item is hovered (default to both)
    SegaUiInputFlags_CondActive          = 1 << 5,   // Only set if item is active (default to both)
    SegaUiInputFlags_CondDefault_        = SegaUiInputFlags_CondHovered | SegaUiInputFlags_CondActive,
    SegaUiInputFlags_CondMask_           = SegaUiInputFlags_CondHovered | SegaUiInputFlags_CondActive,

    // Flags for SetKeyOwner(), SetItemKeyOwner()
    SegaUiInputFlags_LockThisFrame       = 1 << 6,   // Access to key data will require EXPLICIT owner ID (SegaUiKeyOwner_Any/0 will NOT accepted for polling). Cleared at end of frame. This is useful to make input-owner-aware code steal keys from non-input-owner-aware code.
    SegaUiInputFlags_LockUntilRelease    = 1 << 7,   // Access to key data will require EXPLICIT owner ID (SegaUiKeyOwner_Any/0 will NOT accepted for polling). Cleared when the key is released or at end of each frame if key is released. This is useful to make input-owner-aware code steal keys from non-input-owner-aware code.

    // Routing policies for Shortcut() + low-level SetShortcutRouting()
    // - The general idea is that several callers register interest in a shortcut, and only one owner gets it.
    // - When a policy (other than _RouteAlways) is set, Shortcut() will register itself with SetShortcutRouting(),
    //   allowing the system to decide where to route the input among other route-aware calls.
    // - Shortcut() uses SegaUiInputFlags_RouteFocused by default: meaning that a simple Shortcut() poll
    //   will register a route and only succeed when parent window is in the focus stack and if no-one
    //   with a higher priority is claiming the shortcut.
    // - Using SegaUiInputFlags_RouteAlways is roughly equivalent to doing e.g. IsKeyPressed(key) + testing mods.
    // - Priorities: GlobalHigh > Focused (when owner is active item) > Global > Focused (when focused window) > GlobalLow.
    // - Can select only 1 policy among all available.
    SegaUiInputFlags_RouteFocused        = 1 << 8,   // (Default) Register focused route: Accept inputs if window is in focus stack. Deep-most focused window takes inputs. ActiveId takes inputs over deep-most focused window.
    SegaUiInputFlags_RouteGlobalLow      = 1 << 9,   // Register route globally (lowest priority: unless a focused window or active item registered the route) -> recommended Global priority.
    SegaUiInputFlags_RouteGlobal         = 1 << 10,  // Register route globally (medium priority: unless an active item registered the route, e.g. CTRL+A registered by InputText).
    SegaUiInputFlags_RouteGlobalHigh     = 1 << 11,  // Register route globally (highest priority: unlikely you need to use that: will interfere with every active items)
    SegaUiInputFlags_RouteMask_          = SegaUiInputFlags_RouteFocused | SegaUiInputFlags_RouteGlobal | SegaUiInputFlags_RouteGlobalLow | SegaUiInputFlags_RouteGlobalHigh, // _Always not part of this!
    SegaUiInputFlags_RouteAlways         = 1 << 12,  // Do not register route, poll keys directly.
    SegaUiInputFlags_RouteUnlessBgFocused= 1 << 13,  // Global routes will not be applied if underlying background/void is focused (== no Dear SegaUi windows are focused). Useful for overlay applications.
    SegaUiInputFlags_RouteExtraMask_     = SegaUiInputFlags_RouteAlways | SegaUiInputFlags_RouteUnlessBgFocused,

    // [Internal] Mask of which function support which flags
    SegaUiInputFlags_SupportedByIsKeyPressed     = SegaUiInputFlags_Repeat | SegaUiInputFlags_RepeatRateMask_,
    SegaUiInputFlags_SupportedByShortcut         = SegaUiInputFlags_Repeat | SegaUiInputFlags_RepeatRateMask_ | SegaUiInputFlags_RouteMask_ | SegaUiInputFlags_RouteExtraMask_,
    SegaUiInputFlags_SupportedBySetKeyOwner      = SegaUiInputFlags_LockThisFrame | SegaUiInputFlags_LockUntilRelease,
    SegaUiInputFlags_SupportedBySetItemKeyOwner  = SegaUiInputFlags_SupportedBySetKeyOwner | SegaUiInputFlags_CondMask_,
};

//-----------------------------------------------------------------------------
// [SECTION] Clipper support
//-----------------------------------------------------------------------------

// Note that Max is exclusive, so perhaps should be using a Begin/End convention.
struct SegaUiListClipperRange
{
    int     Min;
    int     Max;
    bool    PosToIndexConvert;      // Begin/End are absolute position (will be converted to indices later)
    ImS8    PosToIndexOffsetMin;    // Add to Min after converting to indices
    ImS8    PosToIndexOffsetMax;    // Add to Min after converting to indices

    static SegaUiListClipperRange    FromIndices(int min, int max)                               { SegaUiListClipperRange r = { min, max, false, 0, 0 }; return r; }
    static SegaUiListClipperRange    FromPositions(float y1, float y2, int off_min, int off_max) { SegaUiListClipperRange r = { (int)y1, (int)y2, true, (ImS8)off_min, (ImS8)off_max }; return r; }
};

// Temporary clipper data, buffers shared/reused between instances
struct SegaUiListClipperData
{
    SegaUiListClipper*               ListClipper;
    float                           LossynessOffset;
    int                             StepNo;
    int                             ItemsFrozen;
    ImVector<SegaUiListClipperRange> Ranges;

    SegaUiListClipperData()          { memset(this, 0, sizeof(*this)); }
    void                            Reset(SegaUiListClipper* clipper) { ListClipper = clipper; StepNo = ItemsFrozen = 0; Ranges.resize(0); }
};

//-----------------------------------------------------------------------------
// [SECTION] Navigation support
//-----------------------------------------------------------------------------

enum SegaUiActivateFlags_
{
    SegaUiActivateFlags_None                 = 0,
    SegaUiActivateFlags_PreferInput          = 1 << 0,       // Favor activation that requires keyboard text input (e.g. for Slider/Drag). Default for Enter key.
    SegaUiActivateFlags_PreferTweak          = 1 << 1,       // Favor activation for tweaking with arrows or gamepad (e.g. for Slider/Drag). Default for Space key and if keyboard is not used.
    SegaUiActivateFlags_TryToPreserveState   = 1 << 2,       // Request widget to preserve state if it can (e.g. InputText will try to preserve cursor/selection)
};

// Early work-in-progress API for ScrollToItem()
enum SegaUiScrollFlags_
{
    SegaUiScrollFlags_None                   = 0,
    SegaUiScrollFlags_KeepVisibleEdgeX       = 1 << 0,       // If item is not visible: scroll as little as possible on X axis to bring item back into view [default for X axis]
    SegaUiScrollFlags_KeepVisibleEdgeY       = 1 << 1,       // If item is not visible: scroll as little as possible on Y axis to bring item back into view [default for Y axis for windows that are already visible]
    SegaUiScrollFlags_KeepVisibleCenterX     = 1 << 2,       // If item is not visible: scroll to make the item centered on X axis [rarely used]
    SegaUiScrollFlags_KeepVisibleCenterY     = 1 << 3,       // If item is not visible: scroll to make the item centered on Y axis
    SegaUiScrollFlags_AlwaysCenterX          = 1 << 4,       // Always center the result item on X axis [rarely used]
    SegaUiScrollFlags_AlwaysCenterY          = 1 << 5,       // Always center the result item on Y axis [default for Y axis for appearing window)
    SegaUiScrollFlags_NoScrollParent         = 1 << 6,       // Disable forwarding scrolling to parent window if required to keep item/rect visible (only scroll window the function was applied to).
    SegaUiScrollFlags_MaskX_                 = SegaUiScrollFlags_KeepVisibleEdgeX | SegaUiScrollFlags_KeepVisibleCenterX | SegaUiScrollFlags_AlwaysCenterX,
    SegaUiScrollFlags_MaskY_                 = SegaUiScrollFlags_KeepVisibleEdgeY | SegaUiScrollFlags_KeepVisibleCenterY | SegaUiScrollFlags_AlwaysCenterY,
};

enum SegaUiNavHighlightFlags_
{
    SegaUiNavHighlightFlags_None             = 0,
    SegaUiNavHighlightFlags_TypeDefault      = 1 << 0,
    SegaUiNavHighlightFlags_TypeThin         = 1 << 1,
    SegaUiNavHighlightFlags_AlwaysDraw       = 1 << 2,       // Draw rectangular highlight if (g.NavId == id) _even_ when using the mouse.
    SegaUiNavHighlightFlags_NoRounding       = 1 << 3,
};

enum SegaUiNavMoveFlags_
{
    SegaUiNavMoveFlags_None                  = 0,
    SegaUiNavMoveFlags_LoopX                 = 1 << 0,   // On failed request, restart from opposite side
    SegaUiNavMoveFlags_LoopY                 = 1 << 1,
    SegaUiNavMoveFlags_WrapX                 = 1 << 2,   // On failed request, request from opposite side one line down (when NavDir==right) or one line up (when NavDir==left)
    SegaUiNavMoveFlags_WrapY                 = 1 << 3,   // This is not super useful but provided for completeness
    SegaUiNavMoveFlags_WrapMask_             = SegaUiNavMoveFlags_LoopX | SegaUiNavMoveFlags_LoopY | SegaUiNavMoveFlags_WrapX | SegaUiNavMoveFlags_WrapY,
    SegaUiNavMoveFlags_AllowCurrentNavId     = 1 << 4,   // Allow scoring and considering the current NavId as a move target candidate. This is used when the move source is offset (e.g. pressing PageDown actually needs to send a Up move request, if we are pressing PageDown from the bottom-most item we need to stay in place)
    SegaUiNavMoveFlags_AlsoScoreVisibleSet   = 1 << 5,   // Store alternate result in NavMoveResultLocalVisible that only comprise elements that are already fully visible (used by PageUp/PageDown)
    SegaUiNavMoveFlags_ScrollToEdgeY         = 1 << 6,   // Force scrolling to min/max (used by Home/End) // FIXME-NAV: Aim to remove or reword, probably unnecessary
    SegaUiNavMoveFlags_Forwarded             = 1 << 7,
    SegaUiNavMoveFlags_DebugNoResult         = 1 << 8,   // Dummy scoring for debug purpose, don't apply result
    SegaUiNavMoveFlags_FocusApi              = 1 << 9,
    SegaUiNavMoveFlags_Tabbing               = 1 << 10,  // == Focus + Activate if item is Inputable + DontChangeNavHighlight
    SegaUiNavMoveFlags_Activate              = 1 << 11,
    SegaUiNavMoveFlags_DontSetNavHighlight   = 1 << 12,  // Do not alter the visible state of keyboard vs mouse nav highlight
};

enum SegaUiNavLayer
{
    SegaUiNavLayer_Main  = 0,    // Main scrolling layer
    SegaUiNavLayer_Menu  = 1,    // Menu layer (access with Alt)
    SegaUiNavLayer_COUNT
};

struct SegaUiNavItemData
{
    SegaUiWindow*        Window;         // Init,Move    // Best candidate window (result->ItemWindow->RootWindowForNav == request->Window)
    SegaUiID             ID;             // Init,Move    // Best candidate item ID
    SegaUiID             FocusScopeId;   // Init,Move    // Best candidate focus scope ID
    ImRect              RectRel;        // Init,Move    // Best candidate bounding box in window relative space
    SegaUiItemFlags      InFlags;        // ????,Move    // Best candidate item flags
    float               DistBox;        //      Move    // Best candidate box distance to current NavId
    float               DistCenter;     //      Move    // Best candidate center distance to current NavId
    float               DistAxial;      //      Move    // Best candidate axial distance to current NavId

    SegaUiNavItemData()  { Clear(); }
    void Clear()        { Window = NULL; ID = FocusScopeId = 0; InFlags = 0; DistBox = DistCenter = DistAxial = FLT_MAX; }
};

//-----------------------------------------------------------------------------
// [SECTION] Columns support
//-----------------------------------------------------------------------------

// Flags for internal's BeginColumns(). Prefix using BeginTable() nowadays!
enum SegaUiOldColumnFlags_
{
    SegaUiOldColumnFlags_None                    = 0,
    SegaUiOldColumnFlags_NoBorder                = 1 << 0,   // Disable column dividers
    SegaUiOldColumnFlags_NoResize                = 1 << 1,   // Disable resizing columns when clicking on the dividers
    SegaUiOldColumnFlags_NoPreserveWidths        = 1 << 2,   // Disable column width preservation when adjusting columns
    SegaUiOldColumnFlags_NoForceWithinWindow     = 1 << 3,   // Disable forcing columns to fit within window
    SegaUiOldColumnFlags_GrowParentContentsSize  = 1 << 4,   // (WIP) Restore pre-1.51 behavior of extending the parent window contents size but _without affecting the columns width at all_. Will eventually remove.

    // Obsolete names (will be removed)
#ifndef SEGAUI_DISABLE_OBSOLETE_FUNCTIONS
    SegaUiColumnsFlags_None                      = SegaUiOldColumnFlags_None,
    SegaUiColumnsFlags_NoBorder                  = SegaUiOldColumnFlags_NoBorder,
    SegaUiColumnsFlags_NoResize                  = SegaUiOldColumnFlags_NoResize,
    SegaUiColumnsFlags_NoPreserveWidths          = SegaUiOldColumnFlags_NoPreserveWidths,
    SegaUiColumnsFlags_NoForceWithinWindow       = SegaUiOldColumnFlags_NoForceWithinWindow,
    SegaUiColumnsFlags_GrowParentContentsSize    = SegaUiOldColumnFlags_GrowParentContentsSize,
#endif
};

struct SegaUiOldColumnData
{
    float               OffsetNorm;             // Column start offset, normalized 0.0 (far left) -> 1.0 (far right)
    float               OffsetNormBeforeResize;
    SegaUiOldColumnFlags Flags;                  // Not exposed
    ImRect              ClipRect;

    SegaUiOldColumnData() { memset(this, 0, sizeof(*this)); }
};

struct SegaUiOldColumns
{
    SegaUiID             ID;
    SegaUiOldColumnFlags Flags;
    bool                IsFirstFrame;
    bool                IsBeingResized;
    int                 Current;
    int                 Count;
    float               OffMinX, OffMaxX;       // Offsets from HostWorkRect.Min.x
    float               LineMinY, LineMaxY;
    float               HostCursorPosY;         // Backup of CursorPos at the time of BeginColumns()
    float               HostCursorMaxPosX;      // Backup of CursorMaxPos at the time of BeginColumns()
    ImRect              HostInitialClipRect;    // Backup of ClipRect at the time of BeginColumns()
    ImRect              HostBackupClipRect;     // Backup of ClipRect during PushColumnsBackground()/PopColumnsBackground()
    ImRect              HostBackupParentWorkRect;//Backup of WorkRect at the time of BeginColumns()
    ImVector<SegaUiOldColumnData> Columns;
    ImDrawListSplitter  Splitter;

    SegaUiOldColumns()   { memset(this, 0, sizeof(*this)); }
};

//-----------------------------------------------------------------------------
// [SECTION] Multi-select support
//-----------------------------------------------------------------------------

#ifdef SEGAUI_HAS_MULTI_SELECT
// <this is filled in 'range_select' branch>
#endif // #ifdef SEGAUI_HAS_MULTI_SELECT

//-----------------------------------------------------------------------------
// [SECTION] Docking support
//-----------------------------------------------------------------------------

#ifdef SEGAUI_HAS_DOCK
// <this is filled in 'docking' branch>
#endif // #ifdef SEGAUI_HAS_DOCK

//-----------------------------------------------------------------------------
// [SECTION] Viewport support
//-----------------------------------------------------------------------------

// SegaUiViewport Private/Internals fields (cardinal sin: we are using inheritance!)
// Every instance of SegaUiViewport is in fact a SegaUiViewportP.
struct SegaUiViewportP : public SegaUiViewport
{
    int                 DrawListsLastFrame[2];  // Last frame number the background (0) and foreground (1) draw lists were used
    ImDrawList*         DrawLists[2];           // Convenience background (0) and foreground (1) draw lists. We use them to draw software mouser cursor when io.MouseDrawCursor is set and to draw most debug overlays.
    ImDrawData          DrawDataP;
    ImDrawDataBuilder   DrawDataBuilder;

    ImVec2              WorkOffsetMin;          // Work Area: Offset from Pos to top-left corner of Work Area. Generally (0,0) or (0,+main_menu_bar_height). Work Area is Full Area but without menu-bars/status-bars (so WorkArea always fit inside Pos/Size!)
    ImVec2              WorkOffsetMax;          // Work Area: Offset from Pos+Size to bottom-right corner of Work Area. Generally (0,0) or (0,-status_bar_height).
    ImVec2              BuildWorkOffsetMin;     // Work Area: Offset being built during current frame. Generally >= 0.0f.
    ImVec2              BuildWorkOffsetMax;     // Work Area: Offset being built during current frame. Generally <= 0.0f.

    SegaUiViewportP()    { DrawListsLastFrame[0] = DrawListsLastFrame[1] = -1; DrawLists[0] = DrawLists[1] = NULL; }
    ~SegaUiViewportP()   { if (DrawLists[0]) IM_DELETE(DrawLists[0]); if (DrawLists[1]) IM_DELETE(DrawLists[1]); }

    // Calculate work rect pos/size given a set of offset (we have 1 pair of offset for rect locked from last frame data, and 1 pair for currently building rect)
    ImVec2  CalcWorkRectPos(const ImVec2& off_min) const                            { return ImVec2(Pos.x + off_min.x, Pos.y + off_min.y); }
    ImVec2  CalcWorkRectSize(const ImVec2& off_min, const ImVec2& off_max) const    { return ImVec2(ImMax(0.0f, Size.x - off_min.x + off_max.x), ImMax(0.0f, Size.y - off_min.y + off_max.y)); }
    void    UpdateWorkRect()            { WorkPos = CalcWorkRectPos(WorkOffsetMin); WorkSize = CalcWorkRectSize(WorkOffsetMin, WorkOffsetMax); } // Update public fields

    // Helpers to retrieve ImRect (we don't need to store BuildWorkRect as every access tend to change it, hence the code asymmetry)
    ImRect  GetMainRect() const         { return ImRect(Pos.x, Pos.y, Pos.x + Size.x, Pos.y + Size.y); }
    ImRect  GetWorkRect() const         { return ImRect(WorkPos.x, WorkPos.y, WorkPos.x + WorkSize.x, WorkPos.y + WorkSize.y); }
    ImRect  GetBuildWorkRect() const    { ImVec2 pos = CalcWorkRectPos(BuildWorkOffsetMin); ImVec2 size = CalcWorkRectSize(BuildWorkOffsetMin, BuildWorkOffsetMax); return ImRect(pos.x, pos.y, pos.x + size.x, pos.y + size.y); }
};

//-----------------------------------------------------------------------------
// [SECTION] Settings support
//-----------------------------------------------------------------------------

// Windows data saved in imgui.ini file
// Because we never destroy or rename SegaUiWindowSettings, we can store the names in a separate buffer easily.
// (this is designed to be stored in a ImChunkStream buffer, with the variable-length Name following our structure)
struct SegaUiWindowSettings
{
    SegaUiID     ID;
    ImVec2ih    Pos;
    ImVec2ih    Size;
    bool        Collapsed;
    bool        WantApply;      // Set when loaded from .ini data (to enable merging/loading .ini data into an already running context)
    bool        WantDelete;     // Set to invalidate/delete the settings entry

    SegaUiWindowSettings()       { memset(this, 0, sizeof(*this)); }
    char* GetName()             { return (char*)(this + 1); }
};

struct SegaUiSettingsHandler
{
    const char* TypeName;       // Short description stored in .ini file. Disallowed characters: '[' ']'
    SegaUiID     TypeHash;       // == ImHashStr(TypeName)
    void        (*ClearAllFn)(SegaUiContext* ctx, SegaUiSettingsHandler* handler);                                // Clear all settings data
    void        (*ReadInitFn)(SegaUiContext* ctx, SegaUiSettingsHandler* handler);                                // Read: Called before reading (in registration order)
    void*       (*ReadOpenFn)(SegaUiContext* ctx, SegaUiSettingsHandler* handler, const char* name);              // Read: Called when entering into a new ini entry e.g. "[Window][Name]"
    void        (*ReadLineFn)(SegaUiContext* ctx, SegaUiSettingsHandler* handler, void* entry, const char* line); // Read: Called for every line of text within an ini entry
    void        (*ApplyAllFn)(SegaUiContext* ctx, SegaUiSettingsHandler* handler);                                // Read: Called after reading (in registration order)
    void        (*WriteAllFn)(SegaUiContext* ctx, SegaUiSettingsHandler* handler, SegaUiTextBuffer* out_buf);      // Write: Output every entries into 'out_buf'
    void*       UserData;

    SegaUiSettingsHandler() { memset(this, 0, sizeof(*this)); }
};

//-----------------------------------------------------------------------------
// [SECTION] Localization support
//-----------------------------------------------------------------------------

// This is experimental and not officially supported, it'll probably fall short of features, if/when it does we may backtrack.
enum SegaUiLocKey : int
{
    SegaUiLocKey_TableSizeOne,
    SegaUiLocKey_TableSizeAllFit,
    SegaUiLocKey_TableSizeAllDefault,
    SegaUiLocKey_TableResetOrder,
    SegaUiLocKey_WindowingMainMenuBar,
    SegaUiLocKey_WindowingPopup,
    SegaUiLocKey_WindowingUntitled,
    SegaUiLocKey_COUNT
};

struct SegaUiLocEntry
{
    SegaUiLocKey     Key;
    const char*     Text;
};


//-----------------------------------------------------------------------------
// [SECTION] Metrics, Debug Tools
//-----------------------------------------------------------------------------

enum SegaUiDebugLogFlags_
{
    // Event types
    SegaUiDebugLogFlags_None             = 0,
    SegaUiDebugLogFlags_EventActiveId    = 1 << 0,
    SegaUiDebugLogFlags_EventFocus       = 1 << 1,
    SegaUiDebugLogFlags_EventPopup       = 1 << 2,
    SegaUiDebugLogFlags_EventNav         = 1 << 3,
    SegaUiDebugLogFlags_EventClipper     = 1 << 4,
    SegaUiDebugLogFlags_EventSelection   = 1 << 5,
    SegaUiDebugLogFlags_EventIO          = 1 << 6,
    SegaUiDebugLogFlags_EventMask_       = SegaUiDebugLogFlags_EventActiveId  | SegaUiDebugLogFlags_EventFocus | SegaUiDebugLogFlags_EventPopup | SegaUiDebugLogFlags_EventNav | SegaUiDebugLogFlags_EventClipper | SegaUiDebugLogFlags_EventSelection | SegaUiDebugLogFlags_EventIO,
    SegaUiDebugLogFlags_OutputToTTY      = 1 << 10,  // Also send output to TTY
};

struct SegaUiMetricsConfig
{
    bool        ShowDebugLog = false;
    bool        ShowStackTool = false;
    bool        ShowWindowsRects = false;
    bool        ShowWindowsBeginOrder = false;
    bool        ShowTablesRects = false;
    bool        ShowDrawCmdMesh = true;
    bool        ShowDrawCmdBoundingBoxes = true;
    bool        ShowAtlasTintedWithTextColor = false;
    int         ShowWindowsRectsType = -1;
    int         ShowTablesRectsType = -1;
};

struct SegaUiStackLevelInfo
{
    SegaUiID                 ID;
    ImS8                    QueryFrameCount;            // >= 1: Query in progress
    bool                    QuerySuccess;               // Obtained result from DebugHookIdInfo()
    SegaUiDataType           DataType : 8;
    char                    Desc[57];                   // Arbitrarily sized buffer to hold a result (FIXME: could replace Results[] with a chunk stream?) FIXME: Now that we added CTRL+C this should be fixed.

    SegaUiStackLevelInfo()   { memset(this, 0, sizeof(*this)); }
};

// State for Stack tool queries
struct SegaUiStackTool
{
    int                     LastActiveFrame;
    int                     StackLevel;                 // -1: query stack and resize Results, >= 0: individual stack level
    SegaUiID                 QueryId;                    // ID to query details for
    ImVector<SegaUiStackLevelInfo> Results;
    bool                    CopyToClipboardOnCtrlC;
    float                   CopyToClipboardLastTime;

    SegaUiStackTool()        { memset(this, 0, sizeof(*this)); CopyToClipboardLastTime = -FLT_MAX; }
};

//-----------------------------------------------------------------------------
// [SECTION] Generic context hooks
//-----------------------------------------------------------------------------

typedef void (*SegaUiContextHookCallback)(SegaUiContext* ctx, SegaUiContextHook* hook);
enum SegaUiContextHookType { SegaUiContextHookType_NewFramePre, SegaUiContextHookType_NewFramePost, SegaUiContextHookType_EndFramePre, SegaUiContextHookType_EndFramePost, SegaUiContextHookType_RenderPre, SegaUiContextHookType_RenderPost, SegaUiContextHookType_Shutdown, SegaUiContextHookType_PendingRemoval_ };

struct SegaUiContextHook
{
    SegaUiID                     HookId;     // A unique ID assigned by AddContextHook()
    SegaUiContextHookType        Type;
    SegaUiID                     Owner;
    SegaUiContextHookCallback    Callback;
    void*                       UserData;

    SegaUiContextHook()          { memset(this, 0, sizeof(*this)); }
};

//-----------------------------------------------------------------------------
// [SECTION] SegaUiContext (main Dear SegaUi context)
//-----------------------------------------------------------------------------

struct SegaUiContext
{
    bool                    Initialized;
    bool                    FontAtlasOwnedByContext;            // IO.Fonts-> is owned by the SegaUiContext and will be destructed along with it.
    SegaUiIO                 IO;
    SegaUiStyle              Style;
    ImFont*                 Font;                               // (Shortcut) == FontStack.empty() ? IO.Font : FontStack.back()
    float                   FontSize;                           // (Shortcut) == FontBaseSize * g.CurrentWindow->FontWindowScale == window->FontSize(). Text height for current window.
    float                   FontBaseSize;                       // (Shortcut) == IO.FontGlobalScale * Font->Scale * Font->FontSize. Base text height.
    ImDrawListSharedData    DrawListSharedData;
    double                  Time;
    int                     FrameCount;
    int                     FrameCountEnded;
    int                     FrameCountRendered;
    bool                    WithinFrameScope;                   // Set by NewFrame(), cleared by EndFrame()
    bool                    WithinFrameScopeWithImplicitWindow; // Set by NewFrame(), cleared by EndFrame() when the implicit debug window has been pushed
    bool                    WithinEndChild;                     // Set within EndChild()
    bool                    GcCompactAll;                       // Request full GC
    bool                    TestEngineHookItems;                // Will call test engine hooks: SegaUiTestEngineHook_ItemAdd(), SegaUiTestEngineHook_ItemInfo(), SegaUiTestEngineHook_Log()
    void*                   TestEngine;                         // Test engine user data

    // Inputs
    ImVector<SegaUiInputEvent> InputEventsQueue;                 // Input events which will be trickled/written into IO structure.
    ImVector<SegaUiInputEvent> InputEventsTrail;                 // Past input events processed in NewFrame(). This is to allow domain-specific application to access e.g mouse/pen trail.
    SegaUiMouseSource        InputEventsNextMouseSource;
    ImU32                   InputEventsNextEventId;

    // Windows state
    ImVector<SegaUiWindow*>  Windows;                            // Windows, sorted in display order, back to front
    ImVector<SegaUiWindow*>  WindowsFocusOrder;                  // Root windows, sorted in focus order, back to front.
    ImVector<SegaUiWindow*>  WindowsTempSortBuffer;              // Temporary buffer used in EndFrame() to reorder windows so parents are kept before their child
    ImVector<SegaUiWindowStackData> CurrentWindowStack;
    SegaUiStorage            WindowsById;                        // Map window's SegaUiID to SegaUiWindow*
    int                     WindowsActiveCount;                 // Number of unique windows submitted by frame
    ImVec2                  WindowsHoverPadding;                // Padding around resizable windows for which hovering on counts as hovering the window == ImMax(style.TouchExtraPadding, WINDOWS_HOVER_PADDING)
    SegaUiWindow*            CurrentWindow;                      // Window being drawn into
    SegaUiWindow*            HoveredWindow;                      // Window the mouse is hovering. Will typically catch mouse inputs.
    SegaUiWindow*            HoveredWindowUnderMovingWindow;     // Hovered window ignoring MovingWindow. Only set if MovingWindow is set.
    SegaUiWindow*            MovingWindow;                       // Track the window we clicked on (in order to preserve focus). The actual window that is moved is generally MovingWindow->RootWindow.
    SegaUiWindow*            WheelingWindow;                     // Track the window we started mouse-wheeling on. Until a timer elapse or mouse has moved, generally keep scrolling the same window even if during the course of scrolling the mouse ends up hovering a child window.
    ImVec2                  WheelingWindowRefMousePos;
    int                     WheelingWindowStartFrame;           // This may be set one frame before WheelingWindow is != NULL
    float                   WheelingWindowReleaseTimer;
    ImVec2                  WheelingWindowWheelRemainder;
    ImVec2                  WheelingAxisAvg;

    // Item/widgets state and tracking information
    SegaUiID                 DebugHookIdInfo;                    // Will call core hooks: DebugHookIdInfo() from GetID functions, used by Stack Tool [next HoveredId/ActiveId to not pull in an extra cache-line]
    SegaUiID                 HoveredId;                          // Hovered widget, filled during the frame
    SegaUiID                 HoveredIdPreviousFrame;
    bool                    HoveredIdAllowOverlap;
    bool                    HoveredIdDisabled;                  // At least one widget passed the rect test, but has been discarded by disabled flag or popup inhibit. May be true even if HoveredId == 0.
    float                   HoveredIdTimer;                     // Measure contiguous hovering time
    float                   HoveredIdNotActiveTimer;            // Measure contiguous hovering time where the item has not been active
    SegaUiID                 ActiveId;                           // Active widget
    SegaUiID                 ActiveIdIsAlive;                    // Active widget has been seen this frame (we can't use a bool as the ActiveId may change within the frame)
    float                   ActiveIdTimer;
    bool                    ActiveIdIsJustActivated;            // Set at the time of activation for one frame
    bool                    ActiveIdAllowOverlap;               // Active widget allows another widget to steal active id (generally for overlapping widgets, but not always)
    bool                    ActiveIdNoClearOnFocusLoss;         // Disable losing active id if the active id window gets unfocused.
    bool                    ActiveIdHasBeenPressedBefore;       // Track whether the active id led to a press (this is to allow changing between PressOnClick and PressOnRelease without pressing twice). Used by range_select branch.
    bool                    ActiveIdHasBeenEditedBefore;        // Was the value associated to the widget Edited over the course of the Active state.
    bool                    ActiveIdHasBeenEditedThisFrame;
    ImVec2                  ActiveIdClickOffset;                // Clicked offset from upper-left corner, if applicable (currently only set by ButtonBehavior)
    SegaUiWindow*            ActiveIdWindow;
    SegaUiInputSource        ActiveIdSource;                     // Activating source: SegaUiInputSource_Mouse OR SegaUiInputSource_Keyboard OR SegaUiInputSource_Gamepad
    int                     ActiveIdMouseButton;
    SegaUiID                 ActiveIdPreviousFrame;
    bool                    ActiveIdPreviousFrameIsAlive;
    bool                    ActiveIdPreviousFrameHasBeenEditedBefore;
    SegaUiWindow*            ActiveIdPreviousFrameWindow;
    SegaUiID                 LastActiveId;                       // Store the last non-zero ActiveId, useful for animation.
    float                   LastActiveIdTimer;                  // Store the last non-zero ActiveId timer since the beginning of activation, useful for animation.

    // [EXPERIMENTAL] Key/Input Ownership + Shortcut Routing system
    // - The idea is that instead of "eating" a given key, we can link to an owner.
    // - Input query can then read input by specifying SegaUiKeyOwner_Any (== 0), SegaUiKeyOwner_None (== -1) or a custom ID.
    // - Routing is requested ahead of time for a given chord (Key + Mods) and granted in NewFrame().
    SegaUiKeyOwnerData       KeysOwnerData[SegaUiKey_NamedKey_COUNT];
    SegaUiKeyRoutingTable    KeysRoutingTable;
    ImU32                   ActiveIdUsingNavDirMask;            // Active widget will want to read those nav move requests (e.g. can activate a button and move away from it)
    bool                    ActiveIdUsingAllKeyboardKeys;       // Active widget will want to read all keyboard keys inputs. (FIXME: This is a shortcut for not taking ownership of 100+ keys but perhaps best to not have the inconsistency)
#ifndef SEGAUI_DISABLE_OBSOLETE_KEYIO
    ImU32                   ActiveIdUsingNavInputMask;          // If you used this. Since (SEGAUI_VERSION_NUM >= 18804) : 'g.ActiveIdUsingNavInputMask |= (1 << SegaUiNavInput_Cancel);' becomes 'SetKeyOwner(SegaUiKey_Escape, g.ActiveId) and/or SetKeyOwner(SegaUiKey_NavGamepadCancel, g.ActiveId);'
#endif

    // Next window/item data
    SegaUiID                 CurrentFocusScopeId;                // == g.FocusScopeStack.back()
    SegaUiItemFlags          CurrentItemFlags;                   // == g.ItemFlagsStack.back()
    SegaUiID                 DebugLocateId;                      // Storage for DebugLocateItemOnHover() feature: this is read by ItemAdd() so we keep it in a hot/cached location
    SegaUiNextItemData       NextItemData;                       // Storage for SetNextItem** functions
    SegaUiLastItemData       LastItemData;                       // Storage for last submitted item (setup by ItemAdd)
    SegaUiNextWindowData     NextWindowData;                     // Storage for SetNextWindow** functions

    // Shared stacks
    ImVector<SegaUiColorMod> ColorStack;                         // Stack for PushStyleColor()/PopStyleColor() - inherited by Begin()
    ImVector<SegaUiStyleMod> StyleVarStack;                      // Stack for PushStyleVar()/PopStyleVar() - inherited by Begin()
    ImVector<ImFont*>       FontStack;                          // Stack for PushFont()/PopFont() - inherited by Begin()
    ImVector<SegaUiID>       FocusScopeStack;                    // Stack for PushFocusScope()/PopFocusScope() - inherited by BeginChild(), pushed into by Begin()
    ImVector<SegaUiItemFlags>ItemFlagsStack;                     // Stack for PushItemFlag()/PopItemFlag() - inherited by Begin()
    ImVector<SegaUiGroupData>GroupStack;                         // Stack for BeginGroup()/EndGroup() - not inherited by Begin()
    ImVector<SegaUiPopupData>OpenPopupStack;                     // Which popups are open (persistent)
    ImVector<SegaUiPopupData>BeginPopupStack;                    // Which level of BeginPopup() we are in (reset every frame)
    int                     BeginMenuCount;

    // Viewports
    ImVector<SegaUiViewportP*> Viewports;                        // Active viewports (Size==1 in 'master' branch). Each viewports hold their copy of ImDrawData.

    // Gamepad/keyboard Navigation
    SegaUiWindow*            NavWindow;                          // Focused window for navigation. Could be called 'FocusedWindow'
    SegaUiID                 NavId;                              // Focused item for navigation
    SegaUiID                 NavFocusScopeId;                    // Identify a selection scope (selection code often wants to "clear other items" when landing on an item of the selection set)
    SegaUiID                 NavActivateId;                      // ~~ (g.ActiveId == 0) && (IsKeyPressed(SegaUiKey_Space) || IsKeyDown(SegaUiKey_Enter) || IsKeyPressed(SegaUiKey_NavGamepadActivate)) ? NavId : 0, also set when calling ActivateItem()
    SegaUiID                 NavActivateDownId;                  // ~~ IsKeyDown(SegaUiKey_Space) || IsKeyDown(SegaUiKey_Enter) || IsKeyDown(SegaUiKey_NavGamepadActivate) ? NavId : 0
    SegaUiID                 NavActivatePressedId;               // ~~ IsKeyPressed(SegaUiKey_Space) || IsKeyPressed(SegaUiKey_Enter) || IsKeyPressed(SegaUiKey_NavGamepadActivate) ? NavId : 0 (no repeat)
    SegaUiActivateFlags      NavActivateFlags;
    SegaUiID                 NavJustMovedToId;                   // Just navigated to this id (result of a successfully MoveRequest).
    SegaUiID                 NavJustMovedToFocusScopeId;         // Just navigated to this focus scope id (result of a successfully MoveRequest).
    SegaUiKeyChord           NavJustMovedToKeyMods;
    SegaUiID                 NavNextActivateId;                  // Set by ActivateItem(), queued until next frame.
    SegaUiActivateFlags      NavNextActivateFlags;
    SegaUiInputSource        NavInputSource;                     // Keyboard or Gamepad mode? THIS CAN ONLY BE SegaUiInputSource_Keyboard or SegaUiInputSource_Mouse
    SegaUiNavLayer           NavLayer;                           // Layer we are navigating on. For now the system is hard-coded for 0=main contents and 1=menu/title bar, may expose layers later.
    bool                    NavIdIsAlive;                       // Nav widget has been seen this frame ~~ NavRectRel is valid
    bool                    NavMousePosDirty;                   // When set we will update mouse position if (io.ConfigFlags & SegaUiConfigFlags_NavEnableSetMousePos) if set (NB: this not enabled by default)
    bool                    NavDisableHighlight;                // When user starts using mouse, we hide gamepad/keyboard highlight (NB: but they are still available, which is why NavDisableHighlight isn't always != NavDisableMouseHover)
    bool                    NavDisableMouseHover;               // When user starts using gamepad/keyboard, we hide mouse hovering highlight until mouse is touched again.

    // Navigation: Init & Move Requests
    bool                    NavAnyRequest;                      // ~~ NavMoveRequest || NavInitRequest this is to perform early out in ItemAdd()
    bool                    NavInitRequest;                     // Init request for appearing window to select first item
    bool                    NavInitRequestFromMove;
    SegaUiID                 NavInitResultId;                    // Init request result (first item of the window, or one for which SetItemDefaultFocus() was called)
    ImRect                  NavInitResultRectRel;               // Init request result rectangle (relative to parent window)
    bool                    NavMoveSubmitted;                   // Move request submitted, will process result on next NewFrame()
    bool                    NavMoveScoringItems;                // Move request submitted, still scoring incoming items
    bool                    NavMoveForwardToNextFrame;
    SegaUiNavMoveFlags       NavMoveFlags;
    SegaUiScrollFlags        NavMoveScrollFlags;
    SegaUiKeyChord           NavMoveKeyMods;
    SegaUiDir                NavMoveDir;                         // Direction of the move request (left/right/up/down)
    SegaUiDir                NavMoveDirForDebug;
    SegaUiDir                NavMoveClipDir;                     // FIXME-NAV: Describe the purpose of this better. Might want to rename?
    ImRect                  NavScoringRect;                     // Rectangle used for scoring, in screen space. Based of window->NavRectRel[], modified for directional navigation scoring.
    ImRect                  NavScoringNoClipRect;               // Some nav operations (such as PageUp/PageDown) enforce a region which clipper will attempt to always keep submitted
    int                     NavScoringDebugCount;               // Metrics for debugging
    int                     NavTabbingDir;                      // Generally -1 or +1, 0 when tabbing without a nav id
    int                     NavTabbingCounter;                  // >0 when counting items for tabbing
    SegaUiNavItemData        NavMoveResultLocal;                 // Best move request candidate within NavWindow
    SegaUiNavItemData        NavMoveResultLocalVisible;          // Best move request candidate within NavWindow that are mostly visible (when using SegaUiNavMoveFlags_AlsoScoreVisibleSet flag)
    SegaUiNavItemData        NavMoveResultOther;                 // Best move request candidate within NavWindow's flattened hierarchy (when using SegaUiWindowFlags_NavFlattened flag)
    SegaUiNavItemData        NavTabbingResultFirst;              // First tabbing request candidate within NavWindow and flattened hierarchy

    // Navigation: Windowing (CTRL+TAB for list, or Menu button + keys or directional pads to move/resize)
    SegaUiKeyChord           ConfigNavWindowingKeyNext;          // = SegaUiMod_Ctrl | SegaUiKey_Tab, for reconfiguration (see #4828)
    SegaUiKeyChord           ConfigNavWindowingKeyPrev;          // = SegaUiMod_Ctrl | SegaUiMod_Shift | SegaUiKey_Tab
    SegaUiWindow*            NavWindowingTarget;                 // Target window when doing CTRL+Tab (or Pad Menu + FocusPrev/Next), this window is temporarily displayed top-most!
    SegaUiWindow*            NavWindowingTargetAnim;             // Record of last valid NavWindowingTarget until DimBgRatio and NavWindowingHighlightAlpha becomes 0.0f, so the fade-out can stay on it.
    SegaUiWindow*            NavWindowingListWindow;             // Internal window actually listing the CTRL+Tab contents
    float                   NavWindowingTimer;
    float                   NavWindowingHighlightAlpha;
    bool                    NavWindowingToggleLayer;
    ImVec2                  NavWindowingAccumDeltaPos;
    ImVec2                  NavWindowingAccumDeltaSize;

    // Render
    float                   DimBgRatio;                         // 0.0..1.0 animation when fading in a dimming background (for modal window and CTRL+TAB list)
    SegaUiMouseCursor        MouseCursor;

    // Drag and Drop
    bool                    DragDropActive;
    bool                    DragDropWithinSource;               // Set when within a BeginDragDropXXX/EndDragDropXXX block for a drag source.
    bool                    DragDropWithinTarget;               // Set when within a BeginDragDropXXX/EndDragDropXXX block for a drag target.
    SegaUiDragDropFlags      DragDropSourceFlags;
    int                     DragDropSourceFrameCount;
    int                     DragDropMouseButton;
    SegaUiPayload            DragDropPayload;
    ImRect                  DragDropTargetRect;                 // Store rectangle of current target candidate (we favor small targets when overlapping)
    SegaUiID                 DragDropTargetId;
    SegaUiDragDropFlags      DragDropAcceptFlags;
    float                   DragDropAcceptIdCurrRectSurface;    // Target item surface (we resolve overlapping targets by prioritizing the smaller surface)
    SegaUiID                 DragDropAcceptIdCurr;               // Target item id (set at the time of accepting the payload)
    SegaUiID                 DragDropAcceptIdPrev;               // Target item id from previous frame (we need to store this to allow for overlapping drag and drop targets)
    int                     DragDropAcceptFrameCount;           // Last time a target expressed a desire to accept the source
    SegaUiID                 DragDropHoldJustPressedId;          // Set when holding a payload just made ButtonBehavior() return a press.
    ImVector<unsigned char> DragDropPayloadBufHeap;             // We don't expose the ImVector<> directly, SegaUiPayload only holds pointer+size
    unsigned char           DragDropPayloadBufLocal[16];        // Local buffer for small payloads

    // Clipper
    int                             ClipperTempDataStacked;
    ImVector<SegaUiListClipperData>  ClipperTempData;

    // Tables
    SegaUiTable*                     CurrentTable;
    int                             TablesTempDataStacked;      // Temporary table data size (because we leave previous instances undestructed, we generally don't use TablesTempData.Size)
    ImVector<SegaUiTableTempData>    TablesTempData;             // Temporary table data (buffers reused/shared across instances, support nesting)
    ImPool<SegaUiTable>              Tables;                     // Persistent table data
    ImVector<float>                 TablesLastTimeActive;       // Last used timestamp of each tables (SOA, for efficient GC)
    ImVector<ImDrawChannel>         DrawChannelsTempMergeBuffer;

    // Tab bars
    SegaUiTabBar*                    CurrentTabBar;
    ImPool<SegaUiTabBar>             TabBars;
    ImVector<SegaUiPtrOrIndex>       CurrentTabBarStack;
    ImVector<SegaUiShrinkWidthItem>  ShrinkWidthBuffer;

    // Hover Delay system
    SegaUiID                 HoverDelayId;
    SegaUiID                 HoverDelayIdPreviousFrame;
    float                   HoverDelayTimer;                    // Currently used IsItemHovered(), generally inferred from g.HoveredIdTimer but kept uncleared until clear timer elapse.
    float                   HoverDelayClearTimer;               // Currently used IsItemHovered(): grace time before g.TooltipHoverTimer gets cleared.

    // Widget state
    ImVec2                  MouseLastValidPos;
    SegaUiInputTextState     InputTextState;
    SegaUiInputTextDeactivatedState InputTextDeactivatedState;
    ImFont                  InputTextPasswordFont;
    SegaUiID                 TempInputId;                        // Temporary text input when CTRL+clicking on a slider, etc.
    SegaUiColorEditFlags     ColorEditOptions;                   // Store user options for color edit widgets
    SegaUiID                 ColorEditCurrentID;                 // Set temporarily while inside of the parent-most ColorEdit4/ColorPicker4 (because they call each others).
    SegaUiID                 ColorEditSavedID;                   // ID we are saving/restoring HS for
    float                   ColorEditSavedHue;                  // Backup of last Hue associated to LastColor, so we can restore Hue in lossy RGB<>HSV round trips
    float                   ColorEditSavedSat;                  // Backup of last Saturation associated to LastColor, so we can restore Saturation in lossy RGB<>HSV round trips
    ImU32                   ColorEditSavedColor;                // RGB value with alpha set to 0.
    ImVec4                  ColorPickerRef;                     // Initial/reference color at the time of opening the color picker.
    SegaUiComboPreviewData   ComboPreviewData;
    float                   SliderGrabClickOffset;
    float                   SliderCurrentAccum;                 // Accumulated slider delta when using navigation controls.
    bool                    SliderCurrentAccumDirty;            // Has the accumulated slider delta changed since last time we tried to apply it?
    bool                    DragCurrentAccumDirty;
    float                   DragCurrentAccum;                   // Accumulator for dragging modification. Always high-precision, not rounded by end-user precision settings
    float                   DragSpeedDefaultRatio;              // If speed == 0.0f, uses (max-min) * DragSpeedDefaultRatio
    float                   ScrollbarClickDeltaToGrabCenter;    // Distance between mouse and center of grab box, normalized in parent space. Use storage?
    float                   DisabledAlphaBackup;                // Backup for style.Alpha for BeginDisabled()
    short                   DisabledStackSize;
    short                   TooltipOverrideCount;
    ImVector<char>          ClipboardHandlerData;               // If no custom clipboard handler is defined
    ImVector<SegaUiID>       MenusIdSubmittedThisFrame;          // A list of menu IDs that were rendered at least once

    // Platform support
    SegaUiPlatformImeData    PlatformImeData;                    // Data updated by current frame
    SegaUiPlatformImeData    PlatformImeDataPrev;                // Previous frame data (when changing we will call io.SetPlatformImeDataFn
    char                    PlatformLocaleDecimalPoint;         // '.' or *localeconv()->decimal_point

    // Settings
    bool                    SettingsLoaded;
    float                   SettingsDirtyTimer;                 // Save .ini Settings to memory when time reaches zero
    SegaUiTextBuffer         SettingsIniData;                    // In memory .ini settings
    ImVector<SegaUiSettingsHandler>      SettingsHandlers;       // List of .ini settings handlers
    ImChunkStream<SegaUiWindowSettings>  SettingsWindows;        // SegaUiWindow .ini settings entries
    ImChunkStream<SegaUiTableSettings>   SettingsTables;         // SegaUiTable .ini settings entries
    ImVector<SegaUiContextHook>          Hooks;                  // Hooks for extensions (e.g. test engine)
    SegaUiID                             HookIdNext;             // Next available HookId

    // Localization
    const char*             LocalizationTable[SegaUiLocKey_COUNT];

    // Capture/Logging
    bool                    LogEnabled;                         // Currently capturing
    SegaUiLogType            LogType;                            // Capture target
    ImFileHandle            LogFile;                            // If != NULL log to stdout/ file
    SegaUiTextBuffer         LogBuffer;                          // Accumulation buffer when log to clipboard. This is pointer so our GSegaUi static constructor doesn't call heap allocators.
    const char*             LogNextPrefix;
    const char*             LogNextSuffix;
    float                   LogLinePosY;
    bool                    LogLineFirstItem;
    int                     LogDepthRef;
    int                     LogDepthToExpand;
    int                     LogDepthToExpandDefault;            // Default/stored value for LogDepthMaxExpand if not specified in the LogXXX function call.

    // Debug Tools
    SegaUiDebugLogFlags      DebugLogFlags;
    SegaUiTextBuffer         DebugLogBuf;
    SegaUiTextIndex          DebugLogIndex;
    ImU8                    DebugLogClipperAutoDisableFrames;
    ImU8                    DebugLocateFrames;                  // For DebugLocateItemOnHover(). This is used together with DebugLocateId which is in a hot/cached spot above.
    ImS8                    DebugBeginReturnValueCullDepth;     // Cycle between 0..9 then wrap around.
    bool                    DebugItemPickerActive;              // Item picker is active (started with DebugStartItemPicker())
    ImU8                    DebugItemPickerMouseButton;
    SegaUiID                 DebugItemPickerBreakId;             // Will call IM_DEBUG_BREAK() when encountering this ID
    SegaUiMetricsConfig      DebugMetricsConfig;
    SegaUiStackTool          DebugStackTool;

    // Misc
    float                   FramerateSecPerFrame[60];           // Calculate estimate of framerate for user over the last 60 frames..
    int                     FramerateSecPerFrameIdx;
    int                     FramerateSecPerFrameCount;
    float                   FramerateSecPerFrameAccum;
    int                     WantCaptureMouseNextFrame;          // Explicit capture override via SetNextFrameWantCaptureMouse()/SetNextFrameWantCaptureKeyboard(). Default to -1.
    int                     WantCaptureKeyboardNextFrame;       // "
    int                     WantTextInputNextFrame;
    ImVector<char>          TempBuffer;                         // Temporary text buffer

    SegaUiContext(ImFontAtlas* shared_font_atlas)
    {
        IO.Ctx = this;
        InputTextState.Ctx = this;

        Initialized = false;
        FontAtlasOwnedByContext = shared_font_atlas ? false : true;
        Font = NULL;
        FontSize = FontBaseSize = 0.0f;
        IO.Fonts = shared_font_atlas ? shared_font_atlas : IM_NEW(ImFontAtlas)();
        Time = 0.0f;
        FrameCount = 0;
        FrameCountEnded = FrameCountRendered = -1;
        WithinFrameScope = WithinFrameScopeWithImplicitWindow = WithinEndChild = false;
        GcCompactAll = false;
        TestEngineHookItems = false;
        TestEngine = NULL;

        InputEventsNextMouseSource = SegaUiMouseSource_Mouse;
        InputEventsNextEventId = 1;

        WindowsActiveCount = 0;
        CurrentWindow = NULL;
        HoveredWindow = NULL;
        HoveredWindowUnderMovingWindow = NULL;
        MovingWindow = NULL;
        WheelingWindow = NULL;
        WheelingWindowStartFrame = -1;
        WheelingWindowReleaseTimer = 0.0f;

        DebugHookIdInfo = 0;
        HoveredId = HoveredIdPreviousFrame = 0;
        HoveredIdAllowOverlap = false;
        HoveredIdDisabled = false;
        HoveredIdTimer = HoveredIdNotActiveTimer = 0.0f;
        ActiveId = 0;
        ActiveIdIsAlive = 0;
        ActiveIdTimer = 0.0f;
        ActiveIdIsJustActivated = false;
        ActiveIdAllowOverlap = false;
        ActiveIdNoClearOnFocusLoss = false;
        ActiveIdHasBeenPressedBefore = false;
        ActiveIdHasBeenEditedBefore = false;
        ActiveIdHasBeenEditedThisFrame = false;
        ActiveIdClickOffset = ImVec2(-1, -1);
        ActiveIdWindow = NULL;
        ActiveIdSource = SegaUiInputSource_None;
        ActiveIdMouseButton = -1;
        ActiveIdPreviousFrame = 0;
        ActiveIdPreviousFrameIsAlive = false;
        ActiveIdPreviousFrameHasBeenEditedBefore = false;
        ActiveIdPreviousFrameWindow = NULL;
        LastActiveId = 0;
        LastActiveIdTimer = 0.0f;

        ActiveIdUsingNavDirMask = 0x00;
        ActiveIdUsingAllKeyboardKeys = false;
#ifndef SEGAUI_DISABLE_OBSOLETE_KEYIO
        ActiveIdUsingNavInputMask = 0x00;
#endif

        CurrentFocusScopeId = 0;
        CurrentItemFlags = SegaUiItemFlags_None;
        BeginMenuCount = 0;

        NavWindow = NULL;
        NavId = NavFocusScopeId = NavActivateId = NavActivateDownId = NavActivatePressedId = 0;
        NavJustMovedToId = NavJustMovedToFocusScopeId = NavNextActivateId = 0;
        NavActivateFlags = NavNextActivateFlags = SegaUiActivateFlags_None;
        NavJustMovedToKeyMods = SegaUiMod_None;
        NavInputSource = SegaUiInputSource_Keyboard;
        NavLayer = SegaUiNavLayer_Main;
        NavIdIsAlive = false;
        NavMousePosDirty = false;
        NavDisableHighlight = true;
        NavDisableMouseHover = false;
        NavAnyRequest = false;
        NavInitRequest = false;
        NavInitRequestFromMove = false;
        NavInitResultId = 0;
        NavMoveSubmitted = false;
        NavMoveScoringItems = false;
        NavMoveForwardToNextFrame = false;
        NavMoveFlags = SegaUiNavMoveFlags_None;
        NavMoveScrollFlags = SegaUiScrollFlags_None;
        NavMoveKeyMods = SegaUiMod_None;
        NavMoveDir = NavMoveDirForDebug = NavMoveClipDir = SegaUiDir_None;
        NavScoringDebugCount = 0;
        NavTabbingDir = 0;
        NavTabbingCounter = 0;

        ConfigNavWindowingKeyNext = SegaUiMod_Ctrl | SegaUiKey_Tab;
        ConfigNavWindowingKeyPrev = SegaUiMod_Ctrl | SegaUiMod_Shift | SegaUiKey_Tab;
        NavWindowingTarget = NavWindowingTargetAnim = NavWindowingListWindow = NULL;
        NavWindowingTimer = NavWindowingHighlightAlpha = 0.0f;
        NavWindowingToggleLayer = false;

        DimBgRatio = 0.0f;
        MouseCursor = SegaUiMouseCursor_Arrow;

        DragDropActive = DragDropWithinSource = DragDropWithinTarget = false;
        DragDropSourceFlags = SegaUiDragDropFlags_None;
        DragDropSourceFrameCount = -1;
        DragDropMouseButton = -1;
        DragDropTargetId = 0;
        DragDropAcceptFlags = SegaUiDragDropFlags_None;
        DragDropAcceptIdCurrRectSurface = 0.0f;
        DragDropAcceptIdPrev = DragDropAcceptIdCurr = 0;
        DragDropAcceptFrameCount = -1;
        DragDropHoldJustPressedId = 0;
        memset(DragDropPayloadBufLocal, 0, sizeof(DragDropPayloadBufLocal));

        ClipperTempDataStacked = 0;

        CurrentTable = NULL;
        TablesTempDataStacked = 0;
        CurrentTabBar = NULL;

        HoverDelayId = HoverDelayIdPreviousFrame = 0;
        HoverDelayTimer = HoverDelayClearTimer = 0.0f;

        TempInputId = 0;
        ColorEditOptions = SegaUiColorEditFlags_DefaultOptions_;
        ColorEditCurrentID = ColorEditSavedID = 0;
        ColorEditSavedHue = ColorEditSavedSat = 0.0f;
        ColorEditSavedColor = 0;
        SliderGrabClickOffset = 0.0f;
        SliderCurrentAccum = 0.0f;
        SliderCurrentAccumDirty = false;
        DragCurrentAccumDirty = false;
        DragCurrentAccum = 0.0f;
        DragSpeedDefaultRatio = 1.0f / 100.0f;
        ScrollbarClickDeltaToGrabCenter = 0.0f;
        DisabledAlphaBackup = 0.0f;
        DisabledStackSize = 0;
        TooltipOverrideCount = 0;

        PlatformImeData.InputPos = ImVec2(0.0f, 0.0f);
        PlatformImeDataPrev.InputPos = ImVec2(-1.0f, -1.0f); // Different to ensure initial submission
        PlatformLocaleDecimalPoint = '.';

        SettingsLoaded = false;
        SettingsDirtyTimer = 0.0f;
        HookIdNext = 0;

        memset(LocalizationTable, 0, sizeof(LocalizationTable));

        LogEnabled = false;
        LogType = SegaUiLogType_None;
        LogNextPrefix = LogNextSuffix = NULL;
        LogFile = NULL;
        LogLinePosY = FLT_MAX;
        LogLineFirstItem = false;
        LogDepthRef = 0;
        LogDepthToExpand = LogDepthToExpandDefault = 2;

        DebugLogFlags = SegaUiDebugLogFlags_OutputToTTY;
        DebugLocateId = 0;
        DebugLogClipperAutoDisableFrames = 0;
        DebugLocateFrames = 0;
        DebugBeginReturnValueCullDepth = -1;
        DebugItemPickerActive = false;
        DebugItemPickerMouseButton = SegaUiMouseButton_Left;
        DebugItemPickerBreakId = 0;

        memset(FramerateSecPerFrame, 0, sizeof(FramerateSecPerFrame));
        FramerateSecPerFrameIdx = FramerateSecPerFrameCount = 0;
        FramerateSecPerFrameAccum = 0.0f;
        WantCaptureMouseNextFrame = WantCaptureKeyboardNextFrame = WantTextInputNextFrame = -1;
    }
};

//-----------------------------------------------------------------------------
// [SECTION] SegaUiWindowTempData, SegaUiWindow
//-----------------------------------------------------------------------------

// Transient per-window data, reset at the beginning of the frame. This used to be called SegaUiDrawContext, hence the DC variable name in SegaUiWindow.
// (That's theory, in practice the delimitation between SegaUiWindow and SegaUiWindowTempData is quite tenuous and could be reconsidered..)
// (This doesn't need a constructor because we zero-clear it as part of SegaUiWindow and all frame-temporary data are setup on Begin)
struct SEGAUI_API SegaUiWindowTempData
{
    // Layout
    ImVec2                  CursorPos;              // Current emitting position, in absolute coordinates.
    ImVec2                  CursorPosPrevLine;
    ImVec2                  CursorStartPos;         // Initial position after Begin(), generally ~ window position + WindowPadding.
    ImVec2                  CursorMaxPos;           // Used to implicitly calculate ContentSize at the beginning of next frame, for scrolling range and auto-resize. Always growing during the frame.
    ImVec2                  IdealMaxPos;            // Used to implicitly calculate ContentSizeIdeal at the beginning of next frame, for auto-resize only. Always growing during the frame.
    ImVec2                  CurrLineSize;
    ImVec2                  PrevLineSize;
    float                   CurrLineTextBaseOffset; // Baseline offset (0.0f by default on a new line, generally == style.FramePadding.y when a framed item has been added).
    float                   PrevLineTextBaseOffset;
    bool                    IsSameLine;
    bool                    IsSetPos;
    ImVec1                  Indent;                 // Indentation / start position from left of window (increased by TreePush/TreePop, etc.)
    ImVec1                  ColumnsOffset;          // Offset to the current column (if ColumnsCurrent > 0). FIXME: This and the above should be a stack to allow use cases like Tree->Column->Tree. Need revamp columns API.
    ImVec1                  GroupOffset;
    ImVec2                  CursorStartPosLossyness;// Record the loss of precision of CursorStartPos due to really large scrolling amount. This is used by clipper to compensate and fix the most common use case of large scroll area.

    // Keyboard/Gamepad navigation
    SegaUiNavLayer           NavLayerCurrent;        // Current layer, 0..31 (we currently only use 0..1)
    short                   NavLayersActiveMask;    // Which layers have been written to (result from previous frame)
    short                   NavLayersActiveMaskNext;// Which layers have been written to (accumulator for current frame)
    bool                    NavIsScrollPushableX;   // Set when current work location may be scrolled horizontally when moving left / right. This is generally always true UNLESS within a column.
    bool                    NavHideHighlightOneFrame;
    bool                    NavWindowHasScrollY;    // Set per window when scrolling can be used (== ScrollMax.y > 0.0f)

    // Miscellaneous
    bool                    MenuBarAppending;       // FIXME: Remove this
    ImVec2                  MenuBarOffset;          // MenuBarOffset.x is sort of equivalent of a per-layer CursorPos.x, saved/restored as we switch to the menu bar. The only situation when MenuBarOffset.y is > 0 if when (SafeAreaPadding.y > FramePadding.y), often used on TVs.
    SegaUiMenuColumns        MenuColumns;            // Simplified columns storage for menu items measurement
    int                     TreeDepth;              // Current tree depth.
    ImU32                   TreeJumpToParentOnPopMask; // Store a copy of !g.NavIdIsAlive for TreeDepth 0..31.. Could be turned into a ImU64 if necessary.
    ImVector<SegaUiWindow*>  ChildWindows;
    SegaUiStorage*           StateStorage;           // Current persistent per-window storage (store e.g. tree node open/close state)
    SegaUiOldColumns*        CurrentColumns;         // Current columns set
    int                     CurrentTableIdx;        // Current table index (into g.Tables)
    SegaUiLayoutType         LayoutType;
    SegaUiLayoutType         ParentLayoutType;       // Layout type of parent window at the time of Begin()

    // Local parameters stacks
    // We store the current settings outside of the vectors to increase memory locality (reduce cache misses). The vectors are rarely modified. Also it allows us to not heap allocate for short-lived windows which are not using those settings.
    float                   ItemWidth;              // Current item width (>0.0: width in pixels, <0.0: align xx pixels to the right of window).
    float                   TextWrapPos;            // Current text wrap pos.
    ImVector<float>         ItemWidthStack;         // Store item widths to restore (attention: .back() is not == ItemWidth)
    ImVector<float>         TextWrapPosStack;       // Store text wrap pos to restore (attention: .back() is not == TextWrapPos)
};

// Storage for one window
struct SEGAUI_API SegaUiWindow
{
    SegaUiContext*           Ctx;                                // Parent UI context (needs to be set explicitly by parent).
    char*                   Name;                               // Window name, owned by the window.
    SegaUiID                 ID;                                 // == ImHashStr(Name)
    SegaUiWindowFlags        Flags;                              // See enum SegaUiWindowFlags_
    SegaUiViewportP*         Viewport;                           // Always set in Begin(). Inactive windows may have a NULL value here if their viewport was discarded.
    ImVec2                  Pos;                                // Position (always rounded-up to nearest pixel)
    ImVec2                  Size;                               // Current size (==SizeFull or collapsed title bar size)
    ImVec2                  SizeFull;                           // Size when non collapsed
    ImVec2                  ContentSize;                        // Size of contents/scrollable client area (calculated from the extents reach of the cursor) from previous frame. Does not include window decoration or window padding.
    ImVec2                  ContentSizeIdeal;
    ImVec2                  ContentSizeExplicit;                // Size of contents/scrollable client area explicitly request by the user via SetNextWindowContentSize().
    ImVec2                  WindowPadding;                      // Window padding at the time of Begin().
    float                   WindowRounding;                     // Window rounding at the time of Begin(). May be clamped lower to avoid rendering artifacts with title bar, menu bar etc.
    float                   WindowBorderSize;                   // Window border size at the time of Begin().
    float                   DecoOuterSizeX1, DecoOuterSizeY1;   // Left/Up offsets. Sum of non-scrolling outer decorations (X1 generally == 0.0f. Y1 generally = TitleBarHeight + MenuBarHeight). Locked during Begin().
    float                   DecoOuterSizeX2, DecoOuterSizeY2;   // Right/Down offsets (X2 generally == ScrollbarSize.x, Y2 == ScrollbarSizes.y).
    float                   DecoInnerSizeX1, DecoInnerSizeY1;   // Applied AFTER/OVER InnerRect. Specialized for Tables as they use specialized form of clipping and frozen rows/columns are inside InnerRect (and not part of regular decoration sizes).
    int                     NameBufLen;                         // Size of buffer storing Name. May be larger than strlen(Name)!
    SegaUiID                 MoveId;                             // == window->GetID("#MOVE")
    SegaUiID                 ChildId;                            // ID of corresponding item in parent window (for navigation to return from child window to parent window)
    ImVec2                  Scroll;
    ImVec2                  ScrollMax;
    ImVec2                  ScrollTarget;                       // target scroll position. stored as cursor position with scrolling canceled out, so the highest point is always 0.0f. (FLT_MAX for no change)
    ImVec2                  ScrollTargetCenterRatio;            // 0.0f = scroll so that target position is at top, 0.5f = scroll so that target position is centered
    ImVec2                  ScrollTargetEdgeSnapDist;           // 0.0f = no snapping, >0.0f snapping threshold
    ImVec2                  ScrollbarSizes;                     // Size taken by each scrollbars on their smaller axis. Pay attention! ScrollbarSizes.x == width of the vertical scrollbar, ScrollbarSizes.y = height of the horizontal scrollbar.
    bool                    ScrollbarX, ScrollbarY;             // Are scrollbars visible?
    bool                    Active;                             // Set to true on Begin(), unless Collapsed
    bool                    WasActive;
    bool                    WriteAccessed;                      // Set to true when any widget access the current window
    bool                    Collapsed;                          // Set when collapsing window to become only title-bar
    bool                    WantCollapseToggle;
    bool                    SkipItems;                          // Set when items can safely be all clipped (e.g. window not visible or collapsed)
    bool                    Appearing;                          // Set during the frame where the window is appearing (or re-appearing)
    bool                    Hidden;                             // Do not display (== HiddenFrames*** > 0)
    bool                    IsFallbackWindow;                   // Set on the "Debug##Default" window.
    bool                    IsExplicitChild;                    // Set when passed _ChildWindow, left to false by BeginDocked()
    bool                    HasCloseButton;                     // Set when the window has a close button (p_open != NULL)
    signed char             ResizeBorderHeld;                   // Current border being held for resize (-1: none, otherwise 0-3)
    short                   BeginCount;                         // Number of Begin() during the current frame (generally 0 or 1, 1+ if appending via multiple Begin/End pairs)
    short                   BeginCountPreviousFrame;            // Number of Begin() during the previous frame
    short                   BeginOrderWithinParent;             // Begin() order within immediate parent window, if we are a child window. Otherwise 0.
    short                   BeginOrderWithinContext;            // Begin() order within entire imgui context. This is mostly used for debugging submission order related issues.
    short                   FocusOrder;                         // Order within WindowsFocusOrder[], altered when windows are focused.
    SegaUiID                 PopupId;                            // ID in the popup stack when this window is used as a popup/menu (because we use generic Name/ID for recycling)
    ImS8                    AutoFitFramesX, AutoFitFramesY;
    ImS8                    AutoFitChildAxises;
    bool                    AutoFitOnlyGrows;
    SegaUiDir                AutoPosLastDirection;
    ImS8                    HiddenFramesCanSkipItems;           // Hide the window for N frames
    ImS8                    HiddenFramesCannotSkipItems;        // Hide the window for N frames while allowing items to be submitted so we can measure their size
    ImS8                    HiddenFramesForRenderOnly;          // Hide the window until frame N at Render() time only
    ImS8                    DisableInputsFrames;                // Disable window interactions for N frames
    SegaUiCond               SetWindowPosAllowFlags : 8;         // store acceptable condition flags for SetNextWindowPos() use.
    SegaUiCond               SetWindowSizeAllowFlags : 8;        // store acceptable condition flags for SetNextWindowSize() use.
    SegaUiCond               SetWindowCollapsedAllowFlags : 8;   // store acceptable condition flags for SetNextWindowCollapsed() use.
    ImVec2                  SetWindowPosVal;                    // store window position when using a non-zero Pivot (position set needs to be processed when we know the window size)
    ImVec2                  SetWindowPosPivot;                  // store window pivot for positioning. ImVec2(0, 0) when positioning from top-left corner; ImVec2(0.5f, 0.5f) for centering; ImVec2(1, 1) for bottom right.

    ImVector<SegaUiID>       IDStack;                            // ID stack. ID are hashes seeded with the value at the top of the stack. (In theory this should be in the TempData structure)
    SegaUiWindowTempData     DC;                                 // Temporary per-window data, reset at the beginning of the frame. This used to be called SegaUiDrawContext, hence the "DC" variable name.

    // The best way to understand what those rectangles are is to use the 'Metrics->Tools->Show Windows Rectangles' viewer.
    // The main 'OuterRect', omitted as a field, is window->Rect().
    ImRect                  OuterRectClipped;                   // == Window->Rect() just after setup in Begin(). == window->Rect() for root window.
    ImRect                  InnerRect;                          // Inner rectangle (omit title bar, menu bar, scroll bar)
    ImRect                  InnerClipRect;                      // == InnerRect shrunk by WindowPadding*0.5f on each side, clipped within viewport or parent clip rect.
    ImRect                  WorkRect;                           // Initially covers the whole scrolling region. Reduced by containers e.g columns/tables when active. Shrunk by WindowPadding*1.0f on each side. This is meant to replace ContentRegionRect over time (from 1.71+ onward).
    ImRect                  ParentWorkRect;                     // Backup of WorkRect before entering a container such as columns/tables. Used by e.g. SpanAllColumns functions to easily access. Stacked containers are responsible for maintaining this. // FIXME-WORKRECT: Could be a stack?
    ImRect                  ClipRect;                           // Current clipping/scissoring rectangle, evolve as we are using PushClipRect(), etc. == DrawList->clip_rect_stack.back().
    ImRect                  ContentRegionRect;                  // FIXME: This is currently confusing/misleading. It is essentially WorkRect but not handling of scrolling. We currently rely on it as right/bottom aligned sizing operation need some size to rely on.
    ImVec2ih                HitTestHoleSize;                    // Define an optional rectangular hole where mouse will pass-through the window.
    ImVec2ih                HitTestHoleOffset;

    int                     LastFrameActive;                    // Last frame number the window was Active.
    float                   LastTimeActive;                     // Last timestamp the window was Active (using float as we don't need high precision there)
    float                   ItemWidthDefault;
    SegaUiStorage            StateStorage;
    ImVector<SegaUiOldColumns> ColumnsStorage;
    float                   FontWindowScale;                    // User scale multiplier per-window, via SetWindowFontScale()
    int                     SettingsOffset;                     // Offset into SettingsWindows[] (offsets are always valid as we only grow the array from the back)

    ImDrawList*             DrawList;                           // == &DrawListInst (for backward compatibility reason with code using imgui_internal.h we keep this a pointer)
    ImDrawList              DrawListInst;
    SegaUiWindow*            ParentWindow;                       // If we are a child _or_ popup _or_ docked window, this is pointing to our parent. Otherwise NULL.
    SegaUiWindow*            ParentWindowInBeginStack;
    SegaUiWindow*            RootWindow;                         // Point to ourself or first ancestor that is not a child window. Doesn't cross through popups/dock nodes.
    SegaUiWindow*            RootWindowPopupTree;                // Point to ourself or first ancestor that is not a child window. Cross through popups parent<>child.
    SegaUiWindow*            RootWindowForTitleBarHighlight;     // Point to ourself or first ancestor which will display TitleBgActive color when this window is active.
    SegaUiWindow*            RootWindowForNav;                   // Point to ourself or first ancestor which doesn't have the NavFlattened flag.

    SegaUiWindow*            NavLastChildNavWindow;              // When going to the menu bar, we remember the child window we came from. (This could probably be made implicit if we kept g.Windows sorted by last focused including child window.)
    SegaUiID                 NavLastIds[SegaUiNavLayer_COUNT];    // Last known NavId for this window, per layer (0/1)
    ImRect                  NavRectRel[SegaUiNavLayer_COUNT];    // Reference rectangle, in window relative space
    ImVec2                  NavPreferredScoringPosRel[SegaUiNavLayer_COUNT]; // Preferred X/Y position updated when moving on a given axis, reset to FLT_MAX.
    SegaUiID                 NavRootFocusScopeId;                // Focus Scope ID at the time of Begin()

    int                     MemoryDrawListIdxCapacity;          // Backup of last idx/vtx count, so when waking up the window we can preallocate and avoid iterative alloc/copy
    int                     MemoryDrawListVtxCapacity;
    bool                    MemoryCompacted;                    // Set when window extraneous data have been garbage collected

public:
    SegaUiWindow(SegaUiContext* context, const char* name);
    ~SegaUiWindow();

    SegaUiID     GetID(const char* str, const char* str_end = NULL);
    SegaUiID     GetID(const void* ptr);
    SegaUiID     GetID(int n);
    SegaUiID     GetIDFromRectangle(const ImRect& r_abs);

    // We don't use g.FontSize because the window may be != g.CurrentWindow.
    ImRect      Rect() const            { return ImRect(Pos.x, Pos.y, Pos.x + Size.x, Pos.y + Size.y); }
    float       CalcFontSize() const    { SegaUiContext& g = *Ctx; float scale = g.FontBaseSize * FontWindowScale; if (ParentWindow) scale *= ParentWindow->FontWindowScale; return scale; }
    float       TitleBarHeight() const  { SegaUiContext& g = *Ctx; return (Flags & SegaUiWindowFlags_NoTitleBar) ? 0.0f : CalcFontSize() + g.Style.FramePadding.y * 2.0f; }
    ImRect      TitleBarRect() const    { return ImRect(Pos, ImVec2(Pos.x + SizeFull.x, Pos.y + TitleBarHeight())); }
    float       MenuBarHeight() const   { SegaUiContext& g = *Ctx; return (Flags & SegaUiWindowFlags_MenuBar) ? DC.MenuBarOffset.y + CalcFontSize() + g.Style.FramePadding.y * 2.0f : 0.0f; }
    ImRect      MenuBarRect() const     { float y1 = Pos.y + TitleBarHeight(); return ImRect(Pos.x, y1, Pos.x + SizeFull.x, y1 + MenuBarHeight()); }
};

//-----------------------------------------------------------------------------
// [SECTION] Tab bar, Tab item support
//-----------------------------------------------------------------------------

// Extend SegaUiTabBarFlags_
enum SegaUiTabBarFlagsPrivate_
{
    SegaUiTabBarFlags_DockNode                   = 1 << 20,  // Part of a dock node [we don't use this in the master branch but it facilitate branch syncing to keep this around]
    SegaUiTabBarFlags_IsFocused                  = 1 << 21,
    SegaUiTabBarFlags_SaveSettings               = 1 << 22,  // FIXME: Settings are handled by the docking system, this only request the tab bar to mark settings dirty when reordering tabs
};

// Extend SegaUiTabItemFlags_
enum SegaUiTabItemFlagsPrivate_
{
    SegaUiTabItemFlags_SectionMask_              = SegaUiTabItemFlags_Leading | SegaUiTabItemFlags_Trailing,
    SegaUiTabItemFlags_NoCloseButton             = 1 << 20,  // Track whether p_open was set or not (we'll need this info on the next frame to recompute ContentWidth during layout)
    SegaUiTabItemFlags_Button                    = 1 << 21,  // Used by TabItemButton, change the tab item behavior to mimic a button
};

// Storage for one active tab item (sizeof() 40 bytes)
struct SegaUiTabItem
{
    SegaUiID             ID;
    SegaUiTabItemFlags   Flags;
    int                 LastFrameVisible;
    int                 LastFrameSelected;      // This allows us to infer an ordered list of the last activated tabs with little maintenance
    float               Offset;                 // Position relative to beginning of tab
    float               Width;                  // Width currently displayed
    float               ContentWidth;           // Width of label, stored during BeginTabItem() call
    float               RequestedWidth;         // Width optionally requested by caller, -1.0f is unused
    ImS32               NameOffset;             // When Window==NULL, offset to name within parent SegaUiTabBar::TabsNames
    ImS16               BeginOrder;             // BeginTabItem() order, used to re-order tabs after toggling SegaUiTabBarFlags_Reorderable
    ImS16               IndexDuringLayout;      // Index only used during TabBarLayout(). Tabs gets reordered so 'Tabs[n].IndexDuringLayout == n' but may mismatch during additions.
    bool                WantClose;              // Marked as closed by SetTabItemClosed()

    SegaUiTabItem()      { memset(this, 0, sizeof(*this)); LastFrameVisible = LastFrameSelected = -1; RequestedWidth = -1.0f; NameOffset = -1; BeginOrder = IndexDuringLayout = -1; }
};

// Storage for a tab bar (sizeof() 152 bytes)
struct SEGAUI_API SegaUiTabBar
{
    ImVector<SegaUiTabItem> Tabs;
    SegaUiTabBarFlags    Flags;
    SegaUiID             ID;                     // Zero for tab-bars used by docking
    SegaUiID             SelectedTabId;          // Selected tab/window
    SegaUiID             NextSelectedTabId;      // Next selected tab/window. Will also trigger a scrolling animation
    SegaUiID             VisibleTabId;           // Can occasionally be != SelectedTabId (e.g. when previewing contents for CTRL+TAB preview)
    int                 CurrFrameVisible;
    int                 PrevFrameVisible;
    ImRect              BarRect;
    float               CurrTabsContentsHeight;
    float               PrevTabsContentsHeight; // Record the height of contents submitted below the tab bar
    float               WidthAllTabs;           // Actual width of all tabs (locked during layout)
    float               WidthAllTabsIdeal;      // Ideal width if all tabs were visible and not clipped
    float               ScrollingAnim;
    float               ScrollingTarget;
    float               ScrollingTargetDistToVisibility;
    float               ScrollingSpeed;
    float               ScrollingRectMinX;
    float               ScrollingRectMaxX;
    SegaUiID             ReorderRequestTabId;
    ImS16               ReorderRequestOffset;
    ImS8                BeginCount;
    bool                WantLayout;
    bool                VisibleTabWasSubmitted;
    bool                TabsAddedNew;           // Set to true when a new tab item or button has been added to the tab bar during last frame
    ImS16               TabsActiveCount;        // Number of tabs submitted this frame.
    ImS16               LastTabItemIdx;         // Index of last BeginTabItem() tab for use by EndTabItem()
    float               ItemSpacingY;
    ImVec2              FramePadding;           // style.FramePadding locked at the time of BeginTabBar()
    ImVec2              BackupCursorPos;
    SegaUiTextBuffer     TabsNames;              // For non-docking tab bar we re-append names in a contiguous buffer.

    SegaUiTabBar();
};

//-----------------------------------------------------------------------------
// [SECTION] Table support
//-----------------------------------------------------------------------------

#define IM_COL32_DISABLE                IM_COL32(0,0,0,1)   // Special sentinel code which cannot be used as a regular color.
#define SEGAUI_TABLE_MAX_COLUMNS         512                 // May be further lifted

// Our current column maximum is 64 but we may raise that in the future.
typedef ImS16 SegaUiTableColumnIdx;
typedef ImU16 SegaUiTableDrawChannelIdx;

// [Internal] sizeof() ~ 104
// We use the terminology "Enabled" to refer to a column that is not Hidden by user/api.
// We use the terminology "Clipped" to refer to a column that is out of sight because of scrolling/clipping.
// This is in contrast with some user-facing api such as IsItemVisible() / IsRectVisible() which use "Visible" to mean "not clipped".
struct SegaUiTableColumn
{
    SegaUiTableColumnFlags   Flags;                          // Flags after some patching (not directly same as provided by user). See SegaUiTableColumnFlags_
    float                   WidthGiven;                     // Final/actual width visible == (MaxX - MinX), locked in TableUpdateLayout(). May be > WidthRequest to honor minimum width, may be < WidthRequest to honor shrinking columns down in tight space.
    float                   MinX;                           // Absolute positions
    float                   MaxX;
    float                   WidthRequest;                   // Master width absolute value when !(Flags & _WidthStretch). When Stretch this is derived every frame from StretchWeight in TableUpdateLayout()
    float                   WidthAuto;                      // Automatic width
    float                   StretchWeight;                  // Master width weight when (Flags & _WidthStretch). Often around ~1.0f initially.
    float                   InitStretchWeightOrWidth;       // Value passed to TableSetupColumn(). For Width it is a content width (_without padding_).
    ImRect                  ClipRect;                       // Clipping rectangle for the column
    SegaUiID                 UserID;                         // Optional, value passed to TableSetupColumn()
    float                   WorkMinX;                       // Contents region min ~(MinX + CellPaddingX + CellSpacingX1) == cursor start position when entering column
    float                   WorkMaxX;                       // Contents region max ~(MaxX - CellPaddingX - CellSpacingX2)
    float                   ItemWidth;                      // Current item width for the column, preserved across rows
    float                   ContentMaxXFrozen;              // Contents maximum position for frozen rows (apart from headers), from which we can infer content width.
    float                   ContentMaxXUnfrozen;
    float                   ContentMaxXHeadersUsed;         // Contents maximum position for headers rows (regardless of freezing). TableHeader() automatically softclip itself + report ideal desired size, to avoid creating extraneous draw calls
    float                   ContentMaxXHeadersIdeal;
    ImS16                   NameOffset;                     // Offset into parent ColumnsNames[]
    SegaUiTableColumnIdx     DisplayOrder;                   // Index within Table's IndexToDisplayOrder[] (column may be reordered by users)
    SegaUiTableColumnIdx     IndexWithinEnabledSet;          // Index within enabled/visible set (<= IndexToDisplayOrder)
    SegaUiTableColumnIdx     PrevEnabledColumn;              // Index of prev enabled/visible column within Columns[], -1 if first enabled/visible column
    SegaUiTableColumnIdx     NextEnabledColumn;              // Index of next enabled/visible column within Columns[], -1 if last enabled/visible column
    SegaUiTableColumnIdx     SortOrder;                      // Index of this column within sort specs, -1 if not sorting on this column, 0 for single-sort, may be >0 on multi-sort
    SegaUiTableDrawChannelIdx DrawChannelCurrent;            // Index within DrawSplitter.Channels[]
    SegaUiTableDrawChannelIdx DrawChannelFrozen;             // Draw channels for frozen rows (often headers)
    SegaUiTableDrawChannelIdx DrawChannelUnfrozen;           // Draw channels for unfrozen rows
    bool                    IsEnabled;                      // IsUserEnabled && (Flags & SegaUiTableColumnFlags_Disabled) == 0
    bool                    IsUserEnabled;                  // Is the column not marked Hidden by the user? (unrelated to being off view, e.g. clipped by scrolling).
    bool                    IsUserEnabledNextFrame;
    bool                    IsVisibleX;                     // Is actually in view (e.g. overlapping the host window clipping rectangle, not scrolled).
    bool                    IsVisibleY;
    bool                    IsRequestOutput;                // Return value for TableSetColumnIndex() / TableNextColumn(): whether we request user to output contents or not.
    bool                    IsSkipItems;                    // Do we want item submissions to this column to be completely ignored (no layout will happen).
    bool                    IsPreserveWidthAuto;
    ImS8                    NavLayerCurrent;                // SegaUiNavLayer in 1 byte
    ImU8                    AutoFitQueue;                   // Queue of 8 values for the next 8 frames to request auto-fit
    ImU8                    CannotSkipItemsQueue;           // Queue of 8 values for the next 8 frames to disable Clipped/SkipItem
    ImU8                    SortDirection : 2;              // SegaUiSortDirection_Ascending or SegaUiSortDirection_Descending
    ImU8                    SortDirectionsAvailCount : 2;   // Number of available sort directions (0 to 3)
    ImU8                    SortDirectionsAvailMask : 4;    // Mask of available sort directions (1-bit each)
    ImU8                    SortDirectionsAvailList;        // Ordered of available sort directions (2-bits each)

    SegaUiTableColumn()
    {
        memset(this, 0, sizeof(*this));
        StretchWeight = WidthRequest = -1.0f;
        NameOffset = -1;
        DisplayOrder = IndexWithinEnabledSet = -1;
        PrevEnabledColumn = NextEnabledColumn = -1;
        SortOrder = -1;
        SortDirection = SegaUiSortDirection_None;
        DrawChannelCurrent = DrawChannelFrozen = DrawChannelUnfrozen = (ImU8)-1;
    }
};

// Transient cell data stored per row.
// sizeof() ~ 6
struct SegaUiTableCellData
{
    ImU32                       BgColor;    // Actual color
    SegaUiTableColumnIdx         Column;     // Column number
};

// Per-instance data that needs preserving across frames (seemingly most others do not need to be preserved aside from debug needs. Does that means they could be moved to SegaUiTableTempData?)
struct SegaUiTableInstanceData
{
    SegaUiID                     TableInstanceID;
    float                       LastOuterHeight;            // Outer height from last frame
    float                       LastFirstRowHeight;         // Height of first row from last frame (FIXME: this is used as "header height" and may be reworked)
    float                       LastFrozenHeight;           // Height of frozen section from last frame

    SegaUiTableInstanceData()    { TableInstanceID = 0; LastOuterHeight = LastFirstRowHeight = LastFrozenHeight = 0.0f; }
};

// FIXME-TABLE: more transient data could be stored in a stacked SegaUiTableTempData: e.g. SortSpecs, incoming RowData
struct SEGAUI_API SegaUiTable
{
    SegaUiID                     ID;
    SegaUiTableFlags             Flags;
    void*                       RawData;                    // Single allocation to hold Columns[], DisplayOrderToIndex[] and RowCellData[]
    SegaUiTableTempData*         TempData;                   // Transient data while table is active. Point within g.CurrentTableStack[]
    ImSpan<SegaUiTableColumn>    Columns;                    // Point within RawData[]
    ImSpan<SegaUiTableColumnIdx> DisplayOrderToIndex;        // Point within RawData[]. Store display order of columns (when not reordered, the values are 0...Count-1)
    ImSpan<SegaUiTableCellData>  RowCellData;                // Point within RawData[]. Store cells background requests for current row.
    ImBitArrayPtr               EnabledMaskByDisplayOrder;  // Column DisplayOrder -> IsEnabled map
    ImBitArrayPtr               EnabledMaskByIndex;         // Column Index -> IsEnabled map (== not hidden by user/api) in a format adequate for iterating column without touching cold data
    ImBitArrayPtr               VisibleMaskByIndex;         // Column Index -> IsVisibleX|IsVisibleY map (== not hidden by user/api && not hidden by scrolling/cliprect)
    SegaUiTableFlags             SettingsLoadedFlags;        // Which data were loaded from the .ini file (e.g. when order is not altered we won't save order)
    int                         SettingsOffset;             // Offset in g.SettingsTables
    int                         LastFrameActive;
    int                         ColumnsCount;               // Number of columns declared in BeginTable()
    int                         CurrentRow;
    int                         CurrentColumn;
    ImS16                       InstanceCurrent;            // Count of BeginTable() calls with same ID in the same frame (generally 0). This is a little bit similar to BeginCount for a window, but multiple table with same ID look are multiple tables, they are just synched.
    ImS16                       InstanceInteracted;         // Mark which instance (generally 0) of the same ID is being interacted with
    float                       RowPosY1;
    float                       RowPosY2;
    float                       RowMinHeight;               // Height submitted to TableNextRow()
    float                       RowTextBaseline;
    float                       RowIndentOffsetX;
    SegaUiTableRowFlags          RowFlags : 16;              // Current row flags, see SegaUiTableRowFlags_
    SegaUiTableRowFlags          LastRowFlags : 16;
    int                         RowBgColorCounter;          // Counter for alternating background colors (can be fast-forwarded by e.g clipper), not same as CurrentRow because header rows typically don't increase this.
    ImU32                       RowBgColor[2];              // Background color override for current row.
    ImU32                       BorderColorStrong;
    ImU32                       BorderColorLight;
    float                       BorderX1;
    float                       BorderX2;
    float                       HostIndentX;
    float                       MinColumnWidth;
    float                       OuterPaddingX;
    float                       CellPaddingX;               // Padding from each borders
    float                       CellPaddingY;
    float                       CellSpacingX1;              // Spacing between non-bordered cells
    float                       CellSpacingX2;
    float                       InnerWidth;                 // User value passed to BeginTable(), see comments at the top of BeginTable() for details.
    float                       ColumnsGivenWidth;          // Sum of current column width
    float                       ColumnsAutoFitWidth;        // Sum of ideal column width in order nothing to be clipped, used for auto-fitting and content width submission in outer window
    float                       ColumnsStretchSumWeights;   // Sum of weight of all enabled stretching columns
    float                       ResizedColumnNextWidth;
    float                       ResizeLockMinContentsX2;    // Lock minimum contents width while resizing down in order to not create feedback loops. But we allow growing the table.
    float                       RefScale;                   // Reference scale to be able to rescale columns on font/dpi changes.
    ImRect                      OuterRect;                  // Note: for non-scrolling table, OuterRect.Max.y is often FLT_MAX until EndTable(), unless a height has been specified in BeginTable().
    ImRect                      InnerRect;                  // InnerRect but without decoration. As with OuterRect, for non-scrolling tables, InnerRect.Max.y is
    ImRect                      WorkRect;
    ImRect                      InnerClipRect;
    ImRect                      BgClipRect;                 // We use this to cpu-clip cell background color fill, evolve during the frame as we cross frozen rows boundaries
    ImRect                      Bg0ClipRectForDrawCmd;      // Actual ImDrawCmd clip rect for BG0/1 channel. This tends to be == OuterWindow->ClipRect at BeginTable() because output in BG0/BG1 is cpu-clipped
    ImRect                      Bg2ClipRectForDrawCmd;      // Actual ImDrawCmd clip rect for BG2 channel. This tends to be a correct, tight-fit, because output to BG2 are done by widgets relying on regular ClipRect.
    ImRect                      HostClipRect;               // This is used to check if we can eventually merge our columns draw calls into the current draw call of the current window.
    ImRect                      HostBackupInnerClipRect;    // Backup of InnerWindow->ClipRect during PushTableBackground()/PopTableBackground()
    SegaUiWindow*                OuterWindow;                // Parent window for the table
    SegaUiWindow*                InnerWindow;                // Window holding the table data (== OuterWindow or a child window)
    SegaUiTextBuffer             ColumnsNames;               // Contiguous buffer holding columns names
    ImDrawListSplitter*         DrawSplitter;               // Shortcut to TempData->DrawSplitter while in table. Isolate draw commands per columns to avoid switching clip rect constantly
    SegaUiTableInstanceData      InstanceDataFirst;
    ImVector<SegaUiTableInstanceData>    InstanceDataExtra;  // FIXME-OPT: Using a small-vector pattern would be good.
    SegaUiTableColumnSortSpecs   SortSpecsSingle;
    ImVector<SegaUiTableColumnSortSpecs> SortSpecsMulti;     // FIXME-OPT: Using a small-vector pattern would be good.
    SegaUiTableSortSpecs         SortSpecs;                  // Public facing sorts specs, this is what we return in TableGetSortSpecs()
    SegaUiTableColumnIdx         SortSpecsCount;
    SegaUiTableColumnIdx         ColumnsEnabledCount;        // Number of enabled columns (<= ColumnsCount)
    SegaUiTableColumnIdx         ColumnsEnabledFixedCount;   // Number of enabled columns (<= ColumnsCount)
    SegaUiTableColumnIdx         DeclColumnsCount;           // Count calls to TableSetupColumn()
    SegaUiTableColumnIdx         HoveredColumnBody;          // Index of column whose visible region is being hovered. Important: == ColumnsCount when hovering empty region after the right-most column!
    SegaUiTableColumnIdx         HoveredColumnBorder;        // Index of column whose right-border is being hovered (for resizing).
    SegaUiTableColumnIdx         AutoFitSingleColumn;        // Index of single column requesting auto-fit.
    SegaUiTableColumnIdx         ResizedColumn;              // Index of column being resized. Reset when InstanceCurrent==0.
    SegaUiTableColumnIdx         LastResizedColumn;          // Index of column being resized from previous frame.
    SegaUiTableColumnIdx         HeldHeaderColumn;           // Index of column header being held.
    SegaUiTableColumnIdx         ReorderColumn;              // Index of column being reordered. (not cleared)
    SegaUiTableColumnIdx         ReorderColumnDir;           // -1 or +1
    SegaUiTableColumnIdx         LeftMostEnabledColumn;      // Index of left-most non-hidden column.
    SegaUiTableColumnIdx         RightMostEnabledColumn;     // Index of right-most non-hidden column.
    SegaUiTableColumnIdx         LeftMostStretchedColumn;    // Index of left-most stretched column.
    SegaUiTableColumnIdx         RightMostStretchedColumn;   // Index of right-most stretched column.
    SegaUiTableColumnIdx         ContextPopupColumn;         // Column right-clicked on, of -1 if opening context menu from a neutral/empty spot
    SegaUiTableColumnIdx         FreezeRowsRequest;          // Requested frozen rows count
    SegaUiTableColumnIdx         FreezeRowsCount;            // Actual frozen row count (== FreezeRowsRequest, or == 0 when no scrolling offset)
    SegaUiTableColumnIdx         FreezeColumnsRequest;       // Requested frozen columns count
    SegaUiTableColumnIdx         FreezeColumnsCount;         // Actual frozen columns count (== FreezeColumnsRequest, or == 0 when no scrolling offset)
    SegaUiTableColumnIdx         RowCellDataCurrent;         // Index of current RowCellData[] entry in current row
    SegaUiTableDrawChannelIdx    DummyDrawChannel;           // Redirect non-visible columns here.
    SegaUiTableDrawChannelIdx    Bg2DrawChannelCurrent;      // For Selectable() and other widgets drawing across columns after the freezing line. Index within DrawSplitter.Channels[]
    SegaUiTableDrawChannelIdx    Bg2DrawChannelUnfrozen;
    bool                        IsLayoutLocked;             // Set by TableUpdateLayout() which is called when beginning the first row.
    bool                        IsInsideRow;                // Set when inside TableBeginRow()/TableEndRow().
    bool                        IsInitializing;
    bool                        IsSortSpecsDirty;
    bool                        IsUsingHeaders;             // Set when the first row had the SegaUiTableRowFlags_Headers flag.
    bool                        IsContextPopupOpen;         // Set when default context menu is open (also see: ContextPopupColumn, InstanceInteracted).
    bool                        IsSettingsRequestLoad;
    bool                        IsSettingsDirty;            // Set when table settings have changed and needs to be reported into SegaUiTableSetttings data.
    bool                        IsDefaultDisplayOrder;      // Set when display order is unchanged from default (DisplayOrder contains 0...Count-1)
    bool                        IsResetAllRequest;
    bool                        IsResetDisplayOrderRequest;
    bool                        IsUnfrozenRows;             // Set when we got past the frozen row.
    bool                        IsDefaultSizingPolicy;      // Set if user didn't explicitly set a sizing policy in BeginTable()
    bool                        HasScrollbarYCurr;          // Whether ANY instance of this table had a vertical scrollbar during the current frame.
    bool                        HasScrollbarYPrev;          // Whether ANY instance of this table had a vertical scrollbar during the previous.
    bool                        MemoryCompacted;
    bool                        HostSkipItems;              // Backup of InnerWindow->SkipItem at the end of BeginTable(), because we will overwrite InnerWindow->SkipItem on a per-column basis

    SegaUiTable()                { memset(this, 0, sizeof(*this)); LastFrameActive = -1; }
    ~SegaUiTable()               { IM_FREE(RawData); }
};

// Transient data that are only needed between BeginTable() and EndTable(), those buffers are shared (1 per level of stacked table).
// - Accessing those requires chasing an extra pointer so for very frequently used data we leave them in the main table structure.
// - We also leave out of this structure data that tend to be particularly useful for debugging/metrics.
struct SEGAUI_API SegaUiTableTempData
{
    int                         TableIndex;                 // Index in g.Tables.Buf[] pool
    float                       LastTimeActive;             // Last timestamp this structure was used

    ImVec2                      UserOuterSize;              // outer_size.x passed to BeginTable()
    ImDrawListSplitter          DrawSplitter;

    ImRect                      HostBackupWorkRect;         // Backup of InnerWindow->WorkRect at the end of BeginTable()
    ImRect                      HostBackupParentWorkRect;   // Backup of InnerWindow->ParentWorkRect at the end of BeginTable()
    ImVec2                      HostBackupPrevLineSize;     // Backup of InnerWindow->DC.PrevLineSize at the end of BeginTable()
    ImVec2                      HostBackupCurrLineSize;     // Backup of InnerWindow->DC.CurrLineSize at the end of BeginTable()
    ImVec2                      HostBackupCursorMaxPos;     // Backup of InnerWindow->DC.CursorMaxPos at the end of BeginTable()
    ImVec1                      HostBackupColumnsOffset;    // Backup of OuterWindow->DC.ColumnsOffset at the end of BeginTable()
    float                       HostBackupItemWidth;        // Backup of OuterWindow->DC.ItemWidth at the end of BeginTable()
    int                         HostBackupItemWidthStackSize;//Backup of OuterWindow->DC.ItemWidthStack.Size at the end of BeginTable()

    SegaUiTableTempData()        { memset(this, 0, sizeof(*this)); LastTimeActive = -1.0f; }
};

// sizeof() ~ 12
struct SegaUiTableColumnSettings
{
    float                   WidthOrWeight;
    SegaUiID                 UserID;
    SegaUiTableColumnIdx     Index;
    SegaUiTableColumnIdx     DisplayOrder;
    SegaUiTableColumnIdx     SortOrder;
    ImU8                    SortDirection : 2;
    ImU8                    IsEnabled : 1; // "Visible" in ini file
    ImU8                    IsStretch : 1;

    SegaUiTableColumnSettings()
    {
        WidthOrWeight = 0.0f;
        UserID = 0;
        Index = -1;
        DisplayOrder = SortOrder = -1;
        SortDirection = SegaUiSortDirection_None;
        IsEnabled = 1;
        IsStretch = 0;
    }
};

// This is designed to be stored in a single ImChunkStream (1 header followed by N SegaUiTableColumnSettings, etc.)
struct SegaUiTableSettings
{
    SegaUiID                     ID;                     // Set to 0 to invalidate/delete the setting
    SegaUiTableFlags             SaveFlags;              // Indicate data we want to save using the Resizable/Reorderable/Sortable/Hideable flags (could be using its own flags..)
    float                       RefScale;               // Reference scale to be able to rescale columns on font/dpi changes.
    SegaUiTableColumnIdx         ColumnsCount;
    SegaUiTableColumnIdx         ColumnsCountMax;        // Maximum number of columns this settings instance can store, we can recycle a settings instance with lower number of columns but not higher
    bool                        WantApply;              // Set when loaded from .ini data (to enable merging/loading .ini data into an already running context)

    SegaUiTableSettings()        { memset(this, 0, sizeof(*this)); }
    SegaUiTableColumnSettings*   GetColumnSettings()     { return (SegaUiTableColumnSettings*)(this + 1); }
};

//-----------------------------------------------------------------------------
// [SECTION] SegaUi internal API
// No guarantee of forward compatibility here!
//-----------------------------------------------------------------------------

namespace SegaUi
{
    // Windows
    // We should always have a CurrentWindow in the stack (there is an implicit "Debug" window)
    // If this ever crash because g.CurrentWindow is NULL it means that either
    // - SegaUi::NewFrame() has never been called, which is illegal.
    // - You are calling SegaUi functions after SegaUi::EndFrame()/SegaUi::Render() and before the next SegaUi::NewFrame(), which is also illegal.
    inline    SegaUiWindow*  GetCurrentWindowRead()      { SegaUiContext& g = *GSegaUi; return g.CurrentWindow; }
    inline    SegaUiWindow*  GetCurrentWindow()          { SegaUiContext& g = *GSegaUi; g.CurrentWindow->WriteAccessed = true; return g.CurrentWindow; }
    SEGAUI_API SegaUiWindow*  FindWindowByID(SegaUiID id);
    SEGAUI_API SegaUiWindow*  FindWindowByName(const char* name);
    SEGAUI_API void          UpdateWindowParentAndRootLinks(SegaUiWindow* window, SegaUiWindowFlags flags, SegaUiWindow* parent_window);
    SEGAUI_API ImVec2        CalcWindowNextAutoFitSize(SegaUiWindow* window);
    SEGAUI_API bool          IsWindowChildOf(SegaUiWindow* window, SegaUiWindow* potential_parent, bool popup_hierarchy);
    SEGAUI_API bool          IsWindowWithinBeginStackOf(SegaUiWindow* window, SegaUiWindow* potential_parent);
    SEGAUI_API bool          IsWindowAbove(SegaUiWindow* potential_above, SegaUiWindow* potential_below);
    SEGAUI_API bool          IsWindowNavFocusable(SegaUiWindow* window);
    SEGAUI_API void          SetWindowPos(SegaUiWindow* window, const ImVec2& pos, SegaUiCond cond = 0);
    SEGAUI_API void          SetWindowSize(SegaUiWindow* window, const ImVec2& size, SegaUiCond cond = 0);
    SEGAUI_API void          SetWindowCollapsed(SegaUiWindow* window, bool collapsed, SegaUiCond cond = 0);
    SEGAUI_API void          SetWindowHitTestHole(SegaUiWindow* window, const ImVec2& pos, const ImVec2& size);
    SEGAUI_API void          SetWindowHiddendAndSkipItemsForCurrentFrame(SegaUiWindow* window);
    inline ImRect           WindowRectAbsToRel(SegaUiWindow* window, const ImRect& r) { ImVec2 off = window->DC.CursorStartPos; return ImRect(r.Min.x - off.x, r.Min.y - off.y, r.Max.x - off.x, r.Max.y - off.y); }
    inline ImRect           WindowRectRelToAbs(SegaUiWindow* window, const ImRect& r) { ImVec2 off = window->DC.CursorStartPos; return ImRect(r.Min.x + off.x, r.Min.y + off.y, r.Max.x + off.x, r.Max.y + off.y); }
    inline ImVec2           WindowPosRelToAbs(SegaUiWindow* window, const ImVec2& p)  { ImVec2 off = window->DC.CursorStartPos; return ImVec2(p.x + off.x, p.y + off.y); }

    // Windows: Display Order and Focus Order
    SEGAUI_API void          FocusWindow(SegaUiWindow* window, SegaUiFocusRequestFlags flags = 0);
    SEGAUI_API void          FocusTopMostWindowUnderOne(SegaUiWindow* under_this_window, SegaUiWindow* ignore_window, SegaUiViewport* filter_viewport, SegaUiFocusRequestFlags flags);
    SEGAUI_API void          BringWindowToFocusFront(SegaUiWindow* window);
    SEGAUI_API void          BringWindowToDisplayFront(SegaUiWindow* window);
    SEGAUI_API void          BringWindowToDisplayBack(SegaUiWindow* window);
    SEGAUI_API void          BringWindowToDisplayBehind(SegaUiWindow* window, SegaUiWindow* above_window);
    SEGAUI_API int           FindWindowDisplayIndex(SegaUiWindow* window);
    SEGAUI_API SegaUiWindow*  FindBottomMostVisibleWindowWithinBeginStack(SegaUiWindow* window);

    // Fonts, drawing
    SEGAUI_API void          SetCurrentFont(ImFont* font);
    inline ImFont*          GetDefaultFont() { SegaUiContext& g = *GSegaUi; return g.IO.FontDefault ? g.IO.FontDefault : g.IO.Fonts->Fonts[0]; }
    inline ImDrawList*      GetForegroundDrawList(SegaUiWindow* window) { IM_UNUSED(window); return GetForegroundDrawList(); } // This seemingly unnecessary wrapper simplifies compatibility between the 'master' and 'docking' branches.
    SEGAUI_API ImDrawList*   GetBackgroundDrawList(SegaUiViewport* viewport);                     // get background draw list for the given viewport. this draw list will be the first rendering one. Useful to quickly draw shapes/text behind dear imgui contents.
    SEGAUI_API ImDrawList*   GetForegroundDrawList(SegaUiViewport* viewport);                     // get foreground draw list for the given viewport. this draw list will be the last rendered one. Useful to quickly draw shapes/text over dear imgui contents.

    // Init
    SEGAUI_API void          Initialize();
    SEGAUI_API void          Shutdown();    // Since 1.60 this is a _private_ function. You can call DestroyContext() to destroy the context created by CreateContext().

    // NewFrame
    SEGAUI_API void          UpdateInputEvents(bool trickle_fast_inputs);
    SEGAUI_API void          UpdateHoveredWindowAndCaptureFlags();
    SEGAUI_API void          StartMouseMovingWindow(SegaUiWindow* window);
    SEGAUI_API void          UpdateMouseMovingWindowNewFrame();
    SEGAUI_API void          UpdateMouseMovingWindowEndFrame();

    // Generic context hooks
    SEGAUI_API SegaUiID       AddContextHook(SegaUiContext* context, const SegaUiContextHook* hook);
    SEGAUI_API void          RemoveContextHook(SegaUiContext* context, SegaUiID hook_to_remove);
    SEGAUI_API void          CallContextHooks(SegaUiContext* context, SegaUiContextHookType type);

    // Viewports
    SEGAUI_API void          SetWindowViewport(SegaUiWindow* window, SegaUiViewportP* viewport);

    // Settings
    SEGAUI_API void                  MarkIniSettingsDirty();
    SEGAUI_API void                  MarkIniSettingsDirty(SegaUiWindow* window);
    SEGAUI_API void                  ClearIniSettings();
    SEGAUI_API void                  AddSettingsHandler(const SegaUiSettingsHandler* handler);
    SEGAUI_API void                  RemoveSettingsHandler(const char* type_name);
    SEGAUI_API SegaUiSettingsHandler* FindSettingsHandler(const char* type_name);

    // Settings - Windows
    SEGAUI_API SegaUiWindowSettings*  CreateNewWindowSettings(const char* name);
    SEGAUI_API SegaUiWindowSettings*  FindWindowSettingsByID(SegaUiID id);
    SEGAUI_API SegaUiWindowSettings*  FindWindowSettingsByWindow(SegaUiWindow* window);
    SEGAUI_API void                  ClearWindowSettings(const char* name);

    // Localization
    SEGAUI_API void          LocalizeRegisterEntries(const SegaUiLocEntry* entries, int count);
    inline const char*      LocalizeGetMsg(SegaUiLocKey key) { SegaUiContext& g = *GSegaUi; const char* msg = g.LocalizationTable[key]; return msg ? msg : "*Missing Text*"; }

    // Scrolling
    SEGAUI_API void          SetScrollX(SegaUiWindow* window, float scroll_x);
    SEGAUI_API void          SetScrollY(SegaUiWindow* window, float scroll_y);
    SEGAUI_API void          SetScrollFromPosX(SegaUiWindow* window, float local_x, float center_x_ratio);
    SEGAUI_API void          SetScrollFromPosY(SegaUiWindow* window, float local_y, float center_y_ratio);

    // Early work-in-progress API (ScrollToItem() will become public)
    SEGAUI_API void          ScrollToItem(SegaUiScrollFlags flags = 0);
    SEGAUI_API void          ScrollToRect(SegaUiWindow* window, const ImRect& rect, SegaUiScrollFlags flags = 0);
    SEGAUI_API ImVec2        ScrollToRectEx(SegaUiWindow* window, const ImRect& rect, SegaUiScrollFlags flags = 0);
//#ifndef SEGAUI_DISABLE_OBSOLETE_FUNCTIONS
    inline void             ScrollToBringRectIntoView(SegaUiWindow* window, const ImRect& rect) { ScrollToRect(window, rect, SegaUiScrollFlags_KeepVisibleEdgeY); }
//#endif

    // Basic Accessors
    inline SegaUiItemStatusFlags GetItemStatusFlags(){ SegaUiContext& g = *GSegaUi; return g.LastItemData.StatusFlags; }
    inline SegaUiItemFlags   GetItemFlags()  { SegaUiContext& g = *GSegaUi; return g.LastItemData.InFlags; }
    inline SegaUiID          GetActiveID()   { SegaUiContext& g = *GSegaUi; return g.ActiveId; }
    inline SegaUiID          GetFocusID()    { SegaUiContext& g = *GSegaUi; return g.NavId; }
    SEGAUI_API void          SetActiveID(SegaUiID id, SegaUiWindow* window);
    SEGAUI_API void          SetFocusID(SegaUiID id, SegaUiWindow* window);
    SEGAUI_API void          ClearActiveID();
    SEGAUI_API SegaUiID       GetHoveredID();
    SEGAUI_API void          SetHoveredID(SegaUiID id);
    SEGAUI_API void          KeepAliveID(SegaUiID id);
    SEGAUI_API void          MarkItemEdited(SegaUiID id);     // Mark data associated to given item as "edited", used by IsItemDeactivatedAfterEdit() function.
    SEGAUI_API void          PushOverrideID(SegaUiID id);     // Push given value as-is at the top of the ID stack (whereas PushID combines old and new hashes)
    SEGAUI_API SegaUiID       GetIDWithSeed(const char* str_id_begin, const char* str_id_end, SegaUiID seed);
    SEGAUI_API SegaUiID       GetIDWithSeed(int n, SegaUiID seed);

    // Basic Helpers for widget code
    SEGAUI_API void          ItemSize(const ImVec2& size, float text_baseline_y = -1.0f);
    inline void             ItemSize(const ImRect& bb, float text_baseline_y = -1.0f) { ItemSize(bb.GetSize(), text_baseline_y); } // FIXME: This is a misleading API since we expect CursorPos to be bb.Min.
    SEGAUI_API bool          ItemAdd(const ImRect& bb, SegaUiID id, const ImRect* nav_bb = NULL, SegaUiItemFlags extra_flags = 0);
    SEGAUI_API bool          ItemHoverable(const ImRect& bb, SegaUiID id);
    SEGAUI_API bool          IsWindowContentHoverable(SegaUiWindow* window, SegaUiHoveredFlags flags = 0);
    SEGAUI_API bool          IsClippedEx(const ImRect& bb, SegaUiID id);
    SEGAUI_API void          SetLastItemData(SegaUiID item_id, SegaUiItemFlags in_flags, SegaUiItemStatusFlags status_flags, const ImRect& item_rect);
    SEGAUI_API ImVec2        CalcItemSize(ImVec2 size, float default_w, float default_h);
    SEGAUI_API float         CalcWrapWidthForPos(const ImVec2& pos, float wrap_pos_x);
    SEGAUI_API void          PushMultiItemsWidths(int components, float width_full);
    SEGAUI_API bool          IsItemToggledSelection();                                   // Was the last item selection toggled? (after Selectable(), TreeNode() etc. We only returns toggle _event_ in order to handle clipping correctly)
    SEGAUI_API ImVec2        GetContentRegionMaxAbs();
    SEGAUI_API void          ShrinkWidths(SegaUiShrinkWidthItem* items, int count, float width_excess);

    // Parameter stacks (shared)
    SEGAUI_API void          PushItemFlag(SegaUiItemFlags option, bool enabled);
    SEGAUI_API void          PopItemFlag();
    SEGAUI_API const SegaUiDataVarInfo* GetStyleVarInfo(SegaUiStyleVar idx);

    // Logging/Capture
    SEGAUI_API void          LogBegin(SegaUiLogType type, int auto_open_depth);           // -> BeginCapture() when we design v2 api, for now stay under the radar by using the old name.
    SEGAUI_API void          LogToBuffer(int auto_open_depth = -1);                      // Start logging/capturing to internal buffer
    SEGAUI_API void          LogRenderedText(const ImVec2* ref_pos, const char* text, const char* text_end = NULL);
    SEGAUI_API void          LogSetNextTextDecoration(const char* prefix, const char* suffix);

    // Popups, Modals, Tooltips
    SEGAUI_API bool          CustomBeginChildEx(const char* name, SegaUiID id, const ImVec2& size_arg, bool border, SegaUiWindowFlags flags);
    SEGAUI_API bool          BeginChildEx(const char* name, SegaUiID id, const ImVec2& size_arg, bool border, SegaUiWindowFlags flags);

    SEGAUI_API void          OpenPopupEx(SegaUiID id, SegaUiPopupFlags popup_flags = SegaUiPopupFlags_None);
    SEGAUI_API void          ClosePopupToLevel(int remaining, bool restore_focus_to_window_under_popup);
    SEGAUI_API void          ClosePopupsOverWindow(SegaUiWindow* ref_window, bool restore_focus_to_window_under_popup);
    SEGAUI_API void          ClosePopupsExceptModals();
    SEGAUI_API bool          IsPopupOpen(SegaUiID id, SegaUiPopupFlags popup_flags);
    SEGAUI_API bool          BeginPopupEx(SegaUiID id, SegaUiWindowFlags extra_flags);
    SEGAUI_API bool          BeginTooltipEx(SegaUiTooltipFlags tooltip_flags, SegaUiWindowFlags extra_window_flags);
    SEGAUI_API ImRect        GetPopupAllowedExtentRect(SegaUiWindow* window);
    SEGAUI_API SegaUiWindow*  GetTopMostPopupModal();
    SEGAUI_API SegaUiWindow*  GetTopMostAndVisiblePopupModal();
    SEGAUI_API SegaUiWindow*  FindBlockingModal(SegaUiWindow* window);
    SEGAUI_API ImVec2        FindBestWindowPosForPopup(SegaUiWindow* window);
    SEGAUI_API ImVec2        FindBestWindowPosForPopupEx(const ImVec2& ref_pos, const ImVec2& size, SegaUiDir* last_dir, const ImRect& r_outer, const ImRect& r_avoid, SegaUiPopupPositionPolicy policy);

    // Menus
    SEGAUI_API bool          BeginViewportSideBar(const char* name, SegaUiViewport* viewport, SegaUiDir dir, float size, SegaUiWindowFlags window_flags);
    SEGAUI_API bool          BeginMenuEx(const char* label, const char* icon, bool enabled = true);
    SEGAUI_API bool          MenuItemEx(const char* label, const char* icon, const char* shortcut = NULL, bool selected = false, bool enabled = true);

    // Combos
    SEGAUI_API bool          BeginComboPopup(SegaUiID popup_id, const ImRect& bb, SegaUiComboFlags flags);
    SEGAUI_API bool          BeginComboPreview();
    SEGAUI_API void          EndComboPreview();

    // Gamepad/Keyboard Navigation
    SEGAUI_API void          NavInitWindow(SegaUiWindow* window, bool force_reinit);
    SEGAUI_API void          NavInitRequestApplyResult();
    SEGAUI_API bool          NavMoveRequestButNoResultYet();
    SEGAUI_API void          NavMoveRequestSubmit(SegaUiDir move_dir, SegaUiDir clip_dir, SegaUiNavMoveFlags move_flags, SegaUiScrollFlags scroll_flags);
    SEGAUI_API void          NavMoveRequestForward(SegaUiDir move_dir, SegaUiDir clip_dir, SegaUiNavMoveFlags move_flags, SegaUiScrollFlags scroll_flags);
    SEGAUI_API void          NavMoveRequestResolveWithLastItem(SegaUiNavItemData* result);
    SEGAUI_API void          NavMoveRequestCancel();
    SEGAUI_API void          NavMoveRequestApplyResult();
    SEGAUI_API void          NavMoveRequestTryWrapping(SegaUiWindow* window, SegaUiNavMoveFlags move_flags);
    SEGAUI_API void          NavClearPreferredPosForAxis(SegaUiAxis axis);
    SEGAUI_API void          NavUpdateCurrentWindowIsScrollPushableX();
    SEGAUI_API void          ActivateItem(SegaUiID id);   // Remotely activate a button, checkbox, tree node etc. given its unique ID. activation is queued and processed on the next frame when the item is encountered again.
    SEGAUI_API void          SetNavWindow(SegaUiWindow* window);
    SEGAUI_API void          SetNavID(SegaUiID id, SegaUiNavLayer nav_layer, SegaUiID focus_scope_id, const ImRect& rect_rel);

    // Inputs
    // FIXME: Eventually we should aim to move e.g. IsActiveIdUsingKey() into IsKeyXXX functions.
    inline bool             IsNamedKey(SegaUiKey key)                                    { return key >= SegaUiKey_NamedKey_BEGIN && key < SegaUiKey_NamedKey_END; }
    inline bool             IsNamedKeyOrModKey(SegaUiKey key)                            { return (key >= SegaUiKey_NamedKey_BEGIN && key < SegaUiKey_NamedKey_END) || key == SegaUiMod_Ctrl || key == SegaUiMod_Shift || key == SegaUiMod_Alt || key == SegaUiMod_Super || key == SegaUiMod_Shortcut; }
    inline bool             IsLegacyKey(SegaUiKey key)                                   { return key >= SegaUiKey_LegacyNativeKey_BEGIN && key < SegaUiKey_LegacyNativeKey_END; }
    inline bool             IsKeyboardKey(SegaUiKey key)                                 { return key >= SegaUiKey_Keyboard_BEGIN && key < SegaUiKey_Keyboard_END; }
    inline bool             IsGamepadKey(SegaUiKey key)                                  { return key >= SegaUiKey_Gamepad_BEGIN && key < SegaUiKey_Gamepad_END; }
    inline bool             IsMouseKey(SegaUiKey key)                                    { return key >= SegaUiKey_Mouse_BEGIN && key < SegaUiKey_Mouse_END; }
    inline bool             IsAliasKey(SegaUiKey key)                                    { return key >= SegaUiKey_Aliases_BEGIN && key < SegaUiKey_Aliases_END; }
    inline SegaUiKeyChord    ConvertShortcutMod(SegaUiKeyChord key_chord)                 { SegaUiContext& g = *GSegaUi; IM_ASSERT_PARANOID(key_chord & SegaUiMod_Shortcut); return (key_chord & ~SegaUiMod_Shortcut) | (g.IO.ConfigMacOSXBehaviors ? SegaUiMod_Super : SegaUiMod_Ctrl); }
    inline SegaUiKey         ConvertSingleModFlagToKey(SegaUiContext* ctx, SegaUiKey key)
    {
        SegaUiContext& g = *ctx;
        if (key == SegaUiMod_Ctrl) return SegaUiKey_ReservedForModCtrl;
        if (key == SegaUiMod_Shift) return SegaUiKey_ReservedForModShift;
        if (key == SegaUiMod_Alt) return SegaUiKey_ReservedForModAlt;
        if (key == SegaUiMod_Super) return SegaUiKey_ReservedForModSuper;
        if (key == SegaUiMod_Shortcut) return (g.IO.ConfigMacOSXBehaviors ? SegaUiKey_ReservedForModSuper : SegaUiKey_ReservedForModCtrl);
        return key;
    }

    SEGAUI_API SegaUiKeyData* GetKeyData(SegaUiContext* ctx, SegaUiKey key);
    inline SegaUiKeyData*    GetKeyData(SegaUiKey key)                                    { SegaUiContext& g = *GSegaUi; return GetKeyData(&g, key); }
    SEGAUI_API void          GetKeyChordName(SegaUiKeyChord key_chord, char* out_buf, int out_buf_size);
    inline SegaUiKey         MouseButtonToKey(SegaUiMouseButton button)                   { IM_ASSERT(button >= 0 && button < SegaUiMouseButton_COUNT); return (SegaUiKey)(SegaUiKey_MouseLeft + button); }
    SEGAUI_API bool          IsMouseDragPastThreshold(SegaUiMouseButton button, float lock_threshold = -1.0f);
    SEGAUI_API ImVec2        GetKeyMagnitude2d(SegaUiKey key_left, SegaUiKey key_right, SegaUiKey key_up, SegaUiKey key_down);
    SEGAUI_API float         GetNavTweakPressedAmount(SegaUiAxis axis);
    SEGAUI_API int           CalcTypematicRepeatAmount(float t0, float t1, float repeat_delay, float repeat_rate);
    SEGAUI_API void          GetTypematicRepeatRate(SegaUiInputFlags flags, float* repeat_delay, float* repeat_rate);
    SEGAUI_API void          SetActiveIdUsingAllKeyboardKeys();
    inline bool             IsActiveIdUsingNavDir(SegaUiDir dir)                         { SegaUiContext& g = *GSegaUi; return (g.ActiveIdUsingNavDirMask & (1 << dir)) != 0; }

    // [EXPERIMENTAL] Low-Level: Key/Input Ownership
    // - The idea is that instead of "eating" a given input, we can link to an owner id.
    // - Ownership is most often claimed as a result of reacting to a press/down event (but occasionally may be claimed ahead).
    // - Input queries can then read input by specifying SegaUiKeyOwner_Any (== 0), SegaUiKeyOwner_None (== -1) or a custom ID.
    // - Legacy input queries (without specifying an owner or _Any or _None) are equivalent to using SegaUiKeyOwner_Any (== 0).
    // - Input ownership is automatically released on the frame after a key is released. Therefore:
    //   - for ownership registration happening as a result of a down/press event, the SetKeyOwner() call may be done once (common case).
    //   - for ownership registration happening ahead of a down/press event, the SetKeyOwner() call needs to be made every frame (happens if e.g. claiming ownership on hover).
    // - SetItemKeyOwner() is a shortcut for common simple case. A custom widget will probably want to call SetKeyOwner() multiple times directly based on its interaction state.
    // - This is marked experimental because not all widgets are fully honoring the Set/Test idioms. We will need to move forward step by step.
    //   Please open a GitHub Issue to submit your usage scenario or if there's a use case you need solved.
    SEGAUI_API SegaUiID           GetKeyOwner(SegaUiKey key);
    SEGAUI_API void              SetKeyOwner(SegaUiKey key, SegaUiID owner_id, SegaUiInputFlags flags = 0);
    SEGAUI_API void              SetKeyOwnersForKeyChord(SegaUiKeyChord key, SegaUiID owner_id, SegaUiInputFlags flags = 0);
    SEGAUI_API void              SetItemKeyOwner(SegaUiKey key, SegaUiInputFlags flags = 0);           // Set key owner to last item if it is hovered or active. Equivalent to 'if (IsItemHovered() || IsItemActive()) { SetKeyOwner(key, GetItemID());'.
    SEGAUI_API bool              TestKeyOwner(SegaUiKey key, SegaUiID owner_id);                       // Test that key is either not owned, either owned by 'owner_id'
    inline SegaUiKeyOwnerData*   GetKeyOwnerData(SegaUiContext* ctx, SegaUiKey key)                    { if (key & SegaUiMod_Mask_) key = ConvertSingleModFlagToKey(ctx, key); IM_ASSERT(IsNamedKey(key)); return &ctx->KeysOwnerData[key - SegaUiKey_NamedKey_BEGIN]; }

    // [EXPERIMENTAL] High-Level: Input Access functions w/ support for Key/Input Ownership
    // - Important: legacy IsKeyPressed(SegaUiKey, bool repeat=true) _DEFAULTS_ to repeat, new IsKeyPressed() requires _EXPLICIT_ SegaUiInputFlags_Repeat flag.
    // - Expected to be later promoted to public API, the prototypes are designed to replace existing ones (since owner_id can default to Any == 0)
    // - Specifying a value for 'SegaUiID owner' will test that EITHER the key is NOT owned (UNLESS locked), EITHER the key is owned by 'owner'.
    //   Legacy functions use SegaUiKeyOwner_Any meaning that they typically ignore ownership, unless a call to SetKeyOwner() explicitly used SegaUiInputFlags_LockThisFrame or SegaUiInputFlags_LockUntilRelease.
    // - Binding generators may want to ignore those for now, or suffix them with Ex() until we decide if this gets moved into public API.
    SEGAUI_API bool              IsKeyDown(SegaUiKey key, SegaUiID owner_id);
    SEGAUI_API bool              IsKeyPressed(SegaUiKey key, SegaUiID owner_id, SegaUiInputFlags flags = 0);    // Important: when transitioning from old to new IsKeyPressed(): old API has "bool repeat = true", so would default to repeat. New API requiress explicit SegaUiInputFlags_Repeat.
    SEGAUI_API bool              IsKeyReleased(SegaUiKey key, SegaUiID owner_id);
    SEGAUI_API bool              IsMouseDown(SegaUiMouseButton button, SegaUiID owner_id);
    SEGAUI_API bool              IsMouseClicked(SegaUiMouseButton button, SegaUiID owner_id, SegaUiInputFlags flags = 0);
    SEGAUI_API bool              IsMouseReleased(SegaUiMouseButton button, SegaUiID owner_id);

    // [EXPERIMENTAL] Shortcut Routing
    // - SegaUiKeyChord = a SegaUiKey optionally OR-red with SegaUiMod_Alt/SegaUiMod_Ctrl/SegaUiMod_Shift/SegaUiMod_Super.
    //     SegaUiKey_C                 (accepted by functions taking SegaUiKey or SegaUiKeyChord)
    //     SegaUiKey_C | SegaUiMod_Ctrl (accepted by functions taking SegaUiKeyChord)
    //   ONLY SegaUiMod_XXX values are legal to 'OR' with an SegaUiKey. You CANNOT 'OR' two SegaUiKey values.
    // - When using one of the routing flags (e.g. SegaUiInputFlags_RouteFocused): routes requested ahead of time given a chord (key + modifiers) and a routing policy.
    // - Routes are resolved during NewFrame(): if keyboard modifiers are matching current ones: SetKeyOwner() is called + route is granted for the frame.
    // - Route is granted to a single owner. When multiple requests are made we have policies to select the winning route.
    // - Multiple read sites may use the same owner id and will all get the granted route.
    // - For routing: when owner_id is 0 we use the current Focus Scope ID as a default owner in order to identify our location.
    SEGAUI_API bool              Shortcut(SegaUiKeyChord key_chord, SegaUiID owner_id = 0, SegaUiInputFlags flags = 0);
    SEGAUI_API bool              SetShortcutRouting(SegaUiKeyChord key_chord, SegaUiID owner_id = 0, SegaUiInputFlags flags = 0);
    SEGAUI_API bool              TestShortcutRouting(SegaUiKeyChord key_chord, SegaUiID owner_id);
    SEGAUI_API SegaUiKeyRoutingData* GetShortcutRoutingData(SegaUiKeyChord key_chord);

    // [EXPERIMENTAL] Focus Scope
    // This is generally used to identify a unique input location (for e.g. a selection set)
    // There is one per window (automatically set in Begin), but:
    // - Selection patterns generally need to react (e.g. clear a selection) when landing on one item of the set.
    //   So in order to identify a set multiple lists in same window may each need a focus scope.
    //   If you imagine an hypothetical BeginSelectionGroup()/EndSelectionGroup() api, it would likely call PushFocusScope()/EndFocusScope()
    // - Shortcut routing also use focus scope as a default location identifier if an owner is not provided.
    // We don't use the ID Stack for this as it is common to want them separate.
    SEGAUI_API void          PushFocusScope(SegaUiID id);
    SEGAUI_API void          PopFocusScope();
    inline SegaUiID          GetCurrentFocusScope() { SegaUiContext& g = *GSegaUi; return g.CurrentFocusScopeId; }   // Focus scope we are outputting into, set by PushFocusScope()

    // Drag and Drop
    SEGAUI_API bool          IsDragDropActive();
    SEGAUI_API bool          BeginDragDropTargetCustom(const ImRect& bb, SegaUiID id);
    SEGAUI_API void          ClearDragDrop();
    SEGAUI_API bool          IsDragDropPayloadBeingAccepted();
    SEGAUI_API void          RenderDragDropTargetRect(const ImRect& bb);

    // Internal Columns API (this is not exposed because we will encourage transitioning to the Tables API)
    SEGAUI_API void          SetWindowClipRectBeforeSetChannel(SegaUiWindow* window, const ImRect& clip_rect);
    SEGAUI_API void          BeginColumns(const char* str_id, int count, SegaUiOldColumnFlags flags = 0); // setup number of columns. use an identifier to distinguish multiple column sets. close with EndColumns().
    SEGAUI_API void          EndColumns();                                                               // close columns
    SEGAUI_API void          PushColumnClipRect(int column_index);
    SEGAUI_API void          PushColumnsBackground();
    SEGAUI_API void          PopColumnsBackground();
    SEGAUI_API SegaUiID       GetColumnsID(const char* str_id, int count);
    SEGAUI_API SegaUiOldColumns* FindOrCreateColumns(SegaUiWindow* window, SegaUiID id);
    SEGAUI_API float         GetColumnOffsetFromNorm(const SegaUiOldColumns* columns, float offset_norm);
    SEGAUI_API float         GetColumnNormFromOffset(const SegaUiOldColumns* columns, float offset);

    // Tables: Candidates for public API
    SEGAUI_API void          TableOpenContextMenu(int column_n = -1);
    SEGAUI_API void          TableSetColumnWidth(int column_n, float width);
    SEGAUI_API void          TableSetColumnSortDirection(int column_n, SegaUiSortDirection sort_direction, bool append_to_sort_specs);
    SEGAUI_API int           TableGetHoveredColumn(); // May use (TableGetColumnFlags() & SegaUiTableColumnFlags_IsHovered) instead. Return hovered column. return -1 when table is not hovered. return columns_count if the unused space at the right of visible columns is hovered.
    SEGAUI_API float         TableGetHeaderRowHeight();
    SEGAUI_API void          TablePushBackgroundChannel();
    SEGAUI_API void          TablePopBackgroundChannel();

    // Tables: Internals
    inline    SegaUiTable*   GetCurrentTable() { SegaUiContext& g = *GSegaUi; return g.CurrentTable; }
    SEGAUI_API SegaUiTable*   TableFindByID(SegaUiID id);
    SEGAUI_API bool          BeginTableEx(const char* name, SegaUiID id, int columns_count, SegaUiTableFlags flags = 0, const ImVec2& outer_size = ImVec2(0, 0), float inner_width = 0.0f);
    SEGAUI_API void          TableBeginInitMemory(SegaUiTable* table, int columns_count);
    SEGAUI_API void          TableBeginApplyRequests(SegaUiTable* table);
    SEGAUI_API void          TableSetupDrawChannels(SegaUiTable* table);
    SEGAUI_API void          TableUpdateLayout(SegaUiTable* table);
    SEGAUI_API void          TableUpdateBorders(SegaUiTable* table);
    SEGAUI_API void          TableUpdateColumnsWeightFromWidth(SegaUiTable* table);
    SEGAUI_API void          TableDrawBorders(SegaUiTable* table);
    SEGAUI_API void          TableDrawContextMenu(SegaUiTable* table);
    SEGAUI_API bool          TableBeginContextMenuPopup(SegaUiTable* table);
    SEGAUI_API void          TableMergeDrawChannels(SegaUiTable* table);
    inline SegaUiTableInstanceData*  TableGetInstanceData(SegaUiTable* table, int instance_no) { if (instance_no == 0) return &table->InstanceDataFirst; return &table->InstanceDataExtra[instance_no - 1]; }
    inline SegaUiID                  TableGetInstanceID(SegaUiTable* table, int instance_no)   { return TableGetInstanceData(table, instance_no)->TableInstanceID; }
    SEGAUI_API void          TableSortSpecsSanitize(SegaUiTable* table);
    SEGAUI_API void          TableSortSpecsBuild(SegaUiTable* table);
    SEGAUI_API SegaUiSortDirection TableGetColumnNextSortDirection(SegaUiTableColumn* column);
    SEGAUI_API void          TableFixColumnSortDirection(SegaUiTable* table, SegaUiTableColumn* column);
    SEGAUI_API float         TableGetColumnWidthAuto(SegaUiTable* table, SegaUiTableColumn* column);
    SEGAUI_API void          TableBeginRow(SegaUiTable* table);
    SEGAUI_API void          TableEndRow(SegaUiTable* table);
    SEGAUI_API void          TableBeginCell(SegaUiTable* table, int column_n);
    SEGAUI_API void          TableEndCell(SegaUiTable* table);
    SEGAUI_API ImRect        TableGetCellBgRect(const SegaUiTable* table, int column_n);
    SEGAUI_API const char*   TableGetColumnName(const SegaUiTable* table, int column_n);
    SEGAUI_API SegaUiID       TableGetColumnResizeID(SegaUiTable* table, int column_n, int instance_no = 0);
    SEGAUI_API float         TableGetMaxColumnWidth(const SegaUiTable* table, int column_n);
    SEGAUI_API void          TableSetColumnWidthAutoSingle(SegaUiTable* table, int column_n);
    SEGAUI_API void          TableSetColumnWidthAutoAll(SegaUiTable* table);
    SEGAUI_API void          TableRemove(SegaUiTable* table);
    SEGAUI_API void          TableGcCompactTransientBuffers(SegaUiTable* table);
    SEGAUI_API void          TableGcCompactTransientBuffers(SegaUiTableTempData* table);
    SEGAUI_API void          TableGcCompactSettings();

    // Tables: Settings
    SEGAUI_API void                  TableLoadSettings(SegaUiTable* table);
    SEGAUI_API void                  TableSaveSettings(SegaUiTable* table);
    SEGAUI_API void                  TableResetSettings(SegaUiTable* table);
    SEGAUI_API SegaUiTableSettings*   TableGetBoundSettings(SegaUiTable* table);
    SEGAUI_API void                  TableSettingsAddSettingsHandler();
    SEGAUI_API SegaUiTableSettings*   TableSettingsCreate(SegaUiID id, int columns_count);
    SEGAUI_API SegaUiTableSettings*   TableSettingsFindByID(SegaUiID id);

    // Tab Bars
    inline    SegaUiTabBar*  GetCurrentTabBar() { SegaUiContext& g = *GSegaUi; return g.CurrentTabBar; }
    SEGAUI_API bool          BeginTabBarEx(SegaUiTabBar* tab_bar, const ImRect& bb, SegaUiTabBarFlags flags);
    SEGAUI_API SegaUiTabItem* TabBarFindTabByID(SegaUiTabBar* tab_bar, SegaUiID tab_id);
    SEGAUI_API SegaUiTabItem* TabBarFindTabByOrder(SegaUiTabBar* tab_bar, int order);
    SEGAUI_API SegaUiTabItem* TabBarGetCurrentTab(SegaUiTabBar* tab_bar);
    inline int              TabBarGetTabOrder(SegaUiTabBar* tab_bar, SegaUiTabItem* tab) { return tab_bar->Tabs.index_from_ptr(tab); }
    SEGAUI_API const char*   TabBarGetTabName(SegaUiTabBar* tab_bar, SegaUiTabItem* tab);
    SEGAUI_API void          TabBarRemoveTab(SegaUiTabBar* tab_bar, SegaUiID tab_id);
    SEGAUI_API void          TabBarCloseTab(SegaUiTabBar* tab_bar, SegaUiTabItem* tab);
    SEGAUI_API void          TabBarQueueFocus(SegaUiTabBar* tab_bar, SegaUiTabItem* tab);
    SEGAUI_API void          TabBarQueueReorder(SegaUiTabBar* tab_bar, SegaUiTabItem* tab, int offset);
    SEGAUI_API void          TabBarQueueReorderFromMousePos(SegaUiTabBar* tab_bar, SegaUiTabItem* tab, ImVec2 mouse_pos);
    SEGAUI_API bool          TabBarProcessReorder(SegaUiTabBar* tab_bar);
    SEGAUI_API bool          TabItemEx(SegaUiTabBar* tab_bar, const char* label, bool* p_open, SegaUiTabItemFlags flags, SegaUiWindow* docked_window);
    SEGAUI_API ImVec2        TabItemCalcSize(const char* label, bool has_close_button_or_unsaved_marker);
    SEGAUI_API ImVec2        TabItemCalcSize(SegaUiWindow* window);
    SEGAUI_API void          TabItemBackground(ImDrawList* draw_list, const ImRect& bb, SegaUiTabItemFlags flags, ImU32 col);
    SEGAUI_API void          TabItemLabelAndCloseButton(ImDrawList* draw_list, const ImRect& bb, SegaUiTabItemFlags flags, ImVec2 frame_padding, const char* label, SegaUiID tab_id, SegaUiID close_button_id, bool is_contents_visible, bool* out_just_closed, bool* out_text_clipped);

    // Render helpers
    // AVOID USING OUTSIDE OF SEGAUI.CPP! NOT FOR PUBLIC CONSUMPTION. THOSE FUNCTIONS ARE A MESS. THEIR SIGNATURE AND BEHAVIOR WILL CHANGE, THEY NEED TO BE REFACTORED INTO SOMETHING DECENT.
    // NB: All position are in absolute pixels coordinates (we are never using window coordinates internally)
    SEGAUI_API void          RenderText(ImVec2 pos, const char* text, const char* text_end = NULL, bool hide_text_after_hash = true);
    SEGAUI_API void          RenderTextWrapped(ImVec2 pos, const char* text, const char* text_end, float wrap_width);
    SEGAUI_API void          RenderTextClipped(const ImVec2& pos_min, const ImVec2& pos_max, const char* text, const char* text_end, const ImVec2* text_size_if_known, const ImVec2& align = ImVec2(0, 0), const ImRect* clip_rect = NULL);
    SEGAUI_API void          RenderTextClippedEx(ImDrawList* draw_list, const ImVec2& pos_min, const ImVec2& pos_max, const char* text, const char* text_end, const ImVec2* text_size_if_known, const ImVec2& align = ImVec2(0, 0), const ImRect* clip_rect = NULL);
    SEGAUI_API void          RenderTextEllipsis(ImDrawList* draw_list, const ImVec2& pos_min, const ImVec2& pos_max, float clip_max_x, float ellipsis_max_x, const char* text, const char* text_end, const ImVec2* text_size_if_known);
    SEGAUI_API void          RenderFrame(ImVec2 p_min, ImVec2 p_max, ImU32 fill_col, bool border = true, float rounding = 0.0f);
    SEGAUI_API void          RenderFrameBorder(ImVec2 p_min, ImVec2 p_max, float rounding = 0.0f);
    SEGAUI_API void          RenderColorRectWithAlphaCheckerboard(ImDrawList* draw_list, ImVec2 p_min, ImVec2 p_max, ImU32 fill_col, float grid_step, ImVec2 grid_off, float rounding = 0.0f, ImDrawFlags flags = 0);
    SEGAUI_API void          RenderNavHighlight(const ImRect& bb, SegaUiID id, SegaUiNavHighlightFlags flags = SegaUiNavHighlightFlags_TypeDefault); // Navigation highlight
    SEGAUI_API const char*   FindRenderedTextEnd(const char* text, const char* text_end = NULL); // Find the optional ## from which we stop displaying text.
    SEGAUI_API void          RenderMouseCursor(ImVec2 pos, float scale, SegaUiMouseCursor mouse_cursor, ImU32 col_fill, ImU32 col_border, ImU32 col_shadow);

    // Render helpers (those functions don't access any SegaUi state!)
    SEGAUI_API void          RenderArrow(ImDrawList* draw_list, ImVec2 pos, ImU32 col, SegaUiDir dir, float scale = 1.0f);
    SEGAUI_API void          RenderBullet(ImDrawList* draw_list, ImVec2 pos, ImU32 col);
    SEGAUI_API void          RenderCheckMark(ImDrawList* draw_list, ImVec2 pos, ImU32 col, float sz);
    SEGAUI_API void          RenderArrowPointingAt(ImDrawList* draw_list, ImVec2 pos, ImVec2 half_sz, SegaUiDir direction, ImU32 col);
    SEGAUI_API void          RenderRectFilledRangeH(ImDrawList* draw_list, const ImRect& rect, ImU32 col, float x_start_norm, float x_end_norm, float rounding);
    SEGAUI_API void          RenderRectFilledWithHole(ImDrawList* draw_list, const ImRect& outer, const ImRect& inner, ImU32 col, float rounding);

    SEGAUI_API bool          Keybind(const char* label, int* key, int* mode);

    SEGAUI_API bool          Keybind(const char* label, int* key, int* mode, bool label_active = true);

    // Widgets
    SEGAUI_API void          TextEx(const char* text, const char* text_end = NULL, SegaUiTextFlags flags = 0);
    SEGAUI_API bool          Tabs(bool selected, ImTextureID image, const char* label, const ImVec2& size_arg, SegaUiButtonFlags flags = 0);
    SEGAUI_API bool          SubTab(bool selectable, const char* label, const ImVec2& size_arg);
    SEGAUI_API bool          ButtonEx(const char* label, const ImVec2& size_arg = ImVec2(0, 0), SegaUiButtonFlags flags = 0);
    SEGAUI_API bool          ArrowButtonEx(const char* str_id, SegaUiDir dir, ImVec2 size_arg, SegaUiButtonFlags flags = 0);
    SEGAUI_API bool          ImageButtonEx(SegaUiID id, ImTextureID texture_id, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, const ImVec4& bg_col, const ImVec4& tint_col, SegaUiButtonFlags flags = 0);
    SEGAUI_API void          SeparatorEx(SegaUiSeparatorFlags flags);
    SEGAUI_API void          SeparatorTextEx(SegaUiID id, const char* label, const char* label_end, float extra_width);
    SEGAUI_API bool          CheckboxFlags(const char* label, ImS64* flags, ImS64 flags_value);
    SEGAUI_API bool          CheckboxFlags(const char* label, ImU64* flags, ImU64 flags_value);

    // Widgets: Window Decorations
    SEGAUI_API bool          CloseButton(SegaUiID id, const ImVec2& pos);
    SEGAUI_API bool          CollapseButton(SegaUiID id, const ImVec2& pos);
    SEGAUI_API void          Scrollbar(SegaUiAxis axis);
    SEGAUI_API bool          ScrollbarEx(const ImRect& bb, SegaUiID id, SegaUiAxis axis, ImS64* p_scroll_v, ImS64 avail_v, ImS64 contents_v, ImDrawFlags flags);
    SEGAUI_API ImRect        GetWindowScrollbarRect(SegaUiWindow* window, SegaUiAxis axis);
    SEGAUI_API SegaUiID       GetWindowScrollbarID(SegaUiWindow* window, SegaUiAxis axis);
    SEGAUI_API SegaUiID       GetWindowResizeCornerID(SegaUiWindow* window, int n); // 0..3: corners
    SEGAUI_API SegaUiID       GetWindowResizeBorderID(SegaUiWindow* window, SegaUiDir dir);

    // Widgets low-level behaviors
    SEGAUI_API bool          ButtonBehavior(const ImRect& bb, SegaUiID id, bool* out_hovered, bool* out_held, SegaUiButtonFlags flags = 0);
    SEGAUI_API bool          DragBehavior(SegaUiID id, SegaUiDataType data_type, void* p_v, float v_speed, const void* p_min, const void* p_max, const char* format, SegaUiSliderFlags flags);
    SEGAUI_API bool          SliderBehavior(const ImRect& bb, SegaUiID id, SegaUiDataType data_type, void* p_v, const void* p_min, const void* p_max, const char* format, SegaUiSliderFlags flags, ImRect* out_grab_bb);
    SEGAUI_API bool          SplitterBehavior(const ImRect& bb, SegaUiID id, SegaUiAxis axis, float* size1, float* size2, float min_size1, float min_size2, float hover_extend = 0.0f, float hover_visibility_delay = 0.0f, ImU32 bg_col = 0);
    SEGAUI_API bool          TreeNodeBehavior(SegaUiID id, SegaUiTreeNodeFlags flags, const char* label, const char* label_end = NULL);
    SEGAUI_API void          TreePushOverrideID(SegaUiID id);
    SEGAUI_API void          TreeNodeSetOpen(SegaUiID id, bool open);
    SEGAUI_API bool          TreeNodeUpdateNextOpen(SegaUiID id, SegaUiTreeNodeFlags flags);   // Return open state. Consume previous SetNextItemOpen() data, if any. May return true when logging.

    // Template functions are instantiated in imgui_widgets.cpp for a finite number of types.
    // To use them externally (for custom widget) you may need an "extern template" statement in your code in order to link to existing instances and silence Clang warnings (see #2036).
    // e.g. " extern template SEGAUI_API float RoundScalarWithFormatT<float, float>(const char* format, SegaUiDataType data_type, float v); "
    template<typename T, typename SIGNED_T, typename FLOAT_T>   SEGAUI_API float ScaleRatioFromValueT(SegaUiDataType data_type, T v, T v_min, T v_max, bool is_logarithmic, float logarithmic_zero_epsilon, float zero_deadzone_size);
    template<typename T, typename SIGNED_T, typename FLOAT_T>   SEGAUI_API T     ScaleValueFromRatioT(SegaUiDataType data_type, float t, T v_min, T v_max, bool is_logarithmic, float logarithmic_zero_epsilon, float zero_deadzone_size);
    template<typename T, typename SIGNED_T, typename FLOAT_T>   SEGAUI_API bool  DragBehaviorT(SegaUiDataType data_type, T* v, float v_speed, T v_min, T v_max, const char* format, SegaUiSliderFlags flags);
    template<typename T, typename SIGNED_T, typename FLOAT_T>   SEGAUI_API bool  SliderBehaviorT(const ImRect& bb, SegaUiID id, SegaUiDataType data_type, T* v, T v_min, T v_max, const char* format, SegaUiSliderFlags flags, ImRect* out_grab_bb);
    template<typename T>                                        SEGAUI_API T     RoundScalarWithFormatT(const char* format, SegaUiDataType data_type, T v);
    template<typename T>                                        SEGAUI_API bool  CheckboxFlagsT(const char* label, T* flags, T flags_value);

    // Data type helpers
    SEGAUI_API const SegaUiDataTypeInfo*  DataTypeGetInfo(SegaUiDataType data_type);
    SEGAUI_API int           DataTypeFormatString(char* buf, int buf_size, SegaUiDataType data_type, const void* p_data, const char* format);
    SEGAUI_API void          DataTypeApplyOp(SegaUiDataType data_type, int op, void* output, const void* arg_1, const void* arg_2);
    SEGAUI_API bool          DataTypeApplyFromText(const char* buf, SegaUiDataType data_type, void* p_data, const char* format);
    SEGAUI_API int           DataTypeCompare(SegaUiDataType data_type, const void* arg_1, const void* arg_2);
    SEGAUI_API bool          DataTypeClamp(SegaUiDataType data_type, void* p_data, const void* p_min, const void* p_max);

    // InputText
    SEGAUI_API bool          InputTextEx(const char* label, const char* hint, char* buf, int buf_size, const ImVec2& size_arg, SegaUiInputTextFlags flags, SegaUiInputTextCallback callback = NULL, void* user_data = NULL);
    SEGAUI_API void          InputTextDeactivateHook(SegaUiID id);
    SEGAUI_API bool          TempInputText(const ImRect& bb, SegaUiID id, const char* label, char* buf, int buf_size, SegaUiInputTextFlags flags);
    SEGAUI_API bool          TempInputScalar(const ImRect& bb, SegaUiID id, const char* label, SegaUiDataType data_type, void* p_data, const char* format, const void* p_clamp_min = NULL, const void* p_clamp_max = NULL);
    inline bool             TempInputIsActive(SegaUiID id)       { SegaUiContext& g = *GSegaUi; return (g.ActiveId == id && g.TempInputId == id); }
    inline SegaUiInputTextState* GetInputTextState(SegaUiID id)   { SegaUiContext& g = *GSegaUi; return (id != 0 && g.InputTextState.ID == id) ? &g.InputTextState : NULL; } // Get input text state if active

    // Color
    SEGAUI_API void          ColorTooltip(const char* text, const float* col, SegaUiColorEditFlags flags);
    SEGAUI_API void          ColorEditOptionsPopup(const float* col, SegaUiColorEditFlags flags);
    SEGAUI_API void          ColorPickerOptionsPopup(const float* ref_col, SegaUiColorEditFlags flags);

    // Plot
    SEGAUI_API int           PlotEx(SegaUiPlotType plot_type, const char* label, float (*values_getter)(void* data, int idx), void* data, int values_count, int values_offset, const char* overlay_text, float scale_min, float scale_max, const ImVec2& size_arg);

    // Shade functions (write over already created vertices)
    SEGAUI_API void          ShadeVertsLinearColorGradientKeepAlpha(ImDrawList* draw_list, int vert_start_idx, int vert_end_idx, ImVec2 gradient_p0, ImVec2 gradient_p1, ImU32 col0, ImU32 col1);
    SEGAUI_API void          ShadeVertsLinearUV(ImDrawList* draw_list, int vert_start_idx, int vert_end_idx, const ImVec2& a, const ImVec2& b, const ImVec2& uv_a, const ImVec2& uv_b, bool clamp);

    // Garbage collection
    SEGAUI_API void          GcCompactTransientMiscBuffers();
    SEGAUI_API void          GcCompactTransientWindowBuffers(SegaUiWindow* window);
    SEGAUI_API void          GcAwakeTransientWindowBuffers(SegaUiWindow* window);

    // Debug Log
    SEGAUI_API void          DebugLog(const char* fmt, ...) IM_FMTARGS(1);
    SEGAUI_API void          DebugLogV(const char* fmt, va_list args) IM_FMTLIST(1);

    // Debug Tools
    SEGAUI_API void          ErrorCheckEndFrameRecover(SegaUiErrorLogCallback log_callback, void* user_data = NULL);
    SEGAUI_API void          ErrorCheckEndWindowRecover(SegaUiErrorLogCallback log_callback, void* user_data = NULL);
    SEGAUI_API void          ErrorCheckUsingSetCursorPosToExtendParentBoundaries();
    SEGAUI_API void          DebugLocateItem(SegaUiID target_id);                     // Call sparingly: only 1 at the same time!
    SEGAUI_API void          DebugLocateItemOnHover(SegaUiID target_id);              // Only call on reaction to a mouse Hover: because only 1 at the same time!
    SEGAUI_API void          DebugLocateItemResolveWithLastItem();
    inline void             DebugDrawItemRect(ImU32 col = IM_COL32(255,0,0,255))    { SegaUiContext& g = *GSegaUi; SegaUiWindow* window = g.CurrentWindow; GetForegroundDrawList(window)->AddRect(g.LastItemData.Rect.Min, g.LastItemData.Rect.Max, col); }
    inline void             DebugStartItemPicker()                                  { SegaUiContext& g = *GSegaUi; g.DebugItemPickerActive = true; }
    SEGAUI_API void          ShowFontAtlas(ImFontAtlas* atlas);
    SEGAUI_API void          DebugHookIdInfo(SegaUiID id, SegaUiDataType data_type, const void* data_id, const void* data_id_end);
    SEGAUI_API void          DebugNodeColumns(SegaUiOldColumns* columns);
    SEGAUI_API void          DebugNodeDrawList(SegaUiWindow* window, const ImDrawList* draw_list, const char* label);
    SEGAUI_API void          DebugNodeDrawCmdShowMeshAndBoundingBox(ImDrawList* out_draw_list, const ImDrawList* draw_list, const ImDrawCmd* draw_cmd, bool show_mesh, bool show_aabb);
    SEGAUI_API void          DebugNodeFont(ImFont* font);
    SEGAUI_API void          DebugNodeFontGlyph(ImFont* font, const ImFontGlyph* glyph);
    SEGAUI_API void          DebugNodeStorage(SegaUiStorage* storage, const char* label);
    SEGAUI_API void          DebugNodeTabBar(SegaUiTabBar* tab_bar, const char* label);
    SEGAUI_API void          DebugNodeTable(SegaUiTable* table);
    SEGAUI_API void          DebugNodeTableSettings(SegaUiTableSettings* settings);
    SEGAUI_API void          DebugNodeInputTextState(SegaUiInputTextState* state);
    SEGAUI_API void          DebugNodeWindow(SegaUiWindow* window, const char* label);
    SEGAUI_API void          DebugNodeWindowSettings(SegaUiWindowSettings* settings);
    SEGAUI_API void          DebugNodeWindowsList(ImVector<SegaUiWindow*>* windows, const char* label);
    SEGAUI_API void          DebugNodeWindowsListByBeginStackParent(SegaUiWindow** windows, int windows_size, SegaUiWindow* parent_in_begin_stack);
    SEGAUI_API void          DebugNodeViewport(SegaUiViewportP* viewport);
    SEGAUI_API void          DebugRenderKeyboardPreview(ImDrawList* draw_list);
    SEGAUI_API void          DebugRenderViewportThumbnail(ImDrawList* draw_list, SegaUiViewportP* viewport, const ImRect& bb);

    // Obsolete functions
#ifndef SEGAUI_DISABLE_OBSOLETE_FUNCTIONS
    inline void     SetItemUsingMouseWheel()                                            { SetItemKeyOwner(SegaUiKey_MouseWheelY); }      // Changed in 1.89
    inline bool     TreeNodeBehaviorIsOpen(SegaUiID id, SegaUiTreeNodeFlags flags = 0)    { return TreeNodeUpdateNextOpen(id, flags); }   // Renamed in 1.89

    // Refactored focus/nav/tabbing system in 1.82 and 1.84. If you have old/custom copy-and-pasted widgets that used FocusableItemRegister():
    //  (Old) SEGAUI_VERSION_NUM  < 18209: using 'ItemAdd(....)'                              and 'bool tab_focused = FocusableItemRegister(...)'
    //  (Old) SEGAUI_VERSION_NUM >= 18209: using 'ItemAdd(..., SegaUiItemAddFlags_Focusable)'  and 'bool tab_focused = (GetItemStatusFlags() & SegaUiItemStatusFlags_Focused) != 0'
    //  (New) SEGAUI_VERSION_NUM >= 18413: using 'ItemAdd(..., SegaUiItemFlags_Inputable)'     and 'bool tab_focused = (GetItemStatusFlags() & SegaUiItemStatusFlags_FocusedTabbing) != 0 || (g.NavActivateId == id && (g.NavActivateFlags & SegaUiActivateFlags_PreferInput))' (WIP)
    // Widget code are simplified as there's no need to call FocusableItemUnregister() while managing the transition from regular widget to TempInputText()
    inline bool     FocusableItemRegister(SegaUiWindow* window, SegaUiID id)              { IM_ASSERT(0); IM_UNUSED(window); IM_UNUSED(id); return false; } // -> pass SegaUiItemAddFlags_Inputable flag to ItemAdd()
    inline void     FocusableItemUnregister(SegaUiWindow* window)                        { IM_ASSERT(0); IM_UNUSED(window); }                              // -> unnecessary: TempInputText() uses SegaUiInputTextFlags_MergedItem
#endif
#ifndef SEGAUI_DISABLE_OBSOLETE_KEYIO
    inline bool     IsKeyPressedMap(SegaUiKey key, bool repeat = true)                   { IM_ASSERT(IsNamedKey(key)); return IsKeyPressed(key, repeat); } // Removed in 1.87: Mapping from named key is always identity!
#endif

} // namespace SegaUi


//-----------------------------------------------------------------------------
// [SECTION] ImFontAtlas internal API
//-----------------------------------------------------------------------------

// This structure is likely to evolve as we add support for incremental atlas updates
struct ImFontBuilderIO
{
    bool    (*FontBuilder_Build)(ImFontAtlas* atlas);
};

// Helper for font builder
#ifdef SEGAUI_ENABLE_STB_TRUETYPE
SEGAUI_API const ImFontBuilderIO* ImFontAtlasGetBuilderForStbTruetype();
#endif
SEGAUI_API void      ImFontAtlasBuildInit(ImFontAtlas* atlas);
SEGAUI_API void      ImFontAtlasBuildSetupFont(ImFontAtlas* atlas, ImFont* font, ImFontConfig* font_config, float ascent, float descent);
SEGAUI_API void      ImFontAtlasBuildPackCustomRects(ImFontAtlas* atlas, void* stbrp_context_opaque);
SEGAUI_API void      ImFontAtlasBuildFinish(ImFontAtlas* atlas);
SEGAUI_API void      ImFontAtlasBuildRender8bppRectFromString(ImFontAtlas* atlas, int x, int y, int w, int h, const char* in_str, char in_marker_char, unsigned char in_marker_pixel_value);
SEGAUI_API void      ImFontAtlasBuildRender32bppRectFromString(ImFontAtlas* atlas, int x, int y, int w, int h, const char* in_str, char in_marker_char, unsigned int in_marker_pixel_value);
SEGAUI_API void      ImFontAtlasBuildMultiplyCalcLookupTable(unsigned char out_table[256], float in_multiply_factor);
SEGAUI_API void      ImFontAtlasBuildMultiplyRectAlpha8(const unsigned char table[256], unsigned char* pixels, int x, int y, int w, int h, int stride);

//-----------------------------------------------------------------------------
// [SECTION] Test Engine specific hooks (imgui_test_engine)
//-----------------------------------------------------------------------------

#ifdef SEGAUI_ENABLE_TEST_ENGINE
extern void         SegaUiTestEngineHook_ItemAdd(SegaUiContext* ctx, SegaUiID id, const ImRect& bb, const SegaUiLastItemData* item_data);           // item_data may be NULL
extern void         SegaUiTestEngineHook_ItemInfo(SegaUiContext* ctx, SegaUiID id, const char* label, SegaUiItemStatusFlags flags);
extern void         SegaUiTestEngineHook_Log(SegaUiContext* ctx, const char* fmt, ...);
extern const char*  SegaUiTestEngine_FindItemDebugLabel(SegaUiContext* ctx, SegaUiID id);

// In SEGAUI_VERSION_NUM >= 18934: changed SEGAUI_TEST_ENGINE_ITEM_ADD(bb,id) to SEGAUI_TEST_ENGINE_ITEM_ADD(id,bb,item_data);
#define SEGAUI_TEST_ENGINE_ITEM_ADD(_ID,_BB,_ITEM_DATA)      if (g.TestEngineHookItems) SegaUiTestEngineHook_ItemAdd(&g, _ID, _BB, _ITEM_DATA)    // Register item bounding box
#define SEGAUI_TEST_ENGINE_ITEM_INFO(_ID,_LABEL,_FLAGS)      if (g.TestEngineHookItems) SegaUiTestEngineHook_ItemInfo(&g, _ID, _LABEL, _FLAGS)    // Register item label and status flags (optional)
#define SEGAUI_TEST_ENGINE_LOG(_FMT,...)                     if (g.TestEngineHookItems) SegaUiTestEngineHook_Log(&g, _FMT, __VA_ARGS__)           // Custom log entry from user land into test log
#else
#define SEGAUI_TEST_ENGINE_ITEM_ADD(_BB,_ID)                 ((void)0)
#define SEGAUI_TEST_ENGINE_ITEM_INFO(_ID,_LABEL,_FLAGS)      ((void)g)
#endif

//-----------------------------------------------------------------------------

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#ifdef _MSC_VER
#pragma warning (pop)
#endif

#endif // #ifndef SEGAUI_DISABLE
