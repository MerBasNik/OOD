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
	static inline const std::string GROUP_NAME = "group";
	static inline const std::string GROUP_END_NAME = "end";

	explicit ShapeSerializer(std::ostream& output);
	void Visit(const Rectangle& rectangle) override;
	void Visit(const Ellipse& ellipse) override;
	void Visit(const Triangle& triangle) override;
	void Visit(const GroupShape& group) override;

private:
	std::ostream& m_output;
	size_t m_depth = 0;

	void Write(const std::string& typeName, const IShape& shape);
	void WriteIndent();
};

#endif // OOD_EDITOR_SHAPESERIALIZER_H
