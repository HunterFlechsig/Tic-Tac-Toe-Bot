# Tic-Tac-Toe Bot

A tic-tac-toe opponent whose moves come from a C++ minimax engine. The website, the API tests, and a measurement notebook all call that same library. Nothing in Python or JavaScript reimplements the rules or the search.

**Play it:** [tic-tac-toe-bot-1-waa8.onrender.com](https://tic-tac-toe-bot-1-waa8.onrender.com)

Choose X or O. Choosing O makes the bot move first. The bot searches the rest of the game on every turn. Two copies of it always tie. Against a person, it does not lose.

The host sleeps when idle, so the first visit can take a few seconds to wake.

## What this demonstrates

- A C++23 game library with a player interface, so the same loop can run a person, a random baseline, or the search bot.
- A pybind11 extension that Flask imports directly. The API stores games and translates JSON. It does not own the board logic.
- A React client that shows your mark immediately, then reveals the bot’s reply. A finished game does not wait on an extra move.
- Docker images that compile the extension into the API and serve the UI from nginx. nginx proxies `/game` to the API, so the public site is one URL.
- Tests on both sides of the binding: GoogleTest for rules and search behavior, pytest for the HTTP API.
- A notebook that plays 1,000 games per matchup and records nodes visited, decision time, and how fast the bot wins.

## Architecture

```text
Browser
  React (Vite)
    │  POST /game, POST /game/:id/move
    ▼
nginx
  proxies /game, serves the built UI
    ▼
Flask
  in-memory games, JSON board state
    ▼
tictactoe  (pybind11)
    ▼
C++ engine
  Board · GameState · MinimaxPlayer
```

| Path | Role |
| --- | --- |
| `engine/` | C++ library, terminal CLI, GoogleTest |
| `bindings/python/` | pybind11 module, built with scikit-build-core |
| `backend/` | Flask API and pytest |
| `frontend/` | React UI |
| `experiments/` | Jupyter notebook that drives the extension |
| `deploy/` | Dockerfiles, nginx template, Compose file |

## The bot

`MinimaxPlayer` scores each line from its own point of view:

- its own win is `10 - depth`, so an earlier win ranks higher
- the opponent’s win is `depth - 10`, so a later loss ranks higher
- a filled board with no winner is `0`

`prune` enables alpha-beta cutoffs. `useDepth` turns the depth term on or off, which lets the notebook compare “win eventually” with “win sooner.” The API constructs the bot with both flags on.

Each decision records nodes visited and elapsed time. The notebook in `experiments/src/experiments/notebook.ipynb` uses those counters. In the saved run of 1,000 games:

- minimax versus minimax tied every game
- minimax never lost to a seeded random player
- alpha-beta is plotted against a full search on nodes and milliseconds per ply

GoogleTest checks the same behavior on single positions: take an immediate win, block an immediate loss, prefer the faster win, and match plain minimax when alpha-beta is on.

## API

Games live in memory on the API process. A restart clears them. Cells are `X`, `O`, or `Empty`. `status` is `playing`, `X`, `O`, or `Tie`. Square `0` is the top left and `8` is the bottom right.

| Method | Path | Body | Result |
| --- | --- | --- | --- |
| `GET` | `/health` | | `{ "status": "ok" }` |
| `POST` | `/game` | `{ "ai_marker": "X" \| "O" }` | Creates a game. When the bot is X, the response already includes its first mark. |
| `GET` | `/game/<id>` | | Board, whose turn it is, and status |
| `POST` | `/game/<id>/move` | `{ "move": 0-8 }` | Applies the human move, then the bot’s reply while the game is still open |

An occupied square, a square outside the board, or a move after the game has ended returns `400`.

## Stack

C++23, CMake, GoogleTest, pybind11, scikit-build-core, uv, Python 3.14, Flask, pytest, React 19, Vite, Docker, nginx.

## Run it locally

From the repository root. A local build needs a C++23 compiler, CMake, Python 3.14, and [uv](https://docs.astral.sh/uv/).

Engine, CLI, and C++ tests. The CLI plays minimax as X against a person in the terminal:

```bash
cmake -S . -B build
cmake --build build
./build/tictactoe
ctest --test-dir build --output-on-failure
```

API. `uv sync` builds the C++ extension into the backend environment. The server listens on port 5000.

```bash
uv sync --project backend
uv run --project backend backend
```

UI. The Vite dev server proxies `/game` to `http://127.0.0.1:5000`.

```bash
npm --prefix frontend install
npm --prefix frontend run dev
```

API tests, after the backend environment is synced:

```bash
uv run --project backend pytest
```

Notebook, after syncing the experiments environment:

```bash
uv sync --project experiments
uv run --project experiments jupyter notebook
```

Both services in Docker. The UI is on port 8080 and the API is on port 5000.

```bash
docker compose -f deploy/docker-compose.yml up --build
```
