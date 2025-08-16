#pragma once

#include "vstgui4/vstgui/vstgui.h"

#include "vstgui4/vstgui/uidescription/icontroller.h"
#include "vstgui4/vstgui/uidescription/delegationcontroller.h"

#include "PartialGraph.h"

class PartialGraphController : public VSTGUI::DelegationController {

public:
	explicit PartialGraphController(IController* parent) : DelegationController(parent) {};

	void forgetView(VSTGUI::CView* view)
	{
		if (view == myGraph)
		{
			myGraph = nullptr;
		}
	}

	VSTGUI::CView* verifyView(VSTGUI::CView* view, const VSTGUI::UIAttributes& attributes, const VSTGUI::IUIDescription* description) override {

		if (auto graph = dynamic_cast<VSTGUI::PartialGraphView*>(view)) {

			myGraph = graph;
		
		}

		return view;
	};

	
	// TODO Called when the view is about to be destroyed
	//void viewWillDelete(VSTGUI::CView* view) override;

	// Public method your EditController will call
	void sendDataToView(const float* amps, const float* ratios, const float* decays, int numPartials) {
		if (myGraph) { // ALWAYS check if the pointer is valid!

			myGraph->setNumPartials(numPartials);
			
			myGraph->setDecayData(decays, numPartials);

			myGraph->setAmpData(amps, numPartials);

			myGraph->setRatioData(ratios, numPartials);

			myGraph->setDirty(true);

		}
	};


private:
	VSTGUI::PartialGraphView* myGraph = nullptr;
	



};


