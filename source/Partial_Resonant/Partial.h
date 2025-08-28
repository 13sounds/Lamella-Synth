#pragma once
#include "Helpers/AudioHelpers.h"

#include "Partial_Resonant/Resonator.h"

namespace LAMELLA_INST {
	/// <summary>
	/// A single additive synth partial. Comprises a sine
	/// oscillator and an envelope
	/// </summary>
	class ResonantPartial {
	public:
		void setSampleRateBlockSize(SetupInfo Setup) {

			this->Setup = Setup;
			Resonator.setSampleRateBlockSize(Setup);

		
		}
		void getBlock(AudioBuffer& output, ProcessInfo Info) {

			for (int i = 0; i < Info.numSamples; i++) {
				int index = i + Info.startIndex;
				output[index] = Resonator.getNext() * mAmp;
			}

		}
		/// <summary>
		/// Trigger note on, set frequency and amplitudes
		/// </summary>
		/// 
		void noteOn() {
			Resonator.trigger(1);
		
		}
		void setFrequencyHz(float freq_hz) {
			mCurrentFreqHz = freq_hz;
			Resonator.setFrequency(freq_hz);
		}
		void setAmplitude(float value) {
			mAmp = value;
		}

		void setDecay(float decay) {
			const float decayMaxScale = 5;
			Resonator.setDecayTimeSeconds(decay * decayMaxScale);
		}
		void setBlur(float value) {
			
		}

		float getFreqHz() const {
			return mCurrentFreqHz;
		}

		bool isActive() const {
			return Resonator.isActive();
		}

	private:
		SetupInfo Setup;

		ResonatorFilter Resonator;
	

		float mCurrentFreqHz = 0;
		float mAmp = 0.5f;
	};

}
