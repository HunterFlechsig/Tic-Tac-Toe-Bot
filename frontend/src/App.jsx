import { useRef, useState } from 'react'
import { createGame, makeMove } from './api'
import './App.css'

const BOT_MOVE_DELAY_MS = 300

const EMPTY_BOARD = Array(9).fill('Empty')

const SQUARES = [
  'top left',
  'top center',
  'top right',
  'middle left',
  'center',
  'middle right',
  'bottom left',
  'bottom center',
  'bottom right',
]

function resultMessage(status, playerMarker) {
  if (status === 'Tie') return "It's a tie"
  if (status === playerMarker) return 'You win'
  return 'Bot wins'
}

function waitRemaining(startedAt) {
  const remaining = BOT_MOVE_DELAY_MS - (Date.now() - startedAt)
  if (remaining <= 0) return Promise.resolve()
  return new Promise((resolve) => {
    setTimeout(resolve, remaining)
  })
}

function botPlacedAMark(before, after, playerIndex = -1) {
  return after.some(
    (cell, index) =>
      index !== playerIndex && cell !== 'Empty' && before[index] === 'Empty',
  )
}

function App() {
  const [playerMarker, setPlayerMarker] = useState(null)
  const [game, setGame] = useState(null)
  const [pending, setPending] = useState(false)
  const [error, setError] = useState(null)
  const roundRef = useRef(0)

  const board = game?.board ?? EMPTY_BOARD
  const finished = game && game.status !== 'playing'
  const choosing = playerMarker === null

  async function chooseMarker(marker) {
    if (!choosing || pending) return

    const round = roundRef.current
    const startedAt = Date.now()
    setPending(true)
    setError(null)
    try {
      const created = await createGame(marker === 'X' ? 'O' : 'X')
      if (roundRef.current !== round) return
      setPlayerMarker(marker)
      if (botPlacedAMark(EMPTY_BOARD, created.board)) {
        await waitRemaining(startedAt)
      }
      if (roundRef.current !== round) return
      setGame(created)
    } catch (err) {
      if (roundRef.current === round) setError(err.message)
    } finally {
      if (roundRef.current === round) setPending(false)
    }
  }

  async function playSquare(index) {
    if (
      pending ||
      !game ||
      game.status !== 'playing' ||
      game.board[index] !== 'Empty'
    ) {
      return
    }

    const round = roundRef.current
    const startedAt = Date.now()
    const before = game.board
    const shown = before.slice()
    shown[index] = playerMarker
    setGame({ ...game, board: shown })
    setPending(true)
    setError(null)
    try {
      const updated = await makeMove(game.game_id, index)
      if (roundRef.current !== round) return
      if (botPlacedAMark(before, updated.board, index)) {
        await waitRemaining(startedAt)
        if (roundRef.current !== round) return
      }
      setGame(updated)
    } catch (err) {
      if (roundRef.current !== round) return
      setGame({ ...game, board: before })
      setError(err.message)
    } finally {
      if (roundRef.current === round) setPending(false)
    }
  }

  function reset() {
    roundRef.current += 1
    setPlayerMarker(null)
    setGame(null)
    setError(null)
    setPending(false)
  }

  let status = 'Pick a side'
  if (finished) status = resultMessage(game.status, playerMarker)
  else if (game) status = 'Your turn'

  const outcome =
    game?.status === 'X' || game?.status === 'O'
      ? `winner-${game.status.toLowerCase()}`
      : game?.status === 'Tie'
        ? 'tie'
        : ''

  return (
    <main className="app">
      <header>
        <h1>Tic-Tac-Toe</h1>
        <p>Play the minimax bot</p>
      </header>

      <div className="choices" role="group" aria-label="Play as">
        {['X', 'O'].map((marker) => (
          <button
            key={marker}
            type="button"
            className={`choice ${marker.toLowerCase()}${
              playerMarker === marker ? ' selected' : ''
            }`}
            aria-pressed={playerMarker === marker}
            disabled={!choosing || pending}
            onClick={() => chooseMarker(marker)}
          >
            {marker}
          </button>
        ))}
      </div>

      <p className="status" role="status" aria-live="polite">
        {status}
      </p>

      <div
        className={`board${outcome ? ` ${outcome}` : ''}`}
        role="group"
        aria-label="Board"
      >
        {board.map((cell, index) => {
          const empty = cell === 'Empty'
          const locked = !game || finished || pending || !empty
          return (
            <button
              key={index}
              type="button"
              className={`square ${empty ? 'empty' : cell.toLowerCase()}`}
              aria-label={`${SQUARES[index]}, ${empty ? 'empty' : cell}`}
              disabled={locked}
              onClick={() => playSquare(index)}
            >
              {empty ? '' : cell}
            </button>
          )
        })}
      </div>

      <button
        type="button"
        className="reset"
        onClick={reset}
        disabled={!game || pending}
      >
        Reset
      </button>

      {error && <p className="error">{error}</p>}
    </main>
  )
}

export default App
