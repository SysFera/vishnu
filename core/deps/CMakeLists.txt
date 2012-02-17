set(CMAKE_CXX_FLAGS "")
set(CMAKE_CXX_FLAGS_DEBUG "-Wall -g -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-Wall -O3 -funroll-loops")

set(ecore_SRCS
  emf4cpp/ecore.cpp
  emf4cpp/ecore/EcorePackage.cpp
  emf4cpp/ecore/EcoreFactory.cpp
  emf4cpp/ecore/EcorePackageImpl.cpp
  emf4cpp/ecore/EcoreFactoryImpl.cpp
  emf4cpp/ecore/EAttribute.cpp
  emf4cpp/ecore/EAttributeImpl.cpp
  emf4cpp/ecore/EAnnotation.cpp
  emf4cpp/ecore/EAnnotationImpl.cpp
  emf4cpp/ecore/EClass.cpp
  emf4cpp/ecore/EClassImpl.cpp
  emf4cpp/ecore/EClassifier.cpp
  emf4cpp/ecore/EClassifierImpl.cpp
  emf4cpp/ecore/EDataType.cpp
  emf4cpp/ecore/EDataTypeImpl.cpp
  emf4cpp/ecore/EEnum.cpp
  emf4cpp/ecore/EEnumImpl.cpp
  emf4cpp/ecore/EEnumLiteral.cpp
  emf4cpp/ecore/EEnumLiteralImpl.cpp
  emf4cpp/ecore/EFactory.cpp
  emf4cpp/ecore/EFactoryImpl.cpp
  emf4cpp/ecore/EModelElement.cpp
  emf4cpp/ecore/EModelElementImpl.cpp
  emf4cpp/ecore/ENamedElement.cpp
  emf4cpp/ecore/ENamedElementImpl.cpp
  emf4cpp/ecore/EObject.cpp
  emf4cpp/ecore/EObjectImpl.cpp
  emf4cpp/ecore/EOperation.cpp
  emf4cpp/ecore/EOperationImpl.cpp
  emf4cpp/ecore/EPackage.cpp
  emf4cpp/ecore/EPackageImpl.cpp
  emf4cpp/ecore/EParameter.cpp
  emf4cpp/ecore/EParameterImpl.cpp
  emf4cpp/ecore/EReference.cpp
  emf4cpp/ecore/EReferenceImpl.cpp
  emf4cpp/ecore/EStructuralFeature.cpp
  emf4cpp/ecore/EStructuralFeatureImpl.cpp
  emf4cpp/ecore/ETypedElement.cpp
  emf4cpp/ecore/ETypedElementImpl.cpp
  emf4cpp/ecore/EStringToStringMapEntry.cpp
  emf4cpp/ecore/EStringToStringMapEntryImpl.cpp
  emf4cpp/ecore/EGenericType.cpp
  emf4cpp/ecore/EGenericTypeImpl.cpp
  emf4cpp/ecore/ETypeParameter.cpp
  emf4cpp/ecore/ETypeParameterImpl.cpp)

set(ecore_HEADERS
  emf4cpp/ecore.hpp
  emf4cpp/ecore_forward.hpp
  emf4cpp/ecore/EcorePackage.hpp
  emf4cpp/ecore/EcoreFactory.hpp
  emf4cpp/ecore/EAttribute.hpp
  emf4cpp/ecore/EAnnotation.hpp
  emf4cpp/ecore/EClass.hpp
  emf4cpp/ecore/EClassifier.hpp
  emf4cpp/ecore/EDataType.hpp
  emf4cpp/ecore/EEnum.hpp
  emf4cpp/ecore/EEnumLiteral.hpp
  emf4cpp/ecore/EFactory.hpp
  emf4cpp/ecore/EModelElement.hpp
  emf4cpp/ecore/ENamedElement.hpp
  emf4cpp/ecore/EObject.hpp
  emf4cpp/ecore/EOperation.hpp
  emf4cpp/ecore/EPackage.hpp
  emf4cpp/ecore/EParameter.hpp
  emf4cpp/ecore/EReference.hpp
  emf4cpp/ecore/EStructuralFeature.hpp
  emf4cpp/ecore/ETypedElement.hpp
  emf4cpp/ecore/EStringToStringMapEntry.hpp
  emf4cpp/ecore/EGenericType.hpp
  emf4cpp/ecore/ETypeParameter.hpp)

set(ecorecpp_SRCS
  emf4cpp/ecorecpp/MetaModelRepository.cpp
  emf4cpp/ecorecpp/util/print.cpp
  emf4cpp/ecorecpp/parser/parser.cpp
  emf4cpp/ecorecpp/parser/handler.cpp
  emf4cpp/ecorecpp/serializer/serializer.cpp
  emf4cpp/ecorecpp/json/serializer.cpp
  # notify
  emf4cpp/ecorecpp/notify/Adapter.cpp
  emf4cpp/ecorecpp/notify/Notification.cpp
  emf4cpp/ecorecpp/notify/Notifier.cpp)

set(ecorecpp_HEADERS
  emf4cpp/ecorecpp.hpp
  emf4cpp/ecorecpp/json/json_serializer.hpp
  emf4cpp/ecorecpp/json/serializer.hpp
  emf4cpp/ecorecpp/mapping/any.hpp
  emf4cpp/ecorecpp/mapping/any_traits.hpp
  emf4cpp/ecorecpp/mapping/EList.hpp
  emf4cpp/ecorecpp/mapping/EListImpl.hpp
  emf4cpp/ecorecpp/mapping_forward.hpp
  emf4cpp/ecorecpp/mapping.hpp
  emf4cpp/ecorecpp/mapping/list.hpp
  emf4cpp/ecorecpp/mapping/out_ptr.hpp
  emf4cpp/ecorecpp/mapping/set_traits.hpp
  emf4cpp/ecorecpp/mapping/string_traits.hpp
  emf4cpp/ecorecpp/mapping/type_traits.hpp
  emf4cpp/ecorecpp/MetaModelRepository.hpp
  emf4cpp/ecorecpp/notify/Adapter.hpp
  emf4cpp/ecorecpp/notify_forward.hpp
  emf4cpp/ecorecpp/notify.hpp
  emf4cpp/ecorecpp/notify/Notification.hpp
  emf4cpp/ecorecpp/notify/Notifier.hpp
  emf4cpp/ecorecpp/parser/handler.hpp
  emf4cpp/ecorecpp/parser/handler-xerces.hpp
  emf4cpp/ecorecpp/parser/localstr.hpp
  emf4cpp/ecorecpp/parser/parser_common.hpp
  emf4cpp/ecorecpp/parser/parser.hpp
  emf4cpp/ecorecpp/parser/parser-xerces.hpp
  emf4cpp/ecorecpp/parser/reference_parser.hpp
  emf4cpp/ecorecpp/parser/simple_xml_parser.hpp
  emf4cpp/ecorecpp/parser/unresolved_reference.hpp
  emf4cpp/ecorecpp/serializer/externalstr.hpp
  emf4cpp/ecorecpp/serializer/greedy_serializer.hpp
  emf4cpp/ecorecpp/serializer/serializer.hpp
  emf4cpp/ecorecpp/serializer/serializer-xerces.hpp
  emf4cpp/ecorecpp/util/debug.hpp
  emf4cpp/ecorecpp/util/print.hpp)

include_directories(
  ${EMF4CPP_INCLUDE_DIR}
  ${DEPS_INCLUDE_DIR}
)

IF (WIN32)
  add_library(emf4cpp-vishnu ${ecore_SRCS} ${ecorecpp_SRCS} OSIndependance.cpp)
ELSE (WIN32)
  add_library(emf4cpp-vishnu ${ecore_SRCS} ${ecorecpp_SRCS})
ENDIF (WIN32)
IF (WIN32)
target_link_libraries(emf4cpp-vishnu ws2_32.lib)
ENDIF (WIN32)
set_target_properties(emf4cpp-vishnu PROPERTIES VERSION ${VISHNU_VERSION})
install(TARGETS emf4cpp-vishnu DESTINATION ${LIB_INSTALL_DIR})