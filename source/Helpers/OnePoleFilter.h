#pragma once
#include <math.h>
#include "Helpers/AudioHelpers.h"

namespace LAMELLA_INST {
	class OnePoleFilter {
	public:
		void setSampleRateBlockSize(SetupInfo Info) {
			sampleRate = Info.sampleRate;
			blockSize = Info.blockSize;

		}
		void setTimeConstant(float time_s) {
		
			alpha = -1.0f / (time_s * sampleRate);
			alpha = expf(alpha);
		}
		float getNext(float x) {
			z1 = ((1.0f - alpha) * x) + (alpha * z1);
			return z1;
		}
		void setInternalState(float value) {
			z1 = value;
		}
		float getInternalState() const {
			return z1;
		}
		void ResetState() {
			z1 = 0;
		}
	private:
		int sampleRate = -1;
		int blockSize = -1;

		float z1 = 0;
		float alpha = 0;
	};

}

