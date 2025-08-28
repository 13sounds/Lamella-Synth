#pragma once


#include <vstgui/vstgui.h>
#include <vector>



namespace VSTGUI {


	class PartialGraphView : public VSTGUI::CView {

	public:
		PartialGraphView(const VSTGUI::CRect& size) : CView(size) {
			viewSize = size;

			partialAmps.resize(64);
			partialRatios.resize(64);
			partialDecays.resize(64);


			for (int i = 0; i < 64; i++) {
				partialAmps[i] = 0;
				partialRatios[i] = 0;
				partialDecays[i] = 0;
			}
		}

		void setNumPartials(int num) {
			

		}
		void setAmpData(const float* values, int num_partials) {

			//TODO size pointer issue when closing window

			for (int i = 0; i < numPartials; i++) {
				partialAmps[i] = values[i];
			}
		}
		void setRatioData(const float* values, int num_partials) {
			for (int i = 0; i < numPartials; i++) {
				partialRatios[i] = values[i];
			}
		}
		void setDecayData(const float* values, int num_partials) {

			if (!values) {
				return;
			}

			for (int i = 0; i < numPartials; i++) {
				partialDecays[i] = values[i];

			}
		}

		void draw(VSTGUI::CDrawContext* pContext) override;



		void setBGColor(VSTGUI::CColor color) {
			bgColor = color;
			setDirty();
		}
		void setFMColor(VSTGUI::CColor color) {
			barColor = color;
			setDirty();
		}
		void setTag(int tag) {
			controlTag = tag;
			setDirty();
		}
		int getTag() const {
			return controlTag;
		}

		void setRoundnessNormalised(float value) {
			mRoundnessFactor = value;
		}
		void setBarWidthNormalised(float value) {
			mBarWidth = value;
		}
		// Getters
		float getRoundnessNormalised() const {
			return mRoundnessFactor;
		}
		float getBarWidth() const {
			return mBarWidth;

		}
		CColor getBGColor() const {
			return bgColor;
		}
		CColor getFMColor() const {
			return barColor;
		}
	private:
		VSTGUI::CRect viewSize;
		VSTGUI::CColor bgColor = VSTGUI::kBlackCColor;
		VSTGUI::CColor barColor = VSTGUI::kBlueCColor;
		VSTGUI::CColor frameColor = VSTGUI::kTransparentCColor;

		int controlTag = -1;

		

		int numPartials = 16;
		std::vector<float> partialAmps;
		std::vector<float> partialRatios;
		std::vector<float> partialDecays;


		//CLASS_METHODS(PartialGraphView, CView);

		float mframeThickness = 0.2;
		float mRoundnessFactor = 0.5;
		float mBarWidth = 0.02;

	};

};