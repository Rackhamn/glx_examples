// 2022-08-05
// gcc -std=c99 -g opengl_triangle.c x11.c -D_GNU_SOURCE -lm -lX11 -lXrandr -lXext -lGL -lGLU -o tri;

#include "common.h"
// #include "x11.h"

#define GL_GLEXT_PROTOTYPES why

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>

#include <GL/glx.h>
#include <GL/glu.h>
#include <GL/gl.h>

// Normalized pixel coordinates (from -1 to 1)
const char * shader_vtx = "\
#version 330 \n\
in vec3 pos; \n\
void main() { \n\
	gl_Position = vec4(pos, 1); \n\
}";

const char * shader_frag = "\
#version 330 \n\
out vec4 fragColor; \n\
void main() \n\
{ \n\
	vec2 iResolution = vec2(512.0, 512.0); \n\
	vec2 uv = (gl_FragCoord.xy/iResolution.xy)*2.0 - vec2(1.0,1.0); \n\
	uv.y *= iResolution.y/iResolution.x; \n\
	fragColor = abs(uv.yxyx); \n\
}";

#define CANVAS_WIDTH 512
#define CANVAS_HEIGHT 512
#define DEBUG 	1

static unsigned char fbdata[4 * CANVAS_HEIGHT * CANVAS_WIDTH];

int main(int argc, char ** argv) {
	// asm volatile("sub $8, %rsp\n");
	Display * dpy = NULL;
	Window root = 0, win = 0;
	XVisualInfo * vi = NULL;
	Colormap cmap;
	
	// to hide cursor with
	XColor xcolor; 
	Pixmap csr;
	Cursor cursor; 
	
	// TODO: somewhere there is uninitialized (e.g: UB) variables.
	
	// likely uninit here.
	static GLint att[2] = { GLX_RGBA, None };
	static XSetWindowAttributes swa = { .override_redirect = 1, .event_mask = ExposureMask | KeyPressMask };
		
	/*
	static XSetWindowAttributes swa = { 
		.x = 0, .y = 0, .width = CANVAS_WIDTH, .height = CANVAS_HEIGHT,
		.border_width = 1,
		event_mask = ExposureMask | KeyPressMask | OverideRedirect,
		.visual = vi->visual };
	*/
	
	GLXContext glc;
	GLuint textureA;
	GLuint fboA;
	GLuint isCompiled, f, v, p;
	
	dpy = XOpenDisplay("");
	root = DefaultRootWindow(dpy);

	// drops data
	vi = glXChooseVisual(dpy, 0, att);

	//I really hate this and I wish this call was unneeded. it feels useless
	cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);

	//hide cursor
	#if 0
	csr = XCreatePixmap(dpy,root,1,1,1);
	cursor = XCreatePixmapCursor(dpy,csr,csr,&xcolor,&xcolor,1,1); 
	#endif

	//this enables things like events, fullscreen, and sets the invisible cursor
	
	swa.colormap = cmap;
	swa.cursor = cursor;
	win = XCreateWindow(dpy, root, 0, 0, CANVAS_WIDTH, CANVAS_HEIGHT, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask | CWOverrideRedirect | CWCursor, &swa);
	
	// center the OpenGL window
	XMoveWindow(dpy, win, 
				(1600 * .5) - (CANVAS_WIDTH * .5), 
				(900 * .5) - (CANVAS_HEIGHT * .5));

	// something seems wrong with the window. (no border or anything)
	// this actually opens the window
	XMapWindow(dpy, win);
	#if 0
	XMapRaised(dpy, win);
	XClearWindow(dpy, win);
	XFlush(dpy);
	#endif

	//now we can do opengl calls!!!!
	glc = glXCreateContext(dpy, vi, NULL, 1);

	if (glc == NULL) {
		printf("Error with OpenGL Context\n");
		return 0;
	}
	
	#if 0
	// check that OpenGL 4.5 is available
	if(1) {
		GLint major_ver = 0, minor_ver = 0, profile = 0, flags = 0;
		
		glGetIntegerv(GL_MAJOR_VERSION, &major_ver);
		glGetIntegerv(GL_MINOR_VERSION, &minor_ver);
		// glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile);
		// glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
		
		printf("GL STRING VERSION: %s\n", glGetString(GL_VERSION));
		
		printf("GL VERSION: %i.%i\n", major_ver, minor_ver);
		
		if(major_ver == 4 && minor_ver == 5) {
			printf("GL VERSION 4.5 SUPPORTED!\n");
		} else {
			printf("GL VERSION 4.5 *** NOT *** SUPPORTED!\n");
			// return -1;
		}
	}
	#endif

	glXMakeCurrent(dpy, win, glc);

	glClear(GL_COLOR_BUFFER_BIT);

	//oh yeah grab the keyboard
	XGrabKeyboard(dpy, win, 1, GrabModeAsync, GrabModeAsync, CurrentTime);

	//create a floating point backing texture for a framebuffer
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &textureA); // GL_TEXTURE_2D
	glBindTexture(GL_TEXTURE_2D, textureA);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, CANVAS_WIDTH, CANVAS_HEIGHT, 0, GL_BGRA, GL_UNSIGNED_BYTE, fbdata);

	//create a framebuffer we can render everything to
	glGenFramebuffers(1, &fboA);
	glBindFramebuffer(GL_FRAMEBUFFER, fboA);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, textureA, 0);

	// compile vertex shader
	isCompiled = 0;
	v = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(v, 1, &shader_vtx, NULL);
	glCompileShader(v);

	#ifdef DEBUG
		isCompiled = 0;
		glGetShaderiv(v, GL_COMPILE_STATUS, &isCompiled);
		if(isCompiled == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(v, GL_INFO_LOG_LENGTH, &maxLength);

			char* error = malloc(maxLength);
			glGetShaderInfoLog(v, maxLength, &maxLength, error);
			printf("%s\n", error);

			exit(-10);
		}
	#endif
	
	// compile fragment shader
	isCompiled = 0;
	f = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(f, 1, &shader_frag, NULL);
	glCompileShader(f);

	#ifdef DEBUG
		isCompiled = 0;
		glGetShaderiv(f, GL_COMPILE_STATUS, &isCompiled);
		if(isCompiled == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(f, GL_INFO_LOG_LENGTH, &maxLength);

			char* error = malloc(maxLength);
			glGetShaderInfoLog(f, maxLength, &maxLength, error);
			printf("%s\n", error);

			exit(-10);
		}
	#endif

	// link shader
	p = glCreateProgram();
	glAttachShader(p,v);
	glAttachShader(p,f);
	glLinkProgram(p);

	#ifdef DEBUG
		GLint isLinked = 0;
		glGetProgramiv(p, GL_LINK_STATUS, (int *)&isLinked);
		if (isLinked == GL_FALSE) {
			GLint maxLength = 0;
			glGetProgramiv(p, GL_INFO_LOG_LENGTH, &maxLength);

			char* error = malloc(maxLength);
			glGetProgramInfoLog(p, maxLength, &maxLength,error);
			printf("%s\n", error);

			exit(-10);
		}
	#endif

	glUseProgram(p);
	glFlush();

	//switch to using our framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, fboA);

	glFinish();
	glRecti(-1, -1, 1, 1);
	glFinish();

#if 0
	glBegin(GL_TRIANGLES);
	
		glColor4f(1, 0, 0, 1); // red
		
		glVertex3f(0, 0, -1);
		glVertex3f(1, 0, -1);
		glVertex3f(0, 1, -1);
		
	glEnd();
#endif

	//blit our framebuffer to the screen
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fboA);
	

	glBlitFramebuffer(0, 0, CANVAS_WIDTH, CANVAS_HEIGHT, 0, 0, CANVAS_WIDTH, CANVAS_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	static XEvent xev;
	while(1) {
		XNextEvent(dpy, &xev);

		if(xev.type == KeyPress && xev.xkey.keycode == 0x09) {
			break;
		}

		#if 0
		//wait for escape key, then exit without glib :3
		if(xev.type == KeyPress && xev.xkey.keycode == 0x09) {
			//blackle mori no likey AT&T
			asm volatile(".intel_syntax noprefix");
			asm volatile("push 60");
			asm volatile("pop rax");
			asm volatile("xor edi, edi");
			asm volatile("syscall");
			asm volatile(".att_syntax prefix");
			__builtin_unreachable();
		}
		#endif
	}
	
	// free the things as well

	// remove data from GLX
	glDeleteShader(v);
	glDeleteShader(f);
	glDeleteProgram(p);
	glDeleteTextures(1, &textureA);
	glDeleteFramebuffers(1, &fboA);
	
	glXDestroyContext(dpy, glc);
	
	// remove data from X11
	XUngrabKeyboard(dpy, CurrentTime);
	
	// XVisualInfo * vi;
	// XFree(vi->visual);
	XFree(vi);
	
	// cursor, pixmap, attributes
	XUnmapWindow(dpy, win);
	XDestroyWindow(dpy, win);
	
	XCloseDisplay(dpy);
	
	return 0;
}
