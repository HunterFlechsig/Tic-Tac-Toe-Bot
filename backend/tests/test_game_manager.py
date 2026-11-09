import tictactoe as ttt

from backend import GameManager


def test_create_game_can_be_fetched_by_id():
    manager = GameManager()

    game_id = manager.create_game()
    game = manager.get_game(game_id)

    assert isinstance(game_id, str)
    assert game_id
    assert game.getCurrentTurn() == ttt.Marker.X
    assert [game.getBoard().getCell(index) for index in range(9)] == [ttt.Marker.Empty] * 9


def test_update_game_with_move_returns_that_state():
    manager = GameManager()
    game_id = manager.create_game()

    moved = ttt.GameState(ttt.Marker.X)
    assert moved.makeMove(4)
    manager.update_game(game_id, moved)

    stored = manager.get_game(game_id)

    assert stored is moved
    assert stored.getBoard().getCell(4) == ttt.Marker.X
    assert stored.getCurrentTurn() == ttt.Marker.O
