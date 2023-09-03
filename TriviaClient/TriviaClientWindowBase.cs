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
