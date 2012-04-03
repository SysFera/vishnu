// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * AMS_Data_forward.hpp
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

#ifndef _AMS_DATA_FORWARD_HPP
#define _AMS_DATA_FORWARD_HPP

#include <ecorecpp/mapping_forward.hpp>

/*PROTECTED REGION ID(AMS_Data_forward) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
// Additional headers here
/*PROTECTED REGION END*/

// EPackage


#include <ecore_forward.hpp> // for EDataTypes
namespace AMS_Data
{

    // EDataType


    // EClass

    // Application
    class Application;
    typedef Application* Application_ptr;

    // ListApplications
    class ListApplications;
    typedef ListApplications* ListApplications_ptr;

    // ListApplicationsOp
    class ListApplicationsOp;
    typedef ListApplicationsOp* ListApplicationsOp_ptr;

    // Package & Factory
    class AMS_DataFactory;
    typedef AMS_DataFactory * AMS_DataFactory_ptr;
    class AMS_DataPackage;
    typedef AMS_DataPackage * AMS_DataPackage_ptr;

    template< typename T, typename S >
    inline T* instanceOf(S* _s)
    {
        return dynamic_cast< T* > (_s);
    }

} // AMS_Data


#endif // _AMS_DATA_FORWARD_HPP
