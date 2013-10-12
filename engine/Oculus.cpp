#include "Oculus.h"
#include "OS.h"

using namespace OVR::Util::Render;

Oculus::Oculus()
{
	OVR::System::Init(OVR::Log::ConfigureDefaultLog(OVR::LogMask_All));

	m_manager = *OVR::DeviceManager::Create();
	m_hmd     = *m_manager->EnumerateDevices<OVR::HMDDevice>().CreateDevice();

	if (m_hmd) {
		m_sensor = *m_hmd->GetSensor();
		if (m_hmd->GetDeviceInfo(&m_hmdinfo)) {
			OS::Log("Oculus device name: %s:", m_hmdinfo.DisplayDeviceName);
			m_sconfig.SetHMDInfo(m_hmdinfo);
		}
	} else {
		OS::Log("Oculus: Could not enumerate devices");
	}

	if (!m_hmd && !m_sensor)
		OS::Log("Oculus Rift initialization totally failed");

	if (m_sensor) {
		m_sfusion.AttachToSensor(m_sensor);
		m_sfusion.SetDelegateMessageHandler(this);
		m_sfusion.SetPredictionEnabled(true);
	}

	m_sconfig.SetFullViewport(Viewport(0, 0, 1280, 800));
	m_sconfig.SetStereoMode(Stereo_LeftRight_Multipass);

    if (m_hmdinfo.HScreenSize > 0.0f)
    {
        if (m_hmdinfo.HScreenSize > 0.140f) // 7"
            m_sconfig.SetDistortionFitPointVP(-1.0f, 0.0f);
        else
            m_sconfig.SetDistortionFitPointVP(0.0f, 1.0f);
    }

	m_sconfig.Set2DAreaFov(OVR::DegreeToRad(85.f));
}

Oculus::~Oculus()
{
}

void Oculus::OnMessage(const OVR::Message& msg)
{
	if (msg.Type == OVR::Message_DeviceAdded && msg.pDevice == m_manager)
		OS::Log("Oculus: DeviceManager reported device added.");
	else if (msg.Type == OVR::Message_DeviceRemoved && msg.pDevice == m_manager)
		OS::Log("Oculus: DeviceManager reported device removed.");
	else if (msg.Type == OVR::Message_DeviceAdded && msg.pDevice == m_sensor)
		OS::Log("Oculus: Sensor reported device added.");
	else if (msg.Type == OVR::Message_DeviceRemoved && msg.pDevice == m_sensor)
		OS::Log("Oculus: Sensor reported device removed.");
}

glm::quat Oculus::GetOrientation()
{
	if (m_sensor) {
		OVR::Quatf hmdOrient = m_sfusion.GetOrientation();

		m_lastOrient.w = hmdOrient.w;
		m_lastOrient.x = hmdOrient.x;
		m_lastOrient.y = hmdOrient.y;
		m_lastOrient.z = hmdOrient.z;
	}
	return m_lastOrient;
}

glm::mat4 convert_matrix(const OVR::Matrix4f &from) {
	glm::mat4 to;
	//transpose
	to[0][0] = from.M[0][0];
	to[0][1] = from.M[1][0];
	to[0][2] = from.M[2][0];
	to[0][3] = from.M[3][0];

	to[1][0] = from.M[0][1];
	to[1][1] = from.M[1][1];
	to[1][2] = from.M[2][1];
	to[1][3] = from.M[3][1];

	to[2][0] = from.M[0][2];
	to[2][1] = from.M[1][2];
	to[2][2] = from.M[2][2];
	to[2][3] = from.M[3][2];

	to[3][0] = from.M[0][3];
	to[3][1] = from.M[1][3];
	to[3][2] = from.M[2][3];
	to[3][3] = from.M[3][3];

	return to;
}

glm::mat4 Oculus::GetLookat()
{
	float yaw   = 0.f;
	float pitch = 0.f;
	float roll  = 0.f;
	if (m_sensor) {
		OVR::Quatf hmdOrient = m_sfusion.GetOrientation();

		m_lastOrient.w = hmdOrient.w;
		m_lastOrient.x = hmdOrient.x;
		m_lastOrient.y = hmdOrient.y;
		m_lastOrient.z = hmdOrient.z;

		hmdOrient.GetEulerAngles<OVR::Axis_Y, OVR::Axis_X, OVR::Axis_Z>(&yaw, &pitch, &roll);
	}

	const OVR::Vector3f UpVector(0.f, 1.f, 0.f);
	const OVR::Vector3f ForwardVector(0.f, 0.f, 1.f);

	OVR::Matrix4f rollPitchYaw = OVR::Matrix4f::RotationY(yaw) * OVR::Matrix4f::RotationX(pitch) *
		OVR::Matrix4f::RotationZ(roll);
	OVR::Vector3f up  = rollPitchYaw.Transform(UpVector);
	OVR::Vector3f fwd = rollPitchYaw.Transform(ForwardVector);

	const float headBaseToEyeHeight     = 0.005f;
	const float headBaseToEyeProtrusion = -0.001f;
	const OVR::Vector3f EyePos(0.f, -0.75f, 0.f);
	OVR::Vector3f eyeCenterInHeadFrame(0.f, headBaseToEyeHeight, -headBaseToEyeProtrusion);
	OVR::Vector3f shiftedEyePos = EyePos + rollPitchYaw.Transform(eyeCenterInHeadFrame);
	shiftedEyePos.y -= eyeCenterInHeadFrame.y;

	return convert_matrix(OVR::Matrix4f::LookAtLH(shiftedEyePos, shiftedEyePos + fwd, up));
}

StereoEyeParams Oculus::GetEyeParams(StereoEye stereoMode)
{
	return m_sconfig.GetEyeRenderParams(stereoMode);
}