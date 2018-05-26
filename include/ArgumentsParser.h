#ifndef ARGUMENTS_PARSER_H
#define ARGUMENTS_PARSER_H

#include <iostream>
#include <map>
#include <string>
#include <sstream>

/**
 * This file define class for works with arguments of command line
 * The usage:
 *     ArgumentParser parser;
 *     parser.configure<type>(string, [ default_value ]);
 *
 *     @type can be int, std::string, bool. In case of bool parser just check to existence of option
 *     @string - the key of some option, for example "--key"
 *     @default_value - the default value of option, if it is not set, the option is required
 *
 *     parser.Parse(argv, argc);
 *
 *     argumentsParser.CheckArguments();
 *
 *     type var = argumentsParser.Get<type>(string);
 *
 *     @type - the type of var, that we want to receive
 *     @string - the name of option, that we want to receive
 */

class ValueInterface {
public:
    ValueInterface() {}
    virtual ~ValueInterface() {}

    virtual int SetData(char **args_list, int current_index) = 0;

    template <typename T>
    T Get() {}

    template <typename T>
    void SetDefault(T default_value) {}

    virtual bool Stored() = 0;
};

class BaseImplValue : public ValueInterface {
public:

    BaseImplValue(): _data(nullptr), _stored(false) {}

    template <typename T>
    T Get() {
        return *(T*)GetData();
    }

    void *GetData();

    bool Stored() override;

    void *_data;
    bool _stored;
};

class IntValue : public BaseImplValue {
public:
    IntValue();

    ~IntValue() override;

    int SetData(char **args_list, int current_index) override;

    void SetDefault(int default_value, bool has_default_arg);
};

class StringValue : public BaseImplValue {
public:
    StringValue() = default;
    ~StringValue() override;

    int SetData(char **args_list, int current_index) override;

    void SetDefault(const std::string &default_value, bool has_default_arg);
};

class BoolValue : public BaseImplValue {
public:
    BoolValue();
    ~BoolValue() override;

    int SetData(char **args_list, int current_index) override;

    void SetDefault(bool default_value, bool has_default_arg);
};

namespace Configure {
template <typename T>
inline void make(
    std::map<std::string, BaseImplValue*> &args,
    const std::string &arg,
    const T &default_value,
    bool has_default_arg) {

    std::stringstream ss;
    ss << "Unknown type: " << typeid(T).name();
    throw std::runtime_error(ss.str());
}

template <>
inline void make<int>(
    std::map<std::string, BaseImplValue*> &args,
    const std::string &arg,
    const int &default_value,
    bool has_default_arg) {

    auto new_value = new IntValue();
    new_value->SetDefault(default_value, has_default_arg);
    args[arg] = new_value;
}

template<>
inline void make<std::string>(
    std::map<std::string, BaseImplValue*> &args,
    const std::string &arg,
    const std::string &default_value,
    bool has_default_arg) {

    auto new_value = new StringValue();
    new_value->SetDefault(default_value, has_default_arg);
    args[arg] = new_value;
}

template<>
inline void make<bool>(
    std::map<std::string, BaseImplValue*> &args,
    const std::string &arg,
    const bool &default_value,
    bool has_default_arg) {

    auto new_value = new BoolValue();
    new_value->SetDefault(default_value, has_default_arg);
    args[arg] = new_value;
}

} // namespace Configure

class ArgumentsParser {
public:
    ArgumentsParser() = default;

    template<typename T>
    void configure(const std::string &arg, const T &default_value, bool has_default_arg) {
        Configure::make<T>(_args, arg, default_value, has_default_arg);
    }

    template<typename T>
    void configure(const char *arg, const T &default_value) {
        configure<T>(std::string(arg), default_value, true);
    }

    template<typename T>
    void configure(const char *arg) {
        configure<T>(std::string(arg), T(), false);
    }

    void Parse(char **argv, int argc);

    template<typename T>
    T Get(const std::string &arg) {
        auto it = _args.find(arg);
        if (it == _args.end()) {
            std::stringstream ss;
            ss << "Unknown argument: [" << arg << "], type --help for see options";
            throw std::runtime_error(ss.str());
        }

        if (not it->second->Stored()) {
            std::stringstream ss;
            ss << "Argument " << arg << " was not initialized, type --help for see options";
            throw std::runtime_error(ss.str());
        }

        return it->second->Get<T>();
    }

    template<typename T>
    T Get(const char *arg) {
        return Get<T>(std::string(arg));
    }

    void CheckArguments();
private:
    std::map<std::string, BaseImplValue*> _args;
};

#endif // ARGUMENTS_PARSER_H