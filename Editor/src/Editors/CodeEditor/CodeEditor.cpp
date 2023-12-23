#include "epch.h"
#include "CodeEditor.h"

#include "Editors/CodeEditor/ColorPalette.h"

CodeEditor::CodeEditor()
{
	m_TextEditor = CreateRef<TextEditor>();
}

CodeEditor::CodeEditor(TextEditor::LanguageDefinition lang)
{
	m_TextEditor = CreateRef<TextEditor>();
	m_TextEditor->SetShowWhitespaces(false);
	m_TextEditor->SetPalette(ColorPalette::GetDraculaPalette());
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
	std::string name = "CodeEditor" + m_Filepath;
	const float footerHightToReserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeight();
	if (ImGui::BeginChild("Editor", ImVec2(0, -footerHightToReserve), false, ImGuiWindowFlags_HorizontalScrollbar))
	{
		m_TextEditor->Render(name.c_str());
		ImGui::EndChild();
	}

	auto cpos = m_TextEditor->GetCursorPosition();
	ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, m_TextEditor->GetTotalLines(),
		m_TextEditor->IsOverwrite() ? "Ovr" : "Ins",
		m_TextEditor->CanUndo() ? "*" : " ",
		m_TextEditor->GetLanguageDefinition().mName.c_str(), m_Filepath.c_str());
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
	str.pop_back();
	std::ofstream ofs(m_Filepath, std::ofstream::trunc);
	ofs << str;
	ofs.close();
	m_TextEditor->SetText(str);
}

void CodeEditor::Undo()
{
	if (m_TextEditor->CanUndo())
		m_TextEditor->Undo();
}

void CodeEditor::Redo()
{
	if (m_TextEditor->CanRedo())
		m_TextEditor->Redo();
}

void CodeEditor::MarkErrors(std::map<int, std::string> errors)
{
	m_TextEditor->SetErrorMarkers(errors);
}

bool CodeEditor::IsModified() const
{
	return m_TextEditor->CanUndo();
}
