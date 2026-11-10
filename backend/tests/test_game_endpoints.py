import tictactoe as ttt

from backend import app, game_manager


def test_create_game_with_ai_marker_x_updates_board():
    client = app.test_client()

    created = client.post("/game", json={"ai_marker": "X"})

    assert created.status_code == 200
    body = created.get_json()
    game_id = body["game_id"]
    assert isinstance(game_id, str)
    assert game_id

    board = ["Empty"] * 9
    board[body["board"].index("X")] = "X"
    assert body == {
        "game_id": game_id,
        "board": board,
        "current_turn": "O",
        "status": "playing",
    }

    fetched = client.get(f"/game/{game_id}")

    assert fetched.status_code == 200
    assert fetched.get_json() == body


def create_game(client, ai_marker):
    created = client.post("/game", json={"ai_marker": ai_marker})
    assert created.status_code == 200
    return created.get_json()


def store_position(game_id, moves):
    game = ttt.GameState(ttt.Marker.X)
    for move in moves:
        assert game.makeMove(move)
    game_manager.update_game(game_id, game)


def test_move_places_player_mark_and_ai_reply():
    client = app.test_client()
    game_id = create_game(client, "O")["game_id"]

    moved = client.post(f"/game/{game_id}/move", json={"move": 0})

    assert moved.status_code == 200
    body = moved.get_json()
    assert body["game_id"] == game_id
    assert body["board"][0] == "X"
    assert body["board"].count("X") == 1
    assert body["board"].count("O") == 1
    assert body["current_turn"] == "X"
    assert body["status"] == "playing"


def test_winning_move_returns_winner_without_ai_reply():
    client = app.test_client()
    game_id = create_game(client, "O")["game_id"]
    store_position(game_id, [0, 3, 1, 4])

    moved = client.post(f"/game/{game_id}/move", json={"move": 2})

    assert moved.status_code == 200
    assert moved.get_json() == {
        "game_id": game_id,
        "board": ["X", "X", "X", "O", "O", "Empty", "Empty", "Empty", "Empty"],
        "current_turn": "O",
        "status": "X",
    }


def test_ai_reply_returns_ai_winner():
    client = app.test_client()
    game_id = create_game(client, "O")["game_id"]
    store_position(game_id, [3, 0, 4, 1])

    moved = client.post(f"/game/{game_id}/move", json={"move": 8})

    assert moved.status_code == 200
    assert moved.get_json() == {
        "game_id": game_id,
        "board": ["O", "O", "O", "X", "X", "Empty", "Empty", "Empty", "X"],
        "current_turn": "X",
        "status": "O",
    }


def test_second_game_does_not_change_the_first_bots_marker():
    client = app.test_client()
    o_game = create_game(client, "O")["game_id"]
    create_game(client, "X")
    store_position(o_game, [3, 0, 4, 1])

    moved = client.post(f"/game/{o_game}/move", json={"move": 8})

    assert moved.status_code == 200
    assert moved.get_json() == {
        "game_id": o_game,
        "board": ["O", "O", "O", "X", "X", "Empty", "Empty", "Empty", "X"],
        "current_turn": "X",
        "status": "O",
    }


def test_filling_move_returns_tie():
    client = app.test_client()
    game_id = create_game(client, "O")["game_id"]
    store_position(game_id, [0, 1, 2, 3, 4, 6, 5, 8])

    moved = client.post(f"/game/{game_id}/move", json={"move": 7})

    assert moved.status_code == 200
    assert moved.get_json() == {
        "game_id": game_id,
        "board": ["X", "O", "X", "O", "X", "X", "O", "X", "O"],
        "current_turn": "O",
        "status": "Tie",
    }
