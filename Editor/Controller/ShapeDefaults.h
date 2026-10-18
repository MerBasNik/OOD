//
// Created by Вадим Патрушев on 18.10.2026.
//

#ifndef OOD_EDITOR_SHAPEDEFAULTS_H
#define OOD_EDITOR_SHAPEDEFAULTS_H

#include "Model/Types/Color.h"
#include "Model/Types/Frame.h"

class ShapeDefaults
{
public:
	static constexpr double MIN_SIZE = 10;
	static constexpr double WIDTH = 160;
	static constexpr double HEIGHT = 120;

	static constexpr Color RECTANGLE_COLOR = 0x1111ee;
	static constexpr Color ELLIPSE_COLOR = 0x11ee11;
	static constexpr Color TRIANGLE_COLOR = 0xee1111;

	static Frame GetDefaultFrame(const Frame& bounds);
};

#endif // OOD_EDITOR_SHAPEDEFAULTS_H
