#include "TextRenderer.h"

bool TextRenderer::init( const glm::mat4& projection )
{
	projection_ = projection;

	// Position of text
	layout_.push<GLfloat>( 2 );
	// Texture coordinates
	layout_.push<GLfloat>( 2 );

	if( FT_Init_FreeType( &ft_ ) )
	{
		SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Unable to initialize Freetype Library!\n" );
		return false;
	}

	if( !shader_.init( "./shaders/text.vertex", "./shaders/text.fragment" ) )
	{
		SDL_LogWarn( SDL_LOG_CATEGORY_APPLICATION, "Unable to initialize Text Shaders!\n" );
		return false;
	}

	return true;
}

bool TextRenderer::loadTTF( const std::string& font, GLuint size )
{
	// Clear old font set
	characters_.clear();

	FT_Face face;
	if( FT_New_Face( ft_, font.c_str(), 0, &face ) )
	{
		SDL_LogError( SDL_LOG_CATEGORY_APPLICATION, "Unable to load font!\n" );
		return false;
	}

	// Set size of glyphs
	FT_Set_Pixel_Sizes( face, 0, size );

	// Disable byte-alignment to one byte per pixel since FT loads characters in one channel grayscale
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	for( GLubyte ch = 0; ch < 128; ch++ )
	{
		if( FT_Load_Char( face, ch, FT_LOAD_RENDER ) ) continue; // If char doesn't exist

		// Generate texture
		GLuint texture;
		glGenTextures( 1, &texture );
		glBindTexture( GL_TEXTURE_2D, texture );
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_R8,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);

		// Set Texture options
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

		// Store character
		Character chara = {
			texture,
			glm::vec2( face->glyph->bitmap.width, face->glyph->bitmap.rows ),
			glm::vec2( face->glyph->bitmap_left, face->glyph->bitmap_top ),
			face->glyph->advance.x,
		};

		GLfloat width = chara.size.x;
		GLfloat height = chara.size.y;
		height_ = height;

		GLfloat x_pos = chara.bearing.x;
		GLfloat y_pos = height - chara.bearing.y; // For characters rendered below baseline https://learnopengl.com/In-Practice/Text-Rendering

		chara.vertices = {
			{ glm::vec2( x_pos,			y_pos ), glm::vec2( 0.0f,  0.0f ) }, // Top Left
			{ glm::vec2( x_pos + width, y_pos ), glm::vec2( 1.0f, 0.0f ) }, // Top right
			{ glm::vec2( x_pos + width, y_pos + height ), glm::vec2( 1.0f, 1.0f) }, // Bottom right
			{ glm::vec2( x_pos,			y_pos + height ), glm::vec2( 0.0f, 1.0f) }  // Bottom left
		};
		characters_.insert( std::pair<GLubyte, Character>(ch, chara) );
	}

	std::vector<GLuint> indices = { 0, 1, 2, 2, 3, 0 };

	// Initialize first number as a mesh
	mesh_.init( characters_[ '0' ].vertices, indices, layout_, GL_DYNAMIC_DRAW );
	mesh_.addModel( glm::mat4( 1.0f ) );

	// Unbind Texture
	glBindTexture( GL_TEXTURE_2D, 0 );

	// Destroy Freetype
	FT_Done_Face( face );
	return true;
}

GLfloat TextRenderer::getTextWidth( const std::string& text ) const
{
	GLfloat size = 0;
	Character chara;
	for( size_t i = 0; i < text.size() - 1; i++ )
	{
		chara = characters_.at( text[i] );
		size += ( chara.offset >> 6 );
	}
	size += ( chara.offset >> 6 ) / 2.0f; // Last offset is divided by 2
	return size;
}

GLfloat TextRenderer::getFontHeight() const
{
	return height_;
}

void TextRenderer::renderText( const std::string& text, GLfloat x, GLfloat y, GLfloat scale, const glm::vec4& color, bool advance_left )
{
	GLfloat x_offset = x;
	glm::mat4 font_scale = glm::scale( glm::mat4( 1.0f ), glm::vec3( scale, scale, 0.0f ) );

	shader_.bind();

	// Set text color
	shader_.setUniform4f( "text_color", color );
	glActiveTexture( GL_TEXTURE0 );

	auto lambda = [&]( const Character& chara )
	{
		glm::mat4 translation = glm::translate( glm::mat4( 1.0f ), glm::vec3( x_offset, y, 0.0f ) );
		glm::mat4 model = translation * font_scale;
		glm::mat4 mvp = projection_ * model;

		shader_.setUniformMat4f( "mvp", mvp );
		glBindTexture( GL_TEXTURE_2D, chara.id );
		mesh_.setVertices( chara.vertices, 0 );

		mesh_.render();

		if( advance_left )
			x_offset -= ( chara.offset >> 6 ) * scale;
		else
			x_offset += ( chara.offset >> 6 ) * scale;
	};

	if( advance_left )
		for( auto it = text.rbegin(); it != text.rend(); it++ )
			lambda( characters_[ *it ] );
	else
		for( const auto& ch : text )
			lambda( characters_[ ch ] );


	glBindTexture( GL_TEXTURE_2D, 0 );
}

void TextRenderer::shutDown()
{
	FT_Done_FreeType( ft_ );
	mesh_.shutDown();
	shader_.shutDown();
}

TextRenderer::TextRenderer() {}
TextRenderer::~TextRenderer() {}