#pragma once

#include "Helpers/AudioHelpers.h"
#include "Instrument/LamellaInstrument.h"
#include "Instrument/LamellaVoice.h"
#include "Helpers/DataExchangeInfo.h"

#define NUM_VOICES 8
#define NUM_PARTIALS 16
/// <summary>
/// The voice manager for the LamellaVoice classes
/// </summary>
namespace LAMELLA_INST {

	class LamellaInstrument {
	public:
		/// <summary>
		/// Get sample rate and block size from the processor
		/// </summary>
		/// <param name="sample_rate"></param>
		/// <param name="block_size"></param>
		void getSampleRateBlockSize(int sample_rate, int block_size) {
			SetupInfo Setup(sample_rate, block_size);

			for (int i = 0; i < NUM_VOICES; i++) {
				Voices[i].getSampleRateBlockSize(Setup);
				VoiceBuffers[i].resize(Setup.blockSize);
			}
		}

		void getBlock(float* Buffer, ProcessInfo Info) {
			for (int i = 0; i < NUM_VOICES; i++) {
				Voices[i].getBlock(VoiceBuffers[i], Info);
			}
			mixBuffers(Buffer, VoiceBuffers, NUM_VOICES, Info);

			//Master Level control
			const float maxLevelScale = 4.0f;
			multBuffer(Buffer, mMasterLevel * maxLevelScale, Info);
		}
		/// <summary>
		/// Get parameter object from processor
		/// and pass to voices
		/// </summary>
		/// <param name="params"></param>
		void getParameters(LamellaParams& params) {
			// First, get global parameters
			mMasterLevel = params.vLevel.getSmoothed();

			for (int i = 0; i < NUM_VOICES; i++) {
				Voices[i].getParams(params);
			}

			params.setAllClean();
		}

		/// <summary>
		/// Trigger a note on in the oldest voice
		/// </summary>
		/// <param name="note_num"></param>
		/// <param name="velocity"></param>
		void noteOn(int note_num, float velocity) {
			Message Msg;
			Msg.noteNum = note_num;
			Msg.velocity = velocity;


			currentVoice = currentVoice % NUM_VOICES;
			Voices[currentVoice].noteOn(Msg);
			currentVoice++;
			
		}
		/// <summary>
		/// Returns a UIInfo block giving the processor 
		/// access to internal insturment data for the UI
		/// </summary>
		/// <returns> UIInfoBlock from DataExchangeinfo.h</returns>
		UIInfoBlock* getUIData() {

			// get info from voice 1
			float Amps[NUM_PARTIALS];
			float Ratios[NUM_PARTIALS];
			float Decays[NUM_PARTIALS];

			Voices[0].getValuesForUI(Amps, Ratios, Decays);

			UIInfoBlock* Block = new UIInfoBlock;

			for (int i = 0; i < NUM_PARTIALS; i++) {

				Block->AmpData[i] = Amps[i];
				Block->RatioData[i] = Ratios[i];
				Block->DecayData[i] = Decays[i];
				Block->SetupData[Block->kNumPartials] = NUM_PARTIALS;

			}

			return Block;
		}
		

	private:
		LamellaVoice Voices[NUM_VOICES];
		AudioBuffer VoiceBuffers[NUM_VOICES];
		
		int currentVoice = 0;

		float mMasterLevel = 0.5;
	};



}
