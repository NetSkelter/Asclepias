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
		glfwSetCursorPosCallback(App::window().window_, MousePosEvent);
		glfwSetMouseButtonCallback(App::window().window_, MouseBtnEvent);
		glfwSetScrollCallback(App::window().window_, MouseScrollEvent);
	}

	void InputMgr::update() {
		for (std::pair<const int, std::pair<bool, bool>>& k : keys_) {
			k.second.second = k.second.first;
		}
		mousePos_.second = mousePos_.first;
		for (std::pair<const int, std::pair<bool, bool>>& b : mouseBtns_) {
			b.second.second = b.second.first;
		}
		mouseScroll_.second = mouseScroll_.first;
		mouseScroll_.first = glm::vec2(0.0f, 0.0f);
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
		mouseBtns_.clear();
	}

	bool InputMgr::isKeyDown(int key) const {
		if (keys_.find(key) == keys_.end()) {
			return false;
		}
		return keys_.at(key).first;
	}

	void InputMgr::setMouseEnabled(bool enabled) {
		if (enabled && !mouseEnabled_) {
			if (mouseVisible_) {
				glfwSetInputMode(App::window().window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
			else {
				glfwSetInputMode(App::window().window_, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			}
		}
		else if (!enabled && mouseEnabled_) {
			glfwSetInputMode(App::window().window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		mouseEnabled_ = enabled;
	}

	void InputMgr::setMouseVisible(bool visible) {
		if (visible && !mouseVisible_) {
			if (mouseEnabled_) {
				glfwSetInputMode(App::window().window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
			else {
				glfwSetInputMode(App::window().window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
		}
		else if (!visible && mouseVisible_) {
			if (mouseEnabled_) {
				glfwSetInputMode(App::window().window_, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			}
			else {
				glfwSetInputMode(App::window().window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
		}
		mouseVisible_ = visible;
	}

	bool InputMgr::isMouseBtnDown(int btn) const {
		if (mouseBtns_.find(btn) == mouseBtns_.end()) {
			return false;
		}
		return mouseBtns_.at(btn).first;
	}

	bool InputMgr::wasKeyDown(int key) const {
		if (keys_.find(key) == keys_.end()) {
			return false;
		}
		return keys_.at(key).second;
	}

	bool InputMgr::wasMouseBtnDown(int btn) const {
		if (mouseBtns_.find(btn) == mouseBtns_.end()) {
			return false;
		}
		return mouseBtns_.at(btn).second;
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
			keys_[key] = std::pair<bool, bool>(false, true);
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

	void InputMgr::moveMouse(const glm::vec2& pos) {
		if (!mouseEnabled_) {
			return;
		}
		mousePos_.first = pos;
		for (InputLstr* lstr : lstrs_) {
			lstr->mouseMoved(pos);
		}
	}

	void InputMgr::pressMouseBtn(int btn) {
		if (!mouseEnabled_) {
			return;
		}
		if (mouseBtns_.find(btn) == mouseBtns_.end()) {
			mouseBtns_[btn] = std::pair<bool, bool>(true, false);
		}
		mouseBtns_[btn].first = true;
		for (InputLstr* lstr : lstrs_) {
			lstr->mouseBtnPressed(btn);
		}
	}

	void InputMgr::releaseMouseBtn(int btn) {
		if (!mouseEnabled_) {
			return;
		}
		if (mouseBtns_.find(btn) == mouseBtns_.end()) {
			mouseBtns_[btn] = std::pair<bool, bool>(false, true);
		}
		mouseBtns_[btn].first = false;
		for (InputLstr* lstr : lstrs_) {
			lstr->mouseBtnReleased(btn);
		}
	}

	void InputMgr::scrollMouse(const glm::vec2& scroll) {
		if (!mouseEnabled_) {
			return;
		}
		mouseScroll_.first = scroll;
		for (InputLstr* lstr : lstrs_) {
			lstr->mouseScrolled(scroll);
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

	void InputMgr::MousePosEvent(GLFWwindow* window, double x, double y) {
		App::input().moveMouse(glm::vec2(x, App::window().getDims().y - y));
	}

	void InputMgr::MouseBtnEvent(GLFWwindow* window, int button, int action, int mods) {
		if (action == GLFW_PRESS) {
			App::input().pressMouseBtn(button);
		}
		else if (action == GLFW_RELEASE) {
			App::input().releaseMouseBtn(button);
		}
	}

	void InputMgr::MouseScrollEvent(GLFWwindow* window, double x, double y) {
		App::input().scrollMouse(glm::vec2(x, y));
	}
}