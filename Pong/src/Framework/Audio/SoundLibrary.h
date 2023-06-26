#pragma once
#include <AL\al.h>
#include <vector>

#define SFX_LOAD SoundLibrary::get()->load
#define SFX_UNLOAD SoundLibrary::get()->unload

class SoundLibrary
{
public:
	static SoundLibrary* get();

	ALuint load(const char* fileName);
	bool unload(const ALuint& buffer);

private:
	SoundLibrary();
	~SoundLibrary();

	std::vector<ALuint> p_SoundBuffers;
};
