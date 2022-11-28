/*
* File:		Title.cpp
* Author:	NetSkelter
* Created:	2022.11.22
*/

#include "SandBox.h"

bool Title::init() {
	bg_.init(glm::vec3(), glm::vec2(), App::renderer().getTexture("Assets/texture/title_bg.png"));
	glm::vec2 btnDims(0.5f, 0.15f);
	mainUI_.init(*this, 0, App::renderer().getShader(), App::renderer().getFont("Assets/font/arial.ttf"));
	playBtn_.init(glm::vec3(-0.25f, 0.1f, 0.0f), btnDims, SandBox::BTN_TEX, "Connect",
		SandBox::TEXT_SCALE, SandBox::TEXT_COLOR);
	mainUI_.addCmpt(playBtn_);
	optionsBtn_.init(glm::vec3(-0.25f, -0.1f, 0.0f), btnDims, SandBox::BTN_TEX, "Options",
		SandBox::TEXT_SCALE, SandBox::TEXT_COLOR);
	mainUI_.addCmpt(optionsBtn_);
	quitBtn_.init(glm::vec3(-0.25f, -0.3f, 0.0f), btnDims, SandBox::BTN_TEX, "Quit", SandBox::TEXT_SCALE,
		SandBox::TEXT_COLOR);
	mainUI_.addCmpt(quitBtn_);
	quit_ = false;
	return true;
}

void Title::enter(Scene& prev) {
	ASCLOG(Title, Info, "Entered test scene 1.");
	App::renderer().getShader().getCamera().pos = glm::vec3(0.0f, 0.0f, 0.0f);
}

void Title::draw() {
	mainUI_.draw();
	App::renderer().submit(bg_);
}

bool Title::processInput() {
	mainUI_.processInput();
	return !quit_;
}

void Title::msgReceived(NetMsg& msg) {
	
}

void Title::cmptEvent(unsigned int gID, unsigned int cID, unsigned int eID) {
	if (gID == mainUI_.getID()) {
		if (cID == playBtn_.getID()) {
			if (eID == Button::CLICKED_EVENT) {
				App::network().connect(Options::ADDRESS, Options::PORT);
			}
		}
		else if (cID == optionsBtn_.getID()) {
			if (eID == Button::CLICKED_EVENT) {
				App::SetScene(SandBox::OPTIONS_SCENE);
			}
		}
		else if (cID == quitBtn_.getID()) {
			if (eID == Button::CLICKED_EVENT) {
				quit_ = true;
			}
		}
	}
}

void Title::update(float dt) {
	mainUI_.update(dt);
	glm::vec2 wd = -(App::window().getDims() / 2);
	bg_.pos = glm::vec3(wd.x, wd.y, -0.1f);
	bg_.dims = App::window().getDims();
}

void Title::leave(Scene& next) {
	ASCLOG(Title, Info, "Left test scene 1.");
}

void Title::destroy() {
	ASCLOG(Title, Info, "Destroying test scene 1.");
	mainUI_.destroy();
}

Title SandBox::TITLE_SCENE;
Options SandBox::OPTIONS_SCENE;
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
	Options::LoadOptions();
	App::Config conf(SandBox::OPTIONS_SCENE);
	conf.window.dims = Options::WINDOW_DIMS;
	conf.window.fullscreen = Options::FULLSCREEN;
	conf.audio.volume = Options::VOLUME;
	std::cout << "Options:" << std::endl << "(" << Options::WINDOW_DIMS.x << ", "
		<< Options::WINDOW_DIMS.y << "), " << Options::FULLSCREEN << ", " << Options::VOLUME << ", "
		<< Options::USERNAME << ", " << Options::ADDRESS << ", " << Options::PORT << std::endl;
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