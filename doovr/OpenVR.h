#pragma once
#ifndef Utilities_H_
#define Utilities_H_

//! Namespace that handles rendering to an Oculus Rift
namespace OpenVR {
	//! Handles tracking devices and objects in a scene. Renders to an Oculus Rift
	int runOpenVR(int argc, char* argv[]);
}

#endif