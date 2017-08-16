#include "ModelClass.h"
#include <vector>
#include <string>
#include <sstream>

void ModelClass::hit()
{
	healthcolor -= 0.2;
	if (healthcolor < 0)
	{
		isbreaked = true;
	}
}

ModelClass::ModelClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_Texture = 0;
	m_TextureArray = 0;

	D3DXMatrixRotationY(&rotatebefore, 0);
	D3DXMatrixRotationY(&rotateafter, 0);
	D3DXMatrixTranslation(&translate, 0, 0, 0);
	D3DXMatrixTranslation(&translateafter, 0, 0, 0);
	D3DXMatrixScaling(&scale, 1, 1, 1);
}


ModelClass::~ModelClass()
{
}

bool ModelClass::Initialize(ID3D11Device *device, char* modelFilename, char* textureFilename, int *threads)
{
	(*threads)++;
	bool result;

	// Load in the model data,
	//result = LoadModel(modelFilename);
	result = LoadObjModel(modelFilename);
	if (!result)
	{
		(*threads)--;
		return false;
	}


	result = InitializeBuffers(device);
	if (!result)
	{
		(*threads)--;
		return false;
	}

	// Load the texture for this model.
	result = LoadTexture(device, textureFilename);
	if (!result)
	{
		(*threads)--;
		return false;
	}
	(*threads)--;
	return true;
}

bool ModelClass::Initialize1(ID3D11Device* device, char* modelFilename, char* textureFilename1, char* textureFilename2, int* threads)
{
	(*threads)++;
	bool result;
	multitextured = true;

	// Load in the model data,
	result = LoadObjModel(modelFilename);
	if (!result)
	{
		(*threads)--;
		return false;
	}

	// Calculate the normal, tangent, and binormal vectors for the model.
	CalculateModelVectors();

	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}
	// Load the textures for this model.
	result = LoadTextures(device, textureFilename1, textureFilename2);
	if (!result)
	{
		(*threads)--;
		return false;
	}
	(*threads)--;
	return true;
}

bool ModelClass::Initialize2(ID3D11Device* device, char* modelFilename, char* textureFilename1, char* textureFilename2, char* textureFilename3, int* threads)
{
	(*threads)++;
	bool result;
	specularmapped = true;
	multitextured = true;

	// Load in the model data,
	result = LoadObjModel(modelFilename);
	if (!result)
	{
		(*threads)--;
		return false;
	}

	// Calculate the normal, tangent, and binormal vectors for the model.
	CalculateModelVectors();

	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}
	// Load the textures for this model.
	result = LoadTextures(device, textureFilename1, textureFilename2, textureFilename3);
	if (!result)
	{
		(*threads)--;
		return false;
	}
	(*threads)--;
	return true;
}

void ModelClass::Shutdown()
{
	// Release the model textures.
	ReleaseTexture();
	ShutdownBuffers();
	// Release the model data.
	ReleaseModel();

	return;
}

ID3D11ShaderResourceView** ModelClass::GetTextureArray()
{
	return m_TextureArray->GetTextureArray();
}

bool ModelClass::LoadTextures(ID3D11Device* device, char* filename1, char* filename2)
{
	bool result;


	// Create the texture array object.
	m_TextureArray = new TextureArrayClass;
	if (!m_TextureArray)
	{
		return false;
	}

	// Initialize the texture array object.
	result = m_TextureArray->Initialize(device, filename1, filename2);
	if (!result)
	{
		return false;
	}

	return true;
}

bool ModelClass::LoadTextures(ID3D11Device* device, char* filename1, char* filename2, char* filename3)
{
	bool result;


	// Create the texture array object.
	m_TextureArray = new TextureArrayClass;
	if (!m_TextureArray)
	{
		return false;
	}

	// Initialize the texture array object.
	result = m_TextureArray->Initialize(device, filename1, filename2, filename3);
	if (!result)
	{
		return false;
	}

	return true;
}

void ModelClass::ReleaseTextures()
{
	// Release the texture array object.
	if (m_TextureArray)
	{
		m_TextureArray->Shutdown();
		delete m_TextureArray;
		m_TextureArray = 0;
	}

	return;
}

ID3D11ShaderResourceView* ModelClass::GetTexture()
{
	return m_Texture->GetTexture();
}

void ModelClass::Render(ID3D11DeviceContext *deviceContext)
{
	RenderBuffers(deviceContext);

	return;
}

unsigned long ModelClass::getFilesize(char * filename)
{
	unsigned long begin, end;
	ifstream file;
	file.open(filename, ios::in);
	begin = file.tellg();
	file.seekg(0, ios::end);
	end = file.tellg();
	file.close();
	return end - begin;
}

int ModelClass::GetIndexCount()
{
	return m_indexCount;
}

bool ModelClass::InitializeBuffers(ID3D11Device *device)
{
	int i;
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	//m_indexCount = 6;
	//m_vertexCount = 4;
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}
	// Load the vertex array and index array with data.
	for (i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = D3DXVECTOR3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = D3DXVECTOR2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = D3DXVECTOR3(m_model[i].nx, m_model[i].ny, m_model[i].nz);
		vertices[i].tangent = D3DXVECTOR3(m_model[i].tx, m_model[i].ty, m_model[i].tz);
		vertices[i].binormal = D3DXVECTOR3(m_model[i].bx, m_model[i].by, m_model[i].bz);
		indices[i] = i;
	}

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)*m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)*m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	delete[] vertices;
	vertices = 0;
	delete[] indices;
	indices = 0;

	return true;

}

void ModelClass::ShutdownBuffers()
{
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}

void ModelClass::RenderBuffers(ID3D11DeviceContext *deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexType);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

bool ModelClass::LoadTexture(ID3D11Device* device, char* filename)
{
	bool result;


	// Create the texture object.
	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture->Initialize(device, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

void ModelClass::ReleaseTexture()
{
	// Release the texture object.
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}

bool ModelClass::LoadModel(char* filename)
{
	ifstream fin;
	char input;
	int i;


	// Open the model file.
	fin.open(filename);

	// If it could not open the file then exit.
	if (fin.fail())
	{
		return false;
	}

	// Read up to the value of vertex count.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	// Read in the vertex count.
	fin >> m_vertexCount;

	// Set the number of indices to be the same as the vertex count.
	m_indexCount = m_vertexCount;

	//// Create the model using the vertex count that was read in.
	//m_model = new ModelType[m_vertexCount];
	//if (!m_model)
	//{
	//	return false;
	//}

	// Read up to the beginning of the data.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in the vertex data.
	for (i = 0; i < m_vertexCount; i++)
	{
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}

	// Close the model file.
	fin.close();

	return true;
}

bool ModelClass::LoadObjModel(char* filename)
{
	ifstream file1;
	int i;
	vector<vector<float>> vertexes;
	vector<vector<float>> vt;
	vector<vector<float>> vn;
	stringstream file;

	// Open the model file.
	file1.open(filename, ios::in | ios::binary);

	// If it could not open the file then exit.
	if (file1.fail())
	{
		return false;
	}
	file1.seekg(0);
	file << file1.rdbuf();
	file1.close();
	float maxX = 0, minX = 0;
	float maxY = 0, minY = 0;
	float maxZ = 0, minZ = 0;
	unsigned int vertexindex = 0;
	unsigned int normalindex = 0;
	unsigned int uvindex = 0;
	bool havenormal = 0;
	bool haveuv = 0;
	string temp;
	bool isfirstvertex = true;
	while (!file.eof())
	{
		file >> temp;
		if (temp == "v")
		{
			float x;
			vertexes.push_back(vector<float>());
			for (int i = 0; i < 3; i++)
			{
				file >> x;
				vertexes[vertexindex].push_back(x);
				if (i == 0)
				{
					if (x < minX)
					{
						minX = x;
					}
					else if (x > maxX)
					{
						maxX = x;
					}
				}
				else if (i == 1)
				{
					if (x < minY)
					{
						minY = x;
					}
					else if (x > maxY)
					{
						maxY = x;
					}
				}
				else
				{
					if (x < minZ)
					{
						minZ = x;
					}
					else if (x > maxZ)
					{
						maxZ = x;
					}
				}
				if (isfirstvertex)
				{
					if (i == 0)
					{
						minX = x;
						maxX = x;
					}
					else if (i == 1)
					{

						minY = x;
						maxY = x;
					}
					else
					{
						minZ = x;
						maxZ = x;
					}
				}
			}
			vertexindex++;
			isfirstvertex = false;
		}
		else if (temp == "vn")
		{
			havenormal = 1;
			float x;
			vn.push_back(vector<float>());
			for (int i = 0; i < 3; i++)
			{
				file >> x;
				vn[normalindex].push_back(x);
			}
			normalindex++;
		}
		else if (temp == "vt")
		{
			haveuv = 1;
			float x;
			vt.push_back(vector<float>());
			for (int i = 0; i < 2; i++)
			{
				file >> x;
				if (i == 1)
				{
					x = 1.0f - x;
				}
				vt[uvindex].push_back(x);
			}
			uvindex++;
		}
		else if (temp == "f")
		{
			int x;
			if (!haveuv && !havenormal)
			{
				for (int i = 0; i < 3; i++)
				{
					file >> x;
					ModelType temp;
					temp.x = vertexes[x - 1][0];
					temp.y = vertexes[x - 1][1];
					temp.z = vertexes[x - 1][2];
					m_model.push_back(temp);
				}
			}
			else if (!havenormal&&haveuv)
			{
				unsigned int x;
				unsigned int y;
				for (int i = 0; i < 3; i++)
				{
					file >> x;
					file.get();
					file >> y;
					ModelType temp;
					temp.x = vertexes[x - 1][0];
					temp.y = vertexes[x - 1][1];
					temp.z = vertexes[x - 1][2];
					temp.tu = vt[y - 1][0];
					temp.tv = vt[y - 1][1];
					m_model.push_back(temp);

				}
			}
			else if (havenormal&&haveuv)
			{
				unsigned int x;
				unsigned int y;
				unsigned int z;
				for (int i = 0; i < 3; i++)
				{
					file >> x;
					file.get();
					file >> y;
					file.get();
					file >> z;
					ModelType temp;
					temp.x = vertexes[x - 1][0];
					temp.y = vertexes[x - 1][1];
					temp.z = vertexes[x - 1][2];
					temp.nx = vn[z - 1][0];
					temp.ny = vn[z - 1][1];
					temp.nz = vn[z - 1][2];
					temp.tu = vt[y - 1][0];
					temp.tv = vt[y - 1][1];
					m_model.push_back(temp);
				}
			}
			else if (havenormal && !haveuv)
			{
				unsigned int x;
				unsigned int y;
				unsigned int z;
				for (int i = 0; i < 3; i++)
				{
					file >> x;
					file.get();
					file.get();
					file >> z;
					ModelType temp;
					temp.x = vertexes[x - 1][0];
					temp.y = vertexes[x - 1][1];
					temp.z = vertexes[x - 1][2];
					temp.nx = vn[z - 1][0];
					temp.ny = vn[z - 1][1];
					temp.nz = vn[z - 1][2];
					m_model.push_back(temp);
				}
			}
		}
	}
	maxX = (float)((int)(maxX * 100)) / 100;
	maxY = (float)((int)(maxY * 100)) / 100;
	maxZ = (float)((int)(maxZ * 100)) / 100;
	minX = (float)((int)(minX * 100)) / 100;
	minY = (float)((int)(minY * 100)) / 100;
	minZ = (float)((int)(minZ * 100)) / 100;
	centerX = ((maxX - minX) / 2) + minX;
	centerY = ((maxY - minY) / 2) + minY;
	centerZ = (((maxZ - minZ) / 2) + minZ);
	sizeX = (maxX - minX);
	sizeY = (maxY - minY);
	sizeZ = (maxZ - minZ);
	m_vertexCount = m_model.size();
	m_indexCount = m_vertexCount;
	vertexes.clear();
	vt.clear();
	vn.clear();


	return true;
}

void ModelClass::ReleaseModel()
{
	if (m_model.size() != 0)
	{
		//delete[] m_model;
		//m_model = 0;
		m_model.clear();
	}

	return;
}

void ModelClass::setPosition(float x, float y, float z)
{
	positionX = x;
	positionY = y;
	positionZ = z;
}

void ModelClass::GetData(float* positionX, float* positionY, float* positionZ, float* sizeX, float* sizeY, float* sizeZ)
{
	*positionX = this->positionX - centerX;
	*positionY = this->positionY - centerY;
	*positionZ = this->positionZ - centerZ;
	D3DXVECTOR3  scale, translate;
	D3DXQUATERNION rotate;
	D3DXMatrixDecompose(&scale, &rotate, &translate, &this->scale);
	*sizeX = this->sizeX*scale.x;
	*sizeY = this->sizeY*scale.y;
	*sizeZ = this->sizeZ*scale.z;
}

void ModelClass::CalculateModelVectors()
{
	int faceCount, i, index;
	TempVertexType vertex1, vertex2, vertex3;
	VectorType tangent, binormal, normal;


	// Calculate the number of faces in the model.
	faceCount = m_vertexCount / 3;

	// Initialize the index to the model data.
	index = 0;

	// Go through all the faces and calculate the the tangent, binormal, and normal vectors.
	for (i = 0; i < faceCount; i++)
	{
		// Get the three vertices for this face from the model.
		vertex1.x = m_model[index].x;
		vertex1.y = m_model[index].y;
		vertex1.z = m_model[index].z;
		vertex1.tu = m_model[index].tu;
		vertex1.tv = m_model[index].tv;
		vertex1.nx = m_model[index].nx;
		vertex1.ny = m_model[index].ny;
		vertex1.nz = m_model[index].nz;
		index++;

		vertex2.x = m_model[index].x;
		vertex2.y = m_model[index].y;
		vertex2.z = m_model[index].z;
		vertex2.tu = m_model[index].tu;
		vertex2.tv = m_model[index].tv;
		vertex2.nx = m_model[index].nx;
		vertex2.ny = m_model[index].ny;
		vertex2.nz = m_model[index].nz;
		index++;

		vertex3.x = m_model[index].x;
		vertex3.y = m_model[index].y;
		vertex3.z = m_model[index].z;
		vertex3.tu = m_model[index].tu;
		vertex3.tv = m_model[index].tv;
		vertex3.nx = m_model[index].nx;
		vertex3.ny = m_model[index].ny;
		vertex3.nz = m_model[index].nz;
		index++;

		// Calculate the tangent and binormal of that face.
		CalculateTangentBinormal(vertex1, vertex2, vertex3, tangent, binormal);

		// Calculate the new normal using the tangent and binormal.
		CalculateNormal(tangent, binormal, normal);

		// Store the normal, tangent, and binormal for this face back in the model structure.
		m_model[index - 1].nx = normal.x;
		m_model[index - 1].ny = normal.y;
		m_model[index - 1].nz = normal.z;
		m_model[index - 1].tx = tangent.x;
		m_model[index - 1].ty = tangent.y;
		m_model[index - 1].tz = tangent.z;
		m_model[index - 1].bx = binormal.x;
		m_model[index - 1].by = binormal.y;
		m_model[index - 1].bz = binormal.z;

		m_model[index - 2].nx = normal.x;
		m_model[index - 2].ny = normal.y;
		m_model[index - 2].nz = normal.z;
		m_model[index - 2].tx = tangent.x;
		m_model[index - 2].ty = tangent.y;
		m_model[index - 2].tz = tangent.z;
		m_model[index - 2].bx = binormal.x;
		m_model[index - 2].by = binormal.y;
		m_model[index - 2].bz = binormal.z;

		m_model[index - 3].nx = normal.x;
		m_model[index - 3].ny = normal.y;
		m_model[index - 3].nz = normal.z;
		m_model[index - 3].tx = tangent.x;
		m_model[index - 3].ty = tangent.y;
		m_model[index - 3].tz = tangent.z;
		m_model[index - 3].bx = binormal.x;
		m_model[index - 3].by = binormal.y;
		m_model[index - 3].bz = binormal.z;
	}

	return;
}

void ModelClass::CalculateTangentBinormal(TempVertexType vertex1, TempVertexType vertex2, TempVertexType vertex3,
	VectorType& tangent, VectorType& binormal)
{
	float vector1[3], vector2[3];
	float tuVector[2], tvVector[2];
	float den;
	float length;


	// Calculate the two vectors for this face.
	vector1[0] = vertex2.x - vertex1.x;
	vector1[1] = vertex2.y - vertex1.y;
	vector1[2] = vertex2.z - vertex1.z;

	vector2[0] = vertex3.x - vertex1.x;
	vector2[1] = vertex3.y - vertex1.y;
	vector2[2] = vertex3.z - vertex1.z;

	// Calculate the tu and tv texture space vectors.
	tuVector[0] = vertex2.tu - vertex1.tu;
	tvVector[0] = vertex2.tv - vertex1.tv;

	tuVector[1] = vertex3.tu - vertex1.tu;
	tvVector[1] = vertex3.tv - vertex1.tv;

	// Calculate the denominator of the tangent/binormal equation.
	den = 1.0f / (tuVector[0] * tvVector[1] - tuVector[1] * tvVector[0]);

	// Calculate the cross products and multiply by the coefficient to get the tangent and binormal.
	tangent.x = (tvVector[1] * vector1[0] - tvVector[0] * vector2[0]) * den;
	tangent.y = (tvVector[1] * vector1[1] - tvVector[0] * vector2[1]) * den;
	tangent.z = (tvVector[1] * vector1[2] - tvVector[0] * vector2[2]) * den;

	binormal.x = (tuVector[0] * vector2[0] - tuVector[1] * vector1[0]) * den;
	binormal.y = (tuVector[0] * vector2[1] - tuVector[1] * vector1[1]) * den;
	binormal.z = (tuVector[0] * vector2[2] - tuVector[1] * vector1[2]) * den;

	// Calculate the length of this normal.
	length = sqrt((tangent.x * tangent.x) + (tangent.y * tangent.y) + (tangent.z * tangent.z));

	// Normalize the normal and then store it
	tangent.x = tangent.x / length;
	tangent.y = tangent.y / length;
	tangent.z = tangent.z / length;

	// Calculate the length of this normal.
	length = sqrt((binormal.x * binormal.x) + (binormal.y * binormal.y) + (binormal.z * binormal.z));

	// Normalize the normal and then store it
	binormal.x = binormal.x / length;
	binormal.y = binormal.y / length;
	binormal.z = binormal.z / length;

	return;
}

void ModelClass::CalculateNormal(VectorType tangent, VectorType binormal, VectorType& normal)
{
	float length;


	// Calculate the cross product of the tangent and binormal which will give the normal vector.
	normal.x = (tangent.y * binormal.z) - (tangent.z * binormal.y);
	normal.y = (tangent.z * binormal.x) - (tangent.x * binormal.z);
	normal.z = (tangent.x * binormal.y) - (tangent.y * binormal.x);

	// Calculate the length of the normal.
	length = sqrt((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z));

	// Normalize the normal.
	normal.x = normal.x / length;
	normal.y = normal.y / length;
	normal.z = normal.z / length;

	return;
}
