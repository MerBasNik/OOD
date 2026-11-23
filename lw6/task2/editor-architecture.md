# Задание 2

## Управление приложением

Application создает и открывает документы, обрабатывает события и отрисовывает окна. AppDocument
владеет моделью Document и ее CommandHistory, а AppWindow получает модель и историю у AppDocument,
передает их контроллерам вместе с состоянием своего окна и связывает контроллеры с EditorWindow
[Application](../../Editor/Application/Application.cpp),
[AppDocument](../../Editor/Application/AppDocument.cpp),
[AppWindow](../../Editor/Application/AppWindow.cpp)

## View

EditorWindow принимает события SFML и направляет ввод в EditorController. События из ShapeListView
и TimelineView идут в соответствующие контроллеры. CanvasView рисует фигуры из документа с учетом
времени и превью изменений окно и рисует выделение
[EditorWindow](../../Editor/View/Window/EditorWindow.cpp),
[ShapeList](../../Editor/View/ShapeList/ShapeListView.cpp),
[Timeline](../../Editor/View/Timeline/TimelineView.cpp),
[CanvasView](../../Editor/View/Canvas/CanvasView.cpp)

## Controller

EditorController определяет, какие фигуры затронуло действие, обновляет выделение и формирует
команды. ShapeListController обрабатывает выбор и перестановку фигур в списке. TimelineController
обрабатывает выбор на шкале времени и формирует команды изменения интервалов
[EditorController](../../Editor/Controller/EditorController.cpp),
[ShapeListController](../../Editor/Controller/ShapeListController.cpp),
[TimelineController](../../Editor/Controller/TimelineController.cpp)

## Model

Document хранит доменные данные фигуры. При удалении фигуры он также удаляет ее интервалы из
Timeline. EditorState хранит выделение и превью состояние окна
[Delete](../../Editor/Model/Document/Document.cpp),
[Timeline shapes](../../Editor/Model/Timeline/Timeline.cpp),
[EditorState](../../Editor/Model/EditorState/EditorState.h),
[Move](../../Editor/Controller/EditorController.cpp),
[Завершение жеста](../../Editor/Controller/EditorController.cpp)

## Команды

Отдельная команда изменяет документ и сохраняет данные для отмены. MacroCommand объединяет несколько
команд, выполняет их по порядку и отменяет в обратном
[DeleteShapeCommand](../../Editor/Command/DeleteShapeCommand/DeleteShapeCommand.cpp),
[MacroCommand](../../Editor/Command/MacroCommand/MacroCommand.cpp)

## История команд

CommandHistory после успешного выполнения сохраняет одиночную команду или макрокоманду одной
записью. Undo отменяет последную выполненную команду, а Redo выполняет следующую. Если после Undo
выполнить новую команду, то прежние записи для Redo сбрасываются
[Execute, Unexecute](../../Editor/Command/CommandHistory/CommandHistory.cpp),
[Undo и Redo](../../Editor/Command/CommandHistory/CommandHistory.cpp)
