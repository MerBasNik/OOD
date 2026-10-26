//
// Created by Вадим Патрушев on 26.10.2026.
//

#ifndef OOD_EDITOR_SHAPESERIALIZER_H
#define OOD_EDITOR_SHAPESERIALIZER_H

#include "Model/Shapes/IShape.h"
#include <ostream>
#include <string>

class ShapeSerializer : public IShapeVisitor
{
public:
	static inline const std::string RECTANGLE_NAME = "rectangle";
	static inline const std::string ELLIPSE_NAME = "ellipse";
	static inline const std::string TRIANGLE_NAME = "triangle";

	explicit ShapeSerializer(std::ostream& output);
	void Visit(const Rectangle& rectangle) override;
	void Visit(const Ellipse& ellipse) override;
	void Visit(const Triangle& triangle) override;

private:
	std::ostream& m_output;

	void Write(const std::string& typeName, const IShape& shape);
};

#endif // OOD_EDITOR_SHAPESERIALIZER_H
