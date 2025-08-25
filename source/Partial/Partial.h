#pragma once
#include "Helpers/AudioHelpers.h"

#include "Partial/Oscillator.h"
#include "Partial/ImpulseEnvelope.h"

namespace LAMELLA_INST {
	/// <summary>
	/// A single additive synth partial. Comprises a sine
	/// oscillator and an envelope
	/// </summary>
	class Partial {
	public:
		void setSampleRateBlockSize(SetupInfo Setup) {

			this->Setup = Setup;
				Osc.setSampleRateBlockSize(Setup);
				Env.setSampleRateBlockSize(Setup);
			
				InternalBuffer.resize(Setup.blockSize);
		}
		void getBlock(AudioBuffer& output, ProcessInfo Info) {

				Osc.getBlock(output, Info);
				Env.applyBlock(output, Info.numSamples, Info.startIndex);

				
	
		}
		/// <summary>
		/// Trigger note on, set frequency and amplitudes
		/// </summary>
		/// <param name="freq_hz"></param>
		/// <param name="amplitude"></param>
		/// <param name="phase"> start phase (0,1)</param>
		void noteOn() {
	
			Osc.setPhase(0);
			Osc.setAmp(mAmp);
		
			Env.noteOn();
		}
		void setFrequencyHz(float freq_hz) {
			Osc.setFrequencyHz(freq_hz);
		}
		void setAmplitude(float value) {
			mAmp = value;
		}

		void setAttackDecay(float attack, float decay) {
			Env.setAttack(attack);
			Env.setDecay(decay);
		}
		void setBlur(float value) {
			Osc.setBlur(value);
		}

		float getFreqHz() const {
			return mCurrentFreqHz;
		}

		bool isActive() const {
			return Env.isActive();
		}

	private:
		SetupInfo Setup;
		Oscillator Osc;
		ImpulseEnvelope Env;

		AudioBuffer InternalBuffer;

		float mCurrentFreqHz = 0;
		float mAmp = 1.0f;
	};

}

