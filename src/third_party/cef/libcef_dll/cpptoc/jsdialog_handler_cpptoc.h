// Copyright (c) 2011 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
//
// ---------------------------------------------------------------------------
//
// This file was generated by the CEF translator tool and should not edited
// by hand. See the translator.README.txt file in the tools directory for
// more information.
//
#ifndef _JSDIALOGHANDLER_CPPTOC_H
#define _JSDIALOGHANDLER_CPPTOC_H

#ifndef USING_CEF_SHARED
#pragma message("Warning: "__FILE__" may be accessed wrapper-side only")
#else // USING_CEF_SHARED

#include "include/cef.h"
#include "include/cef_capi.h"
#include "libcef_dll/cpptoc/cpptoc.h"

// Wrap a C++ class with a C structure.
// This class may be instantiated and accessed wrapper-side only.
class CefJSDialogHandlerCppToC
    : public CefCppToC<CefJSDialogHandlerCppToC, CefJSDialogHandler,
        cef_jsdialog_handler_t>
{
public:
  CefJSDialogHandlerCppToC(CefJSDialogHandler* cls);
  virtual ~CefJSDialogHandlerCppToC() {}
};

#endif // USING_CEF_SHARED
#endif // _JSDIALOGHANDLER_CPPTOC_H

