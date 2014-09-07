Project 0
=========

Project 0 : Get CUDA set up and running

## NOTE :
This project (and all other projects in this course) requires a NVIDIA graphics
card with CUDA capabilityi!  Any card with compute capability 2.0 and up will
work.  This means any card from the GeForce 400 and 500 series and afterwards
will work.  If you do not have a machine with these specs, feel free to use
computers in the SIG Lab.  All computers in SIG lab have CUDA capable cards and
should already have the CUDA SDK installed. 

Unlike other semesters, the computers in Moore 100 do
NOT meet specifications.  If you have any issue with accessing SIG lab, contact
Harmony (harmoli@seas.upenn.edu) as soon as possible.

## PART 1 : INSTALL CUDA
### WINDOWS
* 1. Make sure you are running Windows XP/Vista/7 and that your graphics drivers
   are up to date. You will need support for OpenGL 4.0 or better in this class.
* 2. Make sure you have installed Visual Studio 2010 (available for free to
   Penn Engineering students from MSDN Academic Alliance). Visual Studio 2012/3
   should also be compatible with a little elbow grease and supports some of 
   the newer features of C++11. 
* 3. Install the CUDA 4.0 (or greater) For Windows Toolkit, Development
   Drivers, and SDK from http://developer.nvidia.com/cuda/cuda-downloads
   or https://developer.nvidia.com/cuda-toolkit-archive. Make sure you
   download and install the appropriate versions of each package for your
   machine. (As of version 5.5 all three components are bundeled together
   in one package). We suggest installing CUDA 6.5, as later labs will also be
   using this.
* 4. You're good to go! All Windows base code for CIS565 will make use of
   Visual Studio 2010 and will be distributed as Visual Studio 2010
   projects.

### MAC OSX
* 1. Make sure you are running OSX 10.9. We will need OpenGL 4.0 support
   in this class, so 10.9 is mandatory if you are planning on using OS X.
* 2. Make sure you have installed XCode (available for free from the App
   Store).
* 3. Make sure you have installed the OSX Unix Command Line Development
   Tools (XCode->Preferences->Downloads->Command Line Tools->Install).
* 4. Install the CUDA 4.0 (or greater) For Mac Toolkit, Development Drivers, and 
   SDK from http://developer.nvidia.com/cuda/cuda-downloads or 
   https://developer.nvidia.com/cuda-toolkit-archive. (As of version 5.5 all 
   three components are bundeled together in one package).
* 5. Install CMake from http://www.cmake.org/.  (NOTE: Homebrew will only
     install version 2.8.11, and we are targeting 3.0.+) Unless there is a need,
     we will provide CMake files from which you can build the makefiles.  If you 
     have an issue with this, feel free to contact Harmony.
* 6. You're good to go! All OSX base code for CIS565 will make use of 
   the Unix make toolchain, hence the need for OSX Unix Command Line Development 
   Tools.

## PART 2 : FORK / CLONE PROJECT 0
* 1. Use Github to fork this repository into your own account.
* 2. Clone from Github onto your machine.
NOTE : If you are unfamiliar with git, or would like a refresher, refer to this
primer doc.

## PART 3 : BUILD / RUN PROJECT 0
Project 0 is a simple program that demonstrates CUDA and OpenGL functionality and
interoperability on systems that CUDA has been properly installed on. If the
machine you are working on has CUDA properly set up and has OpenGL 4.0 support,
then when you run CUDATEST, you should see a window displaying either two
horizonal colored bars OR one solid color. What gets displayed depends on your
graphics card, so your results will most likely vary from some or many of your
classmates. The window title should be a combination of your name and the model
of your graphics card.

Please note: The Windows version of Project 0 uses GLEW/freeglut, whereas the
OSX version of Project 0 uses GLFW. The two versions of CUDATEST are
functionally the same and share the same CUDA code, but the GL harnesses for
each are slightly different.

WINDOWS:

Contents:

The Windows version of CUDATEST is in the PROJ0_WIN/ folder. The contents of the
folder are the following:

src/ contains the actual source code for CUDATEST
external/ contains the libraries and includes for freeglut and glew
To Build:

Open CIS565_PROJ_0.sln in Visual Studio 2010. The project should build straight from
Visual Studio 2010 without modification.

To Run:

CIS565_PROJ_0 should run straight from Visual Studio 2010 without modification after
building if CUDA has been installed properly on your machine.

OSX:

Contents:

The OSX version of must be generated and made from the command line.

To Generate:
Create a new folder in and cd into it.  Run 'cmake ../' from the folder.  This
will produce cmake cache files and a makefile.  

To Build:
Run 'make' from the folder you just created.  This will produce an executable
named CIS565_PROJ_0.  

To Run:
Run CIS565_PROJ_0 from the command line by calling it as a relative path from
the current directory.  

## PART 4 : MODIFY AND SUBMIT
* 1. Line 13 of main.cpp contains a variable string set by default to "Your Name
   Here". Replace "Your Name Here" with your name, rebuild, and run CUDATEST.
   Take a screenshot of Project 0 working on your machine.
* 2. ADD your screenshot to your Git repository and check in your modified
    main.cpp and added screenshot.
* 2.5 If you are using Windows, make sure your modified CUDATEST can run in
    the SIG Lab.
* 3. Open a Pull Request so we can see that you have finished.
* 4. Send an email to Harmony with your name, github account name, and the
    grade you believe you deserve.
* 5. You're done with Project 0!
