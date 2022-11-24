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
	void cmpEvent(int, int, int) override;
	void update(float) override;
	void leave(Scene&) override;
	void destroy() override;
};

class TestScene2 : public Scene {
public:
	bool init() override;
	void enter(Scene&) override;
	void draw() override;
	bool processInput() override;
	void cmpEvent(int, int, int) override;
	void update(float) override;
	void leave(Scene&) override;
	void destroy() override;
	void mouseMoved(const glm::vec2&) override;
	void mouseBtnPressed(int) override;
	void mouseBtnReleased(int) override;
	void mouseScrolled(const glm::vec2&) override;
};

class SandBox {
public:
	static TestScene1 TS1;
	static TestScene2 TS2;
};

#endif