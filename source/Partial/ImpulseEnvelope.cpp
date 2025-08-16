#include "ImpulseEnvelope.h"

namespace LAMELLA_INST {
	void ImpulseEnvelope::getBlock(AudioBuffer& buffer, int num_samples, int start_index) {
		assert(buffer.size() >= num_samples);

		for (int s = 0; s < num_samples; s++) {
			getNext();
			buffer[s + start_index] = outputValue;
		}
	};
	void ImpulseEnvelope::applyBlock(AudioBuffer& buffer, int num_samples, int start_index) {
		assert(buffer.size() >= num_samples);

		for (int s = 0; s < num_samples; s++) {
			getNext();
			buffer[s + start_index] *= outputValue;
		}
	};

	void ImpulseEnvelope::getNext() {
	
		switch (currentStatus) {
		case kAttacking:
		
			AttackPhase();
			break;
		case kDecaying:
			internalValue = 0;
			DecayPhase();
			break;
		case kInactive:
			internalValue = 0;
			outputValue = 0;
			break;
		default:
			break;
		}

	}

	void ImpulseEnvelope::noteOn(float velocity) {
		
		mNoteVel = velocity;

		if (mVelDecay > 0) {
			DecayFilter.setTimeConstant(mVelDecay * decayModifier * mDecayValue * mNoteVel);
		}
		else {
			DecayFilter.setTimeConstant(decayModifier * mDecayValue);
		}

		// If decay hasn't finished, start from current value

		//if (currentStatus == kDecaying) {
		//	outputValue = DecayFilter.getInternalState();
		//}
		currentStatus = kAttacking;
		outputValue = 0;
		DecayFilter.ResetState();
	}

	void ImpulseEnvelope::AttackPhase() {
		if (mAttackValue < 0.0001) {
			currentStatus = kDecaying;
			DecayFilter.setInternalState(0.9);

			return;
		}

		outputValue = outputValue + attackStep;
		if (outputValue >= 1.0f) {
			// At very low non-zero attack time, value can overshoot 1, so clamp it to 1
			currentStatus = kDecaying;
			outputValue = 1.0f;
			DecayFilter.setInternalState(outputValue);
			return;
		}
		return;
	}
	void ImpulseEnvelope::DecayPhase() {

		outputValue = DecayFilter.getNext(0);
		if (mDecayValue < 0.0001) {
			currentStatus = kInactive;
			return;
		}

		if (outputValue <= 0.0f) {
			currentStatus = kInactive;
			return;
		}
	}
}