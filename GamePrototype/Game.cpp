#include "pch.h"
#include "Game.h"
#include "utils.h"

#include <iostream>

using namespace utils;

Game::Game( const Window& window ) 
	:BaseGame{ window }
{
	Initialize();
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
  const Rectf center{
    GetViewPort().width / 2.f - 300.f / 2.f,
    0.f,
    300.f,
    300.f
  };

  const float itemSize{ 75.f };
  const Point2f top{ 0.f, GetViewPort().height - itemSize };

  m_Pollution = 0;
  m_Pot = new Pot(center, 3, 3, 5.f, .5f, .5f, 5.f);
  //m_Pot = new Pot(center, 14, 14, .1f, .1f, .1f, .1f);
  m_Score = 0;
  m_Level = 1; // Start at level 1

  m_TotalDamageSecs = 1.5f;
  m_DamageSecs = 0.f;

  m_ScoreTexture = new Texture(std::to_string(m_Score), "fonts/roboto.ttf", 32, Color4f{ 1.f, 1.f, 1.f, 1.f });
  m_LevelTexture = new Texture("Level: " + std::to_string(m_Level), "fonts/roboto.ttf", 32, Color4f{ 1.f, 1.f, 1.f, 1.f });
  m_HealthTexture = new Texture("Health: " + std::to_string(int((1 - (m_Pollution / float(m_MaxPollution))) * 100)) + "%", "fonts/roboto.ttf", 32, Color4f{ 1.f, 1.f, 1.f, 1.f });
}

void Game::Cleanup( )
{
  delete m_Pot;
  delete m_LevelTexture;
  delete m_ScoreTexture;
  delete m_HealthTexture;
}

void Game::Update( float elapsedSec )
{
  if (m_Pot->Update(elapsedSec)) {
    m_Pollution++;
    m_DamageSecs = m_TotalDamageSecs;
    m_HealthTexture = new Texture("Health: " + std::to_string(int((1 - (m_Pollution / float(m_MaxPollution))) * 100)) + "%", "fonts/roboto.ttf", 32, Color4f{ 1.f, 1.f, 1.f, 1.f });
  }

  if (m_Pollution >= m_MaxPollution) {
    std::cout << "Score: " << m_Score << std::endl;

    Cleanup();
    Initialize();
    return;
  }

  m_DamageSecs = std::max(0.f, m_DamageSecs - elapsedSec);
}

void Game::Draw( ) const
{
	ClearBackground( );

  m_Pot->Draw(GetViewPort().width);

  // Draw pollution
  Rectf pollutionRect{
    0.f,
    0.f,
    GetViewPort().width,
    GetViewPort().height
  };

  SetColor(Color4f(.8f, 0.2f, 0.2f, m_DamageSecs / m_TotalDamageSecs));
  FillRect(pollutionRect);

  m_ScoreTexture->Draw(Point2f{ GetViewPort().left + GetViewPort().width - 10.f - m_ScoreTexture->GetWidth(), GetViewPort().bottom + GetViewPort().height - 10.f - m_ScoreTexture->GetHeight()});
  m_LevelTexture->Draw(Point2f( 10.f, GetViewPort().height - 10.f - m_LevelTexture->GetHeight() ));
  m_HealthTexture->Draw(Point2f{ 10.f, 10.f });
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	//std::cout << "KEYUP event: " << e.keysym.sym << std::endl;
	switch ( e.keysym.sym )
	{
	case SDLK_SPACE:
		break;
	}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;

}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{

}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
  if (m_Pot->GetState() != Pot::State::Select) {
    return;
  }

  int id{ m_Pot->GetShapeId(GetViewPort().width, Point2f{ float(e.x), float(e.y) }) };
  if (id < 0) {
    return;
  }

  float time{ m_Pot->GetTime() };

  if (!m_Pot->IsSelected(id)) {
    m_Pollution++;
    m_DamageSecs = m_TotalDamageSecs;
    m_HealthTexture = new Texture("Health: " + std::to_string(int((1 - (m_Pollution / float(m_MaxPollution))) * 100)) + "%", "fonts/roboto.ttf", 32, Color4f{ 1.f, 1.f, 1.f, 1.f });
    m_Pot->GenerateRecipe();
    return;
  }

  m_Score += int(100 * (1 - (time / 5.f)));

  if (m_Score - (500 * (m_Level -1)) > 500) {
    m_Pot->SetIngredients(std::min(14, m_Pot->GetIngredients() + 1));
    m_Pot->SetVariations(std::min(14, m_Pot->GetVariations() + 1));
    m_Pot->SetViewingTime(m_Pot->GetViewingTime() * 0.9);
    m_Level++;

    delete m_LevelTexture;
    m_LevelTexture = new Texture("Level: " + std::to_string(m_Level), "fonts/roboto.ttf", 32, Color4f{1.f, 1.f, 1.f, 1.f});
  }

  delete m_ScoreTexture;
  m_ScoreTexture = new Texture(std::to_string(m_Score), "fonts/roboto.ttf", 32, Color4f{ 1.f, 1.f, 1.f, 1.f });

  m_Pot->GenerateRecipe();
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}
