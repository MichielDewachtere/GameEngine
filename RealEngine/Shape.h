#ifndef SHAPE_H
#define SHAPE_H

#include "Component.h"

namespace real
{
	class Shape final : public Component
	{
	public:
		explicit Shape() = default;
		~Shape() = default;

		Shape(const Shape& other) = delete;
		Shape& operator=(const Shape& rhs) = delete;
		Shape(Shape&& other) = delete;
		Shape& operator=(Shape&& rhs) = delete;

	private:

	};
}

#endif // SHAPE_H