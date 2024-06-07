#pragma once
#include "BaseGame.h"
#include "Pot.h"
#include "Ability.h"
#include "GameState.h"

#include <vector>

class Ability;

class Game : public BaseGame
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	// http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-override
	~Game();

	void Update( float elapsedSec ) override;
	void Draw( ) const override;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e ) override;
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e ) override;
  void ProcessMouseUpEvent(const SDL_MouseButtonEvent& e) override;
private:

	// FUNCTIONS
	void Initialize();
	void Cleanup( );
	void ClearBackground( ) const;

  // VARIABLES
  GameState m_GameState{};
  Point2f m_MousePos{};
  Ability* m_AbilityHover{ nullptr };

  float m_DamageSecs;
  float m_TotalDamageSecs;

  // Text textures
  Texture* m_ScoreTexture;
  Texture* m_LevelTexture;
  Texture* m_HealthTexture;

  Pot* m_Pot;

  std::vector<Ability*> m_ActiveAbilities;
};