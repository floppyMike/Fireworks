#include <CustomLibrary/SDL/Engine.h>

#include "Application.h"

using namespace ctl;

int main()
{
	sdl::SDL s;

	Fireworks					  app;
	sdl::SimpleRunLoop<Fireworks> loop(&app);
	loop.run(60);

	return 0;
}