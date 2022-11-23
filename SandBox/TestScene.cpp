/*
* File:		SandBox.cpp
* Author:	NetSkelter
* Created:	2022.11.22
*/

#include <iostream>

#include "SandBox.h"

bool TestScene::init() {
	ASCLOG(TS, Info, "Initializing test scene.");
	return true;
}

void TestScene::enter(Scene& prev) {
	ASCLOG(TS, Info, "Entered from scene ", &prev, ".");
	startTime_ = glfwGetTime();
}

void TestScene::draw() {

}

bool TestScene::processInput() {
	if (glfwGetTime() - startTime_ > 5.0 && !chSize_) {
		chSize_ = true;
		App::window().setDims(glm::ivec2(900, 500));
	}
	if (glfwGetTime() - startTime_ > 10.0 && !chTitle_) {
		chTitle_ = true;
		App::window().setTitle("TestScene");
	}
	if (glfwGetTime() - startTime_ > 15.0 && !chFullscreen_) {
		chFullscreen_ = true;
		App::window().setFullscreen(true);
	}
	if (glfwGetTime() - startTime_ > 20.0 && !chFullscreen2_) {
		chFullscreen2_ = true;
		App::window().setFullscreen(false);
	}
	return true;
}

void TestScene::cmpEvent(int gID, int cID, int eID) {

}

void TestScene::update(float dt) {

}

void TestScene::leave(Scene& next) {
	ASCLOG(TS, Info, "Left for scene ", &next, ".");
}

void TestScene::destroy() {
	ASCLOG(TS, Info, "Destroying test scene.");
}

TestScene SandBox::TS;

int main(int argc, char** argv) {
	App::Config conf(SandBox::TS);
	if (!App::Init(conf)) {
		return -1;
	}
	App::Run();
	App::Destroy();
	return 0;
}