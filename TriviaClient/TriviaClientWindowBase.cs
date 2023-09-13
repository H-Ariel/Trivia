using System.Windows;

namespace TriviaClient
{
	class WindowsHandler
	{
		public static void ShowWindow(Window src, Window newWnd)
		{
			newWnd.Top = src.Top;
			newWnd.Left = src.Left;
			newWnd.Height = src.Height;
			newWnd.Width = src.Width;
			newWnd.Show();
			src.Close();
		}

		public static void ShowMainMenu(Window src)
		{
			ShowWindow(src, new MainMenu());
		}
	}
}
