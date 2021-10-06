#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include "Mesh.h"
#include "GLWindow.h"
#include "renderer/BufferLayout.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>

struct Character
{
	GLuint id;
	glm::vec2 size;
	glm::vec2 bearing;
	FT_Pos offset;
	std::vector<TexVertex> vertices;
};

class TextRenderer
{
	public:
		bool init( const glm::mat4& projection );
		void shutDown();

		bool loadTTF( const std::string& font, GLuint size );
		void renderText( const std::string& text, GLfloat x, GLfloat y, GLfloat scale, const glm::vec4& color, bool advance_left );
		GLfloat getTextWidth( const std::string& text ) const;
		GLfloat getFontHeight() const;

		TextRenderer();
		~TextRenderer();
	private:
		std::unordered_map<GLchar, Character> characters_;
		Shader shader_;
		Mesh<TexVertex> mesh_;
		BufferLayout layout_;
		glm::mat4 projection_;

		GLfloat height_;

		FT_Library ft_;
};


#endif // !TEXT_RENDERER_H
