import core.Client;
import app.SDLAppViewFrame;

#ifdef _DEBUG
#include <catch2/catch_session.hpp>

int main(int argc, char* args[])

#else  //!_DEBUG
#include <Windows.h>

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
#endif //_DEBUG

{
#ifdef _DEBUG
	//run test cases
	Catch::Session().run();
#endif

	//main definition
	mbg::Client client1(mbg::SDLAppViewFrame::createPointer(60));
	client1.run(0);

	return 0;
}
