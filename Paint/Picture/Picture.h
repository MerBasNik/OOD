//
// Created by Вадим Патрушев on 28.09.2026.
//

#ifndef OOD_PICTURE_H
#define OOD_PICTURE_H
#include "IShape.h"
#include <vector>

class Picture
{
public:
	Picture();
	void AddShape(std::unique_ptr<IShape> shape);
	IShape GetShape(std::string id);
	void MoveShape(std::string id, Point position);
	void MovePicture(Point position);
	void DeleteShape(std::string id);
	void ChangeColor(std::string id, Color color);
	void ChangeShape(std::string id, std::unique_ptr<IShapeBehavior> newBehavior);
	void DrawShape(std::string id, ICanvas& canvas);
	void DrawPicture(ICanvas& canvas);
	void List();

private:
	std::vector<std::unique_ptr<IShape>> m_shapes;
};

#endif //OOD_PICTURE_H
