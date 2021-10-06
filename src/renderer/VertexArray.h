#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include "VertexBuffer.h"
#include "BufferLayout.h"

#include <GL/glew.h>

class VertexArray
{
	public:
		/// <summary>
		/// Generates vertex array
		/// </summary>
		void init();
		/// <summary>
		/// Deletes vertex array
		/// </summary>
		void shutDown();

		/// <summary>
		/// Attach vertex array object to buffer 
		/// </summary>
		/// <param name="vb"></param>
		/// <param name="layout"></param>
		void addBuffer( const VertexBuffer& vb, const BufferLayout& layout );

		/// <summary>
		/// Bind vertex array
		/// </summary>
		void bind() const;
		/// <summary>
		/// Unbind vertex array
		/// </summary>
		void unbind() const;

		VertexArray();
		~VertexArray();
	private:
		/// <summary>
		/// Array id
		/// </summary>
		GLuint renderer_id_;
};

#endif // !VERTEX_ARRAY_H
