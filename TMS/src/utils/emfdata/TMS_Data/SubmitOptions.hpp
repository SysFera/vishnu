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

/**
 * \file SubmitOptions.hpp
 * \brief The SubmitOptions class
 * \author Generated file
 * \date 31/03/2011
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

    /**
     * \class SubmitOptions
     * \brief Implementation of the SubmitOptions class
     */
    class SubmitOptions: public virtual ::ecore::EObject

    {
    public:
        /**
         * \brief The default constructor for SubmitOptions
         */
        SubmitOptions();
        /**
         * \brief The destructor for SubmitOptions
         */
        virtual ~SubmitOptions();

        /**
         * \brief Internal method
         */
        virtual void _initialize();

        // Operations


        // Attributes
        /**
         * \brief To get the name
         * \return The name attribute value
         **/
        ::ecore::EString const& getName() const;
        /**
         * \brief To set the name
         * \param _name The name value
         **/
        void setName(::ecore::EString const& _name);

        /**
         * \brief To get the queue
         * \return The queue attribute value
         **/
        ::ecore::EString const& getQueue() const;
        /**
         * \brief To set the queue
         * \param _queue The queue value
         **/
        void setQueue(::ecore::EString const& _queue);

        /**
         * \brief To get the wallTime
         * \return The wallTime attribute value
         **/
        ::ecore::EInt getWallTime() const;
        /**
         * \brief To set the wallTime
         * \param _wallTime The wallTime value
         **/
        void setWallTime(::ecore::EInt _wallTime);

        /**
         * \brief To get the memory
         * \return The memory attribute value
         **/
        ::ecore::EInt getMemory() const;
        /**
         * \brief To set the memory
         * \param _memory The memory value
         **/
        void setMemory(::ecore::EInt _memory);

        /**
         * \brief To get the nbCpu
         * \return The nbCpu attribute value
         **/
        ::ecore::EInt getNbCpu() const;
        /**
         * \brief To set the nbCpu
         * \param _nbCpu The nbCpu value
         **/
        void setNbCpu(::ecore::EInt _nbCpu);

        /**
         * \brief To get the nbNodesAndCpuPerNode
         * \return The nbNodesAndCpuPerNode attribute value
         **/
        ::ecore::EString const& getNbNodesAndCpuPerNode() const;
        /**
         * \brief To set the nbNodesAndCpuPerNode
         * \param _nbNodesAndCpuPerNode The nbNodesAndCpuPerNode value
         **/
        void setNbNodesAndCpuPerNode(
                ::ecore::EString const& _nbNodesAndCpuPerNode);

        /**
         * \brief To get the outPutPath
         * \return The outPutPath attribute value
         **/
        ::ecore::EString const& getOutPutPath() const;
        /**
         * \brief To set the outPutPath
         * \param _outPutPath The outPutPath value
         **/
        void setOutPutPath(::ecore::EString const& _outPutPath);

        /**
         * \brief To get the errorPath
         * \return The errorPath attribute value
         **/
        ::ecore::EString const& getErrorPath() const;
        /**
         * \brief To set the errorPath
         * \param _errorPath The errorPath value
         **/
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
