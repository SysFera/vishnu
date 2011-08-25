// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * IMS_Data/IMS_DataFactory.cpp
 * Copyright (C) Cátedra SAES-UMU 2010 <andres.senac@um.es>
 *
 * EMF4CPP is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * EMF4CPP is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <IMS_Data/IMS_DataFactory.hpp>

using namespace ::IMS_Data;

std::auto_ptr< ::IMS_Data::IMS_DataFactory > IMS_DataFactory::s_instance;

::IMS_Data::IMS_DataFactory_ptr IMS_DataFactory::_instance()
{
    if (!s_instance.get())
    {
        new IMS_DataFactory();
    }
    return s_instance.get();
}

