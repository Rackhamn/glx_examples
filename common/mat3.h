#ifndef MATH_MAT3_H
#define MATH_MAT3_H

#include "math.h"

/* *** MATRIX IS COLUMN MAJOR LIKE IN OPENGL *** */
/* *** CAMERA AND PROJECTION IS RIGHT HANDED *** */

void mat3_zero(mat3_ a) {
	a[0][0] = 0;
	a[0][1] = 0;
	a[0][2] = 0;
	
	a[1][0] = 0;
	a[1][1] = 0;
	a[1][2] = 0;
	
	a[2][0] = 0;
	a[2][1] = 0;
	a[2][2] = 0;
}

void mat3_identity(mat3_ a) {
	a[0][0] = 1;
	a[0][1] = 0;
	a[0][2] = 0;
	
	a[1][0] = 0;
	a[1][1] = 1;
	a[1][2] = 0;
	
	a[2][0] = 0;
	a[2][1] = 0;
	a[2][2] = 1;
}

void mat3_copy(mat3_ a, mat3_ out) {
	// could use memcpy, likely slower.
	
	out[0][0] = a[0][0];
	out[0][1] = a[0][1];
	out[0][2] = a[0][2];
	
	out[1][0] = a[1][0];
	out[1][1] = a[1][1];
	out[1][2] = a[1][2];
	
	out[2][0] = a[2][0];
	out[2][1] = a[2][1];
	out[2][2] = a[2][2];
}

// todo: rewrite
void mat3_mul(mat3_ a, mat3_ b, mat3_ out) {
	float 
		a00 = a[0][0], a01 = a[0][1], a02 = a[0][2],
		a10 = a[1][0], a11 = a[1][1], a12 = a[1][2],
		a20 = a[2][0], a21 = a[2][1], a22 = a[2][2],

		b00 = b[0][0], b01 = b[0][1], b02 = b[0][2],
		b10 = b[1][0], b11 = b[1][1], b12 = b[1][2],
		b20 = b[2][0], b21 = b[2][1], b22 = b[2][2];

	out[0][0] = a00 * b00 + a10 * b01 + a20 * b02;
	out[0][1] = a01 * b00 + a11 * b01 + a21 * b02;
	out[0][2] = a02 * b00 + a12 * b01 + a22 * b02;
	out[1][0] = a00 * b10 + a10 * b11 + a20 * b12;
	out[1][1] = a01 * b10 + a11 * b11 + a21 * b12;
	out[1][2] = a02 * b10 + a12 * b11 + a22 * b12;
	out[2][0] = a00 * b20 + a10 * b21 + a20 * b22;
	out[2][1] = a01 * b20 + a11 * b21 + a21 * b22;
	out[2][2] = a02 * b20 + a12 * b21 + a22 * b22;
}

void mat3_mul_vec3(mat3_ a, vec3 b, vec3 out) {
	vec3 tmp;
	tmp[0] = a[0][0] * b[0] + a[1][0] * b[1] + a[2][0] * b[2];
	tmp[1] = a[0][1] * b[0] + a[1][1] * b[1] + a[2][1] * b[2];
	tmp[2] = a[0][2] * b[0] + a[1][2] * b[1] + a[2][2] * b[2];
	glm_vec3_copy(tmp, out);
}

// todo: rewrite
void mat3_muls(mat3_ a, float s) {
	a[0][0] *= s; a[0][1] *= s; a[0][2] *= s;
	a[1][0] *= s; a[1][1] *= s; a[1][2] *= s;
	a[2][0] *= s; a[2][1] *= s; a[2][2] *= s;
}

// todo: rewrite
void mat3_transpose_to(mat3_ a, mat3_ out) {
	out[0][0] = a[0][0];
	out[0][1] = a[1][0];
	out[0][2] = a[2][0];
	out[1][0] = a[0][1];
	out[1][1] = a[1][1];
	out[1][2] = a[2][1];
	out[2][0] = a[0][2];
	out[2][1] = a[1][2];
	out[2][2] = a[2][2];
}

void mat3_transpose(mat3_ a) {
	mat3_ tmp;
	mat3_transpose_to(a, tmp);
	mat3_copy(tmp, a);
}

// todo: rewrite
float mat3_determinant(mat3_ mat) {
	float 
		a = mat[0][0], b = mat[0][1], c = mat[0][2],
		d = mat[1][0], e = mat[1][1], f = mat[1][2],
		g = mat[2][0], h = mat[2][1], i = mat[2][2];

	return a * (e * i - h * f) - d * (b * i - c * h) + g * (b * f - c * e);
}

void mat3_inv(mat3_ mat, mat3_ out) {
	float det;
	float 
		a = mat[0][0], b = mat[0][1], c = mat[0][2],
		d = mat[1][0], e = mat[1][1], f = mat[1][2],
		g = mat[2][0], h = mat[2][1], i = mat[2][2];

	out[[0][0] =   e * i - f * h;
	out[[0][1] = -(b * i - h * c);
	out[[0][2] =   b * f - e * c;
	out[[1][0] = -(d * i - g * f);
	out[[1][1] =   a * i - c * g;
	out[[1][2] = -(a * f - d * c);
	out[[2][0] =   d * h - g * e;
	out[[2][1] = -(a * h - g * b);
	out[[2][2] =   a * e - b * d;

	det = 1.0f / (a * out[[0][0] + b * out[[1][0] + c * out[[2][0]);

	mat3_muls(out, det);
}

#endif /* MATH_MAT3_H */
