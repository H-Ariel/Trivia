package com.ariel_apps.androidtriviaclient;


import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.EditText;
import android.content.Intent;
import android.widget.Toast;


public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        findViewById(R.id.exit).setOnClickListener(v ->{
            finish();
        });
        findViewById(R.id.login).setOnClickListener(v -> loginToServer(true));
        findViewById(R.id.signup).setOnClickListener(v -> loginToServer(false));
    }

    @Override
    public void finish() {
        TriviaSocket.close();
        super.finish();
    }

    void loginToServer(boolean isLogin) { // isLogin: true - user clicked login, false - user clicked signup
        boolean hasError = false;

        try {
            String username = ((EditText) findViewById(R.id.username)).getText().toString();
            String password = ((EditText) findViewById(R.id.password)).getText().toString();
            String email = ((EditText) findViewById(R.id.email)).getText().toString();

            TriviaSocket.connectToServer();

            if (isLogin) {
                Message.LoginRequest msg = new Message.LoginRequest(username, password);
                TriviaSocket.send(msg);
            } else {
                Message.SignupRequest msg = new Message.SignupRequest(username, password, email);
                TriviaSocket.send(msg);
            }

            Message serverAnswer = TriviaSocket.read();

            switch (serverAnswer.code) {
                case OK:
                    Message.LoginSignupResponse resp = new Message.LoginSignupResponse(serverAnswer);
                    if (resp.status != Message.Codes.OK.getValue()) {
                        showToast("Unknown Error");
                        hasError = true;
                    }
                    break;

                case ERROR:
                    Message.ErrorResponse eResp = new Message.ErrorResponse(serverAnswer);
                    showToast(eResp.message);
                    hasError = true;
                    break;
            }

        } catch (Exception e) {
            showToast("App Exception: " + e.getMessage());
            hasError = true;
        }

        if (!hasError) {
            Intent i = new Intent(this, MainMenu.class);
            //i.addFlags(Intent.FLAG_ACTIVITY_NO_HISTORY);
            startActivity(i);
        }
    }

    void showToast(final String text) {
        runOnUiThread(() -> Toast.makeText(this, text, Toast.LENGTH_SHORT).show());
    }
}
