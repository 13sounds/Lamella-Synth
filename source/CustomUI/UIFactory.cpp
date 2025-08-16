

#include "vstgui/vstgui.h"
#include "vstgui/vstgui_uidescription.h"
#include "vstgui/uidescription/detail/uiviewcreatorattributes.h"

#include "vstgui/uidescription/uiattributes.h" 

#include "PartialGraph.h"

#define ATTR_BG_COLOR  "draw-bg-color"
#define ATTR_BAR_COLOR  "draw-bar-color"
#define ATTR_TAG	   "control-tag"
#define ATTR_BARWIDTH "partial-bar-width"
#define ATTR_ROUNDNESS "partial-rect-roundness"

namespace VSTGUI {

	

	class PartialGraphFactory : public ViewCreatorAdapter {
	public:
		

		PartialGraphFactory() {
			UIViewFactory::registerViewCreator(*this);

			
		}

		IdStringPtr getViewName() const {
			return "Partial Graph";
		}
		IdStringPtr getBaseViewName() const {
			return UIViewCreator::kCView;
		}
		CView* create(const UIAttributes& attributes, const IUIDescription* description) const override {
			CRect size(0, 0, 100, 100);
			
			auto myView = new PartialGraphView(size);
	
			return myView;
			
		}

		bool apply(CView* view, const UIAttributes& attributes, const IUIDescription* description) const override
		{
			ViewCreatorAdapter::apply(view, attributes, description);

			
			
			PartialGraphView* partialGraph = dynamic_cast<PartialGraphView*>(view);
			if (!partialGraph) {
				return false;
			}

				
			// Only apply attributes that are custom PartialGraphView
	
			CColor bgColor;
			const std::string* bgColorName = attributes.getAttributeValue(ATTR_BG_COLOR);
			if (bgColorName && description->getColor(bgColorName->data(), bgColor)) {
				partialGraph->setBGColor(bgColor);
				partialGraph->setDirty(true);
			}
			CColor barColor;
			const std::string* fmColorName = attributes.getAttributeValue(ATTR_BAR_COLOR);
			if (fmColorName && description->getColor(fmColorName->data(), barColor)) {
				partialGraph->setFMColor(barColor);
				partialGraph->setDirty(true);
			}


			int controlTag = -99;
			attributes.getIntegerAttribute(ATTR_TAG, controlTag);
			if (controlTag > -1) {
				partialGraph->setTag(controlTag);
				partialGraph->setDirty(true);

			}
			
			double roundFactor = 0.5;
			if (attributes.getDoubleAttribute(ATTR_ROUNDNESS, roundFactor)) {
		
				partialGraph->setRoundnessNormalised(roundFactor);
				partialGraph->setDirty(true);

			}
			double barWidth = 0.5;
			if (attributes.getDoubleAttribute(ATTR_BARWIDTH, barWidth)) {

				partialGraph->setBarWidthNormalised(barWidth);
				partialGraph->setDirty(true);

			}
			
			return true;
		}

		bool getAttributeNames(StringList& attributeNames) const override {
			attributeNames.emplace_back(ATTR_BG_COLOR);
			attributeNames.emplace_back(ATTR_BAR_COLOR);
			attributeNames.emplace_back(ATTR_TAG);
			attributeNames.emplace_back(ATTR_BARWIDTH);
			attributeNames.emplace_back(ATTR_ROUNDNESS);

			return ViewCreatorAdapter::getAttributeNames(attributeNames);
		}
		AttrType getAttributeType(const string& attributeName) const override {
			if (strcmp(attributeName.data(), ATTR_BG_COLOR) == 0) {
				return AttrType::kColorType;
			}
			if (strcmp(attributeName.data(), ATTR_BAR_COLOR) == 0) {
				return AttrType::kColorType;
			}
			if (strcmp(attributeName.data(), ATTR_BARWIDTH) == 0) {
				return AttrType::kFloatType;
			}
			if (strcmp(attributeName.data(), ATTR_ROUNDNESS) == 0) {
				return AttrType::kFloatType;
			}
			if (strcmp(attributeName.data(), ATTR_BARWIDTH) == 0) {
				return AttrType::kFloatType;
			}
			return ViewCreatorAdapter::getAttributeType(attributeName);
		}
		bool getPossibleListValues(const string& attributeName,
			ConstStringPtrList& values) const override {

			

			return ViewCreatorAdapter::getPossibleListValues(attributeName,values);
		}
		bool getAttributeValue(CView* view, const string& attributeName, string& stringValue,
			const IUIDescription* desc) const override
		{
			PartialGraphView* myView = dynamic_cast<PartialGraphView*>(view);

			if (attributeName == ATTR_ROUNDNESS) {
				stringValue = std::to_string(myView->getRoundnessNormalised());
				return true;
			}
			if (attributeName == ATTR_BARWIDTH) {
				stringValue = std::to_string(myView->getBarWidth());
				return true;
			}
	
			if (attributeName == ATTR_BG_COLOR) {
				stringValue = myView->getBGColor().toString();
				return true;
			}
			if (attributeName == ATTR_BAR_COLOR) {
				stringValue = myView->getFMColor().toString();
				return true;
			}
			if (attributeName == ATTR_TAG) {
				stringValue = std::to_string(myView->getTag());
				return true;
			}


			return ViewCreatorAdapter::getAttributeValue(view, attributeName,stringValue, desc);
		}

	private:
		
	};

	PartialGraphFactory __gNewPartialGraphFactory;

}