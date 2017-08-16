#pragma once


#include <D3D11.h>
#include <D3DX10math.h>
#include "textureclass.h"
#include <fstream>
#include <vector>
#include "TextureArrayClass.h"
using namespace std;


class ModelClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
		D3DXVECTOR3 tangent;
		D3DXVECTOR3 binormal;
	};
	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
		float tx, ty, tz;
		float bx, by, bz;
	};
	struct TempVertexType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	struct VectorType
	{
		float x, y, z;
	};
	float sizeX, sizeY, sizeZ;

public:
	float centerX, centerY, centerZ;
	float alphachannel = 1.0f;
	void hit();
	float healthcolor = 1.0f;
	float positionX, positionY, positionZ;
	float rotatebeforedegreepitch = 0;
	float rotatebeforedegreeyaw = 0;
	float rotatebeforedegreeroll = 0;
	float rotateafterdegreepitch = 0;
	float rotateafterdegreeyaw = 0;
	float rotateafterdegreeroll = 0;
	D3DXMATRIX rotatebefore;
	D3DXMATRIX rotateafter;
	D3DXMATRIX translate;
	D3DXMATRIX scale;
	D3DXMATRIX translateafter;
	bool multitextured = 0;
	bool specularmapped = 0;
	bool isbreakable = true;
	bool isbreaked = false;

	ModelClass();
	~ModelClass();

	bool Initialize(ID3D11Device*, char*, char*, int*);
	bool Initialize1(ID3D11Device*, char*, char*, char*, int*);
	bool Initialize2(ID3D11Device*, char*, char*, char*, char*, int*);
	void Shutdown();
	ID3D11ShaderResourceView ** GetTextureArray();
	bool LoadTextures(ID3D11Device * device, char * filename1, char * filename2);
	bool LoadTextures(ID3D11Device * device, char * filename1, char * filename2, char * filename3);
	void ReleaseTextures();
	void Render(ID3D11DeviceContext*);
	unsigned long getFilesize(char* filename);

	int GetIndexCount();

	ID3D11ShaderResourceView* GetTexture();
	void GetData(float *, float *, float *, float *, float *, float *);
	void setPosition(float, float, float);

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, char*);
	void ReleaseTexture();

	bool LoadModel(char*);
	bool LoadObjModel(char * filename);
	void ReleaseModel();

	void CalculateModelVectors();
	void CalculateTangentBinormal(TempVertexType, TempVertexType, TempVertexType, VectorType&, VectorType&);
	void CalculateNormal(VectorType, VectorType, VectorType&);
private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;

	TextureClass* m_Texture;
	vector<ModelType> m_model;
	TextureArrayClass* m_TextureArray;
};

