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
	Document(double width, double height, double duration = DEFAULT_DURATION);
	Document(const Document&) = delete;
	Document& operator=(const Document&) = delete;
	Frame GetBounds() const;
	double GetDuration() const;
	int AddShape(std::unique_ptr<IShape> shape);
	int AddShapeAt(std::unique_ptr<IShape> shape, size_t index);
	void RemoveShape(int id);
	std::unique_ptr<IShape> DeleteShape(int id, std::optional<int> parentId = std::nullopt);
	void InsertShape(std::unique_ptr<IShape> shape, size_t index, std::optional<int> parentId = std::nullopt);
	int GroupShapes(const std::vector<int>& shapeIds, std::unique_ptr<IShape> group, size_t index,
		std::optional<int> parentId = std::nullopt);
	std::unique_ptr<IShape> UngroupShape(int groupId, const std::vector<size_t>& childIndices,
		std::optional<int> parentId = std::nullopt);
	void StartUpdate();
	void EndUpdate();
	void SetShapeFrame(int id, const Frame& frame);
	void SetShapeColors(int id, Color fillColor, Color borderColor);
	void SetShapeIndex(int id, size_t index);
	void SetShapeTimeInterval(int id, const TimeInterval& timeInterval);
	void ReplaceShapes(std::vector<std::unique_ptr<IShape>> shapes);
	void Clear();
	size_t GetShapeCount(std::optional<int> parentId = std::nullopt) const;
	const IShape& GetShape(size_t index) const;
	int GetShapeId(size_t index) const;
	const IShape* FindShape(int id) const;
	std::optional<int> FindTargetShape(int rootId, Point point, double time) const;
	std::optional<size_t> FindShapeIndex(int id, std::optional<int> parentId = std::nullopt) const;
	std::optional<int> FindParentShapeId(int id) const;
	std::optional<int> FindRootShapeId(int id) const;
	std::optional<int> FindTopShape(Point point, double time) const;
	void AddObserver(IDocumentObserver& observer);
	void RemoveObserver(IDocumentObserver& observer);

private:
	Frame m_bounds;
	double m_duration;
	std::vector<std::unique_ptr<IShape>> m_shapesInner;
	std::vector<IDocumentObserver*> m_observers;
	size_t m_updateDepth = 0;

	std::vector<std::unique_ptr<IShape>>::iterator FindItem(int id);
	IShape* GetShapeById(int id);
	GroupShape& GetGroup(int id);
	const GroupShape& GetGroup(int id) const;
	std::unique_ptr<IShape> ExtractShape(int id, std::optional<int> parentId);
	void InsertShapeInto(std::unique_ptr<IShape> shape, size_t index, std::optional<int> parentId);
	std::optional<int> FindParentInShape(const IShape& shape, int id) const;
	std::optional<int> FindTargetInShape(const IShape& shape, Point point, double time) const;
	void CheckFrame(const Frame& frame) const;
	void CheckTimeInterval(const TimeInterval& timeInterval) const;
	void CheckShape(const IShape& shape) const;
	void CheckGrouping(const std::vector<int>& shapeIds, const IShape* group, size_t index,
		std::optional<int> parentId) const;
	void CheckUngrouping(const IShape* group, const std::vector<size_t>& childIndices,
		std::optional<int> parentId) const;
	void NotifyObservers();
};

#endif // OOD_EDITOR_DOCUMENT_H
