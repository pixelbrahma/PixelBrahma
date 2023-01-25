namespace PixelBrahma
{
	// Input handler class
	public class Input
	{
		// Check key press funtion
		public static bool IsKeyDown(KeyCode keycode)
		{
			return InternalCalls.Input_IsKeyDown(keycode);
		}
	}
}
