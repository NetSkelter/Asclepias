/*
* File:		SandBoxSrv.cpp
* Author:	NetSkelter
* Created:	2022.11.25
*/

#include "SandBoxSrv.h"

SandBoxSrv::SandBoxSrv(unsigned short port) : NetServer(port) {}

bool SandBoxSrv::clientConnected(NetConnPtr client) {
	if (this->getClientCount() < 3) {
		NSLOG(SBS, Info, "Client ", client->getID(), " connected.");
		return true;
	}
	return false;
}

void SandBoxSrv::msgReceived(NetMsg& msg) {
	NSLOG(SBS, Info, "Client ", msg.owner->getID(), " send ", msg, ".");
	sendAll(msg, msg.owner);
}

void SandBoxSrv::clientDisconnected(NetConnPtr client) {
	NSLOG(SBS, Info, "Client ", client->getID(), " disconnected.");
}

int main(int argc, char** argv) {
	SandBoxSrv sbs(2773);
	if (!sbs.init()) {
		std::cout << "Failed to initialize server." << std::endl;
		return EXIT_FAILURE;
	}
	while (sbs.isConnected() && !(GetKeyState(VK_ADD) & 0x8000)) {
		sbs.update();
	}
	sbs.destroy();
	return EXIT_SUCCESS;
}