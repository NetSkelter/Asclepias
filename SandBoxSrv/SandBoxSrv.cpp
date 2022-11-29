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
		assignUsername(msg.owner->getID(), username);
	}
}

void SandBoxSrv::clientDisconnected(NetConnPtr client) {
	NSLOG(SBS, Info, "Client ", client->getID(), " disconnected.");
}

void SandBoxSrv::assignUsername(unsigned int ID, const std::string& username) {
	NSLOG(SBS, Info, "Player ", ID, " assigned username ", username, ".");
	Player p;
	p.ID = ID;
	p.username = username;
	players_.push_back(p);
	NetMsg idmsg(ID_ASSIGN);
	idmsg << ID << convertUsername(username);
	sendAll(idmsg);
	if (players_.size() > 1) {
		NSLOG(SBS, Info, "Players already connected, sending list message.");
		NetMsg listmsg(PLAYER_LIST);
		for (Player& p : players_) {
			if (p.ID != ID) {
				listmsg << p.ID << convertUsername(p.username);
			}
		}
		send(listmsg, getClientByID(ID));
	}
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