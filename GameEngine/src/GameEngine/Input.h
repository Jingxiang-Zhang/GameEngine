#pragma once
#include "GameEngine/Core.h"
#include "GameEngine/KeyCodes.h"
#include "GameEngine/MouseCodes.h"
// #include <glm/glm.hpp>

namespace GE {

	class GE_API Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);
		static bool IsMouseButtonPressed(MouseCode button);
		// static glm::vec2 GetMousePosition();
		static std::pair <float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();

	protected:
		virtual bool IsKeyPressedImpl(KeyCode key) = 0;

	private:
		static Input* s_Instance;
	};
}
