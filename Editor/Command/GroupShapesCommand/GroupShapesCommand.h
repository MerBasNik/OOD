//
// Created by Вадим Патрушев on 09.11.2026.
//

#ifndef OOD_EDITOR_GROUPSHAPESCOMMAND_H
#define OOD_EDITOR_GROUPSHAPESCOMMAND_H

#include "Command/ICommand.h"
#include "Model/Document/Document.h"
#include <memory>
#include <optional>
#include <vector>

class GroupShapesCommand : public ICommand
{
public:
	GroupShapesCommand(Document& document, std::vector<int> shapeIds);
	void Execute() override;
	void Unexecute() override;
	std::vector<int> GetShapeIds() const override;
	std::optional<int> GetGroupId() const;

private:
	Document& m_document;
	std::vector<int> m_shapeIds;
	std::vector<size_t> m_indices;
	std::unique_ptr<IShape> m_group;
	std::optional<int> m_groupId;
	size_t m_groupIndex = 0;

	void SortShapesByLayer();
};

#endif // OOD_EDITOR_GROUPSHAPESCOMMAND_H
