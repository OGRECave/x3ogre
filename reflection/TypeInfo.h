/*
 * TypeInfo.h
 *
 *  Created on: 13.02.2014
 *      Author: parojtbe
 */

#pragma once

#include <string>
#include <map>
#include <memory>

#include "Object.h"

namespace reflection {

struct ISetterAdapter {
    virtual bool supports(Object* arg) const = 0;
    virtual const char* argumentTypeName() const = 0;
    virtual void operator()(void* obj, const std::string& val) = 0;
    virtual void operator()(void* obj, void* arg) = 0;
    virtual void operator()(void* obj, const std::shared_ptr<void>& arg) = 0;
};

struct IGetterAdapter {
    virtual const void* operator()(const void* obj) = 0;
    virtual std::string stringValue(const void* obj) = 0;
};

struct TypeInfoCommon {
    /**
     * for internal use only
     */
    void _resolveParentInfo();

    /**
     * call setter function by string name
     * @param obj pointer to instance
     * @param name name of the setter function
     * @param value pointer to value. must be castable to the underlying type of the function
     */
    void callMember(void* obj, const std::string& name, void* value);

    /**
     * call setter function by string name
     * @param obj pointer to instance
     * @param name name of the setter function
     * @param value pointer to value. must be castable to the underlying type of the function
     */
    void callMember(void* obj, const std::string& name, const std::shared_ptr<void>& value);

    /**
     * call setter function by string name and with string value
     * @param obj pointer to instance
     * @param name name of the setter function
     * @param value string representation of the value. uses StringConverter to convert to underlying type
     */
    void callMember(void* obj, const std::string& name, const std::string& value);

    /**
     * call getter function by string name
     * @param obj pointer to instance
     * @param name name of the getter function
     * @return pointer to the value
     */
    const void* callMember(const void* obj, const std::string& name);

    /**
     * call getter function by string name
     * @param obj pointer to instance
     * @param name name of the getter function
     * @return string representation of the value
     */
    std::string callMemberString(const void* obj, const std::string& name);

    /**
     * get member name by argument type
     * @param arg
     * @return
     */
    const std::string& memberByArgType(Object* arg);

    const std::map<const std::string, ISetterAdapter*>& setters() const {
        return _setters;
    }

    virtual Object* create() = 0;
protected:
    std::map<const std::string, ISetterAdapter*> _setters;
    std::map<const std::string, IGetterAdapter*> _getters;

    const std::string _parentType;
    TypeInfoCommon(const std::string& parentType);
};

}

