#include "pluginterfaces/vst/ivstevents.h"
#include "Instrument/LamellaInstrument.h"
#include "Helpers/AudioHelpers.h"

class MidiHandler {
	public:
	

		int nextMidiEvent(int index) {
			Steinberg::Vst::Event Msg;
			Events->getEvent(index, Msg);
			return Msg.sampleOffset;
		}
		void handleEvent(Steinberg::Vst::Event& Msg, LAMELLA_INST::LamellaInstrument* Instrument) {
	
			if (Msg.type == Steinberg::Vst::Event::kNoteOnEvent) {
				Instrument->noteOn(Msg.noteOn.pitch, Msg.noteOn.velocity);

			}
		
		}
		/// <summary>
		/// Checks midi and processes a block, sample accurate midi
		/// TODO Clearer naming convention
		/// </summary>
		/// <param name="data"></param>
		/// <param name="Buffer"></param>
		void processMidi(Steinberg::Vst::ProcessData& data, LAMELLA_INST::LamellaInstrument* Instrument, float* Buffer) {

			Events = data.inputEvents;

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
				handleEvent(Msg, Instrument);


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
		Steinberg::Vst::IEventList* Events = nullptr;
		LAMELLA_INST::ProcessInfo Info;


};


