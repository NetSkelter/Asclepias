/*
* File:		Game.cpp
* Author:	NetSkelter
* Created:	2022.11.28
*/

#include <SandBoxSrv/MsgTypes.h>
#include "SandBox.h"

bool Game::init() {
	player_.init(glm::vec3(), glm::vec2(), App::renderer().getTexture("Assets/texture/player.png"));
	return true;
}

void Game::enter(Scene& prev) {
	NetMsg msg(USERNAME_ASSIGN);
	msg << convertUsername(Options::USERNAME);
	App::network().send(msg);
}

void Game::draw() {

}

bool Game::processInput() {
	if (App::input().isKeyPressed(ASC_CTL_ESC)) {
		App::network().disconnect();
		App::SetScene(SandBox::TITLE_SCENE);
	}
	return true;
}

void Game::msgReceived(NetMsg& msg) {
	if (msg.header.type == ASC_NET_DISCONNECTED) {
		ASCLOG(Game, Info, "Disconnect message received.");
		destroy();
		App::SetScene(SandBox::TITLE_SCENE);
	}
	else if (msg.header.type == ID_ASSIGN) {
		ASCLOG(Game, Info, "Received player ID assignment message.");
		std::array<char, 20> usrbuf;
		unsigned int ID;
		msg >> usrbuf >> ID;
		std::string username(usrbuf.data());
		ASCLOG(Game, Info, "Received ID ", ID, " for username ", username, ".");
	}
	else if (msg.header.type == PLAYER_LIST) {
		ASCLOG(Game, Info, "Received player list message.");
		while (msg.header.size > 0) {
			std::array<char, 20> usrbuf;
			unsigned int ID;
			msg >> usrbuf >> ID;
			std::string username(usrbuf.data());
			ASCLOG(Game, Info, "Previously connected player: ", ID, " named ",
				username, ".");
		}
	}
}

void Game::cmptEvent(unsigned int gID, unsigned int cID, unsigned int eID) {

}

void Game::update(float dt) {

}

void Game::leave(Scene& next) {

}

void Game::destroy() {

}

std::array<char, 20> Game::convertUsername(const std::string& username) {
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