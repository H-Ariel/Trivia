using System.Windows;

namespace TriviaClient
{
	/// <summary>
	/// Interaction logic for MainMenu.xaml
	/// </summary>
	public partial class MainMenu : Window
	{
		public MainMenu()
		{
			InitializeComponent();
		}

		private void Logout(object sender, RoutedEventArgs e)
		{
			TriviaSocket.SendAndRead(new TriviaMessage(TriviaMessage.MessageCodes.Logout));
			TriviaSocket.Close();
			this.Close();
		}

		private void CreateRoom(object sender, RoutedEventArgs e)
		{
			WindowsHandler.ShowWindow(this, new CreateRoom());
		}

		private void JoinRoom(object sender, RoutedEventArgs e)
		{
			WindowsHandler.ShowWindow(this, new JoinRoom());
		}

		private void Statistics(object sender, RoutedEventArgs e)
		{
			WindowsHandler.ShowWindow(this, new StatisticsWindow());
		}
	}
}
