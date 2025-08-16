//------------------------------------------------------------------------
// Copyright(c) 2025 Dadou.
//------------------------------------------------------------------------

#include "controller.h"
#include "cids.h"
#include "vstgui/plugin-bindings/vst3editor.h"
#include "Instrument/ParamRegistration.h"

using namespace Steinberg;

namespace LAMELLA_INST {

	//------------------------------------------------------------------------
	// LamellaController Implementation
	//------------------------------------------------------------------------
	tresult PLUGIN_API LamellaController::initialize(FUnknown* context)
	{
		// Here the Plug-in will be instantiated

		//---do not forget to call parent ------
		tresult result = EditControllerEx1::initialize(context);
		if (result != kResultOk)
		{
			return result;
		}

		registerParameters(parameters);

		// Default 16
		RxPartialAmplitudes.resize(16);
		RxPartialRatios.resize(16);
		RxPartialDecays.resize(16);


		return result;
	}

	//------------------------------------------------------------------------
	tresult PLUGIN_API LamellaController::terminate()
	{
		// Here the Plug-in will be de-instantiated, last possibility to remove some memory!

		//---do not forget to call parent ------
		return EditControllerEx1::terminate();
	}

	//------------------------------------------------------------------------
	tresult PLUGIN_API LamellaController::setComponentState(IBStream* state)
	{
		// Here you get the state of the component (Processor part)
		if (!state)
			return kResultFalse;

		return kResultOk;
	}

	//------------------------------------------------------------------------
	tresult PLUGIN_API LamellaController::setState(IBStream* state)
	{
		// Here you get the state of the controller

		return kResultTrue;
	}

	//------------------------------------------------------------------------
	tresult PLUGIN_API LamellaController::getState(IBStream* state)
	{
		// Here you are asked to deliver the state of the controller (if needed)
		// Note: the real state of your plug-in is saved in the processor

		return kResultTrue;
	}

	//------------------------------------------------------------------------
	IPlugView* PLUGIN_API LamellaController::createView(FIDString name)
	{
		// Here the Host wants to open your editor (if you have one)
		if (FIDStringsEqual(name, Vst::ViewType::kEditor))
		{
			// create your editor here and return a IPlugView ptr of it
			auto* view = new VSTGUI::VST3Editor(this, "view", "editor.uidesc");
			return view;
		}
		return nullptr;
	}

	//------------------------------------------------------------------------
	tresult PLUGIN_API LamellaController::notify(Steinberg::Vst::IMessage* message)
	{
		if (dataExchange.onMessage(message))
			return kResultTrue;
		return EditControllerEx1::notify(message);
	}

	void PLUGIN_API LamellaController::queueOpened(Steinberg::Vst::DataExchangeUserContextID userContextID,
		uint32 blockSize,
		TBool& dispatchOnBackgroundThread)
	{
		FDebugPrint("Data Exchange Queue opened.\n");
	}

	void PLUGIN_API LamellaController::queueClosed(Steinberg::Vst::DataExchangeUserContextID userContextID)
	{
		FDebugPrint("Data Exchange Queue closed.\n");
	}

	void PLUGIN_API LamellaController::onDataExchangeBlocksReceived(
		Steinberg::Vst::DataExchangeUserContextID userContextID, uint32 numBlocks, Steinberg::Vst::DataExchangeBlock* blocks,
		TBool onBackgroundThread)
	{
		if (numBlocks == 0) return;
		if (!blocks) return;

		for (auto index = 0u; index < numBlocks; ++index)
		{
			auto dataBlock = toDataBlock(blocks[index]);
			if (dataBlock) {

				numPartials = 16;

				for (int i = 0; i < numPartials; i++) {
					RxPartialAmplitudes[i] = dataBlock->AmpData[i];
					RxPartialRatios[i] = dataBlock->RatioData[i];
					RxPartialDecays[i] = dataBlock->DecayData[i];
				}

			}

		}

		if (pGraphController) {
			pGraphController->sendDataToView(RxPartialAmplitudes.data(), RxPartialRatios.data(), RxPartialDecays.data(), numPartials);

		}


	}
	//------------------------------------------------------------------------
} // namespace ADADOU
