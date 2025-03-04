// Copyright 2018-2021 JXMaster. All rights reserved.
/*
* @file Thread.cpp
* @author JXMaster
* @date 2020/7/30
*/
#pragma once
#include <Runtime/PlatformDefines.hpp>

#ifdef LUNA_PLATFORM_WINDOWS

#include "../../OS.hpp"
#include "MiniWin.hpp"

namespace Luna
{
	namespace OS
	{
		struct Thread
		{
			thread_callback_func_t* m_func;
			void* m_params;
		};

		struct Fiber
		{
			LPVOID m_handle;
			thread_callback_func_t* m_func;
			void* m_params;
		};

	}
}

DWORD WINAPI WinThreadEntry(LPVOID cookie)
{
	Luna::OS::Thread* ctx = (Luna::OS::Thread*)cookie;
	ctx->m_func(ctx->m_params);
	memdelete(ctx);
	return 0;
}
VOID __stdcall WinFiberEntry(LPVOID cookie)
{
	Luna::OS::Thread* ctx = (Luna::OS::Thread*)cookie;
	ctx->m_func(ctx->m_params);
}

namespace Luna
{
	namespace OS
	{
		R<handle_t> new_thread(thread_callback_func_t* callback, void* params, usize stack_size)
		{
			luassert(callback);
			Thread* t = memnew<Thread>();
			t->m_func = callback;
			t->m_params = params;
			DWORD tid;
			HANDLE h = ::CreateThread(NULL, stack_size, &WinThreadEntry, t, CREATE_SUSPENDED, &tid);
			if (!h)
			{
				DWORD dw = ::GetLastError();
				return BasicError::bad_system_call;
			}
			::ResumeThread(h);
			return h;
		}
		void wait_thread(handle_t thread)
		{
			if (::WaitForSingleObject(thread, INFINITE) != WAIT_OBJECT_0)
			{
				lupanic_msg_always("WaitForSingleObject failed for thread object");
			}
		}
		RV try_wait_thread(handle_t thread)
		{
			DWORD r = ::WaitForSingleObject(thread, 0);
			if (r == WAIT_OBJECT_0)
			{
				return RV();
			}
			if (r == WAIT_TIMEOUT)
			{
				return BasicError::timeout;
			}
			return BasicError::bad_system_call;
		}
		void detach_thread(handle_t thread)
		{
			::CloseHandle(thread);
		}
		handle_t get_current_thread()
		{
			return ::GetCurrentThread();
		}
		void sleep(u32 time_milliseconds)
		{
			::Sleep(time_milliseconds);
		}
		void fast_sleep(u32 time_microseconds)
		{
			LARGE_INTEGER currentTime;
			::QueryPerformanceCounter(&currentTime);
			LARGE_INTEGER ticksPerSnd;
			::QueryPerformanceFrequency(&ticksPerSnd);
			u64 endTime = currentTime.QuadPart + ticksPerSnd.QuadPart * time_microseconds / 1000000;
			if (time_microseconds > 4000) //4ms
			{
				::Sleep(time_microseconds / 1000);
			}
			::QueryPerformanceCounter(&currentTime);
			while ((u64)(currentTime.QuadPart) < endTime)
			{
				::SwitchToThread();
				::SwitchToThread();
				::SwitchToThread();
				::SwitchToThread();
			}
		}
		void yield_current_thread()
		{
			SwitchToThread();
		}
		R<handle_t> convert_thread_to_fiber()
		{
			LPVOID handle = ::ConvertThreadToFiber(NULL);
			if (!handle)
			{
				return BasicError::bad_system_call;
			}
			return handle;
		}
		RV convert_fiber_to_thread()
		{
			if (!::ConvertFiberToThread())
			{
				return BasicError::bad_system_call;
			}
			return RV();
		}
		R<handle_t> new_fiber(thread_callback_func_t* callback, void* params, usize stack_size)
		{
			luassert(callback);
			if (!stack_size)
			{
				stack_size = (u32)1_mb;
			}
			Fiber* f = memnew<Fiber>();
			f->m_func = callback;
			f->m_handle = ::CreateFiber(stack_size, WinFiberEntry, f);
			if (!f->m_handle)
			{
				memdelete(f);
				return BasicError::bad_system_call;
			}
			return f;
		}

		void delete_fiber(handle_t fiber)
		{
			Fiber* f = (Fiber*)fiber;
			::DeleteFiber(f->m_handle);
			memdelete(f);
		}

		void switch_to_fiber(handle_t fiber)
		{
			Fiber* f = (Fiber*)fiber;
			::SwitchToFiber(f->m_handle);
		}

		R<handle_t> tls_alloc()
		{
			DWORD index = TlsAlloc();
			if (index == TLS_OUT_OF_INDEXES)
			{
				return BasicError::bad_system_call;
			}
			return (handle_t)(usize)index;
		}
		RV tls_free(handle_t handle)
		{
			if (TlsFree((DWORD)(usize)handle))
			{
				return RV();
			}
			return BasicError::bad_system_call;
		}
		RV tls_set(handle_t handle, void* ptr)
		{
			if (TlsSetValue((DWORD)(usize)handle, ptr))
			{
				return RV();
			}
			return BasicError::bad_system_call;
		}
		R<void*> tls_get(handle_t handle)
		{
			void* d = ::TlsGetValue((DWORD)(usize)handle);
			DWORD err = ::GetLastError();
			if (err == ERROR_SUCCESS)
			{
				return d;
			}
			return BasicError::bad_system_call;
		}
	}
}

#endif
