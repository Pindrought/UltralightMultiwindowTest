#include "MyApp.h"
#include <Windows.h>
#include <string>

#define WINDOW_WIDTH  600
#define WINDOW_HEIGHT 400

MyApp::MyApp() {
  ///
  /// Create our main App instance.
  ///
  app_ = App::Create();

  ///
  /// Create a resizable window by passing by OR'ing our window flags with
  /// kWindowFlags_Resizable.
  ///
  window_ = Window::Create(app_->main_monitor(), WINDOW_WIDTH, WINDOW_HEIGHT,
    false, kWindowFlags_Titled | kWindowFlags_Resizable);

  ///
  /// Create our HTML overlay-- we don't care about its initial size and
  /// position because it'll be calculated when we call OnResize() below.
  ///
  overlay_ = Overlay::Create(window_, 1, 1, 0, 0);

  ///
  /// Force a call to OnResize to perform size/layout of our overlay.
  ///
  OnResize(window_.get(), window_->width(), window_->height());

  ///
  /// Load a page into our overlay's View
  ///
  overlay_->view()->LoadURL("file:///app.html");

  ///
  /// Register our MyApp instance as an AppListener so we can handle the
  /// App's OnUpdate event below.
  ///
  app_->set_listener(this);

  ///
  /// Register our MyApp instance as a WindowListener so we can handle the
  /// Window's OnResize event below.
  ///
  window_->set_listener(this);

  ///
  /// Register our MyApp instance as a LoadListener so we can handle the
  /// View's OnFinishLoading and OnDOMReady events below.
  ///
  overlay_->view()->set_load_listener(this);

  ///
  /// Register our MyApp instance as a ViewListener so we can handle the
  /// View's OnChangeCursor and OnChangeTitle events below.
  ///
  overlay_->view()->set_view_listener(this);
}

MyApp::~MyApp() {
}

void MyApp::Run() {
  app_->Run();
}

void MyApp::OnUpdate() {
  ///
  /// This is called repeatedly from the application's update loop.
  ///
  /// You should update any app logic here.
  ///
    if (GetAsyncKeyState('D')) //If D is pressed
    {
        while (GetAsyncKeyState('D')) //Wait until D is no longer pressed
        {
            Sleep(1);
        }
        for (int i = 0; i < 30; i++)
        {
            SpawnTestWindow();
        }
    }
    if (GetAsyncKeyState('R')) //If R is pressed
    {
        while (GetAsyncKeyState('R')) //Wait until R is no longer pressed
        {
            Sleep(1);
        }
        SpawnTestWindow();
    }
    if (GetAsyncKeyState('V')) //If V is pressed
    {
        while (GetAsyncKeyState('V')) //Wait until V is no longer pressed
        {
            Sleep(1);
        }
        for (int i = m_WindowsVec.size()-1; i >= 0; i--) //Close all custom spawned windows
        {
            m_WindowsVec[i]->Close();
        }
    }
    if (GetAsyncKeyState(VK_F1)) //If F1 is pressed
    {
        while (GetAsyncKeyState(VK_F1)) //Wait until F1 is no longer pressed
        {
            Sleep(1);
        }
        static int purgeMemoryCount = 0;
        purgeMemoryCount += 1;
        std::string purgeString = "Purge Count [";
        purgeString += std::to_string(purgeMemoryCount);
        purgeString += "]";
        window_->SetTitle(purgeString.c_str());
        OutputDebugStringA("PurgeMemory()\n");
        app_->renderer()->PurgeMemory();
    }
}

void MyApp::OnClose(ultralight::Window* window) {
    if (window == this->window_)
    {
        app_->Quit();
    }
    else
    {
        int index = -1;
        for (int i = 0; i < m_WindowsVec.size(); i++)
        {
            if (window == m_WindowsVec[i])
            {
                index = i;
            }
        }
        m_OverlaysVec.erase(m_OverlaysVec.begin() + index);
        m_WindowsVec.erase(m_WindowsVec.begin() + index);
    }
}

void MyApp::OnResize(ultralight::Window* window, uint32_t width, uint32_t height) {
  ///
  /// This is called whenever the window changes size (values in pixels).
  ///
  /// We resize our overlay here to take up the entire window.
  ///
  overlay_->Resize(width, height);
}

void MyApp::OnFinishLoading(ultralight::View* caller,
                            uint64_t frame_id,
                            bool is_main_frame,
                            const String& url) {
  ///
  /// This is called when a frame finishes loading on the page.
  ///
}

void MyApp::OnDOMReady(ultralight::View* caller,
                       uint64_t frame_id,
                       bool is_main_frame,
                       const String& url) {
  ///
  /// This is called when a frame's DOM has finished loading on the page.
  ///
  /// This is the best time to setup any JavaScript bindings.
  ///
}

void MyApp::OnChangeCursor(ultralight::View* caller,
                           Cursor cursor) {
  ///
  /// This is called whenever the page requests to change the cursor.
  ///
  /// We update the main window's cursor here.
  ///
  window_->SetCursor(cursor);
}

void MyApp::OnChangeTitle(ultralight::View* caller,
                          const String& title) {
  ///
  /// This is called whenever the page requests to change the title.
  ///
  /// We update the main window's title here.
  ///
  window_->SetTitle(title.utf8().data());
}

void MyApp::SpawnTestWindow()
{
    static int windowId = 0;
    windowId += 1;
    auto newWindow = Window::Create(app_->main_monitor(), WINDOW_WIDTH, WINDOW_HEIGHT,
                                    false, kWindowFlags_Titled | kWindowFlags_Resizable);
    newWindow->set_listener(this);
    std::string windowTitle = "Window [";
    windowTitle += std::to_string(windowId);
    windowTitle += "]";
    newWindow->SetTitle(windowTitle.c_str());
    auto newOverlay = Overlay::Create(newWindow, newWindow->width(), newWindow->height(), 0, 0);
    newOverlay->view()->set_load_listener(this);
    newOverlay->view()->set_view_listener(this);
    newOverlay->view()->LoadURL("http://www.google.com");
    m_WindowsVec.push_back(newWindow);
    m_OverlaysVec.push_back(newOverlay);
}
