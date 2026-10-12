//
// Created by Вадим Патрушев on 28.09.2026.
//

#ifndef OOD_SHAPE_H
#define OOD_SHAPE_H

#include "Color.h"
#include "IShape.h"

class Shape : public IShape
{
public:
	~Shape() override = default;
	Shape(const std::string& id, Color color, std::unique_ptr<IShapeBehavior> behavior);
	Shape(const Shape& shape) = delete;
	Shape& operator=(const Shape& shape) = delete;
	std::string GetId() const override;
	void SetId(const std::string& newId) override;
	Color GetColor() const override;
	std::string GetName() const override;
	std::string GetInfo() const override;
	void ChangeColor(Color color) override;
	void Move(Point position) override;
	void Draw(ICanvas& canvas) const override;
	void ChangeBehavior(std::unique_ptr<IShapeBehavior> newBehavior) override;
	std::unique_ptr<IShape> Clone() const override;
	void AddObserver(IShapeObserver& observer) override;
	void DeleteObserver(const IShapeObserver& observer) override;

private:
	std::string m_id;
	Color m_color;
	std::unique_ptr<IShapeBehavior> m_behavior;
	std::vector<IShapeObserver*> m_observers;

	void NotifyObservers() const;
};

#endif // OOD_SHAPE_H
