// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * TMS_Data/SubmitOptions.hpp
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

#ifndef TMS_DATA_SUBMITOPTIONS_HPP
#define TMS_DATA_SUBMITOPTIONS_HPP

#include <TMS_Data_forward.hpp>
#include <ecorecpp/mapping_forward.hpp>

#include <ecore_forward.hpp>
#include <ecore_forward.hpp>

#include <ecore/EObject.hpp>

/*PROTECTED REGION ID(SubmitOptions_pre) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

namespace TMS_Data
{

    class SubmitOptions: public virtual ::ecore::EObject

    {
    public:
        SubmitOptions();

        virtual ~SubmitOptions();

        virtual void _initialize();

        // Operations


        // Attributes
        ::ecore::EString const& getName() const;
        void setName(::ecore::EString const& _name);

        ::TMS_Data::JobPriority getPriority() const;
        void setPriority(::TMS_Data::JobPriority _priority);

        ::ecore::EString const& getQueue() const;
        void setQueue(::ecore::EString const& _queue);

        ::ecore::EInt getWallTime() const;
        void setWallTime(::ecore::EInt _wallTime);

        ::ecore::EInt getMemory() const;
        void setMemory(::ecore::EInt _memory);

        ::ecore::EInt getNbCpu() const;
        void setNbCpu(::ecore::EInt _nbCpu);

        ::ecore::EString const& getNbNodesAndCpuPerNode() const;
        void setNbNodesAndCpuPerNode(
                ::ecore::EString const& _nbNodesAndCpuPerNode);

        ::ecore::EString const& getOutPutPath() const;
        void setOutPutPath(::ecore::EString const& _outPutPath);

        ::ecore::EString const& getErrorPath() const;
        void setErrorPath(::ecore::EString const& _errorPath);

        // References


        /*PROTECTED REGION ID(SubmitOptions) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

        // EObjectImpl
        virtual ::ecore::EJavaObject eGet(::ecore::EInt _featureID,
                ::ecore::EBoolean _resolve);
        virtual void eSet(::ecore::EInt _featureID,
                ::ecore::EJavaObject const& _newValue);
        virtual ::ecore::EBoolean eIsSet(::ecore::EInt _featureID);
        virtual void eUnset(::ecore::EInt _featureID);
        virtual ::ecore::EClass_ptr _eClass();

        /*PROTECTED REGION ID(SubmitOptionsImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        // Attributes

        ::ecore::EString m_name;

        ::TMS_Data::JobPriority m_priority;

        ::ecore::EString m_queue;

        ::ecore::EInt m_wallTime;

        ::ecore::EInt m_memory;

        ::ecore::EInt m_nbCpu;

        ::ecore::EString m_nbNodesAndCpuPerNode;

        ::ecore::EString m_outPutPath;

        ::ecore::EString m_errorPath;

        // References

    };

} // TMS_Data

#endif // TMS_DATA_SUBMITOPTIONS_HPP
