//
// Created by Вадим Патрушев on 09.11.2026.
//

#ifndef OOD_EDITOR_UNGROUPSHAPECOMMAND_H
#define OOD_EDITOR_UNGROUPSHAPECOMMAND_H

#include "Command/ICommand.h"
#include "Model/Document/Document.h"
#include <memory>
#include <vector>

class UngroupShapeCommand : public ICommand
{
public:
	UngroupShapeCommand(Document& document, int groupId);
	void Execute() override;
	void Unexecute() override;
	std::vector<int> GetShapeIds() const override;
	const std::vector<int>& GetChildIds() const;

private:
	Document& m_document;
	int m_groupId;
	size_t m_index = 0;
	std::unique_ptr<IShape> m_group;
	std::vector<int> m_childIds;
};

#endif // OOD_EDITOR_UNGROUPSHAPECOMMAND_H
