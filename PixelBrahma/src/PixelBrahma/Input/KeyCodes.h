#pragma once

namespace PixelBrahma
{
	// Keycode enum class
	typedef enum class KeyCode : uint16_t
	{
		// From glfw3.h
		Space          = 32,
		Apostrophe     = 39,     /* ' */
		Comma          = 44,     /* , */
		Minus          = 45,     /* - */
		Period         = 46,     /* . */
		Slash          = 47,     /* / */

		D0             = 48,     /* 0 */
		D1             = 49,     /* 1 */
		D2             = 50,     /* 2 */
		D3             = 51,     /* 3 */
		D4             = 52,     /* 4 */
		D5             = 53,     /* 5 */
		D6             = 54,     /* 6 */
		D7             = 55,     /* 7 */
		D8             = 56,     /* 8 */
		D9             = 57,     /* 9 */

		Semicolon      = 59,     /* ; */
		Equal          = 61,     /* = */

		A              = 65,
		B              = 66,
		C              = 67,
		D              = 68,
		E              = 69,
		F              = 70,
		G              = 71,
		H              = 72,
		I              = 73,
		J              = 74,
		K              = 75,
		L              = 76,
		M              = 77,
		N              = 78,
		O              = 79,
		P              = 80,
		Q              = 81,
		R              = 82,
		S              = 83,
		T              = 84,
		U              = 85,
		V              = 86,
		W              = 87,
		X              = 88,
		Y              = 89,
		Z              = 90,

		LeftBracket    = 91,     /* [ */
		Backslash      = 92,     /* \ */
		RightBracket   = 93,     /* ] */
		GraveAccent    = 96,     /* ` */

		World1         = 161,    /* non-US #1 */
		World2         = 162,    /* non-US #2 */

		/* Function keys */
		Escape         = 256,
		Enter          = 257,
		Tab            = 258,
		Backspace      = 259,
		Insert         = 260,
		Delete         = 261,
		Right          = 262,
		Left           = 263,
		Down           = 264,
		Up             = 265,
		PageUp         = 266,
		PageDown       = 267,
		Home           = 268,
		End            = 269,
		CapsLock       = 280,
		ScrollLock     = 281,
		NumLock        = 282,
		PrintScreen    = 283,
		Pause          = 284,
		F1             = 290,
		F2             = 291,
		F3             = 292,
		F4             = 293,
		F5             = 294,
		F6             = 295,
		F7             = 296,
		F8             = 297,
		F9             = 298,
		F10            = 299,
		F11            = 300,
		F12            = 301,
		F13            = 302,
		F14            = 303,
		F15            = 304,
		F16            = 305,
		F17            = 306,
		F18            = 307,
		F19            = 308,
		F20            = 309,
		F21            = 310,
		F22            = 311,
		F23            = 312,
		F24            = 313,
		F25            = 314,

		/* Keypad */
		KP0            = 320,
		KP1            = 321,
		KP2            = 322,
		KP3            = 323,
		KP4            = 324,
		KP5            = 325,
		KP6            = 326,
		KP7            = 327,
		KP8            = 328,
		KP9            = 329,
		KPDecimal      = 330,
		KPDivide       = 331,
		KPMultiply     = 332,
		KPSubtract     = 333,
		KPAdd          = 334,
		KPEnter        = 335,
		KPEqual        = 336,

		LeftShift      = 340,
		LeftControl    = 341,
		LeftAlt        = 342,
		LeftSuper      = 343,
		RightShift     = 344,
		RightControl   = 345,
		RightAlt       = 346,
		RightSuper     = 347,
		Menu           = 348
	} Key;

	// Operator overloading for printing keycode to the output stream
	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
	}
}

// From glfw3.h
#define PB_KEY_SPACE           ::PixelBrahma::Key::Space
#define PB_KEY_APOSTROPHE      ::PixelBrahma::Key::Apostrophe    /* ' */
#define PB_KEY_COMMA           ::PixelBrahma::Key::Comma         /* , */
#define PB_KEY_MINUS           ::PixelBrahma::Key::Minus         /* - */
#define PB_KEY_PERIOD          ::PixelBrahma::Key::Period        /* . */
#define PB_KEY_SLASH           ::PixelBrahma::Key::Slash         /* / */
#define PB_KEY_0               ::PixelBrahma::Key::D0
#define PB_KEY_1               ::PixelBrahma::Key::D1
#define PB_KEY_2               ::PixelBrahma::Key::D2
#define PB_KEY_3               ::PixelBrahma::Key::D3
#define PB_KEY_4               ::PixelBrahma::Key::D4
#define PB_KEY_5               ::PixelBrahma::Key::D5
#define PB_KEY_6               ::PixelBrahma::Key::D6
#define PB_KEY_7               ::PixelBrahma::Key::D7
#define PB_KEY_8               ::PixelBrahma::Key::D8
#define PB_KEY_9               ::PixelBrahma::Key::D9
#define PB_KEY_SEMICOLON       ::PixelBrahma::Key::Semicolon     /* ; */
#define PB_KEY_EQUAL           ::PixelBrahma::Key::Equal         /* = */
#define PB_KEY_A               ::PixelBrahma::Key::A
#define PB_KEY_B               ::PixelBrahma::Key::B
#define PB_KEY_C               ::PixelBrahma::Key::C
#define PB_KEY_D               ::PixelBrahma::Key::D
#define PB_KEY_E               ::PixelBrahma::Key::E
#define PB_KEY_F               ::PixelBrahma::Key::F
#define PB_KEY_G               ::PixelBrahma::Key::G
#define PB_KEY_H               ::PixelBrahma::Key::H
#define PB_KEY_I               ::PixelBrahma::Key::I
#define PB_KEY_J               ::PixelBrahma::Key::J
#define PB_KEY_K               ::PixelBrahma::Key::K
#define PB_KEY_L               ::PixelBrahma::Key::L
#define PB_KEY_M               ::PixelBrahma::Key::M
#define PB_KEY_N               ::PixelBrahma::Key::N
#define PB_KEY_O               ::PixelBrahma::Key::O
#define PB_KEY_P               ::PixelBrahma::Key::P
#define PB_KEY_Q               ::PixelBrahma::Key::Q
#define PB_KEY_R               ::PixelBrahma::Key::R
#define PB_KEY_S               ::PixelBrahma::Key::S
#define PB_KEY_T               ::PixelBrahma::Key::T
#define PB_KEY_U               ::PixelBrahma::Key::U
#define PB_KEY_V               ::PixelBrahma::Key::V
#define PB_KEY_W               ::PixelBrahma::Key::W
#define PB_KEY_X               ::PixelBrahma::Key::X
#define PB_KEY_Y               ::PixelBrahma::Key::Y
#define PB_KEY_Z               ::PixelBrahma::Key::Z
#define PB_KEY_LEFT_BRACKET    ::PixelBrahma::Key::LeftBracket   /* [ */
#define PB_KEY_BACKSLASH       ::PixelBrahma::Key::Backslash     /* \ */
#define PB_KEY_RIGHT_BRACKET   ::PixelBrahma::Key::RightBracket  /* ] */
#define PB_KEY_GRAVE_ACCENT    ::PixelBrahma::Key::GraveAccent   /* ` */
#define PB_KEY_WORLD_1         ::PixelBrahma::Key::World1        /* non-US #1 */
#define PB_KEY_WORLD_2         ::PixelBrahma::Key::World2        /* non-US #2 */

/* Function keys */
#define PB_KEY_ESCAPE          ::PixelBrahma::Key::Escape
#define PB_KEY_ENTER           ::PixelBrahma::Key::Enter
#define PB_KEY_TAB             ::PixelBrahma::Key::Tab
#define PB_KEY_BACKSPACE       ::PixelBrahma::Key::Backspace
#define PB_KEY_INSERT          ::PixelBrahma::Key::Insert
#define PB_KEY_DELETE          ::PixelBrahma::Key::Delete
#define PB_KEY_RIGHT           ::PixelBrahma::Key::Right
#define PB_KEY_LEFT            ::PixelBrahma::Key::Left
#define PB_KEY_DOWN            ::PixelBrahma::Key::Down
#define PB_KEY_UP              ::PixelBrahma::Key::Up
#define PB_KEY_PAGE_UP         ::PixelBrahma::Key::PageUp
#define PB_KEY_PAGE_DOWN       ::PixelBrahma::Key::PageDown
#define PB_KEY_HOME            ::PixelBrahma::Key::Home
#define PB_KEY_END             ::PixelBrahma::Key::End
#define PB_KEY_CAPS_LOCK       ::PixelBrahma::Key::CapsLock
#define PB_KEY_SCROLL_LOCK     ::PixelBrahma::Key::ScrollLock
#define PB_KEY_NUM_LOCK        ::PixelBrahma::Key::NumLock
#define PB_KEY_PRINT_SCREEN    ::PixelBrahma::Key::PrintScreen
#define PB_KEY_PAUSE           ::PixelBrahma::Key::Pause
#define PB_KEY_F1              ::PixelBrahma::Key::F1
#define PB_KEY_F2              ::PixelBrahma::Key::F2
#define PB_KEY_F3              ::PixelBrahma::Key::F3
#define PB_KEY_F4              ::PixelBrahma::Key::F4
#define PB_KEY_F5              ::PixelBrahma::Key::F5
#define PB_KEY_F6              ::PixelBrahma::Key::F6
#define PB_KEY_F7              ::PixelBrahma::Key::F7
#define PB_KEY_F8              ::PixelBrahma::Key::F8
#define PB_KEY_F9              ::PixelBrahma::Key::F9
#define PB_KEY_F10             ::PixelBrahma::Key::F10
#define PB_KEY_F11             ::PixelBrahma::Key::F11
#define PB_KEY_F12             ::PixelBrahma::Key::F12
#define PB_KEY_F13             ::PixelBrahma::Key::F13
#define PB_KEY_F14             ::PixelBrahma::Key::F14
#define PB_KEY_F15             ::PixelBrahma::Key::F15
#define PB_KEY_F16             ::PixelBrahma::Key::F16
#define PB_KEY_F17             ::PixelBrahma::Key::F17
#define PB_KEY_F18             ::PixelBrahma::Key::F18
#define PB_KEY_F19             ::PixelBrahma::Key::F19
#define PB_KEY_F20             ::PixelBrahma::Key::F20
#define PB_KEY_F21             ::PixelBrahma::Key::F21
#define PB_KEY_F22             ::PixelBrahma::Key::F22
#define PB_KEY_F23             ::PixelBrahma::Key::F23
#define PB_KEY_F24             ::PixelBrahma::Key::F24
#define PB_KEY_F25             ::PixelBrahma::Key::F25

/* Keypad */
#define PB_KEY_KP_0            ::PixelBrahma::Key::KP0
#define PB_KEY_KP_1            ::PixelBrahma::Key::KP1
#define PB_KEY_KP_2            ::PixelBrahma::Key::KP2
#define PB_KEY_KP_3            ::PixelBrahma::Key::KP3
#define PB_KEY_KP_4            ::PixelBrahma::Key::KP4
#define PB_KEY_KP_5            ::PixelBrahma::Key::KP5
#define PB_KEY_KP_6            ::PixelBrahma::Key::KP6
#define PB_KEY_KP_7            ::PixelBrahma::Key::KP7
#define PB_KEY_KP_8            ::PixelBrahma::Key::KP8
#define PB_KEY_KP_9            ::PixelBrahma::Key::KP9
#define PB_KEY_KP_DECIMAL      ::PixelBrahma::Key::KPDecimal
#define PB_KEY_KP_DIVIDE       ::PixelBrahma::Key::KPDivide
#define PB_KEY_KP_MULTIPLY     ::PixelBrahma::Key::KPMultiply
#define PB_KEY_KP_SUBTRACT     ::PixelBrahma::Key::KPSubtract
#define PB_KEY_KP_ADD          ::PixelBrahma::Key::KPAdd
#define PB_KEY_KP_ENTER        ::PixelBrahma::Key::KPEnter
#define PB_KEY_KP_EQUAL        ::PixelBrahma::Key::KPEqual

#define PB_KEY_LEFT_SHIFT      ::PixelBrahma::Key::LeftShift
#define PB_KEY_LEFT_CONTROL    ::PixelBrahma::Key::LeftControl
#define PB_KEY_LEFT_ALT        ::PixelBrahma::Key::LeftAlt
#define PB_KEY_LEFT_SUPER      ::PixelBrahma::Key::LeftSuper
#define PB_KEY_RIGHT_SHIFT     ::PixelBrahma::Key::RightShift
#define PB_KEY_RIGHT_CONTROL   ::PixelBrahma::Key::RightControl
#define PB_KEY_RIGHT_ALT       ::PixelBrahma::Key::RightAlt
#define PB_KEY_RIGHT_SUPER     ::PixelBrahma::Key::RightSuper
#define PB_KEY_MENU            ::PixelBrahma::Key::Menu