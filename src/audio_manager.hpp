#ifndef AUDIO_MANAGER
#define AUDIO_MANAGER
#include <string>
class AudioManager{
public:
	~AudioManager();
	void init();
	void finish();
	void play();
	void play(std::string sound_path);
};

#endif
