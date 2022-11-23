/*
* File:		SandBox.h
* Author:	NetSkelter
* Created:	2022.11.22
*/

#ifndef SB_SAND_BOX_H
#define SB_SAND_BOX_H

#include <Asclepias/Asclepias.h>

using namespace ASC;

class TestScene : public Scene {
public:
	bool init() override;
	void enter(Scene&) override;
	void draw() override;
	bool processInput() override;
	void cmpEvent(int, int, int) override;
	void update(float) override;
	void leave(Scene&) override;
	void destroy() override;

private:
	double startTime_ = 0.0;
	bool chSize_ = false, chTitle_ = false, chFullscreen_ = false, chFullscreen2_ = false;
};

class SandBox {
public:
	static TestScene TS;
};

#endif