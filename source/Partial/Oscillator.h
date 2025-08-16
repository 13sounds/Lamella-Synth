#pragma once
#include "Helpers/AudioHelpers.h"


namespace LAMELLA_INST {
	class Oscillator
	{
	public:
		void setSampleRateBlockSize(SetupInfo Info) {
			sampleRate = Info.sampleRate;
			blockSize = Info.blockSize;

			phaseIncrement = 1.0f / sampleRate;

			FreqRangeHz = (maxFreqHz - minFreqHz);
		}
		void getBlock(std::vector<float>& buffer, ProcessInfo Info) {

			for (int i = 0; i < Info.numSamples; i++) {
				int index = i + Info.startIndex;
				buffer[index] = getNext() * mAmp;
			}
		}
		void applyBlock(std::vector<float>& buffer, ProcessInfo Info) {

			for (int i = 0; i < Info.numSamples; i++) {
				int index = i + Info.startIndex;
				buffer[index] = buffer[index] + (getNext() * mAmp);
			}
		}
		void getModulatedBlock(std::vector<float>& buffer, std::vector<float>& mod_buffer, ProcessInfo Info) {

			for (int i = 0; i < Info.numSamples; i++) {
				int index = i + Info.startIndex;
				buffer[index] = getPMNext(mod_buffer[index]);
			}
		}
		void setFrequencyNormalised(float value) {
			mFrequencyNormalised = value;
			mFrequencyHz = (value * FreqRangeHz) + minFreqHz;
		}
		void setFrequencyHz(float Hz) {
			mFrequencyHz = Hz;
		}
		void setPMAmount(float value) {
			mModAmt = value;
		}
		void resetPhase() {
			currentPhase = 0;
		}
		void setAmp(float value) {
			mAmp = value;
		}
		void setPhase(float value) {
			currentPhase = value;
		}
		void setBlur(float value) {
			mBlur = value;
		}
	private:
		int sampleRate = 0;
		int blockSize = 0;

		float currentPhase = 0;
		float phaseIncrement = 0;
		float startPhase = 0;
		float mFrequencyNormalised = 0.5;
		float mFrequencyHz = 440;

		const float maxFreqHz = 1046.50f; // C6
		const float minFreqHz = 32.70f;   // C1
		float FreqRangeHz = 0;

		float mModAmt = 0;
		float mAmp = 0.5;
		float mBlur = 0.0f;

		float getNext() {

			if (mFrequencyHz < 1.0f) {
				return 0;
			}

			const float maxBlurMod = 1000.0f;

			float f = mFrequencyHz + (mBlur * maxBlurMod * ((float)rand() / (float)RAND_MAX));

			currentPhase += (phaseIncrement * f);
			startPhase = currentPhase - 1.0f;

			if (currentPhase > 1.0f) {
				currentPhase = startPhase;
			}
			return sinf(M_PI * 2.0f * currentPhase);

		}
		float getPMNext(float mod_sample) {
			float moddedPhase = (mod_sample * mModAmt);

			currentPhase += (phaseIncrement * mFrequencyHz);
			startPhase = currentPhase - 1.0f;

			if (currentPhase > 1.0f) {
				currentPhase = startPhase;
			}
			return sinf((M_PI * 2.0f * currentPhase) + moddedPhase);

		}
	};
};



