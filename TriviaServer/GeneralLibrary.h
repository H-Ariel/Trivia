#pragma once

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifdef _DEBUG
// check for memory leaks
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
	#define DBG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#else
	#define DBG_NEW new // do not check for memory leaks
#endif


#include <WinSock2.h>
#include <Windows.h>

#include <iostream>
#include <map>
#include <thread>
#include <mutex>
#include <memory>
#include <format>

#include "structures.h"
#include "Exception.hpp"

#pragma comment (lib, "ws2_32.lib")

#define FIND_IN_ARRAY(arr, val) (find(begin(arr), end(arr), val) != end(arr))


enum MessageCodes : uint8_t
{
	Error,
	OK,

	Login = 5,
	Signup,

	Logout,
	CreateRoom,
	GetRooms,
	JoinRoom,
	GetStatistics,

	GetRoomState,
	LeaveRoom, // if the user is admin it also CloseRoom
	StartGame,

	GetQuestion,
	SubmitAnswer,
	GetGameResults,
	LeaveGame
};


// allocate new object as `shared_ptr`
template <class T, class ... Args>
shared_ptr<T> allocNewSharedPtr(Args... args)
{
	return shared_ptr<T>(DBG_NEW T(args...));
}
