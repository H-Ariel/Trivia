using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net.Sockets;
using System.Windows.Controls;
using System.Security.Cryptography;
using System.IO;

namespace TriviaClient
{
	internal class TriviaSocket
	{
		static readonly string SERVER_IP = "127.0.0.1";
		static readonly int SERVER_PORT = 9568;


		private static TcpClient? socket;
		private static NetworkStream? stream;


		public static void Connect()
		{
			socket = new TcpClient();
			socket.Connect(SERVER_IP, SERVER_PORT);
			stream = socket.GetStream();
		}
		public static void Close()
		{
			if (stream != null)
			{
				stream.Close();
				stream = null;
			}
			if (socket != null)
			{
				socket.Close();
				socket = null;
			}
		}


		static byte[] IntToBytes(int i)
		{
			return BitConverter.GetBytes(i);
		}
		static int BytesToInt(byte[] b, int startIndex = 0)
		{
			return BitConverter.ToInt32(b, startIndex);
		}

		static public TriviaMessage SendAndRead(TriviaMessage msg)
		{
			Send(msg);
			return Read();
		}

		static void Send(TriviaMessage msg)
		{
			if (stream != null)
			{
				MemoryStream mem = new MemoryStream();
				mem.WriteByte(msg.Code);
				mem.Write(IntToBytes(msg.Data.Length));
				mem.Write(Encoding.ASCII.GetBytes(msg.Data));

				stream.Write(mem.ToArray());
				stream.Flush();
			}
		}

		static TriviaMessage Read()
		{
			TriviaMessage msg = new();

			if (stream != null)
			{
				msg.Code = (byte)stream.ReadByte();

				byte[] bytes = new byte[4];
				stream.Read(bytes);
				int len = BytesToInt(bytes);

				if (len > 0)
				{
					bytes = new byte[len];
					stream.Read(bytes);
					msg.Data = Encoding.ASCII.GetString(bytes);
				}
			}

			return msg;
		}
	}
}
