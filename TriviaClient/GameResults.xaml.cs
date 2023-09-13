using System.Collections.Generic;
using System.Threading;
using System.Windows;
using System.Windows.Controls;

namespace TriviaClient
{
	/// <summary>
	/// Interaction logic for GameResults.xaml
	/// </summary>
	public partial class GameResults : Window
	{
		bool RunThread;

		public GameResults()
		{
			InitializeComponent();

			this.Closing += (s, e) => RunThread = false;

			RunThread = true;
			new Thread(GetResults).Start();
		}

		void GetResults()
		{
			while (RunThread)
			{
				TriviaMessage.HandleMessage(TriviaMessage.MessageCodes.GetGameResults,
					msg =>
					{
						this.Dispatcher.Invoke(DisplayResults, GetGameResultsResponse.Parse(msg.Data).results);
						this.RunThread = false;
					},
					false);

				Thread.Sleep(3000);
			}
		}

		void DisplayResults(List<PlayerResults> results)
		{
			spGameResults.Children.Clear();

			{
				WrapPanel wrapPanel = new WrapPanel();
				wrapPanel.Children.Add(new TextBlock() { Text = "username", Width = 200, FontSize = 25, TextWrapping = TextWrapping.Wrap });
				wrapPanel.Children.Add(new Separator() { Width = 10, Opacity = 0 });
				wrapPanel.Children.Add(new TextBlock() { Text = "score", Width = 180, FontSize = 25, TextWrapping = TextWrapping.Wrap });
				spGameResults.Children.Add(wrapPanel);
			}

			foreach (PlayerResults res in results)
			{
				spGameResults.Children.Add(new Separator() { Height = 5 });
				WrapPanel wrapPanel = new WrapPanel();
				wrapPanel.Children.Add(new TextBlock() { Text = res.username, Width = 200, FontSize = 25, TextWrapping = TextWrapping.Wrap });
				wrapPanel.Children.Add(new Separator() { Width = 10, Opacity = 0 });
				wrapPanel.Children.Add(new TextBlock() { Text = res.score.ToString(), Width = 180, FontSize = 25, TextWrapping = TextWrapping.Wrap });
				spGameResults.Children.Add(wrapPanel);
			}
		}

		private void BackToMenu(object sender, RoutedEventArgs e)
		{
			TriviaMessage.HandleMessage(TriviaMessage.MessageCodes.LeaveGame, () => WindowsHandler.ShowMainMenu(this));
		}
	}
}
