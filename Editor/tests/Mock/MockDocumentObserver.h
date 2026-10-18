//
// Created by Вадим Патрушев on 19.10.2026.
//

#ifndef OOD_MOCKDOCUMENTOBSERVER_H
#define OOD_MOCKDOCUMENTOBSERVER_H
#include "Model/Document/IDocumentObserver.h"

#include <gmock/gmock.h>

class MockDocumentObserver : public IDocumentObserver
{
public:
	MOCK_METHOD(void, DocumentUpdate, (), (override));
};

#endif // OOD_MOCKDOCUMENTOBSERVER_H
