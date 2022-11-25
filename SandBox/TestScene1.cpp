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
	if (App::input().isMouseBtnPressed(ASC_MB_LEFT)) {
		App::audio().playEffect("Assets/audio/effect1.wav");
	}
	if (App::input().isMouseBtnPressed(ASC_MB_RIGHT)) {
		App::audio().playEffect("Assets/audio/effect2.wav");
	}
	if (App::input().isKeyPressed(ASC_KEY_A)) {
		App::audio().playMusic("Assets/audio/music1.wav");
	}
	if (App::input().isKeyPressed(ASC_KEY_S)) {
		App::audio().playMusic("Assets/audio/music2.wav");
	}
	if (App::input().isKeyPressed(ASC_KEY_D)) {
		App::audio().playMusic();
	}
	if (App::input().isKeyPressed(ASC_KEY_F)) {
		App::audio().pauseMusic();
	}
	if (App::input().isKeyPressed(ASC_KEY_G)) {
		App::audio().stopMusic();
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