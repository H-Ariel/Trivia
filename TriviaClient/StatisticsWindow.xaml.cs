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
	/// Interaction logic for StatisticsWindow.xaml
	/// </summary>
	public partial class StatisticsWindow : Window
	{
		public StatisticsWindow()
		{
			InitializeComponent();

			TriviaMessage.HandleMessage(
				new TriviaMessage(TriviaMessage.MessageCodes.GetStatistics),
				resp => {

					GetStatisticsResponse statisticsResponse = GetStatisticsResponse.Parse(resp.Data);

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
							Text = $"Top {statisticsResponse.highScore.Count()}:",
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
