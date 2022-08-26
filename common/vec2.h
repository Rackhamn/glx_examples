#ifndef MATH_VEC2_H
#define MATH_VEC2_H

#include "math.h"

void vec2_zero(vec2_ out) {
	out[0] = 0;
	out[1] = 0;
}

void vec2_set(vec2_ out, float x, float y) {
	out[0] = x;
	out[1] = y;
}

void vec2_copy(vec2_ a, vec2_ out) {
	out[0] = a[0];
	out[1] = a[1];
}

void vec2_add(vec2_ a, vec2_ b, vec2_ out) {
	out[0] = a[0] + b[0];
	out[1] = a[1] + b[1];
}

void vec2_adds(vec2_ a, float s, vec2_ out) {
	out[0] = a[0] + s;
	out[1] = a[1] + s;
}

void vec2_sub(vec2_ a, vec2_ b, vec2_ out) {
	out[0] = a[0] - b[0];
	out[1] = a[1] - b[1];
}

void vec2_subs(vec2_ a, float s, vec2_ out) {
	out[0] = a[0] - s;
	out[1] = a[1] - s;
}

void vec2_div(vec2_ a, vec2_ b, vec2_ out) {
	out[0] = a[0] / b[0];
	out[1] = a[1] / b[1];
}

void vec2_divs(vec2_ a, float s, vec2_ out) {
	out[0] = a[0] / s;
	out[1] = a[1] / s;
}

void vec2_mul(vec2_ a, vec2_ b, vec2_ out) {
	out[0] = a[0] * b[0];
	out[1] = a[1] * b[1];
}

void vec2_muls(vec2_ a, float s, vec2_ out) {
	out[0] = a[0] * s;
	out[1] = a[1] * s;
}

float vec2_dot(vec2_ a, vec2_ b) {
	return (a[0] * b[0]) + (a[1] * b[1]);
}

float vec2_magnitude(vec2_ a) {
	return sqrt((a[0] * a[0]) + (a[1] * a[1]));
}

void vec2_normalize(vec2_ a) {
	float m = vec2_magnitude(a);
	
	if(m == 0.0f) {
		a[0] = 0;
		a[1] = 0;
		return;
	}
	
	// mul (1.0 / m) instead?
	a[0] /= m;
	a[1] /= m;
}

void vec2_normalize_to(vec2_ a, vec2_ out) {
	float m = vec2_magnitude(a);
	
	if(m == 0.0f) {
		out[0] = 0;
		out[1] = 0;
		return;
	}
	
	// mul (1.0 / m) instead?
	out[0] = a[0] / m;
	out[1] = a[1] / m;
}

void vec2_cross(vec2_ a, vec2_ b, vec2_ out) {
	out[0] = (a[1] * b[2]) - (b[1] * a[2]);
	out[1] = (a[2] * b[0]) - (b[2] * a[0]);
}

// returns radian
float vec2_angle(vec2_ a, vec2_ b) {
	return vec2_dot(a, b);
	/*
	vec2_ c;
	vec2_sub(a, b, c);
	return atan2(c[1], c[0]);
	*/
}

void vec2_scale(vec2_ a, float s, vec2_ out) {
	float m = vec2_magnitude(a);
	
	if(m == 0.0f) {
		out[0] = 0;
		out[1] = 0;
		return;
	}
	
	vec2_muls(a, s / m, out);
}

void vec2_print(vec2_ a) {
	printf("(%f, %f)\n", a[0], a[1]);
}

void test_v2(void) {
	vec2_ a, b, c;
	vec2s d;
	float angle;
	
	vec2_set(a, 1, 0);
	vec2_set(b, 0, 1);
	
	vec2_set(d.raw, 1, 2);
	vec2_print(d.raw);
	printf("x: %f, y: %f\n", d.x, d.y);
	
	vec2_cross(a, b, c);
	
	angle = vec2_angle(a, b);
	
	printf("angle: %f\n", angle * R2A);
	vec2_print(c);
}

#endif /* MATH_VEC2_H */
