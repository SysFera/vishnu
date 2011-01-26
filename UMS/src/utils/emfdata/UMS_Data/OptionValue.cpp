// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * UMS_Data/OptionValue.cpp
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

#include "OptionValue.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "UMS_Data/UMS_DataPackage.hpp"
#include <ecorecpp/mapping.hpp>

#ifdef ECORECPP_NOTIFICATION_API
#include <ecorecpp/notify.hpp>
#endif

using namespace ::UMS_Data;

// Default constructor
OptionValue::OptionValue() :
    m_optionName("")
{

    /*PROTECTED REGION ID(OptionValueImpl__OptionValueImpl) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

OptionValue::~OptionValue()
{
}

/*PROTECTED REGION ID(OptionValue.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

// Attributes

::ecore::EString const& OptionValue::getOptionName() const
{
    return m_optionName;
}

void OptionValue::setOptionName(::ecore::EString const& _optionName)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_optionName = m_optionName;
#endif
    m_optionName = _optionName;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getOptionValue__optionName(),
                _old_optionName,
                m_optionName
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& OptionValue::getValue() const
{
    return m_value;
}

void OptionValue::setValue(::ecore::EString const& _value)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_value = m_value;
#endif
    m_value = _value;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::UMS_Data::UMS_DataPackage::_instance()->getOptionValue__value(),
                _old_value,
                m_value
        );
        eNotify(&notification);
    }
#endif
}

// References

