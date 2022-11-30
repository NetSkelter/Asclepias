/*
* File:		SandBoxSrv.cpp
* Author:	NetSkelter
* Created:	2022.11.25
*/

#include "MsgTypes.h"
#include "SandBoxSrv.h"

SandBoxSrv::SandBoxSrv(unsigned short port) : NetServer(port) {}

bool SandBoxSrv::clientConnected(NetConnPtr client) {
	if (this->getClientCount() >= 10) {
		return false;
	}
	NSLOG(SBS, Info, "Client ", client->getID(), " connected.");
	return true;
}

void SandBoxSrv::msgReceived(NetMsg& msg) {
	NSLOG(SBS, Info, "Client ", msg.owner->getID(), " send ", msg, ".");
	if (msg.header.type == USERNAME_ASSIGN) {
		std::array<char, 20> usrbuf;
		msg >> usrbuf;
		std::string username(usrbuf.data());
		Player np;
		np.ID = msg.owner->getID();
		np.username = username;
		players_.push_back(np);
		NetMsg idmsg(ID_ASSIGN);
		idmsg << np.ID << convertUsername(np.username);
		sendAll(idmsg);
		NSLOG(SBS, Info, "Sent ID assignment.");
		if (players_.size() > 1) {
			NetMsg listmsg(PLAYER_LIST);
			for (Player& pl : players_) {
				if (pl.ID != np.ID) {
					listmsg << pl.ID << convertUsername(pl.username);
				}
			}
			send(listmsg, msg.owner);
			NSLOG(SBS, Info, "Sent player list.");
		}
	}
}

void SandBoxSrv::clientDisconnected(NetConnPtr client) {
	NSLOG(SBS, Info, "Client ", client->getID(), " disconnected.");

}

std::array<char, 20> SandBoxSrv::convertUsername(const std::string& username) {
	std::array<char, 20> usrbuf;
	for (int i = 0; i < 20; i++) {
		if (i < username.length()) {
			usrbuf[i] = username.at(i);
		}
		else {
			usrbuf[i] = '\0';
		}
	}
	return usrbuf;
}

int main(int argc, char** argv) {
	SandBoxSrv sbs(2773);
	if (!sbs.init()) {
		std::cout << "Failed to initialize server." << std::endl;
		return EXIT_FAILURE;
	}
	while (sbs.isConnected() && !(GetKeyState(VK_ADD) & 0x8000)) {
		sbs.update();
		Sleep(1);
	}
	sbs.destroy();
	return EXIT_SUCCESS;
}