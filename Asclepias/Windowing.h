/*
* File:		Windowing.h
* Author:	NetSkelter
* Created:	2022.11.23
*/

#ifndef ASC_WINDOWING_H
#define ASC_WINDOWING_H

#include <iostream>
#include <string>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace ASC {
	class WindowMgr {
	public:
		bool init(const glm::ivec2&, const std::string&, bool);
		bool update();
		void destroy();
		inline const glm::ivec2& getDims() const {
			return dims_;
		}
		void setDims(const glm::ivec2&);
		inline const std::string& getTitle() const {
			return title_;
		}
		void setTitle(const std::string&);
		inline bool isFullscreen() const {
			return fullscreen_;
		}
		void setFullscreen(bool);
		void setMonitor(GLFWmonitor*);

	private:
		glm::ivec2 dims_ = glm::ivec2();
		glm::ivec2 wDims_ = glm::ivec2();
		std::string title_ = "";
		bool fullscreen_ = false;
		GLFWmonitor* monitor_ = 0;
		GLFWwindow* window_ = 0;

		void center();
	};
}

#endif