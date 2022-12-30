#pragma once
#include <GL/glew.h>
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
using namespace std;

class VertexArray {

private:

	unsigned int m_RendererID;
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void Bind() const;
	void Unbind() const;
};