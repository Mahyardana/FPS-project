#pragma once

#include "fontclass.h"
#include "fontshaderclass.h"

class TextClass
{
private:
	struct SentenceType
	{
		ID3D11Buffer *vertexBuffer, *indexBuffer;
		int vertexCount, indexCount, maxLength;
		float red, green, blue;
	};
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

public:
	TextClass();
	TextClass(const TextClass&);
	~TextClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, HWND, int, int);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, string, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);
	bool SetSentence(string);
	void setPosition(float, float);
	void setColor(float, float, float);

private:
	bool InitializeSentence(SentenceType**, int, ID3D11Device*);
	bool UpdateSentence(SentenceType*, char*, int, int, float, float, float, ID3D11DeviceContext*);
	void ReleaseSentence(SentenceType**);
	bool RenderSentence(ID3D11DeviceContext*, SentenceType*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX);

private:
	D3DXVECTOR3 color;
	D3DXVECTOR2 position;
	FontClass* m_Font;
	FontShaderClass* m_FontShader;
	int m_screenWidth, m_screenHeight;
	ID3D11Device* m_Device;
	ID3D11DeviceContext* m_DeviceContext;
	SentenceType* m_sentence;
};
