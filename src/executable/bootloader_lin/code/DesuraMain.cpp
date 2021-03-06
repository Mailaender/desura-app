/*
Desura is the leading indie game distribution platform
Copyright (C) 2011 Mark Chandler (Desura Net Pty Ltd)

$LicenseInfo:firstyear=2014&license=lgpl$
Copyright (C) 2014, Linden Research, Inc.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation;
version 2.1 of the License only.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, see <http://www.gnu.org/licenses/>
or write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

Linden Research, Inc., 945 Battery Street, San Francisco, CA  94111  USA
$/LicenseInfo$
*/

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>

#include <fcntl.h>
#include <unistd.h> // execl()
#include <cstring> // strerror()
#include <cerrno> // errno

#include "UICoreI.h" // UICoreI
#include "MiniDumpGenerator.h"

#include "DesuraMain.h"
#include "UtilFile.h"
#include "util/UtilLinux.h"
#include "util/UtilOs.h"
#include "util/UtilString.h"
#include <branding/branding.h>

#ifdef DESURA_OFFICIAL_BUILD
	int DownloadFilesForTest();
	int InstallFilesForTest();
	bool CheckForUpdate(bool force, bool skip);
#endif

MainApp* g_pMainApp;
SharedObjectLoader g_CrashObject;
UploadCrashFn g_UploadCrashfn;

void SendMessage(const char* msg)
{
    int socketConnect,len;
    struct sockaddr_un remote;

    if ((socketConnect = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    {
        ERROR_OUTPUT("Error setting up socket");
        return;
    }

    remote.sun_family = AF_UNIX;
    strcpy(remote.sun_path, UTIL::LIN::SOCK_PATH());
    len = strlen(remote.sun_path) + sizeof(remote.sun_family);
    if (connect(socketConnect, (struct sockaddr*)&remote, len) == -1)
    {
        ERROR_OUTPUT("Error connecting to socket");
        return;
    }

    if (send(socketConnect, msg, strlen(msg), 0) == -1)
    {
        ERROR_OUTPUT("Error sending data");
    	close(socketConnect);
        return;
    }
    else
    {
    	close(socketConnect);
        return;
    }

    close(socketConnect);
}

void ShowHelpDialog(const char* msg, const char* url, const char* type)
{
	std::string zenityString;
	zenityString += "--text=";
	zenityString += msg;

	execlp("zenity", "zenity", type, zenityString.c_str(), NULL);
	execlp("kdialog", "kdialog", type, msg, NULL);
	execlp("gxmessage", "gxmessage", "-buttons", "GTK_STOCK_OK", "-center", "-default", "GTK_STOCK_OK", "-title", "Information", "-wrap", msg, NULL);
	execlp("gmessage", "gmessage", "-buttons", "GTK_STOCK_OK", "-center", "-default", "OK", "-title", "Information", "-wrap", msg, NULL);
	execlp("xmessage", "xmessage", "-center", msg, NULL);
	fprintf(stderr, "%s\n", msg);

	if (url)
	{
		std::string systemString("xdg-open ");
		systemString += url;

		system(systemString.c_str());
	}
}

int main(int argc, char** argv)
{
	UTIL::LIN::setupXDGVars();
	setenv("CEF_DATA_PATH", UTIL::STRING::toStr(UTIL::OS::getDataPath()).c_str(), 0);

	MainApp app(argc, argv);
	g_pMainApp = &app;
	
	if(!app.testDeps())
		return 1;

	return app.run();
}

MainApp::MainApp(int argc, char** argv)
{
	m_Argc = argc;
	m_Argv = argv;
	
	m_pUICore = NULL;
	strcpy(m_szUser, "linux_Unknown");
}

MainApp::~MainApp()
{
}

int MainApp::run()
{
	ERROR_OUTPUT("#########################################");
	ERROR_OUTPUT("#               DEBUG BUILD             #");
	ERROR_OUTPUT("#########################################");

	if (!ChangeToAppDir())
		return -1;

	bool usingGDB = false;
#ifdef WITH_GTEST
	bool runGtest = false;
#endif

#ifdef DESURA_OFFICIAL_BUILD
	bool forceUpdate = false;
	bool skipUpdate = false;
	bool testDownload = false;
	bool testInstall = false;
#endif

	for (int x=0; x<m_Argc; x++)
	{
		if (!m_Argv[x])
			continue;

		if (strcasecmp(m_Argv[x], "-g") == 0 || strcasecmp(m_Argv[x], "--gdb") == 0)
			usingGDB = true;

#ifdef WITH_GTEST
		if (strcasecmp(m_Argv[x], "-u") == 0 || strcasecmp(m_Argv[x], "--unittests") == 0)
			runGtest = true;
#endif

#ifdef DESURA_OFFICIAL_BUILD
		if (strcasecmp(m_Argv[x], "-td") == 0 || strcasecmp(m_Argv[x], "--testdownload") == 0)
			testDownload = true;

		if (strcasecmp(m_Argv[x], "-ti") == 0 || strcasecmp(m_Argv[x], "--testinstall") == 0)
			testInstall = true;

		if (strcasecmp(m_Argv[x], "-s") == 0 || strcasecmp(m_Argv[x], "--skipupdate") == 0 || strncasecmp(m_Argv[x], "desura://", 9) == 0)
			skipUpdate = true;

		if (strcasecmp(m_Argv[x], "-f") == 0 || strcasecmp(m_Argv[x], "--forceupdate") == 0)
			forceUpdate = true;
#endif
	}

#ifdef DESURA_OFFICIAL_BUILD
	if (testInstall)
		return InstallFilesForTest();

	if (testDownload)
		return DownloadFilesForTest();
#endif

#ifndef DEBUG
	std::string lockPath = UTIL::LIN::expandPath("$XDG_RUNTIME_DIR/desura/lock");

	if (!FileExists(lockPath.c_str())) // if desura isn't already running - simple check
	{
#ifdef DESURA_OFFICIAL_BUILD
		if (CheckForUpdate(forceUpdate, skipUpdate))
			return 0;
#endif

		checkUnityWhitelist();
	}
#endif

	if (!loadUICore())
	{
		ERROR_OUTPUT(dlerror());
		return -1;
	}

#ifdef WITH_GTEST
	if (runGtest)
	{
		return m_pUICore->runUnitTests(m_Argc, m_Argv);
	}
#endif

	if (!m_pUICore->singleInstantCheck())
	{
		sendArgs();
		return 0;
	}

#ifndef DEBUG
	if (!loadCrashHelper())
		return -1;
#endif

	m_pUICore->disableSingleInstanceLock();

	if (usingGDB)
	{
		ERROR_OUTPUT("Running with GDB -- Not setting up dump handler");
	}
	else
	{
		MiniDumpGenerator m_MDumpHandle;
		m_MDumpHandle.showMessageBox(true);
		m_MDumpHandle.setCrashCallback(&MainApp::onCrash);	
	}

	return m_pUICore->initWxWidgets(m_Argc, m_Argv);
}

bool MainApp::testDeps()
{
	void* gtkHandle = dlopen("libgtk-x11-2.0.so.0", RTLD_LAZY);

	if (!gtkHandle)
	{
		ShowHelpDialog(	"It appears as though you don't have GTK installed on your system.\n"
					"Please refer to http://www.desura.com/groups/desura/linux#gtk-help for more information.\n\n"
					"If possible, this program has opened the URL in your default browser for you.",
					"http://www.desura.com/groups/desura/linux#gtk-help");
		return false;
	}

	dlclose(gtkHandle);

	if (!utf8Test())
		return false;

	return true;
}

bool MainApp::loadUICore()
{
	if (!g_UICoreDll.load("libuicore.so"))
		return false;

	dlerror(); // reset errors

	UICoreFP UICoreInterface = g_UICoreDll.getFunction<UICoreFP>("GetInterface");

	if (!UICoreInterface || dlerror())
		return false;

	char version[100] = {0};
	snprintf(version, 100, "%d.%d.%d.%d", VERSION_MAJOR, VERSION_MINOR, VERSION_BUILDNO, VERSION_EXTEND);

	m_pUICore = UICoreInterface();

	if (!m_pUICore)
		return false;

	m_pUICore->setDesuraVersion(version);
	m_pUICore->setRestartFunction(&MainApp::restartFromUICore);
	m_pUICore->setCrashDumpSettings(&MainApp::setCrashSettings);

	return true;
}

void MainApp::shutdownUICore()
{
	if (m_pUICore)
		m_pUICore->destroySingleInstanceCheck();
	
	m_pUICore = NULL;
	g_UICoreDll.unload();
	g_UICoreDll = SharedObjectLoader();
}

void MainApp::restartFromUICore(const char* args)
{
	ERROR_OUTPUT("######### RESTARTING ##########");
	
	if (execl("desura", "desura", args, NULL) == 0)
	{
		ERROR_OUTPUT("Call to execl() failed:");
		ERROR_OUTPUT(strerror(errno));
	}
}

bool MainApp::onCrash(const char* path)
{
	if (!path)
	{
		fprintf(stderr, "on crash path is null!");
		return false;
	}
	else
	{
		strcpy(g_pMainApp->m_pCrashArgs->file, path);
	}

	int appid = 0;
	int build = 0;

	FILE* fh = fopen("version", "r");

	if (fh)
	{
		fscanf(fh, "BRANCH=%d\nBUILD=%d", &appid, &build);
		fclose(fh);
	}

	g_pMainApp->m_pCrashArgs->type = 'c';

	strcpy(g_pMainApp->m_pCrashArgs->user, g_pMainApp->m_szUser);
	g_pMainApp->m_pCrashArgs->branch = appid;
	g_pMainApp->m_pCrashArgs->build = build;

	g_pMainApp->processCrash();
	return true;
}

void MainApp::processCrash()
{
#ifndef DEBUG
	if (!m_pCrashArgs)
	{
		ERROR_OUTPUT("No crash args passed!");
		return;
	}

	pid_t pid1 = fork();

	if (pid1 == 0)
	{
		execl("crashdlg", "crashdlg", m_Argv, NULL);
	}
	else
	{
		g_UploadCrashfn(m_pCrashArgs->file, m_pCrashArgs->user, m_pCrashArgs->build, m_pCrashArgs->branch);
		return;
	}
#endif
}

void MainApp::setUser(const char* user)
{
	if (user)
		strcpy(m_szUser, user);
}

void MainApp::setCrashSettings(const char* user, bool upload)
{
	g_pMainApp->setUser(user);
}

bool MainApp::utf8Test()
{
	bool hasUtf8 = false;

	signed char result[PATH_MAX] = {0};
	ssize_t count = readlink("/proc/self/exe", (char*)result, PATH_MAX);

	for (ssize_t x=0; x<count; x++)
	{
		if (result[x] > 0)
			continue;

		hasUtf8 = true;
		break;
	}
	
	if (hasUtf8)
		ShowHelpDialog(PRODUCT_NAME " currently doesnt support running from a directory with UTF8 characters. Please move " PRODUCT_NAME " to a normal directory.", NULL, "--error");

	return !hasUtf8;
}

bool MainApp::loadCrashHelper()
{
	if (!g_CrashObject.load("libcrashuploader.so"))
	{
		fprintf(stderr, "Failed to load dump uploader.\n\t[%s]\n", dlerror());
		return false;
	}

	g_UploadCrashfn = g_CrashObject.getFunction<UploadCrashFn>("UploadCrash");

	if (!g_UploadCrashfn)
	{
		fprintf(stderr, "Failed to find UploadCrash function.\n\t[%s]\n", dlerror());
		return false;
	}
	
	return true;
}

void MainApp::sendArgs()
{
	std::string args;
	
	for (int x=1; x<m_Argc; x++)
	{
		if (!m_Argv[x])
			continue;
		
		if (args.size() != 0)
			args += " "; 
		args +=  "\"";
		args += m_Argv[x];
		args += "\"";	
	}

	SendMessage(args.c_str());
	shutdownUICore();
}

void MainApp::checkUnityWhitelist()
{
	int ret = system("which gsettings 2>/dev/null 1>/dev/null"); // check gsettings exists
	
	if (ret != 0) 
		return;
		
	// if it does
	ret = system("gsettings get com.canonical.Unity.Panel systray-whitelist 2>/dev/null 1>/dev/null"); // check that this schema exists
	
	if (ret != 0) 
		return;
		
	// if it does
	ret = system("gsettings get com.canonical.Unity.Panel systray-whitelist | grep desura 2>/dev/null 1>/dev/null"); // check for desura already being whitelisted
	
	if (ret == 0) 
		return;
		
	// if it's not
	ret = system("gsettings set com.canonical.Unity.Panel systray-whitelist \"`gsettings get com.canonical.Unity.Panel systray-whitelist | sed -e \"s/]/,\\ 'desura']/g\"`\" 2>/dev/null 1>/dev/null");
	
	if (ret == 0)
		ShowHelpDialog(PRODUCT_NAME " has been added to the Unity panel whitelist. You should log out and back in for this to take effect or you may experience problems using " PRODUCT_NAME, NULL, "--info"); 
}
