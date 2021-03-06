///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1998-2011, Industrial Light & Magic, a division of Lucas
// Digital Ltd. LLC
// 
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// *       Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
// *       Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
// *       Neither the name of Industrial Light & Magic nor the names of
// its contributors may be used to endorse or promote products derived
// from this software without specific prior written permission. 
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
///////////////////////////////////////////////////////////////////////////

#ifndef _PyImathVec4ArrayImpl_h_
#define _PyImathVec4ArrayImpl_h_

//
// This .C file was turned into a header file so that instantiations
// of the various V4* types can be spread across multiple files in
// order to work around MSVC limitations.
//

#include <PyImathVec.h>
#include "PyImathDecorators.h"
#include <Python.h>
#include <boost/python.hpp>
#include <boost/python/make_constructor.hpp>
#include <boost/format.hpp>
#include <PyImath.h>
#include <ImathVec.h>
#include <ImathVecAlgo.h>
#include <Iex.h>
#include <PyImathMathExc.h>
#include <PyImathOperators.h>
#include <PyImathVecOperators.h>

namespace PyImath {
using namespace boost::python;
using namespace Imath;

// XXX fixme - template this
// really this should get generated automatically...

template <class T,int index>
static FixedArray<T>
Vec4Array_get(FixedArray<Imath::Vec4<T> > &va)
{
    return FixedArray<T>(&va[0][index],va.len(),4*va.stride(),va.handle());
}

template <class T>
static void
setItemTuple(FixedArray<Imath::Vec4<T> > &va, Py_ssize_t index, const tuple &t)
{
    if(t.attr("__len__")() == 4)
    {
        Vec4<T> v;
        v.x = extract<T>(t[0]);
        v.y = extract<T>(t[1]);
        v.z = extract<T>(t[2]);
        v.w = extract<T>(t[3]);

        va[va.canonical_index(index)] = v;
    }
    else
        THROW(Iex::LogicExc, "tuple of length 4 expected");
}

template <class T>
class_<FixedArray<Imath::Vec4<T> > >
register_Vec4Array()
{
    using boost::mpl::true_;

    class_<FixedArray<Imath::Vec4<T> > > vec4Array_class = FixedArray<Imath::Vec4<T> >::register_("Fixed length array of Imath::Vec4");
    vec4Array_class
        .add_property("x",&Vec4Array_get<T,0>)
        .add_property("y",&Vec4Array_get<T,1>)
        .add_property("z",&Vec4Array_get<T,2>)
        .add_property("w",&Vec4Array_get<T,3>)
        .def("__setitem__", &setItemTuple<T>)
        ;

    add_arithmetic_math_functions(vec4Array_class);
    add_comparison_functions(vec4Array_class);

    generate_member_bindings<op_vecLength<Imath::Vec4<T> >     >(vec4Array_class,"length","");
    generate_member_bindings<op_vecLength2<Imath::Vec4<T> >    >(vec4Array_class,"length2","");
    generate_member_bindings<op_vecNormalize<Imath::Vec4<T> >  >(vec4Array_class,"normalize","");
    generate_member_bindings<op_vecNormalized<Imath::Vec4<T> > >(vec4Array_class,"normalized","");

    generate_member_bindings<op_vecDot<Imath::Vec4<T> >,true_>(vec4Array_class,"dot","return the inner product of (self,x)",boost::python::args("x"));
    generate_member_bindings<op_mul<Imath::Vec4<T>,T>,  true_>(vec4Array_class,"__mul__" ,"self*x", boost::python::args("x"));
    generate_member_bindings<op_mul<Imath::Vec4<T>,T>,  true_>(vec4Array_class,"__rmul__","x*self", boost::python::args("x"));
    generate_member_bindings<op_imul<Imath::Vec4<T>,T>, true_>(vec4Array_class,"__imul__","self*=x",boost::python::args("x"));
    generate_member_bindings<op_div<Imath::Vec4<T>,T>,  true_>(vec4Array_class,"__div__" ,"self/x", boost::python::args("x"));
    generate_member_bindings<op_idiv<Imath::Vec4<T>,T>, true_>(vec4Array_class,"__idiv__","self/=x",boost::python::args("x"));

    decoratecopy(vec4Array_class);

    return vec4Array_class;
}


}  // namespace PyImath

#endif   // _PyImathVec4ArrayImpl_h_
