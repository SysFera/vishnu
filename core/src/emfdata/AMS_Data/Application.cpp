// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * AMS_Data/Application.cpp
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

#include "Application.hpp"
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "AMS_Data/AMS_DataPackage.hpp"
#include <ecorecpp/mapping.hpp>

#ifdef ECORECPP_NOTIFICATION_API
#include <ecorecpp/notify.hpp>
#endif

using namespace ::AMS_Data;

// Default constructor
Application::Application() :
    m_name(""), m_description(""), m_input(""), m_output(""), m_script(""),
            m_applicationId("")
{

    /*PROTECTED REGION ID(ApplicationImpl__ApplicationImpl) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

Application::~Application()
{
}

/*PROTECTED REGION ID(Application.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

// Attributes

::ecore::EString const& Application::getName() const
{
    return m_name;
}

void Application::setName(::ecore::EString const& _name)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_name = m_name;
#endif
    m_name = _name;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::AMS_Data::AMS_DataPackage::_instance()->getApplication__name(),
                _old_name,
                m_name
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& Application::getDescription() const
{
    return m_description;
}

void Application::setDescription(::ecore::EString const& _description)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_description = m_description;
#endif
    m_description = _description;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::AMS_Data::AMS_DataPackage::_instance()->getApplication__description(),
                _old_description,
                m_description
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& Application::getInput() const
{
    return m_input;
}

void Application::setInput(::ecore::EString const& _input)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_input = m_input;
#endif
    m_input = _input;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::AMS_Data::AMS_DataPackage::_instance()->getApplication__input(),
                _old_input,
                m_input
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& Application::getOutput() const
{
    return m_output;
}

void Application::setOutput(::ecore::EString const& _output)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_output = m_output;
#endif
    m_output = _output;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::AMS_Data::AMS_DataPackage::_instance()->getApplication__output(),
                _old_output,
                m_output
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& Application::getScript() const
{
    return m_script;
}

void Application::setScript(::ecore::EString const& _script)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_script = m_script;
#endif
    m_script = _script;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::AMS_Data::AMS_DataPackage::_instance()->getApplication__script(),
                _old_script,
                m_script
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& Application::getApplicationId() const
{
    return m_applicationId;
}

void Application::setApplicationId(::ecore::EString const& _applicationId)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_applicationId = m_applicationId;
#endif
    m_applicationId = _applicationId;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::AMS_Data::AMS_DataPackage::_instance()->getApplication__applicationId(),
                _old_applicationId,
                m_applicationId
        );
        eNotify(&notification);
    }
#endif
}

// References

