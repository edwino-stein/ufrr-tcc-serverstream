#if !defined  _TYPES_HPP
    #define _TYPES_HPP
    
    #include <string>
    #include <vector>
    #include <functional>
    #include "ArduinoJson/ArduinoJson.h"

    using String = std::string;
    template <class T> using Vector = std::vector<T>;
    template <class T> using AFunction = std::function<T>;
    namespace json = ArduinoJson;

#endif