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

IShape* Picture::GetShape(const std::string& id)
{
	for (const auto &shape : m_shapes)
	{
		if (shape->GetId() == id)
		{
			return shape.get();
		}
	}
	return nullptr;
}

void Picture::MoveShape(const std::string& id, const Point position)
{
	auto* shape = GetShape(id);
	if (shape != nullptr)
	{
		shape->Move(position);
	}
}

void Picture::MovePicture(const Point position)
{
	for (const auto &shape : m_shapes)
	{
		shape->Move(position);
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

void Picture::ChangeColor(const std::string& id, const Color color)
{
	auto* shape = GetShape(id);
	if (shape != nullptr)
	{
		shape->ChangeColor(color);
	}
}

void Picture::ChangeShape(const std::string& id, std::unique_ptr<IShapeBehavior>& newBehavior)
{
	auto* shape = GetShape(id);
	if (shape != nullptr)
	{
		shape->ChangeBehavior(std::move(newBehavior));
	}
}

void Picture::DrawShape(const std::string& id, ICanvas& canvas) const
{
	const auto* shape = GetShape(id);
	if (shape != nullptr)
	{
		shape->Draw(canvas);
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
	for (size_t i = 0; i < m_shapes.size(); i++)
	{
		PrintShape(i + 1, *m_shapes[i]);
	}
}

void Picture::PrintShape(const size_t index, const IShape& shape)
{
	std::cout << index << " "
		<< shape.GetName() << " "
		<< shape.GetId() << " "
		<< shape.GetColor().ToString() << " "
		<< shape.GetInfo() << std::endl;
}