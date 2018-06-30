#include <stdio.h>
#include "alc.h"
#include "al.h"
#include "audio.h"
#include <time.h>
#define DEFAULT_GAIN  .1f
#define DEFAULT_FREQ 440
static ALuint sid;
static int waveform;
static ALuint buffers[AUDIO_WAVEFORM_MAX];
static unsigned int length;
static unsigned int start;
static float decay;
static float gain;
static float sweep;
static float freqStart=DEFAULT_FREQ;
static float freq;
static float freqEnd;
int audioInit() {
	ALCdevice* device = alcOpenDevice(NULL);
	if (device == NULL)
		return 1;
	ALCcontext* context = alcCreateContext(
		device,
		NULL);
	if (context == NULL) 
		return 1;

	alcMakeContextCurrent(context);

	alGenBuffers(
		AUDIO_WAVEFORM_MAX,// ALsizei n
		buffers// ALuint* buffers
	);
	unsigned char pulse[][8] = {
		{0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00 }, 
		{0xff,0xff,0x00,0x00,0x00,0x00,0x00,0x00 },
		{0xff,0xff,0xff,0xff,0x00,0x00,0x00,0x00 },
		{0xff,0xff,0xff,0xff,0xff,0xff,0x00,0x00 } };
	for (int i = AUDIO_WAVEFORM_PULSE_12_5; i <= AUDIO_WAVEFORM_PULSE_75; i++) {
		ALsizei size = sizeof pulse[0];
		alBufferData(
			buffers[i], //ALuint bid
			AL_FORMAT_MONO8, //ALenum format
			pulse[i], //const ALvoid* data
			size, //ALsizei size
			size * DEFAULT_FREQ //ALsizei freq
		);
	}
	unsigned char triangle[] = {
		0xff,0xee,0xdd,0xcc, 0xbb,0xaa,0x99,0x88,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00,
		0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff
	};
	alBufferData(
		buffers[AUDIO_WAVEFORM_TRIANGLE],
		AL_FORMAT_MONO8,
		triangle,
		sizeof triangle,
		sizeof triangle*440
	);
	
	alGenSources(
		1, //ALsizein
		&sid //ALuint* sources
	);

	alSourcei(
		sid,
		AL_LOOPING,
		AL_TRUE);

	
	return 0;
}
void audioWaveform(int _waveform) {
	waveform = _waveform;
}
void audioLength(unsigned int _millis) {
	length = _millis;
}
void audioDecay(float _decay) {
	decay = _decay;
}
void audioSweep(float _sweep, float _freqEnd) {
	sweep = _sweep;
	freqEnd = _freqEnd;
}

void audioFreq(float _freq) {
	freqStart = _freq;
}
void audioPlay() {
	alSourcef(
		sid,
		AL_GAIN,
		gain=DEFAULT_GAIN);
	freq = freqStart;
	alSourcef(
		sid,
		AL_PITCH,
		freq/DEFAULT_FREQ);
	alSourcei(
		sid,
		AL_BUFFER,
		buffers[waveform]);
	alSourcePlay(sid);
	start = clock();
}
void audioStop() {
	alSourceStop(sid);
}
void audioUpdate() {
	if ((length > 0) && (clock() - start >= length))
		audioStop();
	if ((decay!=0)&&(decay <1)) {
		alSourcef(
			sid,
			AL_GAIN,
			gain *= decay);
	}
	if (sweep != 0) {
		freq *= sweep;
		if (freqEnd != 0) {
			if (
				((sweep > 1) && (freq >= freqEnd))
				|| ((sweep < 1) && (freq <= freqEnd)))
			{
				audioStop();
			}
		}
			alSourcef(
				sid,
				AL_PITCH,
				freq/DEFAULT_FREQ);
	}
}