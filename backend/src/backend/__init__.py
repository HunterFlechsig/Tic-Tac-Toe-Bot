from flask import Flask, request
import tictactoe as ttt
from uuid import uuid4
import os

app = Flask(__name__)

class GameManager:
    def __init__(self):
        self.games = {}

    def create_game(self, ai_marker=ttt.Marker.O):
        game_id = str(uuid4())
        bot = ttt.MinimaxPlayer(prune=True, useDepth=True)
        bot.setMarker(ai_marker)
        self.games[game_id] = {
            "state": ttt.GameState(ttt.Marker.X),
            "bot": bot,
        }
        return game_id

    def get_game(self, game_id: str):
        return self.games[game_id]["state"]

    def get_bot(self, game_id: str):
        return self.games[game_id]["bot"]

    def update_game(self, game_id: str, game_state: ttt.GameState):
        self.games[game_id]["state"] = game_state

def game_to_dict(gameId: str) -> dict:
    game = game_manager.get_game(gameId)
    winner = game.winner()
    if not game.isTerminal():
        status = "playing"
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
    bot = game_manager.get_bot(game_id)
    ai_move = bot.getMove(game)
    game.makeMove(ai_move)
    game_manager.update_game(game_id, game)
    return game

game_manager = GameManager()

@app.get("/health")
def health():
    return {"status": "ok"}

@app.post("/game")
def create_game() -> dict:
    payload = request.get_json(silent=True) or {}
    ai_marker = ttt.Marker.X if payload.get("ai_marker") == "X" else ttt.Marker.O
    game_id = game_manager.create_game(ai_marker)

    if ai_marker == ttt.Marker.X:
        makeAiMove(game_id)

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
    port = int(os.environ.get("PORT", 5000))
    app.run(host="0.0.0.0", port=port)
