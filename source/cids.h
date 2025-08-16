//------------------------------------------------------------------------
// Copyright(c) 2025 Dadou.
//------------------------------------------------------------------------

#pragma once

#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/vst/vsttypes.h"

namespace LAMELLA_INST {
//------------------------------------------------------------------------
static const Steinberg::FUID kLamellaProcessorUID (0x80AE2F99, 0xD644575B, 0x8C0E1F72, 0x08375B2B);
static const Steinberg::FUID kLamellaControllerUID (0x93EF25D4, 0x21FE5214, 0xBCC0D263, 0x62651386);

#define LamellaVST3Category "Instrument"

//------------------------------------------------------------------------
} // namespace LAMELLA_INST
