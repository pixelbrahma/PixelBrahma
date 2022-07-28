#pragma once

namespace PixelBrahma
{
	// Enum class for Mouse codes
	typedef enum class MouseCode : uint16_t	
	{
		// From glfw3.h
		Button0          = 0,
		Button1          = 1,
		Button2          = 2,
		Button3          = 3,
		Button4          = 4,
		Button5          = 5,
		Button6          = 6,
		Button7          = 7,

		ButtonLast       = Button7,
		ButtonLeft       = Button0,
		ButtonRight      = Button1,
		ButtonMiddle     = Button2
	} Mouse;

	// Operator overloading for output stream printing
	inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
	{
		os << static_cast<int32_t>(mouseCode);
		return os;
	}
}

// From glfw3.h
#define PB_MOUSE_BUTTON_0      ::PixelBrahma::Mouse::Button0
#define PB_MOUSE_BUTTON_1      ::PixelBrahma::Mouse::Button1
#define PB_MOUSE_BUTTON_2      ::PixelBrahma::Mouse::Button2
#define PB_MOUSE_BUTTON_3      ::PixelBrahma::Mouse::Button3
#define PB_MOUSE_BUTTON_4      ::PixelBrahma::Mouse::Button4
#define PB_MOUSE_BUTTON_5      ::PixelBrahma::Mouse::Button5
#define PB_MOUSE_BUTTON_6      ::PixelBrahma::Mouse::Button6
#define PB_MOUSE_BUTTON_7      ::PixelBrahma::Mouse::Button7
#define PB_MOUSE_BUTTON_LAST   ::PixelBrahma::Mouse::ButtonLast
#define PB_MOUSE_BUTTON_LEFT   ::PixelBrahma::Mouse::ButtonLeft
#define PB_MOUSE_BUTTON_RIGHT  ::PixelBrahma::Mouse::ButtonRight
#define PB_MOUSE_BUTTON_MIDDLE ::PixelBrahma::Mouse::ButtonMiddle