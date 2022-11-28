/*
* File:		Options.cpp
* Author:	NetSkelter
* Created:	2022.11.23
*/

#include "SandBox.h"

glm::ivec2 Options::WINDOW_DIMS = glm::ivec2(800, 600);
bool Options::FULLSCREEN = false;
float Options::VOLUME = 1.0f;
std::string Options::USERNAME = "Asclepias";
std::string Options::ADDRESS = "127.0.0.1";
unsigned short Options::PORT = 2773;

std::vector<std::string> splitStr(const std::string& str, char regex) {
	std::vector<std::string> split;
	std::string tmp = "";
	for (int i = 0; i < str.length(); i++) {
		if (str[i] == regex) {
			split.push_back(tmp);
			tmp.clear();
		}
		else {
			tmp += str[i];
		}
	}
	if (!tmp.empty()) {
		split.push_back(tmp);
	}
	return split;
}

void Options::LoadOptions() {
	std::ifstream optionsFile("options.txt");
	if (optionsFile.good()) {
		std::string line;
		while (std::getline(optionsFile, line)) {
			std::vector<std::string> sp = splitStr(line, ':');
			if (sp.size() != 2) {
				continue;
			}
			if (sp.at(0) == "window_dims") {
				if (sp.at(1) == "800x600") {
					WINDOW_DIMS = glm::ivec2(800, 600);
				}
				else if (sp.at(1) == "1080x720") {
					WINDOW_DIMS = glm::ivec2(1080, 720);
				}
				else if (sp.at(1) == "1920x1080") {
					WINDOW_DIMS = glm::ivec2(1920, 1080);
				}
			}
			else if (sp.at(0) == "fullscreen") {
				if (sp.at(1) == "true") {
					FULLSCREEN = true;
				}
				else {
					FULLSCREEN = false;
				}
			}
			else if (sp.at(0) == "volume") {
				VOLUME = (float)std::atof(sp.at(1).c_str());
			}
			else if (sp.at(0) == "username") {
				USERNAME = sp.at(1);
			}
			else if (sp.at(0) == "address") {
				ADDRESS = sp.at(1);
			}
			else if (sp.at(0) == "port") {
				PORT = (unsigned short)std::atoi(sp.at(1).c_str());
			}
		}
	}
	else {
		optionsFile.close();
		std::ofstream outFile("options.txt");
		outFile << "window_dims:800x600" << std::endl
			<< "fullscreen:false" << std::endl
			<< "volume:1.0" << std::endl
			<< "username:Asclepias" << std::endl
			<< "address:127.0.0.1" << std::endl
			<< "port:2773" << std::endl;
		outFile.close();
	}
}

bool Options::init() {
	ASCLOG(Options, Info, "Initialized test scene 2.");
	glm::vec2 mainBtnDims = glm::vec2(0.25f, 0.15f);
	mainUI_.init(*this, 0, App::renderer().getShader(), App::renderer().getFont("Assets/font/arial.ttf"));
	backBtn_.init(glm::vec3(-0.5f, -0.5f, 0.0f), mainBtnDims, SandBox::BTN_TEX, "Back", SandBox::TEXT_SCALE,
		SandBox::TEXT_COLOR);
	mainUI_.addCmpt(backBtn_);
	defaultsBtn_.init(glm::vec3(-0.125, -0.5f, 0.0f), mainBtnDims, SandBox::BTN_TEX, "Defaults",
		SandBox::TEXT_SCALE, SandBox::TEXT_COLOR);
	mainUI_.addCmpt(defaultsBtn_);
	applyBtn_.init(glm::vec3(0.25f, -0.5f, 0.0f), mainBtnDims, SandBox::BTN_TEX, "Apply", SandBox::TEXT_SCALE,
		SandBox::TEXT_COLOR);
	mainUI_.addCmpt(applyBtn_);
	glm::vec2 optionDims = glm::vec2(0.48, 0.15f);
	optionsUI_.init(*this, 1, App::renderer().getShader(), App::renderer().getFont("Assets/font/arial.ttf"));
	windowDimsCyc_.init(glm::vec3(-0.49f, 0.3f, 0.0f), optionDims, SandBox::CYC_TEX, SandBox::BTN_TEX,
		{ "800x600", "1080x720", "1920x1080" }, "Window Dimensions", "", "", SandBox::TEXT_SCALE,
		SandBox::TEXT_COLOR, ASC_ALIGN_CENTER, ASC_ALIGN_CENTER, ASC_ALIGN_RIGHT, ASC_ALIGN_TOP,
		ASC_ALIGN_LEFT, ASC_ALIGN_BOTTOM);
	optionsUI_.addCmpt(windowDimsCyc_);
	fullscreenSw_.init(glm::vec3(-0.49f, 0.1f, 0.0f), optionDims, SandBox::SW_TEX, "Fullscreen",
		SandBox::TEXT_SCALE, SandBox::TEXT_COLOR, ASC_ALIGN_TOP, ASC_ALIGN_LEFT, ASC_ALIGN_BOTTOM);
	optionsUI_.addCmpt(fullscreenSw_);
	volumeSl_.init(glm::vec3(-0.49f, -0.1f, 0.0f), optionDims, SandBox::SL_TEX, SandBox::CURSOR_TEX,
		SandBox::CURSOR_WIDTH, "Volume", SandBox::TEXT_SCALE, SandBox::TEXT_COLOR, ASC_ALIGN_TOP,
		ASC_ALIGN_LEFT, ASC_ALIGN_BOTTOM);
	optionsUI_.addCmpt(volumeSl_);
	usernameBox_.init(glm::vec3(0.01f, 0.3f, 0.0f), optionDims, SandBox::BOX_TEX, SandBox::CURSOR_TEX,
		1.0f, "Username", "", SandBox::TEXT_SCALE, SandBox::TEXT_COLOR, 20, "", ASC_ALIGN_CENTER,
		ASC_ALIGN_TOP, ASC_ALIGN_LEFT, ASC_ALIGN_BOTTOM);
	optionsUI_.addCmpt(usernameBox_);
	addressBox_.init(glm::vec3(0.01f, 0.1f, 0.0f), optionDims, SandBox::BOX_TEX, SandBox::CURSOR_TEX,
		1.0f, "Address", "", SandBox::TEXT_SCALE, SandBox::TEXT_COLOR, 20, "", ASC_ALIGN_CENTER,
		ASC_ALIGN_TOP, ASC_ALIGN_LEFT, ASC_ALIGN_BOTTOM);
	optionsUI_.addCmpt(addressBox_);
	portBox_.init(glm::vec3(0.01f, -0.1f, 0.0f), optionDims, SandBox::BOX_TEX, SandBox::CURSOR_TEX,
		1.0f, "Port", "", SandBox::TEXT_SCALE, SandBox::TEXT_COLOR, 6, "0123456789", ASC_ALIGN_CENTER,
		ASC_ALIGN_TOP, ASC_ALIGN_LEFT, ASC_ALIGN_BOTTOM);
	optionsUI_.addCmpt(portBox_);
	return true;
}

void Options::enter(Scene& prev) {
	ASCLOG(Options, Info, "Entered test scene 2.");
	std::ostringstream sstr;
	sstr << WINDOW_DIMS.x << "x" << WINDOW_DIMS.y;
	windowDimsCyc_.setText(sstr.str());
	if (FULLSCREEN) {
		ASCLOG(Options, Info, "Fullscreen was loaded true");
	}
	else {
		ASCLOG(Options, Info, "Fullscreen was loaded false");
	}
	fullscreenSw_.setOn(FULLSCREEN);
	volumeSl_.setValue(VOLUME);
	usernameBox_.setText(USERNAME);
	addressBox_.setText(ADDRESS);
	portBox_.setText(std::to_string(PORT));
}

void Options::draw() {
	mainUI_.draw();
	optionsUI_.draw();
}

bool Options::processInput() {
	mainUI_.processInput();
	optionsUI_.processInput();
	return true;
}

void Options::msgReceived(NetMsg& msg) {

}

void Options::cmptEvent(unsigned int gID, unsigned int cID, unsigned int eID) {
	if (gID == mainUI_.getID()) {
		if (cID == backBtn_.getID()) {
			if (eID == Button::CLICKED_EVENT) {
				App::SetScene(SandBox::TITLE_SCENE);
			}
		}
		else if (cID == defaultsBtn_.getID()) {
			if (eID == Button::CLICKED_EVENT) {
				windowDimsCyc_.setText("800x600");
				fullscreenSw_.setOn(false);
				volumeSl_.setValue(1.0f);
				usernameBox_.setText("Asclepias");
				addressBox_.setText("127.0.0.1");
				portBox_.setText("2773");
				cmptEvent(mainUI_.getID(), applyBtn_.getID(), Button::CLICKED_EVENT);
			}
		}
		else if (cID == applyBtn_.getID()) {
			if (eID == Button::CLICKED_EVENT) {
				if (windowDimsCyc_.getText() == "800x600") {
					WINDOW_DIMS = glm::ivec2(800, 600);
				}
				else if (windowDimsCyc_.getText() == "1080x720") {
					WINDOW_DIMS = glm::ivec2(1080, 720);
				}
				else if (windowDimsCyc_.getText() == "1920x1080") {
					WINDOW_DIMS = glm::ivec2(1920, 1080);
				}
				App::window().setDims(WINDOW_DIMS);
				if (fullscreenSw_.isOn()) {
					FULLSCREEN = true;
				}
				else {
					FULLSCREEN = false;
				}
				App::window().setFullscreen(FULLSCREEN);
				VOLUME = volumeSl_.getValue();
				App::audio().setVolume(VOLUME);
				USERNAME = usernameBox_.getText();
				ADDRESS = addressBox_.getText();
				PORT = (unsigned short)std::atoi(portBox_.getText().c_str());
				saveOptions();
			}
		}
	}
}

void Options::update(float dt) {
	mainUI_.update(dt);
	optionsUI_.update(dt);
}

void Options::leave(Scene& next) {
	ASCLOG(Options, Info, "Left test scene 2.");
}

void Options::destroy() {
	ASCLOG(Options, Info, "Destroyed test scene 2.");
	mainUI_.destroy();
	optionsUI_.destroy();
}

void Options::saveOptions() {
	std::ofstream file("options.txt");
	if (!file.good()) {
		return;
	}
	file << "window_dims:" << WINDOW_DIMS.x << "x" << WINDOW_DIMS.y << std::endl;
	file << "fullscreen:" << (FULLSCREEN ? "true" : "false") << std::endl;
	file << "volume:" << VOLUME << std::endl;
	file << "username:" << USERNAME << std::endl;
	file << "address:" << ADDRESS << std::endl;
	file << "port:" << PORT << std::endl;
	file.close();
}