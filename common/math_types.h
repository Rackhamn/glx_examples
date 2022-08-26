#ifndef MATH_TYPES_H
#define MATH_TYPES_H

// todo: add inline helper.
// todo: add named indexes.
// todo: swizzling? i dont think i want it. never used it in c+++ (except for maybe in the fx shaders)
// todo: compile switch for lowp_* and highp_* float
typedef float 	vec2_[2];
typedef float 	vec3_[3];
typedef float 	vec4_[4];
typedef vec4_	quat_; 	// quaternion, q4 ...
typedef vec2_	mat2_[2];
typedef vec3_	mat3_[3];
typedef vec4_	mat4_[4];

typedef union vec2s {
	vec2_ raw; // badname
	struct { float x, y; };	// euler
	struct { float r, i; };	// imaginary
	struct { float u, v; };	// texture
	struct { float s, t; };	// time
} vec2s;

typedef union vec3s {
	vec3_ raw; // badname
	struct { float x, y, z; };
	struct { float r, g, b; };
} vec3s;

typedef union vec4s {
	vec4_ raw; // badname
	struct { float x, y, z, w; };
	struct { float r, g, b, a; };
} vec4s;

void print_math_types(void) {
	printf("*** math types sizes ***\n");
	
	printf("raw:\n");
	printf("float : %lu bytes\n", sizeof(float));
	printf("vec2_ : %lu bytes\n", sizeof(vec2_));
	printf("vec3_ : %lu bytes\n", sizeof(vec3_));
	printf("vec4_ : %lu bytes\n", sizeof(vec4_));
	printf("mat2_ : %lu bytes\n", sizeof(mat2_));
	printf("mat3_ : %lu bytes\n", sizeof(mat3_));
	printf("mat4_ : %lu bytes\n", sizeof(mat4_));
	
	printf("struct:\n");
	printf("vec2s : %lu bytes\n", sizeof(vec2s));
	printf("vec3s : %lu bytes\n", sizeof(vec3s));
	printf("vec4s : %lu bytes\n", sizeof(vec4s));
	// printf("mat2s_ : %lu bytes\n", sizeof(mat2_));
	// printf("mat3s_ : %lu bytes\n", sizeof(mat3_));
	// printf("mat4s_ : %lu bytes\n", sizeof(mat4_));
}

#endif /* MATH_TYPES_H */
