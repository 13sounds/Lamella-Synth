//------------------------------------------------------------------------
// Copyright(c) 2025 Dadou.
//------------------------------------------------------------------------

#include "processor.h"
#include "cids.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "pluginterfaces/vst/ivstevents.h"

using namespace Steinberg;

namespace LAMELLA_INST {
	//------------------------------------------------------------------------
	// LamellaProcessor
	//------------------------------------------------------------------------
	LamellaProcessor::LamellaProcessor()
	{
		//--- set the wanted controller for our processor
		setControllerClass(kLamellaControllerUID);
	}

	//------------------------------------------------------------------------
	LamellaProcessor::~LamellaProcessor()
	{
	}

	//------------------------------------------------------------------------
	tresult PLUGIN_API LamellaProcessor::initialize(FUnknown* context)
	{
		// Here the Plug-in will be instantiated

		//---always initialize the parent-------
		tresult result = AudioEffect::initialize(context);
		// if everything Ok, continue
		if (result != kResultOk)
		{
			return result;
		}

		//--- create Audio IO ------
		// addAudioInput (STR16 ("Stereo In"), Steinberg::Vst::SpeakerArr::kStereo);
		addAudioOutput(STR16("Stereo Out"), Steinberg::Vst::SpeakerArr::kStereo);

		/* If you don't need an event bus, you can remove the next line */
		addEventInput(STR16("Event In"), 1);

		return kResultOk;
	}

	//------------------------------------------------------------------------
	tresult PLUGIN_API LamellaProcessor::terminate()
	{
		// Here the Plug-in will be de-instantiated, last possibility to remove some memory!

		//---do not forget to call parent ------
		return AudioEffect::terminate();
	}

	//------------------------------------------------------------------------
	tresult PLUGIN_API LamellaProcessor::setActive(TBool state)
	{
		AudioEffect::setActive(state);


		if (state) // When activating
		{
			// processSetup is now valid and can be used here
			dataExchange->onActivate(processSetup, true);
		}
		else // When deactivating
		{
			dataExchange->onDeactivate();
		}

		return kResultTrue;

	}

	void LamellaProcessor::getMidi(Steinberg::Vst::IEventList* Events) {

		int numMsgs = Events->getEventCount();
		if (!numMsgs) {
			return;
		}

		Vst::Event Msg;

		for (int i = 0; i < numMsgs; i++) {
			Events->getEvent(i, Msg);

			if (Msg.type == Vst::Event::kNoteOnEvent) {
				Instrument[0].noteOn(Msg.noteOn.pitch, Msg.noteOn.velocity);
				Instrument[1].noteOn(Msg.noteOn.pitch, Msg.noteOn.velocity);

			}
		}


	}

	//------------------------------------------------------------------------
	tresult PLUGIN_API LamellaProcessor::process(Vst::ProcessData& data)
	{
		//--- First : Read inputs parameter changes-----------

		if (data.inputParameterChanges)
		{
			int32 numParamsChanged = data.inputParameterChanges->getParameterCount();
			for (int32 index = 0; index < numParamsChanged; index++)
			{
				if (auto* paramQueue = data.inputParameterChanges->getParameterData(index))
				{
					Vst::ParamValue value;
					int32 sampleOffset;
					int32 numPoints = paramQueue->getPointCount();
					int32 paramID = paramQueue->getParameterId();
					if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue) {

						c1Params.setParameter(paramID, value);
						c2Params.setParameter(paramID, value);
					}
				}
			}
		}

		// Send to instrument
		Instrument[0].getParameters(c1Params);
		Instrument[1].getParameters(c2Params);


		//--- Here you have to implement your processing
		//-------MIDI-------------------------------------
		if (data.inputEvents) {
			getMidi(data.inputEvents);
		}
		//-------------------------------------------------
		if (data.numSamples > 0)
		{
			if (!data.numOutputs) {
				return kResultOk;
			}

			if (data.outputs[0].numChannels != 2) {
				return kResultOk;
			}




			ProcessInfo Info(data.numSamples, 0);

			for (int i = 0; i < data.outputs[0].numChannels; i++) {
				float* pBuffer = data.outputs[0].channelBuffers32[i];
				Instrument[i].getBlock(pBuffer, Info);

			}






		}


		//-Dat Exchange--------------------------------------

		counter = counter + data.numSamples;


		if (counter >= (48000) / 15) {
			counter = 0;
			acquireNewExchangeBlock();
			UIInfoBlock* block = nullptr;
			if (auto block = toDataBlock(currentExchangeBlock)) {


				memcpy(block, Instrument[0].getUIData(), sizeof(UIInfoBlock));

				if (!dataExchange->sendCurrentBlock())
				{
					// The queue was full. Discard the block to release it back to the pool.
					dataExchange->discardCurrentBlock();
				}

			}

		}

		return kResultOk;
	}

	//------------------------------------------------------------------------
	tresult PLUGIN_API LamellaProcessor::setupProcessing(Vst::ProcessSetup& newSetup)
	{
		//--- called before any processing ----
		Instrument[0].getSampleRateBlockSize(newSetup.sampleRate, newSetup.maxSamplesPerBlock);
		Instrument[1].getSampleRateBlockSize(newSetup.sampleRate, newSetup.maxSamplesPerBlock);

		return AudioEffect::setupProcessing(newSetup);
	}

	//------------------------------------------------------------------------
	tresult PLUGIN_API LamellaProcessor::canProcessSampleSize(int32 symbolicSampleSize)
	{
		// by default kSample32 is supported
		if (symbolicSampleSize == Vst::kSample32)
			return kResultTrue;

		// disable the following comment if your processing support kSample64
		/* if (symbolicSampleSize == Vst::kSample64)
			return kResultTrue; */

		return kResultFalse;
	}

	//------------------------------------------------------------------------
	tresult PLUGIN_API LamellaProcessor::setState(IBStream* state)
	{
		// called when we load a preset, the model has to be reloaded
		IBStreamer streamer(state, kLittleEndian);

		return kResultOk;
	}

	//------------------------------------------------------------------------
	tresult PLUGIN_API LamellaProcessor::getState(IBStream* state)
	{
		// here we need to save the model
		IBStreamer streamer(state, kLittleEndian);

		return kResultOk;
	}


	//--Data Exchange-------------------------------------------------------------

	tresult PLUGIN_API LamellaProcessor::connect(Steinberg::Vst::IConnectionPoint* other)
	{
		auto result = Vst::AudioEffect::connect(other);
		if (result == kResultTrue)
		{
			auto configCallback = [this](Vst::DataExchangeHandler::Config& config,
				const Vst::ProcessSetup& setup) {


					config.blockSize = sizeof(UIInfoBlock);
					config.numBlocks = 8;
					config.alignment = 32;
					config.userContextID = 0;
					return true;
				};

			dataExchange = std::make_unique<Vst::DataExchangeHandler>(this, configCallback);
			dataExchange->onConnect(other, getHostContext());
		}
		return result;
	}


	tresult PLUGIN_API LamellaProcessor::disconnect(Steinberg::Vst::IConnectionPoint* other)
	{
		if (dataExchange)
		{
			dataExchange->onDisconnect(other);
			dataExchange.reset();
		}
		return AudioEffect::disconnect(other);
	}


	void LamellaProcessor::acquireNewExchangeBlock()
	{
		currentExchangeBlock = dataExchange->getCurrentOrNewBlock();
		if (auto block = toDataBlock(currentExchangeBlock))
		{
			for (int i = 0; i < 16; i++) {
				block->AmpData[i] = 0;
				block->RatioData[i] = 0;
				block->DecayData[i] = 0;
			}

		}
	}











	//------------------------------------------------------------------------
} // namespace ADADOU
