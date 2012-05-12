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

#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <d3d9.h>
#include <d3dx9tex.h>
#include <Dxerr.h>
#include <string>

#include "grid.h"

Grid::Grid(
    const MAP& map,
    D3DXVECTOR3 *v0P,
    D3DXVECTOR3 *v1P,
    D3DXVECTOR3 *v2P,
    D3DXVECTOR3 *v3P
    ) {
	this->widthVertices = map.width;
	this->heightVertices = map.height;

	this->numVertices = this->widthVertices * this->heightVertices;
	this->widthFaces = this->widthVertices - 1;
	this->heightFaces = this->heightVertices - 1;
	this->numFaces = this->widthFaces * this->heightFaces * 2; // 2 triangles in each grid cell
	this->v0 = *v0P;
	this->v1 = *v1P;
	this->v2 = *v2P;
	this->v3 = *v3P;
	this->dv = this->v2 - this->v0;
	this->dv.x /= float(this->widthVertices - 1);
	this->dv.y /= 1;
	this->dv.z /= float(this->heightVertices - 1);

	this->scale = 1.0f;
	this->vOffset = 0.0f;
	this->uOffset = 0.0f;
	
	this->vertices = NULL;
	this->mesh = NULL;

	int i, j, k;

	// set up the vertex and normal and texture data
	this->vertices = new GRIDVERTEX[this->numVertices];
	
	D3DXVECTOR3 a, b;
	
	for (j = 0, k = 0; j < this->heightVertices; j++) {
		for (i = 0; i < this->widthVertices; i++, k++) {
			// Set vertex position
			vertices[k].position.x = this->v0.x + float(i) * this->dv.x;
			vertices[k].position.y = this->v0.y + map.array[k] * this->dv.y;
			vertices[k].position.z = this->v0.z + float(j) * this->dv.z;
			
			// Calculate vertex normal (*TODO: should compute normal over a larger patch (3x3 vertices) for smoother features)
			a.x = 1.0f;
			a.y = 0.0f;
			a.z = (map.array[k] - map.array[k + 1]) * this->dv.y;
			
			b.x = 0.0f;
			b.y = 1.0f;
			b.z = (map.array[k] - map.array[k + this->widthVertices]) * this->dv.y;
			
			D3DXVec3Normalize(&this->vertices[k].normal, D3DXVec3Cross(&this->vertices[k].normal, &a, &b));
			
			// Set vertex color
			this->vertices[k].color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}

	UpdateUVs();

	// set up the index data
	indexData = new WORD[numFaces * 3];

	// note direct x uses a CW winding order
	// vertices 0, 1, 2, 3 in the following configuration
	// 2        3
	//  
	//   
	// 0        1
	for (j = 0, k = 0; j < this->heightFaces; j++) {
		for (i = 0; i < this->widthFaces; i++) {
			indexData[k++] = j * this->widthVertices + i; // vertex 0
			indexData[k++] = (j + 1) * this->widthVertices + i + 1; // vertex 3
			indexData[k++] = j * this->widthVertices + i + 1; // vertex 1

			indexData[k++] = j * this->widthVertices + i; // vertex 0
			indexData[k++] = (j + 1) * this->widthVertices + i; // vertex 2
			indexData[k++] = (j + 1) * this->widthVertices + i + 1; // vertex 3
		}
	}
}

void Grid::UpdateUVs() {
	int i, j, k;
	for (j = 0, k = 0; j < this->heightVertices; j++) {
		for (i = 0; i < this->widthVertices; i++, k++) {
			this->vertices[k].u = this->scale * ((float) i + this->uOffset);
			this->vertices[k].v = this->scale * ((float) j + this->vOffset);
		}
	}
}

void Grid::GetGridPoint(
    D3DXVECTOR3 *pt,
    int i, // row (from bottom to top)
    int j // column (from left to right)
    ) {
	*pt = this->vertices[j * this->heightVertices * i].position;
}

void Grid::GetGridNormal(
    D3DXVECTOR3 *normal,
    int i,
    int j
    ) {
	unsigned int k = i + j * this->widthVertices;
	
	normal->x = this->vertices[k].normal.x;
	normal->y = this->vertices[k].normal.y;
	normal->z = this->vertices[k].normal.z;
}

void Grid::CreateDirectXMesh(
    LPDIRECT3DDEVICE9 pd3dDevice
    ) {
	HRESULT hr;
	VOID* pData;
	hr = D3DXCreateMeshFVF(this->numFaces, this->numVertices, D3DXMESH_DYNAMIC /* | D3DXMESH_32BIT */, D3DFVF_GRIDVERTEX, pd3dDevice, &this->mesh);
	//*NOTE: the 32 bit mesh causes the DrawSubset method to have an access violation on my machine....
	if (FAILED(hr)) {
#ifdef DEBUG
		char buf[2048];
		sprintf_s(buf, 2048, "Grid Create Mesh Error: %s error description: %s\n", DXGetErrorString(hr), DXGetErrorDescription(hr));
		OutputDebugString(buf);
#endif
		return;
	}

	hr = this->mesh->LockVertexBuffer(D3DLOCK_DISCARD, (void**) &pData);
	if (FAILED(hr)) {
#ifdef DEBUG
		char buf[2048];
		sprintf_s(buf, 2048, "Grid Lock Vertex Buffer Error: %s error description: %s\n", DXGetErrorString(hr), DXGetErrorDescription(hr));
		OutputDebugString(buf);
#endif
		return;
	}

	// copy the vertices into the buffer
	memcpy(pData, this->vertices, this->numVertices * sizeof(GRIDVERTEX));

	// unlock the vertex buffer
	this->mesh->UnlockVertexBuffer();

	this->mesh->LockIndexBuffer(0, &pData);

	memcpy(pData, this->indexData, this->numFaces * 3 * sizeof(WORD));

	this->mesh->UnlockIndexBuffer();
}

void Grid::DrawDirectXMesh(
    LPDIRECT3DDEVICE9 pd3dDevice
    ) {
	HRESULT hr;
	if (this->mesh != NULL) {
		pd3dDevice->SetFVF(this->mesh->GetFVF());
		hr = this->mesh->DrawSubset(0);
		
		if (FAILED(hr)) {
#ifdef DEBUG
			char buf[2048];
			sprintf_s(buf, 2048, "Grid Draw Error: %s error description: %s\n", DXGetErrorString(hr), DXGetErrorDescription(hr));
			OutputDebugString(buf);
#endif
		}
	}
}
