/*
* File:		TestScene1.cpp
* Author:	NetSkelter
* Created:	2022.11.22
*/

#include "SandBox.h"

bool TestScene1::init() {
	ASCLOG(TS1, Info, "Initializing test scene 1.");
	return true;
}

void TestScene1::enter(Scene& prev) {
	ASCLOG(TS1, Info, "Entered test scene 1.");
}

void TestScene1::draw() {

}

bool TestScene1::processInput() {
	if (App::input().isKeyPressed(ASC_KEY_A)) {
		App::network().connect("127.0.0.1", 2773);
	}
	if (App::input().isKeyPressed(ASC_KEY_S)) {
		App::network().disconnect();
	}
	if (App::input().isMouseBtnPressed(ASC_MB_LEFT)) {
		NetMsg msg(0);
		msg << App::input().getMousePos().x << App::input().getMousePos().y;
		App::network().send(msg);
	}
	if (App::input().isMouseBtnPressed(ASC_MB_RIGHT)) {
		NetMsg msg(1);
		msg << glfwGetTime();
		App::network().send(msg);
	}

	while (!App::network().getMsgs().empty()) {
		NetMsg msg = App::network().getMsgs().popFront();
		switch (msg.header.type) {
		case ASC_NET_CONNECTED:
			ASCLOG(TS1, Info, "Connected to server.");
			break;
		case ASC_NET_FAILED:
			ASCLOG(TS1, Info, "Failed to connect to server.");
			break;
		case ASC_NET_DISCONNECTED:
			ASCLOG(TS1, Info, "Disconnected from server.");
			break;
		default:
			ASCLOG(TS1, Info, "Received message ", msg, ".");
		}
	}

	if (App::input().isKeyPressed(ASC_KEY_2)) {
		App::SetScene(SandBox::TS2);
	}
	return true;
}

void TestScene1::cmpEvent(int gID, int cID, int eID) {

}

void TestScene1::update(float dt) {

}

void TestScene1::leave(Scene& next) {
	ASCLOG(TS, Info, "Left test scene 1.");
}

void TestScene1::destroy() {
	ASCLOG(TS, Info, "Destroying test scene 1.");
}

TestScene1 SandBox::TS1;
TestScene2 SandBox::TS2;

int main(int argc, char** argv) {
	App::Config conf(SandBox::TS1);
	if (!App::Init(conf)) {
		return -1;
	}
	App::Run();
	App::Destroy();
	return 0;
}