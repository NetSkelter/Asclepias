/*
* File:		Asclepias.h
* Author:	NetSkelter
* Created:	2022.11.22
*/

#ifndef ASC_ASCLEPIAS_H
#define ASC_ASCLEPIAS_H

#include "Logging.h"
#include "Windowing.h"
#include "Input.h"
#include "Audio.h"
#include "Networking.h"

namespace ASC {
	class Scene : public InputLstr {
	protected:
		friend class App;
		
		bool initScene();
		virtual bool init() = 0;
		virtual void enter(Scene&) = 0;
		virtual void draw() = 0;
		virtual bool processInput() = 0;
		virtual void cmpEvent(int, int, int) = 0;
		virtual void update(float) = 0;
		virtual void leave(Scene&) = 0;
		virtual void destroy() = 0;
		void destroyScene();

	private:
		bool initialized_ = false;
	};

	class App {
	public:
		struct Config {
			struct Log {
				bool console
#ifdef _DEBUG
					= true;
#else
					= false;
#endif
				std::vector<std::string> fileNames = { "ASC.log" };
				std::string timestampFmt = "%Y.%m.%d.%H%M.%S";
			} log;
			struct Window {
				glm::ivec2 dims = glm::ivec2(800, 600);
				std::string title = "Asclepias";
				bool fullscreen = false;
			} window;
			struct Audio {
				float volume = 1.0f;
			} audio;
			Scene& startScene;

			Config(Scene&);
		};
		static bool Init(const Config&);
		static void SetScene(Scene&);
		static void Run();
		static void Destroy();
		inline static LogMgr& log() {
			return inst_->log_;
		}
		inline static WindowMgr& window() {
			return inst_->window_;
		}
		inline static InputMgr& input() {
			return inst_->input_;
		}
		inline static AudioMgr& audio() {
			return inst_->audio_;
		}
		inline static NetClient& network() {
			return inst_->network_;
		}

	private:
		static App* inst_;
		LogMgr log_;
		WindowMgr window_;
		InputMgr input_;
		AudioMgr audio_;
		NetClient network_;
		std::vector<Scene*> scenes_;
		Scene* scene_ = 0;

		App() {}
		App(const App&) = delete;
		App& operator = (const App&) = delete;
	};
}

#endif