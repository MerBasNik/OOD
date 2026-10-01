//
// Created by Вадим Патрушев on 28.09.2026.
//

#include "Picture.h"

#include <iostream>
#include <ostream>

Picture::Picture()
	: m_shapes()
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

void Picture::MoveShape(const std::string& id, const Point position) const
{
	for (const auto &it : m_shapes)
	{
		if (it->GetId() == id)
		{
			it->Move(position);
		}
	}
}

void Picture::MovePicture(const Point position) const
{
	for (const auto &it : m_shapes)
	{
		it->Move(position);
	}
}

void Picture::DeleteShape(const std::string& id)
{
	for (const auto &it : m_shapes)
	{
		if (it->GetId() == id)
		{
			m_shapes.erase(m_shapes.begin(), m_shapes.begin() + 1);
		}
	}
}

void Picture::ChangeColor(const std::string& id, const Color color) const
{
	for (const auto &it : m_shapes)
	{
		if (it->GetId() == id)
		{
			it->ChangeColor(color);
		}
	}
}

void Picture::ChangeShape(const std::string& id, std::unique_ptr<IShapeBehavior>& newBehavior)
{
	for (const auto &it : m_shapes)
	{
		// if (it->GetId() == id)
		// {
		// 	it->ChangeBehavior(newBehavior);
		// }
	}
}

void Picture::DrawShape(const std::string& id, ICanvas& canvas) const
{
	for (const auto &it : m_shapes)
	{
		if (it->GetId() == id)
		{
			it->Draw(canvas);
		}
	}
}

void Picture::DrawPicture(ICanvas& canvas) const
{
	for (const auto &it : m_shapes)
	{
		it->Draw(canvas);
	}
}

void Picture::List() const
{
	for (const auto &it : m_shapes)
	{
		PrintShape(*it);
	}
}

void Picture::PrintShape(IShape& shape) const
{
	std::cout << shape.GetId() << std::endl;
}