#pragma once

#include "Helpers/AudioHelpers.h"
#include "Partial/Partial.h"



#define NUM_PARTIALS 16

namespace LAMELLA_INST {

	class AdditiveOscillator {
	public:
		/// <summary>
		/// Get sample rate and block size from Instrument Processor
		/// </summary>
		/// <param name="Setup">An AudioHelper object containing setup info</param>
		void getSampleRateBlockSize(SetupInfo Setup) {
			this->Setup = Setup;

			for (int i = 0; i < NUM_PARTIALS; i++) {
				Partials[i].setSampleRateBlockSize(Setup);
				PartialBuffers[i].resize(Setup.blockSize);
			}
			PartialMix.resize(Setup.blockSize);

			// Generate default partial tables
			for (int i = 0; i < NUM_PARTIALS; i++) {
				BaseRatios[i] = i + 1;
				BaseAmps[i] = 1.0f;
				BaseDecays[i] = 1.0f / (i + 1);
			}
		}
		void getBlock(AudioBuffer& buffer, ProcessInfo Info) {

			// If a partial is above nyquist, skip and count
			float nyQuist = Setup.sampleRate / 2.0f;
			int numOverNyquist = 0;
			for (int i = 0; i < NUM_PARTIALS; i++) {

				if (Partials[i].getFreqHz() > nyQuist) {
					numOverNyquist++;
					continue;
				}


				Partials[i].getBlock(PartialBuffers[i], Info);
			}

			// Mix down, normalise to only active partials
			mixBuffers(buffer, PartialBuffers, NUM_PARTIALS - numOverNyquist, Info);

		}

		void setMetallicAmount(float value) {
			mMetallic = value;
		}
		void setBlurAmount(float value) {
			mBlur = value;
		}
		void setMorphAmount(float value) {
			mMorph = value;
		}
		void setStructureMix(float value) {
			mStructure = value;
		}

		void setBrightnessAmount(float value) {
			mBrightness = value;
		}


		void setOrganicRatio(float value) {
			mOrganicRatio = value;
		}
		void setOrganicAmp(float value) {
			mOrganicAmp = value;
		}
		void setOrganicDecay(float value) {
			mOrganicDecay = value;
		}
		void setDecayOffsetAmount(float value) {
			mDecayOffset = value;
		}

		void setStiffnessAmount(float value) {
			mStiffness = value;
		}

		// Velocity
		void setVelocityMetallicAmount(float value) {
			mVelMetallic = value;
		}
		void setVelocityStructureAmount(float value) {
			mVelStructure = value;
		}
		void setVelocityBrightnessAmount(float value) {
			mVelBrightness = value;
		}
		void setVelocityDecayAmount(float value) {
			mVelDecays = value;
		}
		void noteOn(Message Msg);

		/// <summary>
		/// Get the Amp, Ratio and Decay data as calculated using
		/// dummy values, but still reflecting core audio process
		/// </summary>
		/// <param name="Amps"></param>
		/// <param name="Ratios"></param>
		/// <param name="Decays"></param>
		void getValuesForUI(float* Amps, float* Ratios, float* Decays) {

			int startPartial = mStiffness * NUM_PARTIALS;
			const float velMax = 1.0f;
			memcpy(Amps, BaseAmps, sizeof(float) * NUM_PARTIALS);
			memcpy(Ratios, BaseRatios, sizeof(float) * NUM_PARTIALS);
			memcpy(Decays, BaseDecays, sizeof(float) * NUM_PARTIALS);

			updatePartials(BaseAmps, BaseRatios,BaseDecays, Amps, Ratios, Decays, startPartial, velMax);
		

		}

	private:
		SetupInfo Setup;
		Partial Partials[NUM_PARTIALS];
		AudioBuffer PartialMix;
		AudioBuffer PartialBuffers[NUM_PARTIALS];

		float Ratios[NUM_PARTIALS];
		float BaseRatios[NUM_PARTIALS];

		float BaseAmps[NUM_PARTIALS];
		float Amps[NUM_PARTIALS];

		float BaseDecays[NUM_PARTIALS];
		float Decays[NUM_PARTIALS];

		float mDecayOffset = 0.0f;
		float mMetallic = 0;
		float mBlur = 0.0f;
		float mMorph = 0.5;

		float mOrganicRatio = 0;
		float mOrganicAmp = 0;
		float mOrganicDecay = 0;
		float mStructure = 0.5;
		float mBrightness = 0.5;

		float mVelMetallic = 0.5;
		float mVelStructure = 0.5;
		float mVelBrightness = 0.5;
		float mVelDecays = 0.5;

		float mStiffness = 0.0f;

		void updatePartials(float* baseAmps, float* baseRatios, float* baseDecays, float* outAmps, float* outRatios, float* outDecays, int startPartial, float velocity);
		void setPartialFrequency(Message Msg, float* Table);
		void calculateMetallic(float* input, float* output, int fromPartial = 1, float velocity = 0);
		void calculateOddEven(float* AmpInputs, float* AmpOutputs, int fromPartial = 1, float velocity = 0);
		void calculateBrightness(float* AmpInputs, float* AmpOutputs, float velocity = 0);
		void calculateOrganic(float* Ratios, float* Amps, float* Decays, int startPartial = 1);
		void calculateDecays(float* Decays, const float* BaseDecays, float velocity = 0, int startPartial = 1);

		};
}
