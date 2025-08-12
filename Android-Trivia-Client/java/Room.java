package com.ariel_apps.androidtriviaclient;


import androidx.appcompat.app.AppCompatActivity;
import android.annotation.SuppressLint;
import android.content.Intent;
import android.os.Bundle;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;
import java.util.concurrent.locks.ReentrantLock;


public class Room extends AppCompatActivity {

    boolean isAdmin = false, runThreads = true;
    int answerTimeout = 0, totalNumOfQuestions = 0;
    ReentrantLock socketMutex = new ReentrantLock();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_room);

        findViewById(R.id.StartGame).setOnClickListener(v -> startGame());
        findViewById(R.id.LeaveRoom).setOnClickListener(v -> leaveRoom());

        isAdmin = getIntent().getBooleanExtra("isAdmin", false);

        if (isAdmin) {
            Button btn = (Button) findViewById(R.id.LeaveRoom);
            btn.setText("Close Room");
        } else {
            Button btn = (Button) findViewById(R.id.StartGame);
            btn.setVisibility(View.INVISIBLE);
        }
    }

    @Override
    protected void onStart() {
        super.onStart();
        Thread tGetRoomState = new Thread(new GetRoomState());
        tGetRoomState.start();

        if (!isAdmin) {
            Thread tDoesGameStarted = new Thread(new DoesGameStarted());
            tDoesGameStarted.start();
        }
    }


    void startGame() {
        if (!isAdmin) return;

        Message req = new Message("{}", Message.Codes.START_GAME);

        socketMutex.lock();
        TriviaSocket.send(req);
        Message serverAnswer = TriviaSocket.read();
        socketMutex.unlock();

        if (serverAnswer != null) {
            switch (serverAnswer.code) {
                case OK:
                    Message.StartGameResponse resp = new Message.StartGameResponse(serverAnswer);
                    if (resp.status == Message.Codes.OK.getValue()) {
                        openGame();
                    } else {
                        showToast("An error has occurred. Please try again later.\nOr you will not play today.");
                    }
                    break;

                case ERROR:
                    Message.ErrorResponse eResp = new Message.ErrorResponse(serverAnswer);
                    showToast(eResp.message);
                    break;
            }
        }
    }

    void leaveRoom() {
        Message req = new Message("{}", Message.Codes.LEAVE_ROOM);
        if (isAdmin) {
            req.code = Message.Codes.CLOSE_ROOM;
        }

        socketMutex.lock();
        TriviaSocket.send(req);
        Message serverAnswer = TriviaSocket.read();
        socketMutex.unlock();

        if (serverAnswer != null) {
            switch (serverAnswer.code) {
                case OK:
                    Message.LeaveRoomResponse resp = new Message.LeaveRoomResponse(serverAnswer);
                    if (resp.status == Message.Codes.OK.getValue()) {
                        openMainMenu();
                    } else {
                        showToast("You can not be disconnected from the room.\nYou will suffer quietly.");
                    }
                    break;

                case ERROR:
                    Message.ErrorResponse eResp = new Message.ErrorResponse(serverAnswer);
                    showToast(eResp.message);
                    break;
            }
        }
    }


    class GetRoomState implements Runnable {
        public void run() {
            while (runThreads) {
                Message req = new Message("{}", Message.Codes.GET_ROOM_STATE);

                socketMutex.lock();
                TriviaSocket.send(req);
                Message serverAnswer = TriviaSocket.read();
                socketMutex.unlock();

                if (serverAnswer != null) {
                    switch (serverAnswer.code) {
                        case OK:
                            Message.GetRoomStateResponse resp = new Message.GetRoomStateResponse(serverAnswer);
                            if (resp.status == Message.Codes.OK.getValue()) {
                                processGetRoomStateResponse(resp);
                            } else {
                                //showToast("The manager closed the room.");
                                showToast("Unknown Error");
                                openMainMenu();
                            }
                            break;

                        case ERROR:
                            Message.ErrorResponse eResp = new Message.ErrorResponse(serverAnswer);
                            showToast(eResp.message);
                            break;
                    }
                }

                try {
                    Thread.sleep(3000);
                } catch (InterruptedException e) {
                }
            }
        }

        @SuppressLint("SetTextI18n")
        void processGetRoomStateResponse(Message.GetRoomStateResponse resp) {
            runOnUiThread(() -> {
                try {
                    if (!resp.doesRoomExist) {
                        openMainMenu();
                    }

                    answerTimeout = resp.answerTimeout;
                    totalNumOfQuestions = resp.questionCount;

                    ((TextView) findViewById(R.id.tvRoomName)).setText(resp.roomName);
                    ((TextView) findViewById(R.id.tvNumOfPlayers)).setText(Integer.toString(resp.maxPlayers));
                    ((TextView) findViewById(R.id.tvQuestionTime)).setText(Integer.toString(resp.answerTimeout));
                    ((TextView) findViewById(R.id.tvNumOfQuestions)).setText(Integer.toString(resp.questionCount));

                    ArrayAdapter aAdapter = new ArrayAdapter(Room.this, android.R.layout.simple_list_item_1, resp.players);
                    ((ListView) findViewById(R.id.lvPlayersList)).setAdapter(aAdapter);

                } catch (Exception e) {
                    showToast("error: " + e.getMessage());
                }
            });
        }
    }

    /* Checking if the game has started */
    class DoesGameStarted implements Runnable {
        public void run() {
            while (runThreads) {

                Message req = new Message("{}", Message.Codes.START_GAME);
                socketMutex.lock();
                TriviaSocket.send(req);
                Message serverAnswer = TriviaSocket.read();
                socketMutex.unlock();

                if (serverAnswer != null && serverAnswer.code == Message.Codes.OK) {
                    Message.StartGameResponse resp = new Message.StartGameResponse(serverAnswer);
                    if (resp.status == Message.Codes.OK.getValue()) {
                        openGame();
                    }
                }

                try {
                    Thread.sleep(3000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }
    }


    void showToast(final String text) {
        runOnUiThread(() -> Toast.makeText(this, text, Toast.LENGTH_SHORT).show());
    }

    void openMainMenu() {
        runOnUiThread(() -> {
            runThreads = false;
            this.finish();
            Intent i = new Intent(this, MainMenu.class);
            i.addFlags(Intent.FLAG_ACTIVITY_NO_HISTORY);
            startActivity(i);
        });
    }

    void openGame() {
        runOnUiThread(() -> {
            runThreads = false;
            this.finish();
            Intent i = new Intent(this, Game.class);
            i.addFlags(Intent.FLAG_ACTIVITY_NO_HISTORY);

            i.putExtra("answerTimeout", answerTimeout);
            i.putExtra("totalNumOfQuestions", totalNumOfQuestions);

            startActivity(i);
        });
    }
}
