//
// Created by Вадим Патрушев on 28.09.2026.
//

#ifndef OOD_PICTURE_H
#define OOD_PICTURE_H

#include "ICanvas.h"
#include "IShape.h"
#include "IShapeBehavior.h"
#include "Observer/PictureObserver.h"
#include "Point.h"
#include <vector>

class Picture : public IShapeObserver
{
public:
	Picture() = default;
	Picture(const Picture& picture) = delete;
	Picture& operator=(const Picture& picture) = delete;
	void AddShape(std::unique_ptr<IShape> shape);
	IShape* GetShape(const std::string& id) const;
	void MoveShape(const std::string& id, Point position);
	void MovePicture(Point position);
	void DeleteShape(const std::string& id);
	void ChangeColor(const std::string& id, Color color);
	void ChangeShape(const std::string& id, std::unique_ptr<IShapeBehavior> newBehavior);
	void DrawShape(const std::string& id, ICanvas& canvas) const;
	void DrawPicture(ICanvas& canvas) const;
	void List() const;
	size_t GetShapesCount() const;
	void AddObserver(PictureObserver& observer);
	void DeleteObserver(const PictureObserver& observer);
	void Update() override;
	std::unique_ptr<Picture> Clone() const;

private:
	std::vector<std::unique_ptr<IShape>> m_shapes;
	std::vector<PictureObserver*> m_observers;

	static void PrintShape(size_t, const IShape& shape);
	void NotifyObservers();
};

#endif // OOD_PICTURE_H
