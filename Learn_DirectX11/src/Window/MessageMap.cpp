﻿/**
 * @file MessageMap.cpp
 * @author Lhxl
 * @date 2025-2-5
 * @version build7
 */

#include <sstream>
#include <iomanip>

#include "MessageMap.h"
#include "../function.h"

#define WM_UAHDESTROYWINDOW			0x0090
#define WM_UAHDRAWMENU				0x0091
#define WM_UAHDRAWMENUITEM			0x0092
#define WM_UAHINITMENU				0x0093
#define WM_UAHMEASUREMENUITEM		0x0094
#define WM_UAHUAHNCPAINTMENUPOPUP	0x0095

#define REGISTER_MESSAGE(msg) {msg, #msg}

MessageMap::MessageMap()
	: _map({ 
	REGISTER_MESSAGE(WM_NULL),
	REGISTER_MESSAGE(WM_CREATE),
	REGISTER_MESSAGE(WM_DESTROY),
	REGISTER_MESSAGE(WM_MOVE),
	REGISTER_MESSAGE(WM_SIZE),
	REGISTER_MESSAGE(WM_ACTIVATE),
	REGISTER_MESSAGE(WM_SETFOCUS),
	REGISTER_MESSAGE(WM_KILLFOCUS),
	REGISTER_MESSAGE(WM_ENABLE),
	REGISTER_MESSAGE(WM_SETREDRAW),
	REGISTER_MESSAGE(WM_SETTEXT),
	REGISTER_MESSAGE(WM_GETTEXT),
	REGISTER_MESSAGE(WM_GETTEXTLENGTH),
	REGISTER_MESSAGE(WM_PAINT),
	REGISTER_MESSAGE(WM_CLOSE),
	REGISTER_MESSAGE(WM_QUERYENDSESSION),
	REGISTER_MESSAGE(WM_QUIT),
	REGISTER_MESSAGE(WM_QUERYOPEN),
	REGISTER_MESSAGE(WM_ERASEBKGND),
	REGISTER_MESSAGE(WM_SYSCOLORCHANGE),
	REGISTER_MESSAGE(WM_ENDSESSION),
	REGISTER_MESSAGE(WM_SHOWWINDOW),
	REGISTER_MESSAGE(WM_WININICHANGE),
	REGISTER_MESSAGE(WM_DEVMODECHANGE),
	REGISTER_MESSAGE(WM_ACTIVATEAPP),
	REGISTER_MESSAGE(WM_FONTCHANGE),
	REGISTER_MESSAGE(WM_TIMECHANGE),
	REGISTER_MESSAGE(WM_CANCELMODE),
	REGISTER_MESSAGE(WM_SETCURSOR),
	REGISTER_MESSAGE(WM_MOUSEACTIVATE),
	REGISTER_MESSAGE(WM_CHILDACTIVATE),
	REGISTER_MESSAGE(WM_QUEUESYNC),
	REGISTER_MESSAGE(WM_GETMINMAXINFO),
	REGISTER_MESSAGE(WM_PAINTICON),
	REGISTER_MESSAGE(WM_ICONERASEBKGND),
	REGISTER_MESSAGE(WM_NEXTDLGCTL),
	REGISTER_MESSAGE(WM_DELETEITEM),
	REGISTER_MESSAGE(WM_VKEYTOITEM),
	REGISTER_MESSAGE(WM_SETFONT),
	REGISTER_MESSAGE(WM_GETFONT),
	REGISTER_MESSAGE(WM_SETHOTKEY),
	REGISTER_MESSAGE(WM_GETHOTKEY),
	REGISTER_MESSAGE(WM_COMMNOTIFY),
	REGISTER_MESSAGE(WM_WINDOWPOSCHANGING),
	REGISTER_MESSAGE(WM_WINDOWPOSCHANGED),
	REGISTER_MESSAGE(WM_POWER),
	REGISTER_MESSAGE(WM_NOTIFY),
	REGISTER_MESSAGE(WM_HELP),
	REGISTER_MESSAGE(WM_CONTEXTMENU),
	REGISTER_MESSAGE(WM_GETICON),
	REGISTER_MESSAGE(WM_SETICON),
	REGISTER_MESSAGE(WM_NCCREATE),
	REGISTER_MESSAGE(WM_NCDESTROY),
	REGISTER_MESSAGE(WM_NCCALCSIZE),
	REGISTER_MESSAGE(WM_NCHITTEST),
	REGISTER_MESSAGE(WM_NCPAINT),
	REGISTER_MESSAGE(WM_NCACTIVATE),
	REGISTER_MESSAGE(WM_GETDLGCODE),
	REGISTER_MESSAGE(WM_NCMOUSEMOVE),
	REGISTER_MESSAGE(WM_NCLBUTTONDOWN),
	REGISTER_MESSAGE(WM_NCLBUTTONUP),
	REGISTER_MESSAGE(WM_NCLBUTTONDBLCLK),
	REGISTER_MESSAGE(WM_NCRBUTTONDOWN),
	REGISTER_MESSAGE(WM_NCRBUTTONUP),
	REGISTER_MESSAGE(WM_NCRBUTTONDBLCLK),
	REGISTER_MESSAGE(WM_NCMBUTTONDOWN),
	REGISTER_MESSAGE(WM_NCMBUTTONUP),
	REGISTER_MESSAGE(WM_NCMBUTTONDBLCLK),
	REGISTER_MESSAGE(WM_NCXBUTTONDOWN),
	REGISTER_MESSAGE(WM_NCXBUTTONUP),
	REGISTER_MESSAGE(WM_NCXBUTTONDBLCLK),
	REGISTER_MESSAGE(WM_INPUT),
	REGISTER_MESSAGE(WM_KEYDOWN),
	REGISTER_MESSAGE(WM_KEYFIRST),
	REGISTER_MESSAGE(WM_KEYUP),
	REGISTER_MESSAGE(WM_CHAR),
	REGISTER_MESSAGE(WM_DEADCHAR),
	REGISTER_MESSAGE(WM_SYSKEYDOWN),
	REGISTER_MESSAGE(WM_SYSKEYUP),
	REGISTER_MESSAGE(WM_SYSCHAR),
	REGISTER_MESSAGE(WM_SYSDEADCHAR),
	REGISTER_MESSAGE(WM_KEYLAST),
	REGISTER_MESSAGE(WM_IME_ENDCOMPOSITION),
	REGISTER_MESSAGE(WM_IME_COMPOSITION),
	REGISTER_MESSAGE(WM_IME_KEYLAST),
	REGISTER_MESSAGE(WM_INITDIALOG),
	REGISTER_MESSAGE(WM_COMMAND),
	REGISTER_MESSAGE(WM_SYSCOMMAND),
	REGISTER_MESSAGE(WM_TIMER),
	REGISTER_MESSAGE(WM_HSCROLL),
	REGISTER_MESSAGE(WM_VSCROLL),
	REGISTER_MESSAGE(WM_INITMENU),
	REGISTER_MESSAGE(WM_INITMENUPOPUP),
	REGISTER_MESSAGE(WM_MENUSELECT),
	REGISTER_MESSAGE(WM_ENTERIDLE),
	REGISTER_MESSAGE(WM_UNINITMENUPOPUP),
	REGISTER_MESSAGE(WM_CTLCOLOREDIT),
	REGISTER_MESSAGE(WM_CTLCOLORDLG),
	REGISTER_MESSAGE(WM_MOUSEFIRST),
	REGISTER_MESSAGE(WM_MOUSEMOVE),
	REGISTER_MESSAGE(WM_MOUSEWHEEL),
	REGISTER_MESSAGE(WM_MOUSEHWHEEL),
	REGISTER_MESSAGE(WM_ENTERMENULOOP),
	REGISTER_MESSAGE(WM_EXITMENULOOP),
	REGISTER_MESSAGE(WM_NEXTMENU),
	REGISTER_MESSAGE(WM_SIZING),
	REGISTER_MESSAGE(WM_CAPTURECHANGED),
	REGISTER_MESSAGE(WM_MOVING),
	REGISTER_MESSAGE(WM_MDIDESTROY),
	REGISTER_MESSAGE(WM_MDIRESTORE),
	REGISTER_MESSAGE(WM_MDINEXT),
	REGISTER_MESSAGE(WM_MDITILE),
	REGISTER_MESSAGE(WM_MDISETMENU),
	REGISTER_MESSAGE(WM_ENTERSIZEMOVE),
	REGISTER_MESSAGE(WM_EXITSIZEMOVE),
	REGISTER_MESSAGE(WM_DROPFILES),
	REGISTER_MESSAGE(WM_MDIREFRESHMENU),
	REGISTER_MESSAGE(WM_IME_SETCONTEXT),
	REGISTER_MESSAGE(WM_IME_NOTIFY),
	REGISTER_MESSAGE(WM_IME_CONTROL),
	REGISTER_MESSAGE(WM_IME_COMPOSITIONFULL),
	REGISTER_MESSAGE(WM_IME_SELECT),
	REGISTER_MESSAGE(WM_IME_REQUEST),
	REGISTER_MESSAGE(WM_IME_KEYDOWN),
	REGISTER_MESSAGE(WM_IME_KEYUP),
	REGISTER_MESSAGE(WM_NCMOUSEHOVER),
	REGISTER_MESSAGE(WM_MOUSEHOVER),
	REGISTER_MESSAGE(WM_NCMOUSELEAVE),
	REGISTER_MESSAGE(WM_MOUSELEAVE),
	REGISTER_MESSAGE(WM_CUT),
	REGISTER_MESSAGE(WM_COPY),
	REGISTER_MESSAGE(WM_UNDO),
	REGISTER_MESSAGE(WM_HOTKEY),
	REGISTER_MESSAGE(WM_PRINT),
	REGISTER_MESSAGE(WM_PRINTCLIENT),
	REGISTER_MESSAGE(WM_PENWINFIRST),
	REGISTER_MESSAGE(WM_USER),
	REGISTER_MESSAGE(WM_CHOOSEFONT_GETLOGFONT),
	REGISTER_MESSAGE(WM_PSD_GREEKTEXTRECT),
	REGISTER_MESSAGE(WM_CHOOSEFONT_SETLOGFONT)
	}) {

}

LPWSTR MessageMap::operator()(DWORD msg, LPARAM lp, WPARAM wp) const {
	constexpr int firstColWidth = 25;
	const auto i = _map.find(msg);
	std::ostringstream oss;
	if (i != _map.end()) {
		oss << std::left << std::setw(firstColWidth) << i->second << std::right;
	} else {
		std::ostringstream padss;
		padss << "Unkwown message: 0x" << std::hex << msg;
		oss << std::left << std::setw(firstColWidth) << padss.str() << std::right;
	}
	oss << "    LP: 0x" << std::uppercase << std::hex << std::setfill('0') << std::setw(8) << lp;
	oss << "    WP: 0x" << std::uppercase << std::hex << std::setfill('0') << std::setw(8) << wp << std::endl;
	std::string str = oss.str();
	return str2lpwstr(str);
}