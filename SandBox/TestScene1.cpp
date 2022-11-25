/*
* File:		TestScene1.cpp
* Author:	NetSkelter
* Created:	2022.11.22
*/

#include "SandBox.h"

bool TestScene1::init() {
	ASCLOG(TS1, Info, "Initializing test scene 1.");
	UI_.init(*this, 0, App::renderer().getShader(), App::renderer().getFont("Assets/font/arial.ttf"));
	label1_.init(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.25f, 0.15f),
		App::renderer().getTexture("Assets/texture/anim1.png"), "The first label!", 0.5f,
		glm::vec3(1.0f, 1.0f, 0.0f), ASC_ALIGN_RIGHT, ASC_ALIGN_CENTER);
	UI_.addCmpt(label1_);
	label2_.init(glm::vec3(-0.45f, 0.0f, 0.0f), glm::vec2(0.35f, 0.2f),
		App::renderer().getTexture("Assets/texture/anim2.png"), "second labelll", 0.67f,
		glm::vec3(0.01f, 0.01f, 0.01f), ASC_ALIGN_CENTER, ASC_ALIGN_TOP);
	UI_.addCmpt(label2_);

	return true;
}

void TestScene1::enter(Scene& prev) {
	ASCLOG(TS1, Info, "Entered test scene 1.");
}

void TestScene1::draw() {
	UI_.draw();
}

bool TestScene1::processInput() {
	if (App::input().isKeyPressed(ASC_KEY_A)) {
		App::window().setDims(glm::ivec2(1000, 400));
	}
	if (App::input().isKeyPressed(ASC_KEY_S)) {
		App::window().setDims(glm::ivec2(800, 600));
	}

	if (App::input().isKeyPressed(ASC_KEY_2)) {
		App::SetScene(SandBox::TS2);
	}
	UI_.processInput();
	return true;
}

void TestScene1::msgReceived(NetMsg& msg) {

}

void TestScene1::cmptEvent(unsigned int gID, unsigned int cID, unsigned int eID) {

}

void TestScene1::update(float dt) {
	UI_.update(dt);
}

void TestScene1::leave(Scene& next) {
	ASCLOG(TS, Info, "Left test scene 1.");
}

void TestScene1::destroy() {
	ASCLOG(TS, Info, "Destroying test scene 1.");
	UI_.destroy();
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