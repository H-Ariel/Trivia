package com.ariel_apps.androidtriviaclient;


import androidx.appcompat.app.AppCompatActivity;
import android.content.Intent;
import android.os.Bundle;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.Toast;

public class GameResults extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_game_results);

        findViewById(R.id.grBackToMenu).setOnClickListener(v -> backToMenu());

        Thread tDrawWindow = new Thread(new DrawWindow());
        tDrawWindow.start();
    }

    class DrawWindow implements Runnable {
        public void run() {
            WaitGameEnded();

            Message.PlayerResults[] gameResults = GetGameResults();
            if (gameResults == null) return;

            runOnUiThread(() ->
            {
                String[] resultsList = new String[gameResults.length];

                for (int i = 0; i < gameResults.length; i++) {
                    resultsList[i] = gameResults[i].username +
                            "\nscore: " + gameResults[i].correctAnswerCount +
                            "\naverage answer time: " + gameResults[i].averageAnswerTime + " s";
                }

                ListView lvRoomsList = (ListView) findViewById(R.id.lvResultsList);
                ArrayAdapter aAdapter = new ArrayAdapter(GameResults.this, android.R.layout.simple_list_item_1, resultsList);
                lvRoomsList.setAdapter(aAdapter);
            });
        }

        Message.PlayerResults[] GetGameResults() {
            Message req = new Message("{}", Message.Codes.GET_GAME_RESULTS);
            TriviaSocket.send(req);
            Message serverAnswer = TriviaSocket.read();

            if (serverAnswer != null) {
                switch (serverAnswer.code) {
                    case OK:
                        Message.GetGameResultsResponse resp = new Message.GetGameResultsResponse(serverAnswer);
                        if (resp.status == Message.Codes.OK.getValue()) {
                            return resp.results;
                        }
                        break;

                    case ERROR:
                        Message.ErrorResponse eResp = new Message.ErrorResponse(serverAnswer);
                        showToast(eResp.message);
                        break;
                }
            }

            return null;
        }

        void WaitGameEnded() {
            runOnUiThread(() ->
            {
                String[] resultsList = new String[1];

                resultsList[0] = "The game is not over yet";

                ListView lvResultsList = (ListView) findViewById(R.id.lvResultsList);
                ArrayAdapter aAdapter = new ArrayAdapter(GameResults.this, android.R.layout.simple_list_item_1, resultsList);
                lvResultsList.setAdapter(aAdapter);
            });

            while (!DoesGameEnded()) {
                try {
                    Thread.sleep(3000);
                } catch (InterruptedException e) {
                }
            }
        }

        boolean DoesGameEnded() {
            Message req = new Message("{}", Message.Codes.GET_GAME_STATE);
            TriviaSocket.send(req);
            Message serverAnswer = TriviaSocket.read();

            if (serverAnswer != null && serverAnswer.code == Message.Codes.OK) {
                Message.GetGameStateResponse resp = new Message.GetGameStateResponse(serverAnswer);
                if (resp.status == Message.Codes.OK.getValue()) {
                    return resp.doesGameEnded;
                }
            }
            return false;
        }
    }


    void backToMenu() {
        this.finish();
        Intent i = new Intent(this, MainMenu.class);
        i.addFlags(Intent.FLAG_ACTIVITY_NO_HISTORY);
        startActivity(i);
    }

    void showToast(final String text) {
        runOnUiThread(() -> Toast.makeText(this, text, Toast.LENGTH_SHORT).show());
    }
}
