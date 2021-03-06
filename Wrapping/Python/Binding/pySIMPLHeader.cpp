#include <memory>
#include <typeinfo>
#include <utility>

#include <QtCore/QDateTime>
#include <QtCore/QString>

#include <pybind11/pybind11.h>

#include <pybind11/functional.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

#include "SIMPLib/Common/PhaseType.h"
#include "SIMPLib/Common/QtBackwardCompatibilityMacro.h"
#include "SIMPLib/Common/ShapeType.h"
#include "SIMPLib/CoreFilters/ArrayCalculator.h"
#include "SIMPLib/CoreFilters/ImportHDF5Dataset.h"

namespace py = pybind11;
using namespace py::literals;

PYBIND11_MAKE_OPAQUE(std::vector<int8_t>);
PYBIND11_MAKE_OPAQUE(std::vector<uint8_t>);
PYBIND11_MAKE_OPAQUE(std::vector<int16_t>);
PYBIND11_MAKE_OPAQUE(std::vector<uint16_t>);
PYBIND11_MAKE_OPAQUE(std::vector<int32_t>);
PYBIND11_MAKE_OPAQUE(std::vector<uint32_t>);
PYBIND11_MAKE_OPAQUE(std::vector<int64_t>);
PYBIND11_MAKE_OPAQUE(std::vector<uint64_t>);

PYBIND11_MAKE_OPAQUE(std::vector<float>);
PYBIND11_MAKE_OPAQUE(std::vector<double>);

/*
 * Linux does not like the below line because unsigned long int and size_t are
 * the same thing. Apple (clang) and Windows (MSVC) do not seem to have a problem
 * with the line.
 */
#if defined(__APPLE__)
PYBIND11_MAKE_OPAQUE(std::vector<size_t>);
#endif

#include "Binding/Pybind11CustomTypeCasts.h"
#include "Binding/pySupport.h"

#ifdef SIMPL_EMBED_PYTHON
#include "SIMPLib/Filtering/FilterManager.h"
#include "SIMPLib/Python/FilterPyObject.h"
#include "SIMPLib/Python/PythonFilter.h"
#include "SIMPLib/Python/PythonFilterFactory.hpp"
#endif
