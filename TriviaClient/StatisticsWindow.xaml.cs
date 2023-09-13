using System.Windows;
using System.Windows.Controls;

namespace TriviaClient
{
	/// <summary>
	/// Interaction logic for StatisticsWindow.xaml
	/// </summary>
	public partial class StatisticsWindow : Window
	{
		public StatisticsWindow()
		{
			InitializeComponent();

			TriviaMessage.HandleMessage(TriviaMessage.MessageCodes.GetStatistics, msg =>
			{
				GetStatisticsResponse statisticsResponse = GetStatisticsResponse.Parse(msg.Data);

				statistics.Children.Add(new TextBlock()
				{
					Text = $"your score: {statisticsResponse.userStat.username} - {statisticsResponse.userStat.score}",
					Width = 320,
					FontSize = 28
				});

				statistics.Children.Add(new Separator()
				{
					Opacity = 0,
					Height = 10,
				});

				if (statisticsResponse.highScore != null)
				{
					statistics.Children.Add(new TextBlock()
					{
						Text = $"Top {statisticsResponse.highScore.Count}:",
						Width = 320,
						FontSize = 32
					});

					foreach (UserStatistics uStat in statisticsResponse.highScore)
					{
						statistics.Children.Add(new TextBlock()
						{
							Text = $"your score: {uStat.username} - {uStat.score}",
							Width = 320,
							FontSize = 28
						});
					}
				}
			}
			);
		}

		private void BackToMenu(object sender, RoutedEventArgs e)
		{
			WindowsHandler.ShowMainMenu(this);
		}
	}
}
