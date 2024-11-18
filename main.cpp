import core.Client;
import app.SDLAppViewFrame;

#include "Server.h"
#include <thread>
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
  mbg::Server server;
  std::thread serverThread([&server] { server.run(); });

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	//main definition
	auto appVF = mbg::SDLAppViewFrame::createPointer(60);
	appVF->serverReceiver = server.components_.chessGameReceiver_;

	server.components_.clientNotifier_->tranceiver  = &(appVF->transceiver);
	server.components_.clientNotifier_->tranceiver2 = &(appVF->transceiver2);
	server.components_.clientNotifier_->tranceiver3 = &(appVF->transceiver3);
	server.components_.clientNotifier_->tranceiver4 = &(appVF->transceiver4);

	mbg::Client client1(appVF);
	client1.run(0);

  serverThread.join();	

	return 0;
}
