/*
* File:		TestScene1.cpp
* Author:	NetSkelter
* Created:	2022.11.22
*/

#include "SandBox.h"

bool TestScene1::init() {
	ASCLOG(TS1, Info, "Initializing test scene 1.");
	glm::vec2 cmptDims(0.48f, 0.15f);
	UI_.init(*this, 0, App::renderer().getShader(), SandBox::FONT);
	lbl_.init(glm::vec3(-0.49f, 0.49f - 0.15f, 0.0f), cmptDims, SandBox::LABEL_TEX, "The first label!",
		SandBox::TEXT_SCALE, SandBox::TEXT_COLOR);
	UI_.addCmpt(lbl_);
	btn_.init(glm::vec3(-0.49f, 0.48f - 2 * 0.15f, 0.0f), cmptDims, SandBox::BTN_TEX, "Button",
		SandBox::TEXT_SCALE, SandBox::TEXT_COLOR);
	UI_.addCmpt(btn_);
	sw_.init(glm::vec3(-0.49f, 0.47 - 3 * 0.15f, 0.0f), cmptDims, SandBox::SW_TEX, "Switch",
		SandBox::TEXT_SCALE, SandBox::TEXT_COLOR, ASC_ALIGN_CENTER, ASC_ALIGN_LEFT, ASC_ALIGN_TOP);
	UI_.addCmpt(sw_);
	sl_.init(glm::vec3(-0.49f, 0.46 - 4 * 0.15f, 0.0f), cmptDims, SandBox::SL_TEX, SandBox::CURSOR_TEX,
		SandBox::CURSOR_WIDTH, "Slider", SandBox::TEXT_SCALE, SandBox::TEXT_COLOR, ASC_ALIGN_CENTER,
		ASC_ALIGN_LEFT, ASC_ALIGN_CENTER);
	UI_.addCmpt(sl_);
	cyc_.init(glm::vec3(-0.49f, 0.45 - 5 * 0.15f, 0.0f), cmptDims, SandBox::CYC_TEX, SandBox::BTN_TEX,
		{ "Text 1", "Test 2", "44444" }, "", "", "", SandBox::TEXT_SCALE, SandBox::TEXT_COLOR,
		ASC_ALIGN_CENTER, ASC_ALIGN_CENTER, ASC_ALIGN_LEFT);
	UI_.addCmpt(cyc_);
	box_.init(glm::vec3(-0.49, -0.49f, 0.0f), cmptDims, SandBox::BOX_TEX, SandBox::CURSOR_TEX,
		SandBox::CURSOR_WIDTH, "Label", "", SandBox::TEXT_SCALE, SandBox::TEXT_COLOR);
	UI_.addCmpt(box_);

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
	ASCLOG(TS1, Info, "Event ", eID, " from component ", cID, " of group ", gID, ".");
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
Font& SandBox::FONT = Renderer::NO_FONT;
float SandBox::TEXT_SCALE = 0.5f;
glm::vec3 SandBox::TEXT_COLOR = glm::vec3(1.0f, 1.0f, 0.0f);
GLuint SandBox::LABEL_TEX;
GLuint SandBox::BTN_TEX;
GLuint SandBox::SW_TEX;
GLuint SandBox::SL_TEX;
GLuint SandBox::CURSOR_TEX;
float SandBox::CURSOR_WIDTH = 5.0f;
GLuint SandBox::CYC_TEX;
GLuint SandBox::BOX_TEX;

int main(int argc, char** argv) {
	App::Config conf(SandBox::TS1);
	if (!App::Init(conf)) {
		return -1;
	}
	SandBox::FONT = App::renderer().getFont("Assets/font/arial.ttf");
	SandBox::LABEL_TEX = App::renderer().getTexture("Assets/texture/label.png");
	SandBox::BTN_TEX = App::renderer().getTexture("Assets/texture/button.png");
	SandBox::SW_TEX = App::renderer().getTexture("Assets/texture/switch.png");
	SandBox::SL_TEX = App::renderer().getTexture("Assets/texture/slider.png");
	SandBox::CURSOR_TEX = App::renderer().getTexture("Assets/texture/cursor.png");
	SandBox::CYC_TEX = App::renderer().getTexture("Assets/texture/cycle.png");
	SandBox::BOX_TEX = App::renderer().getTexture("Assets/texture/text_box.png");
	App::Run();
	App::Destroy();
	return 0;
}