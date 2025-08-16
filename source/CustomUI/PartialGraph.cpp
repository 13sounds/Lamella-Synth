#include "PartialGraph.h"



	namespace VSTGUI {


		void PartialGraphView::draw(VSTGUI::CDrawContext* pContext) {
			// Adjust to relative point
			auto viewPos = getViewSize().getTopLeft();
			CDrawContext::Transform t(*pContext, CGraphicsTransform().translate(viewPos));
			// Create a rect (viewBounds) based on this relative point, add allowance for frame
			CRect viewNaive = getViewSize();
			const CRect viewBounds(0, 0, viewNaive.getWidth(), viewNaive.getHeight());

			
			// BG
			pContext->resetClipRect();
			pContext->setFillColor(bgColor);
			pContext->setLineStyle(kLineSolid);

			pContext->drawRect(viewBounds, VSTGUI::kDrawFilled);


			// Partials
			pContext->setFillColor(barColor);

			const float frameClearance = 0.05; // 5% clearance around edges

			// Get max ratio
			float maxRatio = 0;
			float maxAmp = 0;
			float maxDec = 0;

			for (int i = 0; i < numPartials; i++) {
				if (partialRatios[i] > maxRatio) {
					maxRatio = partialRatios[i];
				}
				if (partialAmps[i] > maxAmp) {
					maxAmp = partialAmps[i];
				}
				if (partialDecays[i] > maxDec) {
					maxDec = partialDecays[i];
				}
			}



			for (int i = 0; i < numPartials; i++) {

				// set color offset
				CColor thisColor = barColor;
				pContext->setFillColor(thisColor);
				if (maxDec > 0.0f) {
					thisColor = barColor;
					float decayPc = partialDecays[i] / maxDec;
					thisColor.red *= decayPc;
					//thisColor.green *= decayPc;
					pContext->setFillColor(thisColor);
				}



				float width = 0.2 * mBarWidth;

				// Do calculation normalised to square dim 1 : 1
				float top_x = partialRatios[i] / maxRatio; // relative ratio percentage
		
				top_x = top_x - (1.0f / maxRatio);
				
				
				// Scale from 1st
				top_x = top_x + frameClearance;
				top_x *= (1.0f - frameClearance);

				float height = 1.0f - (partialAmps[i]);

				// Log scale height
				height = powf(height, 2.71828f);
				height += frameClearance; // Border at top


				float bottom_x = top_x + width;

				float bottom_y = 0.95;


				CPoint TopLeft;
				TopLeft.x = top_x * getWidth();
				TopLeft.y = height * getHeight();

				CPoint BotRight;
				BotRight.x = bottom_x * getWidth();
				BotRight.y = bottom_y * getHeight();

				LinePair Pair(TopLeft, BotRight);


				CRect Rect;
				Rect.setTopLeft(TopLeft);
				Rect.setBottomRight(BotRight);

				if (TopLeft.y > BotRight.y) {
					continue;
				}

				float roundRadius = width * getWidth();
				roundRadius *= mRoundnessFactor;
				CGraphicsPath* RRect = pContext->createRoundRectGraphicsPath(Rect, CCoord(roundRadius));
				pContext->drawGraphicsPath(RRect, CDrawContext::kPathFilled);

				RRect->forget();
			}






			setDirty(false);


		}







	};

