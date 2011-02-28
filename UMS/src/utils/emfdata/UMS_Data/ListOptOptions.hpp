// -*- mode: c++; c-basic-style: "bsd"; c-basic-offset: 4; -*-
/*
 * UMS_Data/ListOptOptions.hpp
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
 * \file ListOptOptions.hpp
 * \brief Represents the class to list options of options
 * \author Generated file
 * \date 31/01/2011
 */

#ifndef UMS_DATA_LISTOPTOPTIONS_HPP
#define UMS_DATA_LISTOPTOPTIONS_HPP

#include <UMS_Data_forward.hpp>
#include <ecorecpp/mapping_forward.hpp>

#include <ecore_forward.hpp>
#include <ecore_forward.hpp>

#include <ecore/EObject.hpp>

/*PROTECTED REGION ID(ListOptOptions_pre) START*/
// Please, enable the protected region if you add manually written code.
// To do this, add the keyword ENABLED before START.
/*PROTECTED REGION END*/

namespace UMS_Data
{

    /**
     * \class ListOptOptions
     * \brief A list of the options of the options defined by the user fir its account
     */
    class ListOptOptions: public virtual ::ecore::EObject

    {
    public:
	/**
	 * \brief Default constructor
	 * \fn ListOptOptions()
	 */
        ListOptOptions();

	/**
	 * \brief Destructor
	 * \fn ~ListOptOptions()
	 */
        virtual ~ListOptOptions();

	/**
	 * \brief To initialize the list
	 * \fn void _initialize()
	 */
        virtual void _initialize();

        // Operations


        // Attributes
	/**
	 * \brief If the 'all show' option is set
	 * \fn ::ecore::EBoolean isListAllDeftValue() const
	 * \return If set to yes to show all
	 */
        ::ecore::EBoolean isListAllDeftValue() const;
	/**
	 * \brief To set the 'all show' option 
	 * \fn void setListAllDeftValue(::ecore::EBoolean _listAllDeftValue)
	 * \param _listAllDeftValue Boolean to set to show all
	 */
        void setListAllDeftValue(::ecore::EBoolean _listAllDeftValue);

	/**
	 * \brief To get the user id param
	 * \fn ::ecore::EString const& getUserId() const
	 * \return The user id
	 */
        ::ecore::EString const& getUserId() const;
	/**
	 * \brief To set the user id
	 * \fn void setUserId(::ecore::EString const& _userId)
	 * \param _userId The id of the user
	 */
        void setUserId(::ecore::EString const& _userId);

	/**
	 * \brief To get the name of the option
	 * \fn ::ecore::EString const& getOptionName() const
	 * \return The name of the option
	 */
        ::ecore::EString const& getOptionName() const;
	/**
	 * \brief To set the name of the option
	 * \fn void setOptionName(::ecore::EString const& _optionName)
	 * \param _userId The name of the option
	 */
        void setOptionName(::ecore::EString const& _optionName);

        // References


        /*PROTECTED REGION ID(ListOptOptions) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

        // EObjectImpl
	/**
	 * \brief Getter to get a feature of the class
	 * \fn virtual ::ecore::EJavaObject eGet(::ecore::EInt _featureID,
	 *                                       ::ecore::EBoolean _resolve)
	 * \param _featureID The id of the attribute
	 * \param _resolve 
	 * \return The attribute corresponding to the id
	 */
        virtual ::ecore::EJavaObject eGet(::ecore::EInt _featureID,
                ::ecore::EBoolean _resolve);
	/**
	 * \brief To set a feature value
	 * \fn virtual void eSet(::ecore::EInt _featureID,
	 *                       ::ecore::EJavaObject const& _newValue)
	 * \param _featureID The id of the feature
	 * \param _newValue The new value of the feature
	 */
        virtual void eSet(::ecore::EInt _featureID,
                ::ecore::EJavaObject const& _newValue);
	/**
	 * \brief To know if a feature is set
	 * \fn virtual ::ecore::EBoolean eIsSet(::ecore::EInt _featureID)
	 * \param _featureID The id of the feature
	 * \return If the feature is set
	 */
        virtual ::ecore::EBoolean eIsSet(::ecore::EInt _featureID);
	/**
	 * \brief To unset a feature
	 * \fn virtual void eUnset(::ecore::EInt _featureID)
	 * \param _featureID The id of the feature
	 */
        virtual void eUnset(::ecore::EInt _featureID);
	/**
	 * \brief To get tje class
	 * \fn virtual ::ecore::EClass_ptr _eClass()
	 * \return A point to the class
	 */
        virtual ::ecore::EClass_ptr _eClass();

        /*PROTECTED REGION ID(ListOptOptionsImpl) START*/
        // Please, enable the protected region if you add manually written code.
        // To do this, add the keyword ENABLED before START.
        /*PROTECTED REGION END*/

    protected:
        // Attributes

	/**
	 * \brief If to show all value
	 */
        ::ecore::EBoolean m_listAllDeftValue;

	/**
	 * \brief User id
	 */
        ::ecore::EString m_userId;

	/**
	 * \brief Option name
	 */
        ::ecore::EString m_optionName;

        // References

    };

} // UMS_Data

#endif // UMS_DATA_LISTOPTOPTIONS_HPP
