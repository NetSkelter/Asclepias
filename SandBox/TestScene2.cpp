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

void TestScene2::keyPressed(int key) {
	ASCLOG(TS2, Info, "Key ", key, " pressed.");
}

void TestScene2::keyReleased(int key) {
	ASCLOG(TS2, Info, "Key ", key, " released.");
}

void TestScene2::charTyped(char c) {
	ASCLOG(TS2, Info, "Character ", c, " typed.");
}