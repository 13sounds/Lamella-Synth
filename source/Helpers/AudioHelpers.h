#ifndef AUDIO_HELPERS_H
#define AUDIO_HELPERS_H


#include <vector>
#include <math.h>
/// <summary>
/// Helper class with buffer operations and other misc functions
/// </summary>

#define M_PI 3.1415926535897932384626433832



namespace LAMELLA_INST {

	typedef std::vector<float> AudioBuffer;

	// Struct holding info about the current process call
	// and setup
	struct ProcessInfo {
		ProcessInfo(int numSamples = 0, int startIndex = 0) {
			this->numSamples = numSamples;
			this->startIndex = startIndex;
		}
		int numSamples = 0;
		int startIndex = 0;

	};
	struct SetupInfo {
		SetupInfo(int sample_rate = 0, int block_size = 0) {
			sampleRate = sample_rate;
			blockSize = block_size;
		}

		int sampleRate = 0;
		int blockSize = 0;
	};
	/// <summary>
	/// Simple struct to pass note events
	/// </summary>
	struct Message {
		int noteNum = 0;
		float velocity = 0;
	};

	void copyVectorToBuffer(float* output, std::vector<float>& input, ProcessInfo Info);
	void mixBuffers(AudioBuffer& target, std::vector<float> inputs[], int num_inputs, ProcessInfo Info);
	void mixBuffers(float* target, std::vector<float> inputs[], int num_inputs, ProcessInfo Info);
	void multBuffer(float* buffer, float multiplier, ProcessInfo Info);

	void clearBuffer(AudioBuffer& buffer);

	float noteNumToHz(int note_num);

};

#endif // AUDIO_HELPERS_H

