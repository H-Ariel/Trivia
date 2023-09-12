using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

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
