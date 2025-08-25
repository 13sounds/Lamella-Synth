#pragma once
#include "pluginterfaces/vst/vsttypes.h"
#include "public.sdk/source/vst/utility/dataexchange.h"

#define NUM_PARTIALS 16
#define NUM_INFO_VALS 8

struct UIInfoBlock {
	enum BlockSetupData {
		kNumPartials = 0,
		kSampleRate,
		kNumVoices,
		kNumActiveVoices
	};

	float AmpData[NUM_PARTIALS] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	float RatioData[NUM_PARTIALS] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	float DecayData[NUM_PARTIALS] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	float SetupData[NUM_INFO_VALS] = { 0,0,0,0,0,0,0,0 };
};

inline UIInfoBlock* toDataBlock(const Steinberg::Vst::DataExchangeBlock& block)
{
	if (block.blockID != Steinberg::Vst::InvalidDataExchangeBlockID)
		return reinterpret_cast<UIInfoBlock*> (block.data);
	return nullptr;
}