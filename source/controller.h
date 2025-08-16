//------------------------------------------------------------------------
// Copyright(c) 2025 Dadou.
//------------------------------------------------------------------------

#pragma once

#include "public.sdk/source/vst/vsteditcontroller.h"

#include "public.sdk/source/vst/utility/dataexchange.h"
#include "Helpers/AudioHelpers.h"

#include "Instrument/ParamRegistration.h"
#include "Helpers/DataExchangeInfo.h"
#include <vector>
#include "vstgui/plugin-bindings/vst3editor.h"
#include "CustomUI/TestSubController.h"

namespace LAMELLA_INST {

	//------------------------------------------------------------------------
	//  LamellaController
	//------------------------------------------------------------------------
	class LamellaController : public Steinberg::Vst::EditControllerEx1, public VSTGUI::VST3EditorDelegate, public Steinberg::Vst::IDataExchangeReceiver
	{
	public:
		//------------------------------------------------------------------------
		LamellaController() = default;
		~LamellaController() SMTG_OVERRIDE = default;

		// Create function
		static Steinberg::FUnknown* createInstance(void* /*context*/)
		{
			return (Steinberg::Vst::IEditController*)new LamellaController;
		}

		//--- from IPluginBase -----------------------------------------------
		Steinberg::tresult PLUGIN_API initialize(Steinberg::FUnknown* context) SMTG_OVERRIDE;
		Steinberg::tresult PLUGIN_API terminate() SMTG_OVERRIDE;

		//--- from EditController --------------------------------------------
		Steinberg::tresult PLUGIN_API setComponentState(Steinberg::IBStream* state) SMTG_OVERRIDE;
		Steinberg::IPlugView* PLUGIN_API createView(Steinberg::FIDString name) SMTG_OVERRIDE;
		Steinberg::tresult PLUGIN_API setState(Steinberg::IBStream* state) SMTG_OVERRIDE;
		Steinberg::tresult PLUGIN_API getState(Steinberg::IBStream* state) SMTG_OVERRIDE;

		//---Interface---------
		DEFINE_INTERFACES
			// Here you can add more supported VST3 interfaces
			// DEF_INTERFACE (Vst::IXXX)
			END_DEFINE_INTERFACES(EditController)
			DELEGATE_REFCOUNT(EditController)

			//--- Data Exchange
			Steinberg::tresult PLUGIN_API notify(Steinberg::Vst::IMessage* message);
		void PLUGIN_API queueOpened(Steinberg::Vst::DataExchangeUserContextID userContextID,
			Steinberg::uint32 blockSize,
			Steinberg::TBool& dispatchOnBackgroundThread);

		void PLUGIN_API queueClosed(Steinberg::Vst::DataExchangeUserContextID userContextID);
		void PLUGIN_API onDataExchangeBlocksReceived(
			Steinberg::Vst::DataExchangeUserContextID userContextID, Steinberg::uint32 numBlocks, Steinberg::Vst::DataExchangeBlock* blocks,
			Steinberg::TBool onBackgroundThread);

		//-----------------------------------------
		VSTGUI::IController* createSubController(VSTGUI::UTF8StringPtr name, const VSTGUI::IUIDescription* description,
			VSTGUI::VST3Editor* editor) override {
			if (strcmp(name, "GraphController") == 0) {

				//VSTGUI::IController* parent = dynamic_cast<VSTGUI::IController*>(this);
				PartialGraphController* graphController = new PartialGraphController(editor);

				pGraphController = graphController;

				return graphController;
			}
			return nullptr;
		}



		//------------------------------------------------------------------------
	protected:
		PartialGraphController* pGraphController = nullptr;

		Steinberg::Vst::DataExchangeReceiverHandler dataExchange{ this };


		int numPartials = 16;
		std::vector<float> RxPartialAmplitudes;
		std::vector<float> RxPartialRatios;
		std::vector<float> RxPartialDecays;
	};

	//------------------------------------------------------------------------
} // namespace ADADOU
