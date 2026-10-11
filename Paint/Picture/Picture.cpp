//
// Created by Вадим Патрушев on 28.09.2026.
//

#include "Picture.h"
#include <iostream>
#include <ostream>
#include <set>

void Picture::AddShape(std::unique_ptr<IShape> shape)
{
	shape->AddObserver(*this);
	m_shapes.push_back(std::move(shape));
	NotifyObservers();
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
		// NotifyObservers();
	}
}

void Picture::MovePicture(const Point position)
{
	for (const auto& shape : m_shapes)
	{
		if (shape != nullptr)
		{
			shape->Move(position);
			// NotifyObservers();
		}
	}
}

void Picture::DeleteShape(const std::string& id)
{
	for (auto shape = m_shapes.begin(); shape != m_shapes.end();)
	{
		if ((*shape)->GetId() == id)
		{
			(*shape)->DeleteObserver(*this);
			shape = m_shapes.erase(shape);
			NotifyObservers();
		}
		else
		{
			++shape;
		}
	}
}

void Picture::ChangeColor(const std::string& id, const Color color)
{
	auto* shape = GetShape(id);
	if (shape != nullptr)
	{
		shape->ChangeColor(color);
		// NotifyObservers();
	}
}

void Picture::ChangeShape(const std::string& id, std::unique_ptr<IShapeBehavior> newBehavior)
{
	auto* shape = GetShape(id);
	if (shape != nullptr)
	{
		shape->ChangeBehavior(std::move(newBehavior));
		// NotifyObservers();
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

size_t Picture::GetShapesCount() const
{
	return m_shapes.size();
}

void Picture::AddObserver(PictureObserver& observer)
{
	for (const auto ob : m_observers)
	{
		if (ob == &observer)
		{
			return;
		}
	}
	m_observers.push_back(&observer);
}

void Picture::DeleteObserver(const PictureObserver& observer)
{
	std::erase(m_observers, &observer);
}

void Picture::NotifyObservers()
{
	for (auto* ob : m_observers)
	{
		ob->Update(GetShapesCount());
	}
}

void Picture::Update()
{
	NotifyObservers();
}