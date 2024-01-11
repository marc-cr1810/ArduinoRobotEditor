#pragma once

#include <map>

class Editor
{
public:
	Editor() = default;
	~Editor() = default;

	virtual void OnUpdate() = 0;
	virtual void OnRender() = 0;

	virtual void Save() {}
	virtual void Edit(const char* filepath) { m_Filepath = filepath; }

	virtual bool CanUndo() { return false; };
	virtual void Undo() {};
	virtual bool CanRedo() { return false; };
	virtual void Redo() {};

	virtual void MarkErrors(std::map<int, std::string> errors) {};
	virtual void GetMarkedErrors(std::map<int, std::string> errors) {};

	void SetWindowInit(bool initState) { m_InitWindow = initState; }
	bool GetWindowInit() const { return m_InitWindow; }

	std::string GetFilepath() const { return m_Filepath; }
	std::string GetFilename() const { return std::filesystem::path(m_Filepath).filename().string(); }

	virtual bool IsModified() const { return false; }

	bool HasMenuBar() const { return m_HasMenuBar; }

	bool operator==(const Ref<Editor> b) { return m_Filepath == b->m_Filepath; }
protected:
	std::string m_Filepath;
	bool m_HasMenuBar = false;
	bool m_InitWindow = false;
};