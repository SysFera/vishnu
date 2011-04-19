// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * TMS_Data/ListJobResults.cpp
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

#include "ListJobResults.hpp"
#include <TMS_Data/JobResult.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "TMS_Data/TMS_DataPackage.hpp"
#include <ecorecpp/mapping.hpp>

#ifdef ECORECPP_NOTIFICATION_API
#include <ecorecpp/notify.hpp>
#endif

using namespace ::TMS_Data;

// Default constructor
ListJobResults::ListJobResults()
{

    m_Results.reset(new ::ecorecpp::mapping::ReferenceEListImpl<
            ::TMS_Data::JobResult, -1, true, false >(this, NULL));

    /*PROTECTED REGION ID(ListJobResultsImpl__ListJobResultsImpl) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

ListJobResults::~ListJobResults()
{
}

/*PROTECTED REGION ID(ListJobResults.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

// Attributes

::ecore::EString const& ListJobResults::getNbJobs() const
{
    return m_nbJobs;
}

void ListJobResults::setNbJobs(::ecore::EString const& _nbJobs)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_nbJobs = m_nbJobs;
#endif
    m_nbJobs = _nbJobs;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getListJobResults__nbJobs(),
                _old_nbJobs,
                m_nbJobs
        );
        eNotify(&notification);
    }
#endif
}

// References
::ecorecpp::mapping::EList< ::TMS_Data::JobResult >& ListJobResults::getResults()
{
    return *m_Results;
}

