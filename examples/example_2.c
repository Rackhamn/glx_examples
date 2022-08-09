// 2022-08-09
// Example 2: OpenGL X11 quad render inside of FPS loop.
// gcc -std=c99 -g example_2.c common/timing.c common/mat4.c -D_GNU_SOURCE -lm -lX11 -lXrandr -lXext -lGL -lGLU -o example; ./example
#define TITLE	"glx example 2"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#define GL_GLEXT_PROTOTYPES
#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glext.h>

#include "../common/timing.h"
#include "../common/mat4.h"

typedef struct model_s {
	int id;
	float x, y, z; // pos
	float rx, ry, rz; // rot
	float sx, sy, sz; // scale
} model;

const struct vertex quad_vertices[6] = {
//   X		Y	   Z	 R	 G	 B		(FLOAT)
	{  1.0, -1.0, 0.0, 1.0, 0.0, 0.0 }, // top right
	{ -1.0, -1.0, 0.0, 0.0, 1.0, 0.0 }, // top left
	{ -1.0,  1.0, 0.0, 0.0, 0.0, 1.0 }, // bot left
	{  1.0, -1.0, 0.0, 1.0, 0.0, 0.0 }, // top right 2
	{ -1.0,  1.0, 0.0, 0.0, 0.0, 1.0 }, // bot left 2
	{  1.0,  1.0, 0.0, 0.0, 1.0, 0.0 }, // bot right
};

const char * vertex_shader_src = "\
#version 450 core \n\
in vec3 in_vertex; \n\
in vec3 in_color; \n\
uniform mat4 u_mvp; \n\
out vec4 out_color; \n\
void main() { \n\
	gl_Position = vec4(in_vertex.xyz, 1) * u_mvp; \n\
	out_color = vec4(in_color, 1); \n\
} \n\
";

const char * fragment_shader_src = "\
#version 450 core \n\
in vec4 out_color; \n\
void main() { \n\
	gl_FragColor = out_color; \n\
} \n\
";

#define DEFAULT_WIDTH	512
#define DEFAULT_HEIGHT	512

#define GLX_CONTEXT_MAJOR_VERSION_ARB	   0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB	   0x2092
typedef GLXContext(*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

int compile_shader(const char * vs, const char * fs, GLuint * shader_id) {
	GLuint vertex_shader, fragment_shader, shader_program;
	GLint is_compiled, is_linked, max_length;
	
	// compile vertex shader
	is_compiled = 0;
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vs, NULL);
	glCompileShader(vertex_shader);

	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &is_compiled);
	if(is_compiled == GL_FALSE) {
		max_length = 0;
		
		glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &max_length);
		
		char* error = malloc(max_length);
		glGetShaderInfoLog(vertex_shader, max_length, &max_length, error);
		printf("%s\n", error);
		
		return -1;
	}
	
	// compile fragment shader
	is_compiled = 0;
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fs, NULL);
	glCompileShader(fragment_shader);

	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &is_compiled);
	if(is_compiled == GL_FALSE) {
		max_length = 0;
		
		glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &max_length);
		
		char* error = malloc(max_length);
		glGetShaderInfoLog(fragment_shader, max_length, &max_length, error);
		printf("%s\n", error);
		free(error);
		
		return -2;
	}
	
	// link shader
	shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);

	is_linked = 0;
	glGetProgramiv(shader_program, GL_LINK_STATUS, (int *)&is_linked);
	if(is_linked == GL_FALSE) {
		max_length = 0;
		glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &max_length);

		char* error = malloc(max_length);
		glGetProgramInfoLog(shader_program, max_length, &max_length,error);
		printf("%s\n", error);
		free(error);
		
		return -3;
	}
	
	printf("Shader program compiled successfully.\n");
	
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	
	*shader_id = shader_program;
	
	return 0;
}

int main(int argc, char ** argv) {
	// general
	int fov;
	int win_width, win_height;
	int fbcount;
	Display * display;
	Window win;
	GLXFBConfig * fbc;
	XVisualInfo * vi;
	XSetWindowAttributes swa;
	GLXContext ctx;
	Atom _wm_delete_window;
	
	// runtime
	int xev_count, config_count;
	XEvent xev;
	GLuint vao, vbo;
	GLuint pos_loc, col_loc, uniform_mvp_loc;
	mat4 m4_camera, m4_proj, m4_mvp;
	mat4 umat, scale_matrix, rot_matrix, trans_matrix;
	
	GLuint standard_shader;
	
	int fps, frames_left;
	double time, deltatime;
	unsigned long int frame_ms; // frame time aprox
	unsigned long int frame_start, frame_end;
	
	// model data (for transformations)
	int mcount = 1;
	model models[1];
	
	display = XOpenDisplay("");
	glXCreateContextAttribsARBProc glXCreateContextAttribsARB = NULL; // function

	const char * extensions = glXQueryExtensionsString(display, DefaultScreen(display));
	// printf("%s\n\n", extensions);
	
	static int visual_attribs[] =
	{
		GLX_RENDER_TYPE, GLX_RGBA_BIT,
		GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
		GLX_DOUBLEBUFFER, 1,
		GLX_RED_SIZE, 1,
		GLX_GREEN_SIZE, 1,
		GLX_BLUE_SIZE, 1,
		GLX_DEPTH_SIZE, 16,
		None
	};

	fbcount = 0;
	fbc = glXChooseFBConfig(display, DefaultScreen(display), visual_attribs, &fbcount);
	if(!fbc) {
		printf("Failed to retrieve a framebuffer config.\n");
		return 1;
	}
	printf("%i framebuffer configs found.\n", fbcount);

	printf("Getting XVisualInfo.\n");
	vi = glXGetVisualFromFBConfig(display, fbc[0]);

	printf("Creating colormap.\n");
	swa.colormap = XCreateColormap(display, RootWindow(display, vi->screen), vi->visual, AllocNone);
	swa.border_pixel = 0;
	swa.event_mask = StructureNotifyMask;
	
	win_width = DEFAULT_WIDTH;
	win_height = DEFAULT_HEIGHT;
	printf("Creating Window.\n");
	win = XCreateWindow(display, RootWindow(display, vi->screen), 0, 0,
						win_width, win_height,
						0, vi->depth, InputOutput, vi->visual,
						CWBorderPixel | CWColormap | CWEventMask,
						&swa);
	if(!win) {
		printf("Failed to create window\n");
		return 1;
	}
	
	printf("Set window input.\n");
	XSelectInput(display, win,
		KeyPressMask | KeyReleaseMask |
		PointerMotionMask | ButtonPressMask | ButtonReleaseMask |
		FocusChangeMask |
		StructureNotifyMask |
		PropertyChangeMask |
		SubstructureNotifyMask | SubstructureRedirectMask);
	
	printf("Set X11 Atom Protocols.\n");
	// _NET_CLOSE_WINDOW
	_wm_delete_window = XInternAtom(display, "WM_DELETE_WINDOW", 0);
	XSetWMProtocols(display, win, &_wm_delete_window, 1);	
	
	printf("Set window title.\n");
	XStoreName(display, win, TITLE);

	printf("Mapping window.\n");
	XMapWindow(display, win);
	
	if(1) {
		GLXContext ctx_old = glXCreateContext(display, vi, 0, GL_TRUE);
		glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)glXGetProcAddress((const GLubyte*)"glXCreateContextAttribsARB");
		
		// intermediate context for some quick and dirty checks
		glXMakeCurrent(display, win, ctx_old);
		printf("\nOpenGL Information:\n");
		printf("Card Vendor    = %s\n", glGetString(GL_VENDOR));
		printf("Renderer       = %s\n", glGetString(GL_RENDERER));
		printf("OpenGL Version = %s\n", glGetString(GL_VERSION));
		printf("GLSL Version   = %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
		printf("\n");
		
		glXMakeCurrent(display, 0, 0);
		glXDestroyContext(display, ctx_old);
	}
	
	if(glXCreateContextAttribsARB == NULL) {
		printf("glXCreateContextAttribsARB entry point not found. Aborting.\n");
		return 1;
	}
	
	const int _majv = 3, _minv = 3;
	static int context_attribs[] = {
		GLX_CONTEXT_MAJOR_VERSION_ARB, _majv,
		GLX_CONTEXT_MINOR_VERSION_ARB, _minv,
		// GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_DEBUG_BIT_ARB,
		// GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
		None
	};

	printf("Creating OpenGL %i.%i context\n", _majv, _minv);
	ctx = glXCreateContextAttribsARB(display, fbc[0], NULL, 1, context_attribs);
	if(!ctx) {
		printf("Failed to create GL %i.%i context.\n", _majv, _minv);
		return 1;
	}
	glXMakeCurrent(display, win, ctx);
	
	if(compile_shader(vertex_shader_src, fragment_shader_src, &standard_shader) != 0) {
		printf("Bad Shader. Something went wrong. -> Exit\n");
		exit(-10);
	}
	glUseProgram(standard_shader);
	
	printf("Find shader attribute & uniform locations.\n");
	pos_loc = glGetAttribLocation(standard_shader, "in_vertex");
	col_loc = glGetAttribLocation(standard_shader, "in_color");	
	uniform_mvp_loc = glGetUniformLocation(standard_shader, "u_mvp");
	
	printf("Allocate VAO/VBO.\n");
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(struct vertex) * 6, quad_vertices, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(pos_loc);
	glVertexAttribPointer(pos_loc, 3, GL_FLOAT, GL_FALSE, 24, (void*)0);
	
	glEnableVertexAttribArray(col_loc);
	glVertexAttribPointer(col_loc, 3, GL_FLOAT, GL_FALSE, 24, (void*)(12));

	glEnable(GL_DEPTH_TEST);
  	glDepthFunc(GL_LESS);
  	
	// calculate runtime
	fov = 60;
	fps = 30;
	time = 0;
	deltatime = 1.0 / (double)fps;
	frame_ms = (unsigned long int)(deltatime * 1000);
	frames_left = fps * 100; // lifetime
	
	// calculate the perspective projection matrix
  	mat4_identity(&m4_proj);
	mat4_perspective(&m4_proj, fov * A2R, (float)win_width / (float)win_height, 0.001, 1000.0);
	
	// calculate the view matrix (camera)
	// simple translation pointng towards center of world.
	mat4_identity(&m4_camera);
	m4_camera.data[12] = 0;
	m4_camera.data[13] = 0;
	m4_camera.data[14] = -1;
	
	// set up the translation data for the model (triangle)
	models[0].id = 0;
	models[0].x = 0;
	models[0].y = 0;
	models[0].z = -5;
	models[0].sx = 1;
	models[0].sy = 1;
	models[0].sz = 1;
	models[0].rx = 0;
	models[0].ry = 0;
	models[0].rz = 0;
	
	XAutoRepeatOff(display);
	
	while(frames_left > 0) {
		frame_start = get_time_ns();
		
		// HANDLE INPUT
		config_count = 0;
		xev_count = XPending(display);
		while(xev_count) {
			XNextEvent(display, &xev);
			
			if(xev.type == KeyPress) {
				if(xev.xkey.keycode == 0x09) {
					// escape
					printf("Quit via Escape Key [0x09].\n");
					goto _lbl_end_of_sim;
				}
			}
			else if(xev.type == ConfigureNotify) {
				config_count++;
			}
			else if(xev.type == ClientMessage) {
				Atom protocol = (Atom)xev.xclient.data.l[0];
				if(protocol == _wm_delete_window) {
					printf("Quit via window button (Atom).\n");
					goto _lbl_end_of_sim;
				}
			}
			
			xev_count--;
		}
		
		if(config_count) {
			XWindowAttributes attribs;
			XGetWindowAttributes(display, win, &attribs);
			
			if((int)attribs.width != win_width || (int)attribs.height != win_height) {
				win_width = (int)attribs.width;
				win_height = (int)attribs.height;
				
				// dont allow a divide by 0
				if(win_height == 0) win_height = 1;
				
				glViewport(0, 0, win_width, win_height);
				printf("Resize GL viewport & X11 window (%i, %i).\n", win_width, win_height);
				
				// because the window properties changed, the projection is recalculated.
				mat4_identity(&m4_proj);
				mat4_perspective(&m4_proj,
								fov * A2R,
								(float)win_width / (float)win_height,
								0.001, 1000.0);
			} else {
				// X11 Window was moved.
			}
		}
		
		// RENDER:
		glClearColor(0, 0.5, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glUseProgram(standard_shader);
		
		// realtime model rotation:
		float rad = time * A2R * 6.28 * 10;
		models[0].rx = rad;
		models[0].ry = rad;
		models[0].rz = rad;
		
		int i = 0;
		while(i < mcount) {			
			// 1. Scale
			mat4_identity(&scale_matrix);
			scale_matrix.data[0] = models[i].sx;
			scale_matrix.data[5] = models[i].sy;
			scale_matrix.data[10] = models[i].sz;
			
			// 2. Rotate
			mat4_identity(&rot_matrix);
			mat4_rotate(&rot_matrix, models[i].rx, models[i].ry, models[i].rz);
			
			// 3. Translate
			mat4_identity(&trans_matrix);
			trans_matrix.data[12] = models[i].x;
			trans_matrix.data[13] = models[i].y;
			trans_matrix.data[14] = models[i].z;
			
			// build the Model matrix
			mat4_identity(&m4_mvp);
			mat4_mul(&m4_mvp, &scale_matrix);
			mat4_mul(&m4_mvp, &rot_matrix);
			mat4_mul(&m4_mvp, &trans_matrix);
			
			// build the Model-View-Projection matrix
			mat4_identity(&umat);
			mat4_mul(&umat, &m4_mvp);
			mat4_mul(&umat, &m4_camera);
			mat4_mul(&umat, &m4_proj);
			
			// transpose the row major (C) matrix into column major ordering (Opengl).
			mat4_transpose(&umat); 
			glUniformMatrix4fv(uniform_mvp_loc, 1, GL_FALSE, (GLfloat *)umat.data);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			
			i++;
		}
		
		glUseProgram(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		
		glXSwapBuffers(display, win);
		glFlush();
		
		frame_end = get_time_ns();
		sleep_ms(frame_start, frame_end, frame_ms);
		
		time += deltatime;
		frames_left--;
	}
	_lbl_end_of_sim:
	
	printf("Clean up OpenGL.\n");
	glDeleteProgram(standard_shader);
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	ctx = glXGetCurrentContext();
	glXDestroyContext(display, ctx);
	glXMakeCurrent(display, 0, 0);
	
	printf("Clean up X11\n");
	XAutoRepeatOn(display);
	XFree(fbc); // GLFXBConfig
	XFree(vi);
	XUnmapWindow(display, win);
	XDestroyWindow(display, win);	
	XCloseDisplay(display);	
	
	return 0;
}
