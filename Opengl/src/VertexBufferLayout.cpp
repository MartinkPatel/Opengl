#include "VertexBufferLayout.h"

void VertexBufferLayout::Push(unsigned int type, unsigned int count)
{

	m_Elements.push_back({ type,count,GL_FALSE });
	m_Stride += count * VertexBufferElement::GetSizeofType(type);

}
