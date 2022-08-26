#ifndef MATH_VEC3_H
#define MATH_VEC3_H

#include "math.h"

void vec3_zero(vec3_ out) {
	out[0] = 0;
	out[1] = 0;
	out[2] = 0;
}

void vec3_copy(vec3_ a, vec3_ out) {
	out[0] = a[0];
	out[1] = a[1];
	out[2] = a[2];
}

void vec3_inv(vec3_ a) {
	a[0] = -a[0];
	a[1] = -a[1];
	a[2] = -a[2];
}

void vec3_inv_to(vec3_ a, vec3_ out) {
	out[0] = -a[0];
	out[1] = -a[1];
	out[2] = -a[2];
}

void vec3_set(vec3_ out, float x, float y, float z) {
	out[0] = x;
	out[1] = y;
	out[2] = z;
}

void vec3_set_vec2s(vec3_ out, vec2_ a,  float s) {
	out[0] = a[0];
	out[1] = a[1];
	out[2] = s;
}

void vec3_set_svec2(vec3_ out, float s, vec2_ a) {
	out[0] = s;
	out[1] = a[0];
	out[2] = a[1];
}

void vec3_add(vec3_ a, vec3_ b, vec3_ out) {
	out[0] = a[0] + b[0];
	out[1] = a[1] + b[1];
	out[2] = a[2] + b[2];
}

void vec3_adds(vec3_ a, float s, vec3_ out) {
	out[0] = a[0] + s;
	out[1] = a[1] + s;
	out[2] = a[2] + s;
}

/*
void vec3_addn(vec3_ * a, int n, vec3_ out) {
	int i = 0;
	
	while(i < n) {
		out[0] += a[i][0];
		out[1] += a[i][1];
		out[2] += a[i][2];
		i++;
	}
}
*/

void vec3_sub(vec3_ a, vec3_ b, vec3_ out) {
	out[0] = a[0] - b[0];
	out[1] = a[1] - b[1];
	out[2] = a[2] - b[2];
}

void vec3_subs(vec3_ a, float s, vec3_ out) {
	out[0] = a[0] - s;
	out[1] = a[1] - s;
	out[2] = a[2] - s;
}

void vec3_div(vec3_ a, vec3_ b, vec3_ out) {
	out[0] = a[0] / b[0];
	out[1] = a[1] / b[1];
	out[2] = a[2] / b[2];
}

void vec3_divs(vec3_ a, float s, vec3_ out) {
	out[0] = a[0] / s;
	out[1] = a[1] / s;
	out[2] = a[2] / s;
}

void vec3_mul(vec3_ a, vec3_ b, vec3_ out) {
	out[0] = a[0] * b[0];
	out[1] = a[1] * b[1];
	out[2] = a[2] * b[2];
}

void vec3_muls(vec3_ a, float s, vec3_ out) {
	out[0] = a[0] * s;
	out[1] = a[1] * s;
	out[2] = a[2] * s;
}

float vec3_dot(vec3_ a, vec3_ b) {
	return (a[0] * b[0]) + (a[1] * b[1]) + (a[2] * b[2]);
}

float vec3_magnitude(vec3_ a) {
	// return sqrt(vec3_dot(a, a)); // same thing
	return sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
}

void vec3_normalize(vec3_ a) {
	float m = vec3_magnitude(a);
	
	if(m == 0.0f) {
		a[0] = 0;
		a[1] = 0;
		a[2] = 0;
		return;
	}
	
	// mul (1.0 / m) instead?
	a[0] /= m;
	a[1] /= m;
	a[2] /= m;
}

void vec3_normalize_to(vec3_ a, vec3_ out) {
	float m = vec3_magnitude(a);
	
	if(m == 0.0f) {
		out[0] = 0;
		out[1] = 0;
		out[2] = 0;
		return;
	}
	
	// mul (1.0 / m) instead?
	out[0] = a[0] / m;
	out[1] = a[1] / m;
	out[2] = a[2] / m;
}

void vec3_cross(vec3_ a, vec3_ b, vec3_ out) {
	out[0] = (a[1] * b[2]) - (b[1] * a[2]);
	out[1] = (a[2] * b[0]) - (b[2] * a[0]);
	out[2] = (a[0] * b[1]) - (b[0] * a[1]);
}

// returns radian
float vec3_angle(vec3_ a, vec3_ b) {
	float m, d;
	
	m = 1.0f / (vec3_magnitude(a) * vec3_magnitude(b));
	d = vec3_dot(a, b) * m;
	
	if(d > 1.0f) {
		return 0.0f;
	} else if(d < -1.0f) {
		return 3.1459; // PI
	}
	
	return acosf(d);
}

void vec3_scale(vec3_ a, float s, vec3_ out) {
	float m = vec3_magnitude(a);
	
	if(m == 0.0f) {
		out[0] = 0;
		out[1] = 0;
		out[2] = 0;
		return;
	}
	
	vec3_muls(a, s / m, out);
}

void vec3_print(vec3_ a) {
	printf("(%f, %f, %f)\n", a[0], a[1], a[2]);
}

void test_v3(void) {
	vec3_ a, b, c;
	vec3s d;
	float angle;
	
	vec3_set(a, 1, 0, 0);
	vec3_set(b, 0, 1, 0);
	
	vec3_set(d.raw, 1, 2, 3);
	vec3_print(d.raw);
	printf("x: %f, y: %f, z: %f\n", d.x, d.y, d.z);
	
	vec3_cross(a, b, c);
	
	angle = vec3_angle(a, b);
	
	printf("angle: %f\n", angle * R2A);
	vec3_print(c);
}

#endif /* MATH_VEC3_H */
