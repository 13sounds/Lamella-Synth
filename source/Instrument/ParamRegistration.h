#pragma once

#include "public.sdk/source/vst/vsteditcontroller.h"


namespace LAMELLA_INST {

	enum ParamIDs {

		kLevel = 200,

		kMetallic,
		kMorph,
		kBlur,
		kStructure,
		kBright,
		kOrganicRatio,
		kOrganicAmp,
		kOrganicDecay,
		kDecays,

		kVelStruct,
		kVelMetal,
		kVelBright,
		kVelDecays,

		kStiffness,

	};

	inline void registerParameters(Steinberg::Vst::ParameterContainer& parameters) {

		parameters.addParameter(STR16("Level"), STR16(""), 0, 0.5, 1, ParamIDs::kLevel);
		parameters.addParameter(STR16("Blur"), STR16(""), 0, 0.0, 1, ParamIDs::kBlur);


		parameters.addParameter(STR16("Metallic"), STR16(""), 0, 0.5, 1, ParamIDs::kMetallic);
		parameters.addParameter(STR16("Morph"), STR16(""), 0, 0.0, 1, ParamIDs::kMorph);


		parameters.addParameter(STR16("Stiff"), STR16(""), 0, 0, 1, ParamIDs::kStiffness);

		
		parameters.addParameter(STR16("Structure"), STR16(""), 0, 0.5, 1, ParamIDs::kStructure);
		parameters.addParameter(STR16("Brightness"), STR16(""), 0, 0.5, 1, ParamIDs::kBright);

		parameters.addParameter(STR16("Organic Ratio"), STR16(""), 0, 0, 1, ParamIDs::kOrganicRatio);
		parameters.addParameter(STR16("Organic Amp"), STR16(""), 0, 0, 1, ParamIDs::kOrganicAmp);
		parameters.addParameter(STR16("Organic Decay"), STR16(""), 0, 0, 1, ParamIDs::kOrganicDecay);


		parameters.addParameter(STR16("Decays"), STR16(""), 0, 0.5, 1, ParamIDs::kDecays);


		parameters.addParameter(STR16("Vel Struct"), STR16(""), 0, 0.5, 1, ParamIDs::kVelStruct);
		parameters.addParameter(STR16("Vel Metal"), STR16(""), 0, 0.5, 1, ParamIDs::kVelMetal);
		parameters.addParameter(STR16("Vel Bright"), STR16(""), 0, 0.5, 1, ParamIDs::kVelBright);
		parameters.addParameter(STR16("Vel Decays"), STR16(""), 0, 0.5, 1, ParamIDs::kVelDecays);

	}
	/// <summary>
	/// A single parameter with the optional function of
	/// internal curve mapping
	/// </summary>
	class ParamUnit {
	public:
		ParamUnit(int paramID = 0, float default_value = 0, bool wantsScaling = false) {
			value = default_value;
			curved = wantsScaling;
			id = paramID;
		}
		/// <summary>
		/// Set parameters value and set dirty
		/// </summary>
		/// <param name="value"></param>
		void set(float value) {
			this->value = value;
			setDirty();
		}
		/// <summary>
		/// Get parameters current value
		/// smoothed
		/// </summary>
		/// <returns></returns>
		float getSmoothed()  {
			return value;
		}
		/// <summary>
		/// Get unsmoothed value and set clean
		/// </summary>
		/// <returns></returns>
		float getExplicit() {
		
			return value;

		}
		bool isDirty() const {
			return dirty;
		}
		void setDirty() {
			dirty = true;
		}
		void setClean() {
			dirty = false;
		}

	private:
		float value = 0;
		int id = 0;
		bool curved = false;
		bool dirty = false;
	};

	/// <summary>
	/// Container class for parameters, to allow
	/// easy smoothing and remapping
	/// </summary>
	class LamellaParams {
	public:
		void setParameter(int paramID, float value) {
			switch (paramID) {
			case kLevel:
				vLevel.set(value);
				break;
			case kMetallic:
				vMetallic.set(value);
				break;
			case kMorph:
				vMorph.set(value);
				break;
			case kBlur:
				vBlur.set(value);
				break;
			case kStructure:
				vStructure.set(value);
				break;
			case kBright:
				vBright.set(value);
				break;
			case kOrganicRatio:
				vOrganicRatio.set(value);
				break;
			case kOrganicAmp:
				vOrganicAmp.set(value);
				break;
			case kOrganicDecay:
				vOrganicDecay.set(value);
				break;

			case kDecays:
				vDecays.set(value);
				break;


			case kVelStruct:
				vVelStruct.set(value);
				break;
			case kVelMetal:
				vVelMetal.set(value);
				break;
			case kVelBright:
				vVelBright.set(value);
				break;
			case kVelDecays:
				vVelDecay.set(value);
				break;

			case kStiffness:
				vStiffness.set(value);
				break;
			}
		}

		void setAllClean() {
			vLevel.setClean();
			vDecays.setClean();
			vBright.setClean();
			vMetallic.setClean();
			vMorph.setClean();
			vBlur.setClean();
			vStructure.setClean();

			vOrganicAmp.setClean();
			vOrganicRatio.setClean();
			vOrganicDecay.setClean();


			vVelStruct.setClean();
			vVelMetal.setClean();
			vVelBright.setClean();
			vVelDecay.setClean();

			vStiffness.setClean();
		}
		

	

		//Parameters-----
		ParamUnit vLevel{ kLevel, 0.8, false };
		ParamUnit vDecays{ kDecays, 0.5, false };
		ParamUnit vBright{ kBright, 0.5, false };
		ParamUnit vMetallic{ kMetallic, 0.5, false };
		ParamUnit vMorph{ kMorph, 0.5, false };

		ParamUnit vBlur{ kBlur, 0.0, false };
		ParamUnit vStructure{ kStructure, 0.5, false };

		ParamUnit vOrganicAmp{ kOrganicAmp, 0.0, false };
		ParamUnit vOrganicRatio{ kOrganicRatio, 0.0, false };
		ParamUnit vOrganicDecay{ kOrganicDecay, 0.0, false };


		ParamUnit vVelStruct{ kVelStruct, 0.5, false };
		ParamUnit vVelMetal{ kVelMetal, 0.5, false };
		ParamUnit vVelBright{ kVelBright, 0.5, false };
		ParamUnit vVelDecay{ kVelDecays, 0.5, false };

		ParamUnit vStiffness{ kStiffness, 0.0, false };

	};




}