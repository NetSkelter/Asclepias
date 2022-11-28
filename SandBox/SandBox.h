/*
* File:		SandBox.h
* Author:	NetSkelter
* Created:	2022.11.22
*/

#ifndef SB_SAND_BOX_H
#define SB_SAND_BOX_H

#include <Asclepias/Asclepias.h>

using namespace ASC;

class Title : public Scene {
public:
	bool init() override;
	void enter(Scene&) override;
	void draw() override;
	bool processInput() override;
	void msgReceived(NetMsg&) override;
	void cmptEvent(unsigned int, unsigned int, unsigned int) override;
	void update(float) override;
	void leave(Scene&) override;
	void destroy() override;

private:
	Sprite bg_;
	UIGroup mainUI_;
	Button playBtn_;
	Button optionsBtn_;
	Button quitBtn_;
	bool quit_ = false;
};

class Options : public Scene {
public:
	static glm::ivec2 WINDOW_DIMS;
	static bool FULLSCREEN;
	static float VOLUME;
	static std::string USERNAME;
	static std::string ADDRESS;
	static unsigned short PORT;

	static void LoadOptions();
	bool init() override;
	void enter(Scene&) override;
	void draw() override;
	bool processInput() override;
	void msgReceived(NetMsg&) override;
	void cmptEvent(unsigned int, unsigned int, unsigned int) override;
	void update(float) override;
	void leave(Scene&) override;
	void destroy() override;

private:
	UIGroup mainUI_;
	Button backBtn_;
	Button defaultsBtn_;
	Button applyBtn_;
	UIGroup optionsUI_;
	Cycle windowDimsCyc_;
	Switch fullscreenSw_;
	Slider volumeSl_;
	TextBox usernameBox_;
	TextBox addressBox_;
	TextBox portBox_;

	void saveOptions();
};

class SandBox {
public:
	static Title TITLE_SCENE;
	static Options OPTIONS_SCENE;
	static Font& FONT;
	static float TEXT_SCALE;
	static glm::vec3 TEXT_COLOR;
	static GLuint LABEL_TEX;
	static GLuint BTN_TEX;
	static GLuint SW_TEX;
	static GLuint SL_TEX;
	static GLuint CURSOR_TEX;
	static float CURSOR_WIDTH;
	static GLuint CYC_TEX;
	static GLuint BOX_TEX;
};

#endif