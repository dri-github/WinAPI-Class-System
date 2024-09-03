#ifndef WINAPI_WINDOW_H
#define WINAPI_WINDOW_H

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "base.h"

#define BEGIN_MSG_MAP(theClass, parentClass) \
protected: \
	virtual LRESULT CALLBACK WndProc(UINT message, WPARAM wParam, LPARAM lParam) \
	{ \
		switch(message) \
		{ \
			default: \
				return parentClass::WndProc(message, wParam, lParam);

#define MESSAGE_HANDLER(msg, func) \
	case msg: \
	{ \
		return func(message, wParam, lParam); \
	}

#define END_MSG_MAP() \
		} \
		return NULL; \
	}

#define BASE_WMDPROC(theClass) theClass::WndProc(message, wParam, lParam)

WINAPI_BEGIN
class Window;

struct WINAPI_CLASS_SYSTEM CreateStruct {
	LPCSTR className;
	DWORD style;
	Window* parent;
	int id;

	CreateStruct setClassName(LPCSTR className) {
		this->className = className;
		return *this;
	}
	CreateStruct setStyle(DWORD style) {
		this->style = style;
		return *this;
	}
};

////////////////////////////////////////////////////////////
/// \brief Base class for all "window objects"
///
////////////////////////////////////////////////////////////
class WINAPI_CLASS_SYSTEM Window
{
	friend class TargetWindow;
	friend class RenderWindow;

public:

	////////////////////////////////////////////////////////////
	/// \brief Constructor for creating a window from an existing window
	///
	/// \param hWnd Window handle
	///
	////////////////////////////////////////////////////////////
	Window(HWND hWnd);
	virtual ~Window();

protected:
	Window(CreateStruct createStruct);

public:
	void setTitle(LPCSTR title);
	void setParent(Window* parent);
	void setPosition(POINT position);
	void setPosition(int x, int y);
	void setSize(SIZE size);
	void setSize(int width, int height);
	void setMinSize(int width, int height);
	void setMaxSize(int width, int height);

	LPSTR getTitle() const;
	Window* getParent() const;
	POINT getPosition() const;
	SIZE getSize() const;
	SIZE getClientSize() const;
	int getId() const;
	HWND getHWnd() const;

	static Window* getObjectPtr(HWND hWnd);

	void show();
	void hide();
	void destroy();

protected:
	virtual LRESULT CALLBACK WndProc(UINT message, WPARAM wParam, LPARAM lParam);
	void messageCommand(WPARAM wParam, LPARAM lParam) { if (LOWORD(wParam) == getId()) { m_command = HIWORD(wParam); } else m_command = -1; }

	////////////////////////////////////////////////////////////
	// Member data
	////////////////////////////////////////////////////////////
protected:
	HWND m_hWnd;			///< Window handle
	Window* m_parentWindow;	///< Parent window
	int m_command = -1;

private:
	POINT m_maxSize; ///< Maximum window size
	POINT m_minSize; ///< Minimum window size
};
WINAPI_END

#endif // !WINAPI_WINDOW_H