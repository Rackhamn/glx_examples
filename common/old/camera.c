#include "camera.h"

vec3 cross_vec3(vec3 a, vec3 b) {
	vec3 c;
	
	c.x = (a.y * b.z) - (b.y * a.z);
	c.y = (a.z * b.x) - (b.z * a.x);
	c.z = (a.x * b.y) - (b.x * a.y);
	
	return c;
}

vec3 normalize_vec3(vec3 a) {
	float m = (a.x*a.x) + (a.y*a.y) + (a.z*a.z);
	m = sqrt(m);
	
	if(m > 0.0) {
		a.x /= m;
		a.y /= m;
		a.z /= m;
	}
	
	return a;
}

float dot_vec3(vec3 a, vec3 b) {
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

void init_camera(camera * c) {
	vec3 zero = { 0, 0, 0 };
	
	c->pos = zero;
	
	c->forward.x = 0;
	c->forward.y = 0;
	c->forward.z = -1;
	
	c->up.x = 0;
	c->up.y = 1;
	c->up.z = 0;
	
	// forward cross up = left
	c->right.x = -1;
	c->right.y = 0;
	c->right.z = 0;
	
	printf("fwd: %f, %f, %f\n", c->forward.x, c->forward.y, c->forward.z);
	printf("up: %f, %f, %f\n", c->up.x, c->up.y, c->up.z);
	
	printf("* left: %f, %f, %f\n", c->right.x, c->right.y, c->right.z);
	
	vec3 temp;
	
	temp = cross_vec3(c->forward, c->up); // right
	// temp = cross_vec3(c->up, c->forward);
	printf("left: %f, %f, %f\n", temp.x, temp.y, temp.z);
}

void set_camera_pos(camera * c, float x, float y, float z) {
	c->pos.x = x;
	c->pos.y = y;
	c->pos.z = z;
}

void move_camera_forward(camera * c, float force) {
	c->pos.x += c->forward.x * force;
	c->pos.y += c->forward.y * force;
	c->pos.z += c->forward.z * force;
}

void move_camera_side(camera * c, float force) {
	c->pos.x += c->right.x * force;
	c->pos.y += c->right.y * force;
	c->pos.z += c->right.z * force;
}

void rotate_camera_y(camera * c, float force) {
	float x, z;
	float _cos, _sin;
	
	#if 0
	
	x = c->forward.x;
	z = c->forward.z;
	
	_cos = cos(force);
	_sin = sin(force);
	
	c->forward.x = x * _cos - z * _sin;
	c->forward.y = 0;
	c->forward.z = z * _cos + x * _sin;
	c->forward = normalize_vec3(c->forward);
	
	c->right = cross_vec3(c->forward, c->up);
	#endif
}

void look_at_matrix(mat4 * m, vec3 target, vec3 source) {
	const vec3 temp_up = { 0, 1, 0 };
	
	vec3 forward, right, up;
	
	forward.x = target.x - source.x;
	forward.y = target.y - source.y;
	forward.z = target.z - source.z;
	forward = normalize_vec3(forward);
	
	right = cross_vec3(forward, temp_up);
	right = normalize_vec3(right);
	
	up = cross_vec3(forward, right);
	up = normalize_vec3(up);
	
	float * d = m->data;
	mat4_identity(m);
	
	#if 0
	// X
	d[0] = right.x;
	d[1] = right.y;
	d[2] = right.z;
	
	// Y
	d[4] = up.x;
	d[5] = up.y;
	d[6] = up.z;
	
	// Z
	d[8] = forward.x;
	d[9] = forward.y;
	d[10] = forward.z;
	
	// POS
	d[12] = source.x;
	d[13] = source.y;
	d[14] = source.z;
	#else
	/*
	u	right
	v	up
	n	backward (-forwards)
	*/
	
	float tx = -dot_vec3(right, source);
	float ty = -dot_vec3(up, source);
	float tz = -dot_vec3(forward, source);
	
	// X
	d[0] = right.x;
	d[1] = up.x;
	d[2] = forward.x;
	
	// Y
	d[4] = right.y;
	d[5] = up.y;
	d[6] = forward.y;
	
	// Z
	d[8] = right.z;
	d[9] = up.z;
	d[10] = forward.z;
	
	// POS
	d[12] = tx;
	d[13] = ty;
	d[14] = tz;
	#endif
	
	/*
	printf("fwd: %f, %f, %f\n", forward.x, forward.y, forward.z);
	printf("pos: %f, %f, %f\n", source.x, source.y, source.z);
	*/
}

void calc_camera_view_matrix(mat4 * m, camera * c) {
	float * d = m->data;
	mat4_identity(m);
	
	// X
	d[0] = c->right.x;
	d[1] = c->right.y;
	d[2] = c->right.z;
	
	// Y
	d[4] = c->up.x;
	d[5] = c->up.y;
	d[6] = c->up.z;
	
	// Z
	d[8] = c->forward.x;
	d[9] = c->forward.y;
	d[10] = c->forward.z;
	
	// mat4_mul(m, m);
	
	// POS
	d[12] = c->pos.x;
	d[13] = c->pos.y;
	d[14] = c->pos.z;
	
	printf("pos: %f, %f, %f\n", c->pos.x, c->pos.y, c->pos.z);
}
