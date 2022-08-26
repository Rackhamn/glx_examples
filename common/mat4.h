#ifndef MATH_MAT4_H
#define MATH_MAT4_H

#include "math.h"

/* *** MATRIX IS COLUMN MAJOR LIKE IN OPENGL *** */
/* *** CAMERA AND PROJECTION IS RIGHT HANDED *** */

void mat4_zero(mat4_ a) {
	a[0][0] = 0;
	a[0][1] = 0;
	a[0][2] = 0;
	a[0][3] = 0;
	
	a[1][0] = 0;
	a[1][1] = 0;
	a[1][2] = 0;
	a[1][3] = 0;
	
	a[2][0] = 0;
	a[2][1] = 0;
	a[2][2] = 0;
	a[2][3] = 0;
	
	a[3][0] = 0;
	a[3][1] = 0;
	a[3][2] = 0;
	a[3][3] = 0;
}

void mat4_identity(mat4_ a) {
	a[0][0] = 1;
	a[0][1] = 0;
	a[0][2] = 0;
	a[0][3] = 0;
	
	a[1][0] = 0;
	a[1][1] = 1;
	a[1][2] = 0;
	a[1][3] = 0;
	
	a[2][0] = 0;
	a[2][1] = 0;
	a[2][2] = 1;
	a[2][3] = 0;
	
	a[3][0] = 0;
	a[3][1] = 0;
	a[3][2] = 0;
	a[3][3] = 1;
}

void mat4_copy(mat4_ a, mat4_ out) {
	// could use memcpy, likely slower.
	
	out[0][0] = a[0][0];
	out[0][1] = a[0][1];
	out[0][2] = a[0][2];
	out[0][3] = a[0][3];
	
	out[1][0] = a[1][0];
	out[1][1] = a[1][1];
	out[1][2] = a[1][2];
	out[1][3] = a[1][3];
	
	out[2][0] = a[2][0];
	out[2][1] = a[2][1];
	out[2][2] = a[2][2];
	out[2][3] = a[2][3];
	
	out[3][0] = a[3][0];
	out[3][1] = a[3][1];
	out[3][2] = a[3][2];
	out[3][3] = a[3][3];
}

void mat4_get_mat3(mat4_ a, mat3_ out) {
	// copy top left of mat4 into mat3
	
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

void mat4_set_mat3(mat4_ a, mat3_ b) {
	// set top left of mat4 from mat3
	
	a[0][0] = b[0][0];
	a[0][1] = b[0][1];
	a[0][2] = b[0][2];
	
	a[1][0] = b[1][0];
	a[1][1] = b[1][1];
	a[1][2] = b[1][2];
	
	a[2][0] = b[2][0];
	a[2][1] = b[2][1];
	a[2][2] = b[2][2];
}

// todo: rewrite
void mat4_mul(mat4_ a, mat4_ b, mat4_ out) {
	// general multiplication for two 4x4 matrices
	
	float 	
		// load raw data
		a00 = a[0][0], a01 = a[0][1], a02 = a[0][2], a03 = a[0][3],
		a10 = a[1][0], a11 = a[1][1], a12 = a[1][2], a13 = a[1][3],
		a20 = a[2][0], a21 = a[2][1], a22 = a[2][2], a23 = a[2][3],
		a30 = a[3][0], a31 = a[3][1], a32 = a[3][2], a33 = a[3][3],

		b00 = b[0][0], b01 = b[0][1], b02 = b[0][2], b03 = b[0][3],
		b10 = b[1][0], b11 = b[1][1], b12 = b[1][2], b13 = b[1][3],
		b20 = b[2][0], b21 = b[2][1], b22 = b[2][2], b23 = b[2][3],
		b30 = b[3][0], b31 = b[3][1], b32 = b[3][2], b33 = b[3][3];
	
	// perform the mul
	out[0][0] = (a00 * b00) + (a10 * b01) + (a20 * b02) + (a30 * b03);
	out[0][1] = (a01 * b00) + (a11 * b01) + (a21 * b02) + (a31 * b03);
	out[0][2] = (a02 * b00) + (a12 * b01) + (a22 * b02) + (a32 * b03);
	out[0][3] = (a03 * b00) + (a13 * b01) + (a23 * b02) + (a33 * b03);
	out[1][0] = (a00 * b10) + (a10 * b11) + (a20 * b12) + (a30 * b13);
	out[1][1] = (a01 * b10) + (a11 * b11) + (a21 * b12) + (a31 * b13);
	out[1][2] = (a02 * b10) + (a12 * b11) + (a22 * b12) + (a32 * b13);
	out[1][3] = (a03 * b10) + (a13 * b11) + (a23 * b12) + (a33 * b13);
	out[2][0] = (a00 * b20) + (a10 * b21) + (a20 * b22) + (a30 * b23);
	out[2][1] = (a01 * b20) + (a11 * b21) + (a21 * b22) + (a31 * b23);
	out[2][2] = (a02 * b20) + (a12 * b21) + (a22 * b22) + (a32 * b23);
	out[2][3] = (a03 * b20) + (a13 * b21) + (a23 * b22) + (a33 * b23);
	out[3][0] = (a00 * b30) + (a10 * b31) + (a20 * b32) + (a30 * b33);
	out[3][1] = (a01 * b30) + (a11 * b31) + (a21 * b32) + (a31 * b33);
	out[3][2] = (a02 * b30) + (a12 * b31) + (a22 * b32) + (a32 * b33);
	out[3][3] = (a03 * b30) + (a13 * b31) + (a23 * b32) + (a33 * b33);
}

// todo: rewrite
void mat4_mula(mat4_ a, mat4_ b, mat4_ out) {
	/* affine specific multiplication (in hopes of being slightly faster. atlest fewer ops)
	
	mat4 (affine) format:
		R R R X
		R R R Y
		R R R Z
		0 0 0 W
	*/
	float 
		a00 = a[0][0], a01 = a[0][1], a02 = a[0][2], a03 = a[0][3],
		a10 = a[1][0], a11 = a[1][1], a12 = a[1][2], a13 = a[1][3],
		a20 = a[2][0], a21 = a[2][1], a22 = a[2][2], a23 = a[2][3],
		a30 = a[3][0], a31 = a[3][1], a32 = a[3][2], a33 = a[3][3],

		b00 = b[0][0], b01 = b[0][1], b02 = b[0][2],
		b10 = b[1][0], b11 = b[1][1], b12 = b[1][2],
		b20 = b[2][0], b21 = b[2][1], b22 = b[2][2],
		b30 = b[3][0], b31 = b[3][1], b32 = b[3][2], b33 = b[3][3];

	out[0][0] = a00 * b00 + a10 * b01 + a20 * b02;
	out[0][1] = a01 * b00 + a11 * b01 + a21 * b02;
	out[0][2] = a02 * b00 + a12 * b01 + a22 * b02;
	out[0][3] = a03 * b00 + a13 * b01 + a23 * b02;

	out[1][0] = a00 * b10 + a10 * b11 + a20 * b12;
	out[1][1] = a01 * b10 + a11 * b11 + a21 * b12;
	out[1][2] = a02 * b10 + a12 * b11 + a22 * b12;
	out[1][3] = a03 * b10 + a13 * b11 + a23 * b12;

	out[2][0] = a00 * b20 + a10 * b21 + a20 * b22;
	out[2][1] = a01 * b20 + a11 * b21 + a21 * b22;
	out[2][2] = a02 * b20 + a12 * b21 + a22 * b22;
	out[2][3] = a03 * b20 + a13 * b21 + a23 * b22;

	out[3][0] = a00 * b30 + a10 * b31 + a20 * b32 + a30 * b33;
	out[3][1] = a01 * b30 + a11 * b31 + a21 * b32 + a31 * b33;
	out[3][2] = a02 * b30 + a12 * b31 + a22 * b32 + a32 * b33;
	out[3][3] = a03 * b30 + a13 * b31 + a23 * b32 + a33 * b33;
}

void mat4_mul_rot(mat4_ a, mat4_ b, mat4_ out) {
	/* affine specific multiplication (in hopes of being slightly faster. atlest fewer ops)
	
	mat4 (affine) format:
		R R R 0
		R R R 0
		R R R 0
		0 0 0 1
	*/
	
	float 
		a00 = a[0][0], a01 = a[0][1], a02 = a[0][2], a03 = a[0][3],
		a10 = a[1][0], a11 = a[1][1], a12 = a[1][2], a13 = a[1][3],
		a20 = a[2][0], a21 = a[2][1], a22 = a[2][2], a23 = a[2][3],
		a30 = a[3][0], a31 = a[3][1], a32 = a[3][2], a33 = a[3][3],

		b00 = b[0][0], b01 = b[0][1], b02 = b[0][2],
		b10 = b[1][0], b11 = b[1][1], b12 = b[1][2],
		b20 = b[2][0], b21 = b[2][1], b22 = b[2][2];

	out[0][0] = a00 * b00 + a10 * b01 + a20 * b02;
	out[0][1] = a01 * b00 + a11 * b01 + a21 * b02;
	out[0][2] = a02 * b00 + a12 * b01 + a22 * b02;
	out[0][3] = a03 * b00 + a13 * b01 + a23 * b02;

	out[1][0] = a00 * b10 + a10 * b11 + a20 * b12;
	out[1][1] = a01 * b10 + a11 * b11 + a21 * b12;
	out[1][2] = a02 * b10 + a12 * b11 + a22 * b12;
	out[1][3] = a03 * b10 + a13 * b11 + a23 * b12;

	out[2][0] = a00 * b20 + a10 * b21 + a20 * b22;
	out[2][1] = a01 * b20 + a11 * b21 + a21 * b22;
	out[2][2] = a02 * b20 + a12 * b21 + a22 * b22;
	out[2][3] = a03 * b20 + a13 * b21 + a23 * b22;

	out[3][0] = a30;
	out[3][1] = a31;
	out[3][2] = a32;
	out[3][3] = a33;
}

void mat4_mul_vec4(mat4_ a, vec4_ b, vec4_ out) {
	vec4_ tmp;
	
	tmp[0] = a[0][0] * b[0] + a[1][0] * b[1] + a[2][0] * b[2] + a[3][0] * b[3];
	tmp[1] = a[0][1] * b[0] + a[1][1] * b[1] + a[2][1] * b[2] + a[3][1] * b[3];
	tmp[2] = a[0][2] * b[0] + a[1][2] * b[1] + a[2][2] * b[2] + a[3][2] * b[3];
	tmp[3] = a[0][3] * b[0] + a[1][3] * b[1] + a[2][3] * b[2] + a[3][3] * b[3];
	
	vec4_copy(tmp, out);
}

// todo: rewrite
void mat4_muls(mat4_ a, float s) {
	a[0][0] *= s; a[0][1] *= s; a[0][2] *= s; a[0][3] *= s;
	a[1][0] *= s; a[1][1] *= s; a[1][2] *= s; a[1][3] *= s;
	a[2][0] *= s; a[2][1] *= s; a[2][2] *= s; a[2][3] *= s;
	a[3][0] *= s; a[3][1] *= s; a[3][2] *= s; a[3][3] *= s;
}

// todo: rewrite
void mat4_transpose_to(mat4_ a, mat4_ out) {
	out[0][0] = a[0][0]; 
	out[1][0] = a[0][1];
	out[0][1] = a[1][0];
	out[1][1] = a[1][1];
	out[0][2] = a[2][0];
	out[1][2] = a[2][1];
	out[0][3] = a[3][0];
	out[1][3] = a[3][1];
	out[2][0] = a[0][2];
	out[3][0] = a[0][3];
	out[2][1] = a[1][2];
	out[3][1] = a[1][3];
	out[2][2] = a[2][2];
	out[3][2] = a[2][3];
	out[2][3] = a[3][2];
	out[3][3] = a[3][3];
}

void mat4_translate(mat4_ a, vec3_ b) {
	vec4_muladds(a[0], b[0], a[3]);
	vec4_muladds(a[1], b[1], a[3]);
	vec4_muladds(a[2], b[2], a[3]);
}

void mat4_set_translate(mat4_ a, vec3_ b) {
	mat4_identity(a);
	vec3_copy(b, a[3]);
}

void mat4_transpose(mat4_ a) {
	mat4_ tmp;
	mat4_transpose_to(a, tmp);
	mat4_copy(tmp, a);
}

// todo: rewrite
float mat4_determinant(mat4_ mat) {
	float t[6];
	float 
		a = mat[0][0], b = mat[0][1], c = mat[0][2], d = mat[0][3],
		e = mat[1][0], f = mat[1][1], g = mat[1][2], h = mat[1][3],
		i = mat[2][0], j = mat[2][1], k = mat[2][2], l = mat[2][3],
		m = mat[3][0], n = mat[3][1], o = mat[3][2], p = mat[3][3];

	t[0] = k * p - o * l;
	t[1] = j * p - n * l;
	t[2] = j * o - n * k;
	t[3] = i * p - m * l;
	t[4] = i * o - m * k;
	t[5] = i * n - m * j;

	return a * (f * t[0] - g * t[1] + h * t[2])
		- b * (e * t[0] - g * t[3] + h * t[4])
		+ c * (e * t[1] - f * t[3] + h * t[5])
		- d * (e * t[2] - f * t[4] + g * t[5]);
}

void mat4_inv(mat4_ mat, mat4_ out) {
	float t[6];
	float det;
	float 
		a = mat[0][0], b = mat[0][1], c = mat[0][2], d = mat[0][3],
		e = mat[1][0], f = mat[1][1], g = mat[1][2], h = mat[1][3],
		i = mat[2][0], j = mat[2][1], k = mat[2][2], l = mat[2][3],
		m = mat[3][0], n = mat[3][1], o = mat[3][2], p = mat[3][3];

	t[0] = k * p - o * l; t[1] = j * p - n * l; t[2] = j * o - n * k;
	t[3] = i * p - m * l; t[4] = i * o - m * k; t[5] = i * n - m * j;

	out[0][0] =  f * t[0] - g * t[1] + h * t[2];
	out[1][0] =-(e * t[0] - g * t[3] + h * t[4]);
	out[2][0] =  e * t[1] - f * t[3] + h * t[5];
	out[3][0] =-(e * t[2] - f * t[4] + g * t[5]);

	out[0][1] =-(b * t[0] - c * t[1] + d * t[2]);
	out[1][1] =  a * t[0] - c * t[3] + d * t[4];
	out[2][1] =-(a * t[1] - b * t[3] + d * t[5]);
	out[3][1] =  a * t[2] - b * t[4] + c * t[5];

	t[0] = g * p - o * h; t[1] = f * p - n * h; t[2] = f * o - n * g;
	t[3] = e * p - m * h; t[4] = e * o - m * g; t[5] = e * n - m * f;

	out[0][2] =  b * t[0] - c * t[1] + d * t[2];
	out[1][2] =-(a * t[0] - c * t[3] + d * t[4]);
	out[2][2] =  a * t[1] - b * t[3] + d * t[5];
	out[3][2] =-(a * t[2] - b * t[4] + c * t[5]);

	t[0] = g * l - k * h; t[1] = f * l - j * h; t[2] = f * k - j * g;
	t[3] = e * l - i * h; t[4] = e * k - i * g; t[5] = e * j - i * f;

	out[0][3] =-(b * t[0] - c * t[1] + d * t[2]);
	out[1][3] =  a * t[0] - c * t[3] + d * t[4];
	out[2][3] =-(a * t[1] - b * t[3] + d * t[5]);
	out[3][3] =  a * t[2] - b * t[4] + c * t[5];

	det = 1.0f / (a * out[0][0] + b * out[1][0]
		+ c * out[2][0] + d * out[3][0]);

	mat4_muls(out, det);
}

void mat4_scale_vec3(mat4_ a, vec3_ b) {
	mat4_ tmp;
	mat4_identity(tmp);
	
	tmp[0][0] = b[0];
	tmp[1][1] = b[1];
	tmp[2][2] = b[2];
	
	mat4_mul(a, tmp, a);
}

void mat4_scale(mat4_ a, float s) {
	mat4_muls(a, s);
}

void mat4_rotx(mat4_ a, float radian, mat4_ out) {
	mat4_ tmp;
	float _cos, _sin;
	mat4_identity(tmp);
	
	_cos = cos(radian);
	_sin = sin(radian);
	
	tmp[1][1] = _cos;
	tmp[1][2] = _sin;
	tmp[2][1] = -_sin;
	tmp[2][2] = _cos;
	
	mat4_mul_rot(a, tmp, out);
}

void mat4_roty(mat4_ a, float radian, mat4_ out) {
	mat4_ tmp;
	float _cos, _sin;
	mat4_identity(tmp);
	
	_cos = cos(radian);
	_sin = sin(radian);
	
	tmp[0][0] = _cos;
	tmp[0][2] = -_sin;
	tmp[2][0] = _sin;
	tmp[2][2] = _cos;
	
	mat4_mul_rot(a, tmp, out);
}

void mat4_rotz(mat4_ a, float radian, mat4_ out) {
	mat4_ tmp;
	float _cos, _sin;
	mat4_identity(tmp);
	
	_cos = cos(radian);
	_sin = sin(radian);
	
	tmp[0][0] = _cos;
	tmp[0][1] = _sin;
	tmp[1][0] = -_sin;
	tmp[1][1] = _cos;
	
	mat4_mul_rot(a, tmp, out);
}

// todo: rewrite
void mat4_setrot(mat4_ a, float radian, vec3_ axis) {
	vec3_ axisn, v, vs;
	float c;

	c = cos(radian);

	vec3_normalize_to(axis, axisn);
	vec3_scale(axisn, 1.0f - c, v);
	vec3_scale(axisn, sinf(radian), vs);

	vec3_scale(axisn, v[0], a[0]);
	vec3_scale(axisn, v[1], a[1]);
	vec3_scale(axisn, v[2], a[2]);

	a[0][0] += c;       a[1][0] -= vs[2];   a[2][0] += vs[1];
	a[0][1] += vs[2];   a[1][1] += c;       a[2][1] -= vs[0];
	a[0][2] -= vs[1];   a[1][2] += vs[0];   a[2][2] += c;

	a[0][3] = a[1][3] = a[2][3] = a[3][0] = a[3][1] = a[3][2] = 0.0f;
	a[3][3] = 1.0f;
}

void mat4_rot(mat4_ a, float radian, vec3_ axis) {
	mat4_ rot;
	mat4_setrot(rot, radian, axis);
	mat4_mul_rot(a, rot, a);
}

void mat4_pivot(mat4_ a, vec3_ pivot, float radian, vec3_ axis) {
	vec3_ pivot_inv;
	vec3_inv_to(pivot, pivot_inv);
	mat4_set_translate(a, pivot);
	mat4_rot(a, radian, axis);
	mat4_translate(a, pivot_inv);
}

void mat4_perspective(float fov, float aspect_ratio, float nearz, float farz, mat4_ out) {
	float f, fn;

	mat4_zero(out);

	f  = 1.0f / tanf(fov * 0.5f);
	fn = 1.0f / (nearz - farz);

	out[0][0] = f / aspect_ratio;
	out[1][1] = f;
	out[2][2] = (nearz + farz) * fn;
	out[2][3] =-1.0f;
	out[3][2] = 2.0f * nearz * farz * fn;
}

void mat4_lookat(vec3_ eye, vec3_ dir, vec3_ up, mat4_ out) {
	vec3_ f, u, s, center;
	
	vec3_add(eye, dir, center);
	
	vec3_sub(center, eye, f);
	vec3_normalize(f);

	vec3_cross(f, up, s);
	vec3_normalize(s);
	
	vec3_cross(s, f, u);

	out[0][0] = s[0];
	out[0][1] = u[0];
	out[0][2] =-f[0];
	out[1][0] = s[1];
	out[1][1] = u[1];
	out[1][2] =-f[1];
	out[2][0] = s[2];
	out[2][1] = u[2];
	out[2][2] =-f[2];
	out[3][0] =-vec3_dot(s, eye);
	out[3][1] =-vec3_dot(u, eye);
	out[3][2] = vec3_dot(f, eye);
	out[0][3] = out[1][3] = out[2][3] = 0.0f;
	out[3][3] = 1.0f;
}

#endif /* MATH_MAT4_H */
