/*
* File:		Asclepias.cpp
* Author:	NetSkelter
* Created:	2022.11.22
*/

#include "Asclepias.h"

namespace ASC {
	bool Scene::initScene() {
		if (initialized_) {
			return true;
		}
		initialized_ = init();
		return initialized_;
	}

	void Scene::destroyScene() {
		if (!initialized_) {
			return;
		}
		initialized_ = false;
		destroy();
	}

	App::Config::Config(Scene& scene) : startScene(scene) {}

	App* App::inst_ = 0;

	bool App::Init(const App::Config& conf) {
		if (inst_ != 0) {
			return false;
		}
		inst_ = new App;

		inst_->log_.init(conf.log.console, conf.log.fileNames, conf.log.timestampFmt);
		ASCLOG(App, Info, "Initialized logging system.");

		inst_->scene_ = &conf.startScene;
		return true;
	}

	void App::SetScene(Scene& scene) {
		if (std::find(inst_->scenes_.begin(), inst_->scenes_.end(), &scene)
			== inst_->scenes_.end()) {
			inst_->scenes_.push_back(&scene);
		}
		if (inst_->scene_ != 0 && inst_->scene_ != &scene) {
			inst_->scene_->leave(scene);
		}
		if (scene.initScene()) {
			if (inst_->scene_ != 0) {
				scene.enter(*inst_->scene_);
			}
			else {
				scene.enter(scene);
			}
			inst_->scene_ = &scene;
		}
		ASCLOG(App, Info, "Set to scene at ", &scene, ".");
	}

	void App::Run() {
		SetScene(*inst_->scene_);
	}

	void App::Destroy() {
		inst_->scene_->leave(*inst_->scene_);
		for (Scene* scene : inst_->scenes_) {
			scene->destroyScene();
		}
		inst_->scenes_.clear();
		inst_->scene_ = 0;

		ASCLOG(App, Info, "Destroying logging system.");
		inst_->log_.destroy();

		delete inst_;
	}
}