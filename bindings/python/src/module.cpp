#include <pybind11/native_enum.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <Board.hpp>
#include <Game.hpp>
#include <GameState.hpp>
#include <Player.hpp>

namespace py = pybind11;

PYBIND11_MODULE(tictactoe, m) {
    py::native_enum<Marker>(m, "Marker", "enum.Enum")
        .value("X", Marker::X)
        .value("O", Marker::O)
        .value("Empty", Marker::Empty)
        .finalize();

    py::class_<Board>(m, "Board")
        .def(py::init<>())
        .def("isFull", &Board::isFull)
        .def("place", &Board::place, py::arg("index"), py::arg("marker"))
        .def("getCell", &Board::getCell, py::arg("index"))
        .def("printBoard", &Board::printBoard);

    py::class_<GameState>(m, "GameState")
        .def(py::init<Marker>(), py::arg("current_mark"))
        .def("makeMove", &GameState::makeMove, py::arg("index"))
        .def("switchPlayer", &GameState::switchPlayer)
        .def("isTerminal", &GameState::isTerminal)
        .def("getCurrentTurn", &GameState::getCurrentTurn)
        .def("winner", &GameState::winner)
        .def("getBoard", &GameState::getBoard, py::return_value_policy::reference_internal)
        .def("legalMoves", &GameState::legalMoves);

    py::class_<Player>(m, "Player")
        .def("setMarker", &Player::setMarker, py::arg("marker"))
        .def("getMarker", &Player::getMarker)
        .def("getMove", &Player::getMove, py::arg("state"));

    py::class_<HumanPlayer, Player>(m, "HumanPlayer")
        .def(py::init<>());

    py::class_<RandomPlayer, Player>(m, "RandomPlayer")
        .def(py::init<>())
        .def(py::init<std::uint32_t>(), py::arg("seed"));

    py::class_<MinimaxPlayer, Player>(m, "MinimaxPlayer")
        .def(py::init<bool, bool>(), py::arg("prune"), py::arg("useDepth"))
        .def_readwrite("prune", &MinimaxPlayer::prune)
        .def_readwrite("useDepth", &MinimaxPlayer::useDepth)
        .def("nodesExplored", &MinimaxPlayer::nodesExplored)
        .def("decisionTime", &MinimaxPlayer::decisionTime);

    py::native_enum<Result>(m, "Result", "enum.Enum")
        .value("Xwin", Result::Xwin)
        .value("Owin", Result::Owin)
        .value("Tie", Result::Tie)
        .finalize();

    py::class_<Game>(m, "Game")
        .def(py::init<Player&, Player&>(), py::arg("x_player"), py::arg("o_player"))
        .def("run", &Game::run)
        .def("currentPlayer", &Game::currentPlayer, py::return_value_policy::reference);
}
