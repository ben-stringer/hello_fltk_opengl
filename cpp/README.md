README.md

#mview

Mesh viewer written in C++ using OpenGL and GLUT.  Supports the .m mesh file format.

##Development

This program was written and tested on Red Hat Enterprise Linux 8 (RHEL), Community Enterprise Operating System 8 (CentOS), and Apple's macOS.

The development environment was primarily the standard unix operating system tools. No Integrated Development Environment (IDE) was used, although minor work would be needed to import this project into Eclipse or Visual Studio. A simple Makefile was assembled for building the project, and Vim was used as the editor.

##Building

To build, install the standard build tools and the freeglut development tools using yum.

~~~bash
sudo yum install "Development Tools" freeglut-devel
~~~

Note that there may be additional development tools required that were already pre-installed on the system, however all tools used were available in the standard Red Hat repositories.  No third-party tools were used.

Finally, to build the project, simply run GNU make. This will result in an executable file named 'assignment2'.

~~~bash
$> make
$> make -j9
~~~

##Running

~~~bash
$> ./assignment2
$> ./assignment2 /path/to/bunny.m
~~~

Very little changes were made to the application template, however there were a few additions.  To deal with the lack of a middle mouse button, I added key bindings to zoom in and out.  Zooming in can be achieved with the '+' or '=' button, and zooming out can be achieved with the '-' or '_' button.  Finally, I added support for the 'r' button to reset the view to the initial view from when the application is first launched.
