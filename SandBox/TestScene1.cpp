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
	if (App::input().isKeyPressed(ASC_KEY_L)) {
		std::vector<int> ctrls = App::input().getCtrlIDs();
		ASCLOG(TS1, Info, ctrls.size(), " controllers are connected.");
		for (int ctrl : ctrls) {
			ASCLOG(TS1, Info, "ID ", ctrl, " connected.");
		}
	}
	if (App::input().isCtrlConnected(1)) {
		if (App::input().isCtrlBtnDown(1, ASC_CB_A)) {
			ASCLOG(TS1, Info, "Controller 1's A button is down.");
		}
		if (App::input().isCtrlBtnPressed(1, ASC_CB_B)) {
			ASCLOG(TS1, Info, "Controller 1's B button is pressed.");
		}
		if (App::input().isCtrlBtnReleased(1, ASC_CB_X)) {
			ASCLOG(TS1, Info, "Controller 1's X button is released.");
		}
		if (App::input().isCtrlAxisMoved(1, ASC_CA_R_TRIGGER)) {
			float ap = App::input().getCtrlAxisPos(1, ASC_CA_R_TRIGGER);
			ASCLOG(TS1, Info, "Controller 1's right trigger has moved: ", ap, ".");
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