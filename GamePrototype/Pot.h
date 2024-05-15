#pragma once
#include "Texture.h"
#include "Shape.h"

#include <vector>

// The pot class will contain information about all the combined elements
// The pot may only contain base elements such as
class Pot
{
public:
  // The state the pot is in
  enum class State
  {
    View,
    Craft, // When the pot is crafting
    Show,
    Select, // When the user may select a recipe
  };

  Pot(const Rectf& rectangle, int ingredients, int variations, float viewingTime, float craftingTime, float showingTime, float selectionTime);
  ~Pot();

  void Draw(float screenWidth) const;
  bool Update(float elapsedSec); // Returns true if update was reset

  bool SelectShape(int id);

  int GetShapeId(float screenWidth, const Point2f& position) const;
  Rectf GetRect() const;
  State GetState() const;

  int GetIngredients() const;
  int GetVariations() const;
  float GetViewingTime() const;

  void SetIngredients(int ingredients);
  void SetVariations(int variations);
  void SetViewingTime(float viewingTime);
private:
  void Flush();

  std::vector<Shape*> GetRandomShapes(int size) const;
  Shape* GetRandomShape() const;
  void GenerateRecipe();

  void DrawResult(int id, const Point2f& position) const;

  State m_State;

  const Texture* m_Texture;
  const Rectf m_Rectangle;

  Rectf m_InsertionRect;

  std::vector<std::vector<Shape*>> m_Craftables; // The shapes the user can choose from to craft
  std::vector<Shape*> m_Shapes;     // The shapes that have been added to the pot
  int m_Result;

  int m_Ingredients; // The amount of ingredients to use
  int m_Variations; // The amount of recipes the user can choose from
  float m_ShowingTime;
  float m_SelectionTime;
  float m_CraftingTime;
  float m_ViewingTime;

  float m_Time;
  float m_SineTime;
};

