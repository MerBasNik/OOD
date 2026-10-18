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

private:
	Document& m_document;
	int m_shapeId;
};

#endif // OOD_EDITOR_DELETESHAPECOMMAND_H
