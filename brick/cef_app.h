// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_TESTS_CEFSIMPLE_SIMPLE_APP_H_
#define CEF_TESTS_CEFSIMPLE_SIMPLE_APP_H_

#include <set>
#include <include/cef_request_handler.h>

#include "include/cef_app.h"
#include "brick_app.h"

class ClientApp : public CefApp,
                  public CefBrowserProcessHandler,
                  public CefRenderProcessHandler {
 public:
  class RenderDelegate : public virtual CefBase {
  public:
    virtual void OnRenderThreadCreated(CefRefPtr<ClientApp> app,
       CefRefPtr<CefListValue> extra_info) {}

    virtual void OnWebKitInitialized(CefRefPtr<ClientApp> app) {}

    virtual void OnBrowserCreated(CefRefPtr<ClientApp> app,
       CefRefPtr<CefBrowser> browser) {}

    virtual void OnBrowserDestroyed(CefRefPtr<ClientApp> app,
       CefRefPtr<CefBrowser> browser) {}

    virtual CefRefPtr<CefLoadHandler> GetLoadHandler(CefRefPtr<ClientApp> app) {
      return NULL;
    }

    virtual bool OnBeforeNavigation(CefRefPtr<ClientApp> app,
       CefRefPtr<CefBrowser> browser,
       CefRefPtr<CefFrame> frame,
       CefRefPtr<CefRequest> request,
       cef_navigation_type_t navigation_type,
       bool is_redirect) {
      return false;
    }

    virtual void OnContextCreated(CefRefPtr<ClientApp> app,
       CefRefPtr<CefBrowser> browser,
       CefRefPtr<CefFrame> frame,
       CefRefPtr<CefV8Context> context) {}

    virtual void OnContextReleased(CefRefPtr<ClientApp> app,
       CefRefPtr<CefBrowser> browser,
       CefRefPtr<CefFrame> frame,
       CefRefPtr<CefV8Context> context) {}

    virtual void OnUncaughtException(CefRefPtr<ClientApp> app,
       CefRefPtr<CefBrowser> browser,
       CefRefPtr<CefFrame> frame,
       CefRefPtr<CefV8Context> context,
       CefRefPtr<CefV8Exception> exception,
       CefRefPtr<CefV8StackTrace> stackTrace) {}

    virtual void OnFocusedNodeChanged(CefRefPtr<ClientApp> app,
       CefRefPtr<CefBrowser> browser,
       CefRefPtr<CefFrame> frame,
       CefRefPtr<CefDOMNode> node) {}

    // Called when a process message is received. Return true if the message was
    // handled and should not be passed on to other handlers. RenderDelegates
    // should check for unique message names to avoid interfering with each
    // other.
    virtual bool OnProcessMessageReceived(
       CefRefPtr<ClientApp> app,
       CefRefPtr<CefBrowser> browser,
       CefProcessId source_process,
       CefRefPtr<CefProcessMessage> message) {
      return false;
    }
  };

  typedef std::set<CefRefPtr<RenderDelegate> > RenderDelegateSet;

  ClientApp();

  // CefApp methods:
  virtual void OnRegisterCustomSchemes(
     CefRefPtr<CefSchemeRegistrar> registrar) OVERRIDE;
  virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler()
      OVERRIDE { return this; }
  virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler()
  OVERRIDE { return this; }
  virtual void OnBeforeCommandLineProcessing( const CefString& process_type,
     CefRefPtr<CefCommandLine> command_line) OVERRIDE;

  // CefBrowserProcessHandler methods:
  virtual void OnContextInitialized() OVERRIDE;
  virtual void OnWebKitInitialized() OVERRIDE;

  typedef std::map<int32, std::pair< CefRefPtr<CefV8Context>, CefRefPtr<CefV8Value> > > CallbackMap;

  void AddCallback(int32 id, CefRefPtr<CefV8Context> context, CefRefPtr<CefV8Value> callbackFunction) {
    callback_map_[id] = std::make_pair(context, callbackFunction);
  }

  // Called when a process message is received. Return true if the message was
  // handled and should not be passed on to other handlers. RenderDelegates
  // should check for unique message names to avoid interfering with each
  // other.
  virtual bool OnProcessMessageReceived(
     CefRefPtr<CefBrowser> browser,
     CefProcessId source_process,
     CefRefPtr<CefProcessMessage> message) OVERRIDE;

  std::string GetExtensionJSSource();

  // Creates all of the RenderDelegate objects. Implemented in
  // cef_app_delegates.
//  static void CreateRenderDelegates(RenderDelegateSet& delegates);

 private:
// CefRenderProccessHandler methods:
  virtual bool OnBeforeNavigation(CefRefPtr<CefBrowser> browser,
     CefRefPtr<CefFrame> frame,
     CefRefPtr<CefRequest> request,
     NavigationType navigation_type,
     bool is_redirect) OVERRIDE;

  // Set of supported RenderDelegates.
  RenderDelegateSet render_delegates_;

  // Set of callbacks
  CallbackMap callback_map_;

  // Schemes that will be registered with the global cookie manager. Used in
  // both the browser and renderer process.
  std::vector<CefString> cookieable_schemes_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(ClientApp);
};

#endif  // CEF_TESTS_CEFSIMPLE_SIMPLE_APP_H_
