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
	if (App::input().isKeyDown(ASC_KEY_5)) {
		ASCLOG(TS1, Info, "5 key is down.");
	}
	if (App::input().isKeyPressed(ASC_CTL_L_SHIFT)) {
		ASCLOG(TS1, Info, "Left shift pressed.");
	}
	if (App::input().isKeyReleased(ASC_SYM_APOSTROPHE)) {
		ASCLOG(TS1, Info, "Apostrophe released.");
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