#pragma once

class ContextMenu
{
public:
	ContextMenu(const std::string& id)
		: m_ID(id)
	{}

	void Open() { m_Open = true; }
	void OnRender();
protected:
	virtual void DrawContent() = 0;
private:
	bool m_Open = false;
	std::string m_ID;
};