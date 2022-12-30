#pragma once

#include <vector>
#include "GL/glew.h"
#include <stdexcept>
#include "Renderer.h"
using namespace std;

struct  VertexBufferElement
{
	    unsigned int type;
		unsigned int count;
		unsigned char normalised;

	static	unsigned int GetSizeofType(unsigned int type) {
			
			switch (type)
			{
			case GL_FLOAT:
				return sizeof(GLfloat);
				
			case GL_UNSIGNED_INT:
				return sizeof(GLuint);

			case GL_UNSIGNED_BYTE:
				return sizeof(GLbyte);
			default:
				ASSERT(false);
				break;
			}
		}
};

class VertexBufferLayout {

private:

	vector <VertexBufferElement> m_Elements;
	unsigned int m_Stride;
public:
	VertexBufferLayout()
		:m_Stride(0)
	{
	
	}
	void Push(unsigned int type, unsigned int count);
	
	/*template<typename T>
	void Push(int count)
	{
		ASSERT(false)
	}
	template<>
	void Push<float>(int count)
	{	
		m_Elements.push_back({ GL_FLOAT,count,GL_FALSE });
		m_Stride += sizeof(GLfloat);
	}
	template<>
	void Push<unsigned int>(int count)
	{
		m_Elements.push_back({GL_UNSIGNED_INT,count,GL_FALSE });
		m_Stride += sizeof(GLuint);
	}
	template<>
	void Push<unsigned char>(int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE,count,GL_FALSE });
		m_Stride += sizeof(GLbyte);
	}*/
	inline const vector<VertexBufferElement> GetElements() const { return m_Elements; }
	inline unsigned int GetStride()const { return m_Stride; }
			
};
