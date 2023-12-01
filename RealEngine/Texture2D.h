#ifndef TEXTURE2D_H
#define TEXTURE2D_H
#include <glm/vec2.hpp>

namespace real
{
	class Texture2D
	{
	public:
		virtual ~Texture2D() = default;

		virtual glm::ivec2 GetSize() const = 0;

		void SetScale(const float uniformScale);
		void SetScale(const float scaleX, const float scaleY);
		glm::vec2 GetScale() const { return m_Scale; }

	protected:
		glm::vec2 m_Scale{ 1, 1 };
	};
}

#endif // TEXTURE2D_H