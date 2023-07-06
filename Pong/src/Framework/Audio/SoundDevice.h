#pragma once
#include <AL\alc.h>

#define SD_INIT SoundDevice::init();
#define LISTENER SoundDevice::get()

/*
===========================================================================
SoundDevice: OpenAL implementation
- gets the default system sound device and sets up AL sound context
- provides methods for getting/setting device gain
- singleton
===========================================================================
*/

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

