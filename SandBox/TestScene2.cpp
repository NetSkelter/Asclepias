/*
* File:		TestScene2.cpp
* Author:	NetSkelter
* Created:	2022.11.23
*/

#include "SandBox.h"

bool TestScene2::init() {
	ASCLOG(TS2, Info, "Initialized test scene 2.");
	return true;
}

void TestScene2::enter(Scene& prev) {
	ASCLOG(TS2, Info, "Entered test scene 2.");
}

void TestScene2::draw() {

}

bool TestScene2::processInput() {
	if (App::input().isKeyPressed(ASC_KEY_1)) {
		App::SetScene(SandBox::TS1);
	}
	return true;
}

void TestScene2::cmpEvent(int gID, int cID, int eID) {

}

void TestScene2::update(float dt) {

}

void TestScene2::leave(Scene& next) {
	ASCLOG(TS2, Info, "Left test scene 2.");
}

void TestScene2::destroy() {
	ASCLOG(TS2, Info, "Destroyed test scene 2.");
}

void TestScene2::mouseMoved(const glm::vec2& pos) {
	ASCLOG(TS2, Info, "Mouse moved (", pos.x, ", ", pos.y, ").");
}

void TestScene2::mouseBtnPressed(int btn) {
	ASCLOG(TS2, Info, "Mouse button ", btn, " pressed.");
}

void TestScene2::mouseBtnReleased(int btn) {
	ASCLOG(TS2, Info, "Mouse button ", btn, " released.");
}

void TestScene2::mouseScrolled(const glm::vec2& scroll) {
	ASCLOG(TS2, Info, "Mouse scrolled (", scroll.x, ", ", scroll.y, ").");
}