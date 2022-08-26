#ifndef CAMERA_H
#define CAMERA_H

#include <stdio.h>
#include <math.h>

#include "vertex.h"
#include "mat4.h"

typedef struct camera_s {
	// int id;
	vec3 pos;
	vec3 forward; 
	vec3 up;
	vec3 right; // cache
	// vec3 move_vel;
	// vec3 rot_vel;
	// mat4 view_matrix; // cache
} camera;

vec3 cross_vec3(vec3 a, vec3 b);
vec3 normalize_vec3(vec3 a);
float dot_vec3(vec3 a, vec3 b);

void init_camera(camera * c);
void set_camera_pos(camera * c, float x, float y, float z);
void move_camera_forward(camera * c, float force);
void move_camera_side(camera * c, float force);
void rotate_camera_y(camera * c, float force);

void look_at_matrix(mat4 * m, vec3 target, vec3 source);
void calc_camera_view_matrix(mat4 * m, camera * c);
		
#endif /* CAMERA_H */
