// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * IMS_Data/MetricHistOp.cpp
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

#include "MetricHistOp.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "IMS_Data/IMS_DataPackage.hpp"
#include <ecorecpp/mapping.hpp>

#ifdef ECORECPP_NOTIFICATION_API
#include <ecorecpp/notify.hpp>
#endif

using namespace ::IMS_Data;

// Default constructor
MetricHistOp::MetricHistOp() :
    m_startTime(0), m_endTime(0), m_type(0)
{

    /*PROTECTED REGION ID(MetricHistOpImpl__MetricHistOpImpl) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

MetricHistOp::~MetricHistOp()
{
}

/*PROTECTED REGION ID(MetricHistOp.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

// Attributes

::ecore::ELong MetricHistOp::getStartTime() const
{
    return m_startTime;
}

void MetricHistOp::setStartTime(::ecore::ELong _startTime)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::ELong _old_startTime = m_startTime;
#endif
    m_startTime = _startTime;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::IMS_Data::IMS_DataPackage::_instance()->getMetricHistOp__startTime(),
                _old_startTime,
                m_startTime
        );
        eNotify(&notification);
    }
#endif
}

::ecore::ELong MetricHistOp::getEndTime() const
{
    return m_endTime;
}

void MetricHistOp::setEndTime(::ecore::ELong _endTime)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::ELong _old_endTime = m_endTime;
#endif
    m_endTime = _endTime;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::IMS_Data::IMS_DataPackage::_instance()->getMetricHistOp__endTime(),
                _old_endTime,
                m_endTime
        );
        eNotify(&notification);
    }
#endif
}

::IMS_Data::MetricType MetricHistOp::getType() const
{
    return m_type;
}

void MetricHistOp::setType(::IMS_Data::MetricType _type)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::IMS_Data::MetricType _old_type = m_type;
#endif
    m_type = _type;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::IMS_Data::IMS_DataPackage::_instance()->getMetricHistOp__type(),
                _old_type,
                m_type
        );
        eNotify(&notification);
    }
#endif
}

// References

