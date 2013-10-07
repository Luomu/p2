#pragma once
/*
 * Oculus Rift integration
 */
#include "Common.h"
#include <OVR.h>

class Oculus : public OVR::MessageHandler {
public:
	enum StereoMode {
		STEREO_NONE,
		STEREO_LEFT,
		STEREO_RIGHT
	};

	Oculus();
	~Oculus();

    virtual void OnMessage(const OVR::Message& msg);

	glm::quat GetOrientation();
	glm::mat4 GetLookat();

	OVR::Util::Render::StereoEyeParams
		GetEyeParams(OVR::Util::Render::StereoEye);

private:
	OVR::Ptr<OVR::DeviceManager> m_manager;
	OVR::Ptr<OVR::SensorDevice>  m_sensor;
	OVR::Ptr<OVR::HMDDevice>     m_hmd;
	OVR::SensorFusion            m_sfusion;
	OVR::HMDInfo                 m_hmdinfo;

	OVR::Util::Render::StereoConfig     m_sconfig;
	OVR::Util::Render::DistortionConfig m_dconfig;

	glm::quat m_lastOrient;
};