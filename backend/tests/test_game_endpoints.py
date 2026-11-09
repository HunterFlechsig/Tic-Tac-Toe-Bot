from backend import app


def test_create_game_returns_id_and_get_returns_that_game():
    client = app.test_client()

    created = client.get("/game")

    assert created.status_code == 200
    game_id = created.get_json()["game_id"]
    assert isinstance(game_id, str)
    assert game_id

    fetched = client.get(f"/game/{game_id}")

    assert fetched.status_code == 200
    assert fetched.get_json() == {
        "game": {
            "board": ["Empty"] * 9,
            "current_turn": "X",
        }
    }
