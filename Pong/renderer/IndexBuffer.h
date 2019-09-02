#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

#include <GL/glew.h>

class IndexBuffer
{
	public:
		/// <summary>
		/// Generate indices, binds buffer, and initialize buffer data
		/// </summary>
		/// <param name="data"></param>
		/// <param name="count"></param>
		void init( const void *index_data, GLuint count );
		/// <summary>
		/// Deletes buffer
		/// </summary>
		void shutDown() const;

		/// <summary>
		/// Return count
		/// </summary>
		/// <returns></returns>
		GLuint getCount() const;

		/// <summary>
		/// Bind indices
		/// </summary>
		void bind() const;
		/// <summary>
		/// Unbind indices
		/// </summary>
		void unbind() const;

		IndexBuffer();
		~IndexBuffer();

	private:
		/// <summary>
		/// Buffer id
		/// </summary>
		GLuint renderer_id_;
		/// <summary>
		/// Number of elements
		/// </summary>
		GLuint count_;
};

#endif