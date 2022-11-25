/*
* File:		Audio.h
* Author:	NetSkelter
* Created:	2022.11.24
*/

#ifndef ASC_AUDIO_H
#define ASC_AUDIO_H

#include <SoLoud/soloud.h>
#include <SoLoud/soloud_wav.h>
#include <map>
#include <string>

namespace ASC {
	class AudioMgr {
	public:
		bool init(float);
		bool playEffect(const std::string&);
		bool playMusic(const std::string & = "");
		bool pauseMusic();
		inline bool isMusicPaused() {
			return musicPaused_;
		}
		void stopMusic();
		void destroy();
		inline float getVolume() const {
			return audio_.getGlobalVolume();
		}
		void setVolume(float);

	private:
		SoLoud::Soloud audio_;
		std::map<std::string, SoLoud::Wav> effects_;
		SoLoud::handle music_ = 0;
		bool musicPaused_ = false;

		bool loadEffect(const std::string&);
	};
}

#endif