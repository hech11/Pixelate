#include "PXpch.h"

#include "Pixelate/Audio/AudioPlatformUtils.h"

#include <Windows.h>
#include <mmdeviceapi.h> 
#include <endpointvolume.h>

namespace Pixelate {

	static IMMDeviceEnumerator* s_DeviceEnumerator = 0;
	static IMMDevice* s_Device = 0;

	static IAudioEndpointVolume* s_EndpointVolume = 0;
	static IAudioMeterInformation* s_MeterInfo = 0;

// 	void Audio::InitOSUtilities() {
// 		CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&s_DeviceEnumerator);
// 		s_DeviceEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &s_Device);
// 
// 		//s_Device->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, NULL, (void**)&s_EndpointVolume);
// 		s_Device->Activate(__uuidof(IAudioMeterInformation), CLSCTX_ALL, NULL, (void**)&s_MeterInfo);
// 
// 	}

	void AudioPlatformUtils::Init()
	{
		CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&s_DeviceEnumerator);
		s_DeviceEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &s_Device);

		//s_Device->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, NULL, (void**)&s_EndpointVolume);
		if(s_Device)
			s_Device->Activate(__uuidof(IAudioMeterInformation), CLSCTX_ALL, NULL, (void**)&s_MeterInfo);
	}



	void AudioPlatformUtils::Shutdown() {
		if (s_Device)
		{
			s_MeterInfo->Release();
			//s_EndpointVolume->Release();
			s_Device->Release();
			s_DeviceEnumerator->Release();
		}

	}



	float AudioPlatformUtils::GetPeakValue() {
		
		float result = 0.0f;
		if (s_Device)
		{
			s_MeterInfo->GetPeakValue(&result);
		}

		return result;
	}
}