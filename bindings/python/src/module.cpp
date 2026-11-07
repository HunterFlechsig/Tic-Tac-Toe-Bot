#include <pybind11/pybind11.h>
#include <Board.hpp>
namespace py = pybind11;

PYBIND11_MODULE(board, m, py::smart_holder()) {
    py::class_<Board>(m,"Board")
        .def(py::init())
        .def("isFull", &Board::isFull)
        .def("place", &Board::place)
        .def("getCell", &Board::getCell);
}