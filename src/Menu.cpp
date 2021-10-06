#include "Menu.h"

const GLfloat Menu::TEXT_PADDING = 20.0f;

void Menu::init( const TextRenderer& text_renderer )
{
	current_choice_ = 0;
	text_height_ = 0.0f;
	difficulties_height_ = 0.0f;

	std::vector<std::string> text = { "One Player","Two Players","Quit" };
	std::vector<std::string> difficulties = { "Very Easy", "Easy", "Medium", "Hard", "Impossible" };

	for( const auto& str : text )
	{
		text_.push_back( { str, text_renderer.getTextWidth( str ) } );
		text_height_ += text_renderer.getFontHeight();
	}
	for( const auto& dif : difficulties )
	{
		difficulties_.push_back( { dif, text_renderer.getTextWidth( dif ) } );
		difficulties_height_ += text_renderer.getFontHeight();
	}

	ai_properties_ = { 0.0f, 1.0f };
	buttons_ = text_;
	button_height_ = text_height_;

	repeat_ = false;
	menu_exit_ = false;
	submenu_ = false;
}

GLfloat Menu::getButtonsHeight() const
{
	return button_height_;
}
std::vector<Button> Menu::getButtons() const
{
	return buttons_;
}

GLuint Menu::getCurrentChoice() const
{
	return current_choice_;
}

void Menu::overlay()
{
	menu_exit_ = false;
	submenu_ = false;
	buttons_ = text_;
	button_height_ = text_height_;
	current_choice_ = 0;
	ai_properties_ = { 0.0f, 1.0f };
}

bool Menu::isFinished() const
{
	return menu_exit_;
}

ComputerProperties Menu::getCompProperties() const
{
	return ai_properties_;
}

void Menu::handleEvents( EventHandler& handler )
{
	handler.registerEvent( SDL_KEYDOWN, [ this ] ( const SDL_Event& e ) {
		if( e.key.repeat == 0 )
			repeat_ = false;
	} );
}

void Menu::handleInput( const Uint8* keyboard_state )
{
	if( keyboard_state[ SDL_SCANCODE_UP ] && !repeat_ )
	{
		if( current_choice_ > 0 )
			current_choice_--;
		repeat_ = true;
	}
	else if( keyboard_state[ SDL_SCANCODE_DOWN ] && !repeat_ )
	{
		if( current_choice_ < buttons_.size() - 1 )
			current_choice_++;
		repeat_ = true;
	}
	else if( keyboard_state[ SDL_SCANCODE_RETURN ] && !repeat_ )
	{
		repeat_ = true;
		if( submenu_ ) // If Difficulties menu is currently displayed
		{
			switch( current_choice_ )
			{
				case ( VERY_EASY ): // AI is Braindead and slow
				{
					ai_properties_ = { 1.0f, 1.0f };
					break;
				}
				case( EASY ): // AI is slightly smarter and Faster
				{
					ai_properties_ = { 1.5f, 1.10f };
					break;
				}
				case( MEDIUM ): // AI is faster
				{
					ai_properties_ = { 2.0f, 1.20f };
					break;
				}
				case( HARD ): // AI is Smarter
				{
					ai_properties_ = { 4.0f, 1.30f };
					break;
				}
				case( IMPOSSIBLE ): // AI is Fast and Smart almost Impossible to win
				{
					ai_properties_ = { Entity::PLAYER_SPEED, 1.40f };
					break;
				}
			}
			menu_exit_ = true;
		}
		else
		{
			switch( current_choice_ )
			{
				case ( ONE_PLAYER ):
				{
					submenu_ = true; 
					buttons_ = difficulties_;
					button_height_ = difficulties_height_;
					current_choice_ = difficulties_.size() / 2;
					break;
				}
				case ( TWO_PLAYER ):
				{
					menu_exit_ = true;
					break;
				}
				default: // Quit
				{
					SDL_Event e;
					e.type = SDL_QUIT;
					SDL_PushEvent( &e );
					break;
				}

			}
		}
	}
}

Menu::Menu() {}
Menu::~Menu() {}