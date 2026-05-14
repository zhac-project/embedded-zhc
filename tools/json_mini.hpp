// SPDX-FileCopyrightText: 2025-2026 Evgenij Cjura and project contributors
// SPDX-License-Identifier: Apache-2.0
#pragma once

// Minimal JSON reader for the fixture loader. Handles objects, arrays,
// strings, signed integers, booleans, and null — enough for the fixture
// shape. Not a general-purpose replacement for nlohmann/json.

#include <cctype>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace zhc::json {

enum class Kind { Null, Bool, Int, Float, String, Array, Object };

struct Value {
    Kind        kind{Kind::Null};
    bool        b{false};
    std::int64_t i{0};
    double      f{0.0};
    std::string s;
    std::vector<Value>            arr;
    std::map<std::string, Value>  obj;

    bool is_null()   const { return kind == Kind::Null; }
    bool is_bool()   const { return kind == Kind::Bool; }
    bool is_int()    const { return kind == Kind::Int; }
    bool is_float()  const { return kind == Kind::Float; }
    bool is_string() const { return kind == Kind::String; }
    bool is_array()  const { return kind == Kind::Array; }
    bool is_object() const { return kind == Kind::Object; }

    const Value* get(const char* key) const {
        if (kind != Kind::Object) return nullptr;
        auto it = obj.find(key);
        return it == obj.end() ? nullptr : &it->second;
    }

    const std::string& as_string() const { return s; }
    std::int64_t       as_int()    const { return i; }
    double             as_float()  const {
        return kind == Kind::Float ? f : static_cast<double>(i);
    }
    bool               as_bool()   const { return b; }
};

class Parser {
public:
    explicit Parser(std::string_view text) : t_(text) {}

    Value parse() {
        skip_ws();
        Value v = parse_value();
        skip_ws();
        if (pos_ != t_.size()) throw std::runtime_error("trailing garbage");
        return v;
    }

private:
    std::string_view t_;
    std::size_t pos_{0};

    void skip_ws() {
        while (pos_ < t_.size() &&
               (t_[pos_] == ' ' || t_[pos_] == '\t' ||
                t_[pos_] == '\n' || t_[pos_] == '\r')) {
            ++pos_;
        }
    }

    char peek() const { return pos_ < t_.size() ? t_[pos_] : '\0'; }
    char eat()        { return pos_ < t_.size() ? t_[pos_++] : '\0'; }

    void expect(char c) {
        if (eat() != c) throw std::runtime_error(
            std::string("expected '") + c + "'");
    }

    Value parse_value() {
        skip_ws();
        switch (peek()) {
            case '{': return parse_object();
            case '[': return parse_array();
            case '"': return parse_string();
            case 't': case 'f': return parse_bool();
            case 'n': return parse_null();
            default:  return parse_number();
        }
    }

    Value parse_object() {
        expect('{');
        Value v; v.kind = Kind::Object;
        skip_ws();
        if (peek() == '}') { eat(); return v; }
        for (;;) {
            skip_ws();
            if (peek() != '"') throw std::runtime_error("object key must be string");
            Value key = parse_string();
            skip_ws(); expect(':');
            v.obj.emplace(key.s, parse_value());
            skip_ws();
            if (peek() == ',') { eat(); continue; }
            if (peek() == '}') { eat(); break; }
            throw std::runtime_error("expected ',' or '}'");
        }
        return v;
    }

    Value parse_array() {
        expect('[');
        Value v; v.kind = Kind::Array;
        skip_ws();
        if (peek() == ']') { eat(); return v; }
        for (;;) {
            v.arr.push_back(parse_value());
            skip_ws();
            if (peek() == ',') { eat(); continue; }
            if (peek() == ']') { eat(); break; }
            throw std::runtime_error("expected ',' or ']'");
        }
        return v;
    }

    Value parse_string() {
        expect('"');
        Value v; v.kind = Kind::String;
        while (pos_ < t_.size() && t_[pos_] != '"') {
            char c = t_[pos_++];
            if (c == '\\' && pos_ < t_.size()) {
                const char esc = t_[pos_++];
                switch (esc) {
                    case '"':  v.s += '"'; break;
                    case '\\': v.s += '\\'; break;
                    case '/':  v.s += '/'; break;
                    case 'n':  v.s += '\n'; break;
                    case 't':  v.s += '\t'; break;
                    case 'r':  v.s += '\r'; break;
                    default:   v.s += esc; break;  // minimal
                }
            } else {
                v.s += c;
            }
        }
        expect('"');
        return v;
    }

    Value parse_bool() {
        Value v; v.kind = Kind::Bool;
        if (t_.compare(pos_, 4, "true") == 0)  { pos_ += 4; v.b = true;  return v; }
        if (t_.compare(pos_, 5, "false") == 0) { pos_ += 5; v.b = false; return v; }
        throw std::runtime_error("invalid boolean");
    }

    Value parse_null() {
        if (t_.compare(pos_, 4, "null") == 0) { pos_ += 4; return Value{}; }
        throw std::runtime_error("invalid null");
    }

    Value parse_number() {
        const std::size_t start = pos_;
        if (peek() == '-' || peek() == '+') ++pos_;
        bool is_float = false;
        while (pos_ < t_.size()) {
            char c = t_[pos_];
            if (std::isdigit(static_cast<unsigned char>(c))) { ++pos_; continue; }
            if (c == '.' || c == 'e' || c == 'E' || c == '+' || c == '-') {
                is_float = true; ++pos_; continue;
            }
            break;
        }
        if (pos_ == start) throw std::runtime_error("expected number");
        Value v;
        if (is_float) {
            v.kind = Kind::Float;
            v.f = std::strtod(t_.data() + start, nullptr);
        } else {
            v.kind = Kind::Int;
            v.i = std::strtoll(t_.data() + start, nullptr, 10);
        }
        return v;
    }
};

inline Value parse(std::string_view text) {
    return Parser{text}.parse();
}

}  // namespace zhc::json
