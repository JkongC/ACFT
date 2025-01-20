#include "stdafx.h"

#include "ACFT.h"
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"
#include "VertexArray.h"

namespace ACFT
{
	VertexArray::VertexArray()
	{
		GLCall(glGenVertexArrays(1, &va_id));
	}

	void VertexArray::Bind() const
	{
		GLCall(glBindVertexArray(va_id));
	}

	void VertexArray::Unbind() const
	{
		GLCall(glBindVertexArray(0));
	}
}


