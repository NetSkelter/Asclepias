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
	void processInput() override;
	void cmpEvent(int, int, int) override;
	void update(float) override;
	void leave(Scene&) override;
	void destroy() override;
};

class SandBox {
public:
	static TestScene TS;
};

#endif