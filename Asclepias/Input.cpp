/*
* File:		Input.cpp
* Author:	NetSkelter
* Created:	2022.11.23
*/

#include "Asclepias.h"

namespace ASC {
	void InputMgr::init() {
		glfwSetKeyCallback(App::window().window_, KeyEvent);
		glfwSetCharCallback(App::window().window_, CharEvent);
	}

	void InputMgr::update() {
		for (std::pair<const int, std::pair<bool, bool>>& k : keys_) {
			k.second.second = k.second.first;
		}
		glfwPollEvents();
	}

	bool InputMgr::addLstr(InputLstr& lstr) {
		if (std::find(lstrs_.begin(), lstrs_.end(), &lstr) != lstrs_.end()) {
			return false;
		}
		lstrs_.push_back(&lstr);
		ASCLOG(Input, Info, "Added input listener at ", &lstr, ".");
		return true;
	}

	bool InputMgr::removeLstr(InputLstr& lstr) {
		std::vector<InputLstr*>::iterator it = std::find(lstrs_.begin(), lstrs_.end(), &lstr);
		if (it == lstrs_.end()) {
			return false;
		}
		lstrs_.erase(it);
		ASCLOG(Input, Info, "Removed input listener at ", &lstr, ".");
		return true;
	}

	void InputMgr::destroy() {
		lstrs_.clear();
		keys_.clear();
	}

	bool InputMgr::isKeyDown(int key) const {
		if (keys_.find(key) == keys_.end()) {
			return false;
		}
		return keys_.at(key).first;
	}

	bool InputMgr::wasKeyDown(int key) const {
		if (keys_.find(key) == keys_.end()) {
			return false;
		}
		return keys_.at(key).second;
	}

	void InputMgr::pressKey(int key) {
		if (keys_.find(key) == keys_.end()) {
			keys_[key] = std::pair<bool, bool>(true, false);
		}
		keys_[key].first = true;
		for (InputLstr* lstr : lstrs_) {
			lstr->keyPressed(key);
		}
	}

	void InputMgr::releaseKey(int key) {
		if (keys_.find(key) == keys_.end()) {
			keys_[key] = std::pair<bool, bool>(false, false);
		}
		keys_[key].first = false;
		for (InputLstr* lstr : lstrs_) {
			lstr->keyReleased(key);
		}
	}

	void InputMgr::typeChar(char c) {
		for (InputLstr* lstr : lstrs_) {
			lstr->charTyped(c);
		}
	}

	void InputMgr::KeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (action == GLFW_PRESS) {
			App::input().pressKey(key);
		}
		else if (action == GLFW_RELEASE) {
			App::input().releaseKey(key);
		}
	}

	void InputMgr::CharEvent(GLFWwindow* window, unsigned int codepoint) {
		App::input().typeChar((char)codepoint);
	}
}