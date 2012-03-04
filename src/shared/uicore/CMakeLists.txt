find_package(Boost COMPONENTS date_time REQUIRED)
find_package(GTK2 REQUIRED)
find_package(wxWidgets 2.9 REQUIRED)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/code
  ${GLOBAL_INCLUDE_DIRS}
  ${COMMON_INCLUDE_DIRS}
  ${CEF_INCLUDE_DIRS}

  ${Boost_INCLUDE_DIRS}
  ${GTK2_INCLUDE_DIRS}
  ${IPCPipeIncludeDir}
  ${TINYXML_INCLUDE_DIRS}
  ${wxWidgets_INCLUDE_DIRS}
)

file(GLOB UiCoreHeaders code/*.h)

file(GLOB UiCoreSources code/AboutForm.cpp
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
                        code/FrameButtons.cpp
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
                        code/MainFormCustomFrame.cpp
                        code/MainFormLeftBorder.cpp
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
                        code/UninstallInfoPage.cpp
                        code/UninstallProgressPage.cpp
                        code/UploadForm.cpp
                        code/UploadInfoPage.cpp
                        code/UploadProgPage.cpp
                        code/UploadPrompt.cpp
                        code/UsernameBox.cpp
                        code/*.h)

if(DEFINED WIN32)
  file(GLOB PlattformSources
                        code/CefIPCPipeClient.cpp
                        code/ChromiumMenuInfoFromMem.cpp
                        code/gcWCEvents_win.cpp
                        code/gcWebHost.cpp
                        code/HeaderButton.cpp
                        code/TabHandler.cpp
                        code/UICoreMain.cpp
                        code/UninstallForm.cpp
)
else()
  file(GLOB PlattformSources)
endif()

add_library(uicore SHARED ${UiCoreSources} ${PlattformSources})