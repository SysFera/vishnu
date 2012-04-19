// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * TMS_Data/SubmitOptions.cpp
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

#include "SubmitOptions.hpp"
#include <TMS_Data/LoadCriterion.hpp>
#include <ecore/EObject.hpp>
#include <ecore/EClass.hpp>
#include "TMS_Data/TMS_DataPackage.hpp"
#include <ecorecpp/mapping.hpp>

#ifdef ECORECPP_NOTIFICATION_API
#include <ecorecpp/notify.hpp>
#endif

using namespace ::TMS_Data;

// Default constructor
SubmitOptions::SubmitOptions() :
    m_wallTime(-1), m_memory(-1), m_nbCpu(-1), m_selectQueueAutom(false),
            m_criterion(0)
{

    /*PROTECTED REGION ID(SubmitOptionsImpl__SubmitOptionsImpl) START*/
    // Please, enable the protected region if you add manually written code.
    // To do this, add the keyword ENABLED before START.
    /*PROTECTED REGION END*/
}

SubmitOptions::~SubmitOptions()
{
    if (m_criterion)
    {
        delete m_criterion;
    }
}

/*PROTECTED REGION ID(SubmitOptions.cpp) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

// Attributes

::ecore::EString const& SubmitOptions::getName() const
{
    return m_name;
}

void SubmitOptions::setName(::ecore::EString const& _name)
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
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getSubmitOptions__name(),
                _old_name,
                m_name
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& SubmitOptions::getQueue() const
{
    return m_queue;
}

void SubmitOptions::setQueue(::ecore::EString const& _queue)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_queue = m_queue;
#endif
    m_queue = _queue;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getSubmitOptions__queue(),
                _old_queue,
                m_queue
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EInt SubmitOptions::getWallTime() const
{
    return m_wallTime;
}

void SubmitOptions::setWallTime(::ecore::EInt _wallTime)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EInt _old_wallTime = m_wallTime;
#endif
    m_wallTime = _wallTime;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getSubmitOptions__wallTime(),
                _old_wallTime,
                m_wallTime
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EInt SubmitOptions::getMemory() const
{
    return m_memory;
}

void SubmitOptions::setMemory(::ecore::EInt _memory)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EInt _old_memory = m_memory;
#endif
    m_memory = _memory;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getSubmitOptions__memory(),
                _old_memory,
                m_memory
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EInt SubmitOptions::getNbCpu() const
{
    return m_nbCpu;
}

void SubmitOptions::setNbCpu(::ecore::EInt _nbCpu)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EInt _old_nbCpu = m_nbCpu;
#endif
    m_nbCpu = _nbCpu;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getSubmitOptions__nbCpu(),
                _old_nbCpu,
                m_nbCpu
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& SubmitOptions::getNbNodesAndCpuPerNode() const
{
    return m_nbNodesAndCpuPerNode;
}

void SubmitOptions::setNbNodesAndCpuPerNode(
        ::ecore::EString const& _nbNodesAndCpuPerNode)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_nbNodesAndCpuPerNode = m_nbNodesAndCpuPerNode;
#endif
    m_nbNodesAndCpuPerNode = _nbNodesAndCpuPerNode;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getSubmitOptions__nbNodesAndCpuPerNode(),
                _old_nbNodesAndCpuPerNode,
                m_nbNodesAndCpuPerNode
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& SubmitOptions::getOutputPath() const
{
    return m_outputPath;
}

void SubmitOptions::setOutputPath(::ecore::EString const& _outputPath)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_outputPath = m_outputPath;
#endif
    m_outputPath = _outputPath;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getSubmitOptions__outputPath(),
                _old_outputPath,
                m_outputPath
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& SubmitOptions::getErrorPath() const
{
    return m_errorPath;
}

void SubmitOptions::setErrorPath(::ecore::EString const& _errorPath)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_errorPath = m_errorPath;
#endif
    m_errorPath = _errorPath;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getSubmitOptions__errorPath(),
                _old_errorPath,
                m_errorPath
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& SubmitOptions::getMailNotification() const
{
    return m_mailNotification;
}

void SubmitOptions::setMailNotification(
        ::ecore::EString const& _mailNotification)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_mailNotification = m_mailNotification;
#endif
    m_mailNotification = _mailNotification;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getSubmitOptions__mailNotification(),
                _old_mailNotification,
                m_mailNotification
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& SubmitOptions::getMailNotifyUser() const
{
    return m_mailNotifyUser;
}

void SubmitOptions::setMailNotifyUser(::ecore::EString const& _mailNotifyUser)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_mailNotifyUser = m_mailNotifyUser;
#endif
    m_mailNotifyUser = _mailNotifyUser;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getSubmitOptions__mailNotifyUser(),
                _old_mailNotifyUser,
                m_mailNotifyUser
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& SubmitOptions::getGroup() const
{
    return m_group;
}

void SubmitOptions::setGroup(::ecore::EString const& _group)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_group = m_group;
#endif
    m_group = _group;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getSubmitOptions__group(),
                _old_group,
                m_group
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& SubmitOptions::getWorkingDir() const
{
    return m_workingDir;
}

void SubmitOptions::setWorkingDir(::ecore::EString const& _workingDir)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_workingDir = m_workingDir;
#endif
    m_workingDir = _workingDir;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getSubmitOptions__workingDir(),
                _old_workingDir,
                m_workingDir
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EString const& SubmitOptions::getCpuTime() const
{
    return m_cpuTime;
}

void SubmitOptions::setCpuTime(::ecore::EString const& _cpuTime)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_cpuTime = m_cpuTime;
#endif
    m_cpuTime = _cpuTime;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getSubmitOptions__cpuTime(),
                _old_cpuTime,
                m_cpuTime
        );
        eNotify(&notification);
    }
#endif
}

::ecore::EBoolean SubmitOptions::isSelectQueueAutom() const
{
    return m_selectQueueAutom;
}

void SubmitOptions::setSelectQueueAutom(::ecore::EBoolean _selectQueueAutom)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EBoolean _old_selectQueueAutom = m_selectQueueAutom;
#endif
    m_selectQueueAutom = _selectQueueAutom;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getSubmitOptions__selectQueueAutom(),
                _old_selectQueueAutom,
                m_selectQueueAutom
        );
        eNotify(&notification);
    }
#endif
}

std::vector< ::ecore::EString > const& SubmitOptions::getFileParams() const
{
    return m_fileParams;
}

void SubmitOptions::setFileParams(
        std::vector< ::ecore::EString > const& _fileParams)
{
#ifdef ECORECPP_NOTIFICATION_API
    std::vector< ::ecore::EString > _old_fileParams = m_fileParams;
#endif
    m_fileParams = _fileParams;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getSubmitOptions__fileParams(),
                _old_fileParams,
                m_fileParams
        );
        eNotify(&notification);
    }
#endif
}

void SubmitOptions::addFileParams(
        ::ecore::EString const& _new_element_in_fileParams)
{
    m_fileParams.push_back(_new_element_in_fileParams);
}

void SubmitOptions::setFileParamsAt(size_t _position,
        ::ecore::EString const& _new_element_in_fileParams)
{
    if (_position >= m_fileParams.size())
        m_fileParams.resize(_position + 1, 0);

    m_fileParams[_position] = _new_element_in_fileParams;
}

void SubmitOptions::deleteFileParamsAt(size_t _position)
{
    if (_position >= m_fileParams.size())
        throw "Attribute counter out of bounds!";

    // TODO: This is not actually quite true
    if (_position == m_fileParams.size() - 1)
        m_fileParams.resize(_position);
}

std::vector< ::ecore::EString > const& SubmitOptions::getFileContents() const
{
    return m_fileContents;
}

void SubmitOptions::setFileContents(
        std::vector< ::ecore::EString > const& _fileContents)
{
#ifdef ECORECPP_NOTIFICATION_API
    std::vector< ::ecore::EString > _old_fileContents = m_fileContents;
#endif
    m_fileContents = _fileContents;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getSubmitOptions__fileContents(),
                _old_fileContents,
                m_fileContents
        );
        eNotify(&notification);
    }
#endif
}

void SubmitOptions::addFileContents(
        ::ecore::EString const& _new_element_in_fileContents)
{
    m_fileContents.push_back(_new_element_in_fileContents);
}

void SubmitOptions::setFileContentsAt(size_t _position,
        ::ecore::EString const& _new_element_in_fileContents)
{
    if (_position >= m_fileContents.size())
        m_fileContents.resize(_position + 1, 0);

    m_fileContents[_position] = _new_element_in_fileContents;
}

void SubmitOptions::deleteFileContentsAt(size_t _position)
{
    if (_position >= m_fileContents.size())
        throw "Attribute counter out of bounds!";

    // TODO: This is not actually quite true
    if (_position == m_fileContents.size() - 1)
        m_fileContents.resize(_position);
}

std::vector< ::ecore::EString > const& SubmitOptions::getTextParams() const
{
    return m_textParams;
}

void SubmitOptions::setTextParams(
        std::vector< ::ecore::EString > const& _textParams)
{
#ifdef ECORECPP_NOTIFICATION_API
    std::vector< ::ecore::EString > _old_textParams = m_textParams;
#endif
    m_textParams = _textParams;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getSubmitOptions__textParams(),
                _old_textParams,
                m_textParams
        );
        eNotify(&notification);
    }
#endif
}

void SubmitOptions::addTextParams(
        ::ecore::EString const& _new_element_in_textParams)
{
    m_textParams.push_back(_new_element_in_textParams);
}

void SubmitOptions::setTextParamsAt(size_t _position,
        ::ecore::EString const& _new_element_in_textParams)
{
    if (_position >= m_textParams.size())
        m_textParams.resize(_position + 1, 0);

    m_textParams[_position] = _new_element_in_textParams;
}

void SubmitOptions::deleteTextParamsAt(size_t _position)
{
    if (_position >= m_textParams.size())
        throw "Attribute counter out of bounds!";

    // TODO: This is not actually quite true
    if (_position == m_textParams.size() - 1)
        m_textParams.resize(_position);
}

// References
::TMS_Data::LoadCriterion_ptr SubmitOptions::getCriterion()
{
    return m_criterion;
}

void SubmitOptions::setCriterion(::TMS_Data::LoadCriterion_ptr _criterion)
{
    ::TMS_Data::LoadCriterion_ptr _old_criterion = m_criterion;

    m_criterion = _criterion;

#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getSubmitOptions__criterion(),
                _old_criterion,
                m_criterion
        );
        eNotify(&notification);
    }
#endif

    delete _old_criterion;
}

