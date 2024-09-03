//Необходимо для работы сообщения WM_CUSTOMDRAW
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <WinAPI Class System/RenderWindow.h>
#include <WinAPI Class System/controls/Button.h>
#include <WinAPI Class System/controls/Edit.h>
#include <WinAPI Class System/controls/ProgressBar.h>
#include <WinAPI Class System/controls/Static.h>
#include <WinAPI Class System/controls/ListBox.h>

#include "resource.h"

#include <unordered_map>

class Theme {
protected:
	using ClassStyles = std::unordered_map<std::string, std::string>;

public:
	Theme() : 
		styleSheets(new std::unordered_map<std::string, ClassStyles>{
			{ "normal_button", {
				{ "color", "#000000" },
				{ "background-color", "#000000" }
			}},
			{ "pressed_button", {
				{ "color", "#000000" },
				{ "background-color", "#000000" }
			}}
		}) {}

	

private:
	std::unordered_map<std::string, ClassStyles>* styleSheets;
};

using winapi::RenderWindow;
using winapi::TargetWindow;
using winapi::Window;
using winapi::CreateStruct;

typedef winapi::Button<BS_DEFPUSHBUTTON> Button;
typedef winapi::Button<BS_AUTOCHECKBOX> Toggle;
typedef winapi::Edit<ES_LEFT> Edit;
typedef winapi::ProgressBar ProgressBar;
typedef winapi::Static<> Text;
typedef winapi::ListBox ListBox;

class Scene : public TargetWindow {
public:
	Scene(CreateStruct createStruct) : TargetWindow(createStruct.setStyle(WS_VISIBLE | WS_POPUP | WS_CAPTION)) {
		setTitle("Scene");
	}

protected:
	BEGIN_MSG_MAP(Scene, TargetWindow)
	case WM_ERASEBKGND:
	{
		RECT rect;
		GetClientRect(m_hWnd, &rect);
		FillRect((HDC)wParam, &rect, CreateSolidBrush(RGB(60, 60, 60)));
		return 1;
	}
	END_MSG_MAP()

	virtual void OnPaint(HDC hdc, LPARAM lParam) {
		RECT rect;
		SetRect(&rect, 10, 10, 300, 200);
		FillRect(hdc, &rect, CreateSolidBrush(RGB(100, 248, 50)));
	}
};

#include <string>
#include <map>
class FilterList : public TargetWindow {

	typedef const char* (*getName_t)();
	typedef std::map<std::string, std::string>* (*getParametrs_t)();

public:
	FilterList(CreateStruct createStruct) : TargetWindow(createStruct.setStyle(WS_VISIBLE | WS_POPUP | WS_CAPTION)) {
		setTitle("FilterList");

		//text = getWindow<Text>();
		//text->setSize(100, 20);

		buttonAddFilter = getWindow<Button>();
		buttonAddFilter->setTitle("Add");
		buttonAddFilter->setSize(80, 20);

		buttonDeleteFilter = getWindow<Button>();
		buttonDeleteFilter->setTitle("Del");
		buttonDeleteFilter->setSize(30, 20);

		listBox = getWindow<ListBox>();
		listBox->setSize(120, 240);
		listBox->setPosition(10, 40);

		OnResize(ResizeState::Restored, getClientSize());
	}

	void* getCurrentElement() const {
		return listBox->getCurrentData();
	}

protected:
	BEGIN_MSG_MAP(FilterList, TargetWindow)
	case WM_ERASEBKGND:
	{
		RECT rect;
		GetClientRect(m_hWnd, &rect);
		FillRect((HDC)wParam, &rect, CreateSolidBrush(RGB(60, 60, 60)));
		return 1;
	}
	case WM_NOTIFY: {
		LPNMHDR nmHdr = (LPNMHDR)lParam;
		if (nmHdr->idFrom == buttonAddFilter->getId()) {
			switch (nmHdr->code)
			{
			case NM_CUSTOMDRAW: {
				LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)lParam;
				HDC hdc = item->hdc;

				if (item->uItemState & CDIS_SELECTED)
					FillRect(item->hdc, &item->rc, CreateSolidBrush(RGB(125, 125, 125)));
				else if (item->uItemState & CDIS_HOT)
					FillRect(item->hdc, &item->rc, CreateSolidBrush(RGB(112, 112, 112)));
				else
					FillRect(item->hdc, &item->rc, CreateSolidBrush(RGB(88, 88, 88)));

				DWORD style = BS_DEFPUSHBUTTON;

				UINT format = DT_SINGLELINE;
				format |= (style & BS_LEFT) == BS_LEFT ? DT_LEFT : NULL;
				format |= (style & BS_CENTER) == BS_CENTER ? DT_CENTER : NULL;
				format |= (style & BS_RIGHT) == BS_RIGHT ? DT_RIGHT : NULL;
				format |= (style & BS_TOP) == BS_TOP ? DT_TOP : NULL;
				format |= (style & BS_VCENTER) == BS_VCENTER ? DT_VCENTER : NULL;
				format |= (style & BS_BOTTOM) == BS_BOTTOM ? DT_BOTTOM : NULL;

				if ((style & BS_LEFT) != BS_LEFT && (style & BS_TOP) != BS_TOP)
					format |= DT_CENTER | DT_VCENTER;

				SetBkMode(hdc, TRANSPARENT);
				SetTextColor(hdc, RGB(210, 210, 210));

				DrawTextA(hdc, getTitle(), -1, &item->rc, format);

				return CDRF_SKIPDEFAULT;;
			}
			}

			return NULL;
		}
		break;
	}
	case WM_CTLCOLORLISTBOX: {
		HDC hdc = reinterpret_cast<HDC>(wParam);
		SetBkMode(hdc, TRANSPARENT);
		SetBkColor(hdc, RGB(42, 42, 42));
		SetTextColor(hdc, RGB(210, 210, 210));
		return (LRESULT)CreateSolidBrush(RGB(42, 42, 42));
	}
	END_MSG_MAP()



	void OnCommand(WPARAM wParam, LPARAM lParam) {
		if (wParam == buttonAddFilter->getId()) {
			OPENFILENAME ofn;
			char szFile[MAX_PATH];

			ZeroMemory(szFile, sizeof(szFile));
			ZeroMemory(&ofn, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = m_hWnd;
			ofn.lpstrFile = szFile;
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = "DLL(*.dll)\0*.dll\0All files(*.*)\0*.*\0\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT | OFN_EXPLORER;

			if (GetOpenFileName(&ofn) == true) {
				unsigned int iName = 0;
				char** names = new char* [MAX_PATH / 2];
				ZeroMemory(names, MAX_PATH / 2);
				unsigned int iBuffer = 0;
				char* buffer = new char[MAX_PATH];

				for (int i = 0; i < MAX_PATH; i++) {
					if ((buffer[iBuffer++] = szFile[i]) == '\0') {
						if (iBuffer == 1)
							break;
						names[iName++] = buffer;
						buffer = new char[MAX_PATH];
						iBuffer = 0;
					}
				}

				if (iName > 1) {
					size_t waySize = strlen(names[0]);
					for (unsigned int i = 1; i < iName; i++) {
						size_t nameSize = waySize + strlen(names[i]) + 2;
						char* dllName = new char[nameSize];
						memmove(dllName, names[0], waySize);
						memmove(dllName + waySize, "\\", 1);
						memmove(dllName + waySize + 1, names[i], strlen(names[i]));
						dllName[nameSize - 1] = '\0';

						names[i] = dllName;
					}
				}

				for (unsigned int i = 0; i < iName; i++) {
					HINSTANCE hDll = LoadLibrary(names[i]);
					if (hDll != NULL)
					{
						getParametrs_t getParametrs = (getParametrs_t)GetProcAddress(hDll, "getParametrs");

						getName_t getName = (getName_t)GetProcAddress(hDll, "getName");
						if (getName != NULL) {
							listBox->add(getName(), hDll);
						}
						else {
							MessageBox(NULL, "Unreal function", "Error", NULL);
						}
					}
					else {
						MessageBox(NULL, "Wrong DLL", "Error", NULL);
						//TODO: Error open library
					}
					FreeLibrary(hDll);
				}
			}
		}
		else if (wParam == buttonDeleteFilter->getId()) {
			listBox->deleteString(listBox->getCurrentIndex());
		}
		else if (wParam == listBox->getId()) {
			MessageBox(NULL, "12345", "345", NULL);
		}
	}

	void OnResize(ResizeState resizeState, SIZE newSize) {
		buttonAddFilter->setPosition((newSize.cx - buttonAddFilter->getSize().cx) / 2, 10);
		buttonDeleteFilter->setPosition(newSize.cx - buttonDeleteFilter->getSize().cx - 10, 10);
		listBox->setSize(newSize.cx - 20, 240);
	}

protected:
	Button* buttonAddFilter;
	Button* buttonDeleteFilter;
	ListBox* listBox;
	Text* text;
};

#include <vector>
class Editor : public TargetWindow {
public:
	Editor(CreateStruct createStruct) : TargetWindow(createStruct.setStyle(WS_VISIBLE | WS_POPUPWINDOW | WS_CAPTION)) {
		setTitle("Editor");

		//auto edit = getWindow<Edit>();
		//edit->setTitle("Name");
		//edit->setSize(100, 20);
		//addProperty(edit);
		//
		//auto edit2 = getWindow<Edit>();
		//edit2->setTitle("Title");
		//edit2->setSize(100, 20);
		//addProperty(edit2);
		//
		//auto toggle = getWindow<Toggle>();
		//toggle->setTitle("Is Play");
		//toggle->setSize(100, 20);
		//addProperty(toggle);

		addProperty<int>("testINT", new int(0));
		addProperty<bool>("testBool", new bool(false));
	}

protected:
	template<typename T>
	void addProperty(LPCSTR name, T* value) {

	}

	template<>
	void addProperty<int>(LPCSTR name, int* value) {
		SIZE size = { 200, 400 };

		winapi::Static<SS_RIGHT>* text = getWindow<winapi::Static<SS_RIGHT>>();
		text->setTitle(name);
		text->setSize(size.cx / 2, 20);
		text->setPosition(0, position.y);

		auto* number = getWindow<winapi::Edit<ES_NUMBER>>();
		number->setSize(size.cx / 2, 20);
		number->setPosition(size.cx / 2, position.y);

		position.y += 20;
	}

	template<>
	void addProperty<bool>(LPCSTR name, bool* value) {
		SIZE size = { 200, 400 };

		winapi::Static<SS_RIGHT>* text = getWindow<winapi::Static<SS_RIGHT>>();
		text->setTitle(name);
		text->setSize(size.cx / 2, 20);
		text->setPosition(0, position.y);

		auto* number = getWindow<Toggle>();
		number->setSize(size.cx / 2, 20);
		number->setPosition(size.cx / 2, position.y);

		position.y += 20;
	}

	//template<typename T>
	//void addProperty(LPCSTR name, T* value) {
	//	winapi::Static<SS_RIGHT>* text = getWindow<winapi::Static<SS_RIGHT>>();
	//	text->setTitle(name);
	//
	//	Window* window = NULL;
	//	switch (typeid(T)) {
	//	case typeid(int): {
	//		window = getWindow<Edit>();
	//		window->setSize(100, 20);
	//		window->setPosition(65, position.y);
	//		break;
	//	}
	//	case typeid(bool): {
	//		break;
	//	}
	//	default:
	//		break;
	//	}
	//
	//	if (window == NULL) return;
	//	text->setSize(50, window->getSize().cy);
	//	text->setPosition(10, position.y);
	//
	//	position.y += text->getSize().cy + 5;
	//}

private:
	std::vector<Window*> m_propertys;
	POINT position = { 0, 0 };
};

class MainWindow : public RenderWindow {
public:
	MainWindow() : RenderWindow("WinAPIClassSystem") {
		setTitle("Test Window API");
		SetMenu(m_hWnd, LoadMenu(GetModuleHandle(nullptr), MAKEINTRESOURCE(MAIN_MENU)));
		setSize(1280, 720);

		filterList = getWindow<FilterList>();
		scene = getWindow<Scene>();
		editor = getWindow<Editor>();

		OnResize(ResizeState::Restored, getClientSize());
	}

	void OnCommand(WPARAM wParam, LPARAM lParam) {
		switch (LOWORD(wParam))
		{
		case ID_FILE_OPEN: {
			OPENFILENAME ofn;
			char szFile[MAX_PATH];

			ZeroMemory(szFile, sizeof(szFile));
			ZeroMemory(&ofn, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = m_hWnd;
			ofn.lpstrFile = szFile;
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = "Bitmap files(*.bmp)\0*.bmp\0JPEG files(*.jpg)\0*.jpg\0All files(*.*)\0*.*\0\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT | OFN_EXPLORER;

			if (GetOpenFileName(&ofn) == true) {
				unsigned int iName = 0;
				char** names = new char* [MAX_PATH / 2];
				ZeroMemory(names, MAX_PATH);
				unsigned int iBuffer = 0;
				char* buffer = new char[MAX_PATH];

				for (int i = 0; i < MAX_PATH; i++) {
					if ((buffer[iBuffer++] = szFile[i]) == '\0') {
						if (iBuffer == 1)
							break;
						names[iName++] = buffer;
						buffer = new char[MAX_PATH];
						iBuffer = 0;
					}

				}

				MessageBox(NULL, names[0], names[1], NULL);
			}
			break;
		}
		case ID_FILE_EXIT: {
			destroy();
			break;
		}
		default:
			break;
		}
	}

	void OnResize(ResizeState resizeState, SIZE newSize) {
		if (filterList != nullptr) {
			filterList->setSize(newSize.cx * 0.2f, newSize.cy);
		}

		if (scene != nullptr) {
			scene->setPosition(newSize.cx * 0.2f, 0);
			scene->setSize(newSize.cx * 0.6f, newSize.cy);
		}

		if (editor != nullptr) {
			editor->setPosition(newSize.cx * 0.8f, 0);
			editor->setSize(newSize.cx * 0.2f, newSize.cy);
		}
	}

protected:
	BEGIN_MSG_MAP(MainWindow, RenderWindow)
	case WM_DESTROY: {
		PostQuitMessage(0);
		break;
	}
	case WM_CLOSE: {
		destroy();
		break;
	}
	END_MSG_MAP()

private:
	FilterList* filterList;
	Scene* scene;
	Editor* editor;
};

int main(int argc, char* argv[]) {
	MainWindow* window = new MainWindow();
	window->show();

	while (window->isOpen()) {
		window->processEvents();
	}

	return 0;
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	return main(0, nullptr);
}