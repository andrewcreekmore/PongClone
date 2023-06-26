#include "SoundLibrary.h"
#include <sndfile.h>
#include <inttypes.h>
#include <AL\alext.h>


// singleton getter method; returns a pointer to the only instantiation allowed
SoundLibrary* SoundLibrary::get()
{
	static SoundLibrary* sndbuf = new SoundLibrary();
	return sndbuf;
}


// loads sound file into memory; returns buffer ID for accessing
ALuint SoundLibrary::load(const char* fileName)
{
	ALenum err, format;
	ALuint buffer;
	SNDFILE* sndFile;
	SF_INFO sfInfo;
	short* memoryBuffer;
	sf_count_t numFrames;
	ALsizei numBytes;

	// open the audio file and check that it's usable
	sndFile = sf_open(fileName, SFM_READ, &sfInfo);

	// ensure doesn't fail to open
	if (!sndFile)
	{
		fprintf(stderr, "Could not open audio in %s: %s\n", fileName, sf_strerror(sndFile));
		return 0;
	}

	// ensure has frames
	if (sfInfo.frames < 1 || sfInfo.frames >(sf_count_t)(INT_MAX / sizeof(short)) / sfInfo.channels)
	{
		fprintf(stderr, "Bad sample count in %s (%" PRId64 ")\n", fileName, sfInfo.frames);
		sf_close(sndFile);
		return 0;
	}

	// get the sound format + figure out the OpenAL format
	format = AL_NONE;
	if (sfInfo.channels == 1)
	{ format = AL_FORMAT_MONO16; }
	else if (sfInfo.channels == 2)
	{ format = AL_FORMAT_STEREO16; }
	else if (sfInfo.channels == 3)
	{
		if (sf_command(sndFile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
		{ format = AL_FORMAT_BFORMAT2D_16; }
	}
	else if (sfInfo.channels == 4)
	{
		if (sf_command(sndFile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
		{ format = AL_FORMAT_BFORMAT3D_16; }
	}
	if (!format)
	{
		fprintf(stderr, "Unsupported channel count: %d\n", sfInfo.channels);
		sf_close(sndFile);
		return 0;
	}

	// decode the whole audio file to a buffer
	memoryBuffer = static_cast<short*>(malloc((size_t)(sfInfo.frames * sfInfo.channels) * sizeof(short)));

	numFrames = sf_readf_short(sndFile, memoryBuffer, sfInfo.frames);
	if (numFrames < 1)
	{
		free(memoryBuffer);
		sf_close(sndFile);
		fprintf(stderr, "Failed to read samples in %s (%" PRId64 ")\n", fileName, numFrames);
		return 0;
	}
	numBytes = (ALsizei)(numFrames * sfInfo.channels) * (ALsizei)sizeof(short);

	// buffer the audio data into a new buffer object, free the data, and close the file
	buffer = 0;
	alGenBuffers(1, &buffer);
	alBufferData(buffer, format, memoryBuffer, numBytes, sfInfo.samplerate);

	free(memoryBuffer);
	sf_close(sndFile);

	// check if an error occurred + clean up if so
	err = alGetError();
	if (err != AL_NO_ERROR)
	{
		fprintf(stderr, "OpenAL Error: %s\n", alGetString(err));
		if (buffer && alIsBuffer(buffer)) { alDeleteBuffers(1, &buffer); }
		return 0;
	}

	// add to the list of known buffers
	p_SoundBuffers.push_back(buffer);

	return buffer;
}


// searches for and removes audio file from memory (if found)
bool SoundLibrary::unload(const ALuint& buffer)
{
	auto it = p_SoundBuffers.begin();
	while (it != p_SoundBuffers.end())
	{
		if (*it == buffer) // if found
		{
			// delete buffer
			alDeleteBuffers(1, &*it);

			// remove from buffer tracking vector
			it = p_SoundBuffers.erase(it);

			return true;
		}

		else
		{
			++it;
		} // traverse
	}

	return false;  // couldn't find to remove
}


// initialize/clear buffer
SoundLibrary::SoundLibrary()
{
	p_SoundBuffers.clear();
}


// removes all loaded sounds from memory
SoundLibrary::~SoundLibrary()
{
	alDeleteBuffers(static_cast<ALsizei>(p_SoundBuffers.size()), p_SoundBuffers.data());

	p_SoundBuffers.clear();
}
