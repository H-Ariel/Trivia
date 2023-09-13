using System.Windows;
using System.Windows.Controls;

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

			if (sender is Button btn)
			{
				TriviaMessage? msg = null;
				string str = (string)btn.Content;

				if (str.Equals("Login"))
					msg = new TriviaMessage(new LoginRequest(username.Text, password.Password));
				else if (str.Equals("Signup"))
					msg = new TriviaMessage(new SignupRequest(username.Text, password.Password, email.Text));

				if (msg != null)
					TriviaMessage.HandleMessage(msg, () => WindowsHandler.ShowMainMenu(this));
			}
		}
	}
}
