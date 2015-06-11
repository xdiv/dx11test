#include "GameWindow.h"
#include "Game.h"

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nShowCmd)
{
	Game * g = 0;
	g = new Game();

	g->Init();
	g->Run();

	delete g;
	return 0;
}