package com.ariel_apps.androidtriviaclient;


import androidx.appcompat.app.AppCompatActivity;
import android.content.Intent;
import android.os.Bundle;
import android.widget.Toast;


public class MainMenu extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main_menu);

        findViewById(R.id.logout).setOnClickListener(v -> logout());
        findViewById(R.id.CreateRoom).setOnClickListener(v -> {
            Intent i = new Intent(this, CreateRoom.class);
            i.addFlags(Intent.FLAG_ACTIVITY_NO_HISTORY);
            startActivity(i);
        });
        findViewById(R.id.JoinRoom).setOnClickListener(v -> {
            Intent i = new Intent(this, JoinRoom.class);
            i.addFlags(Intent.FLAG_ACTIVITY_NO_HISTORY);
            startActivity(i);
        });
        findViewById(R.id.Statistics).setOnClickListener(v -> {
            // TODO: add statistics screen
            Toast.makeText(this, "Statistics - unsupported", Toast.LENGTH_SHORT).show();
        });
    }

    private void logout() {
        Message logoutMsg = new Message("{}", Message.Codes.LOGOUT);
        TriviaSocket.send(logoutMsg);
        TriviaSocket.close();
        this.finish();
    }
}
