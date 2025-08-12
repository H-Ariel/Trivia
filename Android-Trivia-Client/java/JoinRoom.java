package com.ariel_apps.androidtriviaclient;


import androidx.appcompat.app.AppCompatActivity;
import android.content.Intent;
import android.os.Bundle;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.Button;
import android.widget.Toast;
import android.widget.AdapterView;
import android.view.View;

public class JoinRoom extends AppCompatActivity {

    boolean runThread = true;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_join_room);
    }

    @Override
    protected void onStart() {
        super.onStart();
        Thread tGetRooms = new Thread(new GetRooms());
        tGetRooms.start();
    }

    void joinRoom(int roomId) {
        boolean hasError = false;

        Message.JoinRoomRequest req = new Message.JoinRoomRequest(roomId);
        TriviaSocket.send(req);

        Message serverAnswer = TriviaSocket.read();

        if (serverAnswer != null) {
            switch (serverAnswer.code) {
                case OK:
                    Message.JoinRoomResponse resp = new Message.JoinRoomResponse(serverAnswer);
                    if (resp.status != Message.Codes.OK.getValue()) {
                        hasError = true;
                        showToast("An error has occurred.\nPlease try again later or choose another room.");
                    }
                    break;

                case ERROR:
                    Message.ErrorResponse eResp = new Message.ErrorResponse(serverAnswer);
                    showToast(eResp.message);
                    hasError = true;
                    break;
            }
        } else {
            showToast("Error: serverAnswer=null");
            hasError = true;
        }

        if (!hasError) {
            runThread = false;
            Intent i = new Intent(this, Room.class);
            i.putExtra("isAdmin", false);
            i.addFlags(Intent.FLAG_ACTIVITY_NO_HISTORY);
            startActivity(i);
        }
    }

    class GetRooms implements Runnable {
        public void run() {
            while (runThread) {

                Message req = new Message("{}", Message.Codes.GET_ROOMS);
                TriviaSocket.send(req);
                Message serverAnswer = TriviaSocket.read();


                if (serverAnswer != null) {
                    switch (serverAnswer.code) {
                        case OK:
                            Message.GetRoomsResponse resp = new Message.GetRoomsResponse(serverAnswer);
                            if (resp.status == Message.Codes.OK.getValue()) {
                                processGetRoomsResponse(resp);
                            } else {
                                //showToast("The manager closed the room.");
                                showToast("Unknown Error");
                            }
                            break;

                        case ERROR:
                            Message.ErrorResponse eResp = new Message.ErrorResponse(serverAnswer);
                            showToast(eResp.message);
                            break;
                    }
                } else {
                    showToast("Error: serverAnswer=null");
                }


                try {
                    Thread.sleep(3000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }

        void processGetRoomsResponse(Message.GetRoomsResponse resp) {
            runOnUiThread(() -> {
                try {
                    String[] roomsList = new String[resp.rooms.length];
                    for (int i = 0; i < resp.rooms.length; i++) {
                        roomsList[i] = resp.rooms[i].name;
                    }

                    ListView lvRoomsList = (ListView) findViewById(R.id.lvRoomsList);
                    ArrayAdapter aAdapter = new ArrayAdapter(JoinRoom.this, android.R.layout.simple_list_item_1, roomsList);
                    lvRoomsList.setAdapter(aAdapter);
                    lvRoomsList.setOnItemClickListener((a, v, i, l) -> joinRoom(resp.rooms[i].id));

                } catch (Exception e) {
                    showToast("error: " + e.getMessage());
                }
            });
        }
    }


    void showToast(final String text) {
        runOnUiThread(() -> Toast.makeText(this, text, Toast.LENGTH_SHORT).show());
    }

}
