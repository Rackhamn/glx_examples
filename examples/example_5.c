// 2022-08-10
// Example 5: OpenGL X11 Cube render inside of FPS loop.
// gcc -std=c99 -g example_5.c ../common/timing.c ../common/mat4.c -D_GNU_SOURCE -lm -lX11 -lXrandr -lXext -lGL -lGLU -o example; ./example
#define TITLE	"glx example 5"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#define GL_GLEXT_PROTOTYPES
#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glext.h>

#include "../common/timing.h"
#include "../common/vertex.h"
#include "../common/models.h"
#include "../common/math.h"
#include "../common/shader.h"

// camera
float crot = A2R * -90;
vec3_ c_eye = { 7, 0, 35 }, c_dir = { 0, 0, 1 }, c_up = { 0, 1, 0 }; // camera

#define DEFAULT_WIDTH	512
#define DEFAULT_HEIGHT	512

#define GLX_CONTEXT_MAJOR_VERSION_ARB	   0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB	   0x2092
typedef GLXContext(*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

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
	GLuint pos_loc, col_loc, norm_loc, uniform_mvp_loc;
	GLuint uniform_model_matrix_loc, uniform_view_matrix_loc, uniform_proj_matrix_loc, uniform_light_loc;
	GLuint standard_shader;
	mat4_ m4_camera, m4_proj, m4_mvp;
	mat4_ umat, scale_matrix, rot_matrix, trans_matrix;
	
	struct model_manager mm;
	
	int fps, frames_left;
	double time, deltatime;
	unsigned long int frame_ms; // frame time aprox
	unsigned long int frame_start, frame_end;
	
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
	
	
	if(load_shader("vs.glsl", "fs.glsl", &standard_shader) != 0) {
		printf("Bad Shader. Something went wrong. -> Exit\n");
		exit(-10);	
	}
	glUseProgram(standard_shader);
	
	printf("Find shader attribute & uniform locations.\n");
	pos_loc = glGetAttribLocation(standard_shader, "in_vertex");
	norm_loc = glGetAttribLocation(standard_shader, "in_normal");
	col_loc = glGetAttribLocation(standard_shader, "in_color");
	
	uniform_model_matrix_loc = glGetUniformLocation(standard_shader, "u_model_matrix");
	uniform_view_matrix_loc = glGetUniformLocation(standard_shader, "u_view_matrix");
	uniform_proj_matrix_loc = glGetUniformLocation(standard_shader, "u_proj_matrix");
	uniform_light_loc = glGetUniformLocation(standard_shader, "u_light_pos");
	
	printf("Allocate VAO/VBO.\n");
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	
	glEnableVertexAttribArray(pos_loc);
	glVertexAttribPointer(pos_loc, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	
	glEnableVertexAttribArray(norm_loc);
	glVertexAttribPointer(norm_loc, 3, GL_FLOAT, GL_FALSE, 36, (void*)(12));
	
	glEnableVertexAttribArray(col_loc);
	glVertexAttribPointer(col_loc, 3, GL_FLOAT, GL_FALSE, 36, (void*)(24));
	
	// set depth
	glEnable(GL_DEPTH_TEST);
  	glDepthFunc(GL_LESS);
  	
  	// set culling
  	glFrontFace(GL_CCW);
  	glEnable(GL_CULL_FACE);
  	glCullFace(GL_BACK);
  	
  	// set blending
  	glEnable(GL_BLEND);  
  	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  	
	// calculate runtime
	fov = 60;
	fps = 30;
	time = 0;
	deltatime = 1.0 / (double)fps;
	frame_ms = (unsigned long int)(deltatime * 1000);
	frames_left = fps * 100 * 10; // lifetime
	
	// calculate the perspective projection matrix
  	mat4_identity(m4_proj);
	mat4_perspective(fov * A2R, (float)win_width / (float)win_height, 0.001, 1000.0, m4_proj);
	
	// load and push meshes.
	// todo: use matching tags/id/hash to find the right one for the 'model' entity.
	mm_init(&mm, 32, 32);
	
	// load meshes
	int cube_id = mm_load_mesh(&mm, "../models/cube.obj");
	int isocahedron_id = mm_load_mesh(&mm, "../models/isocahedron.obj");
	int suzanne_id = mm_load_mesh(&mm, "../models/suzanne.obj");
	int torus_id = mm_load_mesh(&mm, "../models/torus.obj");
	
	// load meshes into GL
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	mm_write_meshes_to_opengl(&mm);
	
	// push models (instances of meshes)
	vec3_ scale = { 2, 2, 2 };
	mm_push_model(&mm, cube_id, (vec3_){ 0, 0, 0 }, (vec3_){ 0, 0, 0 }, (vec3_){ 1, 1, 1 } );
	mm_push_model(&mm, cube_id, (vec3_){ -5, 0, 0 }, (vec3_){ 0, 0, 0 }, scale );
	mm_push_model(&mm, isocahedron_id, (vec3_){ 0, 0, 0 }, (vec3_){ 0, 0, 0 }, scale );
	mm_push_model(&mm, suzanne_id, (vec3_){ 5, 0, 0 }, (vec3_){ 0, 0, 0 }, scale );
	mm_push_model(&mm, torus_id, (vec3_){ 10, 0, 0 }, (vec3_){ 0, 0, 0 }, scale );
	
	// note: can remove the client side meshes (that we have loaded) since their data is on the GPU.
	
	XAutoRepeatOff(display);
	
	#define KI_LEFT 	0
	#define KI_RIGHT	1
	#define KI_UP		2
	#define KI_DOWN		3
	int reload_shaders = 0;
	
	int ki[4] = { 0 }; // temp keyboard
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
				if(xev.xkey.keycode == 113) ki[KI_LEFT] = 1;
				if(xev.xkey.keycode == 114) ki[KI_RIGHT] = 1;
				if(xev.xkey.keycode == 116) ki[KI_UP] = 1;
				if(xev.xkey.keycode == 111) ki[KI_DOWN] = 1;
			}
			if(xev.type == KeyRelease) {
				if(xev.xkey.keycode == 113) ki[KI_LEFT] = 0;
				if(xev.xkey.keycode == 114) ki[KI_RIGHT] = 0;
				if(xev.xkey.keycode == 116) ki[KI_UP] = 0;
				if(xev.xkey.keycode == 111) ki[KI_DOWN] = 0;
				if(xev.xkey.keycode == 71) reload_shaders = 1; // F5
				if(xev.xkey.keycode == 33) {
					// P
					printf("camera pos: %f, %f, %f\n", c_eye[0], c_eye[1], c_eye[2]);
					printf("camera dir: %f, %f, %f\n", c_dir[0], c_dir[1], c_dir[2]);
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
				mat4_identity(m4_proj);
				mat4_perspective(fov * A2R,
								(float)win_width / (float)win_height,
								0.001, 1000.0, m4_proj);
			} else {
				// X11 Window was moved.
			}
		}
		
		if(reload_shaders) {
			reload_shaders = 0;
			
			glUseProgram(0);
			glDeleteProgram(standard_shader);
			
			standard_shader = -1;
			if(load_shader("vs.glsl", "fs.glsl", &standard_shader) != 0) {
				printf("Bad Shader. Something went wrong. -> Exit\n");
				exit(-10);	
			}
			glUseProgram(standard_shader);
			
			// "reread the attribute data locations and enable vertex attribs
			pos_loc = glGetAttribLocation(standard_shader, "in_vertex");
			norm_loc = glGetAttribLocation(standard_shader, "in_normal");
			col_loc = glGetAttribLocation(standard_shader, "in_color");
	
			uniform_model_matrix_loc = glGetUniformLocation(standard_shader, "u_model_matrix");
			uniform_view_matrix_loc = glGetUniformLocation(standard_shader, "u_view_matrix");
			uniform_proj_matrix_loc = glGetUniformLocation(standard_shader, "u_proj_matrix");
			uniform_light_loc = glGetUniformLocation(standard_shader, "u_light_pos");
			
			glEnableVertexAttribArray(pos_loc);
			glVertexAttribPointer(pos_loc, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	
			glEnableVertexAttribArray(norm_loc);
			glVertexAttribPointer(norm_loc, 3, GL_FLOAT, GL_FALSE, 36, (void*)(12));
	
			glEnableVertexAttribArray(col_loc);
			glVertexAttribPointer(col_loc, 3, GL_FLOAT, GL_FALSE, 36, (void*)(24));
			
			printf("reloaded shader F5\n");
		}
		
		mat4_identity(m4_proj);
		mat4_perspective(fov * A2R,
						(float)win_width / (float)win_height,
						0.001, 1000.0, m4_proj);
		
		mat4_identity(m4_camera);
		
		float sign = 0;
		if(ki[KI_LEFT]) sign = -1;
		if(ki[KI_RIGHT]) sign = 1;
		
		// rotate around self.
		crot += sign * deltatime;
		c_dir[0] = cos(crot);
		c_dir[1] = 0;
		c_dir[2] = sin(crot);
		
		// forward, backwards
		sign = 0;
		if(ki[KI_UP]) sign = -1;
		if(ki[KI_DOWN]) sign = 1;
		if(sign > 0) vec3_add(c_eye, c_dir, c_eye);
		if(sign < 0) vec3_sub(c_eye, c_dir, c_eye);
		
		mat4_lookat(c_eye, c_dir, c_up, m4_camera);
		
		// RENDER:
		glClearColor(0, 0.5, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glUseProgram(standard_shader);
		
		float light_pos[3] = { 10, 10, 40 };
		glUniformMatrix4fv(uniform_view_matrix_loc, 1, GL_FALSE, (GLfloat *)m4_camera);
		glUniformMatrix4fv(uniform_proj_matrix_loc, 1, GL_FALSE, (GLfloat *)m4_proj);
		glUniform3fv(uniform_light_loc, 1, (GLfloat *)light_pos);
		
		vec3_copy(light_pos, mm.models[0].pos);
		// rotate all around self.
		float r = time;
		int i = 1;
		while(i < mm.model_count) {
			mm.models[i].rot[1] = r;
			i++;
		}
		
		mm_render_models_opengl(&mm, uniform_model_matrix_loc);
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glUseProgram(0);
		
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
	
	mm_free(&mm);
	
	return 0;
}
