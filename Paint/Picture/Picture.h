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
	IShape GetShape(const std::string &id) const;
	void MoveShape(const std::string& id, Point position)const;
	void MovePicture(Point position)const;
	void DeleteShape(const std::string& id);
	void ChangeColor(const std::string& id, Color color)const;
	void ChangeShape(const std::string& id, std::unique_ptr<IShapeBehavior>& newBehavior);
	void DrawShape(const std::string& id, ICanvas& canvas)const;
	void DrawPicture(ICanvas& canvas)const;
	void List() const;

private:
	std::vector<std::unique_ptr<IShape>> m_shapes;

	void PrintShape(IShape& shape)const;
};

#endif //OOD_PICTURE_H
