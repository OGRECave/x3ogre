/*
 * MemberAdapter.h
 *
 *  Created on: 19.02.2014
 *      Author: parojtbe
 */

#pragma once

#include <reflection/TypeInfo.h>

#include <stdexcept>
#include <type_traits>

#include <core/StringConverter.h>
#include <reflection/TypeName.h>

namespace reflection {

template <typename T, typename M, const M& (T::*_getter)() const>
struct GetterAdapter : public IGetterAdapter {
    const void* operator()(const void* obj) {
        const M& m = (static_cast<const T*>(obj)->*_getter)();
        return &m;
    }

    std::string stringValue(const void* obj) {
        const M& m = (static_cast<const T*>(obj)->*_getter)();
        return X3D::StringConverter::toString(m);
    }
};

template <typename T, typename M, const std::shared_ptr<M>& (T::*_getter)() const>
struct GetterAdapter<T, std::shared_ptr<M>, _getter> : public IGetterAdapter {
    const void* operator()(const void* obj) {
        const std::shared_ptr<M>& m = (static_cast<const T*>(obj)->*_getter)();
        return &m;
    }

    std::string stringValue(const void* obj) {
        const std::shared_ptr<M>& m = (static_cast<const T*>(obj)->*_getter)();
        return X3D::StringConverter::toString(std::static_pointer_cast<Object>(m));
    }
};

template <typename T, typename M, void (T::*_setter)(const M&)>
struct SetterAdapter : public ISetterAdapter {
    void operator()(void* obj, const std::string& str) {
        M val;
        X3D::StringConverter::parse(str, val);
        operator ()(obj, &val);
    };

    const char* argumentTypeName() const {
        return TypeName<M>::name();
    }

    bool supports(Object* arg) const {
        return false;
    }

    void operator()(void* obj, void* arg) {
        (static_cast<T*>(obj)->*_setter)(*static_cast<M*>(arg));
    }

    void operator()(void* obj, const std::shared_ptr<void>& arg) {
        throw std::runtime_error("do not use shared_ptr assignment on plain data types");
    }
};

template <typename T, typename M, void (T::*_setter)(const std::shared_ptr<M>&)>
struct SetterAdapter<T, std::shared_ptr<M>, _setter> : public ISetterAdapter {
    void operator()(void* obj, const std::string& arg) {
        throw std::runtime_error("setting class nodes from string not supported");
    };

    const char* argumentTypeName() const {
        return TypeName<M>::name();
    }

    bool supports(Object* arg) const {
        return dynamic_cast<M*>(arg);
    }

    void operator()(void* obj, void* arg) {
        throw std::runtime_error("do not use plain ptr assignment on object data types");
    };

    void operator()(void* obj, const std::shared_ptr<void>& arg) {
        (static_cast<T*>(obj)->*_setter)(std::static_pointer_cast<M>(arg));
    }
};

}
