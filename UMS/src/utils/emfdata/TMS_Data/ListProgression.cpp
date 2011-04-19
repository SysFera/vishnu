// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * TMS_Data/ListProgression.cpp
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

#include "ListProgression.hpp"
#include <TMS_Data/Progression.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "TMS_Data/TMS_DataPackage.hpp"
#include <ecorecpp/mapping.hpp>

#ifdef ECORECPP_NOTIFICATION_API
#include <ecorecpp/notify.hpp>
#endif

using namespace ::TMS_Data;

// Default constructor
ListProgression::ListProgression()
{

    m_progress.reset(new ::ecorecpp::mapping::ReferenceEListImpl<
            ::TMS_Data::Progression, -1, true, false >(this, NULL));

    /*PROTECTED REGION ID(ListProgressionImpl__ListProgressionImpl) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

ListProgression::~ListProgression()
{
}

/*PROTECTED REGION ID(ListProgression.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

// Attributes

::ecore::EInt ListProgression::getNbJobs() const
{
    return m_nbJobs;
}

void ListProgression::setNbJobs(::ecore::EInt _nbJobs)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EInt _old_nbJobs = m_nbJobs;
#endif
    m_nbJobs = _nbJobs;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getListProgression__nbJobs(),
                _old_nbJobs,
                m_nbJobs
        );
        eNotify(&notification);
    }
#endif
}

// References
::ecorecpp::mapping::EList< ::TMS_Data::Progression >& ListProgression::getProgress()
{
    return *m_progress;
}

