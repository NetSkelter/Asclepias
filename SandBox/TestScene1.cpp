/*
* File:		TestScene1.cpp
* Author:	NetSkelter
* Created:	2022.11.22
*/

#include "SandBox.h"

bool TestScene1::init() {
	ASCLOG(TS1, Info, "Initializing test scene 1.");
	s_.init(glm::vec3(0.0f, 0.0f, -0.1f), glm::vec2(100.0f, 100.0f),
		App::renderer().getTexture("Assets/texture/player.png"));
	return true;
}

void TestScene1::enter(Scene& prev) {
	ASCLOG(TS1, Info, "Entered test scene 1.");
}

void TestScene1::draw() {
	App::renderer().submit(anims_);
	App::renderer().submit(s_);
}

bool TestScene1::processInput() {
	if (App::input().isMouseBtnPressed(ASC_MB_LEFT)) {
		glm::vec2 mp = App::input().getMousePos(App::renderer().getShader().getCamera());
		anims_.push_back(Animation());
		ASCLOG(TS1, Info, "Added animation.");
		anims_.back().init(glm::vec3(mp.x, mp.y, 0.0f), glm::vec2(50.0f, 70.0f),
			App::renderer().getTexture("Assets/texture/anim1.png"), glm::ivec2(3, 2), 30.0f);
		anims_.back().play();
	}
	if (App::input().isMouseBtnPressed(ASC_MB_RIGHT)) {
		glm::vec2 mp = App::input().getMousePos(App::renderer().getShader().getCamera());
		anims_.push_back(Animation());
		anims_.back().init(glm::vec3(mp.x, mp.y, 0.1f), glm::vec2(90.0f, 25.0f),
			App::renderer().getTexture("Assets/texture/anim2.png"), glm::ivec2(4, 1), 50.0f);
		anims_.back().play();
	}

	if (App::input().isKeyDown(ASC_KEY_D)) {
		App::renderer().getShader().getCamera().vel.x = 5.0f;
	}
	else if (App::input().isKeyDown(ASC_KEY_A)) {
		App::renderer().getShader().getCamera().vel.x = -5.0f;
	}
	else {
		App::renderer().getShader().getCamera().vel.x = 0.0f;
	}
	if (App::input().isKeyDown(ASC_KEY_W)) {
		App::renderer().getShader().getCamera().vel.y = 5.0f;
	}
	else if (App::input().isKeyDown(ASC_KEY_S)) {
		App::renderer().getShader().getCamera().vel.y = -5.0f;
	}
	else {
		App::renderer().getShader().getCamera().vel.y = 0.0f;
	}
	if (App::input().isKeyDown(ASC_KEY_E)) {
		App::renderer().getShader().getCamera().scaleVel = 0.1f;
	}
	else if (App::input().isKeyDown(ASC_KEY_F)) {
		App::renderer().getShader().getCamera().scaleVel = -0.1f;
	}
	else {
		App::renderer().getShader().getCamera().scaleVel = 0.0f;
	}

	if (App::input().isKeyDown(ASC_CTL_RIGHT)) {
		s_.vel.x = 2.0f;
	}
	else if (App::input().isKeyDown(ASC_CTL_LEFT)) {
		s_.vel.x = -2.0f;
	}
	else {
		s_.vel.x = 0.0f;
	}
	if (App::input().isKeyDown(ASC_CTL_UP)) {
		s_.vel.y = 2.0f;
	}
	else if (App::input().isKeyDown(ASC_CTL_DOWN)) {
		s_.vel.y = -2.0f;
	}
	else {
		s_.vel.y = 0.0f;
	}

	if (App::input().isKeyPressed(ASC_KEY_2)) {
		App::SetScene(SandBox::TS2);
	}
	return true;
}

void TestScene1::cmpEvent(int gID, int cID, int eID) {

}

void TestScene1::update(float dt) {
	for (Animation& a : anims_) {
		a.update(dt);
	}
	s_.update(dt);
}

void TestScene1::leave(Scene& next) {
	ASCLOG(TS, Info, "Left test scene 1.");
}

void TestScene1::destroy() {
	ASCLOG(TS, Info, "Destroying test scene 1.");
	for (Animation& a : anims_) {
		a.destroy();
	}
	anims_.clear();
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