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

#include "Common.h"
#include "IPCInstallMcf.h"
#include "IPCManager.h"

#ifndef DESURA_CLIENT
	#include "InstallProcess.h"
#endif

REG_IPC_CLASS( IPCInstallMcf );

IPCInstallMcf::IPCInstallMcf(IPC::IPCManager* mang, uint32 id, DesuraId itemId) : IPC::IPCClass(mang, id, itemId)
{
	registerFunctions();
	m_pThread = nullptr;
}

IPCInstallMcf::~IPCInstallMcf()
{
#ifndef DESURA_CLIENT
	safe_delete(m_pThread);
#endif
}

void IPCInstallMcf::registerFunctions()
{
#ifndef DESURA_CLIENT
	REG_FUNCTION_VOID( IPCInstallMcf, pause );
	REG_FUNCTION_VOID( IPCInstallMcf, unpause );
	REG_FUNCTION_VOID( IPCInstallMcf, stop );
	REG_FUNCTION_VOID( IPCInstallMcf, start );

	LINK_EVENT( onProgressIPCEvent, uint64 );
	LINK_EVENT( onErrorEvent, gcException );
	LINK_EVENT_VOID( onCompleteEvent );
#else
	REG_EVENT( onProgressIPCEvent );
	REG_EVENT( onErrorEvent );
	REG_EVENT_VOID( onCompleteEvent );

	onProgressIPCEvent += delegate(this, &IPCInstallMcf::onProgress);
#endif
}


#ifdef DESURA_CLIENT
void IPCInstallMcf::start(const char* mcfpath, const char* inspath, const char* installScript, uint8 workercount, bool delfiles, bool makeWriteable)
{
	IPC::functionCallV(this, "start", mcfpath, inspath, installScript, workercount, delfiles, makeWriteable);
}

void IPCInstallMcf::pause()
{
	IPC::functionCallAsync(this, "pause");
}

void IPCInstallMcf::unpause()
{
	IPC::functionCallAsync(this, "unpause");
}

void IPCInstallMcf::stop()
{
	IPC::functionCallAsync(this, "stop");
}


void IPCInstallMcf::onProgress(uint64& prog)
{
	MCFCore::Misc::ProgressInfo pi(prog);
	onProgressEvent(pi);
}

#else

void IPCInstallMcf::start(const char* mcfpath, const char* inspath, const char* installScript, uint8 workercount, bool delfiles, bool makeWriteable)
{
	if (m_pThread)
		return;

	m_pThread = new InstallProcess(mcfpath, inspath, installScript, workercount, delfiles, makeWriteable);
	m_pThread->onErrorEvent += delegate(&onErrorEvent);
	m_pThread->onCompleteEvent += delegate(&onCompleteEvent);
	m_pThread->onProgressEvent += delegate(&onProgressIPCEvent);
	m_pThread->onFinishEvent += delegate(&onFinishEvent);

	m_pThread->start();
}

void IPCInstallMcf::pause()
{
	if (m_pThread)
		m_pThread->pause();
}

void IPCInstallMcf::unpause()
{
	if (m_pThread)
		m_pThread->unpause();
}

void IPCInstallMcf::stop()
{
	if (m_pThread)
		m_pThread->nonBlockStop();
}


#endif

