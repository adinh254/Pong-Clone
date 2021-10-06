#ifndef BUFFER_LAYOUT_H
#define BUFFER_LAYOUT_H

#include <GL/glew.h>

#include <vector>

/// <summary>
/// count: counts the number of vertices in the object
/// type: Labels the vertices type
/// normalized: Boolean to determine whether to convert to floating point
/// </summary>
struct BufferElement
{
	GLuint count;
	GLuint type;
	GLboolean normalized;
};

class BufferLayout
{
	public:
		/// <summary>
		/// Push back a buffer element
		/// </summary>
		template<typename T>
		void push( GLuint count );

		/// <summary>
		/// Return number of elements in vector
		/// </summary>
		/// <returns></returns>
		const std::vector<BufferElement>& getElements() const;
		/// <summary>
		/// Return stride of elements in vector
		/// </summary>
		/// <returns></returns>
		const GLsizei getStride() const;

		BufferLayout();
		~BufferLayout();

	private:
		/// <summary>
		/// Vector of buffer elements
		/// </summary>
		std::vector<BufferElement> elements_;
		/// <summary>
		/// Stride of each element
		/// </summary>
		GLsizei stride_;
};

#endif