/*
 * TypeInfo.cpp
 *
 *  Created on: 13.02.2014
 *      Author: parojtbe
 */

#include <reflection/TypeInfo.h>
#include <reflection/db.h>

#include <stdexcept>

using namespace std;

namespace reflection {
TypeInfoCommon::TypeInfoCommon(const string& parentType) : _parentType(parentType) {
}

void TypeInfoCommon::_resolveParentInfo() {
    string parentType = _parentType;
    TypeInfoCommon* parentInfo = nullptr;

    while((parentInfo = getTypeInfo(parentType))) {
        _setters.insert(parentInfo->_setters.begin(), parentInfo->_setters.end());
        parentType = parentInfo->_parentType;
    }

#if 0
    map<string, ISetterAdapter*>::iterator i = _setters.begin();

    for(; i != _setters.end(); ++i) {
        cout << "\t" << i->first << endl;
    }
#endif
}

const std::string& TypeInfoCommon::memberByArgType(Object* arg) {
    for(const auto& e : _setters) {
        if(e.second->supports(arg)) {
            return e.first;
        }
    }

    throw runtime_error("could not find member by type");
}

void TypeInfoCommon::callMember(void* obj, const string& member, void* value) {
    auto i = _setters.find(member);

    if(i == _setters.end()) {
        throw runtime_error("no such member: '"+member+"'");
    }

    (*i->second)(obj, value);
}

void TypeInfoCommon::callMember(void* obj, const string& member, const string& value) {
    auto i = _setters.find(member);

    if(i == _setters.end()) {
        throw runtime_error("no such member: '"+member+"'");
    }

    (*i->second)(obj, value);
}

const void* TypeInfoCommon::callMember(const void* obj, const std::string& name) {
    auto i = _getters.find(name);

    if(i == _getters.end()) {
        throw runtime_error("no such member: '"+name+"'");
    }

    return (*i->second)(obj);
}

std::string TypeInfoCommon::callMemberString(const void* obj, const std::string& name) {
    auto i = _getters.find(name);

    if(i == _getters.end()) {
        throw runtime_error("no such member: '"+name+"'");
    }

    return i->second->stringValue(obj);
}

void TypeInfoCommon::callMember(void* obj, const std::string& member, const std::shared_ptr<void>& value) {
    auto i = _setters.find(member);

    if(i == _setters.end()) {
        throw runtime_error("no such member: '"+member+"'");
    }

    (*i->second)(obj, value);
}


}
