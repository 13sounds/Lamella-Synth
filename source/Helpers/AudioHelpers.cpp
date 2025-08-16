#include "AudioHelpers.h"




namespace LAMELLA_INST {

	void copyVectorToBuffer(float* output, std::vector<float>& input, ProcessInfo Info) {

		if (!output) {
			return;
		}

		memcpy(&output[Info.startIndex], &input[Info.startIndex], sizeof(float) * Info.numSamples);
	}

	void mixBuffers(AudioBuffer& target, std::vector<float> inputs[], int num_inputs, ProcessInfo Info) {

		if (num_inputs == 0) {
			return;
		}


		float scaleFactor = 1.0f / num_inputs;
		// Sum
		for (int n = 0; n < Info.numSamples; n++) {
			int index = n + Info.startIndex;
			float sum = 0;

			for (int i = 0; i < num_inputs; i++) {

				sum += inputs[i][index];
			}
			target[index] = sum * scaleFactor;
		}

	}

	void mixBuffers(float* target, std::vector<float> inputs[], int num_inputs, ProcessInfo Info) {

		if (num_inputs == 0 || !target) {
			return;
		}


		float scaleFactor = 1.0f / num_inputs;
		// Sum
		for (int n = 0; n < Info.numSamples; n++) {
			int index = n + Info.startIndex;
			float sum = 0;

			for (int i = 0; i < num_inputs; i++) {

				sum += inputs[i][index];
			}
			target[index] = sum * scaleFactor;
		}

	}
	void multBuffer(float* buffer, float multiplier, ProcessInfo Info) {
		for (int i = 0; i < Info.numSamples; i++) {
			int index = i + Info.startIndex;
			buffer[index] *= multiplier;


		}

	}

	void clearBuffer(AudioBuffer& buffer) {
		std::fill(buffer.begin(), buffer.end(), 0.0f);
	}

	float noteNumToHz(int note_num) {
		return 440.0f * powf(2.0f, ((note_num - 69.0f) / 12.0f));
	}






}