#pragma once

#include "Helpers/AudioHelpers.h"

namespace LAMELLA_INST {

	class ResonatorFilter {
	public:
		ResonatorFilter() {
			
		}

		void setSampleRateBlockSize(SetupInfo Setup) {

			this->Setup = Setup;
		
		}
		void setFrequency(float freq_hz) {
			freqHz = freq_hz;
			updateCoefficients();
		}
		void setDecayTimeSeconds(float value) {
			if (value <= 0.0f) {
				// Avoid division by zero and log of zero; set to no decay
				DecayCoeff = 1.0f;
			}
			else {
				// T60 formula
				DecayCoeff = expf(logf(0.001f) / (value * Setup.sampleRate));
			}
		
		}
		float getNext() {
			float spl = (C1 * y1) - (C2 * y2);

			y2 = y1;
			y1 = spl;
			return spl;
		}
		void trigger(float amp) {
			y1 = amp;
			y2 = 0.0f;
		}
		bool isActive() const {
			const float threshold = 0.0001;
			if ((fabsf(y1) > threshold) || (fabsf(y2) > threshold)) {
				return true;
			}
			return false;
		}

	private:
		SetupInfo Setup;

		float freqHz = 440;

		float W = 0.0f;
		float C1 = 0.0f;
		float C2 = 0.0f;
		float DecayCoeff = 0.0f;

		float y1 = 0.0f;
		float y2 = 0.0f;

		void updateCoefficients() {
			constexpr float pi = 3.141592653589793238462643f;
			W = 2.0f * pi * (freqHz / Setup.sampleRate);

			C1 = 2.0f * DecayCoeff * cosf(W);
			C2 = DecayCoeff * DecayCoeff;

		}
	};

}

