#ifndef MENU_H
#define MENU_H

#include "Mesh.h"
#include "Entity.h"
#include "EventHandler.h"
#include "TextRenderer.h"

#include <string>

typedef enum Choice
{
	ONE_PLAYER,
	TWO_PLAYER,
	QUIT
} Choice;

typedef enum Difficulty
{
	VERY_EASY,
	EASY,
	MEDIUM,
	HARD,
	IMPOSSIBLE
} Difficulty;

struct Button
{
	std::string text;
	GLfloat width;
};

struct ComputerProperties
{
	GLfloat speed;
	GLfloat prediction;
};

class Menu
{
	public:
		static const GLfloat TEXT_PADDING;

		void init( const TextRenderer& text_renderer );
		bool isFinished() const; // If finished with menu loop

		GLfloat getButtonsHeight() const;
		void handleEvents( EventHandler& handler );
		void handleInput( const Uint8* keyboard_state );
		std::vector<Button> getButtons() const;
		ComputerProperties getCompProperties() const;
		GLuint getCurrentChoice() const;
		void overlay();

		Menu();
		~Menu();
	private:
		std::vector<Button> text_; // Text to render
		std::vector<Button> difficulties_;

		GLfloat text_height_;
		GLfloat difficulties_height_;

		std::vector<Button> buttons_;
		GLfloat button_width_;
		GLfloat button_height_;

		ComputerProperties ai_properties_;

		GLuint current_choice_;

		bool menu_exit_;
		bool submenu_;
		bool repeat_;

};

#endif