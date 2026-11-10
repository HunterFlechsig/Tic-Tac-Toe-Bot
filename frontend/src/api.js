async function readJson(response) {
  const body = await response.json().catch(() => ({}))
  if (!response.ok) {
    throw new Error(body.error || "The server could not complete that request.")
  }
  return body
}

export function createGame(aiMarker) {
  return fetch("/game", {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ ai_marker: aiMarker }),
  }).then(readJson)
}

export function makeMove(gameId, move) {
  return fetch(`/game/${gameId}/move`, {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ move }),
  }).then(readJson)
}
