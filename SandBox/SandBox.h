/*
* File:		SandBox.h
* Author:	NetSkelter
* Created:	2022.11.22
*/

#ifndef SB_SAND_BOX_H
#define SB_SAND_BOX_H

#include <Asclepias/Asclepias.h>

using namespace ASC;

class TestScene1 : public Scene {
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
	UIGroup UI_;
	Label lbl_;
	Button btn_;
	Switch sw_;
	Slider sl_;
	Cycle cyc_;
	TextBox box_;
};

class TestScene2 : public Scene {
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
};

class SandBox {
public:
	static TestScene1 TS1;
	static TestScene2 TS2;
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