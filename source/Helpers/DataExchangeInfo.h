#pragma once
#include "pluginterfaces/vst/vsttypes.h"
#include "public.sdk/source/vst/utility/dataexchange.h"

struct UIInfoBlock {
	enum BlockSetupData {
		kNumPartials = 1,
		kSampleRate
	};

	float AmpData[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	float RatioData[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	float DecayData[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	float SetupData[8] = { 0,0,0,0,0,0,0,0 };
};

inline UIInfoBlock* toDataBlock(const Steinberg::Vst::DataExchangeBlock& block)
{
	if (block.blockID != Steinberg::Vst::InvalidDataExchangeBlockID)
		return reinterpret_cast<UIInfoBlock*> (block.data);
	return nullptr;
}