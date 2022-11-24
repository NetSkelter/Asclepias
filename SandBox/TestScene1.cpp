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
	if (App::input().isKeyPressed(ASC_KEY_E)) {
		App::input().setMouseEnabled(!App::input().isMouseEnabled());
		if (App::input().isMouseEnabled()) {
			ASCLOG(TS1, Info, "Mouse enabled.");
		}
		else {
			ASCLOG(TS1, Info, "Mouse disabled.");
		}
	}
	if (App::input().isKeyPressed(ASC_KEY_V)) {
		App::input().setMouseVisible(!App::input().isMouseVisible());
		if (App::input().isMouseVisible()) {
			ASCLOG(TS1, Info, "Mouse visible.");
		}
		else {
			ASCLOG(TS1, Info, "Mouse invisible.");
		}
	}
	if (App::input().isMouseMoved()) {
		glm::vec2 mp = App::input().getMousePos();
		ASCLOG(TS1, Info, "Mouse moved (", mp.x, ", ", mp.y, ").");
	}
	if (App::input().isMouseBtnDown(ASC_MB_LEFT)) {
		ASCLOG(TS1, Info, "Left mouse button down.");
	}
	if (App::input().isMouseBtnPressed(ASC_MB_RIGHT)) {
		ASCLOG(TS1, Info, "Right mouse button pressed.");
	}
	if (App::input().isMouseBtnReleased(ASC_MB_RIGHT)) {
		ASCLOG(TS1, Info, "Right mouse button released.");
	}
	if (App::input().isMouseScrolled()) {
		glm::vec2 ms = App::input().getMouseScroll();
		ASCLOG(TS1, Info, "Mouse scrolled (", ms.x, ", ", ms.y, ").");
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