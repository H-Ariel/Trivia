using System.Text.RegularExpressions;
using System.Windows;
using System.Windows.Input;

namespace TriviaClient
{
	/// <summary>
	/// Interaction logic for CreateRoom.xaml
	/// </summary>
	public partial class CreateRoom : Window
	{
		public CreateRoom()
		{
			InitializeComponent();
		}

		private void NumberValidationTextBox(object sender, TextCompositionEventArgs e)
		{
			e.Handled = new Regex("[^0-9]+").IsMatch(e.Text);
		}

		private void Create(object sender, RoutedEventArgs e)
		{
			if (
				string.IsNullOrEmpty(name.Text) ||
				string.IsNullOrEmpty(maxUsers.Text) ||
				string.IsNullOrEmpty(questionCount.Text) ||
				string.IsNullOrEmpty(answerTimeout.Text)
				)
			{
				MessageBox.Show("invalid input", "Error", MessageBoxButton.OK, MessageBoxImage.Error);
				return;
			}

			TriviaMessage.HandleMessage(
				new TriviaMessage(new CreateRoomRequest(name.Text, int.Parse(maxUsers.Text),
				int.Parse(questionCount.Text), int.Parse(answerTimeout.Text))),
				() => WindowsHandler.ShowWindow(this, new Room(true)));
		}

		void Cancel(object sender, RoutedEventArgs e)
		{
			WindowsHandler.ShowMainMenu(this);
		}
	}
}
