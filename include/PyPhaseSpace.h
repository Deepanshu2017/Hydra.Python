
/*----------------------------------------------------------------------------
 *
 *   Copyright (C) 2017 Deepanshu Thakur
 *
 *
 *   This file is part of Hydra.Python Analysis Framework.
 *
 *   Hydra is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   Hydra is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with Hydra.  If not, see <http://www.gnu.org/licenses/>.
 *
 *---------------------------------------------------------------------------*/

/*
 * Vector.cpp
 *
 *  Created on: Aug 4, 2017
 *      Author: Deepanshu Thakur
 */


/*
 * @file
 *
 * @ingroup
 *
 * @brief
 *
 * @todo
 *
 */
#ifndef PYPHASESPACE_H_
#define PYPHASESPACE_H_
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include "hydra/Types.h"
#include "hydra/PhaseSpace.h"
#include "hydra/Vector4R.h"
#include <hydra/device/System.h>
#include <hydra/host/System.h>
#include <vector>

namespace py = pybind11;

namespace hydra_python {
template<>
void add_object< hydra::PhaseSpace<4, thrust::random::default_random_engine> >(pybind11::module &m) {
    py::class_< hydra::PhaseSpace<4, thrust::random::default_random_engine> >(m, "PhaseSpace")
        .def(py::init<hydra::GReal_t, const std::array<hydra::GReal_t, 4>& >())
//        .def(py::init<hydra::PhaseSpace<4, thrust::random::default_random_engine> >())
        .def("GetSeed", &hydra::PhaseSpace<4, thrust::random::default_random_engine>::GetSeed)
        .def("SetSeed", &hydra::PhaseSpace<4, thrust::random::default_random_engine>::SetSeed)
        .def("Generate", [](hydra::PhaseSpace<4, thrust::random::default_random_engine>& p, hydra::Vector4R& vec, hydra::Events<4, hydra::host::sys_t>& e){
            p.Generate(vec, e.begin(), e.end());
        })
        .def("Generate", [](hydra::PhaseSpace<4, thrust::random::default_random_engine>& p, hydra::Events<4, hydra::host::sys_t>& e1, hydra::Events<4, hydra::host::sys_t>& e2) {
            p.Generate(e1.DaughtersBegin(0), e1.DaughtersEnd(0), e2.begin());
        })
        .def("AverageOnHost", [](hydra::PhaseSpace<4, thrust::random::default_random_engine>& p, hydra::Vector4R& v, py::function& fun, size_t nentries) {
            auto functor = [=](hydra::Vector4R* data) {
                std::vector<hydra::Vector4R> vec;
                for (int i = 0; i < 3; ++i)
                    vec.push_back(data[i]);
                return fun(vec).cast<hydra::GReal_t>();
            };
            return p.AverageOn(hydra::host::sys, v, functor, nentries);
        })
        .def("AverageOnHost", [](hydra::PhaseSpace<4, thrust::random::default_random_engine>& p, hydra::Events<4, hydra::host::sys_t>& e, py::function& fun) {
            //Enter typical implementation of Functor.
            //return p.AverageOn(e.begin(), e.end(), functor);
            return 42.0;
        })
        .def("AverageOnDevice", [](hydra::PhaseSpace<4, thrust::random::default_random_engine>& p, hydra::Vector4R& v, py::function& fun, size_t nentries) {
            auto functor = [=](hydra::Vector4R* data) {
                std::vector<hydra::Vector4R> vec;
                for (int i = 0; i < 3; ++i)
                    vec.push_back(data[i]);
                return fun(vec).cast<hydra::GReal_t>();
            };
            return p.AverageOn(hydra::device::sys, v, functor, nentries);
        })
        .def("AverageOnDevice", [](hydra::PhaseSpace<4, thrust::random::default_random_engine>& p, hydra::Events<4, hydra::host::sys_t>& e, py::function& fun) {
            //Enter typical implementation of Functor.
            //return p.AverageOn(e.begin(), e.end(), functor);
            return 42.0;
        })
//        .def("Evaluate", [](hydra::PhaseSpace<4, thrust::random::default_random_engine>& p, hydra::Vector4R& v, hydra::Events<4, hydra::host::sys_t>& e, py::function& ... fun){
//            return 42.0;
//        })
    ;
}
}
#endif /* PYPHASESPACE_H_ */
