include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/code
  ${CEF_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIR}
  ${GTK2_INCLUDE_DIRS}
  ${IPC_PIPE_INCLUDE_DIRS}
  ${wxWidgets_INCLUDE_DIRS}
)

file(GLOB Sources       code/AboutForm.cpp
                        code/AboutPageDev.cpp
                        code/AboutPageMain.cpp
                        code/BaseInstallPage.cpp
                        code/BaseMenuButton.cpp
                        code/BasePage.cpp
                        code/BaseTabPage.cpp
                        code/BaseToolBarControl.cpp
                        code/BreadCrumb.cpp
                        code/ButtonStrip.cpp
                        code/CDKeyForm.cpp
                        code/CDKInfo.cpp
                        code/CDKProgress.cpp
                        code/ChangeLogForm.cpp
                        code/ComplexPrompt.cpp
                        code/Console.cpp
                        code/CreateForm.cpp
                        code/CreateInfoPage.cpp
                        code/CreateOVPage.cpp
                        code/CreateProgPage.cpp
                        code/DesuraControl.cpp
                        code/DesuraServiceError.cpp
                        code/DispLoading.cpp
                        code/DStripMenuControls.cpp
                        code/EulaForm.cpp
                        code/ExeSelectForm.cpp
                        code/GameDiscForm.cpp
                        code/gcImgLoader.cpp
                        code/gcJSBinding.cpp
                        code/gcJSBranchInfo.cpp
                        code/gcJSEvents.cpp
                        code/gcJSItemInfo.cpp
                        code/gcJSLinks.cpp
                        code/gcJSSettings.cpp
                        code/gcJSUploads.cpp
                        code/gcJSUserInfo.cpp
                        code/gcMiscWebControl.cpp
                        code/gcSchemeBase.cpp
                        code/gcThemeLoader.cpp
                        code/gcUpdateForm.cpp
                        code/gcWCEvents.cpp
                        code/gcWCEvents_js.cpp
                        code/gcWCUtil.cpp
                        code/gcWebControl.cpp
                        code/gcWebFakeBrowser.cpp
                        code/HtmlTabPage.cpp
                        code/HtmlToolBarControl.cpp
                        code/ICheckFinishPage.cpp
                        code/ICheckProgressPage.cpp
                        code/InstallBannerPage.cpp
                        code/InstallBranch.cpp
                        code/InstallDLPage.cpp
                        code/InstallDLToolPage.cpp
                        code/InstallDVPage.cpp
                        code/InstallGIPage.cpp
                        code/InstallINCPage.cpp
                        code/InstallINPage.cpp
                        code/InstallINToolPage.cpp
                        code/InstallPrompt.cpp
                        code/InstallVInfoPage.cpp
                        code/InstallVIPage.cpp
                        code/InstallWaitPage.cpp
                        code/InternalLink.cpp
                        code/ItemForm.cpp
                        code/ItemTabPage.cpp
                        code/ItemToolBarControl.cpp
                        code/ItemUpdateForm.cpp
                        code/LaunchPrompt.cpp
                        code/Log.cpp
                        code/LoginForm.cpp
                        code/MainApp.cpp
                        code/MainApp_cvar.cpp
						code/MainApp_internal.cpp
                        code/MainApp_wildcards.cpp
                        code/MainForm.cpp
                        code/MainMenuButton.cpp
                        code/Managers.cpp
                        code/MenuFiller.cpp
                        code/MenuSeperator.cpp
                        code/MenuStrip.cpp
                        code/ModWizard.cpp
                        code/ModWizardFinPage.cpp
                        code/ModWizardInfoPage.cpp
                        code/ModWizardProgPage.cpp
                        code/NewAccountDialog.cpp
                        code/NewsForm.cpp
                        code/PasswordReminder.cpp
                        code/PreloadPage.cpp
                        code/SearchControl.cpp
                        code/SteamUser.cpp
                        code/StripMenuButton.cpp
                        code/TabButton.cpp
                        code/TabControl.cpp
                        code/TaskBarIcon_Ballon.cpp
                        code/TaskBarIcon.cpp
                        code/TaskBarIcon_Icon.cpp
                        code/TBI_BaseMenu.cpp
                        code/TBI_GameMenu.cpp
                        code/TBI_ModMenu.cpp
                        code/TBI_UpdateMenu.cpp
                        code/TBI_WindowMenu.cpp
                        code/UICoreEntry.cpp
                        code/UICoreMain.cpp
                        code/UninstallInfoPage.cpp
                        code/UninstallProgressPage.cpp
                        code/UploadForm.cpp
                        code/UploadInfoPage.cpp
                        code/UploadProgPage.cpp
                        code/UsernameBox.cpp
                        code/*.h
						code/AltLoginDialog.cpp
						code/LanguageTestDialog.cpp)

if(WIN32)
  file(GLOB PlattformSources
                        code/CefIPCPipeClient.cpp
                        code/ChromiumMenuInfoFromMem.cpp
                        code/gcWCEvents_win.cpp
                        code/gcWebHost.cpp
)
# code/HeaderButton.cpp code/UninstallForm.cpp code/TabHandler.cpp
else()
  file(GLOB PlattformSources)
endif()

if(WITH_GTEST)
  file(GLOB Gtest_Sources code/gcUnitTestPage.cpp)
endif()

if(NOT WIN32)
	add_definitions(-DUI_HIDE_MODS)
endif()

add_definitions(-DENABLE_SEARCH)

if(WIN32)
  add_definitions(-D_WINDOWS
	              -DSVENHACK
				  -DINCLUDE_CVAR_CONCOMMAND
				  -DINCLUDE_WXWIDGETS
				  -DDESURA_CLIENT
				  -DWXUSINGDLL)
  set(WINDOWS_LIBRARIES )
  if(MINGW)
    set(WINDOWS_LIBRARIES ${WINDOWS_LIBRARIES})
  else()
    set(WINDOWS_LIBRARIES ${WINDOWS_LIBRARIES} comctl32 comsuppwd odbc32 rpcrt4 Wininet winmm wsock32)
  endif()
  use_unicode_here()
endif()

set(AllSources ${Sources} ${PlattformSources} ${Gtest_Sources} ${BROWSER_PIPE_SPOURCES})

setup_precompiled_header(Common.h ${COMMON_INCLUDE_DIRS}/Common.cpp AllSources)
add_library(uicore SHARED ${AllSources})

target_link_libraries(uicore
    gcJSBase
    managers
    mcfcore
    ipc_pipe
    threads
    umcf
    util
    util_fs
    usercore
    webcore
    wx_controls
    ${LIBNOTIFY_LIBRARIES}
    ${SQLITE_LIBRARIES}
    ${SQLITE3X_LIBRARIES}
    ${TINYXML_LIBRARIES}
    ${wxWidgets_LIBRARIES}
    ${WINDOWS_LIBRARIES}
)

add_dependencies(uicore wxWidgets tinyxml2 sqlite sqlite3x)

if(WIN32)
  SetSharedRuntime(uicore)
endif()

link_with_gtest(uicore)
install_library(uicore)
