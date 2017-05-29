/*
 * StringConverter.cpp
 *
 *  Created on: 06.02.2014
 *      Author: parojtbe
 */

#include "StringConverter.h"
#include <cstdlib>
#include <memory>

#include <core/Node.h>

#include <OgreStringConverter.h>

using namespace std;

namespace {
// templated declaration
template <typename T> struct components {
};

// components definition
template <> struct components<bool> {
    static const size_t num = 1;
};

template <> struct components<double> {
    static const size_t num = 1;
};

template <> struct components<float> {
    static const size_t num = 1;
};

template <> struct components<int32_t> {
    static const size_t num = 1;
};

template <> struct components<Ogre::Vector2> {
    static const size_t num = 2;
};

template <> struct components<Ogre::Vector3> {
    static const size_t num = 3;
};

template <> struct components<Ogre::ColourValue> {
    static const size_t num = 3;
};

template <> struct components<Ogre::Quaternion> {
    static const size_t num = 4;
};

template <> struct components<Ogre::Matrix4> {
    static const size_t num = 16;
};

}

namespace X3D {
namespace StringConverter {

template <typename T>
void parse(const std::string& str, T& v) {
    Ogre::StringConverter::parse(str, v);
}

// parse definitions for single values
template <> void parse(const string& str, string& v) {
    v = str;
}

template <> void parse(const string& str, double& v) {
    v = Ogre::StringConverter::parseReal(str);
};

template <> void parse(const string& str, float& v) {
    v = Ogre::StringConverter::parseReal(str);
};

template <> void parse(const string& str, Ogre::Quaternion& v) {
    Ogre::Vector4 tmp = Ogre::StringConverter::parseVector4(str);
    v = Ogre::Quaternion(Ogre::Radian(tmp[3]), Ogre::Vector3(tmp.ptr()));
    assert(abs(Ogre::Vector3(tmp.ptr()).length() - 1.0) < numeric_limits<float>::epsilon());
}

// parse definition for vectors values
template <typename T>
void parse(const std::string& str, std::vector<T>& vec) {
    vec.clear();

    std::string _str(str); // local copy

    T v;
    size_t i = 0;
    const char* token = NULL;

    bool skipws = false;

    for (size_t c = 0; c <= _str.size(); ++c) {
        switch (_str[c]) {
        case '\0':
        case ',':
        case ' ':
        case '\t':
        case '\r':
        case '\n':
            if (token) {
                if(not skipws) {
                    i++;
                    skipws = true;
                }

                if (i == components<T>::num) {
                    _str[c] = '\0';
                    parse(token, v);
                    vec.push_back(v);

                    token = NULL;
                    i = 0;
                }
            }

            break;
        default:
            if (token == NULL) {
                token = &_str[c];
            }

            skipws = false;
            break;
        }
    }
}

template <>
void parse(const std::string& str, std::vector<std::string>& vec) {
    vec.clear();
    string _str(str);

    char* start = NULL;

    for (size_t c = 0; c <= _str.size(); ++c) {
        switch (_str[c]) {
        case '\0':
        case ',':
        case '"':
        case '\n':
            if (start) {
                _str[c] = '\0';
                vec.push_back(start);
                start = NULL;
            }
            break;
        default:
            if (!start)
                start = &_str[c];
            break;
        }
    }
}

// toString definition
template<typename T> string toString(const T& v) {
    return Ogre::StringConverter::toString(v);
}

template<> string toString(const string& v) {
    return v;
}

template<> string toString(const bool& v) {
    return v ? "true" : "false";
}

template<> string toString(const std::shared_ptr<reflection::Object>& v) {
    return "reflection::Object";
}

template<typename T>
string toString(const vector<T>& vec) {
    std::stringstream ss;

    for (size_t i = 0; i < vec.size(); i++) {
        ss << toString(vec[i]);
        if (i != vec.size() - 1) ss << ",";
    }

    return ss.str();
}

// template instantiations
template void parse(const std::string&, bool&);
template void parse(const std::string&, int32_t&);
template void parse(const std::string&, Ogre::Vector2&);
template void parse(const std::string&, Ogre::Vector3&);
template void parse(const std::string&, Ogre::ColourValue&);
template void parse(const std::string&, Ogre::Matrix4&);

//template void parse(const std::string& str, std::vector<double>& vec);
template void parse(const std::string& str, std::vector<float>& vec);
template void parse(const std::string& str, std::vector<Ogre::Vector2>& vec);
template void parse(const std::string& str, std::vector<Ogre::Vector3>& vec);
template void parse(const std::string& str, std::vector<Ogre::Quaternion>& vec);
template void parse(const std::string& str, std::vector<Ogre::Matrix4>& vec);
template void parse(const std::string& str, std::vector<int>& vec);

template string toString(const int& v);
template string toString(const float& v);
template string toString(const double& v);
template string toString(const Ogre::ColourValue& v);
template string toString(const Ogre::Vector3& v);
template string toString(const Ogre::Matrix4& vec);
template string toString(const Ogre::Quaternion& quat);

template string toString(const std::vector<float>& vec);
template string toString(const std::vector<int>& vec);
template string toString(const std::vector<Ogre::Vector2>& vec);
template string toString(const std::vector<std::string>& vec);
}
}

