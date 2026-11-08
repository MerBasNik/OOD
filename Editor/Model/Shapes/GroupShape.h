//
// Created by Вадим Патрушев on 09.11.2026.
//

#ifndef OOD_EDITOR_GROUPSHAPE_H
#define OOD_EDITOR_GROUPSHAPE_H

#include "IShape.h"
#include <memory>
#include <vector>

class GroupShape : public IShape
{
public:
	GroupShape() = default;
	explicit GroupShape(std::vector<std::unique_ptr<IShape>> children);

	size_t GetChildCount() const;
	const IShape& GetChild(size_t index) const;
	void AddChild(std::unique_ptr<IShape> child);
	std::vector<std::unique_ptr<IShape>> ReleaseChildren();

	bool HitTest(Point point) const override;
	std::unique_ptr<IShape> Clone() const override;
	Frame GetFrame() const override;
	void SetFrame(const Frame& frame) override;
	Color GetFillColor() const override;
	Color GetBorderColor() const override;
	void SetFillColor(Color color) override;
	void SetBorderColor(Color color) override;
	void Accept(IShapeVisitor& visitor) const override;
	TimeInterval GetTimeInterval() const override;
	void SetTimeInterval(TimeInterval timeInterval) override;
	GroupShape* GetGroup() override;
	const GroupShape* GetGroup() const override;

private:
	std::vector<std::unique_ptr<IShape>> m_children;

	static double Scale(double value, double oldStart, double oldEnd, double newStart, double newEnd);
};

#endif // OOD_EDITOR_GROUPSHAPE_H
