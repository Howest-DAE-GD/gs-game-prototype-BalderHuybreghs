#include "pch.h"
#include "Game.h"
#include "utils.h"

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

  m_ScoreTexture = new Texture(std::to_string(m_Score), "fonts/roboto.ttf", 32, Color4f{ 1.f, 1.f, 1.f, 1.f });
}

void Game::Cleanup( )
{
  delete m_Pot;
  delete m_ScoreTexture;
}

void Game::Update( float elapsedSec )
{
  if (m_Pot->Update(elapsedSec)) {
    m_Pollution++;
  }
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

  SetColor(Color4f(1.f, 0.f, 0.f, m_Pollution / float(m_MaxPollution)));
  FillRect(pollutionRect);

  m_ScoreTexture->Draw(Point2f{ 10.f, 10.f });
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

  if (!m_Pot->SelectShape(id)) {
    m_Pollution++;
    return;
  }

  m_Score += 100;

  if (m_Score % 400 == 0) {
    m_Pot->SetIngredients((m_Pot->GetIngredients() + 1) & 14);
    m_Pot->SetVariations((m_Pot->GetVariations() + 1) % 14);
    m_Pot->SetViewingTime(m_Pot->GetViewingTime() * 0.9);
  }

  delete m_ScoreTexture;
  m_ScoreTexture = new Texture(std::to_string(m_Score), "fonts/roboto.ttf", 32, Color4f{ 1.f, 1.f, 1.f, 1.f });
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}
