#include "RGFpch.h"
#include "AudioTest.h"
#include "MINIAUDIO/miniaudio.h"

#include "RetroGF/Core/Input.h"
#include "RetroGF/Core/KeyCodes.h"

#define DR_FLAC_IMPLEMENTATION
#include "MINIAUDIO/extras/dr_flac.h"  /* Enables FLAC decoding. */
#define DR_MP3_IMPLEMENTATION
#include "MINIAUDIO/extras/dr_mp3.h"   /* Enables MP3 decoding. */
#define DR_WAV_IMPLEMENTATION
#include "MINIAUDIO/extras/dr_wav.h"   /* Enables WAV decoding. */


#define MINIAUDIO_IMPLEMENTATION
#include "MINIAUDIO/miniaudio.h"

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
	ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
	if (pDecoder == NULL) {
		return;
	}

	ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount);

	(void)pInput;
}

static ma_result result;
static ma_decoder decoder;
static ma_device_config deviceConfig;
static ma_device device;



void RGF::Audio::Init(const std::string& filepath) {
	result = ma_decoder_init_file(filepath.c_str(), NULL, &decoder);

	deviceConfig = ma_device_config_init(ma_device_type_playback);
	deviceConfig.playback.format = decoder.outputFormat;
	deviceConfig.playback.channels = decoder.outputChannels;
	deviceConfig.sampleRate = decoder.outputSampleRate;
	deviceConfig.dataCallback = data_callback;
	deviceConfig.pUserData = &decoder;
	if (result != MA_SUCCESS) {
		RGF_CORE_ERROR("Failed to load %s!", filepath.c_str());
	}



}

void RGF::Audio::Play()
{

	if (RGF::Input::IsKeyDown(RGF_KEY_P)) {
		if (ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS) {
			RGF_CORE_ERROR("Failed to open playback device.\n");
			ma_decoder_uninit(&decoder);

		}

		if (ma_device_start(&device) != MA_SUCCESS) {
			RGF_CORE_ERROR("Failed to start playback device.\n");
			ma_device_uninit(&device);
			ma_decoder_uninit(&decoder);
		}


	}

}

void RGF::Audio::Shutdown() {
	ma_device_uninit(&device);
	ma_decoder_uninit(&decoder);

}
