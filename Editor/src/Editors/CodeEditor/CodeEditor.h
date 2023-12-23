#pragma once

#include "Editors/Editor.h"
#include "Editors/CodeEditor/Languages.h"

#include "TextEditor.h"

#define CODE_LANG_C			TextEditor::LanguageDefinition::C()
#define CODE_LANG_CPP		TextEditor::LanguageDefinition::CPlusPlus()
#define CODE_LANG_ARDUINO	CustomLanguages::Arduino()

class CodeEditor : public Editor
{
public:
	CodeEditor();
	CodeEditor(TextEditor::LanguageDefinition lang);
	~CodeEditor();

	void OnUpdate() override;
	void OnRender() override;

	void Edit(const char* filepath) override;
	void Save() override;

	virtual bool CanUndo() { return m_TextEditor->CanUndo(); };
	void Undo() override;
	virtual bool CanRedo() { return m_TextEditor->CanRedo(); };
	void Redo() override;

	void MarkErrors(std::map<int, std::string> errors) override;

	bool IsModified() const override;
private:
	Ref<TextEditor> m_TextEditor;
};