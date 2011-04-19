// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * TMS_Data/ListQueues.cpp
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

#include "ListQueues.hpp"
#include <TMS_Data/Queue.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "TMS_Data/TMS_DataPackage.hpp"
#include <ecorecpp/mapping.hpp>

#ifdef ECORECPP_NOTIFICATION_API
#include <ecorecpp/notify.hpp>
#endif

using namespace ::TMS_Data;

// Default constructor
ListQueues::ListQueues()
{

    m_queues.reset(new ::ecorecpp::mapping::ReferenceEListImpl<
            ::TMS_Data::Queue, -1, true, false >(this, NULL));

    /*PROTECTED REGION ID(ListQueuesImpl__ListQueuesImpl) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

ListQueues::~ListQueues()
{
}

/*PROTECTED REGION ID(ListQueues.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

// Attributes

::ecore::EInt ListQueues::getNbQueues() const
{
    return m_nbQueues;
}

void ListQueues::setNbQueues(::ecore::EInt _nbQueues)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EInt _old_nbQueues = m_nbQueues;
#endif
    m_nbQueues = _nbQueues;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getListQueues__nbQueues(),
                _old_nbQueues,
                m_nbQueues
        );
        eNotify(&notification);
    }
#endif
}

// References
::ecorecpp::mapping::EList< ::TMS_Data::Queue >& ListQueues::getQueues()
{
    return *m_queues;
}

