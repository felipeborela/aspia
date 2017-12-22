//
// PROJECT:         Aspia
// FILE:            ui/system_info/system_info_window.h
// LICENSE:         Mozilla Public License Version 2.0
// PROGRAMMERS:     Dmitry Chapyshev (dmitry@aspia.ru)
//

#ifndef _ASPIA_UI__SYSTEM_INFO__SYSTEM_INFO_WINDOW_H
#define _ASPIA_UI__SYSTEM_INFO__SYSTEM_INFO_WINDOW_H

#include "base/message_loop/message_loop_thread.h"
#include "report/report_creator.h"
#include "ui/system_info/system_info_toolbar.h"
#include "ui/system_info/category_tree_ctrl.h"
#include "ui/system_info/info_list_ctrl.h"
#include "ui/base/splitter.h"
#include "ui/resource.h"

namespace aspia {

class SystemInfoWindow
    : public CWindowImpl<SystemInfoWindow, CWindow, CFrameWinTraits>,
      private MessageLoopThread::Delegate,
      private MessageLoop::Dispatcher
{
public:
    class Delegate
    {
    public:
        virtual ~Delegate() = default;

        virtual void OnWindowClose() = 0;
        virtual void OnRequest(std::string_view guid,
                               std::shared_ptr<ReportCreatorProxy> report_creater) = 0;
    };

    SystemInfoWindow(Delegate* delegate);
    ~SystemInfoWindow();

private:
    // MessageLoopThread::Delegate implementation.
    void OnBeforeThreadRunning() override;
    void OnThreadRunning(MessageLoop* message_loop) override;
    void OnAfterThreadRunning() override;

    // MessageLoop::Dispatcher implementation.
    bool Dispatch(const NativeEvent& event) override;

    static const int kTreeControl = 100;
    static const int kListControl = 101;

    BEGIN_MSG_MAP(SystemInfoWindow)
        MESSAGE_HANDLER(WM_CREATE, OnCreate)
        MESSAGE_HANDLER(WM_SIZE, OnSize)
        MESSAGE_HANDLER(WM_GETMINMAXINFO, OnGetMinMaxInfo)
        MESSAGE_HANDLER(WM_CLOSE, OnClose)
        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)

        NOTIFY_HANDLER(kTreeControl, TVN_SELCHANGED, OnCategorySelected)
        NOTIFY_HANDLER(kListControl, NM_RCLICK, OnListRightClick)
        NOTIFY_CODE_HANDLER(TBN_DROPDOWN, OnToolBarDropDown)

        COMMAND_ID_HANDLER(ID_SAVE_SELECTED, OnSaveSelectedButton)
        COMMAND_ID_HANDLER(ID_SAVE, OnSaveSelectedButton)
        COMMAND_ID_HANDLER(ID_SAVE_ALL, OnSaveAllButton)
        COMMAND_ID_HANDLER(ID_SAVE_CURRENT, OnSaveCurrentButton)
        COMMAND_ID_HANDLER(ID_SCREENSHOT, OnScreenshotButton)
        COMMAND_ID_HANDLER(ID_REFRESH, OnRefreshButton)
        COMMAND_ID_HANDLER(ID_COPY, OnCopyButton)
        COMMAND_ID_HANDLER(ID_COPY_ALL, OnCopyAllButton)
        COMMAND_ID_HANDLER(ID_COPY_VALUE, OnCopyValueButton)
        COMMAND_ID_HANDLER(ID_ABOUT, OnAboutButton)
        COMMAND_ID_HANDLER(ID_EXIT, OnExitButton)
    END_MSG_MAP()

    LRESULT OnCreate(UINT message, WPARAM wparam, LPARAM lparam, BOOL& handled);
    LRESULT OnDestroy(UINT message, WPARAM wparam, LPARAM lparam, BOOL& handled);
    LRESULT OnSize(UINT message, WPARAM wparam, LPARAM lparam, BOOL& handled);
    LRESULT OnGetMinMaxInfo(UINT message, WPARAM wparam, LPARAM lparam, BOOL& handled);
    LRESULT OnClose(UINT message, WPARAM wparam, LPARAM lparam, BOOL& handled);

    LRESULT OnCategorySelected(int control_id, LPNMHDR hdr, BOOL& handled);
    LRESULT OnListRightClick(int control_id, LPNMHDR hdr, BOOL& handled);
    LRESULT OnToolBarDropDown(int control_id, LPNMHDR hdr, BOOL& handled);

    LRESULT OnSaveSelectedButton(WORD notify_code, WORD control_id, HWND control, BOOL& handled);
    LRESULT OnSaveAllButton(WORD notify_code, WORD control_id, HWND control, BOOL& handled);
    LRESULT OnSaveCurrentButton(WORD notify_code, WORD control_id, HWND control, BOOL& handled);
    LRESULT OnScreenshotButton(WORD notify_code, WORD control_id, HWND control, BOOL& handled);
    LRESULT OnRefreshButton(WORD notify_code, WORD control_id, HWND control, BOOL& handled);
    LRESULT OnCopyButton(WORD notify_code, WORD control_id, HWND control, BOOL& handled);
    LRESULT OnCopyAllButton(WORD notify_code, WORD control_id, HWND control, BOOL& handled);
    LRESULT OnCopyValueButton(WORD notify_code, WORD control_id, HWND control, BOOL& handled);
    LRESULT OnAboutButton(WORD notify_code, WORD control_id, HWND control, BOOL& handled);
    LRESULT OnExitButton(WORD notify_code, WORD control_id, HWND control, BOOL& handled);

    void ShowDropDownMenu(int button_id, RECT* button_rect);
    void Save(CategoryList* category_list);
    void Refresh(Category* category);
    std::wstring GetListHeaderText();
    void CopyTextToClipboard(const std::wstring& text);
    void OnRequest(std::string_view guid, std::shared_ptr<ReportCreatorProxy> report_creator);

    MessageLoopThread ui_thread_;
    std::shared_ptr<MessageLoopProxy> runner_;

    Delegate* delegate_;

    CategoryList category_list_;

    SystemInfoToolbar toolbar_;
    CategoryTreeCtrl tree_;
    InfoListCtrl list_;
    VerticalSplitter splitter_;
    CStatusBarCtrl statusbar_;

    CAccelerator accelerator_;

    CIcon small_icon_;
    CIcon big_icon_;
    CIcon statusbar_icon_;

    DISALLOW_COPY_AND_ASSIGN(SystemInfoWindow);
};

} // namespace aspia

#endif // _ASPIA_UI__SYSTEM_INFO__SYSTEM_INFO_WINDOW_H
