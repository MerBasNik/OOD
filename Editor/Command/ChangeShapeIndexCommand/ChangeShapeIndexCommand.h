//
// Created by Вадим Патрушев on 26.10.2026.
//

#ifndef OOD_EDITOR_CHANGESHAPEINDEXCOMMAND_H
#define OOD_EDITOR_CHANGESHAPEINDEXCOMMAND_H

#include "Command/ICommand.h"
#include "Model/Document/Document.h"

class ChangeShapeIndexCommand : public ICommand
{
public:
	ChangeShapeIndexCommand(Document& document, int shapeId, size_t index);
	void Execute() override;

private:
	Document& m_document;
	int m_shapeId;
	size_t m_index;
};

#endif // OOD_EDITOR_CHANGESHAPEINDEXCOMMAND_H
