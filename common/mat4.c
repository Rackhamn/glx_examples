#include "mat4.h"

void printf_mat4(mat4 * m) {
	// %g to remove trailing zeroes
	int i;
	float * p;
	
	printf("mat4: \n");
	if(m == NULL || m->data == NULL) {
		printf("\t(null)\n");
		return;
	}
	
	i = 0;
	while(i < 4) {	
		p = m->data + (i * 4);
		printf("\t%.2f, %.2f, %.2f, %.2f,\n", p[0], p[1], p[2], p[3]);
		i++;
	}
	printf("\n");
}

void mat4_identity(mat4 * m) {
	static const float _mat4_identity[16] = 
	{ 1, 0, 0, 0,
	  0, 1, 0, 0,
	  0, 0, 1, 0,
	  0, 0, 0, 1
	};
	
	memcpy(m->data, _mat4_identity, sizeof(float) * 16);
}

void mat4_perspective(mat4 * m, float rad_fov, float ratio, float near, float far) {
	float * d = m->data;
	
	// clear it to be sure (maybe not use memset)
	memset(d, 0, sizeof(float) * 16); 
	
	// perspective
	float r, l, b, t;
	t = tan(rad_fov * 0.5) * near;
	r = ratio * t;
	l = -r;
	b = -t;
	
	// frustum
	d[0] = (2 * near) / (r - l);
	d[5] = (2 * near) / (t - b);
	d[8] = (r + l) / (r - l);
	d[9] = (t + b) / (t - b);
	d[10] = -((far + near) / (far - near));
	d[11] = -1.0;
	d[14] = -((2 * far * near) / (far - near));
	d[15] = 0; // yes
}

// OpenGL uses Column Major matrices

void mat4_mul(mat4 * ma, mat4 * mb) {
	float d[16] = { 0 };
	float * a = ma->data, * b = mb->data;
	// mul each row with each column
	
	// perfect for optimization. (known order and known operands and constant known size)
	// 64 muls and 64 sums...
	// one can break it out into a mat3, right and bot edges are often 0001
	// one can also maybe cache the x axis (row) from A.
	// it can also easily be loopizized
	
	/*
	0 1 2 3
	4 5 6 7
	8 9 A B
	C D E F
	*/
	
	// row 1
	d[0] = (a[0] * b[0]) + (a[1] * b[4]) + (a[2] * b[8]) + (a[3] * b[12]);
	d[1] = (a[0] * b[1]) + (a[1] * b[5]) + (a[2] * b[9]) + (a[3] * b[13]);
	d[2] = (a[0] * b[2]) + (a[1] * b[6]) + (a[2] * b[10]) + (a[3] * b[14]);
	d[3] = (a[0] * b[3]) + (a[1] * b[7]) + (a[2] * b[11]) + (a[3] * b[15]);
	
	// row 2
	d[4] = (a[4] * b[0]) + (a[5] * b[4]) + (a[6] * b[8]) + (a[7] * b[12]);
	d[5] = (a[4] * b[1]) + (a[5] * b[5]) + (a[6] * b[9]) + (a[7] * b[13]);
	d[6] = (a[4] * b[2]) + (a[5] * b[6]) + (a[6] * b[10]) + (a[7] * b[14]);
	d[7] = (a[4] * b[3]) + (a[5] * b[7]) + (a[6] * b[11]) + (a[7] * b[15]);
	
	// row 3
	d[8] = (a[8] * b[0]) + (a[9] * b[4]) + (a[10] * b[8]) + (a[11] * b[12]);
	d[9] = (a[8] * b[1]) + (a[9] * b[5]) + (a[10] * b[9]) + (a[11] * b[13]);
	d[10] = (a[8] * b[2]) + (a[9] * b[6]) + (a[10] * b[10]) + (a[11] * b[14]);
	d[11] = (a[8] * b[3]) + (a[9] * b[7]) + (a[10] * b[11]) + (a[11] * b[15]);
	
	// row 4
	d[12] = (a[12] * b[0]) + (a[13] * b[4]) + (a[14] * b[8]) + (a[15] * b[12]);
	d[13] = (a[12] * b[1]) + (a[13] * b[5]) + (a[14] * b[9]) + (a[15] * b[13]);
	d[14] = (a[12] * b[2]) + (a[13] * b[6]) + (a[14] * b[10]) + (a[15] * b[14]);
	d[15] = (a[12] * b[3]) + (a[13] * b[7]) + (a[14] * b[11]) + (a[15] * b[15]);
	
	memcpy(a, d, sizeof(float) * 16);
}

void mat4_mulr(mat4 * m, mat4 * ma, mat4 * mb) {
	float * d = m->data;
	const float * a = ma->data, * b = mb->data;
	// mul each row with each column
	
	// perfect for optimization. (known order and known operands and constant known size)
	// 64 muls and 64 sums...
	// one can break it out into a mat3, right and bot edges are often 0001
	// one can also maybe cache the x axis (row) from A.
	// it can also easily be loopizized
	
	/*
	0 1 2 3
	4 5 6 7
	8 9 A B
	C D E F
	*/
	
	// row 1
	d[0] = (a[0] * b[0]) + (a[1] * b[4]) + (a[2] * b[8]) + (a[3] * b[12]);
	d[1] = (a[0] * b[1]) + (a[1] * b[5]) + (a[2] * b[9]) + (a[3] * b[13]);
	d[2] = (a[0] * b[2]) + (a[1] * b[6]) + (a[2] * b[10]) + (a[3] * b[14]);
	d[3] = (a[0] * b[3]) + (a[1] * b[7]) + (a[2] * b[11]) + (a[3] * b[15]);
	
	// row 2
	d[4] = (a[4] * b[0]) + (a[5] * b[4]) + (a[6] * b[8]) + (a[7] * b[12]);
	d[5] = (a[4] * b[1]) + (a[5] * b[5]) + (a[6] * b[9]) + (a[7] * b[13]);
	d[6] = (a[4] * b[2]) + (a[5] * b[6]) + (a[6] * b[10]) + (a[7] * b[14]);
	d[7] = (a[4] * b[3]) + (a[5] * b[7]) + (a[6] * b[11]) + (a[7] * b[15]);
	
	// row 3
	d[8] = (a[8] * b[0]) + (a[9] * b[4]) + (a[10] * b[8]) + (a[11] * b[12]);
	d[9] = (a[8] * b[1]) + (a[9] * b[5]) + (a[10] * b[9]) + (a[11] * b[13]);
	d[10] = (a[8] * b[2]) + (a[9] * b[6]) + (a[10] * b[10]) + (a[11] * b[14]);
	d[11] = (a[8] * b[3]) + (a[9] * b[7]) + (a[10] * b[11]) + (a[11] * b[15]);
	
	// row 4
	d[12] = (a[12] * b[0]) + (a[13] * b[4]) + (a[14] * b[8]) + (a[15] * b[12]);
	d[13] = (a[12] * b[1]) + (a[13] * b[5]) + (a[14] * b[9]) + (a[15] * b[13]);
	d[14] = (a[12] * b[2]) + (a[13] * b[6]) + (a[14] * b[10]) + (a[15] * b[14]);
	d[15] = (a[12] * b[3]) + (a[13] * b[7]) + (a[14] * b[11]) + (a[15] * b[15]);
}

#define M4SWPt(a, b) (t=(a);(a)=(b);(b)=t;)
void mat4_transpose(mat4 * m) {
	float t, * d = m->data;
	/*
	0  1  2  3 
	4  5  6  7
	8  9  10 11
	12 13 14 15
	*/
	
	static const int swp[12] = { 
		1, 4,
		2, 8,
		6, 9, 
		3, 12, 
		7, 13, 
		11, 44 
	};
	
	int i = 0;
	while(i < 12) {
	
		t = d[swp[i]];
		d[swp[i]] = d[swp[i+1]];
		d[swp[i+1]] = t;
	
		i += 2;
	}
}

void mat4_rotate_x(mat4 * m, float rad) {
	const float c = cos(rad), s = sin(rad);	
	mat4 r;

	mat4_identity(&r);
	
	r.data[5] = c;
	r.data[6] = s;
	r.data[9] = -s;
	r.data[10] = c;
	
	mat4_mul(m, &r);
}

void mat4_rotate_y(mat4 * m, float rad) {
	const float c = cos(rad), s = sin(rad);	
	mat4 r;

	mat4_identity(&r);
	
	r.data[0] = c;
	r.data[2] = -s;
	r.data[8] = s;
	r.data[10] = c;
	
	mat4_mul(m, &r);
}

void mat4_rotate_z(mat4 * m, float rad) {
	const float c = cos(rad), s = sin(rad);	
	mat4 r;

	mat4_identity(&r);
	
	r.data[0] = c;
	r.data[1] = s;
	r.data[4] = -s;
	r.data[5] = c;
	
	mat4_mul(m, &r);
}

void mat4_rotate(mat4 * m, float radx, float rady, float radz) {	
	// super ugly. assumes that you dont already know the fucking direction
	// also, the cos/sin direction is a circle, e.g: with 3 you have a sphere
	// that is a fucking vec3f with length of 1. (far easier and no extra math)
	// const float cx = cos(radx), sx = sin(radx), cy = cos(rady), sy = sin(rady), cz = cos(radz), sz = sin(radz);

	mat4 r;
	mat4_identity(&r);
	/*
	r.data[0] = cy * cx;
	r.data[1] = cy * sx;
	r.data[2] = -sy;
	r.data[4] = (sz * sy * cx) - (cz * sx);
	r.data[5] = (sz * sy * sx) + (cz * cx);
	r.data[6] = sz * cy;
	r.data[8] = (cz * sy * cx) + (sz * sx);
	r.data[9] = (cz * sy * sx) - (sz * cx);
	r.data[10] = cz * cy;
	*/
	// mat4_mul(&r, m);
	
	mat4_rotate_x(&r, radx);
	mat4_rotate_y(&r, rady);
	mat4_rotate_z(&r, radz);
	
	mat4_mul(m, &r);
}

void mat4_mvp(mat4 * m, mat4 * model, mat4 * view, mat4 * proj) {
	mat4_identity(m);
	
	mat4_mul(m, model);
	mat4_mul(m, view);
	mat4_mul(m, proj);
}

void mat4_test(void) {
	mat4 a, b, c;
	
	mat4_identity(&a);
	mat4_identity(&b);
	
	int i = 0;
	while(i < 16) {
		b.data[i] = i;
		i++;
	}
	
	float scalar = 2;
	i = 0;
	while(i<16) {
		a.data[i] *= scalar;
		i++;
	}
	
	a.data[0] = 0;
	
	mat4_mulr(&c, &a, &b);
	
	printf_mat4(&a);
	printf_mat4(&b);
	printf_mat4(&c);
}
