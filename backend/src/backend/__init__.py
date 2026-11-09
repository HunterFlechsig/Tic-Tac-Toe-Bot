from flask import Flask
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

ai = ttt.MinimaxPlayer(prune=True, useDepth=True)
game_manager = GameManager()

@app.get("/health")
def health():
    return {"status": "ok"}

@app.get("/game")
def create_game():
    game_id = game_manager.create_game()
    return {"game_id": game_id}

def game_to_dict(game: ttt.GameState) -> dict:
    board = game.getBoard()
    return {
        "board": [board.getCell(index).name for index in range(9)],
        "current_turn": game.getCurrentTurn().name,
    }

@app.get("/game/<game_id>")
def get_game(game_id: str):
    game = game_manager.get_game(game_id)
    return {"game": game_to_dict(game)}


# @app.post("/game/<game_id>/move")
# def make_move(game_id: str, move: int):
#     if(game.is_terminal()):

#     game = game_manager.get_game(game_id)
#     if not game.make_move(move):
#         return {"error": "Invalid move"}, 400
#     ai_move = ai.get_move(game)
#     game.make_move(ai_move)
#     game_manager.update_game(game_id, game)
#     return {"game": game.to_dict()}

def main() -> None:
    app.run(host="0.0.0.0", port=5000)
