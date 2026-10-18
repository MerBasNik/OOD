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
	std::optional<int> GetShapeId() const;

private:
	Document& m_document;
	std::unique_ptr<IShape> m_shape;
	std::optional<int> m_shapeId;
};

#endif // OOD_EDITOR_ADDSHAPECOMMAND_H
