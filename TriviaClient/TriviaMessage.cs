using System;
using System.Collections.Generic;
using System.Diagnostics.Contracts;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Text.Json;
using System.Windows.Interop;
using System.Windows;
using System.Numerics;
using System.Runtime.Intrinsics;

namespace TriviaClient
{
	internal class TriviaMessage
	{
		public enum MessageCodes
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
		}



		public TriviaMessage(MessageCodes code = MessageCodes.OK, string data = "")
		{
			this.Code = (byte)code;
			this.Data = data;
		}

		public byte Code { set; get; }
		public string Data { set; get; }



		public delegate void ResponseOkCallback(TriviaMessage msg);


		// call to `callback` if server response is not `Error`
		static public void HandleMessage(TriviaMessage msgToServer, ResponseOkCallback okCallback, bool handleErrors = true)
		{
			TriviaMessage msg = TriviaSocket.SendAndRead(msgToServer);

			switch (msg.Code)
			{
				case (byte)MessageCodes.OK:
					okCallback?.Invoke(msg);
					break;

				case (byte)MessageCodes.Error:
					if (handleErrors)
						MessageBox.Show(ErrorResponse.Parse(msg.Data).message, "Error", MessageBoxButton.OK, MessageBoxImage.Error);
					break;

				default:
					if (handleErrors)
						MessageBox.Show("unknown response", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
					break;
			}
		}
		static public void HandleMessage(TriviaMessage msgToServer, Action okCallback, bool handleErrors = true)
		{
			HandleMessage(msgToServer, TriviaMessage => okCallback?.Invoke(), handleErrors);
		}



		public TriviaMessage(LoginRequest req)
		{
			this.Code = (byte)MessageCodes.Login;
			this.Data = JsonSerializer.Serialize(req);
		}
		public TriviaMessage(SignupRequest req)
		{
			this.Code = (byte)MessageCodes.Signup;
			this.Data = JsonSerializer.Serialize(req);
		}
		public TriviaMessage(CreateRoomRequest req)
		{
			this.Code = (byte)MessageCodes.CreateRoom;
			this.Data = JsonSerializer.Serialize(req);
		}
		public TriviaMessage(JoinRoomRequest req)
		{
			this.Code = (byte)MessageCodes.JoinRoom;
			this.Data = JsonSerializer.Serialize(req);
		}
		public TriviaMessage(SubmitAnswerRequest req)
		{
			this.Code = (byte)MessageCodes.SubmitAnswer;
			this.Data = JsonSerializer.Serialize(req);
		}
	}



	/* Request */

	class LoginRequest
	{
		public LoginRequest(string username, string password)
		{
			this.username = username;
			this.password = password;
		}

		public string username { get; }
		public string password { get; }
	}

	class SignupRequest
	{
		public SignupRequest(string username, string password, string email)
		{
			this.username = username;
			this.password = password;
			this.email = email;
		}

		public string username { get; }
		public string password { get; }
		public string email { get; }
	}

	class CreateRoomRequest
	{
		public CreateRoomRequest(string roomName, int maxUsers, int questionCount, int answerTimeout)
		{
			this.roomName = roomName;
			this.maxUsers = maxUsers;
			this.questionCount = questionCount;
			this.answerTimeout = answerTimeout;
		}

		public string roomName { get; }
		public int maxUsers { get; }
		public int questionCount { get; }
		public int answerTimeout { get; }
	}

	class JoinRoomRequest
	{
		public JoinRoomRequest(int roomId)
		{
			this.roomId = roomId;
		}

		public int roomId { get; }
	}

	class SubmitAnswerRequest
	{
		public SubmitAnswerRequest(int answerId)
		{
			this.answerId = answerId;
		}

		public int answerId { get; }
	}


	/* Response */

	class ErrorResponse
	{
		static public ErrorResponse Parse(string data)
		{
			return JsonSerializer.Deserialize<ErrorResponse>(data); // TODO: use c'tor ?
		}

		public string message { set; get; }
	}

	class RoomData
	{
		public string name { set; get; }
		public int id { set; get; }
	}

	class GetRoomsResponse
	{
		static public GetRoomsResponse Parse(string data)
		{
			return JsonSerializer.Deserialize<GetRoomsResponse>(data);
		}

		public List<RoomData> rooms { set; get; }
	};


	class GetRoomStateResponse
	{
		static public GetRoomStateResponse Parse(string data)
		{
			return JsonSerializer.Deserialize<GetRoomStateResponse>(data);
		}

		public string name { set; get; }
		public List<string> players { set; get; }
		public int questionCount { set; get; }
		public int answerTimeout { set; get; }
		public bool hasGameStarted { set; get; }
		public bool isActive { set; get; }
	}

	class GetQuestionResponse
	{
		static public GetQuestionResponse Parse(string data)
		{
			return JsonSerializer.Deserialize<GetQuestionResponse>(data);
		}

		public string question { set; get; }
		public List<string> answers { set; get; } // the first answer is the correct, the other are wrong
												  //    public bool hasMoreQuestions { set; get; }
	}

	class PlayerResults
	{
		public string username { set; get; }
		public int correctAnswerCount { set; get; }
		public int wrongAnswerCount { set; get; }
		public float averangeAnswerTime { set; get; }
	}
	class GetGameResultsResponse
	{
		static public GetGameResultsResponse Parse(string data)
		{
			return JsonSerializer.Deserialize<GetGameResultsResponse>(data);
		}
		public List<PlayerResults> results { set; get; }
	}
}
