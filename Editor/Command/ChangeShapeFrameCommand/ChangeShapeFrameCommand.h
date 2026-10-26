//
// Created by Вадим Патрушев on 18.10.2026.
//

#ifndef OOD_EDITOR_CHANGESHAPEFRAMECOMMAND_H
#define OOD_EDITOR_CHANGESHAPEFRAMECOMMAND_H

#include "Command/ICommand.h"
#include "Model/Document/Document.h"
#include <optional>

class ChangeShapeFrameCommand : public ICommand
{
public:
	ChangeShapeFrameCommand(Document& document, int shapeId, const Frame& frame);
	void Execute() override;
	void Unexecute() override;

private:
	Document& m_document;
	int m_shapeId;
	Frame m_frame;
	std::optional<Frame> m_oldFrame;
};

#endif // OOD_EDITOR_CHANGESHAPEFRAMECOMMAND_H
