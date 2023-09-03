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
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace TriviaClient
{
	/// <summary>
	/// Interaction logic for MainWindow.xaml
	/// </summary>
	public partial class LoginWindow : Window
	{
		public LoginWindow()
		{
			InitializeComponent();
		}


		private void Login(object sender, RoutedEventArgs e)
		{
			try
			{
				TriviaSocket.Connect();
			}
			catch
			{
				MessageBox.Show("Unable to connect to server", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
				return;
			}

			TriviaMessage? msg = null;
			string str = (string)(sender as Button).Content;

			if (str.Equals("Login"))
				msg = new TriviaMessage(new LoginRequest(username.Text, password.Password));
			else if (str.Equals("Signup"))
				msg = new TriviaMessage(new SignupRequest(username.Text, password.Password, email.Text));

			if (msg != null)
				TriviaMessage.HandleMessage(msg, () => WindowsHandler.ShowMainMenu(this));
		}
	}
}
