#ifndef VERTEX_H
#define VERTEX_H

#include "../common/math.h"

typedef struct vec3_s {
	float x, y, z;
} vec3;

// make vec3
struct vertex {
	float x, y, z;		// pos
	float nx, ny, nz;	// normal
	float r, g, b;		// rgb
};

struct mesh_data {
	// int id; 
	char * path; // relative
	int offset;
	int vertex_count;
	struct vertex * vertices;
};

struct model {
	int id;
	int mesh_index;
	
	vec3_ pos, rot, scale;
};

// todo: separete
struct model_manager {
	int model_count;
	struct model * models;
	
	int mesh_count;
	int _offset;
	int _vertex_count;
	struct mesh_data * meshes;
};

#include "../common/shader.h" //  aaaagh

void mm_init(struct model_manager * mm, int mesh_count, int model_count) {
	mm->model_count = 0;
	mm->mesh_count = 0;
	
	mm->_offset = 0;
	mm->_vertex_count = 0;
	
	mm->models = malloc(sizeof(struct model) * model_count);
	mm->meshes = malloc(sizeof(struct mesh_data) * mesh_count);
}

void mm_free(struct model_manager * mm) {
	free(mm->models);
	
	int i = 0;
	int j = mm->mesh_count;
	
	while(i < j) {
		// todo: maybe do path free as well?
		if(mm->meshes[i].vertices != NULL);
			free(mm->meshes[i].vertices);
		i++;
	}
	
	free(mm->meshes);
}

int mm_load_mesh(struct model_manager * mm, char * path) {
	int id = mm->mesh_count;
	struct mesh_data md;
	
	if(load_model_obj(path, &md)) {
		printf("Could not load model [%s]\n", path);
		return -1;
	}
	
	md.path = path; // do alloc
	md.offset = mm->_offset;
	
	mm->_offset += md.vertex_count;
	mm->_vertex_count += md.vertex_count; 
	
	mm->meshes[mm->mesh_count] = md;
	mm->mesh_count += 1;
	
	return id;
};

// 25 USD to push the plugin
// use Manifest V3 (Json) - JS 
// https://developer.chrome.com/docs/webstore/?hl=sv
// https://developer.chrome.com/docs/extensions/reference/scripting/
// write a browser plugin that swaps 'shorts/' to 'watch?v=' in youtube urls.

void mm_write_meshes_to_opengl(struct model_manager * mm) {
	int vcount;
	struct vertex * vbuf;
	struct mesh_data md;

	printf("meshes: %i\n", mm->mesh_count);
	printf("vertices: %i\n", mm->_vertex_count);

	vcount = mm->_vertex_count;
	vbuf = malloc(sizeof(struct vertex) * vcount);
	
	int i = 0;
	while(i < mm->mesh_count) {
		md = mm->meshes[i];
		memcpy(vbuf + md.offset, md.vertices, sizeof(struct vertex) * md.vertex_count);
		i++;
	}
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(struct vertex) * vcount, vbuf, GL_STATIC_DRAW);
	
	free(vbuf);
}

void mm_render_models_opengl(struct model_manager * mm, GLuint u_model_mat4_loc) {
	int i = 0;
	int j = mm->model_count;
	
	struct model m;
	struct mesh_data md;
	
	mat4_ t, r, s, mvp;
	
	while(i < j) {
		
		m = mm->models[i];
		md = mm->meshes[m.mesh_index];
		
		// placement
		mat4_identity(s);
		mat4_identity(r);
		mat4_identity(t);
		mat4_identity(mvp);
		
		// 1. scale
		mat4_scale_vec3(s, m.scale);
		
		// 2. rotate
		mat4_rotx(r, m.rot[0], r);
		mat4_roty(r, m.rot[1], r);
		mat4_rotz(r, m.rot[2], r);
		
		// 2. translate
		mat4_translate(t, m.pos);
		
		// 3. build model matrix
		mat4_mul(mvp, s, mvp);
		mat4_mul(mvp, t, mvp);
		mat4_mul(mvp, r, mvp);
		
		// 4. push mvp
		glUniformMatrix4fv(u_model_mat4_loc, 1, GL_FALSE, (GLfloat *)mvp);
		
		// 5. render
		glDrawArrays(GL_TRIANGLES, md.offset, md.vertex_count);
	
		i++;
	}
}

void mm_push_model(struct model_manager * mm, int id, vec3_ pos, vec3_ rot, vec3_ scale) {
	struct model m;
	
	m.id = id;
	m.mesh_index = id; // err

	vec3_copy(pos, m.pos);
	vec3_copy(rot, m.rot);
	vec3_copy(scale, m.scale);
	
	mm->models[mm->model_count++] = m;
}

#endif /* VERTEX_H */
