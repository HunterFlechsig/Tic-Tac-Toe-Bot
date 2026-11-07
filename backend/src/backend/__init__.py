from flask import Flask

app = Flask(__name__)


@app.get("/health")
def health():
    return {"status": "ok"}


@app.get("/add/<int:a>/<int:b>")
def add(a: int, b: int):
    import tictactoe

    return {"result": tictactoe.add(a, b)}


def main() -> None:
    app.run(host="0.0.0.0", port=5000)
