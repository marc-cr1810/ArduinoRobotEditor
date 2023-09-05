#pragma once

class Window
{
public:
	virtual void OnUpdate() {};
	virtual void OnRender() = 0;

	void Open() { m_Open = true; }
protected:
	bool m_Open = false;
};