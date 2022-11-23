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
}

void TestScene::draw() {

}

void TestScene::processInput() {

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