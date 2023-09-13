using System.Threading;
using System.Windows;
using System.Windows.Controls;

namespace TriviaClient
{
	/// <summary>
	/// Interaction logic for JoinRoom.xaml
	/// </summary>
	public partial class JoinRoom : Window
	{
		bool RunThread;

		public JoinRoom()
		{
			InitializeComponent();

			this.Closing += (s, e) => RunThread = false;

			RunThread = true;
			new Thread(UpdateRoomsList).Start();
		}

		void JoinTheRoom(object sender, RoutedEventArgs e)
		{
			TriviaMessage.HandleMessage(new TriviaMessage(new JoinRoomRequest((int)((Button)sender).Tag)),
				() => WindowsHandler.ShowWindow(this, new Room()));
		}


		void UpdateRoomsList()
		{
			while (RunThread)
			{
				TriviaMessage.HandleMessage(TriviaMessage.MessageCodes.GetRooms, msg =>
				{
					this.Dispatcher.Invoke(() =>
					{
						RoomsList.Children.Clear();
						foreach (RoomData r in GetRoomsResponse.Parse(msg.Data).rooms)
						{
							Button btn = new()
							{
								Content = r.name,
								Width = 320,
								FontSize = 28,
								Tag = r.id
							};
							btn.Click += JoinTheRoom;

							RoomsList.Children.Add(btn);
						}
					});
				}, false);

				Thread.Sleep(3000);
			}
		}

		private void BackToMenu(object sender, RoutedEventArgs e)
		{
			WindowsHandler.ShowMainMenu(this);
		}
	}
}
