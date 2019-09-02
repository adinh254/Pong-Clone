#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <GL/glew.h>
#include <vector>

class VertexBuffer
{
	public:
		/// <summary>
		/// Generate vertex buffer, binds buffer, and initialize buffer data
		/// </summary>
		/// <param name="data"></param>
		/// <param name="size"></param>
		void init( const void *data, GLuint size, GLenum usage );
		/// <summary>
		/// Delete buffer
		/// </summary>
		void shutDown();

		void setData( const void *data, size_t size, GLenum usage );

		/// <summary>
		/// Manual binding of buffer
		/// </summary>
		void bind() const;
		/// <summary>
		/// Manual unbinding of buffer
		/// </summary>
		void unbind() const;

		VertexBuffer();
		~VertexBuffer();

	private:
		/// <summary>
		/// Buffer id
		/// </summary>
		GLuint renderer_id_;
};

#endif