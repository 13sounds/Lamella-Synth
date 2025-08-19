#include "pluginterfaces/vst/ivstevents.h"
#include "Instrument/LamellaInstrument.h"
#include "Helpers/AudioHelpers.h"

class MidiHandler {
	public:
		void setEventQueue(Steinberg::Vst::IEventList* events) {
			Events = events;
		}
		void setInstrumentPointer(LAMELLA_INST::LamellaInstrument* instrument) {
			Instrument = instrument;
		}
		int nextMidiEvent(int index) {
			Steinberg::Vst::Event Msg;
			Events->getEvent(index, Msg);
			return Msg.sampleOffset;
		}
		void handleEvent(Steinberg::Vst::Event& Msg) {
	
			if (Msg.type == Steinberg::Vst::Event::kNoteOnEvent) {
				Instrument->noteOn(Msg.noteOn.pitch, Msg.noteOn.velocity);

			}
		
		}

		void processMidi(Steinberg::Vst::ProcessData& data, float* Buffer) {

			int numEvents = Events->getEventCount();
			int nextEvent = 0;
			int currentIndex = 0;
			Steinberg::Vst::Event Msg;
			for (int i = 0; i < numEvents; i++) {

				nextEvent = nextMidiEvent(i);

				// Render up to next event
				Info.numSamples = nextEvent - currentIndex;
				Info.startIndex = currentIndex;
				Instrument->getBlock(Buffer, Info);

				currentIndex = Info.numSamples + Info.startIndex;

				// Do the midi
				Events->getEvent(i, Msg);
				handleEvent(Msg);


			}

			int samplesRemaining = data.numSamples - currentIndex;
			if (samplesRemaining == 0) {
				return;
			}

			Info.numSamples = samplesRemaining;
			Info.startIndex = currentIndex;
			Instrument->getBlock(Buffer, Info);




			return;

		}


	private:
		Steinberg::Vst::IEventList* Events;
		LAMELLA_INST::LamellaInstrument* Instrument;
		LAMELLA_INST::ProcessInfo Info;


};


