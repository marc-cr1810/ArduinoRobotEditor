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
protected:
	std::string m_Filepath;
};