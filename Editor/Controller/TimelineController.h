//
// Created by Вадим Патрушев on 08.11.2026.
//

#ifndef OOD_EDITOR_TIMELINECONTROLLER_H
#define OOD_EDITOR_TIMELINECONTROLLER_H

#include "Command/CommandHistory/CommandHistory.h"
#include "Enums/TimelineDragMode.h"
#include "Model/Document/Document.h"
#include "Model/EditorState/EditorState.h"
#include <optional>
#include <vector>

class TimelineController : public IDocumentObserver
{
public:
	TimelineController(Document& document, EditorState& state, CommandHistory& history);
	~TimelineController() override;
	void SetCurrentTime(double time) const;
	void OnMouseDown(std::optional<size_t> row, double time, TimelineDragMode mode, bool isToggle = false);
	void OnMouseMove(size_t slot, double time);
	void OnMouseUp(size_t slot, double time);
	void Cancel();
	TimelineDragMode GetDragMode() const;
	std::optional<size_t> GetDropSlot() const;
	void DocumentUpdate() override;

private:
	Document& m_document;
	EditorState& m_state;
	CommandHistory& m_history;
	TimelineDragMode m_mode = TimelineDragMode::None;
	int m_clickedId = 0;
	double m_startTime = 0;
	bool m_isMoved = false;
	std::vector<int> m_shapeIds;
	std::vector<TimeInterval> m_startIntervals;
	std::optional<size_t> m_dropSlot;

	void StartDrag(int id, double time, TimelineDragMode mode);
	std::vector<TimeInterval> CalculateIntervals(double time) const;
	double ClampOffset(double offset) const;
	void FinishTimeDrag(double time);
	void FinishLayersDrag(size_t slot);
	std::vector<int> CalculateLayerOrder(const std::vector<int>& order, const std::vector<int>& movedIds, size_t slot) const;
	std::vector<std::unique_ptr<ICommand>> CreateLayerCommands(const std::vector<int>& order, const std::vector<int>& newOrder) const;
	std::vector<int> GetLayerOrder() const;
	int GetShapeIdAtRow(size_t row) const;
	void ExecuteCommands(std::vector<std::unique_ptr<ICommand>> commands) const;
};

#endif // OOD_EDITOR_TIMELINECONTROLLER_H
