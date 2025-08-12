package com.ariel_apps.androidtriviaclient;


import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.nio.charset.StandardCharsets;


public class TriviaSocket {
    static final Short SERVER_PORT = 8876;
    static final String SERVER_ADDRESS = "192.168.1.164";

    private static Socket socket = null;
    private static DataOutputStream dOut = null;
    private static DataInputStream dIn = null;

    /* Classes for Threads */
    /*
    Because a socket cannot be used inside the main
    thread, a thread must be opened each time it is used.
    */

    static class ConnectToServer implements Runnable {
        public void run() {
            if (socket != null) {
                close();
            }
            try {
                socket = new Socket(SERVER_ADDRESS, SERVER_PORT);
                dOut = new DataOutputStream(socket.getOutputStream());
                dIn = new DataInputStream(socket.getInputStream());
            } catch (Exception e) {
            }
        }
    }

    static class Close implements Runnable {
        public void run() {
            if (socket != null) {
                try {
                    socket.close();
                    socket = null;
                } catch (IOException e) {
                }
            }
            if (dOut != null) {
                try {
                    dOut.close();
                    dOut = null;
                } catch (IOException e) {
                }
            }
            if (dIn != null) {
                try {
                    dIn.close();
                    dIn = null;
                } catch (IOException e) {
                }
            }
        }
    }

    static class Read implements Runnable {
        Message msg = null;

        public Message getMessage() {
            return msg;
        }

        public void run() {
            try {
                byte code = dIn.readByte();
                int serverAnsLen = Integer.reverseBytes(dIn.readInt());
                byte[] serverAnsRaw = new byte[serverAnsLen];
                dIn.read(serverAnsRaw, 0, serverAnsLen);
                /*int bytesRead = dIn.read(serverAnsRaw, 0, serverAnsLen);
                if (bytesRead != -1 && bytesRead != serverAnsLen) throw new Exception("serverAnsLen != bytesRead");*/
                String serverAns = new String(serverAnsRaw);
                msg = new Message(serverAns, code);
            } catch (IOException e) {
                msg = null;
            }
        }
    }

    static class Send implements Runnable {
        Message msg = null;

        public Send(Message msg) {
            this.msg = msg;
        }

        public void run() {
            try {
                dOut.write(msg.code.getValue());
                dOut.writeInt(Integer.reverseBytes(msg.data.length()));
                dOut.write(msg.data.getBytes(StandardCharsets.US_ASCII));
                dOut.flush();
            } catch (Exception e) {
            }
        }
    }


    /* TriviaSocket's methods */

    public static void connectToServer() {
        try {
            Thread tConnectToServer = new Thread(new ConnectToServer());
            tConnectToServer.start();
            tConnectToServer.join();
        } catch (Exception e) {
        }
    }

    public static void close() {
        try {
            Thread tConnectToServer = new Thread(new Close());
            tConnectToServer.start();
            tConnectToServer.join();
        } catch (Exception e) {
        }
    }

    public static Message read() {
        Read readClass = new Read();
        try {
            Thread tRead = new Thread(readClass);
            tRead.start();
            tRead.join();
        } catch (InterruptedException e) {
        }
        return readClass.getMessage();
    }

    public static void send(Message msg) {
        try {
            Thread tSend = new Thread(new Send(msg));
            tSend.start();
            tSend.join();
        } catch (InterruptedException e) {
        }
    }

    public static void send(Message.LoginRequest req) {
        Message.Codes msgCode = Message.Codes.LOGIN;
        String data = req.toJSON();
        send(new Message(data, msgCode));
    }

    public static void send(Message.SignupRequest req) {
        Message.Codes msgCode = Message.Codes.SIGNUP;
        String data = req.toJSON();
        send(new Message(data, msgCode));
    }

    public static void send(Message.CreateRoomRequest req) {
        Message.Codes msgCode = Message.Codes.CREATE_ROOM;
        String data = req.toJSON();
        send(new Message(data, msgCode));
    }

    public static void send(Message.JoinRoomRequest req) {
        Message.Codes msgCode = Message.Codes.JOIN_ROOM;
        String data = req.toJSON();
        send(new Message(data, msgCode));
    }

    public static void send(Message.SubmitAnswerRequest req) {
        Message.Codes msgCode = Message.Codes.SUBMIT_ANSWER;
        String data = req.toJSON();
        send(new Message(data, msgCode));
    }
}
