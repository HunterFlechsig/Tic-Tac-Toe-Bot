from flask import Flask, request
import tictactoe as ttt
from uuid import uuid4

app = Flask(__name__)

class GameManager:
    def __init__(self):
        self.games = {}
    
    def create_game(self: str):
        game_id = str(uuid4())
        self.games[game_id] = ttt.GameState(ttt.Marker.X)
        return game_id
    
    def get_game(self, game_id: str):
        return self.games[game_id]
    
    def update_game(self, game_id: str, game_state: ttt.GameState):
        self.games[game_id] = game_state

def game_to_dict(gameId: str) -> dict:
    game = game_manager.get_game(gameId)
    winner = game.winner()
    if not game.isTerminal():
        status = "In Progress"
    elif winner is None:
        status = "Tie"
    else:
        status = winner.name

    return {
        "game_id": gameId,
        "board": [game.getBoard().getCell(index).name for index in range(9)],
        "current_turn": game.getCurrentTurn().name,
        "status": status,
    }

def makeAiMove(game_id: str):
    game = game_manager.get_game(game_id)
    ai_move = ai.getMove(game)
    game.makeMove(ai_move)
    game_manager.update_game(game_id, game)
    return game

ai = ttt.MinimaxPlayer(prune=True, useDepth=True)
game_manager = GameManager()

@app.get("/health")
def health():
    return {"status": "ok"}

@app.post("/game")
def create_game() -> dict:
    payload = request.get_json(silent=True) or {}
    ai_marker = payload.get("ai_marker")
    game_id = game_manager.create_game()

    if ai_marker == "X":
        ai.setMarker(ttt.Marker.X)
        makeAiMove(game_id)
    else:
        ai.setMarker(ttt.Marker.O)

    return game_to_dict(game_id)

@app.get("/game/<game_id>")
def get_game(game_id: str):
    return game_to_dict(game_id)

@app.post("/game/<game_id>/move")
def make_move(game_id: str):
    payload = request.get_json(silent=True) or {}
    move = payload.get("move")
    game = game_manager.get_game(game_id)

    if game.isTerminal():
        return {"error": "Game is terminal"}, 400

    if not isinstance(move, int) or not game.makeMove(move):
        return {"error": "Invalid move"}, 400

    game_manager.update_game(game_id, game)

    if not game.isTerminal():
        makeAiMove(game_id)

    return game_to_dict(game_id)

def main() -> None:
    app.run(host="0.0.0.0", port=5000)
