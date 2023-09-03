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
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace TriviaClient
{
	/// <summary>
	/// Interaction logic for GameWindow.xaml
	/// </summary>
	public partial class GameWindow : Window
	{
		readonly int questionCount, answerTimeout;
		int currQuestionId;
		bool newWndOpened;
		Thread? timer;


		public GameWindow(int questionCount, int answerTimeout)
		{
			InitializeComponent();

			this.questionCount = questionCount;
			this.answerTimeout = answerTimeout;

			currQuestionId = 0;
			newWndOpened = false;

			GetNewQuestions();
			InitTimer();
		}


		void InitTimer()
		{
			StopTimer();
			TimeLeft.Text = answerTimeout.ToString();
			timer = new Thread(TimerThread);
			timer.Start();
		}
		void StopTimer()
		{
			if (timer != null)
			{
				timer.Interrupt();
				timer = null;
			}
		}
		void TimerThread()
		{
			try
			{
				int questionAtStart = currQuestionId;
				for (int i = answerTimeout; i > 0; i--)
				{
					Thread.Sleep(1000);
					this.Dispatcher.Invoke(() => TimeLeft.Text = (int.Parse(TimeLeft.Text) - 1).ToString());
				}
				int questionAtEnd = currQuestionId;

				if (questionAtStart == questionAtEnd)
				{
					this.Dispatcher.Invoke(() => SendAnswer(5)); // Will always be wrong, because the answers are between 0 and 3.
				}
			}
			catch (Exception)
			{
			}
		}

		void GetNewQuestions()
		{
			TriviaMessage.HandleMessage(new TriviaMessage(TriviaMessage.MessageCodes.GetQuestion), msg =>
			{
				this.Dispatcher.Invoke(() =>
				{
					GetQuestionResponse resp = GetQuestionResponse.Parse(msg.Data);

					Question.Text = resp.question;
					QuestionsCounter.Text = $"Question: {currQuestionId}/{questionCount}";

					AnswersList.Children.Clear();
					int ansId = 0;
					foreach (string ans in resp.answers)
					{
						Button btn = new()
						{
							FontSize = 25,
							Width = 500,
							HorizontalAlignment = HorizontalAlignment.Center,
							Content = ans,
							Tag = ansId
						};
						btn.Click += SelectAnswer;

						AnswersList.Children.Add(btn);
						AnswersList.Children.Add(new Separator() { Opacity = 0, Height = 5 });

						ansId += 1;
					}

					currQuestionId += 1;
					InitTimer();
				});
			});
		}

		void SelectAnswer(object sender, RoutedEventArgs? e)
		{
			if (sender is Button button)
				SendAnswer((int)button.Tag);
		}
		void SendAnswer(int ansId)
		{
			TriviaMessage.HandleMessage(new TriviaMessage(new SubmitAnswerRequest(ansId)), () =>
			{
				if (currQuestionId < questionCount)
				{
					GetNewQuestions();
				}
				else
				{
					if (!newWndOpened)
					{
						WindowsHandler.ShowWindow(this, new GameResults());
						newWndOpened = true;
					}
				}
			});
		}

		void LeaveGame(object sender, RoutedEventArgs e)
		{
			TriviaMessage.HandleMessage(new TriviaMessage(TriviaMessage.MessageCodes.LeaveGame), () => WindowsHandler.ShowMainMenu(this));
		}
	}
}
