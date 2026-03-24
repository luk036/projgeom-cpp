#pragma once

#include <string>

namespace projgeom {

    /**
     * @brief Supported language codes for greetings.
     */
    enum class LanguageCode { EN, DE, ES, FR };

    /**
     * @brief A class that provides localized greeting messages.
     */
    class Greeter {
      public:
        /**
         * @brief Construct a new Greeter object.
         *
         * @param name The name to use in greetings
         */
        explicit Greeter(std::string name) : name_(std::move(name)) {}

        /**
         * @brief Generate a greeting message in the specified language.
         *
         * @param lang The language code for the greeting
         * @return std::string The greeting message
         */
        std::string greet(LanguageCode lang) const {
            switch (lang) {
                case LanguageCode::DE:
                    return "Hallo " + name_ + "!";
                case LanguageCode::ES:
                    return "¡Hola " + name_ + "!";
                case LanguageCode::FR:
                    return "Bonjour " + name_ + "!";
                default:
                    return "Hello " + name_ + "!";
            }
        }

      private:
        /** @brief The name to use in greetings */
        std::string name_;
    };

}  // namespace projgeom
