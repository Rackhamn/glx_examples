#ifndef MAT4_H
#define MAT4_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#ifndef R2A
	// Radian to Angle
	#define R2A		(57.295779513082)
#endif

#ifndef A2R
	// Angle to Radian
	#define A2R		(0.01745329252)
#endif

typedef struct mat4_s {
	float data[16];
} mat4;

void printf_mat4(mat4 * m);
void mat4_mul(mat4 * ma, mat4 * mb);
void mat4_mulr(mat4 * m, mat4 * ma, mat4 * mb);
void mat4_identity(mat4 * m);
void mat4_perspective(mat4 * m, float rad_fov, float ratio, float near, float far);
void mat4_lookat(mat4 * m, struct vertex pos, struct vertex target, struct vertex up);
void mat4_transpose(mat4 * m);
void mat4_rotate_x(mat4 * m, float rad);
void mat4_rotate_y(mat4 * m, float rad);
void mat4_rotate_z(mat4 * m, float rad);
void mat4_rotate(mat4 * m, float radx, float rady, float radz);
void mat4_mvp(mat4 * m, mat4 * model, mat4 * view, mat4 * proj);

void mat4_test(void);
// TODO: implement cross, norm, dot ... cos, sin ... mul by scalar, mul by vec2
// TODO: implement mat3 as well.

#endif /* MAT4_H */
