/* CIS565 CUDA Checker: A simple CUDA hello-world style program for 
   Patrick Cozzi's CIS565: GPU Computing at the University of Pennsylvania
   Written by Yining Karl Li, Liam Boone, and Harmony Li.
   Copyright (c) 2014 University of Pennsylvania */

#ifndef MAIN_HPP
#define MAIN_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glslUtil/glslUtility.hpp>

#include <cuda_runtime.h>
#include <cuda_gl_interop.h>
#include <iostream>
#include <stdlib.h>
#include <string>

#include "kernel.h"

//====================================
// GL Stuff
//====================================
GLuint 							m_pbo = (GLuint) NULL;
GLFWwindow*         m_window;
std::string 			  m_yourName;
unsigned int 			  m_width; 
unsigned int 				m_height;
unsigned int 				m_major; 
unsigned int 				m_minor;
GLuint 							m_positionLocation = 0;
GLuint 							m_texcoordsLocation = 1;
GLuint 							m_image;

//====================================
// Main
//====================================
int main(int argc, char* argv[]);

//====================================
// Main loop
//====================================
void MainLoop();
void ErrorCallback(int error, const char* description);
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void RunCuda();

//====================================
// Setup/Init Stuff
//====================================
bool Init();
void InitPBO(GLuint* pbo);
void InitCuda();
void InitTextures();
void InitVAO();
GLuint InitShader();

//====================================
// Cleanup Stuff
//====================================
void CleanupCuda();
void DeletePBO(GLuint* pbo);
void DeleteTexture(GLuint* tex);

#endif
