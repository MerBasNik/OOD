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
	IShape* GetShape(const std::string &id);
	void MoveShape(const std::string& id, Point position);
	void MovePicture(Point position);
	void DeleteShape(const std::string& id);
	void ChangeColor(const std::string& id, Color color);
	void ChangeShape(const std::string& id, std::unique_ptr<IShapeBehavior>& newBehavior);
	void DrawShape(const std::string& id, ICanvas& canvas )const;
	void DrawPicture(ICanvas& canvas) const;
	void List() const;

private:
	std::vector<std::unique_ptr<IShape>> m_shapes;

	static void PrintShape(size_t, const IShape& shape) ;
};

#endif //OOD_PICTURE_H
