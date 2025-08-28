#pragma once

#include "AdditiveOscillator.h"

#include <math.h>
#include <random>

namespace LAMELLA_INST {

	void AdditiveOscillator::noteOn(Message Msg) {

		int startPartial = mStiffness * NUM_PARTIALS;

		// Initialise temp arrays (startPartial would leave these uninitialised otherwise
		memcpy(Amps, BaseAmps, sizeof(float) * NUM_PARTIALS);
		memcpy(Decays, BaseDecays, sizeof(float) * NUM_PARTIALS);
		memcpy(Ratios, BaseRatios, sizeof(float) * NUM_PARTIALS);

		// Apply table adjustments
		updatePartials(BaseAmps, BaseRatios, BaseDecays, Amps, Ratios, Decays, startPartial, Msg);
		
		calculateOrganic(Ratios, Amps, Decays);

		float freqThreshold = (Setup.sampleRate / 2.0) - 1000.0; //Nyquist minus clearance
		for (int i = 0; i < NUM_PARTIALS; i++) {

			// Check Nyquist, skip if over
			float thisPartialHz = noteNumToHz(Msg.noteNum) * Ratios[i];
			
			if (thisPartialHz >= freqThreshold) {
				continue; 
			}


			Partials[i].setAmplitude(Amps[i]);
			Partials[i].setBlur(mBlur);
			Partials[i].setDecay(Decays[i]);
			setPartialFrequency(i, Msg, Ratios[i]);
			Partials[i].noteOn();
		}
	}
	void AdditiveOscillator::updatePartials(float* baseAmps, float* baseRatios, float* baseDecays, float* outAmps, float* outRatios, float* outDecays, int startPartial, Message Msg) {

		calculateOddEven(baseAmps, outAmps, 0, Msg.velocity);
		calculateBrightness(outAmps, outAmps, Msg.velocity);
		calculateMetallic(baseRatios, outRatios, startPartial, Msg);
		calculateDecays(outDecays, baseDecays, Msg.velocity);
	}

	/// <summary>
	/// Helper to set all partials to their corresponding frequency
	/// based on the ratio table, and a midi message in a Lamella Midi struct
	/// </summary>
	/// <param name="Msg"></param>
	/// <param name="Table"></param>
	void AdditiveOscillator::setPartialFrequency(int partial_num, Message Msg, float ratio) {
		float baseFreq = noteNumToHz(Msg.noteNum);

			Partials[partial_num].setFrequencyHz(baseFreq * ratio);
		
	}
	/// <summary>
	/// Stretches or compresses the partials according to the Metallic parameter.
	/// Morph blends the stretch between a linear uniform stretch, and an exponential stretch
	/// 0 = Same stretch amount for all partials relative to partial number
	/// 1 = More stretch amount for each successive partial relative to partial number
	/// </summary>
	/// <param name="input"></param>
	/// <param name="output"></param>
	/// <param name="fromPartial"></param>
	/// <param name="velocity"></param>
	void AdditiveOscillator::calculateMetallic(float* input, float* output, int fromPartial, Message Msg) {
		// Convert vel modifier to bipolar
		float velMod = (mVelMetallic * 2.0f) - 1.0f;
		float velWord = velMod * Msg.velocity;

		// Create local keytrack variable, scale keytrack value to -1 : 1
		float keyMod = (2.0 * mKeyMetallic) - 1.0f;
		float keyValue = Msg.noteNum / 127.0f; // Scale current note to max note num (0 - 1)
		
		// Calculate stretch with piecewise linear to get usable range (1 at 0.5, 0 at 0, 4 at 1)
		float stretch = 0;

		if (mMetallic <= 0.5f) {
			stretch = mMetallic + 0.5;
		}
		if (mMetallic > 0.5) {
			stretch = (6.0f * mMetallic) - 2.0f;
		}
		// TODO mMorph isnt very effective
		for (int i = fromPartial; i < NUM_PARTIALS; i++) {
			float partialAmount = 1;
			if (i > 0) {
				partialAmount = powf(i + (i * mMorph), stretch);

			}
			
			output[i] = input[i] * partialAmount ;
		}
	}
	/// <summary>
	/// Uses mStruct parameter to mix between all even (0) and all odd (1) harmonics
	/// </summary>
	/// <param name="AmpInputs"></param>
	/// <param name="AmpOutputs"></param>
	/// <param name="fromPartial"></param>
	/// <param name="velocity"></param>
	void AdditiveOscillator::calculateOddEven(float* AmpInputs, float* AmpOutputs, int fromPartial, float velocity) {
		// Convert vel modifier to bipolar
		float velMod = (mVelStructure * 2) - 1;
		float velWord = velMod * velocity;

		
		// Base
		float oddLevel = (mStructure);
		float evenLevel = 1.0f - mStructure;
		
		// Vel check
		float structVel = mStructure * velWord;
		// If velocity amount not zero...
		if (fabsf(structVel) > 0.001f) {
			// Clamp
			if (structVel > 1.0f) structVel = 1;
			if (structVel < 0.0f) structVel = 0;

			oddLevel = (structVel);
			evenLevel = 1.0f - structVel;

		}


		for (int i = fromPartial; i < NUM_PARTIALS; i++) {
			if (((i + 1) % 2) == 0) {
				AmpOutputs[i] = AmpInputs[i] * evenLevel;
			}
			else {
				AmpOutputs[i] = AmpInputs[i] * oddLevel;
			}
		}
	}
	/// <summary>
	/// Applies a tilt to partials, acting like a tilt EQ
	/// uses mBrightness parameter
	/// </summary>
	/// <param name="AmpInputs"></param>
	/// <param name="AmpOutputs"></param>
	/// <param name="velocity"></param>
	void AdditiveOscillator::calculateBrightness(float* AmpInputs, float* AmpOutputs, float velocity) {

		// A tilt on the amp levels.

		float maxAmp = 0;
		for (int i = 0; i < NUM_PARTIALS; i++) {
			float mappedValue = (mBrightness - 0.5f) * 2.0f;
			float position = (static_cast<float>(i) / (NUM_PARTIALS - 1)) - 0.5f;

			// Convert vel modifier to bipolar
			float velMod = (mVelBrightness * 2) - 1;
			float velWord = velMod * velocity;

			float multiplier = 1.0f + (mappedValue * position * 2.0f);
			if (fabsf(velMod) > 0.001f) {
				multiplier = 1.0f + (mappedValue * position * 2.0f * velWord);
			}
			


			AmpOutputs[i] = AmpInputs[i] * multiplier;

			if (AmpOutputs[i] > maxAmp) {
				maxAmp = AmpOutputs[i];
			}

		}

		float normalizationFactor = (maxAmp > 1.0f) ? 1.0f / maxAmp : 1.0f;

		for (int i = 0; i < NUM_PARTIALS; i++) {
			AmpOutputs[i] = AmpOutputs[i] * normalizationFactor;
		}


	}
	/// <summary>
	/// Performs random changes to ratios, amps and decays
	/// Adjustable with respective mXOrganic parameter
	/// </summary>
	/// <param name="Ratios"></param>
	/// <param name="Amps"></param>
	/// <param name="Decays"></param>
	/// <param name="startPartial"></param>
	void AdditiveOscillator::calculateOrganic(float* Ratios, float* Amps, float* Decays, int startPartial) {

		// Ratio
		for (int i = startPartial; i < NUM_PARTIALS; i++) {
			float randOffset = (float)rand() / (float)RAND_MAX;
			randOffset *= mOrganicRatio;
			Ratios[i] = Ratios[i] + randOffset;
		}

		// Amp
		const float maxAmp = 1.0f;
		for (int i = startPartial; i < NUM_PARTIALS; i++) {
			float randOffset = (float)rand() / (float)RAND_MAX;
			randOffset *= mOrganicAmp;
			Amps[i] = Amps[i] + randOffset;
			Amps[i] >= maxAmp ? maxAmp : Amps[i];
		}

		// Decay
		for (int i = startPartial; i < NUM_PARTIALS; i++) {
			float randOffset = (float)rand() / (float)RAND_MAX;
			randOffset *= mOrganicDecay;
			Decays[i] = Decays[i] + randOffset;
		}
	}
	/// <summary>
	/// Extends or reduces decays from the initial baseDecays amounts
	/// Uses mDecayOffset parameter
	/// </summary>
	/// <param name="Decays"></param>
	/// <param name="BaseDecays"></param>
	/// <param name="velocity"></param>
	/// <param name="startPartial"></param>
	void AdditiveOscillator::calculateDecays(float* Decays, const float* BaseDecays, float velocity, int startPartial) {
		
		float offsetAdj = (2 * mDecayOffset) - 1;
		
		if (fabsf(mVelDecays) > 0.001) {
			offsetAdj *= velocity * mVelDecays;
		}

		for (int i = 0; i < NUM_PARTIALS; i++) {

			Decays[i] = BaseDecays[i] * (1 + offsetAdj);
		}
	}

}

