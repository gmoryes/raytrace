#include "ArgumentsParser.h"

/* IntValue implementation */

IntValue::IntValue() : BaseImplValue() {}

IntValue::~IntValue() {
    delete (int*)_data;
}

int IntValue::SetData(char **args_list, int current_index) {
    _data = new int;
    sscanf(args_list[current_index + 1], "%d", (int*)_data);
    _stored = true;
    return current_index + 2;
}

void IntValue::SetDefault(int default_value, bool has_default_arg) {
    _data = new int(default_value);
    if (has_default_arg)
        _stored = true;
}

/* StringValue implementation */

StringValue::~StringValue() {
    delete (std::string*)_data;
}

int StringValue::SetData(char **args_list, int current_index) {
    _data = new std::string(args_list[current_index + 1]);
    _stored = true;
    return current_index + 2;
}

void StringValue::SetDefault(const std::string &default_value, bool has_default_arg) {
    _data = new std::string(default_value);
    if (has_default_arg)
        _stored = true;
}

/* BoolValue implementation */

BoolValue::BoolValue() : BaseImplValue() {
    _stored = true;
}

BoolValue::~BoolValue() {
    delete (bool*)_data;
}

int BoolValue::SetData(char **args_list, int current_index) {
    *(bool*)_data = true;
    return current_index + 1;
}

void BoolValue::SetDefault(bool default_value, bool has_default_arg) {
    _data = new bool(default_value);
    if (has_default_arg)
        _stored = true;
}

void ArgumentsParser::Parse(char **argv, int argc) {
    for (int i = 1; i < argc;) {
        std::string arg = std::string(argv[i]);

        auto it = _args.find(arg);
        if (it == _args.end()) {
            std::stringstream ss;
            ss << "Unkown argument: " << argv[i] << ", type --help for see options";
            throw std::runtime_error(ss.str());
        }

        i = it->second->SetData(argv, i);
    }
}

void ArgumentsParser::CheckArguments() {
    for (auto &it : _args) {
        if (not it.second->Stored()) {
            std::stringstream ss;
            ss << "Can not find argument: " << it.first << ", type --help for see options";
            throw std::runtime_error(ss.str());
        }
    }
}

void *BaseImplValue::GetData() {
    return _data;
}

bool BaseImplValue::Stored() {
    return _stored;
}
