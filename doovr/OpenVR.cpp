#define OPENVR_BUILD_STATIC
//#include "OpenVR.h"
//#include <vector>
//#include <openvr/openvr.h>
//#include <memory>
//#include "StaticMesh.h"
//#include "MatrixStack.h"
//#include "DynamicMesh.h"
//#include <assert.h>
//
#define GL_CHECK_ERROR                                     \
do {                                                       \
    GLenum glErr = glGetError();                           \
    if (glErr) {                                           \
        switch (glErr) {                                   \
        case GL_INVALID_ENUM:                              \
            printf("GL Error: Invalid enum");              \
            break;                                         \
        case GL_INVALID_VALUE:                             \
            printf("GL Error: Invalid value");             \
            break;                                         \
        case GL_INVALID_OPERATION:                         \
            printf("GL Error: Invalid operation");         \
            break;                                         \
        case GL_STACK_OVERFLOW:                            \
            printf("GL Error: Stack overflow");            \
            break;                                         \
        case GL_STACK_UNDERFLOW:                           \
            printf("GL Error: Stack underflow");           \
            break;                                         \
        case GL_OUT_OF_MEMORY:                             \
            printf("GL Error: Out of memory");             \
            break;                                         \
        default:                                           \
            printf("GL Error: Unknown error");             \
            break;                                         \
        }                                                  \
        abort();                                           \
    }                                                      \
} while(false)

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include <stdio.h>
#include <string>
#include <cstdlib>
#include <vector>
#include <openvr/openvr.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <thread>
#include <chrono>
#include "Shader.h"
#include "MatrixStack.h"
#include "StaticMesh.h"
#include "DynamicMesh.h"

#ifndef _WIN32
#define APIENTRY
#endif


#ifndef _countof
#define _countof(x) (sizeof(x)/sizeof((x)[0]))
#endif

void ThreadSleep(unsigned long nMilliseconds)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(nMilliseconds));
}

class CGLRenderModel
{
public:
	CGLRenderModel(const std::string& sRenderModelName);
	~CGLRenderModel();

	bool BInit(const vr::RenderModel_t& vrModel, const vr::RenderModel_TextureMap_t& vrDiffuseTexture);
	void Cleanup();
	void Draw();
	const std::string& GetName() const { return m_sModelName; }

private:
	GLuint m_glVertBuffer;
	GLuint m_glIndexBuffer;
	GLuint m_glVertArray;
	GLuint m_glTexture;
	GLsizei m_unVertexCount;
	std::string m_sModelName;
};

static bool g_bPrintf = true;

//-----------------------------------------------------------------------------
// Purpose:
//------------------------------------------------------------------------------
class CMainApplication
{
public:
	CMainApplication(int argc, char* argv[]);
	virtual ~CMainApplication();

	bool BInit();
	bool BInitGL();
	bool BInitCompositor();

	void Shutdown();

	void RunMainLoop();
	void HandleInput();
	void ProcessVREvent(const vr::VREvent_t& event);
	void RenderFrame();

	void SetupScene();

	void RenderControllerAxes();

	bool SetupStereoRenderTargets();
	void SetupCompanionWindow();
	void SetupCameras();

	void RenderStereoTargets();
	void RenderCompanionWindow();
	void RenderScene(vr::Hmd_Eye nEye);

	glm::mat4 GetHMDMatrixProjectionEye(vr::Hmd_Eye nEye);
	glm::mat4 GetHMDMatrixPoseEye(vr::Hmd_Eye nEye);
	glm::mat4 GetCurrentViewProjectionMatrix(vr::Hmd_Eye nEye);
	void UpdateHMDMatrixPose();

	glm::mat4 ConvertSteamVRMatrixToMatrix4(const vr::HmdMatrix34_t& matPose);

	GLuint CompileGLShader(const char* pchShaderName, const char* pchVertexShader, const char* pchFragmentShader);
	bool CreateAllShaders();

	CGLRenderModel* FindOrLoadRenderModel(const char* pchRenderModelName);

private:
	bool m_bDebugOpenGL;
	bool m_bVerbose;
	bool m_bPerf;
	bool m_bVblank;
	bool m_bGlFinishHack;

	vr::IVRSystem* m_pHMD;
	std::string m_strDriver;
	std::string m_strDisplay;
	vr::TrackedDevicePose_t m_rTrackedDevicePose[vr::k_unMaxTrackedDeviceCount];
	glm::mat4 m_rmat4DevicePose[vr::k_unMaxTrackedDeviceCount];

	struct ControllerInfo_t
	{
		vr::VRInputValueHandle_t m_source = vr::k_ulInvalidInputValueHandle;
		vr::VRActionHandle_t m_actionPose = vr::k_ulInvalidActionHandle;
		vr::VRActionHandle_t m_actionHaptic = vr::k_ulInvalidActionHandle;
		glm::mat4 m_rmat4Pose;
		CGLRenderModel* m_pRenderModel = nullptr;
		std::string m_sRenderModelName;
		bool m_bShowController;
	};

	enum EHand
	{
		Left = 0,
		Right = 1,
	};
	ControllerInfo_t m_rHand[2];

private: // bookkeeping
	GLFWwindow* m_pCompanionWindow;
	uint32_t m_nCompanionWindowWidth;
	uint32_t m_nCompanionWindowHeight;


private: // OpenGL bookkeeping
	int m_iValidPoseCount;
	int m_iValidPoseCount_Last;
	bool m_bShowCubes;
	float m_vAnalogValue[2];

	std::string m_strPoseClasses;                            // what classes we saw poses for this frame
	char m_rDevClassChar[vr::k_unMaxTrackedDeviceCount];   // for each device, a character representing its class

	int m_iSceneVolumeWidth;
	int m_iSceneVolumeHeight;
	int m_iSceneVolumeDepth;
	float m_fScaleSpacing;
	float m_fScale;

	int m_iSceneVolumeInit;                                  // if you want something other than the default 20x20x20

	float m_fNearClip;
	float m_fFarClip;

	GLuint m_iTexture;

	unsigned int m_uiVertcount;

	GLuint m_glSceneVertBuffer;
	GLuint m_unSceneVAO;
	GLuint m_unCompanionWindowVAO;
	GLuint m_glCompanionWindowIDVertBuffer;
	GLuint m_glCompanionWindowIDIndexBuffer;
	unsigned int m_uiCompanionWindowIndexSize;

	GLuint m_glControllerVertBuffer;
	GLuint m_unControllerVAO;
	unsigned int m_uiControllerVertcount;

	glm::mat4 m_mat4HMDPose;
	glm::mat4 m_mat4eyePosLeft;
	glm::mat4 m_mat4eyePosRight;

	glm::mat4 m_mat4ProjectionCenter;
	glm::mat4 m_mat4ProjectionLeft;
	glm::mat4 m_mat4ProjectionRight;

	struct VertexDataScene
	{
		float position[3];
		float texCoord[2];
	};

	struct VertexDataWindow
	{
		glm::vec2 position;
		glm::vec2 texCoord;

		VertexDataWindow(glm::vec2 pos, glm::vec2 tex) : position(pos[0], pos[1]), texCoord(tex[0], tex[1]) {}
	};

	GLuint m_unSceneProgramID;
	GLuint m_unCompanionWindowProgramID;
	GLuint m_unControllerTransformProgramID;
	GLuint m_unRenderModelProgramID;

	GLint m_nSceneMatrixLocation;
	GLint m_nControllerMatrixLocation;
	GLint m_nRenderModelMatrixLocation;

	struct FramebufferDesc
	{
		GLuint m_nDepthBufferId;
		GLuint m_nRenderTextureId;
		GLuint m_nRenderFramebufferId;
		GLuint m_nResolveTextureId;
		GLuint m_nResolveFramebufferId;
	};
	FramebufferDesc leftEyeDesc;
	FramebufferDesc rightEyeDesc;

	bool CreateFrameBuffer(int nWidth, int nHeight, FramebufferDesc& framebufferDesc);

	uint32_t m_nRenderWidth;
	uint32_t m_nRenderHeight;

	std::vector< CGLRenderModel* > m_vecRenderModels;

	vr::VRActionHandle_t m_actionHideCubes = vr::k_ulInvalidActionHandle;
	vr::VRActionHandle_t m_actionHideThisController = vr::k_ulInvalidActionHandle;
	vr::VRActionHandle_t m_actionTriggerHaptic = vr::k_ulInvalidActionHandle;
	vr::VRActionHandle_t m_actionAnalongInput = vr::k_ulInvalidActionHandle;

	vr::VRActionSetHandle_t m_actionsetDemo = vr::k_ulInvalidActionSetHandle;

private: // Scene data
	Shader m_sceneShader;
	Shader m_meshShader;
	Shader m_flatShader;

	MatrixStack m_MVstack;

	DynamicMesh* m_modellingMesh;

	GLint m_locationFlatMV;
	GLint m_locationFlatP;
	GLint m_locationFlatLP;
	GLint m_locationFlatLP2;
	GLint m_locationFlatM;
};


//---------------------------------------------------------------------------------------------------------------------
// Purpose: Returns true if the action is active and had a rising edge
//---------------------------------------------------------------------------------------------------------------------
bool GetDigitalActionRisingEdge(vr::VRActionHandle_t action, vr::VRInputValueHandle_t* pDevicePath = nullptr)
{
	vr::InputDigitalActionData_t actionData;
	vr::VRInput()->GetDigitalActionData(action, &actionData, sizeof(actionData), vr::k_ulInvalidInputValueHandle);
	if (pDevicePath)
	{
		*pDevicePath = vr::k_ulInvalidInputValueHandle;
		if (actionData.bActive)
		{
			vr::InputOriginInfo_t originInfo;
			if (vr::VRInputError_None == vr::VRInput()->GetOriginTrackedDeviceInfo(actionData.activeOrigin, &originInfo, sizeof(originInfo)))
			{
				*pDevicePath = originInfo.devicePath;
			}
		}
	}
	return actionData.bActive && actionData.bChanged && actionData.bState;
}


//---------------------------------------------------------------------------------------------------------------------
// Purpose: Returns true if the action is active and had a falling edge
//---------------------------------------------------------------------------------------------------------------------
bool GetDigitalActionFallingEdge(vr::VRActionHandle_t action, vr::VRInputValueHandle_t* pDevicePath = nullptr)
{
	vr::InputDigitalActionData_t actionData;
	vr::VRInput()->GetDigitalActionData(action, &actionData, sizeof(actionData), vr::k_ulInvalidInputValueHandle);
	if (pDevicePath)
	{
		*pDevicePath = vr::k_ulInvalidInputValueHandle;
		if (actionData.bActive)
		{
			vr::InputOriginInfo_t originInfo;
			if (vr::VRInputError_None == vr::VRInput()->GetOriginTrackedDeviceInfo(actionData.activeOrigin, &originInfo, sizeof(originInfo)))
			{
				*pDevicePath = originInfo.devicePath;
			}
		}
	}
	return actionData.bActive && actionData.bChanged && !actionData.bState;
}


//---------------------------------------------------------------------------------------------------------------------
// Purpose: Returns true if the action is active and its state is true
//---------------------------------------------------------------------------------------------------------------------
bool GetDigitalActionState(vr::VRActionHandle_t action, vr::VRInputValueHandle_t* pDevicePath = nullptr)
{
	vr::InputDigitalActionData_t actionData;
	vr::VRInput()->GetDigitalActionData(action, &actionData, sizeof(actionData), vr::k_ulInvalidInputValueHandle);
	if (pDevicePath)
	{
		*pDevicePath = vr::k_ulInvalidInputValueHandle;
		if (actionData.bActive)
		{
			vr::InputOriginInfo_t originInfo;
			if (vr::VRInputError_None == vr::VRInput()->GetOriginTrackedDeviceInfo(actionData.activeOrigin, &originInfo, sizeof(originInfo)))
			{
				*pDevicePath = originInfo.devicePath;
			}
		}
	}
	return actionData.bActive && actionData.bState;
}

//-----------------------------------------------------------------------------
// Purpose: Constructor
//-----------------------------------------------------------------------------
CMainApplication::CMainApplication(int argc, char* argv[])
	: m_pCompanionWindow(NULL)
	//, m_pContext(NULL)
	, m_nCompanionWindowWidth(640)
	, m_nCompanionWindowHeight(320)
	, m_unSceneProgramID(0)
	, m_unCompanionWindowProgramID(0)
	, m_unControllerTransformProgramID(0)
	, m_unRenderModelProgramID(0)
	, m_pHMD(NULL)
	, m_bDebugOpenGL(false)
	, m_bVerbose(false)
	, m_bPerf(false)
	, m_bVblank(false)
	, m_bGlFinishHack(true)
	, m_glControllerVertBuffer(0)
	, m_unControllerVAO(0)
	, m_unSceneVAO(0)
	, m_nSceneMatrixLocation(-1)
	, m_nControllerMatrixLocation(-1)
	, m_nRenderModelMatrixLocation(-1)
	, m_iValidPoseCount(0)
	, m_iValidPoseCount_Last(-1)
	, m_iSceneVolumeInit(20)
	, m_strPoseClasses("")
	, m_bShowCubes(true)
{

	for (int i = 1; i < argc; i++)
	{
		if (!_stricmp(argv[i], "-gldebug"))
		{
			m_bDebugOpenGL = true;
		}
		else if (!_stricmp(argv[i], "-verbose"))
		{
			m_bVerbose = true;
		}
		else if (!_stricmp(argv[i], "-novblank"))
		{
			m_bVblank = false;
		}
		else if (!_stricmp(argv[i], "-noglfinishhack"))
		{
			m_bGlFinishHack = false;
		}
		else if (!_stricmp(argv[i], "-noprintf"))
		{
			g_bPrintf = false;
		}
		else if (!_stricmp(argv[i], "-cubevolume") && (argc > i + 1) && (*argv[i + 1] != '-'))
		{
			m_iSceneVolumeInit = atoi(argv[i + 1]);
			i++;
		}
	}
	// other initialization tasks are done in BInit
	memset(m_rDevClassChar, 0, sizeof(m_rDevClassChar));
};


//-----------------------------------------------------------------------------
// Purpose: Destructor
//-----------------------------------------------------------------------------
CMainApplication::~CMainApplication()
{
	// work is done in Shutdown
	//dprintf("Shutdown");
}


//-----------------------------------------------------------------------------
// Purpose: Helper to get a string from a tracked device property and turn it
//			into a std::string
//-----------------------------------------------------------------------------
std::string GetTrackedDeviceString(vr::TrackedDeviceIndex_t unDevice, vr::TrackedDeviceProperty prop, vr::TrackedPropertyError* peError = NULL)
{
	uint32_t unRequiredBufferLen = vr::VRSystem()->GetStringTrackedDeviceProperty(unDevice, prop, NULL, 0, peError);
	if (unRequiredBufferLen == 0)
		return "";

	char* pchBuffer = new char[unRequiredBufferLen];
	unRequiredBufferLen = vr::VRSystem()->GetStringTrackedDeviceProperty(unDevice, prop, pchBuffer, unRequiredBufferLen, peError);
	std::string sResult = pchBuffer;
	delete[] pchBuffer;
	return sResult;
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
bool CMainApplication::BInit()
{
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}

	// Loading the SteamVR Runtime
	vr::EVRInitError eError = vr::VRInitError_None;
	m_pHMD = vr::VR_Init(&eError, vr::VRApplication_Scene);

	if (eError != vr::VRInitError_None)
	{
		m_pHMD = NULL;
		char buf[1024];
		sprintf_s(buf, sizeof(buf), "Unable to init VR runtime: %s", vr::VR_GetVRInitErrorAsEnglishDescription(eError));
		return false;
	}

	int nWindowPosX = 700;
	int nWindowPosY = 100;
	GLFWmonitor* l_Monitor = NULL;
	m_pCompanionWindow = glfwCreateWindow(m_nCompanionWindowWidth, m_nCompanionWindowHeight, "GLFW Oculus Rift Test", l_Monitor, NULL);

	// Check if window creation was succesfull...
	if (!m_pCompanionWindow) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Make the context current for this window...
	glfwMakeContextCurrent(m_pCompanionWindow);

	// Print some info about the OpenGL context...
	int l_Major = glfwGetWindowAttrib(m_pCompanionWindow, GLFW_CONTEXT_VERSION_MAJOR);
	int l_Minor = glfwGetWindowAttrib(m_pCompanionWindow, GLFW_CONTEXT_VERSION_MINOR);
	int l_Profile = glfwGetWindowAttrib(m_pCompanionWindow, GLFW_OPENGL_PROFILE);
	printf("OpenGL: %d.%d ", l_Major, l_Minor);
	if (l_Major >= 3) { // Profiles introduced in OpenGL 3.0...
		if (l_Profile == GLFW_OPENGL_COMPAT_PROFILE) printf("GLFW_OPENGL_COMPAT_PROFILE\n"); else printf("GLFW_OPENGL_CORE_PROFILE\n");
	}
	printf("Vendor: %s\n", (char*)glGetString(GL_VENDOR));
	printf("Renderer: %s\n", (char*)glGetString(GL_RENDERER));

	glewExperimental = GL_TRUE;
	GLenum nGlewError = glewInit();
	if (nGlewError != GLEW_OK)
	{
		printf("%s - Error initializing GLEW! %s\n", __FUNCTION__, glewGetErrorString(nGlewError));
		return false;
	}
	glGetError(); // to clear the error caused deep in GLEW

	// cube array
	m_iSceneVolumeWidth = m_iSceneVolumeInit;
	m_iSceneVolumeHeight = m_iSceneVolumeInit;
	m_iSceneVolumeDepth = m_iSceneVolumeInit;

	m_fScale = 0.3f;
	m_fScaleSpacing = 4.0f;

	m_fNearClip = 0.1f;
	m_fFarClip = 30.0f;

	m_iTexture = 0;
	m_uiVertcount = 0;

	// 		m_MillisecondsTimer.start(1, this);
	// 		m_SecondsTimer.start(1000, this);

	if (!BInitGL())
	{
		printf("%s - Unable to initialize OpenGL!\n", __FUNCTION__);
		return false;
	}

	if (!BInitCompositor())
	{
		printf("%s - Failed to initialize VR Compositor!\n", __FUNCTION__);
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: Initialize OpenGL. Returns true if OpenGL has been successfully
//          initialized, false if shaders could not be created.
//          If failure occurred in a module other than shaders, the function
//          may return true or throw an error. 
//-----------------------------------------------------------------------------
bool CMainApplication::BInitGL()
{
	if (!CreateAllShaders())
		return false;

	// TODO
	//SetupTexturemaps();
	SetupScene();
	SetupCameras();
	SetupStereoRenderTargets();
	SetupCompanionWindow();

	return true;
}


//-----------------------------------------------------------------------------
// Purpose: Initialize Compositor. Returns true if the compositor was
//          successfully initialized, false otherwise.
//-----------------------------------------------------------------------------
bool CMainApplication::BInitCompositor()
{
	vr::EVRInitError peError = vr::VRInitError_None;

	if (!vr::VRCompositor())
	{
		printf("Compositor initialization failed. See log file for details\n");
		return false;
	}

	return true;
} 

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CMainApplication::Shutdown()
{
	if (m_pHMD)
	{
		vr::VR_Shutdown();
		m_pHMD = NULL;
	}

	for (std::vector< CGLRenderModel* >::iterator i = m_vecRenderModels.begin(); i != m_vecRenderModels.end(); i++)
	{
		delete (*i);
	}
	m_vecRenderModels.clear();

	if (m_pCompanionWindow)
	{
		if (m_bDebugOpenGL)
		{
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_FALSE);
			GL_CHECK_ERROR;
			glDebugMessageCallback(nullptr, nullptr);
			GL_CHECK_ERROR;
		}
		glDeleteBuffers(1, &m_glSceneVertBuffer);
		GL_CHECK_ERROR;

		if (m_unSceneProgramID)
		{
			glDeleteProgram(m_unSceneProgramID);
			GL_CHECK_ERROR;
		}
		if (m_unControllerTransformProgramID)
		{
			glDeleteProgram(m_unControllerTransformProgramID);
			GL_CHECK_ERROR;
		}
		if (m_unRenderModelProgramID)
		{
			glDeleteProgram(m_unRenderModelProgramID);
			GL_CHECK_ERROR;
		}
		if (m_unCompanionWindowProgramID)
		{
			glDeleteProgram(m_unCompanionWindowProgramID);
			GL_CHECK_ERROR;
		}

		glDeleteRenderbuffers(1, &leftEyeDesc.m_nDepthBufferId);
		GL_CHECK_ERROR;
		glDeleteTextures(1, &leftEyeDesc.m_nRenderTextureId);
		GL_CHECK_ERROR;
		glDeleteFramebuffers(1, &leftEyeDesc.m_nRenderFramebufferId);
		GL_CHECK_ERROR;
		glDeleteTextures(1, &leftEyeDesc.m_nResolveTextureId);
		GL_CHECK_ERROR;
		glDeleteFramebuffers(1, &leftEyeDesc.m_nResolveFramebufferId);
		GL_CHECK_ERROR;

		glDeleteRenderbuffers(1, &rightEyeDesc.m_nDepthBufferId);
		GL_CHECK_ERROR;
		glDeleteTextures(1, &rightEyeDesc.m_nRenderTextureId);
		GL_CHECK_ERROR;
		glDeleteFramebuffers(1, &rightEyeDesc.m_nRenderFramebufferId);
		GL_CHECK_ERROR;
		glDeleteTextures(1, &rightEyeDesc.m_nResolveTextureId);
		GL_CHECK_ERROR;
		glDeleteFramebuffers(1, &rightEyeDesc.m_nResolveFramebufferId);
		GL_CHECK_ERROR;

		if (m_unCompanionWindowVAO != 0)
		{
			glDeleteVertexArrays(1, &m_unCompanionWindowVAO);
			GL_CHECK_ERROR;
		}
		if (m_unSceneVAO != 0)
		{
			glDeleteVertexArrays(1, &m_unSceneVAO);
			GL_CHECK_ERROR;
		}
		if (m_unControllerVAO != 0)
		{
			glDeleteVertexArrays(1, &m_unControllerVAO);
			GL_CHECK_ERROR;
		}
	}

	if (m_pCompanionWindow)
	{
		glfwDestroyWindow(m_pCompanionWindow);
		m_pCompanionWindow = NULL;
	}

	glfwTerminate();
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CMainApplication::HandleInput()
{
	glfwPollEvents();

	if (glfwGetKey(m_pCompanionWindow, GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(m_pCompanionWindow, GL_TRUE);
	}

	// Process SteamVR events
	vr::VREvent_t event;
	while (m_pHMD->PollNextEvent(&event, sizeof(event)))
	{
		ProcessVREvent(event);
	}

	// Process SteamVR action state
	// UpdateActionState is called each frame to update the state of the actions themselves. The application
	// controls which action sets are active with the provided array of VRActiveActionSet_t structs.
	vr::VRActiveActionSet_t actionSet = { 0 };
	actionSet.ulActionSet = m_actionsetDemo;
	vr::VRInput()->UpdateActionState(&actionSet, sizeof(actionSet), 1);

	m_bShowCubes = !GetDigitalActionState(m_actionHideCubes);

	vr::VRInputValueHandle_t ulHapticDevice;
	if (GetDigitalActionRisingEdge(m_actionTriggerHaptic, &ulHapticDevice))
	{
		if (ulHapticDevice == m_rHand[Left].m_source)
		{
			vr::VRInput()->TriggerHapticVibrationAction(m_rHand[Left].m_actionHaptic, 0, 1, 4.f, 1.0f, vr::k_ulInvalidInputValueHandle);
		}
		if (ulHapticDevice == m_rHand[Right].m_source)
		{
			vr::VRInput()->TriggerHapticVibrationAction(m_rHand[Right].m_actionHaptic, 0, 1, 4.f, 1.0f, vr::k_ulInvalidInputValueHandle);
		}
	}

	vr::InputAnalogActionData_t analogData;
	if (vr::VRInput()->GetAnalogActionData(m_actionAnalongInput, &analogData, sizeof(analogData), vr::k_ulInvalidInputValueHandle) == vr::VRInputError_None && analogData.bActive)
	{
		m_vAnalogValue[0] = analogData.x;
		m_vAnalogValue[1] = analogData.y;
	}

	m_rHand[Left].m_bShowController = true;
	m_rHand[Right].m_bShowController = true;

	vr::VRInputValueHandle_t ulHideDevice;
	if (GetDigitalActionState(m_actionHideThisController, &ulHideDevice))
	{
		if (ulHideDevice == m_rHand[Left].m_source)
		{
			m_rHand[Left].m_bShowController = false;
		}
		if (ulHideDevice == m_rHand[Right].m_source)
		{
			m_rHand[Right].m_bShowController = false;
		}
	}

	int controllerIt = 0;
	vr::InputPoseActionData_t poseData;

	for (unsigned int id = 0; id < vr::k_unMaxTrackedDeviceCount; id++) {
		vr::ETrackedDeviceClass trackedDeviceClass =
			m_pHMD->GetTrackedDeviceClass(id);
		if (trackedDeviceClass !=
			vr::ETrackedDeviceClass::TrackedDeviceClass_Controller ||
			!m_pHMD->IsTrackedDeviceConnected(id))
			continue;

		if (controllerIt == 2)
			break;

		//Confirmed that the device in question is a connected controller

		//This is all copied from above:
		vr::TrackedDevicePose_t trackedDevicePose;
		vr::VRControllerState_t controllerState;
		m_pHMD->GetControllerStateWithPose(
			vr::TrackingUniverseStanding, id, &controllerState,
			sizeof(controllerState), &trackedDevicePose);

		m_rHand[controllerIt].m_rmat4Pose = ConvertSteamVRMatrixToMatrix4(trackedDevicePose.mDeviceToAbsoluteTracking);

		vr::InputOriginInfo_t originInfo;
		std::string sRenderModelName = GetTrackedDeviceString(id, vr::Prop_RenderModelName_String);
		if (sRenderModelName != m_rHand[controllerIt].m_sRenderModelName)
		{
			m_rHand[controllerIt].m_pRenderModel = FindOrLoadRenderModel(sRenderModelName.c_str());
			m_rHand[controllerIt].m_sRenderModelName = sRenderModelName;
		}

		controllerIt++;
	}
		
	
}

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CMainApplication::RunMainLoop()
{
	while (!glfwWindowShouldClose(m_pCompanionWindow))
	{
		HandleInput();

		RenderFrame();
	}
}


//-----------------------------------------------------------------------------
// Purpose: Processes a single VR event
//-----------------------------------------------------------------------------
void CMainApplication::ProcessVREvent(const vr::VREvent_t& event)
{
	switch (event.eventType)
	{
	case vr::VREvent_TrackedDeviceDeactivated:
	{
		printf("Device %u detached.\n", event.trackedDeviceIndex);
	}
	break;
	case vr::VREvent_TrackedDeviceActivated:
	{
		printf("Device %u atached.\n", event.trackedDeviceIndex);
	}
	break;
	case vr::VREvent_TrackedDeviceUpdated:
	{
		printf("Device %u updated.\n", event.trackedDeviceIndex);
	}
	break;
	}
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CMainApplication::RenderFrame()
{
	// for now as fast as possible
	if (m_pHMD)
	{
		RenderControllerAxes();
		RenderStereoTargets();
		RenderCompanionWindow();

		vr::Texture_t leftEyeTexture = { (void*)(uintptr_t)leftEyeDesc.m_nResolveTextureId, vr::TextureType_OpenGL, vr::ColorSpace_Gamma };
		vr::VRCompositor()->Submit(vr::Eye_Left, &leftEyeTexture);
		vr::Texture_t rightEyeTexture = { (void*)(uintptr_t)rightEyeDesc.m_nResolveTextureId, vr::TextureType_OpenGL, vr::ColorSpace_Gamma };
		vr::VRCompositor()->Submit(vr::Eye_Right, &rightEyeTexture);
	}

	if (m_bVblank && m_bGlFinishHack)
	{
		//$ HACKHACK. From gpuview profiling, it looks like there is a bug where two renders and a present
		// happen right before and after the vsync causing all kinds of jittering issues. This glFinish()
		// appears to clear that up. Temporary fix while I try to get nvidia to investigate this problem.
		// 1/29/2014 mikesart
		glFinish();
		GL_CHECK_ERROR;
	}

	// SwapWindow
	{
		glfwSwapBuffers(m_pCompanionWindow);
	}

	// Clear
	{
		// We want to make sure the glFinish waits for the entire present to complete, not just the submission
		// of the command. So, we do a clear here right here so the glFinish will wait fully for the swap.
		glClearColor(0, 0, 0, 1);
		GL_CHECK_ERROR;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		GL_CHECK_ERROR;
	}

	// Flush and wait for swap.
	if (m_bVblank)
	{
		glFlush();
		GL_CHECK_ERROR;
		glFinish();
		GL_CHECK_ERROR;
	}

	// Spew out the controller and pose count whenever they change.
	if (m_iValidPoseCount != m_iValidPoseCount_Last)
	{
		m_iValidPoseCount_Last = m_iValidPoseCount;

		printf("PoseCount:%d(%s)\n", m_iValidPoseCount, m_strPoseClasses.c_str());
	}

	UpdateHMDMatrixPose();
}


//-----------------------------------------------------------------------------
// Purpose: Compiles a GL shader program and returns the handle. Returns 0 if
//			the shader couldn't be compiled for some reason.
//-----------------------------------------------------------------------------
GLuint CMainApplication::CompileGLShader(const char* pchShaderName, const char* pchVertexShader, const char* pchFragmentShader)
{
	GLuint unProgramID = glCreateProgram();
	GL_CHECK_ERROR;

	GLuint nSceneVertexShader = glCreateShader(GL_VERTEX_SHADER);
	GL_CHECK_ERROR;
	glShaderSource(nSceneVertexShader, 1, &pchVertexShader, NULL);
	GL_CHECK_ERROR;
	glCompileShader(nSceneVertexShader);
	GL_CHECK_ERROR;

	GLint vShaderCompiled = GL_FALSE;
	glGetShaderiv(nSceneVertexShader, GL_COMPILE_STATUS, &vShaderCompiled);
	GL_CHECK_ERROR;
	if (vShaderCompiled != GL_TRUE)
	{
		printf("%s - Unable to compile vertex shader %d!\n", pchShaderName, nSceneVertexShader);
		glDeleteProgram(unProgramID);
		GL_CHECK_ERROR;
		glDeleteShader(nSceneVertexShader);
		GL_CHECK_ERROR;
		return 0;
	}
	glAttachShader(unProgramID, nSceneVertexShader);
	GL_CHECK_ERROR;
	glDeleteShader(nSceneVertexShader); // the program hangs onto this once it's attached
	GL_CHECK_ERROR;

	GLuint  nSceneFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(nSceneFragmentShader, 1, &pchFragmentShader, NULL);
	GL_CHECK_ERROR;
	glCompileShader(nSceneFragmentShader);
	GL_CHECK_ERROR;

	GLint fShaderCompiled = GL_FALSE;
	glGetShaderiv(nSceneFragmentShader, GL_COMPILE_STATUS, &fShaderCompiled);
	GL_CHECK_ERROR;
	if (fShaderCompiled != GL_TRUE)
	{
		printf("%s - Unable to compile fragment shader %d!\n", pchShaderName, nSceneFragmentShader);
		glDeleteProgram(unProgramID);
		GL_CHECK_ERROR;
		glDeleteShader(nSceneFragmentShader);
		GL_CHECK_ERROR;
		return 0;
	}

	glAttachShader(unProgramID, nSceneFragmentShader);
	GL_CHECK_ERROR;
	glDeleteShader(nSceneFragmentShader); // the program hangs onto this once it's attached
	GL_CHECK_ERROR;

	glLinkProgram(unProgramID);
	GL_CHECK_ERROR;

	GLint programSuccess = GL_TRUE;
	glGetProgramiv(unProgramID, GL_LINK_STATUS, &programSuccess);
	GL_CHECK_ERROR;
	if (programSuccess != GL_TRUE)
	{
		printf("%s - Error linking program %d!\n", pchShaderName, unProgramID);
		glDeleteProgram(unProgramID);
		GL_CHECK_ERROR;
		return 0;
	}

	glUseProgram(unProgramID);
	GL_CHECK_ERROR;
	glUseProgram(0);
	GL_CHECK_ERROR;

	return unProgramID;
}


//-----------------------------------------------------------------------------
// Purpose: Creates all the shaders used by HelloVR SDL
//-----------------------------------------------------------------------------
bool CMainApplication::CreateAllShaders()
{
	m_unSceneProgramID = CompileGLShader(
		"Scene",

		// Vertex Shader
		"#version 410\n"
		"uniform mat4 matrix;\n"
		"layout(location = 0) in vec4 position;\n"
		"layout(location = 1) in vec2 v2UVcoordsIn;\n"
		"layout(location = 2) in vec3 v3NormalIn;\n"
		"out vec2 v2UVcoords;\n"
		"void main()\n"
		"{\n"
		"	v2UVcoords = v2UVcoordsIn;\n"
		"	gl_Position = matrix * position;\n"
		"}\n",

		// Fragment Shader
		"#version 410 core\n"
		"uniform sampler2D mytexture;\n"
		"in vec2 v2UVcoords;\n"
		"out vec4 outputColor;\n"
		"void main()\n"
		"{\n"
		"   outputColor = vec4(1.0, 0.0, 0.0, 1.0);//texture(mytexture, v2UVcoords);\n"
		"}\n"
	);
	m_nSceneMatrixLocation = glGetUniformLocation(m_unSceneProgramID, "matrix");
	if (m_nSceneMatrixLocation == -1)
	{
		printf("Unable to find matrix uniform in scene shader\n");
		return false;
	}

	m_unControllerTransformProgramID = CompileGLShader(
		"Controller",

		// vertex shader
		"#version 410\n"
		"uniform mat4 matrix;\n"
		"layout(location = 0) in vec4 position;\n"
		"layout(location = 1) in vec3 v3ColorIn;\n"
		"out vec4 v4Color;\n"
		"void main()\n"
		"{\n"
		"	v4Color.xyz = v3ColorIn; v4Color.a = 1.0;\n"
		"	gl_Position = matrix * position;\n"
		"}\n",

		// fragment shader
		"#version 410\n"
		"in vec4 v4Color;\n"
		"out vec4 outputColor;\n"
		"void main()\n"
		"{\n"
		"   outputColor = v4Color;\n"
		"}\n"
	);
	m_nControllerMatrixLocation = glGetUniformLocation(m_unControllerTransformProgramID, "matrix");
	if (m_nControllerMatrixLocation == -1)
	{
		printf("Unable to find matrix uniform in controller shader\n");
		return false;
	}

	m_unRenderModelProgramID = CompileGLShader(
		"render model",

		// vertex shader
		"#version 410\n"
		"uniform mat4 matrix;\n"
		"layout(location = 0) in vec4 position;\n"
		"layout(location = 1) in vec3 v3NormalIn;\n"
		"layout(location = 2) in vec2 v2TexCoordsIn;\n"
		"out vec2 v2TexCoord;\n"
		"void main()\n"
		"{\n"
		"	v2TexCoord = v2TexCoordsIn;\n"
		"	gl_Position = matrix * vec4(position.xyz, 1);\n"
		"}\n",

		//fragment shader
		"#version 410 core\n"
		"uniform sampler2D diffuse;\n"
		"in vec2 v2TexCoord;\n"
		"out vec4 outputColor;\n"
		"void main()\n"
		"{\n"
		"   outputColor = texture( diffuse, v2TexCoord);\n"
		"}\n"

	);
	m_nRenderModelMatrixLocation = glGetUniformLocation(m_unRenderModelProgramID, "matrix");
	if (m_nRenderModelMatrixLocation == -1)
	{
		printf("Unable to find matrix uniform in render model shader\n");
		return false;
	}

	m_unCompanionWindowProgramID = CompileGLShader(
		"CompanionWindow",

		// vertex shader
		"#version 410 core\n"
		"layout(location = 0) in vec4 position;\n"
		"layout(location = 1) in vec2 v2UVIn;\n"
		"noperspective out vec2 v2UV;\n"
		"void main()\n"
		"{\n"
		"	v2UV = v2UVIn;\n"
		"	gl_Position = position;\n"
		"}\n",

		// fragment shader
		"#version 410 core\n"
		"uniform sampler2D mytexture;\n"
		"noperspective in vec2 v2UV;\n"
		"out vec4 outputColor;\n"
		"void main()\n"
		"{\n"
		"		outputColor = texture(mytexture, v2UV);\n"
		"}\n"
	);

	return m_unSceneProgramID != 0
		&& m_unControllerTransformProgramID != 0
		&& m_unRenderModelProgramID != 0
		&& m_unCompanionWindowProgramID != 0;
}

//-----------------------------------------------------------------------------
// Purpose: create a sea of cubes
//-----------------------------------------------------------------------------
void CMainApplication::SetupScene()
{
	if (!m_pHMD)
		return;

	m_sceneShader.createShader("shaders/sceneV.glsl", "shaders/sceneF.glsl");
	GL_CHECK_ERROR;
	m_meshShader.createShader("shaders/meshV.glsl", "shaders/meshF.glsl");
	GL_CHECK_ERROR;
	m_flatShader.createShader("shaders/meshFlatV.glsl", "shaders/meshFlatF.glsl");
	GL_CHECK_ERROR;

	//GLint m_locationLP = glGetUniformLocation(m_sceneShader.programID, "lightPos");
	//GLint m_locationP = glGetUniformLocation(m_sceneShader.programID, "P"); //perspective matrix
	//GLint m_locationMV = glGetUniformLocation(m_sceneShader.programID, "MV"); //modelview matrix
	//GLint m_locationTex = glGetUniformLocation(m_sceneShader.programID, "tex"); //texcoords

	//GLint m_locationMeshMV = glGetUniformLocation(m_meshShader.programID, "MV"); //modelview matrix
	//GLint m_locationMeshP = glGetUniformLocation(m_meshShader.programID, "P"); //perspective matrix
	//GLint m_locationMeshLP = glGetUniformLocation(m_meshShader.programID, "lightPos");
	//GLint m_locationMeshLP2 = glGetUniformLocation(m_meshShader.programID, "lightPos2");
	//GLint m_locationMeshM = glGetUniformLocation(m_meshShader.programID, "modelMatrix");
	//GLint m_locationMeshWP = glGetUniformLocation(m_meshShader.programID, "wandPos");
	//GLint m_locationMeshWD = glGetUniformLocation(m_meshShader.programID, "wandDirr");
	//GLint m_locationMeshTex = glGetUniformLocation(m_meshShader.programID, "tex"); //texture sampler
	//GLint m_locationMeshDTex = glGetUniformLocation(m_meshShader.programID, "dTex"); //texture sampler
	//GLint m_locationMeshLMVP = glGetUniformLocation(m_meshShader.programID, "LMVP");
	//GLint m_locationMeshPP = glGetUniformLocation(m_meshShader.programID, "PP");
	//GLint m_locationMeshIntersectionP = glGetUniformLocation(m_meshShader.programID, "IntersectionP");
	//GLint m_locationMeshIntersectionN = glGetUniformLocation(m_meshShader.programID, "IntersectionN");
	//GLint m_locationMeshRad = glGetUniformLocation(m_meshShader.programID, "Radius");


	m_locationFlatMV = glGetUniformLocation(m_flatShader.programID, "MV"); //modelview matrix
	GL_CHECK_ERROR;
	m_locationFlatP = glGetUniformLocation(m_flatShader.programID, "P"); //perspective matrix
	GL_CHECK_ERROR;
	m_locationFlatLP = glGetUniformLocation(m_flatShader.programID, "lightPos");
	GL_CHECK_ERROR;
	m_locationFlatLP2 = glGetUniformLocation(m_flatShader.programID, "lightPos2");
	GL_CHECK_ERROR;
	m_locationFlatM = glGetUniformLocation(m_flatShader.programID, "modelMatrix");

	m_MVstack.init();
	// TODO look at making this a uniue pointer
	m_modellingMesh = new DynamicMesh();
	glm::vec3 tempVec = glm::vec3(0.0f, 1.40f, 0.0f);
	m_modellingMesh->sphereSubdivide(0.1f);
	m_modellingMesh->setPosition((&tempVec[0]));
	m_modellingMesh->createBuffers();
}


//-----------------------------------------------------------------------------
// Purpose: Draw all of the controllers as X/Y/Z lines
//-----------------------------------------------------------------------------
void CMainApplication::RenderControllerAxes()
{
	// Don't attempt to update controllers if input is not available
	if (!m_pHMD->IsInputAvailable())
		return;

	std::vector<float> vertdataarray;

	m_uiControllerVertcount = 0;

	for (EHand eHand = Left; eHand <= Right; ((int&)eHand)++)
	{
		if (!m_rHand[eHand].m_bShowController)
			continue;

		const glm::mat4& mat = m_rHand[eHand].m_rmat4Pose;

		glm::vec4 center = mat * glm::vec4(0, 0, 0, 1);

		for (int i = 0; i < 3; ++i)
		{
			glm::vec3 color(0, 0, 0);
			glm::vec4 point(0, 0, 0, 1);
			point[i] += 0.05f;  // offset in X, Y, Z
			color[i] = 1.0;  // R, G, B
			point = mat * point;
			vertdataarray.push_back(center.x);
			vertdataarray.push_back(center.y);
			vertdataarray.push_back(center.z);

			vertdataarray.push_back(color.x);
			vertdataarray.push_back(color.y);
			vertdataarray.push_back(color.z);

			vertdataarray.push_back(point.x);
			vertdataarray.push_back(point.y);
			vertdataarray.push_back(point.z);

			vertdataarray.push_back(color.x);
			vertdataarray.push_back(color.y);
			vertdataarray.push_back(color.z);

			m_uiControllerVertcount += 2;
		}

		glm::vec4 start = mat * glm::vec4(0, 0, -0.02f, 1);
		glm::vec4 end = mat * glm::vec4(0, 0, -39.f, 1);
		glm::vec3 color(.92f, .92f, .71f);

		vertdataarray.push_back(start.x); vertdataarray.push_back(start.y); vertdataarray.push_back(start.z);
		vertdataarray.push_back(color.x); vertdataarray.push_back(color.y); vertdataarray.push_back(color.z);

		vertdataarray.push_back(end.x); vertdataarray.push_back(end.y); vertdataarray.push_back(end.z);
		vertdataarray.push_back(color.x); vertdataarray.push_back(color.y); vertdataarray.push_back(color.z);
		m_uiControllerVertcount += 2;
	}

	// Setup the VAO the first time through.
	if (m_unControllerVAO == 0)
	{
		glGenVertexArrays(1, &m_unControllerVAO);
		GL_CHECK_ERROR;
		glBindVertexArray(m_unControllerVAO);
		GL_CHECK_ERROR;

		glGenBuffers(1, &m_glControllerVertBuffer);
		GL_CHECK_ERROR;
		glBindBuffer(GL_ARRAY_BUFFER, m_glControllerVertBuffer);
		GL_CHECK_ERROR;

		GLuint stride = 2 * 3 * sizeof(float);
		uintptr_t offset = 0;

		glEnableVertexAttribArray(0);
		GL_CHECK_ERROR;
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (const void*)offset);
		GL_CHECK_ERROR;

		offset += sizeof(glm::vec3);
		glEnableVertexAttribArray(1);
		GL_CHECK_ERROR;
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (const void*)offset);
		GL_CHECK_ERROR;

		glBindVertexArray(0);
		GL_CHECK_ERROR;
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_glControllerVertBuffer);
	GL_CHECK_ERROR;

	// set vertex data if we have some
	if (vertdataarray.size() > 0)
	{
		//$ TODO: Use glBufferSubData for this...
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertdataarray.size(), &vertdataarray[0], GL_STREAM_DRAW);
		GL_CHECK_ERROR;
	}
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CMainApplication::SetupCameras()
{
	m_mat4ProjectionLeft = GetHMDMatrixProjectionEye(vr::Eye_Left);
	m_mat4ProjectionRight = GetHMDMatrixProjectionEye(vr::Eye_Right);
	m_mat4eyePosLeft = GetHMDMatrixPoseEye(vr::Eye_Left);
	m_mat4eyePosRight = GetHMDMatrixPoseEye(vr::Eye_Right);
}


//-----------------------------------------------------------------------------
// Purpose: Creates a frame buffer. Returns true if the buffer was set up.
//          Returns false if the setup failed.
//-----------------------------------------------------------------------------
bool CMainApplication::CreateFrameBuffer(int nWidth, int nHeight, FramebufferDesc& framebufferDesc)
{
	glGenFramebuffers(1, &framebufferDesc.m_nRenderFramebufferId);
	GL_CHECK_ERROR;
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferDesc.m_nRenderFramebufferId);
	GL_CHECK_ERROR;

	glGenRenderbuffers(1, &framebufferDesc.m_nDepthBufferId);
	GL_CHECK_ERROR; 
	glBindRenderbuffer(GL_RENDERBUFFER, framebufferDesc.m_nDepthBufferId);
	GL_CHECK_ERROR;
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT, nWidth, nHeight);
	GL_CHECK_ERROR;
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, framebufferDesc.m_nDepthBufferId);
	GL_CHECK_ERROR;

	glGenTextures(1, &framebufferDesc.m_nRenderTextureId);
	GL_CHECK_ERROR;
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, framebufferDesc.m_nRenderTextureId);
	GL_CHECK_ERROR;
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA8, nWidth, nHeight, true);
	GL_CHECK_ERROR;
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, framebufferDesc.m_nRenderTextureId, 0);
	GL_CHECK_ERROR;

	glGenFramebuffers(1, &framebufferDesc.m_nResolveFramebufferId);
	GL_CHECK_ERROR;
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferDesc.m_nResolveFramebufferId);
	GL_CHECK_ERROR;

	glGenTextures(1, &framebufferDesc.m_nResolveTextureId);
	GL_CHECK_ERROR;
	glBindTexture(GL_TEXTURE_2D, framebufferDesc.m_nResolveTextureId);
	GL_CHECK_ERROR;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	GL_CHECK_ERROR;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	GL_CHECK_ERROR;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, nWidth, nHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	GL_CHECK_ERROR;
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferDesc.m_nResolveTextureId, 0);
	GL_CHECK_ERROR;

	// check FBO status
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	GL_CHECK_ERROR;

	return true;
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
bool CMainApplication::SetupStereoRenderTargets()
{
	if (!m_pHMD)
		return false;

	m_pHMD->GetRecommendedRenderTargetSize(&m_nRenderWidth, &m_nRenderHeight);

	CreateFrameBuffer(m_nRenderWidth, m_nRenderHeight, leftEyeDesc);
	CreateFrameBuffer(m_nRenderWidth, m_nRenderHeight, rightEyeDesc);

	return true;
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CMainApplication::SetupCompanionWindow()
{
	if (!m_pHMD)
		return;

	std::vector<VertexDataWindow> vVerts;

	// left eye verts
	vVerts.push_back(VertexDataWindow(glm::vec2(-1, -1), glm::vec2(0, 1)));
	vVerts.push_back(VertexDataWindow(glm::vec2(0, -1), glm::vec2(1, 1)));
	vVerts.push_back(VertexDataWindow(glm::vec2(-1, 1), glm::vec2(0, 0)));
	vVerts.push_back(VertexDataWindow(glm::vec2(0, 1), glm::vec2(1, 0)));

	// right eye verts
	vVerts.push_back(VertexDataWindow(glm::vec2(0, -1), glm::vec2(0, 1)));
	vVerts.push_back(VertexDataWindow(glm::vec2(1, -1), glm::vec2(1, 1)));
	vVerts.push_back(VertexDataWindow(glm::vec2(0, 1), glm::vec2(0, 0)));
	vVerts.push_back(VertexDataWindow(glm::vec2(1, 1), glm::vec2(1, 0)));

	GLushort vIndices[] = { 0, 1, 3,   0, 3, 2,   4, 5, 7,   4, 7, 6 };
	m_uiCompanionWindowIndexSize = _countof(vIndices);

	glGenVertexArrays(1, &m_unCompanionWindowVAO);
	GL_CHECK_ERROR;
	glBindVertexArray(m_unCompanionWindowVAO);
	GL_CHECK_ERROR;

	glGenBuffers(1, &m_glCompanionWindowIDVertBuffer);
	GL_CHECK_ERROR;
	glBindBuffer(GL_ARRAY_BUFFER, m_glCompanionWindowIDVertBuffer);
	GL_CHECK_ERROR;
	glBufferData(GL_ARRAY_BUFFER, vVerts.size() * sizeof(VertexDataWindow), &vVerts[0], GL_STATIC_DRAW);
	GL_CHECK_ERROR;

	glGenBuffers(1, &m_glCompanionWindowIDIndexBuffer);
	GL_CHECK_ERROR;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glCompanionWindowIDIndexBuffer);
	GL_CHECK_ERROR;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_uiCompanionWindowIndexSize * sizeof(GLushort), &vIndices[0], GL_STATIC_DRAW);
	GL_CHECK_ERROR;

	glEnableVertexAttribArray(0);
	GL_CHECK_ERROR;
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(VertexDataWindow), (void*)offsetof(VertexDataWindow, position));
	GL_CHECK_ERROR;

	glEnableVertexAttribArray(1);
	GL_CHECK_ERROR;
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexDataWindow), (void*)offsetof(VertexDataWindow, texCoord));
	GL_CHECK_ERROR;

	glBindVertexArray(0);
	GL_CHECK_ERROR;

	glDisableVertexAttribArray(0);
	GL_CHECK_ERROR;
	glDisableVertexAttribArray(1);
	GL_CHECK_ERROR;

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	GL_CHECK_ERROR;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	GL_CHECK_ERROR;
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CMainApplication::RenderStereoTargets()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	GL_CHECK_ERROR;
	glEnable(GL_MULTISAMPLE);
	GL_CHECK_ERROR;

	// Left Eye
	glBindFramebuffer(GL_FRAMEBUFFER, leftEyeDesc.m_nRenderFramebufferId);
	GL_CHECK_ERROR;
	glViewport(0, 0, m_nRenderWidth, m_nRenderHeight);
	GL_CHECK_ERROR;
	RenderScene(vr::Eye_Left);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	GL_CHECK_ERROR;

	glDisable(GL_MULTISAMPLE);
	GL_CHECK_ERROR;

	glBindFramebuffer(GL_READ_FRAMEBUFFER, leftEyeDesc.m_nRenderFramebufferId);
	GL_CHECK_ERROR;
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, leftEyeDesc.m_nResolveFramebufferId);
	GL_CHECK_ERROR;

	glBlitFramebuffer(0, 0, m_nRenderWidth, m_nRenderHeight, 0, 0, m_nRenderWidth, m_nRenderHeight,
		GL_COLOR_BUFFER_BIT,
		GL_LINEAR);
	GL_CHECK_ERROR;

	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	GL_CHECK_ERROR;
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	GL_CHECK_ERROR;

	glEnable(GL_MULTISAMPLE);
	GL_CHECK_ERROR;

	// Right Eye
	glBindFramebuffer(GL_FRAMEBUFFER, rightEyeDesc.m_nRenderFramebufferId);
	GL_CHECK_ERROR;
	glViewport(0, 0, m_nRenderWidth, m_nRenderHeight);
	GL_CHECK_ERROR;
	RenderScene(vr::Eye_Right);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	GL_CHECK_ERROR;

	glDisable(GL_MULTISAMPLE);
	GL_CHECK_ERROR;

	glBindFramebuffer(GL_READ_FRAMEBUFFER, rightEyeDesc.m_nRenderFramebufferId);
	GL_CHECK_ERROR;
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, rightEyeDesc.m_nResolveFramebufferId);
	GL_CHECK_ERROR;

	glBlitFramebuffer(0, 0, m_nRenderWidth, m_nRenderHeight, 0, 0, m_nRenderWidth, m_nRenderHeight,
		GL_COLOR_BUFFER_BIT,
		GL_LINEAR);
	GL_CHECK_ERROR;

	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	GL_CHECK_ERROR;
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	GL_CHECK_ERROR;
}


//-----------------------------------------------------------------------------
// Purpose: Renders a scene with respect to nEye.
//-----------------------------------------------------------------------------
void CMainApplication::RenderScene(vr::Hmd_Eye nEye)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GL_CHECK_ERROR;
	glEnable(GL_DEPTH_TEST);
	GL_CHECK_ERROR;

	bool bIsInputAvailable = m_pHMD->IsInputAvailable();

	if (bIsInputAvailable)
	{
		// draw the controller axis lines
		glUseProgram(m_unControllerTransformProgramID);
		GL_CHECK_ERROR;
		glUniformMatrix4fv(m_nControllerMatrixLocation, 1, GL_FALSE, &GetCurrentViewProjectionMatrix(nEye)[0][0]);
		GL_CHECK_ERROR;
		glBindVertexArray(m_unControllerVAO);
		GL_CHECK_ERROR;
		glDrawArrays(GL_LINES, 0, m_uiControllerVertcount);
		GL_CHECK_ERROR;
		glBindVertexArray(0);
		GL_CHECK_ERROR;
	}

	// ----- Render Model rendering -----
	glUseProgram(m_unRenderModelProgramID);

	for (EHand eHand = Left; eHand <= Right; ((int&)eHand)++)
	{
		if (!m_rHand[eHand].m_bShowController || !m_rHand[eHand].m_pRenderModel)
			continue;

		const glm::mat4& matDeviceToTracking = m_rHand[eHand].m_rmat4Pose;
		glm::mat4 matMVP = GetCurrentViewProjectionMatrix(nEye) * matDeviceToTracking;
		glUniformMatrix4fv(m_nRenderModelMatrixLocation, 1, GL_FALSE, &matMVP[0][0]);
		GL_CHECK_ERROR;

		m_rHand[eHand].m_pRenderModel->Draw();
	}

	glUseProgram(m_flatShader.programID);
	glUniformMatrix4fv(m_locationFlatP, 1, GL_FALSE, &GetCurrentViewProjectionMatrix(nEye)[0][0]);

	m_MVstack.push();
	m_MVstack.translate(m_modellingMesh->getPosition());
	m_MVstack.multiply(m_modellingMesh->getOrientation());
	glUniformMatrix4fv(m_locationFlatMV, 1, GL_FALSE, m_MVstack.getCurrentMatrix());

	glm::vec4 temp = glm::vec4(0.0f, 2.0f, 0.0f, 0.0f);
	glUniform4fv(m_locationFlatLP, 1, &temp[0]);
	glUniform4fv(m_locationFlatLP2, 1, &temp[0]);

	m_modellingMesh->render();
	m_MVstack.pop();

	glUseProgram(0);
	GL_CHECK_ERROR;
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CMainApplication::RenderCompanionWindow()
{
	glDisable(GL_DEPTH_TEST);
	GL_CHECK_ERROR;
	glViewport(0, 0, m_nCompanionWindowWidth, m_nCompanionWindowHeight);
	GL_CHECK_ERROR;

	glBindVertexArray(m_unCompanionWindowVAO);
	GL_CHECK_ERROR;
	glUseProgram(m_unCompanionWindowProgramID);
	GL_CHECK_ERROR;

	// render left eye (first half of index array )
	glBindTexture(GL_TEXTURE_2D, leftEyeDesc.m_nResolveTextureId);
	GL_CHECK_ERROR;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	GL_CHECK_ERROR;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	GL_CHECK_ERROR;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	GL_CHECK_ERROR;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	GL_CHECK_ERROR;
	glDrawElements(GL_TRIANGLES, m_uiCompanionWindowIndexSize / 2, GL_UNSIGNED_SHORT, 0);
	GL_CHECK_ERROR;

	// render right eye (second half of index array )
	glBindTexture(GL_TEXTURE_2D, rightEyeDesc.m_nResolveTextureId);
	GL_CHECK_ERROR;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	GL_CHECK_ERROR;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	GL_CHECK_ERROR;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	GL_CHECK_ERROR;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	GL_CHECK_ERROR;
	glDrawElements(GL_TRIANGLES, m_uiCompanionWindowIndexSize / 2, GL_UNSIGNED_SHORT, (const void*)(uintptr_t)(m_uiCompanionWindowIndexSize));
	GL_CHECK_ERROR;

	glBindVertexArray(0);
	GL_CHECK_ERROR;
	glUseProgram(0);
	GL_CHECK_ERROR;
}


//-----------------------------------------------------------------------------
// Purpose: Gets a Matrix Projection Eye with respect to nEye.
//-----------------------------------------------------------------------------
glm::mat4 CMainApplication::GetHMDMatrixProjectionEye(vr::Hmd_Eye nEye)
{
	if (!m_pHMD)
		return glm::mat4(1.0f);

	vr::HmdMatrix44_t mat = m_pHMD->GetProjectionMatrix(nEye, m_fNearClip, m_fFarClip);

	return glm::mat4(
		mat.m[0][0], mat.m[1][0], mat.m[2][0], mat.m[3][0],
		mat.m[0][1], mat.m[1][1], mat.m[2][1], mat.m[3][1],
		mat.m[0][2], mat.m[1][2], mat.m[2][2], mat.m[3][2],
		mat.m[0][3], mat.m[1][3], mat.m[2][3], mat.m[3][3]
	);
}


//-----------------------------------------------------------------------------
// Purpose: Gets an HMDMatrixPoseEye with respect to nEye.
//-----------------------------------------------------------------------------
glm::mat4 CMainApplication::GetHMDMatrixPoseEye(vr::Hmd_Eye nEye)
{
	if (!m_pHMD)
		return glm::mat4(1.0f);

	vr::HmdMatrix34_t matEyeRight = m_pHMD->GetEyeToHeadTransform(nEye);
	glm::mat4 matrixObj(
		matEyeRight.m[0][0], matEyeRight.m[1][0], matEyeRight.m[2][0], 0.0,
		matEyeRight.m[0][1], matEyeRight.m[1][1], matEyeRight.m[2][1], 0.0,
		matEyeRight.m[0][2], matEyeRight.m[1][2], matEyeRight.m[2][2], 0.0,
		matEyeRight.m[0][3], matEyeRight.m[1][3], matEyeRight.m[2][3], 1.0f
	);

	return glm::inverse(matrixObj);
}


//-----------------------------------------------------------------------------
// Purpose: Gets a Current View Projection Matrix with respect to nEye,
//          which may be an Eye_Left or an Eye_Right.
//-----------------------------------------------------------------------------
glm::mat4 CMainApplication::GetCurrentViewProjectionMatrix(vr::Hmd_Eye nEye)
{
	glm::mat4 matMVP = glm::mat4(1.0f);
	if (nEye == vr::Eye_Left)
	{
		matMVP = m_mat4ProjectionLeft * m_mat4eyePosLeft * m_mat4HMDPose;
	}
	else if (nEye == vr::Eye_Right)
	{
		matMVP = m_mat4ProjectionRight * m_mat4eyePosRight * m_mat4HMDPose;
	}

	return matMVP;
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CMainApplication::UpdateHMDMatrixPose()
{
	if (!m_pHMD)
		return;

	vr::VRCompositor()->WaitGetPoses(m_rTrackedDevicePose, vr::k_unMaxTrackedDeviceCount, NULL, 0);

	m_iValidPoseCount = 0;
	m_strPoseClasses = "";
	for (int nDevice = 0; nDevice < vr::k_unMaxTrackedDeviceCount; ++nDevice)
	{
		if (m_rTrackedDevicePose[nDevice].bPoseIsValid)
		{
			m_iValidPoseCount++;
			m_rmat4DevicePose[nDevice] = ConvertSteamVRMatrixToMatrix4(m_rTrackedDevicePose[nDevice].mDeviceToAbsoluteTracking);
			if (m_rDevClassChar[nDevice] == 0)
			{
				switch (m_pHMD->GetTrackedDeviceClass(nDevice))
				{
				case vr::TrackedDeviceClass_Controller:        m_rDevClassChar[nDevice] = 'C'; break;
				case vr::TrackedDeviceClass_HMD:               m_rDevClassChar[nDevice] = 'H'; break;
				case vr::TrackedDeviceClass_Invalid:           m_rDevClassChar[nDevice] = 'I'; break;
				case vr::TrackedDeviceClass_GenericTracker:    m_rDevClassChar[nDevice] = 'G'; break;
				case vr::TrackedDeviceClass_TrackingReference: m_rDevClassChar[nDevice] = 'T'; break;
				default:                                       m_rDevClassChar[nDevice] = '?'; break;
				}
			}
			m_strPoseClasses += m_rDevClassChar[nDevice];
		}
	}

	if (m_rTrackedDevicePose[vr::k_unTrackedDeviceIndex_Hmd].bPoseIsValid)
	{
		m_mat4HMDPose = m_rmat4DevicePose[vr::k_unTrackedDeviceIndex_Hmd];
		m_mat4HMDPose = inverse(m_mat4HMDPose);
	}
}


//-----------------------------------------------------------------------------
// Purpose: Finds a render model we've already loaded or loads a new one
//-----------------------------------------------------------------------------
CGLRenderModel* CMainApplication::FindOrLoadRenderModel(const char* pchRenderModelName)
{
	CGLRenderModel* pRenderModel = NULL;
	for (std::vector< CGLRenderModel* >::iterator i = m_vecRenderModels.begin(); i != m_vecRenderModels.end(); i++)
	{
		if (!_stricmp((*i)->GetName().c_str(), pchRenderModelName))
		{
			pRenderModel = *i;
			break;
		}
	}

	// load the model if we didn't find one
	if (!pRenderModel)
	{
		vr::RenderModel_t* pModel;
		vr::EVRRenderModelError error;
		while (1)
		{
			error = vr::VRRenderModels()->LoadRenderModel_Async(pchRenderModelName, &pModel);
			if (error != vr::VRRenderModelError_Loading)
				break;

			ThreadSleep(1);
		}

		if (error != vr::VRRenderModelError_None)
		{
			printf("Unable to load render model %s - %s\n", pchRenderModelName, vr::VRRenderModels()->GetRenderModelErrorNameFromEnum(error));
			return NULL; // move on to the next tracked device
		}

		vr::RenderModel_TextureMap_t* pTexture;
		while (1)
		{
			error = vr::VRRenderModels()->LoadTexture_Async(pModel->diffuseTextureId, &pTexture);
			if (error != vr::VRRenderModelError_Loading)
				break;

			ThreadSleep(1);
		}

		if (error != vr::VRRenderModelError_None)
		{
			printf("Unable to load render texture id:%d for render model %s\n", pModel->diffuseTextureId, pchRenderModelName);
			vr::VRRenderModels()->FreeRenderModel(pModel);
			return NULL; // move on to the next tracked device
		}

		pRenderModel = new CGLRenderModel(pchRenderModelName);
		if (!pRenderModel->BInit(*pModel, *pTexture))
		{
			printf("Unable to create GL model from render model %s\n", pchRenderModelName);
			delete pRenderModel;
			pRenderModel = NULL;
		}
		else
		{
			m_vecRenderModels.push_back(pRenderModel);
		}
		vr::VRRenderModels()->FreeRenderModel(pModel);
		vr::VRRenderModels()->FreeTexture(pTexture);
	}
	return pRenderModel;
}


//-----------------------------------------------------------------------------
// Purpose: Converts a SteamVR matrix to our local matrix class
//-----------------------------------------------------------------------------
glm::mat4 CMainApplication::ConvertSteamVRMatrixToMatrix4(const vr::HmdMatrix34_t& matPose)
{
	glm::mat4 matrixObj(
		matPose.m[0][0], matPose.m[1][0], matPose.m[2][0], 0.0,
		matPose.m[0][1], matPose.m[1][1], matPose.m[2][1], 0.0,
		matPose.m[0][2], matPose.m[1][2], matPose.m[2][2], 0.0,
		matPose.m[0][3], matPose.m[1][3], matPose.m[2][3], 1.0f
	);
	return matrixObj;
}


//-----------------------------------------------------------------------------
// Purpose: Create/destroy GL Render Models
//-----------------------------------------------------------------------------
CGLRenderModel::CGLRenderModel(const std::string& sRenderModelName)
	: m_sModelName(sRenderModelName)
{
	m_glIndexBuffer = 0;
	m_glVertArray = 0;
	m_glVertBuffer = 0;
	m_glTexture = 0;
}


CGLRenderModel::~CGLRenderModel()
{
	Cleanup();
}


//-----------------------------------------------------------------------------
// Purpose: Allocates and populates the GL resources for a render model
//-----------------------------------------------------------------------------
bool CGLRenderModel::BInit(const vr::RenderModel_t& vrModel, const vr::RenderModel_TextureMap_t& vrDiffuseTexture)
{
	// create and bind a VAO to hold state for this model
	glGenVertexArrays(1, &m_glVertArray);
	GL_CHECK_ERROR;
	glBindVertexArray(m_glVertArray);
	GL_CHECK_ERROR;

	// Populate a vertex buffer
	glGenBuffers(1, &m_glVertBuffer);
	GL_CHECK_ERROR;
	glBindBuffer(GL_ARRAY_BUFFER, m_glVertBuffer);
	GL_CHECK_ERROR;
	glBufferData(GL_ARRAY_BUFFER, sizeof(vr::RenderModel_Vertex_t) * vrModel.unVertexCount, vrModel.rVertexData, GL_STATIC_DRAW);
	GL_CHECK_ERROR;

	// Identify the components in the vertex buffer
	glEnableVertexAttribArray(0);
	GL_CHECK_ERROR;
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vr::RenderModel_Vertex_t), (void*)offsetof(vr::RenderModel_Vertex_t, vPosition));
	GL_CHECK_ERROR;
	glEnableVertexAttribArray(1);
	GL_CHECK_ERROR;
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vr::RenderModel_Vertex_t), (void*)offsetof(vr::RenderModel_Vertex_t, vNormal));
	GL_CHECK_ERROR;
	glEnableVertexAttribArray(2);
	GL_CHECK_ERROR;
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vr::RenderModel_Vertex_t), (void*)offsetof(vr::RenderModel_Vertex_t, rfTextureCoord));
	GL_CHECK_ERROR;

	// Create and populate the index buffer
	glGenBuffers(1, &m_glIndexBuffer);
	GL_CHECK_ERROR;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glIndexBuffer);
	GL_CHECK_ERROR;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * vrModel.unTriangleCount * 3, vrModel.rIndexData, GL_STATIC_DRAW);
	GL_CHECK_ERROR;

	glBindVertexArray(0);
	GL_CHECK_ERROR;

	// create and populate the texture
	glGenTextures(1, &m_glTexture);
	GL_CHECK_ERROR;
	glBindTexture(GL_TEXTURE_2D, m_glTexture);
	GL_CHECK_ERROR;

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, vrDiffuseTexture.unWidth, vrDiffuseTexture.unHeight,
		0, GL_RGBA, GL_UNSIGNED_BYTE, vrDiffuseTexture.rubTextureMapData);
	GL_CHECK_ERROR;

	// If this renders black ask McJohn what's wrong.
	glGenerateMipmap(GL_TEXTURE_2D);
	GL_CHECK_ERROR;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	GL_CHECK_ERROR;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	GL_CHECK_ERROR;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	GL_CHECK_ERROR;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	GL_CHECK_ERROR;

	GLfloat fLargest;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
	GL_CHECK_ERROR;
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);
	GL_CHECK_ERROR;

	glBindTexture(GL_TEXTURE_2D, 0);
	GL_CHECK_ERROR;

	m_unVertexCount = vrModel.unTriangleCount * 3;

	return true;
}


//-----------------------------------------------------------------------------
// Purpose: Frees the GL resources for a render model
//-----------------------------------------------------------------------------
void CGLRenderModel::Cleanup()
{
	if (m_glVertBuffer)
	{
		glDeleteBuffers(1, &m_glIndexBuffer);
		GL_CHECK_ERROR;
		glDeleteVertexArrays(1, &m_glVertArray);
		GL_CHECK_ERROR;
		glDeleteBuffers(1, &m_glVertBuffer);
		GL_CHECK_ERROR;
		m_glIndexBuffer = 0;
		m_glVertArray = 0;
		m_glVertBuffer = 0;
	}
}


//-----------------------------------------------------------------------------
// Purpose: Draws the render model
//-----------------------------------------------------------------------------
void CGLRenderModel::Draw()
{
	glBindVertexArray(m_glVertArray);
	GL_CHECK_ERROR;

	glActiveTexture(GL_TEXTURE0);
	GL_CHECK_ERROR;
	glBindTexture(GL_TEXTURE_2D, m_glTexture);
	GL_CHECK_ERROR;

	glDrawElements(GL_TRIANGLES, m_unVertexCount, GL_UNSIGNED_SHORT, 0);
	GL_CHECK_ERROR;

	glBindVertexArray(0);
	GL_CHECK_ERROR;
}


////-----------------------------------------------------------------------------
//// Purpose:
////-----------------------------------------------------------------------------
namespace OpenVR {

	int runOpenVR(int argc, char* argv[])
	{
		CMainApplication* pMainApplication = new CMainApplication(argc, argv);

		if (!pMainApplication->BInit())
		{
			pMainApplication->Shutdown();
			return 1;
		}

		pMainApplication->RunMainLoop();

		pMainApplication->Shutdown();

		return 0;
	}

}
