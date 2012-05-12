/**
 * \file
 * \author Unknown - imported by Ricky Curtice
 * \date 20110724
 * \brief Provides a list of the global property key strings created by Graphics Core and their default values
 *
 * <p>
 * </p>
 *
 */
#pragma once

#include <d3d9.h>
#include <d3dx9math.h>
#include <d3dx9tex.h>
#include <d3dx9mesh.h>

#ifndef PI
#define PI 3.14159265f
#endif

struct GRIDVERTEX {
	D3DXVECTOR3 position; // The untransformed, 3D position for the vertex
	D3DXVECTOR3 normal; // normalized normal
	DWORD color; // color for this vertex
	float u, v; // the texture coordinates
};

#define D3DFVF_GRIDVERTEX (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

struct MAP {
	unsigned int width, height;
	float* array;
};

class Grid {
private:
	int widthVertices; // width in vertices
	int heightVertices; // height in vertices
	int widthFaces; // width in faces
	int heightFaces; // height in faces
	unsigned long numVertices;
	int numFaces;
	D3DXVECTOR3 v0, v1, v2, v3; // in D3D face order
	D3DXVECTOR3 dv; // delta v from 0 to 2
	GRIDVERTEX *vertices;
	WORD *indexData;
	LPD3DXMESH mesh;
	float scale;
	float uOffset, vOffset;
	public:
	Grid(
	    const MAP& map,
	    D3DXVECTOR3 *v0P,
	    D3DXVECTOR3 *v1P,
	    D3DXVECTOR3 *v2P,
	    D3DXVECTOR3 *v3P
	    );

	~Grid() {
		delete vertices;
		delete indexData;
	}
	;

	void Startup();

	void GetGridPoint(
	    D3DXVECTOR3 *pt,
	    int i, // row (from bottom to top)
	    int j // column (from left to right)
	    );

	void GetGridNormal(
	    D3DXVECTOR3 *n,
	    int i, // row (from bottom to top)
	    int j // column (from left to right)
	    );

	void UpdateUVs();

	void CreateDirectXMesh(
	    LPDIRECT3DDEVICE9 pd3dDevice
	    );

	void DrawDirectXMesh(
	    LPDIRECT3DDEVICE9 pd3dDevice
	    );

	float getTexScale() {
		return scale;
	}

	void setTexScale(float value) {
		scale = value;
		UpdateUVs();
	}

	float getTexOffsetU() {
		return uOffset;
	}

	void setTexOffsetU(float value) {
		uOffset = value;
		UpdateUVs();
	}

	float getTexOffsetV() {
		return vOffset;
	}

	void setTexOffsetV(float value) {
		vOffset = value;
		UpdateUVs();
	}

};
