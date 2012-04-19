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

std::vector< ::ecore::EString > const& SubmitOptions::getFile() const
{
    return m_file;
}

void SubmitOptions::setFile(std::vector< ::ecore::EString > const& _file)
{
#ifdef ECORECPP_NOTIFICATION_API
    std::vector< ::ecore::EString > _old_file = m_file;
#endif
    m_file = _file;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getSubmitOptions__file(),
                _old_file,
                m_file
        );
        eNotify(&notification);
    }
#endif
}

void SubmitOptions::addFile(::ecore::EString const& _new_element_in_file)
{
    m_file.push_back(_new_element_in_file);
}

void SubmitOptions::setFileAt(size_t _position,
        ::ecore::EString const& _new_element_in_file)
{
    if (_position >= m_file.size())
        m_file.resize(_position + 1, 0);

    m_file[_position] = _new_element_in_file;
}

void SubmitOptions::deleteFileAt(size_t _position)
{
    if (_position >= m_file.size())
        throw "Attribute counter out of bounds!";

    // TODO: This is not actually quite true
    if (_position == m_file.size() - 1)
        m_file.resize(_position);
}

::ecore::EString const& SubmitOptions::getListFiles() const
{
    return m_listFiles;
}

void SubmitOptions::setListFiles(::ecore::EString const& _listFiles)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_listFiles = m_listFiles;
#endif
    m_listFiles = _listFiles;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getSubmitOptions__listFiles(),
                _old_listFiles,
                m_listFiles
        );
        eNotify(&notification);
    }
#endif
}

std::vector< ::ecore::EString > const& SubmitOptions::getParam() const
{
    return m_param;
}

void SubmitOptions::setParam(std::vector< ::ecore::EString > const& _param)
{
#ifdef ECORECPP_NOTIFICATION_API
    std::vector< ::ecore::EString > _old_param = m_param;
#endif
    m_param = _param;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getSubmitOptions__param(),
                _old_param,
                m_param
        );
        eNotify(&notification);
    }
#endif
}

void SubmitOptions::addParam(::ecore::EString const& _new_element_in_param)
{
    m_param.push_back(_new_element_in_param);
}

void SubmitOptions::setParamAt(size_t _position,
        ::ecore::EString const& _new_element_in_param)
{
    if (_position >= m_param.size())
        m_param.resize(_position + 1, 0);

    m_param[_position] = _new_element_in_param;
}

void SubmitOptions::deleteParamAt(size_t _position)
{
    if (_position >= m_param.size())
        throw "Attribute counter out of bounds!";

    // TODO: This is not actually quite true
    if (_position == m_param.size() - 1)
        m_param.resize(_position);
}

::ecore::EString const& SubmitOptions::getListParams() const
{
    return m_listParams;
}

void SubmitOptions::setListParams(::ecore::EString const& _listParams)
{
#ifdef ECORECPP_NOTIFICATION_API
    ::ecore::EString _old_listParams = m_listParams;
#endif
    m_listParams = _listParams;
#ifdef ECORECPP_NOTIFICATION_API
    if (eNotificationRequired())
    {
        ::ecorecpp::notify::Notification notification(
                ::ecorecpp::notify::Notification::SET,
                (::ecore::EObject_ptr) this,
                (::ecore::EStructuralFeature_ptr) ::TMS_Data::TMS_DataPackage::_instance()->getSubmitOptions__listParams(),
                _old_listParams,
                m_listParams
        );
        eNotify(&notification);
    }
#endif
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

