/*
* File:		Input.h
* Author:	NetSkelter
* Created:	2022.11.23
*/

#ifndef ASC_INPUT_H
#define ASC_INPUT_H

#include <map>
#include <vector>

#include "Windowing.h"

namespace ASC {
	enum InputCodes {
		ASC_UNKNOWN = GLFW_KEY_UNKNOWN,

		ASC_SYM_GRAVE = GLFW_KEY_GRAVE_ACCENT, ASC_SYM_MINUS = GLFW_KEY_MINUS,
		ASC_SYM_EQUALS = GLFW_KEY_EQUAL, ASC_SYM_L_BRACKET = GLFW_KEY_LEFT_BRACKET,
		ASC_SYM_R_BRACKET = GLFW_KEY_RIGHT_BRACKET, ASC_SYM_BACKSLASH = GLFW_KEY_BACKSLASH,
		ASC_SYM_SEMICOLON = GLFW_KEY_SEMICOLON, ASC_SYM_APOSTROPHE = GLFW_KEY_APOSTROPHE,
		ASC_SYM_COMMA = GLFW_KEY_COMMA, ASC_SYM_PERIOD = GLFW_KEY_PERIOD,
		ASC_SYM_SLASH = GLFW_KEY_SLASH, ASC_SYM_SPACE = GLFW_KEY_SPACE,
		ASC_SYM_WORLD_1 = GLFW_KEY_WORLD_1, ASC_SYM_WORLD_2 = GLFW_KEY_WORLD_2,
		ASC_SYM_KP_DECIMAL = GLFW_KEY_KP_DECIMAL, ASC_SYM_KP_DIVIDE = GLFW_KEY_KP_DIVIDE,
		ASC_SYM_KP_MULTIPLY = GLFW_KEY_KP_MULTIPLY, ASC_SYM_KP_SUBTRACT = GLFW_KEY_KP_SUBTRACT,
		ASC_SYM_KP_ADD = GLFW_KEY_KP_ADD, ASC_SYM_KP_ENTER = GLFW_KEY_KP_ENTER,
		ASC_SYM_KP_EQUALS = GLFW_KEY_KP_EQUAL,

		ASC_KEY_0 = GLFW_KEY_0, ASC_KEY_1 = GLFW_KEY_1, ASC_KEY_2 = GLFW_KEY_2,
		ASC_KEY_3 = GLFW_KEY_3, ASC_KEY_4 = GLFW_KEY_4, ASC_KEY_5 = GLFW_KEY_5,
		ASC_KEY_6 = GLFW_KEY_6, ASC_KEY_7 = GLFW_KEY_7, ASC_KEY_8 = GLFW_KEY_8,
		ASC_KEY_9 = GLFW_KEY_9,
		ASC_KEY_KP_0 = GLFW_KEY_KP_0, ASC_KEY_KP_1 = GLFW_KEY_KP_1, ASC_KEY_KP_2 = GLFW_KEY_KP_2,
		ASC_KEY_KP_3 = GLFW_KEY_KP_3, ASC_KEY_KP_4 = GLFW_KEY_KP_4, ASC_KEY_KP_5 = GLFW_KEY_KP_5,
		ASC_KEY_KP_6 = GLFW_KEY_KP_6, ASC_KEY_KP_7 = GLFW_KEY_KP_7, ASC_KEY_KP_8 = GLFW_KEY_KP_8,
		ASC_KEY_KP_9 = GLFW_KEY_KP_9,

		ASC_KEY_A = GLFW_KEY_A, ASC_KEY_B = GLFW_KEY_B, ASC_KEY_C = GLFW_KEY_C,
		ASC_KEY_D = GLFW_KEY_D, ASC_KEY_E = GLFW_KEY_E, ASC_KEY_F = GLFW_KEY_F,
		ASC_KEY_G = GLFW_KEY_G, ASC_KEY_H = GLFW_KEY_H, ASC_KEY_I = GLFW_KEY_I,
		ASC_KEY_J = GLFW_KEY_J, ASC_KEY_K = GLFW_KEY_K, ASC_KEY_L = GLFW_KEY_L,
		ASC_KEY_M = GLFW_KEY_M, ASC_KEY_N = GLFW_KEY_N, ASC_KEY_O = GLFW_KEY_O,
		ASC_KEY_P = GLFW_KEY_P, ASC_KEY_Q = GLFW_KEY_Q, ASC_KEY_R = GLFW_KEY_R,
		ASC_KEY_S = GLFW_KEY_S, ASC_KEY_T = GLFW_KEY_T, ASC_KEY_U = GLFW_KEY_U,
		ASC_KEY_V = GLFW_KEY_V, ASC_KEY_W = GLFW_KEY_W, ASC_KEY_X = GLFW_KEY_X,
		ASC_KEY_Y = GLFW_KEY_Y, ASC_KEY_Z = GLFW_KEY_Z,

		ASC_CTL_ESC = GLFW_KEY_ESCAPE, ASC_CTL_ENTER = GLFW_KEY_ENTER, ASC_CTL_TAB = GLFW_KEY_TAB,
		ASC_CTL_BACKSPACE = GLFW_KEY_BACKSPACE, ASC_CTL_INSERT = GLFW_KEY_INSERT,
		ASC_CTL_DELETE = GLFW_KEY_DELETE, ASC_CTL_LEFT = GLFW_KEY_LEFT, ASC_CTL_RIGHT = GLFW_KEY_RIGHT,
		ASC_CTL_UP = GLFW_KEY_UP, ASC_CTL_DOWN = GLFW_KEY_DOWN, ASC_CTL_PG_UP = GLFW_KEY_PAGE_UP,
		ASC_CTL_PG_DOWN = GLFW_KEY_PAGE_DOWN, ASC_CTL_HOME = GLFW_KEY_HOME, ASC_CTL_END = GLFW_KEY_END,
		ASC_CTL_CAPS_LOCK = GLFW_KEY_CAPS_LOCK, ASC_CTL_PRINTSC = GLFW_KEY_PRINT_SCREEN,
		ASC_CTL_PAUSE = GLFW_KEY_PAUSE,
		ASC_CTL_F1 = GLFW_KEY_F1, ASC_CTL_F2 = GLFW_KEY_F2, ASC_CTL_F3 = GLFW_KEY_F3,
		ASC_CTL_F4 = GLFW_KEY_F4, ASC_CTL_F5 = GLFW_KEY_F5, ASC_CTL_F6 = GLFW_KEY_F6,
		ASC_CTL_F7 = GLFW_KEY_F7, ASC_CTL_F8 = GLFW_KEY_F8, ASC_CTL_F9 = GLFW_KEY_F9,
		ASC_CTL_F10 = GLFW_KEY_F10, ASC_CTL_F11 = GLFW_KEY_F11, ASC_CTL_F12 = GLFW_KEY_F12,
		ASC_CTL_F13 = GLFW_KEY_F13, ASC_CTL_F14 = GLFW_KEY_F14, ASC_CTL_F15 = GLFW_KEY_F15,
		ASC_CTL_F16 = GLFW_KEY_F16, ASC_CTL_F17 = GLFW_KEY_F17, ASC_CTL_F18 = GLFW_KEY_F18,
		ASC_CTL_F19 = GLFW_KEY_F19, ASC_CTL_F20 = GLFW_KEY_F20, ASC_CTL_F21 = GLFW_KEY_F21,
		ASC_CTL_F22 = GLFW_KEY_F22, ASC_CTL_F23 = GLFW_KEY_F23, ASC_CTL_F24 = GLFW_KEY_F24,
		ASC_CTL_F25 = GLFW_KEY_F25,
		ASC_CTL_L_SHIFT = GLFW_KEY_LEFT_SHIFT, ASC_CTL_L_CTRL = GLFW_KEY_LEFT_CONTROL,
		ASC_CTL_L_ALT = GLFW_KEY_LEFT_ALT, ASC_CTL_L_SUPER = GLFW_KEY_LEFT_SUPER,
		ASC_CTL_R_SHIFT = GLFW_KEY_RIGHT_SHIFT, ASC_CTL_R_CTRL = GLFW_KEY_RIGHT_CONTROL,
		ASC_CTL_R_ALT = GLFW_KEY_RIGHT_ALT, ASC_CTL_R_SUPER = GLFW_KEY_RIGHT_SUPER,
		ASC_CTL_MENU = GLFW_KEY_MENU, ASC_CTL_LAST = GLFW_KEY_LAST,

		ASC_MB_1 = GLFW_MOUSE_BUTTON_1, ASC_MB_2 = GLFW_MOUSE_BUTTON_2, ASC_MB_3 = GLFW_MOUSE_BUTTON_3,
		ASC_MB_4 = GLFW_MOUSE_BUTTON_4, ASC_MB_5 = GLFW_MOUSE_BUTTON_5, ASC_MB_6 = GLFW_MOUSE_BUTTON_6,
		ASC_MB_7 = GLFW_MOUSE_BUTTON_7, ASC_MB_8 = GLFW_MOUSE_BUTTON_8,
		ASC_MB_LEFT = GLFW_MOUSE_BUTTON_LEFT, ASC_MB_MID = GLFW_MOUSE_BUTTON_MIDDLE,
		ASC_MB_RIGHT = GLFW_MOUSE_BUTTON_RIGHT, ASC_MB_LAST = GLFW_MOUSE_BUTTON_LAST,

		ASC_CB_A = GLFW_GAMEPAD_BUTTON_A, ASC_CB_B = GLFW_GAMEPAD_BUTTON_B,
		ASC_CB_X = GLFW_GAMEPAD_BUTTON_X, ASC_CB_Y = GLFW_GAMEPAD_BUTTON_Y,
		ASC_CB_CROSS = GLFW_GAMEPAD_BUTTON_CROSS, ASC_CB_CIRCLE = GLFW_GAMEPAD_BUTTON_CIRCLE,
		ASC_CB_SQUARE = GLFW_GAMEPAD_BUTTON_SQUARE, ASC_CB_TRIANGLE = GLFW_GAMEPAD_BUTTON_TRIANGLE,
		ASC_CB_L_BUMPER = GLFW_GAMEPAD_BUTTON_LEFT_BUMPER, ASC_CB_R_BUMPER = GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER,
		ASC_CB_L_THUMB = GLFW_GAMEPAD_BUTTON_LEFT_THUMB, ASC_CB_R_THUMB = GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER,
		ASC_CB_DP_UP = GLFW_GAMEPAD_BUTTON_DPAD_UP, ASC_CB_DP_DOWN = GLFW_GAMEPAD_BUTTON_DPAD_DOWN,
		ASC_CB_DP_LEFT = GLFW_GAMEPAD_BUTTON_DPAD_LEFT, ASC_CB_DP_RIGHT = GLFW_GAMEPAD_BUTTON_DPAD_RIGHT,
		ASC_CB_BACK = GLFW_GAMEPAD_BUTTON_BACK, ASC_CB_START = GLFW_GAMEPAD_BUTTON_START,
		ASC_CB_GUIDE = GLFW_GAMEPAD_BUTTON_GUIDE, ASC_CB_LAST = GLFW_GAMEPAD_BUTTON_LAST,

		ASC_CA_L_STICK_X = GLFW_GAMEPAD_AXIS_LEFT_X, ASC_CA_L_STICK_Y = GLFW_GAMEPAD_AXIS_LEFT_Y,
		ASC_CA_R_STICK_X = GLFW_GAMEPAD_AXIS_RIGHT_X, ASC_CA_R_STICK_Y = GLFW_GAMEPAD_AXIS_RIGHT_Y,
		ASC_CA_L_TRIGGER = GLFW_GAMEPAD_AXIS_LEFT_TRIGGER, ASC_CA_R_TRIGGER = GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER,
		ASC_CA_LAST = GLFW_GAMEPAD_AXIS_LAST,
	};

	class InputLstr {
	public:
		virtual void keyPressed(int) {}
		virtual void keyReleased(int) {}
		virtual void charTyped(char) {}
		virtual void mouseMoved(const glm::vec2&) {}
		virtual void mouseBtnPressed(int) {}
		virtual void mouseBtnReleased(int) {}
		virtual void mouseScrolled(const glm::vec2&) {}
		virtual void ctrlConnected(int) {}
		virtual void ctrlDisconnected(int) {}
		virtual void ctrlBtnPressed(int, int) {}
		virtual void ctrlBtnReleased(int, int) {}
		virtual void ctrlAxisMoved(int, int, float) {}
	};

	class InputMgr {
	public:
		void init();
		void update();
		bool addLstr(InputLstr&);
		bool removeLstr(InputLstr&);
		void destroy();
		bool isKeyDown(int) const;
		inline bool isKeyPressed(int key) const {
			return isKeyDown(key) && !wasKeyDown(key);
		}
		inline bool isKeyReleased(int key) const {
			return !isKeyDown(key) && wasKeyDown(key);
		}
		inline bool isMouseEnabled() const {
			return mouseEnabled_;
		}
		void setMouseEnabled(bool);
		inline bool isMouseVisible() const {
			return mouseVisible_;
		}
		void setMouseVisible(bool);
		inline bool isMouseMoved() const {
			return mousePos_.first != mousePos_.second;
		}
		inline const glm::vec2& getMousePos() const {
			return mousePos_.first;
		}
		glm::vec2 getMousePos(const Camera&) const;
		bool isMouseBtnDown(int) const;
		inline bool isMouseBtnPressed(int btn) const {
			return isMouseBtnDown(btn) && !wasMouseBtnDown(btn);
		}
		inline bool isMouseBtnReleased(int btn) const {
			return !isMouseBtnDown(btn) && wasMouseBtnDown(btn);
		}
		inline bool isMouseScrolled() const {
			return (mouseScroll_.first != mouseScroll_.second)
				&& (mouseScroll_.first != glm::vec2(0.0f, 0.0f));
		}
		inline const glm::vec2& getMouseScroll() const {
			return mouseScroll_.first;
		}
		std::vector<int> getCtrlIDs() const;
		bool isCtrlConnected(int) const;
		bool isCtrlBtnDown(int, int) const;
		inline bool isCtrlBtnPressed(int ctrl, int btn) const {
			return isCtrlBtnDown(ctrl, btn) && !wasCtrlBtnDown(ctrl, btn);
		}
		inline bool isCtrlBtnReleased(int ctrl, int btn) const {
			return !isCtrlBtnDown(ctrl, btn) && wasCtrlBtnDown(ctrl, btn);
		}
		bool isCtrlAxisMoved(int, int) const;
		float getCtrlAxisPos(int, int) const;

	private:
		std::vector<InputLstr*> lstrs_;
		std::map<int, std::pair<bool, bool>> keys_;
		bool mouseEnabled_ = true;
		bool mouseVisible_ = true;
		std::pair<glm::vec2, glm::vec2> mousePos_;
		std::map<int, std::pair<bool, bool>> mouseBtns_;
		std::pair<glm::vec2, glm::vec2> mouseScroll_;
		std::map<int, std::pair<GLFWgamepadstate, GLFWgamepadstate>> ctrls_;
		std::vector<int> removedCtrls_;

		bool wasKeyDown(int) const;
		bool wasMouseBtnDown(int) const;
		bool wasCtrlBtnDown(int, int) const;
		void pressKey(int);
		void releaseKey(int);
		void typeChar(char);
		void moveMouse(const glm::vec2&);
		void pressMouseBtn(int);
		void releaseMouseBtn(int);
		void scrollMouse(const glm::vec2&);
		void connectCtrl(int);
		void disconnectCtrl(int);
		static void KeyEvent(GLFWwindow*, int, int, int, int);
		static void CharEvent(GLFWwindow*, unsigned int);
		static void MousePosEvent(GLFWwindow*, double, double);
		static void MouseBtnEvent(GLFWwindow*, int, int, int);
		static void MouseScrollEvent(GLFWwindow*, double, double);
		static void CtrlEvent(int, int);
	};
}

#endif