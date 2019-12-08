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
//
//struct DepthBuffer
//{
//	GLuint        texId;
//
//	DepthBuffer(int width, int height)
//	{
//		glGenTextures(1, &texId);
//		GL_CHECK_ERROR;
//		glBindTexture(GL_TEXTURE_2D, texId);
//		GL_CHECK_ERROR;
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//		GL_CHECK_ERROR;
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//		GL_CHECK_ERROR;
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//		GL_CHECK_ERROR;
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//		GL_CHECK_ERROR;
//
//		GLenum internalFormat = GL_DEPTH_COMPONENT24;
//		GLenum type = GL_UNSIGNED_INT;
//
//		internalFormat = GL_DEPTH_COMPONENT32F;
//		type = GL_FLOAT;
//
//		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, GL_DEPTH_COMPONENT, type, NULL);
//		GL_CHECK_ERROR;
//	}
//	~DepthBuffer()
//	{
//		if (texId)
//		{
//			glDeleteTextures(1, &texId);
//			GL_CHECK_ERROR;
//			texId = 0;
//		}
//	}
//};
//
//struct TextureBuffer
//{
//	GLuint              texId;
//	GLuint              fboId;
//	int					width;
//	int					height;
//
//	TextureBuffer(bool rendertarget, int sourceWidth, int sourceHeight, int mipLevels, unsigned char * data, int sampleCount) :
//		texId(0),
//		fboId(0),
//		width(0),
//		height(0)
//	{
//		width = sourceWidth;
//		height = sourceHeight;
//		
//		glGenTextures(1, &texId);
//		GL_CHECK_ERROR;
//		glBindTexture(GL_TEXTURE_2D, texId);
//		GL_CHECK_ERROR;
//
//		if (rendertarget)
//		{
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//			GL_CHECK_ERROR;
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//			GL_CHECK_ERROR;
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//			GL_CHECK_ERROR;
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//			GL_CHECK_ERROR;
//		}
//		else
//		{
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//			GL_CHECK_ERROR;
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//			GL_CHECK_ERROR;
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//			GL_CHECK_ERROR;
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//			GL_CHECK_ERROR;
//		}
//
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
//		GL_CHECK_ERROR;
//
//		if (mipLevels > 1)
//		{
//			glGenerateMipmap(GL_TEXTURE_2D);
//			GL_CHECK_ERROR;
//		}
//
//		glGenFramebuffers(1, &fboId);
//		GL_CHECK_ERROR;
//	}
//
//	~TextureBuffer()
//	{
//		if (texId)
//		{
//			glDeleteTextures(1, &texId);
//			GL_CHECK_ERROR;
//			texId = 0;
//		}
//		if (fboId)
//		{
//			glDeleteFramebuffers(1, &fboId);
//			GL_CHECK_ERROR;
//			fboId = 0;
//		}
//	}
//
//	void SetAndClearRenderSurface(DepthBuffer* dbuffer)
//	{
//		glBindFramebuffer(GL_FRAMEBUFFER, fboId);
//		GL_CHECK_ERROR;
//		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texId, 0);
//		GL_CHECK_ERROR;
//		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, dbuffer->texId, 0);
//		GL_CHECK_ERROR;
//
//		glViewport(0, 0, width, height);
//		GL_CHECK_ERROR;
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//		GL_CHECK_ERROR;
//		glEnable(GL_FRAMEBUFFER_SRGB);
//		GL_CHECK_ERROR;
//	}
//
//	void UnsetRenderSurface()
//	{
//		glBindFramebuffer(GL_FRAMEBUFFER, fboId);
//		GL_CHECK_ERROR;
//		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
//		GL_CHECK_ERROR;
//		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);
//		GL_CHECK_ERROR;
//	}
//};
//
//
//static std::string get_property_string(vr::IVRSystem* system, vr::TrackedDeviceIndex_t deviceIndex, vr::TrackedDeviceProperty prop, vr::TrackedPropertyError* error = nullptr)
//{
//	unsigned int requiredBufferLen = system->GetStringTrackedDeviceProperty(deviceIndex, prop, nullptr, 0, error);
//	if (requiredBufferLen == 0) {
//		return "";
//	}
//
//	std::vector<char> buffer(requiredBufferLen);
//	requiredBufferLen = system->GetStringTrackedDeviceProperty(deviceIndex, prop, &buffer[0], requiredBufferLen, error);
//
//	return std::string(&buffer[0]);
//}
//
//static void get_active_controllers(vr::IVRSystem* vrSystem, std::vector<std::string>* controllerNames, std::vector<vr::TrackedDeviceIndex_t>* controllerDeviceIndex, int* trackerDeviceIndex)
//{
//	controllerNames->clear();
//	controllerDeviceIndex->clear();
//	*trackerDeviceIndex = -1;
//
//	for (int i = 0; i < vr::k_unMaxTrackedDeviceCount; i++) {
//		std::string deviceName = get_property_string(vrSystem, i, vr::Prop_RenderModelName_String);
//		if (deviceName.size() > 0) {
//
//			// Only add controller devices
//			if (std::string::npos != deviceName.find("controller")) {
//				controllerNames->push_back(deviceName);
//				controllerDeviceIndex->push_back(i);
//
//				// Start the process of loading in the model
//				vr::RenderModel_t* pRenderModel = nullptr;
//				vr::VRRenderModels()->LoadRenderModel_Async(deviceName.c_str(), &pRenderModel);
//			}
//
//			// Add tracker devices
//			if (std::string::npos != deviceName.find("tracker")) {
//				*trackerDeviceIndex = i;
//			}
//		}
//	}
//}
//
//void fetchModels(std::vector<std::unique_ptr<StaticMesh>>* controllerModels, std::vector<std::string>* controllerNames)
//{
//	// Start at controllerModels.size() because we don't want to reload already loaded models.
//	for (std::size_t i = controllerModels->size(); i < controllerNames->size(); i++) {
//
//		vr::RenderModel_t* renderModel = nullptr;
//		vr::EVRRenderModelError renderModelError = vr::VRRenderModels()->LoadRenderModel_Async(controllerNames->at(i).c_str(), &renderModel);
//
//		if (renderModelError == 0) {
//
//			std::vector<float> vertexData;
//			std::vector<int> indexData;
//
//			int vertexCount = renderModel->unVertexCount;
//			int indexCount = renderModel->unTriangleCount * 3;
//			vertexData.resize(vertexCount * 5);
//			indexData.resize(indexCount);
//
//			for (int it = 0; it < indexCount; it++) {
//				indexData.push_back(renderModel->rIndexData[it]);
//			}
//
//			for (int it = 0; it < vertexCount; it++) {
//				vertexData.push_back(renderModel->rVertexData[it].vPosition.v[0]);
//				vertexData.push_back(renderModel->rVertexData[it].vPosition.v[1]);
//				vertexData.push_back(renderModel->rVertexData[it].vPosition.v[2]);
//				vertexData.push_back(renderModel->rVertexData[it].rfTextureCoord[0]);
//				vertexData.push_back(renderModel->rVertexData[it].rfTextureCoord[1]);
//			}
//			
//			std::unique_ptr<StaticMesh> controller = std::make_unique<StaticMesh>();
//			controller->create(vertexData.data(), indexData.data(), vertexCount, int(renderModel->unTriangleCount));
//
//			controllerModels->push_back(std::move(controller));
//		}
//		else {
//			break;
//		}
//	}
//}
//
//static void WindowSizeCallback(GLFWwindow* p_Window, int p_Width, int p_Height) {
//	/*if (p_Width>0 && p_Height>0) {
//	g_Cfg.OGL.Header.BackBufferSize.w = p_Width;
//	g_Cfg.OGL.Header.BackBufferSize.h = p_Height;
//
//	ovrBool l_ConfigureResult = ovrHmd_ConfigureRendering(hmd, &g_Cfg.Config, G_DISTORTIONCAPS, hmd->MaxEyeFov, g_EyeRenderDesc);
//	if (!l_ConfigureResult) {
//	printf("Configure failed.\n");
//	exit(EXIT_FAILURE);
//	}
//	}*/
//}
//
//const bool L_MULTISAMPLING = false;
//
//void GLRenderCallsOpenVR() {
//	// Clear...
//	//GL calls
//	glClearColor(0.01f, 0.01f, 0.01f, 0.0f);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	glEnable(GL_DEPTH_TEST);
//	glEnable(GL_CULL_FACE);
//	glEnable(GL_FRAMEBUFFER_SRGB);
//	//glEnable(GL_FLAT);
//	glShadeModel(GL_FLAT);
//	glCullFace(GL_BACK);
//	//glDisable(GL_TEXTURE);
//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	glFrontFace(GL_CCW);
//	//glEnable(GL_POLYGON_STIPPLE);
//
//	if (L_MULTISAMPLING) {
//		glEnable(GL_MULTISAMPLE);
//	}
//	else {
//		glDisable(GL_MULTISAMPLE);
//	}
//}
//
//
//
//int OpenVR::runOpenVR() {
//	TextureBuffer* eyeRenderTexture[2] = { nullptr, nullptr };
//	DepthBuffer* eyeDepthBuffer[2] = { nullptr, nullptr };
//
//	// init openvr ===================================
//	unsigned int targetWidth = 0;
//	unsigned int targetHeight = 0;
//	
//	vr::EVRInitError error = vr::VRInitError_None;
//	vr::IVRSystem* hmd = vr::VR_Init(&error, vr::VRApplication_Scene);
//	if (error != vr::VRInitError_None) {
//		return -1;
//	}
//
//	hmd->GetRecommendedRenderTargetSize(&targetWidth, &targetHeight);
//
//	std::vector<std::string> controllerNames;
//	std::vector<vr::TrackedDeviceIndex_t> controllerDeviceIndex;
//	std::vector<std::unique_ptr<StaticMesh>> controllerModels;
//	int trackerDeviceIndex;
//	get_active_controllers(hmd, &controllerNames, &controllerDeviceIndex, &trackerDeviceIndex);
//
//	vr::ETrackingUniverseOrigin prevTrackingSpace = vr::VRCompositor()->GetTrackingSpace();
//
//	// INITIALIZE GL \__________________________________________________________________________________________________________________________
//	if (!glfwInit()) {
//		fprintf(stderr, "ERROR: could not start GLFW3\n");
//		return 1;
//	}
//
//	if (L_MULTISAMPLING) glfwWindowHint(GLFW_SAMPLES, 4);
//	else glfwWindowHint(GLFW_SAMPLES, 0);
//
//
//	// SETUP GLFW WINDOW AND CONTEXT /////////////////////////////////////////////////////////////
//	// Create a window...
//	GLFWwindow* l_Window;
//	GLFWmonitor* l_Monitor = NULL;
//	l_Window = glfwCreateWindow(targetWidth, targetHeight, "GLFW Oculus Rift Test", l_Monitor, NULL);
//
//	// Check if window creation was succesfull...
//	if (!l_Window) {
//		glfwTerminate();
//		exit(EXIT_FAILURE);
//	}
//	// Make the context current for this window...
//	glfwMakeContextCurrent(l_Window);
//
//	//start GLEW extension handler
//	glewExperimental = GL_TRUE;
//	GLenum l_GlewResult = glewInit();
//	if (l_GlewResult != GLEW_OK) {
//		printf("glewInit() error.\n");
//		exit(EXIT_FAILURE);
//	}
//
//	// Print some info about the OpenGL context...
//	int l_Major = glfwGetWindowAttrib(l_Window, GLFW_CONTEXT_VERSION_MAJOR);
//	int l_Minor = glfwGetWindowAttrib(l_Window, GLFW_CONTEXT_VERSION_MINOR);
//	int l_Profile = glfwGetWindowAttrib(l_Window, GLFW_OPENGL_PROFILE);
//	printf("OpenGL: %d.%d ", l_Major, l_Minor);
//	if (l_Major >= 3) { // Profiles introduced in OpenGL 3.0...
//		if (l_Profile == GLFW_OPENGL_COMPAT_PROFILE) printf("GLFW_OPENGL_COMPAT_PROFILE\n"); else printf("GLFW_OPENGL_CORE_PROFILE\n");
//	}
//	printf("Vendor: %s\n", (char*)glGetString(GL_VENDOR));
//	printf("Renderer: %s\n", (char*)glGetString(GL_RENDERER));
//
//	// TEXTURE BUFFERS =============================================================================================
//	// Make eye render buffers
//	for (int eye = 0; eye < 2; ++eye)
//	{
//		eyeRenderTexture[eye] = new TextureBuffer(true, targetWidth, targetHeight, 1, NULL, 1);
//		eyeDepthBuffer[eye] = new DepthBuffer(targetWidth, targetHeight);
//	}
//
//	// TODO MIRROR TEXTURE GOES HERE =============================================================================================
//
//
//	//WandView =============================================================================================
//
//	// create and set up the FBO
//	GLuint wandViewFBO;
//	glGenFramebuffers(1, &wandViewFBO);
//	glBindFramebuffer(GL_FRAMEBUFFER, wandViewFBO);
//
//	GLfloat border[] = { 1.0f, 0.0f, 0.0f, 0.0f };
//
//	GLuint wandShadowMap;
//	glGenTextures(1, &wandShadowMap);
//	glBindTexture(GL_TEXTURE_2D, wandShadowMap);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//	//glDrawBuffer(GL_NONE); // No color buffer is drawn to.
//	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
//	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);
//	*/
//	//Assign the shadow map to texture channel 0 
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, wandShadowMap);
//
//	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, wandShadowMap, 0);
//
//	GLuint pickingTexture;
//	glGenTextures(1, &pickingTexture);
//	glBindTexture(GL_TEXTURE_2D, pickingTexture);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB_FLOAT32_ATI, 1024, 1024,
//		0, GL_RGB, GL_FLOAT, NULL);
//	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
//		pickingTexture, 0);
//
//	//glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, wandShadowMap, 0);
//	//glDrawBuffer(GL_NONE);
//	//glReadBuffer(GL_NONE);
//	glDrawBuffer(GL_COLOR_ATTACHMENT0);
//
//	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
//
//	if (Status != GL_FRAMEBUFFER_COMPLETE) {
//		printf("FB error, status: 0x%x\n", Status);
//		return false;
//	}
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//
//	glfwSetWindowSizeCallback(l_Window, WindowSizeCallback);
//
//	// Create the scene =====================================================
//	MatrixStack MVstack; MVstack.init();
//	MatrixStack* MVptr = &MVstack;
//
//	float currTime = 0;
//	float lastTime = 0;
//	float deltaTime = 0;
//
//	DynamicMesh modellingMesh;
//	float dynamicMeshPosition[3] = { 0.0f, 0.0f, 0.0f };
//	modellingMesh.setPosition(dynamicMeshPosition);
//	modellingMesh.sphereSubdivide(0.05f); 
//	modellingMesh.createBuffers();
//
//	// HMD variables ========================================================
//	vr::TrackedDevicePose_t m_rTrackedDevicePose[vr::k_unMaxTrackedDeviceCount];
//
//	// Update ===============================================================
//	while (!glfwWindowShouldClose(l_Window)) {
//
//		lastTime = currTime;
//		currTime = glfwGetTime();
//		deltaTime = currTime - lastTime;
//	
//		// OpenVR Update =========================
//		vr::VREvent_t event = {};
//
//		while (hmd->PollNextEvent(&event, sizeof(event))) {
//			switch (event.eventType) {
//			case vr::EVREventType::VREvent_TrackedDeviceActivated: {
//				// A tracked device was plugged in or otherwise detected by the system. There is no data, but the trackedDeviceIndex will be the index of the new device.
//				controllerModels.clear();
//				get_active_controllers(hmd, &controllerNames, &controllerDeviceIndex, &trackerDeviceIndex);
//				break;
//			}
//			case vr::EVREventType::VREvent_TrackedDeviceDeactivated: {
//				// A tracked device was unplugged or the system is no longer able to contact it in some other way. Data is not used for this event.
//				controllerModels.clear();
//				get_active_controllers(hmd, &controllerNames, &controllerDeviceIndex, &trackerDeviceIndex);
//				break;
//			}
//			case vr::EVREventType::VREvent_ButtonPress: {
//				// The user has pressed a button on a controller. The controller struct in data identifies the button.
//
//				// TODO HANDLE BUTTON PRESSES
//				break;
//			}
//			case vr::EVREventType::VREvent_ButtonUnpress: {
//				// The user has stopped pressing a button on a controller. The controller struct in data identifies the button.
//
//				// TODO HANDLE BUTTON UNPRESSES
//				
//				break;
//			}
//			}
//		}
//
//		// Fetch the models if they are not yet loaded
//		if (controllerNames.size() > controllerModels.size()) {
//			fetchModels(&controllerModels, &controllerNames);
//		}
//
//		for (int eyeIt = 0; eyeIt < 2; eyeIt++)
//		{
//			assert(hmd != nullptr);
//
//			// get eye projection matrics and pose
//			vr::Hmdglm::mat44_t mat = hmd->GetProjectionMatrix((vr::EVREye)eyeIt, 1, 100);
//			float eyeProjectionMat[16] = {
//				mat.m[0][0], mat.m[1][0], mat.m[2][0], mat.m[3][0],
//				mat.m[0][1], mat.m[1][1], mat.m[2][1], mat.m[3][1],
//				mat.m[0][2], mat.m[1][2], mat.m[2][2], mat.m[3][2],
//				mat.m[0][3], mat.m[1][3], mat.m[2][3], mat.m[3][3]
//			};
//
//			vr::HmdMatrix34_t matEyeTrans = hmd->GetEyeToHeadTransform((vr::EVREye)eyeIt);
//			float eyeToHeadTrans[16] = {
//				matEyeTrans.m[0][0], matEyeTrans.m[1][0], matEyeTrans.m[2][0], 0.0,
//				matEyeTrans.m[0][1], matEyeTrans.m[1][1], matEyeTrans.m[2][1], 0.0,
//				matEyeTrans.m[0][2], matEyeTrans.m[1][2], matEyeTrans.m[2][2], 0.0,
//				matEyeTrans.m[0][3], matEyeTrans.m[1][3], matEyeTrans.m[2][3], 1.0f
//			};
//
//			vr::VRCompositor()->WaitGetPoses(m_rTrackedDevicePose, vr::k_unMaxTrackedDeviceCount, NULL, 0);
//
//			for (int nDevice = 0; nDevice < vr::k_unMaxTrackedDeviceCount; ++nDevice)
//			{
//				if (m_rTrackedDevicePose[nDevice].bPoseIsValid)
//				{
//					m_rmat4DevicePose[nDevice] = {
//						matPose.m[0][0], matPose.m[1][0], matPose.m[2][0], 0.0,
//						matPose.m[0][1], matPose.m[1][1], matPose.m[2][1], 0.0,
//						matPose.m[0][2], matPose.m[1][2], matPose.m[2][2], 0.0,
//						matPose.m[0][3], matPose.m[1][3], matPose.m[2][3], 1.0f
//					};	
//						
//					if (m_rDevClassChar[nDevice] == 0)
//					{
//						switch (m_pHMD->GetTrackedDeviceClass(nDevice))
//						{
//						case vr::TrackedDeviceClass_Controller:        m_rDevClassChar[nDevice] = 'C'; break;
//						case vr::TrackedDeviceClass_HMD:               m_rDevClassChar[nDevice] = 'H'; break;
//						case vr::TrackedDeviceClass_Invalid:           m_rDevClassChar[nDevice] = 'I'; break;
//						case vr::TrackedDeviceClass_GenericTracker:    m_rDevClassChar[nDevice] = 'G'; break;
//						case vr::TrackedDeviceClass_TrackingReference: m_rDevClassChar[nDevice] = 'T'; break;
//						default:                                       m_rDevClassChar[nDevice] = '?'; break;
//						}
//					}
//					m_strPoseClasses += m_rDevClassChar[nDevice];
//				}
//			}
//
//			if (m_rTrackedDevicePose[vr::k_unTrackedDeviceIndex_Hmd].bPoseIsValid)
//			{
//				m_mat4HMDPose = m_rmat4DevicePose[vr::k_unTrackedDeviceIndex_Hmd];
//				m_mat4HMDPose.invert();
//			}
//
//		}
//	}
//
//	delete modellingMesh;
//	modellingMesh = nullptr;
//
//	// Clean up window...
//	glfwDestroyWindow(l_Window);
//	glfwTerminate();
//
//	return 1;
//}

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
	void AddCubeToScene(glm::mat4 mat, std::vector<float>& vertdata);
	void AddCubeVertex(float fl0, float fl1, float fl2, float fl3, float fl4, std::vector<float>& vertdata);

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

private: // SDL bookkeeping
	GLFWwindow* m_pCompanionWindow;
	uint32_t m_nCompanionWindowWidth;
	uint32_t m_nCompanionWindowHeight;

	//SDL_GLContext m_pContext;

private: // OpenGL bookkeeping
	int m_iTrackedControllerCount;
	int m_iTrackedControllerCount_Last;
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
	, m_iTrackedControllerCount(0)
	, m_iTrackedControllerCount_Last(-1)
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

	// TODO NOT NEEDED?
	//m_strDriver = "No Driver";
	//m_strDisplay = "No Display";

	//m_strDriver = GetTrackedDeviceString(vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_TrackingSystemName_String);
	//m_strDisplay = GetTrackedDeviceString(vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_SerialNumber_String);

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

	//vr::VRInput()->SetActionManifestPath(Path_MakeAbsolute("../hellovr_actions.json", Path_StripFilename(Path_GetExecutablePath())).c_str());

	vr::VRInput()->GetActionHandle("/actions/demo/in/HideCubes", &m_actionHideCubes);
	vr::VRInput()->GetActionHandle("/actions/demo/in/HideThisController", &m_actionHideThisController);
	vr::VRInput()->GetActionHandle("/actions/demo/in/TriggerHaptic", &m_actionTriggerHaptic);
	vr::VRInput()->GetActionHandle("/actions/demo/in/AnalogInput", &m_actionAnalongInput);

	vr::VRInput()->GetActionSetHandle("/actions/demo", &m_actionsetDemo);

	vr::VRInput()->GetActionHandle("/actions/demo/out/Haptic_Left", &m_rHand[Left].m_actionHaptic);
	vr::VRInput()->GetInputSourceHandle("/user/hand/left", &m_rHand[Left].m_source);
	vr::VRInput()->GetActionHandle("/actions/demo/in/Hand_Left", &m_rHand[Left].m_actionPose);

	vr::VRInput()->GetActionHandle("/actions/demo/out/Haptic_Right", &m_rHand[Right].m_actionHaptic);
	vr::VRInput()->GetInputSourceHandle("/user/hand/right", &m_rHand[Right].m_source);
	vr::VRInput()->GetActionHandle("/actions/demo/in/Hand_Right", &m_rHand[Right].m_actionPose);

	return true;
}


//-----------------------------------------------------------------------------
// Purpose: Outputs the string in message to debugging output.
//          All other parameters are ignored.
//          Does not return any meaningful value or reference.
//-----------------------------------------------------------------------------
void APIENTRY DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char* message, const void* userParam)
{
	printf("GL Error: %s\n", message);
}


//-----------------------------------------------------------------------------
// Purpose: Initialize OpenGL. Returns true if OpenGL has been successfully
//          initialized, false if shaders could not be created.
//          If failure occurred in a module other than shaders, the function
//          may return true or throw an error. 
//-----------------------------------------------------------------------------
bool CMainApplication::BInitGL()
{
	if (m_bDebugOpenGL)
	{
		glDebugMessageCallback((GLDEBUGPROC)DebugCallback, nullptr);
		GL_CHECK_ERROR;
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		GL_CHECK_ERROR;
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		GL_CHECK_ERROR;
	}

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

	for (EHand eHand = Left; eHand <= Right; ((int&)eHand)++)
	{
		vr::InputPoseActionData_t poseData;
		if (vr::VRInput()->GetPoseActionDataForNextFrame(m_rHand[eHand].m_actionPose, vr::TrackingUniverseStanding, &poseData, sizeof(poseData), vr::k_ulInvalidInputValueHandle) != vr::VRInputError_None
			|| !poseData.bActive || !poseData.pose.bPoseIsValid)
		{
			m_rHand[eHand].m_bShowController = false;
		}
		else
		{
			m_rHand[eHand].m_rmat4Pose = ConvertSteamVRMatrixToMatrix4(poseData.pose.mDeviceToAbsoluteTracking);

			vr::InputOriginInfo_t originInfo;
			if (vr::VRInput()->GetOriginTrackedDeviceInfo(poseData.activeOrigin, &originInfo, sizeof(originInfo)) == vr::VRInputError_None
				&& originInfo.trackedDeviceIndex != vr::k_unTrackedDeviceIndexInvalid)
			{
				std::string sRenderModelName = GetTrackedDeviceString(originInfo.trackedDeviceIndex, vr::Prop_RenderModelName_String);
				if (sRenderModelName != m_rHand[eHand].m_sRenderModelName)
				{
					m_rHand[eHand].m_pRenderModel = FindOrLoadRenderModel(sRenderModelName.c_str());
					m_rHand[eHand].m_sRenderModelName = sRenderModelName;
				}
			}
		}
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
	if (m_iTrackedControllerCount != m_iTrackedControllerCount_Last || m_iValidPoseCount != m_iValidPoseCount_Last)
	{
		m_iValidPoseCount_Last = m_iValidPoseCount;
		m_iTrackedControllerCount_Last = m_iTrackedControllerCount;

		printf("PoseCount:%d(%s) Controllers:%d\n", m_iValidPoseCount, m_strPoseClasses.c_str(), m_iTrackedControllerCount);
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
		"   outputColor = vec4(1.0, 0.0, 0.0, 1.0);//v4Color;\n"
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
		"   outputColor = vec4(1.0, 0.0, 0.0, 1.0);//texture( diffuse, v2TexCoord);\n"
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
		"		outputColor = vec4(1.0, 0.0, 0.0, 1.0);//texture(mytexture, v2UV);\n"
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

	std::vector<float> vertdataarray;

	glm::mat4 matScale = glm::mat4(1.0f);
	matScale = glm::scale(matScale, glm::vec3(m_fScale, m_fScale, m_fScale));
	glm::mat4 matTransform = glm::mat4(1.0f);
	matTransform = glm::translate(matTransform, glm::vec3(-((float)m_iSceneVolumeWidth * m_fScaleSpacing) / 2.f, -((float)m_iSceneVolumeHeight * m_fScaleSpacing) / 2.f, -((float)m_iSceneVolumeDepth * m_fScaleSpacing) / 2.f));

	glm::mat4 mat = matScale * matTransform;

	glm::mat4 testMat = mat;
	printf("scene mat \n %f %f %f %f \n %f %f %f %f \n %f %f %f %f \n %f %f %f %f \n", testMat[0][0], testMat[1][0], testMat[2][0], testMat[3][0]
		, testMat[0][1], testMat[1][1], testMat[2][1], testMat[3][1]
		, testMat[0][2], testMat[1][2], testMat[2][2], testMat[3][2]
		, testMat[0][3], testMat[1][3], testMat[2][3], testMat[3][3]);

	for (int z = 0; z < m_iSceneVolumeDepth; z++)
	{
		for (int y = 0; y < m_iSceneVolumeHeight; y++)
		{
			for (int x = 0; x < m_iSceneVolumeWidth; x++)
			{
				AddCubeToScene(mat, vertdataarray);

				glm::mat4 testMat = mat;
				printf("cube index %d \n %f %f %f %f \n %f %f %f %f \n %f %f %f %f \n %f %f %f %f \n", x, testMat[0][0], testMat[1][0], testMat[2][0], testMat[3][0]
					, testMat[0][1], testMat[1][1], testMat[2][1], testMat[3][1]
					, testMat[0][2], testMat[1][2], testMat[2][2], testMat[3][2]
					, testMat[0][3], testMat[1][3], testMat[2][3], testMat[3][3]);

				mat = mat * translate(glm::mat4(1.0f), glm::vec3(m_fScaleSpacing, 0.0f, 0.0f));
			}
			mat = mat * translate(glm::mat4(1.0f), glm::vec3(-((float)m_iSceneVolumeWidth) * m_fScaleSpacing, m_fScaleSpacing, 0.0f));
		}
		mat = mat * translate(glm::mat4(1.0f), glm::vec3(0.0f, -((float)m_iSceneVolumeHeight) * m_fScaleSpacing, m_fScaleSpacing));
	}
	m_uiVertcount = vertdataarray.size() / 5;

	glGenVertexArrays(1, &m_unSceneVAO);
	GL_CHECK_ERROR;
	glBindVertexArray(m_unSceneVAO);
	GL_CHECK_ERROR;

	glGenBuffers(1, &m_glSceneVertBuffer);
	GL_CHECK_ERROR;
	glBindBuffer(GL_ARRAY_BUFFER, m_glSceneVertBuffer);
	GL_CHECK_ERROR;
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertdataarray.size(), &vertdataarray[0], GL_STATIC_DRAW);
	GL_CHECK_ERROR;

	GLsizei stride = sizeof(VertexDataScene);
	uintptr_t offset = 0;

	glEnableVertexAttribArray(0);
	GL_CHECK_ERROR;
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (const void*)offset);
	GL_CHECK_ERROR;

	offset += sizeof(glm::vec3);
	glEnableVertexAttribArray(1);
	GL_CHECK_ERROR;
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (const void*)offset);
	GL_CHECK_ERROR;

	glBindVertexArray(0);
	GL_CHECK_ERROR;
	glDisableVertexAttribArray(0);
	GL_CHECK_ERROR;
	glDisableVertexAttribArray(1);
	GL_CHECK_ERROR;

}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CMainApplication::AddCubeVertex(float fl0, float fl1, float fl2, float fl3, float fl4, std::vector<float>& vertdata)
{
	vertdata.push_back(fl0);
	vertdata.push_back(fl1);
	vertdata.push_back(fl2);
	vertdata.push_back(fl3);
	vertdata.push_back(fl4);
}


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
void CMainApplication::AddCubeToScene(glm::mat4 mat, std::vector<float>& vertdata)
{
	// glm::mat4 mat( outermat.data() );

	glm::vec4 A = mat * glm::vec4(0, 0, 0, 1);
	glm::vec4 B = mat * glm::vec4(1, 0, 0, 1);
	glm::vec4 C = mat * glm::vec4(1, 1, 0, 1);
	glm::vec4 D = mat * glm::vec4(0, 1, 0, 1);
	glm::vec4 E = mat * glm::vec4(0, 0, 1, 1);
	glm::vec4 F = mat * glm::vec4(1, 0, 1, 1);
	glm::vec4 G = mat * glm::vec4(1, 1, 1, 1);
	glm::vec4 H = mat * glm::vec4(0, 1, 1, 1);

	// triangles instead of quads
	AddCubeVertex(E.x, E.y, E.z, 0, 1, vertdata); //Front
	AddCubeVertex(F.x, F.y, F.z, 1, 1, vertdata);
	AddCubeVertex(G.x, G.y, G.z, 1, 0, vertdata);
	AddCubeVertex(G.x, G.y, G.z, 1, 0, vertdata);
	AddCubeVertex(H.x, H.y, H.z, 0, 0, vertdata);
	AddCubeVertex(E.x, E.y, E.z, 0, 1, vertdata);

	AddCubeVertex(B.x, B.y, B.z, 0, 1, vertdata); //Back
	AddCubeVertex(A.x, A.y, A.z, 1, 1, vertdata);
	AddCubeVertex(D.x, D.y, D.z, 1, 0, vertdata);
	AddCubeVertex(D.x, D.y, D.z, 1, 0, vertdata);
	AddCubeVertex(C.x, C.y, C.z, 0, 0, vertdata);
	AddCubeVertex(B.x, B.y, B.z, 0, 1, vertdata);

	AddCubeVertex(H.x, H.y, H.z, 0, 1, vertdata); //Top
	AddCubeVertex(G.x, G.y, G.z, 1, 1, vertdata);
	AddCubeVertex(C.x, C.y, C.z, 1, 0, vertdata);
	AddCubeVertex(C.x, C.y, C.z, 1, 0, vertdata);
	AddCubeVertex(D.x, D.y, D.z, 0, 0, vertdata);
	AddCubeVertex(H.x, H.y, H.z, 0, 1, vertdata);

	AddCubeVertex(A.x, A.y, A.z, 0, 1, vertdata); //Bottom
	AddCubeVertex(B.x, B.y, B.z, 1, 1, vertdata);
	AddCubeVertex(F.x, F.y, F.z, 1, 0, vertdata);
	AddCubeVertex(F.x, F.y, F.z, 1, 0, vertdata);
	AddCubeVertex(E.x, E.y, E.z, 0, 0, vertdata);
	AddCubeVertex(A.x, A.y, A.z, 0, 1, vertdata);

	AddCubeVertex(A.x, A.y, A.z, 0, 1, vertdata); //Left
	AddCubeVertex(E.x, E.y, E.z, 1, 1, vertdata);
	AddCubeVertex(H.x, H.y, H.z, 1, 0, vertdata);
	AddCubeVertex(H.x, H.y, H.z, 1, 0, vertdata);
	AddCubeVertex(D.x, D.y, D.z, 0, 0, vertdata);
	AddCubeVertex(A.x, A.y, A.z, 0, 1, vertdata);

	AddCubeVertex(F.x, F.y, F.z, 0, 1, vertdata); //Right
	AddCubeVertex(B.x, B.y, B.z, 1, 1, vertdata);
	AddCubeVertex(C.x, C.y, C.z, 1, 0, vertdata);
	AddCubeVertex(C.x, C.y, C.z, 1, 0, vertdata);
	AddCubeVertex(G.x, G.y, G.z, 0, 0, vertdata);
	AddCubeVertex(F.x, F.y, F.z, 0, 1, vertdata);
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
	m_iTrackedControllerCount = 0;

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

	if (m_bShowCubes)
	{
		glUseProgram(m_unSceneProgramID);
		GL_CHECK_ERROR; 
		glUniformMatrix4fv(m_nSceneMatrixLocation, 1, GL_FALSE, &GetCurrentViewProjectionMatrix(nEye)[0][0]);
		GL_CHECK_ERROR;
		glBindVertexArray(m_unSceneVAO);
		GL_CHECK_ERROR; 
		glBindTexture(GL_TEXTURE_2D, m_iTexture);
		GL_CHECK_ERROR; 
		glDrawArrays(GL_TRIANGLES, 0, m_uiVertcount);
		glBindVertexArray(0);
		GL_CHECK_ERROR;
	}

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
	
		// Debug HMD pose
		//glm::mat4 testMat = m_rmat4DevicePose[nDevice];
		//if (m_rDevClassChar[nDevice] == 'H')
		//	printf("Device pose \n %f %f %f %f \n %f %f %f %f \n %f %f %f %f \n %f %f %f %f \n", testMat[0][0], testMat[1][0], testMat[2][0], testMat[3][0]
		//																					   , testMat[0][1], testMat[1][1], testMat[2][1], testMat[3][1]
		//																					   , testMat[0][2], testMat[1][2], testMat[2][2], testMat[3][2]
		//																					   , testMat[0][3], testMat[1][3], testMat[2][3], testMat[3][3]);

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
