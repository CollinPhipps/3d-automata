#include "VertexArray.h"

#include "VertexBufferLayout.h"
#include "Renderer.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout, unsigned int startingIndex)
{
	bind();
	vb.bind();
	const auto& elements = layout.getElements();
	unsigned int offset = 0;
	unsigned int currentAttribIndex = 0;
	for (unsigned int i = 0; i < elements.size(); ++i)
	{
		const auto& element = elements[i];
		unsigned int attribIndex = startingIndex + i;
		GLCall(glEnableVertexAttribArray(attribIndex));
		GLCall(glVertexAttribPointer(attribIndex, element.count, element.type, element.normalized, layout.getStride(), (const void*)offset));

		if (element.divisor > 0)
			GLCall(glVertexAttribDivisor(attribIndex, element.divisor));

		offset += element.count * VertexBufferElement::getSizeOfType(element.type);
	}
}

void VertexArray::bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::unbind() const
{
	GLCall(glBindVertexArray(0));
}
