#pragma region std_headers
#include <algorithm>
#include <string>
#pragma endregion std_headers

#pragma region cef_headers
#include <include/cef_browser.h>
#include <include/cef_callback.h>
#include <include/cef_frame.h>
#include <include/cef_resource_handler.h>
#include <include/cef_response.h>
#include <include/cef_request.h>
#include <include/cef_scheme.h>
#include <include/wrapper/cef_helpers.h>
#pragma endregion cef_headers

#include "QCefViewDefaultSchemeHandler.h"

namespace QCefViewDefaultSchemeHandler {
//////////////////////////////////////////////////////////////////////////
// handler
SchemeHandler::SchemeHandler(CCefWindow* pQCefWin)
  : pQCefWindow_(pQCefWin)
  , offset_(0)
{}

bool
SchemeHandler::Open(CefRefPtr<CefRequest> request, bool& handle_request, CefRefPtr<CefCallback> callback)
{
  handle_request = false;
  if (pQCefWindow_) {
    CefString cefStrUrl = request->GetURL();
    QString url = QString::fromStdString(cefStrUrl.ToString());
    pQCefWindow_->processUrlRequest(url);
  }
  // no matter whether we have found the handler or not,
  // we don't response this request.
  return false;
}

void
SchemeHandler::GetResponseHeaders(CefRefPtr<CefResponse> response, int64& response_length, CefString& redirectUrl)
{
  CEF_REQUIRE_IO_THREAD();
  // DCHECK(!data_.empty());
  // response->SetMimeType(mime_type_);
  // response->SetStatus(200);
  //// Set the resulting response length
  // response_length = data_.length();
}

bool
SchemeHandler::Skip(int64 bytes_to_skip, int64& bytes_skipped, CefRefPtr<CefResourceSkipCallback> callback)
{
  bytes_skipped = -2;
  return false;
}

bool
SchemeHandler::Read(void* data_out, int bytes_to_read, int& bytes_read, CefRefPtr<CefResourceReadCallback> callback)
{
  bytes_read = 0;
  if (offset_ < data_.length()) {
    // Copy the next block of data into the buffer.
    int transfer_size = min(bytes_to_read, static_cast<int>(data_.length() - offset_));
    memcpy(data_out, data_.c_str() + offset_, transfer_size);
    offset_ += transfer_size;
    bytes_read = transfer_size;
  }

  return bytes_read > 0;
}

void
SchemeHandler::Cancel()
{}

//////////////////////////////////////////////////////////////////////////
// handler factory
//

CefRefPtr<CefResourceHandler>
SchemeHandlerFactory::Create(CefRefPtr<CefBrowser> browser,
                             CefRefPtr<CefFrame> frame,
                             const CefString& scheme_name,
                             CefRefPtr<CefRequest> request)
{
  //
  // TO DO (Get the correct SchemeHandler corresponding to the browser)
  //

  return new SchemeHandler(nullptr);
}

//////////////////////////////////////////////////////////////////////////
bool
RegisterScheme(CefRawPtr<CefSchemeRegistrar> registrar)
{
  return registrar->AddCustomScheme(scheme_name, 0);
}

bool
RegisterSchemeHandlerFactory()
{
  return CefRegisterSchemeHandlerFactory(scheme_name, "", new SchemeHandlerFactory());
}
}