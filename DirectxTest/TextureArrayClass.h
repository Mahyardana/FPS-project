#pragma once

#include <d3d11.h>
#include <d3dx11tex.h>

class TextureArrayClass
{
public:
	TextureArrayClass();
	TextureArrayClass(const TextureArrayClass&);
	~TextureArrayClass();

	bool Initialize(ID3D11Device*, char*, char*);
	bool Initialize(ID3D11Device * device, char * filename1, char * filename2, char * filename3);
	void Shutdown();

	ID3D11ShaderResourceView** GetTextureArray();

private:

	ID3D11ShaderResourceView* m_textures[3];
};