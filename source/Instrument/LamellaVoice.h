#pragma once

#include "Instrument/ParamRegistration.h"
#include "Helpers/AudioHelpers.h"
#include "AdditiveSynth/AdditiveOscillator.h"
/// <summary>
/// A single voice of the instrument, managed by the LamellaInstrument class
/// </summary>
namespace LAMELLA_INST {

	class LamellaVoice {

	public:
		/// <summary>
		/// Get sample rate and block size from Instrument Processor
		/// </summary>
		/// <param name="Setup">An AudioHelper object containing setup info</param>
		void getSampleRateBlockSize(SetupInfo Setup) {
			this->Setup = Setup;
			AdditiveOsc.getSampleRateBlockSize(Setup);
			
		}
		void getBlock(AudioBuffer& Buffer, ProcessInfo Info) {
			AdditiveOsc.getBlock(Buffer, Info);
		}
		void noteOn(Message Msg) {

			AdditiveOsc.noteOn(Msg);
		}
		bool isActive() const {
			return AdditiveOsc.isActive();
		}
		void getParams(LamellaParams& params) {


			if (params.vMetallic.isDirty()) {
				AdditiveOsc.setMetallicAmount(params.vMetallic.getExplicit());
			}
			if (params.vMorph.isDirty()) {
				AdditiveOsc.setMorphAmount(params.vMorph.getExplicit());
			}
			if (params.vBlur.isDirty()) {
				AdditiveOsc.setBlurAmount(params.vBlur.getExplicit());
			}
			if (params.vStructure.isDirty()) {
				AdditiveOsc.setStructureMix(params.vStructure.getExplicit());
			}
			if (params.vBright.isDirty()) {
				AdditiveOsc.setBrightnessAmount(params.vBright.getExplicit());
			}
			if (params.vDecays.isDirty()) {
				AdditiveOsc.setDecayOffsetAmount(params.vDecays.getExplicit());
			}

			// Organic
			if (params.vOrganicRatio.isDirty()) {
				AdditiveOsc.setOrganicRatio(params.vOrganicRatio.getExplicit());
			}
			if (params.vOrganicAmp.isDirty()) {
				AdditiveOsc.setOrganicAmp(params.vOrganicAmp.getExplicit());
			}
			if (params.vOrganicDecay.isDirty()) {
				AdditiveOsc.setOrganicDecay(params.vOrganicDecay.getExplicit());
			}

			// Velocity
			if (params.vVelMetal.isDirty()) {
				AdditiveOsc.setVelocityMetallicAmount(params.vVelMetal.getExplicit());

			}
			if (params.vVelStruct.isDirty()) {
				AdditiveOsc.setVelocityStructureAmount(params.vVelStruct.getExplicit());

			}
			if (params.vVelBright.isDirty()) {
				AdditiveOsc.setVelocityBrightnessAmount(params.vVelBright.getExplicit());
			}
			if (params.vVelDecay.isDirty()) {
				AdditiveOsc.setVelocityDecayAmount(params.vVelDecay.getExplicit());
			}

			// Key
			if (params.vKeyMetal.isDirty()) {
				AdditiveOsc.setKeyMetallic(params.vKeyMetal.getExplicit());
			}


			if (params.vStiffness.isDirty()) {
				AdditiveOsc.setStiffnessAmount(params.vStiffness.getExplicit());
			}



		}

		void getValuesForUI(float* Amps, float* Ratios, float* Decays) {

			AdditiveOsc.getValuesForUI(Amps, Ratios, Decays);
		}
	private:
		SetupInfo Setup;
		AdditiveOscillator AdditiveOsc;

	
	};





};