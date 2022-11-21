#ifndef CEF2SDL_HH_
#define CEF2SDL_HH_

#include <SDL2/SDL.h>

#include <cef_app.h>
#include <cef_client.h>
#include <cef_render_handler.h>
#include <cef_life_span_handler.h>
#include <cef_load_handler.h>
#include <wrapper/cef_helpers.h>

namespace cef2sdl {
	class RenderHandler : public CefRenderHandler {
public:
    RenderHandler(SDL_Renderer * renderer, int w, int h) :
        width(w),
        height(h),
        renderer(renderer);

    ~RenderHandler();
	
    bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect);

    void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList &dirtyRects, const void * buffer, int w, int h);

    void resize(int w, int h);

    void render();

private:
    int width;
    int height;
    SDL_Renderer * renderer = nullptr;
    SDL_Texture * texture = nullptr;

    IMPLEMENT_REFCOUNTING(RenderHandler);
};

// for manual render handler
class BrowserClient :
    public CefClient,
    public CefLifeSpanHandler,
    public CefLoadHandler {
public:
    BrowserClient(CefRefPtr<CefRenderHandler> ptr) :
        handler(ptr) {}

    virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler();

    virtual CefRefPtr<CefLoadHandler> GetLoadHandler();

    virtual CefRefPtr<CefRenderHandler> GetRenderHandler();

    // CefLifeSpanHandler methods.
    void OnAfterCreated(CefRefPtr<CefBrowser> browser);

    bool DoClose(CefRefPtr<CefBrowser> browser);

    void OnBeforeClose(CefRefPtr<CefBrowser> browser);

    void OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode);

    bool OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefLoadHandler::ErrorCode errorCode, const CefString & failedUrl, CefString & errorText);

    void OnLoadingStateChange(CefRefPtr<CefBrowser> browser, bool isLoading, bool canGoBack, bool canGoForward);

    void OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame);

    bool closeAllowed() const;

    bool isLoaded() const;

private:
    int browser_id;
    bool closing = false;
    bool loaded = false;
    CefRefPtr<CefRenderHandler> handler;

    IMPLEMENT_REFCOUNTING(BrowserClient);
};

#endif
