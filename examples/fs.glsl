#version 450 core

in vec4 out_color;
in vec4 out_frag;
in vec3 out_normal;

uniform vec3 u_light_pos;

out vec4 fragcolor;

void main() {
	fragcolor = out_color;
	
	
	vec3 light_color = vec3(1, 1, 1);
	
	vec3 light_dir = normalize(u_light_pos.xyz - out_frag.xyz);
	
	float diff = dot(out_normal, light_dir);
	
	// float brightness = dot(normal, light_dir) / (length(light_dir) * length(normal));
	
	vec3 diffuse = diff * light_color;
	
	vec3 res = diffuse * vec3(.5, .5, .5); // out_color.xyz;
	
	fragcolor = vec4(res, 1.0);
	
	// fragcolor = vec4(out_color.xyz, 0.5);
}

