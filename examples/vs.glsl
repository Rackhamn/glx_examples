#version 450 core

in vec3 in_vertex;
in vec3 in_normal;
in vec3 in_color;

uniform vec3 u_light_pos;
uniform mat4 u_model_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_proj_matrix;

out vec4 out_color;
out vec4 out_frag;
out vec3 out_normal;

void main() {
	gl_Position = u_proj_matrix * u_view_matrix * u_model_matrix * vec4(in_vertex, 1);
	
	out_frag = u_model_matrix * vec4(in_vertex, 1);
	
	out_color = vec4(in_color, 1);
	
	mat3 nm = transpose(inverse(mat3(u_model_matrix)));
	out_normal = normalize(nm * in_normal);
}
