using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Interop;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

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
			int id = (int)((Button)sender).Tag;

			TriviaMessage.HandleMessage(new TriviaMessage(new JoinRoomRequest(id)), () =>
			{
				WindowsHandler.ShowWindow(this, new Room());
			});
		}


		void UpdateRoomsList()
		{
			while (RunThread)
			{
				TriviaMessage.HandleMessage(new TriviaMessage(TriviaMessage.MessageCodes.GetRooms), msg =>
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
