//
// Created by Вадим Патрушев on 18.10.2026.
//

#ifndef OOD_EDITOR_DELETESHAPECOMMAND_H
#define OOD_EDITOR_DELETESHAPECOMMAND_H

#include "Command/ICommand.h"
#include "Model/Document/Document.h"

class DeleteShapeCommand : public ICommand
{
public:
	DeleteShapeCommand(Document& document, int shapeId);
	void Execute() override;
	void Unexecute() override;
	std::vector<int> GetShapeIds() const override;

private:
	Document& m_document;
	int m_shapeId;
	std::optional<int> m_parentId;
	size_t m_index = 0;
	std::unique_ptr<IShape> m_shape;
	std::map<int, TimeInterval> m_intervals;
};

#endif // OOD_EDITOR_DELETESHAPECOMMAND_H
