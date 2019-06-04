#if !defined  _TYPES_HPP
    #define _TYPES_HPP

    #include <string>
    #include <vector>
    #include <functional>
    #include <mutex>
    #include <queue>

    //Submodulos
    #include "ArduinoJson.h"

    using String = std::string;
    template <class T> using Vector = std::vector<T>;
    template <class T> using AFunction = std::function<T>;
    using Mutex = std::mutex;
    template <class T> using Queue = std::queue<T>;
    namespace json = ArduinoJson;

#endif
