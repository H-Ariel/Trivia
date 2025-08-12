package com.ariel_apps.androidtriviaclient;


import androidx.appcompat.app.AppCompatActivity;
import android.annotation.SuppressLint;
import android.content.Intent;
import android.os.Bundle;
import android.view.Window;
import android.widget.TextView;
import android.widget.Toast;
import java.util.concurrent.locks.ReentrantLock;

// TODO: fix this activity - send answer after timer ended

public class Game extends AppCompatActivity {

    boolean shouldNewQuestion = true, runGetNewQuestionThread = true;
    int answerTimeout = 0, totalNumOfQuestions = 0, curruntQuestion = 0;
    Thread tGetNewQuestion = null;
    Thread tTimer = null;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        requestWindowFeature(Window.FEATURE_NO_TITLE);
        try {
            getSupportActionBar().hide();
        } catch (NullPointerException e) {
        }

        setContentView(R.layout.activity_game);

        Intent i = getIntent();
        answerTimeout = i.getIntExtra("answerTimeout", 0);
        totalNumOfQuestions = i.getIntExtra("totalNumOfQuestions", 0);

        findViewById(R.id.bAns0).setOnClickListener(v -> sendAnswer(0));
        findViewById(R.id.bAns1).setOnClickListener(v -> sendAnswer(1));
        findViewById(R.id.bAns2).setOnClickListener(v -> sendAnswer(2));
        findViewById(R.id.bAns3).setOnClickListener(v -> sendAnswer(3));
        findViewById(R.id.LeaveGame).setOnClickListener(v -> leaveGame());
    }

    @SuppressLint("SetTextI18n")
    @Override
    protected void onStart() {
        super.onStart();
        ((TextView) findViewById(R.id.TimeLeft)).setText(answerTimeout + " s");

        tGetNewQuestion = new Thread(new GetNewQuestion());
        tGetNewQuestion.start();
    }

    class GetNewQuestion implements Runnable {
        public void run() {
            while (runGetNewQuestionThread) {
                try {
                    if (shouldNewQuestion)
                        GetQuestion();
                    Thread.sleep(10);
                } catch (Exception e) {
                    showToast("error 2: " + e.getMessage());
                }
            }
        }

        @SuppressLint("SetTextI18n")
        void GetQuestion() {

            stopTimer();

            Message req = new Message("{}", Message.Codes.GET_QUESTION);

            TriviaSocket.send(req);
            Message serverAnswer = TriviaSocket.read();

            if (serverAnswer != null) {
                switch (serverAnswer.code) {
                    case OK:
                        Message.GetQuestionResponse resp = new Message.GetQuestionResponse(serverAnswer);
                        if (resp.status == Message.Codes.OK.getValue()) {
                            if (!resp.hasMoreQuestions) {
                                openGameResults();
                            } else {
                                runOnUiThread(() -> {

                                    ((TextView) findViewById(R.id.tvQuestion)).setText(resp.question);
                                    ((TextView) findViewById(R.id.bAns0)).setText(resp.answers[0]);
                                    ((TextView) findViewById(R.id.bAns1)).setText(resp.answers[1]);
                                    ((TextView) findViewById(R.id.bAns2)).setText(resp.answers[2]);
                                    ((TextView) findViewById(R.id.bAns3)).setText(resp.answers[3]);

                                    curruntQuestion += 1;
                                    ((TextView) findViewById(R.id.tvQuestionNumber)).setText(curruntQuestion + "/" + totalNumOfQuestions);
                                });
                            }
                        }
                        break;

                    case ERROR:
                        Message.ErrorResponse eResp = new Message.ErrorResponse(serverAnswer);
                        showToast(eResp.message);
                        break;
                }
            }

            startTimer();
            shouldNewQuestion = false;
        }

    }

    class Timer implements Runnable {
        @SuppressLint("SetTextI18n")
        public void run() {
            try {
                int questionAtStart = curruntQuestion;
                for (int i = answerTimeout; i > 0; i--) {
                    Thread.sleep(1000);

                    runOnUiThread(() -> {
                        TextView tvTimeLeft = (TextView) findViewById(R.id.TimeLeft);

                        String n = tvTimeLeft.getText().toString();
                        n = n.substring(0, n.length() - 2); // remove the " s"

                        tvTimeLeft.setText((Integer.parseInt(n) - 1) + " s");
                    });
                }
                int questionAtEnd = curruntQuestion;

                if (questionAtStart == questionAtEnd) {
                    sendAnswer(5); // Will always be wrong, because the answers are between 0 and 3.
                }

            } catch (Exception e) {
            }
        }
    }

    void startTimer() {
        runOnUiThread(() -> {
            TextView tvTimeLeft = (TextView) findViewById(R.id.TimeLeft);
            tvTimeLeft.setText(answerTimeout + " s");
        });

        tTimer = new Thread(new Timer());
        tTimer.start();
    }

    void stopTimer() {
        if (tTimer != null) {
            try {
                tTimer.interrupt();
                tTimer = null;
            } catch (Exception e) {
            }
        }
    }


    void sendAnswer(int ansId) {
        Message.SubmitAnswerRequest req = new Message.SubmitAnswerRequest(ansId);

        TriviaSocket.send(req);
        Message serverAnswer = TriviaSocket.read();

        if (serverAnswer != null) {
            switch (serverAnswer.code) {
                case OK:
                    Message.SubmitAnswerResponse resp = new Message.SubmitAnswerResponse(serverAnswer);
                    if (resp.status == Message.Codes.OK.getValue()) {
                        //boolean isAnswerCorrect = (resp.correctAnswerId == ansId);
                        // TODO: say user he correct (change button color?)
                    } else {
                        showToast("An error occurred. Please try again later.");
                    }
                    break;

                case ERROR:
                    Message.ErrorResponse eResp = new Message.ErrorResponse(serverAnswer);
                    showToast(eResp.message);
                    break;
            }
        }

        shouldNewQuestion = true;
    }

    void leaveGame() {
        Message req = new Message("{}", Message.Codes.LEAVE_GAME);

        TriviaSocket.send(req);
        Message serverAnswer = TriviaSocket.read();

        if (serverAnswer != null) {
            switch (serverAnswer.code) {
                case OK:
                    Message.LeaveGameResponse resp = new Message.LeaveGameResponse(serverAnswer);
                    if (resp.status == Message.Codes.OK.getValue()) {
                        openMainMenu();
                    } else {
                        showToast("You can't be leave the game.\nYou will suffer quietly.");
                    }
                    break;

                case ERROR:
                    Message.ErrorResponse eResp = new Message.ErrorResponse(serverAnswer);
                    showToast(eResp.message);
                    break;
            }
        }

    }

    void openMainMenu() {
        runOnUiThread(() -> {
            runGetNewQuestionThread = false;
            stopTimer();

            this.finish();
            Intent i = new Intent(this, MainMenu.class);
            i.addFlags(Intent.FLAG_ACTIVITY_NO_HISTORY);
            startActivity(i);
        });
    }
    void openGameResults() {
        runOnUiThread(() -> {
            runGetNewQuestionThread = false;
            stopTimer();

            this.finish();
            Intent i = new Intent(this, GameResults.class);
            i.addFlags(Intent.FLAG_ACTIVITY_NO_HISTORY);
            startActivity(i);
        });
    }

    void showToast(final String text) {
        runOnUiThread(() -> Toast.makeText(this, text, Toast.LENGTH_SHORT).show());
    }
}
