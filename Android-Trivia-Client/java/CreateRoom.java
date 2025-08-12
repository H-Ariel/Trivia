package com.ariel_apps.androidtriviaclient;


import androidx.appcompat.app.AppCompatActivity;
import android.content.Intent;
import android.os.Bundle;
import android.widget.EditText;
import android.widget.Toast;


public class CreateRoom extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_create_room);

        findViewById(R.id.cancel).setOnClickListener(v -> backToMenu());
        findViewById(R.id.create).setOnClickListener(v -> createRoom());
    }

    void backToMenu()
    {
        finish();
        Intent i = new Intent(this, MainMenu.class);
        i.addFlags(Intent.FLAG_ACTIVITY_NO_HISTORY);
        startActivity(i);
    }

    void createRoom() {
        boolean hasError = false;

        String roomName = ((EditText) findViewById(R.id.RoomName)).getText().toString();
        String numOfPlayersStr = ((EditText) findViewById(R.id.NumOfPlayers)).getText().toString();
        String questionTimeStr = ((EditText) findViewById(R.id.QuestionTime)).getText().toString();
        String numOfQuestionsStr = ((EditText) findViewById(R.id.NumOfQuestions)).getText().toString();

        if (roomName.equals("") || numOfPlayersStr.equals("") || questionTimeStr.equals("") || numOfQuestionsStr.equals("")) {
            Toast.makeText(this, "Must fill all the fields!", Toast.LENGTH_SHORT).show();
            return;
        }

        int numOfPlayers = Integer.parseInt(numOfPlayersStr);
        int questionTime = Integer.parseInt(questionTimeStr);
        int numOfQuestions = Integer.parseInt(numOfQuestionsStr);

        /*if (numOfPlayers < 0 || questionTime < 0 || numOfQuestions < 0) {
            Toast.makeText(this, "All fields should be positive!", Toast.LENGTH_SHORT).show();
            return;
        }*/

        Message.CreateRoomRequest req = new Message.CreateRoomRequest(roomName, numOfPlayers, questionTime, numOfQuestions);

        TriviaSocket.send(req);
        Message serverAnswer = TriviaSocket.read();

        if (serverAnswer != null) {
            switch (serverAnswer.code) {
                case OK:
                    Message.CreateRoomResponse resp = new Message.CreateRoomResponse(serverAnswer);
                    if (resp.status != Message.Codes.OK.getValue()) {
                        Toast.makeText(CreateRoom.this, "Unknown Error", Toast.LENGTH_SHORT).show();
                        hasError = true;
                    }
                    break;

                case ERROR:
                    Message.ErrorResponse eResp = new Message.ErrorResponse(serverAnswer);
                    Toast.makeText(CreateRoom.this, eResp.message, Toast.LENGTH_SHORT).show();
                    hasError = true;
                    break;
            }

        } else
            Toast.makeText(this, "Error: serverAnswer=null", Toast.LENGTH_SHORT).show();

        if (!hasError) {
            Intent i = new Intent(this, Room.class);
            i.putExtra("isAdmin", true);
            i.addFlags(Intent.FLAG_ACTIVITY_NO_HISTORY);
            startActivity(i);
        }
    }
}
