#include "GameWindow.h"

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nShowCmd)
{
	GameWindow * gw;
	int w, h;
	w = 800;
	h = 600;
	gw = new GameWindow(w, h, L"Hello world", 0.1F, 1000.0f);
	gw->InitializeWindows();
	gw->InitD3D();
	gw->Run();
	gw->Close();

	delete gw;

	/*MD5Model mod;
	meshv1 * mesh = NULL;
	int* list = NULL;

	mod.LoadModel("untl.md5mesh");
	mod.PrepareMesh(mesh, list);*/
	return 0;
}