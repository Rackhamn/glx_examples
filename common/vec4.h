#ifndef MATH_VEC4_H
#define MATH_VEC4_H

#include "math.h"

void vec4_zero(vec4_ out) {
	out[0] = 0;
	out[1] = 0;
	out[2] = 0;
	out[3] = 0;
}

void vec4_copy(vec4_ a, vec4_ out) {
	out[0] = a[0];
	out[1] = a[1];
	out[2] = a[2];
	out[3] = a[3];
}

void vec4_set(vec4_ out, float x, float y, float z, float w) {
	out[0] = x;
	out[1] = y;
	out[2] = z;
	out[3] = 2;
}

void vec4_set_vec3s(vec4_ out, vec3_ a,  float s) {
	out[0] = a[0];
	out[1] = a[1];
	out[2] = a[2];
	out[3] = s;
}

void vec4_add(vec4_ a, vec4_ b, vec4_ out) {
	out[0] = a[0] + b[0];
	out[1] = a[1] + b[1];
	out[2] = a[2] + b[2];
	out[3] = a[3] + b[3];
}

void vec4_adds(vec4_ a, float s, vec4_ out) {
	out[0] = a[0] + s;
	out[1] = a[1] + s;
	out[2] = a[2] + s;
	out[3] = a[3] + s;
}

void vec4_sub(vec4_ a, vec4_ b, vec4_ out) {
	out[0] = a[0] - b[0];
	out[1] = a[1] - b[1];
	out[2] = a[2] - b[2];
	out[3] = a[3] - b[3];
}

void vec4_subs(vec4_ a, float s, vec4_ out) {
	out[0] = a[0] - s;
	out[1] = a[1] - s;
	out[2] = a[2] - s;
	out[3] = a[3] - s;
}

void vec4_div(vec4_ a, vec4_ b, vec4_ out) {
	out[0] = a[0] / b[0];
	out[1] = a[1] / b[1];
	out[2] = a[2] / b[2];
	out[3] = a[3] / b[3];
}

void vec4_divs(vec4_ a, float s, vec4_ out) {
	out[0] = a[0] / s;
	out[1] = a[1] / s;
	out[2] = a[2] / s;
	out[3] = a[3] / s;
}

void vec4_mul(vec4_ a, vec4_ b, vec4_ out) {
	out[0] = a[0] * b[0];
	out[1] = a[1] * b[1];
	out[2] = a[2] * b[2];
	out[3] = a[3] * b[3];
}

void vec4_muls(vec4_ a, float s, vec4_ out) {
	out[0] = a[0] * s;
	out[1] = a[1] * s;
	out[2] = a[2] * s;
	out[3] = a[3] * s;
}

void vec4_muladd(vec4_ a, vec4_ b, vec4_ out) {
	out[0] += a[0] * b[0];
	out[1] += a[1] * b[1];
	out[2] += a[2] * b[2];
	out[3] += a[3] * b[3];
} 

void vec4_muladds(vec4_ a, float s, vec4_ out) {
	out[0] += a[0] * s;
	out[1] += a[1] * s;
	out[2] += a[2] * s;
	out[3] += a[3] * s;
} 

float vec4_dot(vec4_ a, vec4_ b) {
	return (a[0] * b[0]) + (a[1] * b[1]) + (a[2] * b[2]) + (a[3] * b[3]);
}

float vec4_magnitude(vec4_ a) {
	// return sqrt(vec4_dot(a, a)); // same thing
	return sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2] + a[3] * a[3]);
}

void vec4_normalize(vec4_ a) {
	float m = vec4_magnitude(a);
	
	if(m == 0.0f) {
		a[0] = 0;
		a[1] = 0;
		a[2] = 0;
		a[3] = 0;
		return;
	}
	
	// mul (1.0 / m) instead?
	a[0] /= m;
	a[1] /= m;
	a[2] /= m;
	a[3] /= m; // set to 0?
}

void vec4_normalize_to(vec4_ a, vec4_ out) {
	float m = vec4_magnitude(a);
	
	if(m == 0.0f) {
		out[0] = 0;
		out[1] = 0;
		out[2] = 0;
		out[3] = 0;
		return;
	}
	
	// mul (1.0 / m) instead?
	out[0] = a[0] / m;
	out[1] = a[1] / m;
	out[2] = a[2] / m;
	out[3] = a[3] / m; // set to 0?
}

void vec4_cross(vec4_ a, vec4_ b, vec4_ out) {
	out[0] = (a[1] * b[2]) - (b[1] * a[2]);
	out[1] = (a[2] * b[0]) - (b[2] * a[0]);
	out[2] = (a[0] * b[1]) - (b[0] * a[1]);
	out[3] = 0;
}

// returns radian
float vec4_angle(vec4_ a, vec4_ b) {
	return 0;
}

void vec4_scale(vec4_ a, float s, vec4_ out) {
	float m = vec4_magnitude(a);
	
	if(m == 0.0f) {
		out[0] = 0;
		out[1] = 0;
		out[2] = 0;
		out[3] = 0;
		return;
	}
	
	vec4_muls(a, s / m, out);
}

void vec4_print(vec4_ a) {
	printf("(%f, %f, %f, %f)\n", a[0], a[1], a[2], a[3]);
}

void test_v4(void) {
	vec4_ a, b, c;
	vec4s d;
	float angle;
	
	vec4_set(a, 1, 0, 0, 0);
	vec4_set(b, 0, 1, 0, 0);
	
	vec4_set(d.raw, 1, 2, 3, 4);
	vec4_print(d.raw);
	printf("x: %f, y: %f, z: %f, w: %f\n", d.x, d.y, d.z, d.w);
	
	vec4_cross(a, b, c);
	
	angle = vec4_angle(a, b);
	
	printf("angle: %f\n", angle * R2A);
	vec4_print(c);
}

#endif /* MATH_VEC4_H */
