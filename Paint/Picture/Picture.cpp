//
// Created by Вадим Патрушев on 28.09.2026.
//

#include "Picture.h"
#include <iostream>
#include <ostream>
#include <set>

void Picture::AddShape(std::unique_ptr<IShape> shape)
{
	m_shapes.push_back(std::move(shape));
}

IShape* Picture::GetShape(const std::string& id) const
{
	for (const auto& shape : m_shapes)
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
	for (const auto& shape : m_shapes)
	{
		shape->Move(position);
	}
}

void Picture::DeleteShape(const std::string& id)
{
	std::erase_if(m_shapes, [&id](const auto& shape) {
		return shape->GetId() == id;
	});
}

void Picture::ChangeColor(const std::string& id, const Color color)
{
	auto* shape = GetShape(id);
	if (shape != nullptr)
	{
		shape->ChangeColor(color);
	}
}

void Picture::ChangeShape(const std::string& id, std::unique_ptr<IShapeBehavior> newBehavior)
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
	for (const auto& it : m_shapes)
	{
		it->Draw(canvas);
	}
}

void Picture::List() const
{
	unsigned count = 0;
	for (const auto& it : m_shapes)
	{
		PrintShape(++count, *it);
	}
}

void Picture::PrintShape(const size_t index, const IShape& shape)
{
	std::cout << index << ". "
			  << shape.GetName() << " "
			  << shape.GetId() << " "
			  << std::format("#{:06x}", shape.GetColor()) << " "
			  << shape.GetInfo() << std::endl;
}