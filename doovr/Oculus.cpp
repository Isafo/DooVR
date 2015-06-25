//#define GLFW_EXPOSE_NATIVE_WIN32
//#define GLFW_EXPOSE_NATIVE_WGL

#include "Oculus.h"
#include "Shader.h"
#include "MatrixStack.h"
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"
#include "hexBox.h"
#include "Texture.h"
#include "Wand.h"
#include "Passive3D.h"
#include "VRPN.h"

using namespace std;

// ------- Function declerations --------
//! Sets up a glfw window depending on the resolution of the Oculus Rift device
static void WindowSizeCallback(GLFWwindow *p_Window, int p_Width, int p_Height);

void GLRenderCallsOculus();
//! reads wandCalibration from file and sets wand transform
void readCalibration(Vrpn* wand, float& eyeHeight, const char* profileName);

// Declare moveMesh - used for moving around the mesh in the scene.
// TODO: refactor this function to TOOLS namespace?
void moveMesh(Wand* wand, Mesh* mTest, bool buttonPressed, float* changePos, float* differenceR);
void moveEntity(Wand* wand, vector<Entity*> *objectList, float wandRadius);
bool selectFunction(Wand* wand, vector<Entity*> *objectList, int &chooseFunction);
void updatePanel(vector<Entity*> *objectList, int currentFunction, float MAX_HEX_HEIGHT, float MIN_HEX_HEIGHT);
void print_FLOAT_matrix(float *M);

// --- Variable Declerations ------------
const bool L_MULTISAMPLING = false;
const int G_DISTORTIONCAPS = 0
                             | ovrDistortionCap_Vignette
                             | ovrDistortionCap_Chromatic
                             | ovrDistortionCap_Overdrive
//| ovrDistortionCap_TimeWarp // Turning this on gives ghosting???
;

ovrHmd hmd;
ovrGLConfig g_Cfg;
ovrEyeRenderDesc g_EyeRenderDesc[2];

const float EYEHEIGHT{OVR_DEFAULT_EYE_HEIGHT};
// --------------------------------------

// Global Constant variables
const int nFunctions = 7;
const int nLightsources = 3;

// Function ID's
const int DILATEnERODE = 0;
const int DRAGnPULL = 1;
const int moveMESH = 2;
const int moveENTITY = 3;
const int coREGISTER = 4;
const int meshRESET = 5;
const int hexRESET = 6;


int Oculus::runOvr() {

	ovrVector3f g_EyeOffsets[2];
	ovrPosef g_EyePoses[2];
	ovrTexture g_EyeTextures[2];
	OVR::Matrix4f g_ProjectionMatrix[2];
	OVR::Sizei g_RenderTargetSize;
	ovrVector3f g_CameraPosition;

	GLfloat I[16] = { 1.0f, 0.0f, 0.0f, 0.0f,
					  0.0f, 1.0f, 0.0f, 0.0f,
					  0.0f, 0.0f, 1.0f, 0.0f,
					  0.0f, 0.0f, 0.0f, 1.0f };

	GLfloat P[16] = { 2.42f, 0.0f, 0.0f, 0.0f,
					  0.0f, 2.42f, 0.0f, 0.0f,
					  0.0f, 0.0f, -1.0f, -1.0f,
					  0.0f, 0.0f, -0.2f, 0.0f };

	// Lightposition 
	float lPos[4] = { 2.0f, 2.0f, 2.0f, 1.0f};
	float lPos2[4] = { -2.0f, 2.0f, 2.0f, 1.0f };
	float lPosTemp[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float LP[4];
	float* pmat4;
	float mat4[16];

	// Save old positions and transforms
	float changePos[3] = { 0.0f };
	float differenceR[16] = { 0.0f };
	float lastPos[3] = { 0.0f, 0.0f, 0.0f };
	float currPos[3] = { 0.0f, 0.0f, 0.0f };

	// Configuration variables
	int regCounter = 0;
	int n = 0;
	int resetCounter = 0;

	float pos[16] = { 0.0f };
	float invPos[16] = { 0.0f };
	float eyeHeight = OVR_DEFAULT_EYE_HEIGHT;
	float MAX_HEX_HEIGHT = -eyeHeight + 0.95f;
	float MIN_HEX_HEIGHT = -eyeHeight + 0.9f;
	float eye, floor;

	// FPS
	double fps = 0;

	// Translation vector -- used for tranlate objects in SceneGraph
	float translateVector[3] = { 0.0f, 0.0f, 0.0f };

	// Size of the wand tool
	float wandRadius = 0.01f;

	// States
	bool buttonPressed = false;
	bool buttonHeld = false;
	bool buttonReleased = false;
	bool lines = false;

	int currentFunction = moveENTITY;
	int &chooseFunction = currentFunction;

	// Location used for UNIFORMS in shader
	GLint locationLP;
	GLint locationP;
	GLint locationMV;
	GLint locationTex;

	GLint locationMeshMV;
	//GLint locationMeshLP[nLightsources + 1];
	GLint locationMeshP;
	GLint locationMeshLP;
	GLint locationMeshLP2;

	GLint locationWandMV;
	GLint locationWandP;

	//INITIALIZE OVR /////////////////////////////////////////////////////
	ovr_Initialize();
	int det;
	// Check for attached head mounted display...
	hmd = ovrHmd_Create(0);
	if (!hmd) {
		printf("No Oculus Rift device attached, using virtual version...\n");
		hmd = ovrHmd_CreateDebug(ovrHmd_DK2);

		det = ovrHmd_Detect();
		cout << det << endl;
	}

	// Check to see if we are running in "Direct" or "Extended Desktop" mode...
	bool l_DirectMode = ((hmd->HmdCaps & ovrHmdCap_ExtendDesktop) == 0);

	// INITIALIZE GL ////////////////////////////////////////////////////
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}

	if (L_MULTISAMPLING) glfwWindowHint(GLFW_SAMPLES, 4); 
	else glfwWindowHint(GLFW_SAMPLES, 0);


	// SETUP GLFW WINDOW AND CONTEXT /////////////////////////////////////////////////////////////
	// Create a window...
	GLFWwindow* l_Window;
	GLFWmonitor* l_Monitor;
	ovrSizei l_ClientSize;

	if (l_DirectMode) {
		printf("Running in \"Direct\" mode...\n");
		l_Monitor = NULL;

		l_ClientSize.w = hmd->Resolution.w; // Something reasonable, smaller, but maintain aspect ratio...
		l_ClientSize.h = hmd->Resolution.h; // Something reasonable, smaller, but maintain aspect ratio...
	} else {// Extended Desktop mode...
		printf("Running in \"Extended Desktop\" mode...\n");
		int l_Count;
		GLFWmonitor** l_Monitors = glfwGetMonitors(&l_Count);
		switch (l_Count) {
		  case 0: {
		    printf("No monitors found, exiting...\n");
			exit(EXIT_FAILURE);
			break;
		  } case 1: {
			printf("Two monitors expected, found only one, using primary...\n");
			l_Monitor = glfwGetPrimaryMonitor();
			break;
		  } case 2: {
			printf("Two monitors found, using second monitor...\n");
			l_Monitor = l_Monitors[1];
			break;
		  } default: {
			printf("More than two monitors found, using second monitor...\n");
			l_Monitor = l_Monitors[1];
		  }
		}

		l_ClientSize.w = hmd->Resolution.w; // 1920 for DK2...
		l_ClientSize.h = hmd->Resolution.h; // 1080 for DK2...
	}

	l_Window = glfwCreateWindow(l_ClientSize.w, l_ClientSize.h, "GLFW Oculus Rift Test", l_Monitor, NULL);

	// Check if window creation was succesfull...
	if (!l_Window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Attach the window in "Direct Mode"...
#if defined(_WIN32)
	if (l_DirectMode) {
		ovrBool l_AttachResult = ovrHmd_AttachToWindow(hmd, glfwGetWin32Window(l_Window), NULL, NULL);
		if (!l_AttachResult) {
			printf("Could not attach to window...");
			exit(EXIT_FAILURE);
		}
	}
#endif

	// Make the context current for this window...
	glfwMakeContextCurrent(l_Window);

	//start GLEW extension handler
	glewExperimental = GL_TRUE;
	GLenum l_GlewResult = glewInit();
	if (l_GlewResult != GLEW_OK) {
		printf("glewInit() error.\n");
		exit(EXIT_FAILURE);
	}

	// Print some info about the OpenGL context...
	int l_Major = glfwGetWindowAttrib(l_Window, GLFW_CONTEXT_VERSION_MAJOR);
	int l_Minor = glfwGetWindowAttrib(l_Window, GLFW_CONTEXT_VERSION_MINOR);
	int l_Profile = glfwGetWindowAttrib(l_Window, GLFW_OPENGL_PROFILE);
	printf("OpenGL: %d.%d ", l_Major, l_Minor);
	if (l_Major >= 3) { // Profiles introduced in OpenGL 3.0...
		if (l_Profile == GLFW_OPENGL_COMPAT_PROFILE) printf("GLFW_OPENGL_COMPAT_PROFILE\n"); else printf("GLFW_OPENGL_CORE_PROFILE\n");
	}
	printf("Vendor: %s\n", (char*)glGetString(GL_VENDOR));
	printf("Renderer: %s\n", (char*)glGetString(GL_RENDERER));

	//CREATE OCULUS TEXTURES AND BIND THESE TO GL///////////////////////////////////////////////////////////////////////////////
	ovrSizei l_EyeTextureSizes[2];

	l_EyeTextureSizes[ovrEye_Left] = ovrHmd_GetFovTextureSize(hmd, ovrEye_Left, hmd->MaxEyeFov[ovrEye_Left], 1.0f);
	l_EyeTextureSizes[ovrEye_Right] = ovrHmd_GetFovTextureSize(hmd, ovrEye_Right, hmd->MaxEyeFov[ovrEye_Right], 1.0f);

	// Combine for one texture for both eyes...
	g_RenderTargetSize.w = l_EyeTextureSizes[ovrEye_Left].w + l_EyeTextureSizes[ovrEye_Right].w;
	g_RenderTargetSize.h = (l_EyeTextureSizes[ovrEye_Left].h>l_EyeTextureSizes[ovrEye_Right].h ? l_EyeTextureSizes[ovrEye_Left].h : l_EyeTextureSizes[ovrEye_Right].h);

	// Create the FBO being a single one for both eyes (this is open for debate)...
	GLuint l_FBOId;
	glGenFramebuffers(1, &l_FBOId);
	glBindFramebuffer(GL_FRAMEBUFFER, l_FBOId);

	// The texture we're going to render to...
	GLuint l_TextureId;
	glGenTextures(1, &l_TextureId);
	// "Bind" the newly created texture : all future texture functions will modify this texture...
	glBindTexture(GL_TEXTURE_2D, l_TextureId);
	// Give an empty image to OpenGL (the last "0")
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, g_RenderTargetSize.w, g_RenderTargetSize.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	// Linear filtering...
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// Create Depth Buffer...
	GLuint l_DepthBufferId;
	glGenRenderbuffers(1, &l_DepthBufferId);
	glBindRenderbuffer(GL_RENDERBUFFER, l_DepthBufferId);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, g_RenderTargetSize.w, g_RenderTargetSize.h);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, l_DepthBufferId);

	// Set the texture as our colour attachment #0...
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, l_TextureId, 0);

	// Set the list of draw buffers...
	GLenum l_GLDrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, l_GLDrawBuffers); // "1" is the size of DrawBuffers

	// Check if everything is OK...
	GLenum l_Check = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
	if (l_Check != GL_FRAMEBUFFER_COMPLETE) {
		printf("There is a problem with the FBO.\n");
		exit(EXIT_FAILURE);
	}

	// Unbind...
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// SETUP TEXTURES FOR EACH EYE /////////////////////////////////////////////////////////////////////
	// Left eye...
	g_EyeTextures[ovrEye_Left].Header.API = ovrRenderAPI_OpenGL;
	g_EyeTextures[ovrEye_Left].Header.TextureSize = g_RenderTargetSize;
	g_EyeTextures[ovrEye_Left].Header.RenderViewport.Pos.x = 0;
	g_EyeTextures[ovrEye_Left].Header.RenderViewport.Pos.y = 0;
	g_EyeTextures[ovrEye_Left].Header.RenderViewport.Size = l_EyeTextureSizes[ovrEye_Left];
	((ovrGLTexture&)(g_EyeTextures[ovrEye_Left])).OGL.TexId = l_TextureId;

	// Right eye (mostly the same as left but with the viewport on the right side of the texture)
	g_EyeTextures[ovrEye_Right] = g_EyeTextures[ovrEye_Left];
	g_EyeTextures[ovrEye_Right].Header.RenderViewport.Pos.x = (g_RenderTargetSize.w + 1) / 2;
	g_EyeTextures[ovrEye_Right].Header.RenderViewport.Pos.y = 0;

	// OCULUS RIFT EYE CONFIGURATIONS
	g_Cfg.OGL.Header.API = ovrRenderAPI_OpenGL;
	g_Cfg.OGL.Header.BackBufferSize.w = l_ClientSize.w;
	g_Cfg.OGL.Header.BackBufferSize.h = l_ClientSize.h;
	g_Cfg.OGL.Header.Multisample = (L_MULTISAMPLING ? 1 : 0);

	g_Cfg.OGL.Window = glfwGetWin32Window(l_Window);
	g_Cfg.OGL.DC = GetDC(g_Cfg.OGL.Window);

	// Enable capabilities...
	// ovrHmd_SetEnabledCaps(hmd, ovrHmdCap_LowPersistence | ovrHmdCap_DynamicPrediction);

	ovrBool l_ConfigureResult = ovrHmd_ConfigureRendering(hmd, &g_Cfg.Config, G_DISTORTIONCAPS, hmd->MaxEyeFov, g_EyeRenderDesc);
	if (!l_ConfigureResult) {
		printf("Configure failed.\n");
		exit(EXIT_FAILURE);
	}

	// Start the sensor which provides the Rift�s pose and motion...
	uint32_t l_SupportedSensorCaps = ovrTrackingCap_Orientation | ovrTrackingCap_MagYawCorrection | ovrTrackingCap_Position;
	uint32_t l_RequiredTrackingCaps = 0;
	ovrBool l_TrackingResult = ovrHmd_ConfigureTracking(hmd, l_SupportedSensorCaps, l_RequiredTrackingCaps);
	if (!l_TrackingResult) {
		printf("Could not start tracking...");
		exit(EXIT_FAILURE);
	}

	// set projection matrix for each eye
	g_ProjectionMatrix[ovrEye_Left] = ovrMatrix4f_Projection(g_EyeRenderDesc[ovrEye_Left].Fov, 0.3f, 100.0f, true);
	g_ProjectionMatrix[ovrEye_Right] = ovrMatrix4f_Projection(g_EyeRenderDesc[ovrEye_Right].Fov, 0.3f, 100.0f, true);

	// set IPD offset values
	g_EyeOffsets[ovrEye_Left] = g_EyeRenderDesc[ovrEye_Left].HmdToEyeViewOffset;
	g_EyeOffsets[ovrEye_Right] = g_EyeRenderDesc[ovrEye_Right].HmdToEyeViewOffset;

	//glfwSetKeyCallback(l_Window, KeyCallback);
	glfwSetWindowSizeCallback(l_Window, WindowSizeCallback);

	//DECLARE AND CREATE SHADERS ///////////////////////////////////////////////////////////////////////////////////
	Shader phongShader;
	phongShader.createShader("vertexshader.glsl", "fragmentshader.glsl");
	Shader meshShader;
	meshShader.createShader("vshader.glsl", "fshader.glsl");
	Shader sphereShader;
	sphereShader.createShader("vShaderWand.glsl", "fShaderWand.glsl");

	// CREATE MATRIX STACK
	MatrixStack MVstack;
	MVstack.init();
	
	//DECLARE SCENE OBJECTS ///////////////////////////////////////////////////////////////////////////////////

	Plane ground(0.0f, 0.0f, 0.0f, 100.0f, 100.0f);			//Ground plane
	Box board(0.0f, -0.935f, 0.0f, 0.35, 0.01, 0.26);

	Box box(0.0f, -0.935f, 0.0f, 0.5f, 0.2f, 0.5f);			//box in the middle of the wand
	Box boxPoint(0.0f, 0.0f, 0.0f, 0.005f, 0.005f, 0.005f); //hexBox representing the Oculus camera
	hexBox refBox(0.0f, -eyeHeight + 1.5f, -2.0f, 0, 0);

	// ObjectLists
	/*
	vector<Entity*> objectList;
	vector<Entity*> *oPointer;
	vector<Entity*>::iterator it;
	float offset = 0;
	hexBox* tempHex;
	*/
	/*
	// Function-boxes
	float X = 0.142f + 0.215f;
	float Z = -0.0813333f - 0.125f;
	objectList.push_back(new hexBox(X + 0.0f, MIN_HEX_HEIGHT, Z + 0.0f, 3, 0));	// y = -eyeHeight + MIN_HEX_HEIGHT
	objectList.push_back(new hexBox(X + 0.0f, MIN_HEX_HEIGHT, Z - 0.0815f, 2, 0));
	objectList.push_back(new hexBox(X + 0.0f, MIN_HEX_HEIGHT, Z + 0.0815f, 5, 0));
	objectList.push_back(new hexBox(X + 0.07058f, MIN_HEX_HEIGHT, Z + 0.04075f, 6, 0));
	objectList.push_back(new hexBox(X + 0.07058f, MIN_HEX_HEIGHT, Z - 0.04075f, 4, 0));
	objectList.push_back(new hexBox(X - 0.07058f, MIN_HEX_HEIGHT, Z + 0.04075f, 0, 0));
	objectList.push_back(new hexBox(X - 0.07058f, MIN_HEX_HEIGHT, Z - 0.04075f, 1, 0));
	// Movable hexBoxes
	for (int i = 0; i < 48; i++)
	{
		if (i % 2)
			offset = 0;
		else
			offset = -0.04075f;
		for (int j = 0; j < 24; j++)
			objectList.push_back(new hexBox(-1.58f + i * 0.071777f,						// X-axis
											  -eyeHeight - 0.4f,						// Y-axis (-eyeHeight - 0.01, height = 0.1)
											  -1.131f + 0.083984f * j + offset, 0, 1));			// Z-axis
	}
	// Lightsources
	objectList.push_back(new Sphere(0.3f, 0.2f, 0.0f, 0.02f));
	objectList.push_back(new Sphere(-0.3f, 0.2f, 0.0f, 0.02f));
	objectList.push_back(new Sphere(0.0f, 0.2f, -0.5f, 0.02f));
	*/
	// Pointers to the lists
	//oPointer = &objectList;

	// Wand = Box + sphere
	Box boxWand(0.0f, 0.0f, 0.0f, 0.20f, 0.03f, 0.03f);
	Sphere sphereWand(0.0f, 0.0f, 0.0f, 1.0f);

	// Initilise VRPN connection with the Intersense wand
	Vrpn* wand = new Vrpn(true, true, true, "Wand");
	//Passive3D* wand = new Passive3D();

	// read calibration from file and set the transform
	// save configfile as Oculus profile name, if profile name doesn't exist save as: wandCalibration
	readCalibration(wand, eyeHeight, ovrHmd_GetString(hmd, OVR_KEY_USER, "wandCalibration"));

	//readCalibration(wand, eyeHeight);
	

	// TEXTURES ///////////////////////////////////////////////////////////////////////////////////////////////
	glEnable(GL_TEXTURE_2D);
	// Wand function textures
	Texture move("../Textures/move.DDS");
	Texture dilate("../Textures/up.DDS");
	Texture erode("../Textures/down.DDS");
	Texture dnp("../Textures/push.DDS");
	
	// Scene textures
	Texture groundTex("../Textures/floor3.DDS");
	Texture coregister("../Textures/coregister3.DDS");
	Texture hexTex("../Textures/panel3.DDS");

	GLuint currentTexID = move.getTextureID();

	//UNIFORM VARIABLES WITH SHADER ///////////////////////////////////////////////////////////////////////////
	locationMV = glGetUniformLocation(phongShader.programID, "MV");						// ModelView Matrix
	locationP = glGetUniformLocation(phongShader.programID, "P");						// Perspective Matrix
	locationLP = glGetUniformLocation(phongShader.programID, "lightPos");			// Light position
	locationTex = glGetUniformLocation(phongShader.programID, "tex");					// Texture Matrix

	locationMeshMV = glGetUniformLocation(meshShader.programID, "MV");					// ModelView Matrix
	locationMeshP = glGetUniformLocation(meshShader.programID, "P");					// Perspective Matrix
	locationMeshLP = glGetUniformLocation(meshShader.programID, "lightPos");
	locationMeshLP2 = glGetUniformLocation(meshShader.programID, "lightPos2");
	//locationMeshLP2 = glGetUniformLocation(meshShader.programID, "LP2");
	//for (int i = 0; i < nLightsources + 1; i++) {
	//	string uniform = "lightPos[" + to_string(i) + "]";
	//	locationMeshLP[i] = glGetUniformLocation(meshShader.programID, uniform.c_str());			// Light position
	//}

	locationWandMV = glGetUniformLocation(sphereShader.programID, "MV");					// ModelView Matrix
	locationWandP = glGetUniformLocation(sphereShader.programID, "P");					// Perspective Matrix

	//ovrHmd_RecenterPose(hmd);
	ovrHmd_DismissHSWDisplay(hmd); // dismiss health safety warning

	Mesh* mTest = new Mesh(0.05f);

	// Main loop...
	unsigned int l_FrameIndex = 0;
	// RENDER LOOP ////////////////////////////////////////////////////////////////////////////////////////
	while (!glfwWindowShouldClose(l_Window)) {
		// Show fps at the top of the window
		fps = Utilities::displayFPS(l_Window);

		/*

		// STATES //////////////////////////////////////////////////////////////////////////////////////////////
		// All states are originally false
		if (wand->getButtonState() && !buttonPressed && !buttonHeld) { // Button pressed
			buttonPressed = true;
		} else if (!wand->getButtonState()) { // Button released
			buttonReleased = buttonHeld;
			buttonHeld = false;
		} else if (buttonPressed || buttonHeld) { // Button held down
			buttonHeld = true;
			buttonPressed = false;
		}

		if (buttonPressed && wand->getButtonNumber() == 1) {
			readCalibration(wand, eyeHeight);
		}

		// INTERACTION ////////////////////////////////////////////////////////////////////////////////////////
		if (buttonPressed || buttonHeld) {
			switch (wand->getButtonNumber()) {
			// Hotkeys on wand
			case 0: // 2nd from the left
				currentTexID = dilate.getTextureID();
				updatePanel(oPointer, DILATEnERODE, MAX_HEX_HEIGHT, MIN_HEX_HEIGHT);
				//mTest->not_implemented_yet(wand->getTrackerPosition(), false, wandRadius);
				break;
			case 1: // 1st from the left		
				currentTexID = dnp.getTextureID();
				updatePanel(oPointer, DRAGnPULL, MAX_HEX_HEIGHT, MIN_HEX_HEIGHT);
				mTest->sculpt(wand->getWandPosition(), lastPos, wandRadius, true);
				break;
			/*case 2: // 3rd from the left
				currentTexID = move.getTextureID();
				updatePanel(oPointer, moveMESH, MAX_HEX_HEIGHT, MIN_HEX_HEIGHT);
				moveMesh(wand, mTest, buttonPressed, changePos, differenceR);
				break;
			case 3: // 4th from the left
				currentTexID = move.getTextureID();
				updatePanel(oPointer, moveENTITY, MAX_HEX_HEIGHT, MIN_HEX_HEIGHT);
				moveEntity(wand, oPointer, wandRadius);
				break;
			case 4: // co-register, analog button
				currentFunction = coREGISTER;
				ovrHmd_RecenterPose(hmd);
				ovrHmd_DismissHSWDisplay(hmd);
				regCounter = 0;
				renderRegisterSpheres = true;
				wand->setWandTransform(I);
				break;
			case 5: // Trigger button (only non-hotkey)
				if (buttonPressed && currentFunction != coREGISTER)
					if (selectFunction(wand, oPointer, chooseFunction)) {
						resetCounter = 0;
						regCounter = -1;
					}

				if (currentFunction == DILATEnERODE)
					break; //mTest->not_implemented_yet(wand->getTrackerPosition(), false, wandRadius);
				else if (currentFunction == DRAGnPULL)
					mTest->sculpt(wand->getTrackerPosition(), lastPos, wandRadius, true);
				else if (currentFunction == moveMESH)
					moveMesh(wand, mTest, buttonPressed, changePos, differenceR);
				else if (currentFunction == moveENTITY)
					moveEntity(wand, oPointer, wandRadius);
				else if (currentFunction == meshRESET && buttonPressed) {
					resetCounter++;
					if (resetCounter > 1) {
						buttonPressed = false;
						delete mTest;
						mTest = new Mesh(0.3f);
						resetCounter = 0;
						currentFunction = moveMESH;
					}
				}
				else if (currentFunction == hexRESET && buttonPressed) {
					resetCounter++;
					if (resetCounter > 1) {
						buttonPressed = false;
						it = objectList.begin() + nFunctions;
						while (it != objectList.end() - nLightsources) {
							tempHex = static_cast<hexBox*> ((*it));
							tempHex->moveInstant(-eyeHeight - 0.01f);
							++it;
						}
						resetCounter = 0;
						currentFunction = moveMESH;
					}

				}

			default:
				currentFunction = -1;
			}
		}
		// Done when button is released
		if (buttonReleased) {
			it = objectList.begin();
			while (it != objectList.begin() + nFunctions) {
				tempHex = static_cast<hexBox*> ((*it));
				if (currentFunction == tempHex->getFunction())
					tempHex->moveInstant(MAX_HEX_HEIGHT);
				else
					tempHex->moveInstant(MIN_HEX_HEIGHT);
				++it;
			}
		}
			
		// ANALOG BUTTON - change tool size
		if (wand->getAnalogPosition()[0] != 0 || wand->getAnalogPosition()[1] != 0) {
			const float MAX_RADIUS_WAND_TOOL = 0.2f;
			const float MIN_RADIUS_WAND_TOOL = 0.02f;
			// check if tool is to small or to big
			if (wandRadius > MIN_RADIUS_WAND_TOOL && wandRadius < MAX_RADIUS_WAND_TOOL) {
				wandRadius += 0.001f*wand->getAnalogPosition()[1];
			} else if (wandRadius <= MIN_RADIUS_WAND_TOOL && wand->getAnalogPosition()[1] > 0) {
				wandRadius += 0.001f*wand->getAnalogPosition()[1];
			} else if (wandRadius >= MAX_RADIUS_WAND_TOOL && wand->getAnalogPosition()[1] < 0) {
				wandRadius += 0.001f*wand->getAnalogPosition()[1];
			}
		}*/
		//////////////////////////////////////////////////////////////////////////////////////////////
		// KEYBORD EVENTS
		if (glfwGetKey(l_Window, GLFW_KEY_O)) {
			ovrHmd_RecenterPose(hmd);
			ovrHmd_DismissHSWDisplay(hmd);
		}
		if (glfwGetKey(l_Window, GLFW_KEY_SPACE)) {
			mTest->sculpt(wand->getWandPosition(), lastPos, wandRadius, true);
		}
		if (glfwGetKey(l_Window, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(l_Window, GL_TRUE);
		}
		if (glfwGetKey(l_Window, GLFW_KEY_Q)) {
			wandRadius += 0.01f;
		}
		if (glfwGetKey(l_Window, GLFW_KEY_W)) {
			wandRadius -= 0.01f;
		}
		if (glfwGetKey(l_Window, GLFW_KEY_R)) {
			delete mTest; // Reset mesh
			mTest = new Mesh(0.3f);
		}
		// Activate wireframe (hold L)
		if (glfwGetKey(l_Window, GLFW_KEY_L) == GLFW_PRESS && !lines) {
			lines = true;
		} else if (glfwGetKey(l_Window, GLFW_KEY_L) == GLFW_RELEASE && lines){
			lines = false;
		}
		///////////////////////////////////////////////////////////////////////////////////////////////////////
		lastPos[0] = wand->getWandPosition()[0];
		lastPos[1] = wand->getWandPosition()[1];
		lastPos[2] = wand->getWandPosition()[2];

		// Begin the frame...
		ovrHmd_BeginFrame(hmd, l_FrameIndex);

		// Get eye poses for both the left and the right eye. g_EyePoses contains all Rift information: orientation, positional tracking and
		// the IPD in the form of the input variable g_EyeOffsets.
		ovrHmd_GetEyePoses(hmd, l_FrameIndex, g_EyeOffsets, g_EyePoses, NULL);

		// Bind the FBO...
		glBindFramebuffer(GL_FRAMEBUFFER, l_FBOId);

		GLRenderCallsOculus();

		
		for (int l_EyeIndex = 0; l_EyeIndex<ovrEye_Count; l_EyeIndex++) {
			
			//OCULUS/CAMERA TRANSFORMS ------------------------------------------------------------------------------
			MVstack.push();
				ovrEyeType l_Eye = hmd->EyeRenderOrder[l_EyeIndex];

				glViewport(g_EyeTextures[l_Eye].Header.RenderViewport.Pos.x,
					g_EyeTextures[l_Eye].Header.RenderViewport.Pos.y,
					g_EyeTextures[l_Eye].Header.RenderViewport.Size.w,
					g_EyeTextures[l_Eye].Header.RenderViewport.Size.h);

				glUseProgram(phongShader.programID);
				// Pass projection matrix on to OpenGL...
				glUniformMatrix4fv(locationP, 1, GL_FALSE, &(g_ProjectionMatrix[l_Eye].Transposed().M[0][0]));
				glUniform1i(locationTex, 0);

				// Multiply with orientation retrieved from sensor...
				OVR::Quatf l_Orientation = OVR::Quatf(g_EyePoses[l_Eye].Orientation);
				OVR::Matrix4f l_ModelViewMatrix = OVR::Matrix4f(l_Orientation.Inverted());
				MVstack.multiply(&(l_ModelViewMatrix.Transposed().M[0][0]));


				//!-- Translation due to positional tracking (DK2) and IPD...
				float eyePoses[3] = { -g_EyePoses[l_Eye].Position.x, -g_EyePoses[l_Eye].Position.y, -g_EyePoses[l_Eye].Position.z };
				MVstack.translate(eyePoses);
				
				//POSSABLY DOABLE IN SHADER
				pmat4 = MVstack.getCurrentMatrix();
				for (int i = 0; i < 16; i++)
					mat4[i] = pmat4[i];

				linAlg::transpose(mat4);
				linAlg::vectorMatrixMult(mat4, lPos, LP);
				linAlg::vectorMatrixMult(mat4, lPos2, lPosTemp);
				glUniformMatrix4fv(locationMV, 1, GL_FALSE, MVstack.getCurrentMatrix());
			
				//SCENEOBJECT TRANSFORMS----------------------------------------------------------------
				MVstack.push();
					glBindTexture(GL_TEXTURE_2D, hexTex.getTextureID());

					//RENDER DIFFERENT HEXBOXES---------------------------------------------------------------------
					refBox.render();
					glUniform4fv(locationLP, 1, LP);
					/*
					it = objectList.begin();
					n = 0;
					while (it != objectList.begin() + nFunctions) {
						tempHex = static_cast<hexBox*> ((*it));
						tempHex->setFunction(n);
						(*it)->render();
						n++;
						++it;
					}
					while (it != objectList.end() - nLightsources) {
						(*it)->render();
						++it;
					}
					*/
					/*
					// Lightsources - remember to send as unifor
					while (it != objectList.end()) {
						MVstack.push();
							MVstack.translate((*it)->getPosition());
							glUniformMatrix4fv(locationMV, 1, GL_FALSE, MVstack.getCurrentMatrix());
							(*it)->render();
						MVstack.pop();
						++it;
					}
					*/
					/*
					MVstack.push();
						translateVector[0] = 0.0f;
						translateVector[1] = -eyeHeight;
						translateVector[2] = 0.0f;
						MVstack.translate(translateVector);
						glUniformMatrix4fv(locationMV, 1, GL_FALSE, MVstack.getCurrentMatrix());
						glUniform4fv(locationLP, 1, LP);
						glBindTexture(GL_TEXTURE_2D, groundTex.getTextureID());
						ground.render();
					MVstack.pop();
					*/
					//TRACKINGRANGE
					MVstack.push();
						MVstack.translate(board.getPosition());
						glUniformMatrix4fv(locationMV, 1, GL_FALSE, MVstack.getCurrentMatrix());
						glBindTexture(GL_TEXTURE_2D, groundTex.getTextureID());
						//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
						//glLineWidth(5.0f);
						board.render();
						//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					MVstack.pop();

					glBindTexture(GL_TEXTURE_2D, 0);
					//RENDER MESH -----------------------------------------------------------------------
					glUseProgram(meshShader.programID);
					glUniformMatrix4fv(locationMeshP, 1, GL_FALSE, &(g_ProjectionMatrix[l_Eye].Transposed().M[0][0]));

					MVstack.push();
						MVstack.translate(mTest->getPosition());
						MVstack.multiply(mTest->getOrientation());
						glUniformMatrix4fv(locationMeshMV, 1, GL_FALSE, MVstack.getCurrentMatrix());
						//cout << lPosTemp[0] << " " << lPosTemp[1] << " " << lPosTemp[2] << " " << lPosTemp[3] << endl;
						glUniform4fv(locationMeshLP, 1, LP);
						glUniform4fv(locationMeshLP2, 1, lPosTemp);

						//if (lines) {
						if (true) {
							glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
							mTest->render();
							glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
						}
						else
							mTest->render();
					MVstack.pop();
						
					glUseProgram(phongShader.programID);
					glUniformMatrix4fv(locationP, 1, GL_FALSE, &(g_ProjectionMatrix[l_Eye].Transposed().M[0][0]));
					//RENDER WAND---------------------------------------------------------------------------
					MVstack.push();
						MVstack.translate(wand->getWandPosition());
						//MVstack.rotZ(3.141592f / 2.f);
						MVstack.multiply(wand->getWandOrientation());

						glUniformMatrix4fv(locationMV, 1, GL_FALSE, MVstack.getCurrentMatrix());
					//	boxPoint.render();

						MVstack.push();
							translateVector[0] = -0.1f;
							translateVector[1] = 0.0f;
							translateVector[2] = 0.0f;
							MVstack.translate(translateVector);
							glUniformMatrix4fv(locationMV, 1, GL_FALSE, MVstack.getCurrentMatrix());
							glBindTexture(GL_TEXTURE_2D, hexTex.getTextureID());
							boxWand.render();
						MVstack.pop();

						MVstack.push();
							MVstack.scale(wandRadius);
							glUseProgram(sphereShader.programID);
							glUniformMatrix4fv(locationWandP, 1, GL_FALSE, &(g_ProjectionMatrix[l_Eye].Transposed().M[0][0]));
							glUniformMatrix4fv(locationWandMV, 1, GL_FALSE, MVstack.getCurrentMatrix());
							sphereWand.render();
						MVstack.pop();	
					MVstack.pop();
						
					glUseProgram(phongShader.programID);
				MVstack.pop();

			MVstack.pop();			
		}

		// Back to the default framebuffer...
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//Wand callback from VRPN
		//wand->sendtoMainloop();

		// Do everything, distortion, front/back buffer swap...
		ovrHmd_EndFrame(hmd, g_EyePoses, g_EyeTextures);
		++l_FrameIndex;
		glfwPollEvents();
	}

	// Clean up FBO...
	glDeleteRenderbuffers(1, &l_DepthBufferId);
	glDeleteTextures(1, &l_TextureId);
	glDeleteFramebuffers(1, &l_FBOId);

	// Clean up Oculus...
	ovrHmd_Destroy(hmd);
	ovr_Shutdown();

	// Clean up window...
	glfwDestroyWindow(l_Window);
	glfwTerminate();

	return 1;
}

static void WindowSizeCallback(GLFWwindow* p_Window, int p_Width, int p_Height) {
    if (p_Width>0 && p_Height>0) {
        g_Cfg.OGL.Header.BackBufferSize.w = p_Width;
        g_Cfg.OGL.Header.BackBufferSize.h = p_Height;

        ovrBool l_ConfigureResult = ovrHmd_ConfigureRendering(hmd, &g_Cfg.Config, G_DISTORTIONCAPS, hmd->MaxEyeFov, g_EyeRenderDesc);
        if (!l_ConfigureResult) {
            printf("Configure failed.\n");
            exit(EXIT_FAILURE);
        }
    }
}
void GLRenderCallsOculus(){
    // Clear...
    //GL calls
    glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    //glDisable(GL_TEXTURE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glFrontFace(GL_CCW);
    if (L_MULTISAMPLING) {
        glEnable(GL_MULTISAMPLE);
    }
    else {
        glDisable(GL_MULTISAMPLE);
    }
}
void updatePanel(vector<Entity*> *objectList, int currentFunction, float MAX_HEX_HEIGHT, float MIN_HEX_HEIGHT) {
    hexBox* tempHex;
    vector<Entity*>::iterator it = objectList->begin();
    while (it != objectList->begin() + nFunctions) {
        tempHex = static_cast<hexBox*> ((*it));
        if (currentFunction == tempHex->getFunction())
            tempHex->moveInstant(MAX_HEX_HEIGHT);
        else
            tempHex->moveInstant(MIN_HEX_HEIGHT);
        ++it;
    }
}

bool selectFunction(Wand* wand, vector<Entity*> *objectList, int& chooseFunction) {
	vector<Entity*>::iterator it = objectList->begin();
	hexBox *tempHex;
	float vLength[3];
	float radius = 0.04075f;

	while (it != objectList->begin() + nFunctions) {
		tempHex = static_cast<hexBox*> ((*it));
		vLength[0] = wand->getWandPosition()[0] - (*it)->getPosition()[0];
		vLength[1] = wand->getWandPosition()[1] - (*it)->getPosition()[1];
		vLength[2] = wand->getWandPosition()[2] - (*it)->getPosition()[2];
		if (linAlg::vecLength(vLength) < radius) {
			chooseFunction = tempHex->getFunction();
			return true;
		}
		++it;
	}
	return false;
}
void moveMesh(Wand* wand, Mesh* mTest, bool buttonPressed, float* changePos, float* differenceR) {
    // Save first wand rotation transform in wandR
    float* wandR = wand->getWandOrientation();
    float resultR[16];
    float resultPos[3];

    if (buttonPressed) {
        // Offset translation back to the original position of the mesh
        changePos[0] = mTest->getPosition()[0] - wand->getWandPosition()[0];
		changePos[1] = mTest->getPosition()[1] - wand->getWandPosition()[1];
		changePos[2] = mTest->getPosition()[2] - wand->getWandPosition()[2];

        // Get the difference betweeen the original mesh rotation transform and wandR  --   wandR * differenceR = meshR
        float* meshR = mTest->getOrientation();
        float invWandR[16] = { 0.0f };
        linAlg::invertMatrix(wandR, invWandR);
        linAlg::matrixMult(invWandR, meshR, differenceR);
    }

    // Resulting translation to be made on the mesh calculated from origin.
	resultPos[0] = wand->getWandPosition()[0] + changePos[0];
	resultPos[1] = wand->getWandPosition()[1] + changePos[1];
	resultPos[2] = wand->getWandPosition()[2] + changePos[2];

    // Resulting rotation to be made on the mesh
    linAlg::matrixMult(wandR, differenceR, resultR);

    mTest->setPosition(resultPos);
    mTest->setOrientation(resultR);
}
void moveEntity(Wand* wand, vector<Entity*> *objectList, float wandRadius) {
    hexBox *tempHex;
    vector<Entity*> selectedList;
    vector<Entity*>::iterator it;
    bool find = false;
    float vLength[3];

    // Look for lightsources
    it = objectList->end() - nLightsources;
    while (it != objectList->end() && (*it)->getOtype() == 'S') {
        vLength[0] = wand->getWandPosition()[0] - (*it)->getPosition()[0];
        vLength[1] = wand->getWandPosition()[1] - (*it)->getPosition()[1];
        vLength[2] = wand->getWandPosition()[2] - (*it)->getPosition()[2];
        if (linAlg::vecLength(vLength) < wandRadius) {
            find = true;
            selectedList.push_back((*it));
			break;
        }
        ++it;
    }
    // Look for hexboxes
    if (!find) {
        it = objectList->begin() + nFunctions;
        while (it != objectList->end() - nLightsources) {
            // Create vectors between wand and object, depending on object type
            vLength[0] = wand->getWandPosition()[0] - ((*it))->getPosition()[0];
            vLength[1] = 0.0f;
            vLength[2] = wand->getWandPosition()[2] - ((*it))->getPosition()[2];
            // Select object if the distance is smaller than wandRadius
            if (linAlg::vecLength(vLength) < wandRadius) {
                selectedList.push_back((*it));
            }
            ++it;
        }
    }
    // Move selected objects
    it = selectedList.begin();
	if (selectedList.size() != 0) {
		if ((*it)->getOtype() == 'S')
			while (it != selectedList.end()) {
				(*it)->setPosition(wand->getWandPosition());
				++it;
			}
		else
			while (it != selectedList.end()) {
				tempHex = static_cast<hexBox*> ((*it));
				tempHex->move(wand->getWandPosition()[1]);
				++it;
			}
	}
    
}

void print_FLOAT_matrix(float* M) {
    for (int i = 0; i < 16; i++) {
        cout << std::fixed << std::setprecision(2);
        cout << M[i] << "  ";
        if (i == 3 || i == 7 || i == 11)	cout << endl;
    }
    cout << endl << "---------------------" << endl;
}

void readCalibration(Vrpn* wand, float& eyeHeight, const char* profileName){

	string line;
	float value;
	int i = 0;
	float transform[16] = { 0.0f };

	string fileName(profileName);

	ifstream wandCalibration(fileName + ".ini");
	if (wandCalibration.is_open()) {
		while (getline(wandCalibration, line)) {
			std::istringstream in(line);
			in >> value;
			if (i < 16) {
				transform[i] = value;
				i++;
			}
			else {
				eyeHeight = value;
				wand->setWandTransform(transform);
			}
		}
		wandCalibration.close();
	}

	else cout << "No configuration file found, calibrate the wand";
}