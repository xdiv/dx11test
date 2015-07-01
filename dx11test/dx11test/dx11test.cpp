#include "GameWindow.h"
#include "Game.h"
#include "../includes/tinyxml2/tinyxml2.h"
using namespace tinyxml2;



int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nShowCmd)
{
	/*Game * g = 0;
	g = new Game();

	g->Init();
	g->Run();

	delete g;*/

	tinyxml2::XMLDocument xmlDoc;
	XMLNode * pRoot = xmlDoc.NewElement("Root");
	xmlDoc.InsertFirstChild(pRoot);
	XMLElement * pElement = xmlDoc.NewElement("IntValue");
	pElement->SetText(10);
	pRoot->InsertEndChild(pElement);

	pElement = xmlDoc.NewElement("FloatValue");
	pElement->SetText(0.5f);
	pRoot->InsertEndChild(pElement);

	pElement = xmlDoc.NewElement("Date");
	pElement->SetAttribute("day", 26);
	pElement->SetAttribute("month", "April");
	pElement->SetAttribute("year", 2014);
	pElement->SetAttribute("dateFormat", "26/04/2014");
	pRoot->InsertEndChild(pElement);
	pElement = xmlDoc.NewElement("List");

	for (size_t i = 0; i < 6; i++)
	{
		XMLElement * pListElement = xmlDoc.NewElement("Item");
		pListElement->SetText(i);

		pElement->InsertEndChild(pListElement);
	}

	pElement->SetAttribute("itemCount", 6);
	pRoot->InsertEndChild(pElement);

	XMLError eResult = xmlDoc.SaveFile("SavedData.xml");
	//tinyxml2::XMLCheckResult(eResult);

	tinyxml2::XMLDocument xmlDocLoad;
	eResult = xmlDoc.LoadFile("SavedData.xml");
	XMLNode * pRoot2 = xmlDoc.FirstChild();

	return 0;
}