#include <PixelBrahma.h>

class Sandbox : public PixelBrahma::Application
{
public:
	Sandbox() {}
	~Sandbox() {}
};

PixelBrahma::Application* PixelBrahma::CreateApplication()
{
	return new Sandbox();
}