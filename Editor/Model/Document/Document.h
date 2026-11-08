//
// Created by Вадим Патрушев on 15.10.2026.
//

#ifndef OOD_EDITOR_DOCUMENT_H
#define OOD_EDITOR_DOCUMENT_H

#include "IDocumentObserver.h"
#include "Model/Shapes/IShape.h"
#include "Model/Types/DocumentItem.h"

#include <memory>
#include <optional>
#include <vector>

constexpr int START_ID = 1;

class Document
{
public:
	Document(double width, double height, double duration = DEFAULT_DURATION);
	Document(const Document&) = delete;
	Document& operator=(const Document&) = delete;
	Frame GetBounds() const;
	double GetDuration() const;
	int AddShape(std::unique_ptr<IShape> shape);
	void RemoveShape(int id);
	std::unique_ptr<IShape> DeleteShape(int id);
	void InsertShape(int id, std::unique_ptr<IShape> shape, size_t index);
	void SetShapeFrame(int id, const Frame& frame);
	void SetShapeColors(int id, Color fillColor, Color borderColor);
	void SetShapeIndex(int id, size_t index);
	void SetShapeTimeInterval(int id, const TimeInterval& timeInterval);
	void ReplaceShapes(std::vector<std::unique_ptr<IShape>> shapes);
	void Clear();
	size_t GetShapeCount() const;
	const IShape& GetShape(size_t index) const;
	int GetShapeId(size_t index) const;
	const IShape* FindShape(int id) const;
	std::optional<size_t> FindShapeIndex(int id) const;
	std::optional<int> FindTopShape(Point point, double time) const;
	void AddObserver(IDocumentObserver& observer);
	void RemoveObserver(IDocumentObserver& observer);

private:
	Frame m_bounds;
	double m_duration;
	std::vector<DocumentItem> m_shapesInner;
	int m_nextId = START_ID;
	std::vector<IDocumentObserver*> m_observers;

	std::vector<DocumentItem>::iterator FindItem(int id);
	void CheckFrame(const Frame& frame) const;
	void CheckTimeInterval(const TimeInterval& timeInterval) const;
	void CheckShape(const IShape& shape) const;
	void NotifyObservers();
};

#endif // OOD_EDITOR_DOCUMENT_H
