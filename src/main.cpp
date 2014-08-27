/* CIS565 CUDA Checker: A simple CUDA hello-world style program for 
   Patrick Cozzi's CIS565: GPU Computing at the University of Pennsylvania
   Written by Yining Karl Li, Liam Boone, and Harmony Li.
   Copyright (c) 2014 University of Pennsylvania */

#include "main.hpp"

//====================================
// Main
//====================================
int main(int argc, char* argv[]){ 
    // Change this line to use your name!
    m_yourName = "[YOUR NAME HERE]";

    if(Init(argc, argv) == true){
#if _WIN32
		// Glut callbacks
		glutDisplayFunc(MainLoop);
		glutKeyboardFunc(KeyCallback);

		// Start Glut's main loop
		glutMainLoop();
#else 
		MainLoop();
#endif
    }

    return 0;
}

//====================================
// Setup/Init Stuff
//====================================
bool Init(int argc, char **argv){
    // Set window title to "Student Name: GPU Name"
    std::string deviceName;
    cudaDeviceProp deviceProp; 
    int gpudevice = 0;
    int device_count = 0;
    cudaGetDeviceCount(&device_count); 
    if(gpudevice > device_count){ 
        std::cout << "Error: GPU device number is greater than the number of devices!" << 
                     "Perhaps a CUDA-capable GPU is not installed?" << std::endl; 
        return false;
    } 
    cudaGetDeviceProperties(&deviceProp, gpudevice); 
    deviceName = deviceProp.name;
    deviceName = m_yourName + ": " + deviceProp.name;
    m_major = deviceProp.major;
    m_minor = deviceProp.minor;

    // Window setup stuff
#if _WIN32
	glutInit(&argc, argv);
	
	m_width = 800;
	m_height = 800;

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(m_width, m_height);
	m_window = glutCreateWindow(deviceName.c_str());
#else
    glfwSetErrorCallback(ErrorCallback);

    if (!glfwInit()){
        return false;
    }
    m_width = 800;
    m_height = 800;
    m_window = glfwCreateWindow(m_width, m_height, deviceName.c_str(), NULL, NULL);
    if (!m_window){
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(m_window);
    glfwSetKeyCallback(m_window, KeyCallback);
#endif

    glewExperimental = GL_TRUE;
    if(glewInit()!=GLEW_OK){
        return false;
    }

    // Init all of the things
    InitVAO();
    InitTextures();
    InitCuda();
    InitPBO(&m_pbo);

    GLuint passthroughProgram;
    passthroughProgram = InitShader();
    glUseProgram(passthroughProgram);
    glActiveTexture(GL_TEXTURE0);

    return true;
}

void InitPBO(GLuint* pbo){
    if(pbo){
        // set up vertex data parameter
        int num_texels = m_width*m_height;
        int num_values = num_texels * 4;
        int size_tex_data = sizeof(GLubyte) * num_values;

        // Generate a buffer ID called a PBO (Pixel Buffer Object)
        glGenBuffers(1,pbo);
        // Make this the current UNPACK buffer (OpenGL is state-based)
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, *pbo);
        // Allocate data for the buffer. 4-channel 8-bit image
        glBufferData(GL_PIXEL_UNPACK_BUFFER, size_tex_data, NULL, GL_DYNAMIC_COPY);
        cudaGLRegisterBufferObject(*pbo);
    }
}

void InitVAO(){
    GLfloat vertices[] =
    { 
        -1.0f, -1.0f, 
         1.0f, -1.0f, 
         1.0f,  1.0f, 
        -1.0f,  1.0f, 
    };

    GLfloat texcoords[] = 
    { 
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f
    };

    GLushort indices[] = { 0, 1, 3, 3, 1, 2 };

    GLuint vertexBufferObjID[3];
    glGenBuffers(3, vertexBufferObjID);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)m_positionLocation, 2, GL_FLOAT, GL_FALSE, 0, 0); 
    glEnableVertexAttribArray(m_positionLocation);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords), texcoords, GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint)m_texcoordsLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(m_texcoordsLocation);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBufferObjID[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void InitCuda(){
    // Default to device ID 0. If you have more than one GPU and want to test a non-default one,
    // change the device ID.
    cudaGLSetGLDevice(0);

    // Clean up on program exit
    atexit(CleanupCuda);
}

void InitTextures(){
    glGenTextures(1,&m_image);
    glBindTexture(GL_TEXTURE_2D, m_image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_BGRA, 
                 GL_UNSIGNED_BYTE, NULL);
}

GLuint InitShader(){
    const char *attributeLocations[] = { "Position", "Tex" };
    GLuint program = glslUtility::createDefaultProgram(attributeLocations, 2);
    GLint location;
    glUseProgram(program);
    if((location = glGetUniformLocation(program, "u_image")) != -1){
        glUniform1i(location, 0);
    }
    return program;
}

//====================================
// Main loop
//====================================
void RunCuda(){
    // Map OpenGL buffer object for writing from CUDA on a single GPU
    // No data is moved (Win & Linux). When mapped to CUDA, OpenGL should not use this buffer
    uchar4 *dptr = NULL;
    cudaGLMapBufferObject((void**)&dptr, m_pbo);

    // Execute the kernel
    CudaKernel(dptr, m_width, m_height, m_major, m_minor);

    // Unmap buffer object
    cudaGLUnmapBufferObject(m_pbo);
}

void MainLoop(){
#if _WIN32
	RunCuda();

	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_pbo);
    glBindTexture(GL_TEXTURE_2D, m_image);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_RGBA, 
                        GL_UNSIGNED_BYTE, NULL);
    glClear(GL_COLOR_BUFFER_BIT);   

    // VAO, shader program, and texture already bound
    glDrawElements(GL_TRIANGLES, 6,  GL_UNSIGNED_SHORT, 0);
	glutSwapBuffers();
#else
    while(!glfwWindowShouldClose(m_window)){
        glfwPollEvents();
        RunCuda();

        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_pbo);
        glBindTexture(GL_TEXTURE_2D, m_image);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_RGBA, 
                        GL_UNSIGNED_BYTE, NULL);
        glClear(GL_COLOR_BUFFER_BIT);   

        // VAO, shader program, and texture already bound
        glDrawElements(GL_TRIANGLES, 6,  GL_UNSIGNED_SHORT, 0);
        glfwSwapBuffers(m_window);
    }
    glfwDestroyWindow(m_window);
    glfwTerminate();
#endif
}


void ErrorCallback(int error, const char* description){
    fputs(description, stderr);
}

#if _WIN32
void KeyCallback(unsigned char key, int x, int y) {
	switch (key) {
	case 27: // Escape key
	case 'q':
		Cleanup();
		break;
	}
}
#else
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}
#endif

//====================================
// Cleanup Stuff
//====================================
#if _WIN32
void Cleanup(){
	if (m_window) {
		glutDestroyWindow(m_window);
	}
	exit(0);
}
#endif

void CleanupCuda(){
    if(m_pbo){
        DeletePBO(&m_pbo);
    } 
    if(m_image){
        DeleteTexture(&m_image);
    } 
}

void DeletePBO(GLuint* pbo){
    if(pbo){
        // unregister this buffer object with CUDA
        cudaGLUnregisterBufferObject(*pbo);

        glBindBuffer(GL_ARRAY_BUFFER, *pbo);
        glDeleteBuffers(1, pbo);

        *pbo = (GLuint)NULL;
    }
}

void DeleteTexture(GLuint* tex){
    glDeleteTextures(1, tex);
    *tex = (GLuint)NULL;
}
