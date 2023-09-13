using System.Threading;
using System.Windows;
using System.Windows.Controls;

namespace TriviaClient
{
	/// <summary>
	/// Interaction logic for Room.xaml
	/// </summary>
	public partial class Room : Window
	{
		readonly bool isAdmin;
		bool RunThread;
		int questionCount, answerTimeout;


		public Room(bool isAdmin = false)
		{
			InitializeComponent();

			this.isAdmin = isAdmin;
			this.Closing += (s, e) => RunThread = false;

			StartGameButton.IsEnabled = isAdmin; // if the user is not admin he can't start the game

			questionCount = 0;
			answerTimeout = 0;

			RunThread = true;
			new Thread(UpdateRoomData).Start();
		}

		private void LeaveRoom(object sender, RoutedEventArgs e)
		{
			if (isAdmin)
			{
				MessageBoxResult result = MessageBox.Show("This action will cause the room to close.\nAre you sure?", "Warning", MessageBoxButton.OKCancel, MessageBoxImage.Warning);
				if (result == MessageBoxResult.Cancel)
					return;
			}

			TriviaMessage.HandleMessage(TriviaMessage.MessageCodes.LeaveRoom, () => WindowsHandler.ShowMainMenu(this));
		}

		void StartGame()
		{
			WindowsHandler.ShowWindow(this, new GameWindow(questionCount, answerTimeout));
		}
		private void StartGame(object sender, RoutedEventArgs e)
		{
			if (isAdmin)
			{
				TriviaMessage.HandleMessage(TriviaMessage.MessageCodes.StartGame, () => StartGame());
			}
			else
			{
				MessageBox.Show("You are not the admin.\nPlease wait for the admin to start the game.", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
			}
		}

		void UpdateRoomData()
		{
			while (RunThread)
			{
				TriviaMessage.HandleMessage(TriviaMessage.MessageCodes.GetRoomState, msg =>
				{
					this.Dispatcher.Invoke(() =>
					{
						GetRoomStateResponse resp = GetRoomStateResponse.Parse(msg.Data);

						RoomName.Text = resp.name;
						questionCount = resp.questionCount;
						QuestionCount.Text = resp.questionCount.ToString();
						answerTimeout = resp.answerTimeout;
						AnswerTimeout.Text = resp.answerTimeout.ToString();

						PlayersList.Children.Clear();
						foreach (string name in resp.players)
						{
							PlayersList.Children.Add(new TextBlock()
							{
								Text = name,
								FontSize = 25
							});
						}

						if (!resp.isActive)
						{
							MessageBox.Show("The room admin closed this room.", "", MessageBoxButton.OK, MessageBoxImage.Information);
							WindowsHandler.ShowMainMenu(this);
						}
						if (resp.hasGameStarted)
						{
							MessageBox.Show("The admin started the game.", "", MessageBoxButton.OK, MessageBoxImage.Information);
							StartGame();
						}
					});
				}, false);

				Thread.Sleep(3000);
			}
		}
	}
}
