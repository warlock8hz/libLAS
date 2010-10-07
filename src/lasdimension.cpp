/******************************************************************************
 * $Id$
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  LAS Dimension implementation for C++ libLAS 
 * Author:   Howard Butler, hobu.inc@gmail.com
 *
 ******************************************************************************
 * Copyright (c) 2010, Howard Butler
 *
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following 
 * conditions are met:
 * 
 *     * Redistributions of source code must retain the above copyright 
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright 
 *       notice, this list of conditions and the following disclaimer in 
 *       the documentation and/or other materials provided 
 *       with the distribution.
 *     * Neither the name of the Martin Isenburg or Iowa Department 
 *       of Natural Resources nor the names of its contributors may be 
 *       used to endorse or promote products derived from this software 
 *       without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED 
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 ****************************************************************************/

#include <liblas/lasschema.hpp>
#include <liblas/detail/private_utility.hpp>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4512)
#endif

#include <liblas/external/property_tree/xml_parser.hpp>

#ifdef _MSC_VER
#pragma warning(pop)
#endif
// boost
#include <boost/cstdint.hpp>
// std
#include <algorithm>
#include <sstream>

using namespace boost;

namespace liblas { 

Dimension::Dimension(std::string const& name, boost::uint32_t size_in_bits) : 
    m_name(name), 
    m_bit_size(size_in_bits),
    m_required(false),
    m_active(false),
    m_description(std::string("")),
    m_min(0),
    m_max(0),
    m_numeric(false),
    m_signed(false),
    m_integer(false),
    m_position(0)
{
     if (size_in_bits == 0) {
        std::ostringstream oss;
        oss << "The bit size of the dimension is 0, the dimension is invalid.";
        throw std::runtime_error(oss.str());
    }
};

/// copy constructor
Dimension::Dimension(Dimension const& other) :
    m_name(other.m_name)
    , m_bit_size(other.m_bit_size)
    , m_required(other.m_required)
    , m_active(other.m_active)
    , m_description(other.m_description)
    , m_min(other.m_min)
    , m_max(other.m_max)
    , m_numeric(other.m_numeric)
    , m_signed(other.m_signed)
    , m_integer(other.m_integer)
    , m_position(other.m_position)
{
}
// 
// // assignment constructor
Dimension& Dimension::operator=(Dimension const& rhs)
{
    if (&rhs != this)
    {
        m_name = rhs.m_name;
        m_bit_size = rhs.m_bit_size;
        m_required = rhs.m_required;
        m_active = rhs.m_active;
        m_description = rhs.m_description;
        m_min = rhs.m_min;
        m_max = rhs.m_max;
        m_numeric = rhs.m_numeric;
        m_signed = rhs.m_signed;
        m_integer = rhs.m_integer;
        m_position = rhs.m_position;
    }
    
    return *this;
}

std::size_t Dimension::GetByteSize() const 
{

    std::size_t bit_position = m_bit_size % 8;
    if (bit_position > 0) {
        // For dimensions that are not byte aligned,
        // we need to determine how many bytes they 
        // will take.  We have to read at least one byte if the 
        // size in bits is less than 8.  If it is more than 8, 
        // we need to read the number of bytes it takes + 1 extra.
        if (m_bit_size > 8) {
            return m_bit_size/8 + 1;
        } else {
            return 1;
        }
    }
    return m_bit_size / 8;
}

} // namespace liblas
