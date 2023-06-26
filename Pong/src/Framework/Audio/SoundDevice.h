#pragma once
#include <AL\alc.h>

#define SD_INIT SoundDevice::init();
#define LISTENER SoundDevice::get()

class SoundDevice
{
public:

	static SoundDevice* get();
	static void init();

	float getGain();
	void setGain(const float& val);

private:

	SoundDevice();
	~SoundDevice();

	ALCdevice* p_ALCDevice;
	ALCcontext* p_ALCContext;
};

