#include "pch.h"
#include "Pot.h"
#include "MathUtils.h"
#include "RectangleShape.h"
#include "CircleShape.h"
#include "utils.h"

Pot::Pot(const Rectf& rectangle, int ingredients, int variations, float viewingTime, float craftingTime, float showingTime, float selectionTime)
  : m_State(State::View), m_Rectangle(rectangle), m_Ingredients(ingredients), m_Variations(variations), m_ViewingTime(viewingTime), m_ShowingTime(showingTime), m_SelectionTime(selectionTime), m_CraftingTime(craftingTime), m_SineTime(0), m_Result(0)
{
  m_Texture = new Texture("textures/pot.png");

  m_Craftables.reserve(m_Variations);
  m_Shapes.reserve(ingredients);

  GenerateRecipe();
}

Pot::~Pot()
{
  delete m_Texture;

  Flush();
}

void Pot::Draw(float screenWidth) const
{
  switch (m_State) {
    case State::View: {
      float x{
        (screenWidth / 2.f) - ((60.f * (m_Shapes.size() - 1)) / 2.f)
      };

      for (size_t i = 0; i < m_Shapes.size(); i++)
      {
        m_Shapes[i]->Draw(
          Point2f{
            x + 60.f * i,
            m_Rectangle.bottom + m_Rectangle.height + 100.f + MathUtils::Wave(10.f, 5.f, 1.f, 0.f, m_SineTime)
          });
      }

      break;
    }

    case State::Craft:
    {
      float x{
        (screenWidth / 2.f) - ((60.f * (m_Shapes.size() - 1)) / 2.f)
      };

      float timePerShape{ (m_CraftingTime / m_Shapes.size()) };
      float percentage{ m_Time / m_CraftingTime };

      int shapesToHide{ int(percentage * m_Shapes.size()) };
      float lerpProgress{ (m_Time - shapesToHide * timePerShape) / timePerShape };

      for (int i = m_Shapes.size() -1; i >= shapesToHide; i--)
      {
        const Point2f pos{
            x + 60.f * i,
            m_Rectangle.bottom + m_Rectangle.height + 100.f + MathUtils::Wave(10.f, 5.f, 1.f, 0.f, m_SineTime)
        };

        if (i == shapesToHide) {
          m_Shapes[i]->Draw(MathUtils::Lerp(pos, m_Rectangle.Center(), lerpProgress));
          continue;
        }

        m_Shapes[i]->Draw(pos);
      }

      break;
    }

    case State::Show:
    {
      float x{
        (screenWidth / 2.f) - ((60.f * (m_Shapes.size() - 1)) / 2.f)
      };

      float timePerShape{ m_ShowingTime / m_Craftables.size() };
      float percentage{ m_Time / m_ShowingTime };

      int shapesToShow{ int(percentage * m_Craftables.size()) };
      float lerpProgress{ (m_Time - shapesToShow * timePerShape) / timePerShape };

      for (int i = 0; i < shapesToShow; i++)
      {
        const Point2f pos{
            x + 60.f * i,
            m_Rectangle.bottom + m_Rectangle.height + 100.f + MathUtils::Wave(10.f, 5.f, 1.f, 0.f, m_SineTime)
        };

        if (i == shapesToShow - 1) {
          DrawResult(i, MathUtils::Lerp(m_Rectangle.Center(), pos, lerpProgress));
          continue;
        }

        DrawResult(i, pos);
      }

      break;
    }

    case State::Select:
    {
      float x{
        (screenWidth / 2.f) - ((60.f * (m_Shapes.size() - 1)) / 2.f)
      };

      for (int i = 0; i < m_Craftables.size(); i++)
      {
        const Point2f pos{
            x + 60.f * i,
            m_Rectangle.bottom + m_Rectangle.height + 100.f + MathUtils::Wave(10.f, 5.f, 1.f, 0.f, m_SineTime)
        };

        DrawResult(i, pos);
      }

      break;
    }
  }

  m_Texture->Draw(m_Rectangle);
}

bool Pot::Update(float elapsedSec)
{
  bool val{};

  switch (m_State) {
    case State::View:
    {
      if (m_Time > m_ViewingTime) {
        m_Time = 0;
        m_State = State::Craft;
      }

      break;
    }

    case State::Craft:
    {
      if (m_Time > m_CraftingTime) {
        m_Time = 0;
        m_State = State::Show;
      }

      break;
    }

    case State::Show:
    {
      if (m_Time > m_ShowingTime) {
        m_Time = 0;
        m_State = State::Select;
      }

      break;
    }
    case State::Select:
    {

      if (m_Time > m_SelectionTime) {
        m_Time = 0;
        m_State = State::View;
        GenerateRecipe();
        val = true;
      }

      break;
    }
  }

  m_Time += elapsedSec;
  m_SineTime += elapsedSec;
  return val;
}

void Pot::GenerateRecipe()
{
  Flush(); // Flush the previous recipe

  std::vector<Shape*> shapes{ GetRandomShapes(m_Ingredients) };
  std::vector<std::vector<Shape*>> craftables{ };
  craftables.reserve(m_Variations);

  m_Result = MathUtils::RandInt(0, m_Variations -1);

  for (int i = 0; i < m_Variations; i++)
  {
    if (i != m_Result) {
      craftables.push_back(GetRandomShapes(m_Ingredients));
      continue;
    }

    std::vector<Shape*> copiedShapes{};
    copiedShapes.reserve(shapes.size());
    for (const Shape* shapePtr : shapes) {
      copiedShapes.push_back(shapePtr->Copy());
    }

    craftables.push_back(copiedShapes);
  }

  m_Shapes = shapes;
  m_Craftables = craftables;
}

void Pot::Flush()
{
  m_State = State::View;
  m_Time = 0.f;
  m_SineTime = 0.f;
  m_Result = 0;

  for (Shape* shape : m_Shapes) {
    delete shape;
    shape = nullptr;
  }

  m_Shapes.clear();

  for (std::vector<Shape*> craftable : m_Craftables) {
    for (Shape* shape : craftable) {
      delete shape;
      shape = nullptr;
    }
  }

  m_Craftables.clear();
}

std::vector<Shape*> Pot::GetRandomShapes(int size) const
{
  std::vector<Shape*> shapes{};
  shapes.reserve(size);

  for (size_t i = 0; i < size; i++)
  {
    shapes.push_back(GetRandomShape());
  }

  return shapes;
}

Shape* Pot::GetRandomShape() const
{
  bool filled{ MathUtils::RandBool() };
  Color4f color{ MathUtils::RandColor(2) };
  int shape{ MathUtils::RandInt(0, 1) };

  switch (shape) {
    case 0:
    {
      const Point2f size{ MathUtils::RandPoint(10.f, 50.f, 2) };

      return new RectangleShape(
        size,
        color,
        filled
      );
    }

    default:
    {
      const float radius{ MathUtils::RandFloat(5.f, 25.f, 2) };

      return new CircleShape(
        radius,
        color,
        filled
      );
    }
  }
}

void Pot::DrawResult(int id, const Point2f& position) const
{
  const std::vector<Shape*> result{ m_Craftables[id] };
  for (const Shape* shape : result) {
    shape->Draw(position);
  }
}

Rectf Pot::GetRect() const
{
  return m_Rectangle;
}

Pot::State Pot::GetState() const
{
  return m_State;
}

int Pot::GetIngredients() const
{
  return m_Ingredients;
}

int Pot::GetVariations() const
{
  return m_Variations;
}

float Pot::GetViewingTime() const
{
  return m_ViewingTime;
}

void Pot::SetIngredients(int ingredients)
{
  m_Ingredients = ingredients;
}

void Pot::SetVariations(int variations)
{
  m_Variations = variations;
}

void Pot::SetViewingTime(float viewingTime)
{
  m_ViewingTime = viewingTime;
}

int Pot::GetShapeId(float screenWidth, const Point2f& position) const
{
  float x{
    (screenWidth / 2.f) - ((60.f * (m_Shapes.size() - 1)) / 2.f)
  };

  // Check which shape was selected
  for (int i = 0; i < m_Craftables.size(); i++)
  {
    const Rectf area{
      (x + 60.f * i) - 30.f,
      m_Rectangle.bottom + m_Rectangle.height + 100.f - 30.f,
      60.f,
      60.f
    };

    if (utils::IsPointInRect(position, area)) {
      return i;
    }
  }

  return -1;
}

bool Pot::SelectShape(int id)
{
  bool res{ id == m_Result };
  GenerateRecipe();
  return res;
}