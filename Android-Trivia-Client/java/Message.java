package com.ariel_apps.androidtriviaclient;


import java.util.Map;
import java.util.HashMap;
import org.json.JSONObject;
import org.json.JSONArray;
import org.json.JSONException;


public class Message {
    public enum Codes {
        ERROR((byte) 0),
        OK((byte) 1),

        LOGIN((byte) 5),
        SIGNUP((byte) 6),
        LOGOUT((byte) 7),

        GET_ROOMS((byte) 8),
        CREATE_ROOM((byte) 9),
        JOIN_ROOM((byte) 10),
        GET_PLAYERS_IN_ROOM((byte) 11),
        GET_PERSONAL_STATS((byte) 12),
        GET_HIGH_SCORE((byte) 13),

        CLOSE_ROOM((byte) 14),
        START_GAME((byte) 15),
        GET_ROOM_STATE((byte) 16),
        LEAVE_ROOM((byte) 17),

        GET_QUESTION((byte) 18),
        SUBMIT_ANSWER((byte) 19),
        GET_GAME_RESULTS((byte) 20),
        GET_GAME_STATE((byte) 21),
        LEAVE_GAME((byte) 22);


        private final byte value;
        private static final Map map = new HashMap<>();

        Codes(byte value) {
            this.value = value;
        }

        static {
            for (Codes code : Codes.values()) {
                map.put(code.value, code);
            }
        }

        public static Codes valueOf(byte code) {
            return (Codes) map.get(code);
        }

        public byte getValue() {
            return value;
        }
    }


    interface Request {
        public String toJSON();
    }

    public static class LoginRequest implements Request {
        public String username;
        public String password;

        public LoginRequest(String username, String password) {
            this.username = username;
            this.password = password;
        }

        public String toJSON() {
            try {
                JSONObject obj = new JSONObject();
                obj.accumulate("username", username);
                obj.accumulate("password", password);
                return obj.toString();
            } catch (JSONException e) {
                return "";
            }
        }
    }

    public static class SignupRequest implements Request {
        public String username;
        public String password;
        public String email;

        public SignupRequest(String username, String password, String email) {
            this.username = username;
            this.password = password;
            this.email = email;
        }

        public String toJSON() {
            try {
                JSONObject obj = new JSONObject();
                obj.accumulate("username", username);
                obj.accumulate("password", password);
                obj.accumulate("email", email);
                return obj.toString();
            } catch (JSONException e) {
                return "";
            }
        }
    }

    public static class CreateRoomRequest implements Request {
        public String roomName;
        public int maxUsers;
        public int questionCount;
        public int answerTimeout;

        public CreateRoomRequest(String roomName, int maxUsers, int questionCount, int answerTimeout) {
            this.roomName = roomName;
            this.maxUsers = maxUsers;
            this.questionCount = questionCount;
            this.answerTimeout = answerTimeout;
        }

        public String toJSON() {
            try {
                JSONObject obj = new JSONObject();
                obj.accumulate("roomName", roomName);
                obj.accumulate("maxUsers", maxUsers);
                obj.accumulate("questionCount", questionCount);
                obj.accumulate("answerTimeout", answerTimeout);
                return obj.toString();
            } catch (JSONException e) {
                return "";
            }
        }
    }

    public static class JoinRoomRequest implements Request {
        public int roomId;

        public JoinRoomRequest(int roomId) {
            this.roomId = roomId;
        }

        public String toJSON() {
            try {
                JSONObject obj = new JSONObject();
                obj.accumulate("roomId", roomId);
                return obj.toString();
            } catch (JSONException e) {
                return "";
            }
        }
    }

    public static class SubmitAnswerRequest implements Request {
        public int answerId;

        public SubmitAnswerRequest(int answerId) {
            this.answerId = answerId;
        }

        public String toJSON() {
            try {
                JSONObject obj = new JSONObject();
                obj.accumulate("answerId", answerId);
                return obj.toString();
            } catch (JSONException e) {
                return "";
            }
        }
    }


    public static class Response {
        public int status;

        public Response(Message msg) {
            try {
                JSONObject obj = new JSONObject(msg.data);
                status = obj.getInt("status");
            } catch (JSONException e) {
            }
        }
    }

    public static class ErrorResponse {
        public String message;

        public ErrorResponse(Message msg) {
            try {
                JSONObject obj = new JSONObject(msg.data);
                message = obj.getString("message");
            } catch (JSONException e) {
            }
        }
    }

    public static class LoginSignupResponse extends Response {
        public LoginSignupResponse(Message msg) {
            super(msg);
        }
    }

    public static class CreateRoomResponse extends Response {
        public CreateRoomResponse(Message msg) {
            super(msg);
        }
    }

    public static class GetRoomStateResponse extends Response {
        public boolean doesRoomExist;
        public String[] players;
        public int questionCount;
        public int answerTimeout;
        public int maxPlayers;
        public String roomName;

        public GetRoomStateResponse(Message msg) {
            super(msg);
            try {
                JSONObject obj = new JSONObject(msg.data);

                doesRoomExist = obj.getBoolean("doesRoomExist");
                questionCount = obj.getInt("questionCount");
                answerTimeout = obj.getInt("answerTimeout");
                maxPlayers = obj.getInt("maxPlayers");
                roomName = obj.getString("roomName");

                JSONArray jPlayers = obj.getJSONArray("players");
                int playersListLength = jPlayers.length();
                players = new String[playersListLength];
                for (int i = 0; i < playersListLength; i++) {
                    players[i] = jPlayers.getString(i);
                }
            } catch (JSONException e) {
            }
        }
    }

    public static class JoinRoomResponse extends Response {
        public JoinRoomResponse(Message msg) {
            super(msg);
        }
    }

    public static class RoomData {
        public int id;
        public int isActive;
        public int maxPlayers;
        public String name;
        public int numOfQuestionsInGame;
        public int timePerQuestion;

        public RoomData(String jsonData) {
            try {
                JSONObject obj = new JSONObject(jsonData);

                id = obj.getInt("id");
                isActive = obj.getInt("isActive");
                maxPlayers = obj.getInt("maxPlayers");
                name = obj.getString("name");
                numOfQuestionsInGame = obj.getInt("numOfQuestionsInGame");
                timePerQuestion = obj.getInt("timePerQuestion");
            } catch (JSONException e) {
            }
        }
    }

    public static class GetRoomsResponse extends Response {
        public RoomData[] rooms;

        public GetRoomsResponse(Message msg) {
            super(msg);
            try {
                JSONObject obj = new JSONObject(msg.data);
                JSONArray jRooms = obj.getJSONArray("rooms");
                int roomsListLength = jRooms.length();
                rooms = new RoomData[roomsListLength];

                for (int i = 0; i < roomsListLength; i++) {
                    rooms[i] = new RoomData(jRooms.getString(i));
                }
            } catch (JSONException e) {
            }
        }
    }

    public static class LeaveRoomResponse extends Response {
        public LeaveRoomResponse(Message msg) {
            super(msg);
        }
    }

    public static class StartGameResponse extends Response {
        public StartGameResponse(Message msg) {
            super(msg);
        }
    }

    public static class LeaveGameResponse extends Response {
        public LeaveGameResponse(Message msg) {
            super(msg);
        }
    }

    public static class SubmitAnswerResponse extends Response {
        public int correctAnswerId;

        public SubmitAnswerResponse(Message msg) {
            super(msg);
            try {
                JSONObject obj = new JSONObject(msg.data);
                correctAnswerId = obj.getInt("correctAnswerId");
            } catch (JSONException e) {
            }
        }
    }

    public static class GetQuestionResponse extends Response {

        public String question;
        public String[] answers;
        public boolean hasMoreQuestions;

        public GetQuestionResponse(Message msg) {
            super(msg);
            try {
                JSONObject obj = new JSONObject(msg.data);

                question = obj.getString("question");
                hasMoreQuestions = obj.getBoolean("hasMoreQuestions");

                JSONObject jAnswers = obj.getJSONObject("answers");

                answers = new String[jAnswers.length()];
                for (int i = 0; i < jAnswers.length(); i++) {
                    answers[i] = jAnswers.getString(Integer.toString(i));
                }

            } catch (JSONException e) {
            }
        }
    }

    public static class GetGameStateResponse extends Response {
        public boolean doesGameEnded;

        public GetGameStateResponse(Message msg) {
            super(msg);
            try {
                JSONObject obj = new JSONObject(msg.data);
                doesGameEnded = obj.getBoolean("doesGameEnded");

            } catch (JSONException e) {
            }
        }
    }

    public static class PlayerResults {
        public String username;
        public int correctAnswerCount;
        public int wrongAnswerCount;
        public double averageAnswerTime;

        public PlayerResults(String jsonData) {
            try {
                JSONObject obj = new JSONObject(jsonData);

                username = obj.getString("username");
                correctAnswerCount = obj.getInt("correctAnswerCount");
                wrongAnswerCount = obj.getInt("wrongAnswerCount");
                averageAnswerTime = obj.getDouble("averageAnswerTime");
            } catch (JSONException e) {
            }
        }
    }

    public static class GetGameResultsResponse extends Response {
        public PlayerResults[] results;

        public GetGameResultsResponse(Message msg) {
            super(msg);
            try {
                JSONObject obj = new JSONObject(msg.data);
                JSONArray jResults = obj.getJSONArray("results");
                int resultsListLength = jResults.length();
                results = new PlayerResults[resultsListLength];

                for (int i = 0; i < resultsListLength; i++) {
                    results[i] = new PlayerResults(jResults.getString(i));
                }
            } catch (JSONException e) {
            }
        }
    }


    public String data;
    public Codes code;

    public Message(String data, Codes code) {
        this.data = data;
        this.code = code;
    }

    public Message(String data, byte code) {
        this.data = data;
        this.code = Codes.valueOf(code);
    }
}
