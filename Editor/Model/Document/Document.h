//
// Created by Вадим Патрушев on 15.10.2026.
//

#ifndef OOD_EDITOR_DOCUMENT_H
#define OOD_EDITOR_DOCUMENT_H

#include "IDocumentObserver.h"
#include "Model/Shapes/IShape.h"
#include <memory>
#include <optional>
#include <vector>

class Document
{
public:
	const int START_ID = 1;

	Document(double width, double height);
	Document(const Document&) = delete;
	Document& operator=(const Document&) = delete;

	Frame GetBounds() const;

	int AddShape(std::unique_ptr<IShape> shape);
	void RemoveShape(int id);
	void SetShapeFrame(int id, const Frame& frame);
	void ReplaceShapes(std::vector<std::unique_ptr<IShape>> shapes);
	void Clear();

	size_t GetShapeCount() const;
	const IShape& GetShape(size_t index) const;
	int GetShapeId(size_t index) const;
	const IShape* FindShape(int id) const;
	std::optional<int> FindTopShape(Point point) const;

	void AddObserver(IDocumentObserver& observer);
	void RemoveObserver(IDocumentObserver& observer);

private:
	struct ShapeInner
	{
		int m_id;
		std::unique_ptr<IShape> m_shape;
	};

	Frame m_bounds;
	std::vector<ShapeInner> m_shapesInner;
	int m_nextId = START_ID;
	std::vector<IDocumentObserver*> m_observers;

	std::vector<ShapeInner>::iterator FindItem(int id);
	void CheckFrame(const Frame& frame) const;
	void NotifyObservers();
};

#endif // OOD_EDITOR_DOCUMENT_H
