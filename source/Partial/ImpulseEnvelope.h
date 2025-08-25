#pragma once


#include <vector>
#include <assert.h>
#include <math.h>
#include <algorithm>
#include <Helpers/AudioHelpers.h>
#include "Helpers/OnePoleFilter.h"

namespace LAMELLA_INST {

	/// <summary>
	/// An Attack-Decay envelope using a one pole 
	/// filter to generate the decay portion. Does not obey note Offs
	/// </summary>
	class ImpulseEnvelope
	{
	public:
		enum Status {
			kAttacking = 100,
			kDecaying,
			kInactive
		};
		void setSampleRateBlockSize(SetupInfo Info) {
			sampleRate = Info.sampleRate;
			blockSize = Info.blockSize;
			timeStep = 1.0f / sampleRate;
			

			DecayFilter.setSampleRateBlockSize(Info);

			// set defaults
			DecayFilter.setTimeConstant(mDecayValue * decayModifier);
			attackStep = timeStep / (mAttackValue * attackModifier);
		}
		void setAttack(float value) {
			value = std::clamp<float>(value, 0.0f, 1.0f);
			mAttackValue = value;
			attackStep = timeStep / (value * attackModifier);
		
		}
		void setDecay(float value) {
			value = std::clamp<float>(value, 0.0f, 1.0f);
			mDecayValue = value * decayModifier;
			DecayFilter.setTimeConstant(value * decayModifier);
	
		}
		void setVelDecay(float value) {
			mVelDecay = value;
		}
		float getInternalState() const {
			return DecayFilter.getInternalState();
		}
		void noteOn(float velocity = 0);
		void noteOff();
		void Reset() {
			currentStatus = kInactive;
			outputValue = 0;
			internalValue = 0;
		};
		void getBlock(AudioBuffer& buffer, int num_samples, int start_index);
		void applyBlock(AudioBuffer& buffer, int num_samples, int start_index);

		bool isActive() const {
			bool active = true;
			if (currentStatus == kInactive) {
				active = false;
			}
			return active;
		}

	private:
		OnePoleFilter DecayFilter;

		const float attackModifier = 0.01;
		const float decayModifier = 0.8;

		float mAttackValue = 0.005;
		float mDecayValue  = 0.3;
		float mVelDecay	= 0;
		float mNoteVel = 0;

		int sampleRate  = -1;
		int blockSize	= -1;
		float timeStep	= 0;
		float attackStep = 0;
		float internalValue = 0;
		float outputValue	= 0;
		Status currentStatus = kInactive;

		void AttackPhase();
		void DecayPhase();
		void getNext();
		

	};
}



