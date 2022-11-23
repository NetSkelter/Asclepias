/*
* File:		Asclepias.h
* Author:	NetSkelter
* Created:	2022.11.22
*/

#ifndef ASC_ASCLEPIAS_H
#define ASC_ASCLEPIAS_H

#include <vector>

namespace ASC {
	class Scene {
	protected:
		friend class App;
		
		bool initScene();
		virtual bool init() = 0;
		virtual void enter(Scene&) = 0;
		virtual void draw() = 0;
		virtual void processInput() = 0;
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
			Scene& startScene;

			Config(Scene&);
		};
		static bool Init(const Config&);
		static void SetScene(Scene&);
		static void Run();
		static void Destroy();

	private:
		static App* inst_;
		std::vector<Scene*> scenes_;
		Scene* scene_ = 0;

		App() {}
		App(const App&) = delete;
		App& operator = (const App&) = delete;
	};
}

#endif