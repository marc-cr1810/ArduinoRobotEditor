#pragma once

#include "Editors/Editor.h"
#include "Editors/CodeEditor/Languages.h"

#include "TextEditor.h"

#define CODE_LANG_CPP		TextEditor::LanguageDefinition::CPlusPlus()
#define CODE_LANG_ARDUINO	CustomLanguages::Arduino()

class CodeEditor : public Editor
{
public:
	CodeEditor(TextEditor::LanguageDefinition lang);
	~CodeEditor();

	void OnUpdate() override;
	void OnRender() override;

	void Edit(const char* filepath) override;
	void Save() override;
private:
	Ref<TextEditor> m_TextEditor;
};