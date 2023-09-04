#pragma once

class Editor
{
public:
	Editor() = default;
	~Editor() = default;

	virtual void OnUpdate() = 0;
	virtual void OnRender() = 0;

	virtual void Save() {}
	virtual void Edit(const char* filepath) {}

	void SetWindowInit(bool initState) { m_InitWindow = initState; }
	bool GetWindowInit() const { return m_InitWindow; }

	std::string GetFilepath() const { return m_Filepath; }
	std::string GetFilename() const { return std::filesystem::path(m_Filepath).filename().string(); }

	virtual bool IsModified() const { return false; }

	bool operator==(const Ref<Editor> b) { return m_Filepath == b->m_Filepath; }
protected:
	std::string m_Filepath;
	bool m_InitWindow = false;
};