/*
* File:		SandBox.cpp
* Author:	NetSkelter
* Created:	2022.11.22
*/

#include <iostream>

#include "SandBox.h"

bool TestScene::init() {
	std::cout << "Initialized test scene." << std::endl;
	return true;
}

void TestScene::enter(Scene& prev) {
	std::cout << "Entered test scene from " << &prev << "." << std::endl;
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
	std::cout << "Left test scene for " << &next << "." << std::endl;
}

void TestScene::destroy() {
	std::cout << "Destroyed test scene." << std::endl;
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