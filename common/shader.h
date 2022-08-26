#ifndef GLSHADER_H
#define GLSHADER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "vec3.h"

/*
	gl_FragColor = out_color * vec4(1, 0, 0, 1);
	vec3 light_color = vec3(1, 1, 1);
	vec3 light_dir = normalize(u_light_pos.xyz - out_frag.xyz);
	float diff = dot(out_normal, light_dir);
	vec3 diffuse = diff * light_color;
	vec3 res = diffuse * out_color.xyz;
	gl_FragColor = vec4(res, 1.0);
	*/

int load_file(char * path, char ** data, unsigned int * bytes) {
	FILE * f;
	char * raw;
	int size;
	
	if(path == NULL) {
		printf("bad path\n");
		*data = NULL;
		return -1;
	}
	
	f = fopen(path, "r");
	
	if(f == NULL) {
		printf("bad file pointer\n");
		*data = NULL;
		return -1;
	}
	
	// get the size of file
	fseek(f, 0L, SEEK_END);
	size = ftell(f);
	fseek(f, 0L, SEEK_SET);
	
	if(size <= 0) { 
		printf("size <= 0\n");
		return -1;
	}
	
	printf("alloc %i bytes of data\n", size + 1);
	raw = malloc(sizeof(char) * (size + 1));
	if(raw == NULL) { 
		printf("error: bad malloc\n");
		return -1; 
	}
	
	fread(raw, 1, size, f);
	raw[size] = '\0';
	
	*data = raw;
	*bytes = size;
	
	// printf("read [%s] at %i bytes\n", path, *bytes);
	
	fclose(f);
	
	return 0;
}

int load_shader(char * vertex_shader_path, char * fragment_shader_path, GLuint * sh) {
	// load & compile vertex shader
	char * vs_buf = NULL, * fs_buf = NULL;
	unsigned int vs_bytes = 0, fs_bytes = 0;
	GLuint vs, fs, sp;
	
	if(load_file(vertex_shader_path, &vs_buf, &vs_bytes)!=0) {
		printf("bad vertex shader file [%s]\n", vertex_shader_path); 
		return -1;
	}
	if(load_file(fragment_shader_path, &fs_buf, &fs_bytes)!=0) {
		printf("bad fragment shader file [%s]\n", vertex_shader_path); 
		return -1;
	}
	
	if(vs_buf == NULL || vs_bytes <= 0) { 	
		printf("bad vertex shader file [%s]\n", vertex_shader_path); 
		
		if(vs_buf == NULL)
			printf("no buffer allocated\n");
			
		if(vs_bytes <= 0)
			printf("bytes <= 0\n");
	
		if(vs_buf != NULL) free(vs_buf);
		if(fs_buf != NULL) free(fs_buf);
		*sh = 0;
		return -1;
	}
	
	if(fs_buf == NULL || fs_bytes <= 0) { 
		printf("bad fragment shader file [%s]\n", fragment_shader_path); 
		
		if(vs_buf == NULL)
			printf("no buffer allocated\n");
			
		if(vs_bytes <= 0)
			printf("bytes <= 0\n");
		
		if(vs_buf != NULL) free(vs_buf);
		if(fs_buf != NULL) free(fs_buf);
		*sh = 0;
		return -1;
	}
	
	// ok. attempt to compile the shader.
	GLint is_compiled, is_linked, max_length;
	
	is_compiled = 0;
	
	// (const GLchar * const *)
	
	// printf("attempt compile vertex shader\n");
	vs = glCreateShader(GL_VERTEX_SHADER);
	
	// todo: GCC discard warning
	const GLchar * gl_vs_buf = (GLchar *)vs_buf;
	glShaderSource(vs, 1, &gl_vs_buf, NULL); // (const GLchar * const *)
	glCompileShader(vs);

	glGetShaderiv(vs, GL_COMPILE_STATUS, &is_compiled);
	if(is_compiled == GL_FALSE) {
		max_length = 0;
		
		glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &max_length);
		
		char* error = malloc(max_length);
		glGetShaderInfoLog(vs, max_length, &max_length, error);
		printf("%s\n", error);
		free(error);
		
		if(vs_buf != NULL) free(vs_buf);
		if(fs_buf != NULL) free(fs_buf);
		*sh = 0;
		
		return -1;
	}
	
	// compile fragment shader
	// printf("attempt compile fragment shader\n");
	is_compiled = 0;
	fs = glCreateShader(GL_FRAGMENT_SHADER);
	
	const GLchar * gl_fs_buf = (GLchar *)fs_buf;
	glShaderSource(fs, 1, &gl_fs_buf, NULL); // (const GLchar * const*)
	glCompileShader(fs);

	glGetShaderiv(fs, GL_COMPILE_STATUS, &is_compiled);
	if(is_compiled == GL_FALSE) {
		max_length = 0;
		
		glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &max_length);
		
		char* error = malloc(max_length);
		glGetShaderInfoLog(fs, max_length, &max_length, error);
		printf("%s\n", error);
		free(error);
		
		if(vs_buf != NULL) free(vs_buf);
		if(fs_buf != NULL) free(fs_buf);
		*sh = 0;
		
		return -2;
	}
	
	// printf("attempt link shader\n");
	
	// link shader
	sp = glCreateProgram();
	glAttachShader(sp, vs);
	glAttachShader(sp, fs);
	glLinkProgram(sp);

	is_linked = 0;
	glGetProgramiv(sp, GL_LINK_STATUS, (int *)&is_linked);
	if(is_linked == GL_FALSE) {
		max_length = 0;
		glGetProgramiv(sp, GL_INFO_LOG_LENGTH, &max_length);

		char* error = malloc(max_length);
		glGetProgramInfoLog(sp, max_length, &max_length,error);
		printf("%s\n", error);
		free(error);
		
		if(vs_buf != NULL) free(vs_buf);
		if(fs_buf != NULL) free(fs_buf);
		*sh = 0;
		
		return -3;
	}
	
	// printf("Shader program compiled successfully.\n");
	
	// maybe not delete em yet? -> other shaders might use one or the other.
	glDeleteShader(vs);
	glDeleteShader(fs);
	
	if(vs_buf != NULL) free(vs_buf);
	if(fs_buf != NULL) free(fs_buf);
	
	*sh = sp;
	
	return 0;
}

void free_shader(GLuint sh) {
	glDeleteProgram(sh);
}










struct modeldata {
	int vertices_count, normals_count, elements_count;
	// vec3_ * vertices, * normals;
	int * elements;
};

int get_next_char_or_newline_index(char * ptr, int count) {
	int i;
	char c;
	
	i = 0;
	while(i < count) {
		c = ptr[i];
		if(c != ' ' && c != '\t') return i;
		i++;
	}
	
	return -1;
}

int get_next_split_index(char * ptr, int count, char split) {
	int i;
	char c;
	
	i = 0;
	while(i < count) {
		if(ptr[i] == split) return i;
		i++;
	}
	
	return -1;
}

int get_next_nonsplit_index(char * ptr, int count, char split) {
	int i;
	char c;
	
	i = 0;
	while(i < count) {
		if(ptr[i] != split) return i;
		i++;
	}
	
	return -1;
}

// same memory layout:
// in = 'hello my name\nis arabic coffee'
// out[7] = { 'hello', 'my', 'name', '\n', 'is', 'arabic', 'coffee' }

struct stringdata {
	int count;
	int * lengths;
	char ** words;
};

// recursive would be easier.
struct stringdata split_cstring(char * in, int bytes, char split) {
	int word_count = 0;
	char ** words; 
	int * lengths;
	
	// char c, split = ' ';
	int i = 0, j = 0;
	
	// printf("bytes: %i\n", bytes);
	
	word_count = 0;
	words = malloc(sizeof(char **) * 1);
	lengths = malloc(sizeof(int) * 1);
	
	while(i < bytes) {
		int nsi = get_next_split_index(in + i, bytes - i, split);
		int nnsi = get_next_nonsplit_index(in + i, bytes - i, split);
		
		// does not handle \n or \0 -> special case for end of string
		if(nnsi == -1) {
			// there is nothing to be done. - no more possible words
			break;
		}
		if(nsi == -1) {
			// split at the end of the range if nothing found. '\0 case'
			nsi = bytes - i; 
		}
		
		// correct the range
		nsi += i;
		nnsi += i;
		
		// there is a likely word within range
		if(nnsi <= nsi) {
			int wordlen = nsi - i;
			
			// zero length words are not needed right?
			if(wordlen > 0) {
				words = realloc(words, sizeof(char *) * (word_count + 1));
				lengths = realloc(lengths, sizeof(int) * (word_count + 1));
				words[word_count] = malloc(sizeof(char) * wordlen + 1);
				
				j = 0;
				while(j < wordlen) {
					words[word_count][j] = *(in + i + j);
					j++;
				}
				words[word_count][j] = '\0';
				lengths[word_count] = wordlen;
			
				word_count++;
			}
			
			i = nsi - 1;
		}
		else {
		//if(nnsi > nsi) {
		// there is only space in the range
			i += (nnsi - nsi) - 1;
		}
		
	split_cont:
		i++;
	}
	
	#if 0
	printf("words [%i]: \n", word_count);

	i = 0;
	while(i < word_count) {
		int len = strlen(words[i]);
		
		printf("%i : '", i);
		j = 0;
		while(j < len) {
			if(words[i][j] == '\t') {
				printf("\\t");
			}
			else if(words[i][j] == '\n') {
				printf("\\n");
			} else {
				printf("%c", words[i][j]);
			}
			j++;
		}
	
		printf("'\n");
	
		i++;
	}
	#endif
	
	struct stringdata out;
	out.words = words;
	out.count = word_count;
	out.lengths = lengths;
	return out; 
}

void free_stringdata(struct stringdata sd) {
	int i = 0;
	while(i < sd.count) {
		if(sd.words[i] != NULL)
			free(sd.words[i]);
		i++;
	}
	
	if(sd.lengths != NULL) 
		free(sd.lengths);
	
	if(sd.words != NULL)
		free(sd.words);
}

void splittest(void) {
	struct stringdata d, m;
	
	char * string;
	int string_len = 0;
	
	int line_count = 0, comment_count = 0, vertex_count = 0, normal_count = 0, face_count = 0;
	
	if(load_file("../models/rand.obj", &string, &string_len)) {
		printf("error\n");
	}
	
	// char * string = "Hello my name is arabic coffee.\n I like music!";
	// d = split_cstring(string, strlen(string));
	d = split_cstring(string, string_len, '\n');
	
	/*
	vec3_ * vertices;
	vec3_ * normals;
	int * elements;
	*/
	
	int i = 0;
	while(i < d.count) {
	
		line_count++;
		
		m = split_cstring(d.words[i], strlen(d.words[i]), ' ');
		
		if(m.count > 0) {
			if(m.words[0][0] == 'v') { 
				if(m.words[0][1] == 'n') normal_count++;
				else vertex_count++; 
			}
			if(m.words[0][0] == '#') { comment_count++; }
			if(m.words[0][0] == 'f') { face_count++; }
		}
		
		free_stringdata(m);
		
		i++;
	}
	
	/*
	# vertices
	# X   Y    Z
	v 1.0 -1.0 1.0
	v 0.0 -1.0 1.0
	v -1.0 0.0 1.0
	
	# normals
	n 0.0 0.0 -1.0
	n 0.0 0.0 -1.0
	n 0.0 0.0 -1.0
	*/
	
	
	printf("obj:\n");
	printf("lines = %i\n", line_count);
	printf("comments = %i\n", comment_count);
	printf("vertices = %i\n", vertex_count);
	printf("normals = %i\n", normal_count);
	printf("faces = %i\n", face_count);
	
	free_stringdata(d);
}

void obj_parse_stringdata_vec3(struct stringdata d, vec3_ ** buffer, int * buf_count) {
	int index = *buf_count;
	float x, y, z;
	
	if(*buffer == NULL) return;
	
	// allow 3D coordinates only
	if(d.count == 4) {
		// attempt parse
		x = atof(d.words[1]);
		y = atof(d.words[2]);
		z = atof(d.words[3]);
		
		// make space
		*buf_count = index + 1;
		*buffer = realloc(*buffer, sizeof(vec3_) * (*buf_count));
		
		// push
		(*buffer)[index][0] = x;
		(*buffer)[index][1] = y;
		(*buffer)[index][2] = z;
	}
}

void obj_parse_stringdata_face(struct stringdata d, vec3_ ** buffer, int * buf_count, 
													int ** lengths, int * len_count) {
	int index = *buf_count;
	int vertex_id, uv_id, normal_id;
	char split = '/';
	
	int i = 1;
	while(i < d.count) {
		
		// todo: special check for number of faces in a row. --> (quad or tri)
		// * there is a correctness check in there as well. (a model is supposed to have a 
		// consistant face set.
		// * it is helpful to know if we are to convert it into triangles only.
		// then we can build an element list.
		
		// subparse 'word' with matching indexes
		int subindex = 0;
		int val, v, vt, vn;
		int l, r, max; // walker
		
		l = 0;
		r = 0;
		max = d.lengths[i];
		while(r <= max) {
			
			if(d.words[i][r] == '/' || d.words[i][r] == '\0' || d.words[i][r] == '\n') {
				// read left-right range
				val = atoi(d.words[i] + l);
				
				// .OBJ is 1 based indexing. (not 0)
				// if val is -n then it is referencing 'n' steps backwards in the index list.
				// right now we just read the value and write it into our buffer.	
				if(subindex == 0) { v = val; }
				if(subindex == 1) { vt = val; }
				if(subindex == 2) { vn = val; }
				
				l = r + 1;
				subindex++;
			}
		
			r++;
		}
		
		/*
		printf("in: %s\n", d.words[i]);
		printf("out: f %i/%i/%i\n", v, vt, vn);
		*/
		
		*buf_count = index + 1;
		*buffer = realloc(*buffer, sizeof(vec3_) * (*buf_count));
		
		// push
		(*buffer)[index][0] = v;
		(*buffer)[index][1] = vt;
		(*buffer)[index][2] = vn;
		
		index++;
		
		i++;
	}
	
	// face set
	*len_count += 1;
	*lengths = realloc(*lengths, sizeof(int) * (*len_count));
	(*lengths)[(*len_count) - 1] = d.count - 1;
}

int wrap_rangei(int min, int max, int val) {

/*
	min .. max
	0	.. 100
*/

	return (val - (max - min) * floor(val / (max - min)));
}

int load_model_obj(char * path, struct mesh_data * md) {
	
	int file_bytes;
	char * file_data;
	
	struct stringdata data, line;
	
	if(path == NULL || md == NULL) return -1;
	if(load_file(path, &file_data, &file_bytes)) {
		if(file_data != NULL) free(file_data);
		return -1;
	}
	
	// split 'file_data' per line
	data = split_cstring(file_data, file_bytes, '\n');
	
	
	int vertex_count = 0;
	vec3_ * vertices = malloc(sizeof(vec3_) * 1);
	
	int normal_count = 0;
	vec3_ * normals = malloc(sizeof(vec3_) * 1);
	
	int obj_first_face_split_type = 0; // todo: bad name
	int face_count = 0;
	vec3_ * faces = malloc(sizeof(vec3_) * 1); // int3
	
	int face_len_count = 0;
	int * faces_length = malloc(sizeof(int) * 1);
	
	float x, y, z;
	
	int i = 0;
	while(i < data.count) {	
		// parse per line.
		// split each 'line' into words
		line = split_cstring(data.words[i], data.lengths[i], ' ');
		
		// line is a comment. dont parse it and just move on.
		if(line.words[0][0] == '#') goto sslr;
		
		if(line.words[0][0] == 'v') {
			// v, vt, vn
			if(line.words[0][1] == 'n') {
				obj_parse_stringdata_vec3(line, &normals, &normal_count);
			} 
			else if(line.words[0][1] == 't') {
				// ...
			}
			else {
				// regular vertex line
				obj_parse_stringdata_vec3(line, &vertices, &vertex_count);
			}
		}
		
		if(line.words[0][0] == 'f') {
			// face line
			obj_parse_stringdata_face(line, &faces, &face_count, &faces_length, &face_len_count);
		}
		
	sslr:
		free_stringdata(line);
		
		i++;
	}
	
	free_stringdata(data);
	
	// parse the faces and verticies into a single 'vertex' list
	// { VERTEX, NORMAL, COLOR } , ...
	
	printf("vertices: %i\n", vertex_count);
	printf("normals: %i\n", normal_count);
	printf("faces: %i\n", face_count);
	
	// build a 'vertex' list in { XYZ, NXYZ, RGB } format.
	
	
	int vm_count;
	struct vertex * vm;

	vm_count = 0;
	vm = malloc(sizeof(struct vertex) * (8 * face_count + 1)); // temp
	
	vec3_ colors[7] = { { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 }, { 1, 0, 1 }, { 1, 1, 0 }, { 0, 1, 1 } };
	
	int vid, nid, cid = 0, j, k = 0;
	
	i = 0;
	j = 0, k = 0;
	int cc = 0; // color
	struct vertex vtx[3];
	// foreach set of face vertices
	while(i < face_len_count) {
		// build N new vertices
		int len = faces_length[i];
		k = j + len;
		
		// origin buf[1] :: 0
		vid = faces[j][0] - 1;
		nid = faces[j][2] - 1;			
		// load color
		cc += 1;
		if(cc >= 3) {
			cc = 0;
			cid++;
			if(cid >= 6)
				cid = 0;
		}
		
		vtx[0].x = vertices[vid][0];
		vtx[0].y = vertices[vid][1];
		vtx[0].z = vertices[vid][2];
		vtx[0].nx = normals[nid][0];
		vtx[0].ny = normals[nid][1];
		vtx[0].nz = normals[nid][2];
		vtx[0].r = colors[cid][0];
		vtx[0].g = colors[cid][1];
		vtx[0].b = colors[cid][2];
		j++;
		
		while(j < k) {
			// origin buf[1] :: 0
			
			// read #2
			vid = faces[j][0] - 1;
			nid = faces[j][2] - 1;	
		
			vtx[1].x = vertices[vid][0];
			vtx[1].y = vertices[vid][1];
			vtx[1].z = vertices[vid][2];
			vtx[1].nx = normals[nid][0];
			vtx[1].ny = normals[nid][1];
			vtx[1].nz = normals[nid][2];
			vtx[1].r = colors[cid][0];
			vtx[1].g = colors[cid][1];
			vtx[1].b = colors[cid][2];
			
			j++;
			
			// overflow check since we attempt to read two vertices even though there might only be
			// one or zero left.
			if(j < k) {
				// read #3
				vid = faces[j][0] - 1;
				nid = faces[j][2] - 1;	
		
				vtx[2].x = vertices[vid][0];
				vtx[2].y = vertices[vid][1];
				vtx[2].z = vertices[vid][2];
				vtx[2].nx = normals[nid][0];
				vtx[2].ny = normals[nid][1];
				vtx[2].nz = normals[nid][2];
				vtx[2].r = colors[cid][0];
				vtx[2].g = colors[cid][1];
				vtx[2].b = colors[cid][2];
			
			cc += 1;
			if(cc >= 3) {
				cc = 0;
				cid++;
				if(cid >= 6)
					cid = 0;
			}
			
			vm[vm_count] = vtx[0]; vm_count++;
			vm[vm_count] = vtx[1]; vm_count++;
			vm[vm_count] = vtx[2]; vm_count++;
			}
			// vtx[1] = vtx[2]; // cycle
		}
		
		i++;
	}

	vm = realloc(vm, sizeof(struct vertex) * (vm_count + 1)); // reduce
	
	#if 0
	printf("dump obj:\n");
	i = 0;
	while(i < vertex_count) {
		printf("v %f %f %f\n", vertices[i][0], vertices[i][1], vertices[i][2]);
		i++;
	}
	i = 0;
	while(i < normal_count) {
		printf("n %f %f %f\n", normals[i][0], normals[i][1], normals[i][2]);
		i++;
	}
	i = 0;
	while(i < face_count) {
		printf("f %i %i %i\n", (int)faces[i][0], (int)faces[i][1], (int)faces[i][2]);
		i++;
	}
	#endif
	
	#if 0
	// hack (loads a premade cube model)
	md->vertex_count = cube_vertices;
	md->vertices = malloc(sizeof(struct vertex) * cube_vertices);
	memcpy(md->vertices, cube_meshdata, sizeof(struct vertex) * cube_vertices);
	#endif
	
	md->vertex_count = vm_count;
	md->vertices = malloc(sizeof(struct vertex) * vm_count);
	memcpy(md->vertices, vm, sizeof(struct vertex) * vm_count);
	
	free(vm);
	
	free(vertices);
	free(normals);
	free(faces_length);
	free(faces);
	
	return 0;
}













#endif /* GLSHADER_H */
