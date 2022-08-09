#ifndef MODELS_H
#define MODELS_H

#include "vertex.h"

const int triangle_faces = 1;
const int triangle_edges = 3;
const int triangle_vertices = 3;
const struct vertex triangle_meshdata[3] = {
	{ -1.0, -1.0,  0.0, 1, 0, 0 },
	{  0.0,  1.0,  0.0, 0, 1, 0 },
	{  1.0, -1.0,  0.0, 0, 0, 1 } 
};

const int quad_faces = 2;
const int quad_edges = 4;
const int quad_vertices = 6;
const struct vertex quad_meshdata[6] = {
	{  1.0, -1.0,  0.0, 1, 0, 0 },
	{ -1.0, -1.0,  0.0, 0, 1, 0 },
	{ -1.0,  1.0,  0.0, 0, 0, 1 },
	{  1.0, -1.0,  0.0, 1, 0, 0 },
	{ -1.0,  1.0,  0.0, 0, 0, 1 },
	{  1.0,  1.0,  0.0, 0, 1, 0 },
};

const int tetrahedron_faces = 4;
const int tetrahedron_edges = 6;
const int tetrahedron_vertices = 12;
const struct vertex tetrahedron_meshdata[12] = {
	{  0.0,  1.0,  0.0, 1, 0, 0 },
    { -1.0, -1.0,  0.0, 1, 0, 0 },
    {  1.0, -1.0,  0.0, 1, 0, 0 },
    {  0.0,  1.0,  0.0, 0, 1, 0 },
    {  1.0, -1.0,  0.0, 0, 1, 0 },
    {  0.0, -1.0, -1.0, 0, 1, 0 },
    {  0.0, -1.0, -1.0, 0, 0, 1 },
    { -1.0, -1.0,  0.0, 0, 0, 1 },
    {  0.0,  1.0,  0.0, 0, 0, 1 },
    {  0.0, -1.0, -1.0, 1, 1, 0 },
    {  1.0, -1.0,  0.0, 1, 1, 0 },
    { -1.0, -1.0,  0.0, 1, 1, 0 },
};

const int cube_faces = 6;
const int cube_edges = 12;
const int cube_vertices = 36;
const struct vertex cube_meshdata[36] = {
    {  1.0,  1.0,  1.0, 1, 0, 0 },
    {  1.0, -1.0,  1.0, 1, 0, 0 },
    {  1.0,  1.0, -1.0, 1, 0, 0 },
    {  1.0, -1.0,  1.0, 1, 0, 0 },
    {  1.0, -1.0, -1.0, 1, 0, 0 },
    {  1.0,  1.0, -1.0, 1, 0, 0 },
    
    {  1.0,  1.0, -1.0, 0, 1, 0 },
    { -1.0,  1.0,  1.0, 0, 1, 0 },
    {  1.0,  1.0,  1.0, 0, 1, 0 },
    { -1.0,  1.0, -1.0, 0, 1, 0 },
    { -1.0,  1.0,  1.0, 0, 1, 0 },
    {  1.0,  1.0, -1.0, 0, 1, 0 },
    
    { -1.0, -1.0, -1.0, 0, 0, 1 },
    { -1.0, -1.0,  1.0, 0, 0, 1 },
    { -1.0,  1.0, -1.0, 0, 0, 1 },
    { -1.0, -1.0,  1.0, 0, 0, 1 },
    { -1.0,  1.0,  1.0, 0, 0, 1 },
    { -1.0,  1.0, -1.0, 0, 0, 1 },
    
    {  1.0,  1.0,  1.0, 1, 1, 0 },
    { -1.0,  1.0,  1.0, 1, 1, 0 },
    {  1.0, -1.0,  1.0, 1, 1, 0 },
    { -1.0, -1.0,  1.0, 1, 1, 0 },
    {  1.0, -1.0,  1.0, 1, 1, 0 },
    { -1.0,  1.0,  1.0, 1, 1, 0 },
    
    // down
    {  1.0, -1.0, -1.0, 0, 1, 1 },
    {  1.0, -1.0,  1.0, 0, 1, 1 },
    { -1.0, -1.0,  1.0, 0, 1, 1 },
    {  1.0, -1.0, -1.0, 0, 1, 1 },
    { -1.0, -1.0,  1.0, 0, 1, 1 },
    { -1.0, -1.0, -1.0, 0, 1, 1 },
    
    { -1.0,  1.0, -1.0, 1, 0, 1 },
    {  1.0, -1.0, -1.0, 1, 0, 1 },
    { -1.0, -1.0, -1.0, 1, 0, 1 },
    {  1.0,  1.0, -1.0, 1, 0, 1 },
    {  1.0, -1.0, -1.0, 1, 0, 1 },
    { -1.0,  1.0, -1.0, 1, 0, 1 },
};

#endif /* MODELS_H */