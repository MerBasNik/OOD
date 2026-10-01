//
// Created by Вадим Патрушев on 28.09.2026.
//

#include "Picture.h"

Picture::Picture()
{
}

void Picture::AddShape(std::unique_ptr<IShape> shape)
{
	m_shapes.push_back(std::move(shape));
}

IShape Picture::GetShape(const std::string& id) const
{
	for (const auto &it : m_shapes)
	{
		if (it->GetId() == id)
		{
			return *it;
		}
	}
	return {};
}

void Picture::MoveShape(const std::string& id, Point position)
{

}

void Picture::MovePicture(Point position)
{

}

void Picture::DeleteShape(const std::string& id)
{

}

void Picture::ChangeColor(const std::string& id, Color color)
{

}

void Picture::ChangeShape(const std::string& id, std::unique_ptr<IShapeBehavior> newBehavior)
{

}

void Picture::DrawShape(const std::string& id, ICanvas& canvas)
{

}

void Picture::DrawPicture(ICanvas& canvas)
{

}

void Picture::List()
{

}