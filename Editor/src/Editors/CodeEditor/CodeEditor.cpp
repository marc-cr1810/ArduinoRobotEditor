#include "epch.h"
#include "CodeEditor.h"

CodeEditor::CodeEditor(TextEditor::LanguageDefinition lang)
{
	m_TextEditor = CreateRef<TextEditor>();

	m_TextEditor->SetLanguageDefinition(lang);
}

CodeEditor::~CodeEditor()
{
}

void CodeEditor::OnUpdate()
{
}

void CodeEditor::OnRender()
{
	auto cpos = m_TextEditor->GetCursorPosition();
	ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, m_TextEditor->GetTotalLines(),
		m_TextEditor->IsOverwrite() ? "Ovr" : "Ins",
		m_TextEditor->CanUndo() ? "*" : " ",
		m_TextEditor->GetLanguageDefinition().mName.c_str(), m_Filepath.c_str());

	std::string name = "CodeEditor" + m_Filepath;
	m_TextEditor->Render(name.c_str());
}

void CodeEditor::Edit(const char* filepath)
{
	m_Filepath = filepath;

	std::ifstream ifs(filepath);
	if (ifs.good())
	{
		std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
		m_TextEditor->SetText(str);
	}
	ifs.close();
}

void CodeEditor::Save()
{
	auto str = m_TextEditor->GetText();
	std::ofstream ofs(m_Filepath, std::ofstream::trunc);
	ofs << str;
	ofs.close();
	m_TextEditor->SetText(str);
}
