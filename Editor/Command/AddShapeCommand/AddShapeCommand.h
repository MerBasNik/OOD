//
// Created by Вадим Патрушев on 18.10.2026.
//

#ifndef OOD_EDITOR_ADDSHAPECOMMAND_H
#define OOD_EDITOR_ADDSHAPECOMMAND_H

#include "Command/ICommand.h"
#include "Model/Document/Document.h"

class AddShapeCommand : public ICommand
{
public:
	AddShapeCommand(Document& document, std::unique_ptr<IShape> shape);
	void Execute() override;
	void Unexecute() override;
	std::optional<int> GetShapeId() const;

private:
	Document& m_document;
	std::unique_ptr<IShape> m_shape;
	std::optional<int> m_shapeId;
	size_t m_index = 0;
};

#endif // OOD_EDITOR_ADDSHAPECOMMAND_H
