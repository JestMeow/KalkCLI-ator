#pragma once

#include <string>
#include <vector>
#include <functional>
#include <unordered_map>
#include <cmath>
#include <stdexcept>

// Helper for trig functions outputting insanely small values...
static double snap_to_zero(double val, double threshold = 1e-15) {
    return (std::abs(val) < threshold) ? 0.0 : val;
}

struct FunctionInfo {
    size_t arity;
    std::function<double(const std::vector<double>&)> fn;
};

static const std::unordered_map<std::string, FunctionInfo> FUNCTION_REGISTRY = {
    // Single argument functions
    {"sin",     {1, [](const auto& a) { return snap_to_zero(std::sin(a[0])); }}},
    {"cos",     {1, [](const auto& a) { return snap_to_zero(std::cos(a[0])); }}},
    {"tan",     {1, [](const auto& a) { return snap_to_zero(std::tan(a[0])); }}},
    {"asin",    {1, [](const auto& a) { return std::asin(a[0]); }}},
    {"acos",    {1, [](const auto& a) { return std::acos(a[0]); }}},
    {"atan",    {1, [](const auto& a) { return std::atan(a[0]); }}},
    {"arcsin",  {1, [](const auto& a) { return std::asin(a[0]); }}},
    {"arccos",  {1, [](const auto& a) { return std::acos(a[0]); }}},
    {"arctan",  {1, [](const auto& a) { return std::atan(a[0]); }}},
    {"sinh",    {1, [](const auto& a) { return std::sinh(a[0]); }}},
    {"cosh",    {1, [](const auto& a) { return std::cosh(a[0]); }}},
    {"tanh",    {1, [](const auto& a) { return std::tanh(a[0]); }}},
    {"asinh",   {1, [](const auto& a) { return std::asinh(a[0]); }}},
    {"acosh",   {1, [](const auto& a) { return std::acosh(a[0]); }}},
    {"atanh",   {1, [](const auto& a) { return std::atanh(a[0]); }}},
    {"exp",     {1, [](const auto& a) { return std::exp(a[0]); }}},
    {"ln",      {1, [](const auto& a) { return std::log(a[0]); }}},
    {"sqrt",    {1, [](const auto& a) { return std::sqrt(a[0]); }}},
    {"cbrt",    {1, [](const auto& a) { return std::cbrt(a[0]); }}},
    {"abs",     {1, [](const auto& a) { return std::abs(a[0]); }}},
    {"fabs",    {1, [](const auto& a) { return std::abs(a[0]); }}},
    {"ceil",    {1, [](const auto& a) { return std::ceil(a[0]); }}},
    {"floor",   {1, [](const auto& a) { return std::floor(a[0]); }}},
    {"round",   {1, [](const auto& a) { return std::round(a[0]); }}},
    {"trunc",   {1, [](const auto& a) { return std::trunc(a[0]); }}},

    // Two argument functions
    {"pow",       {2, [](const auto& a) { return std::pow(a[0], a[1]); }}},
    {"hypot",     {2, [](const auto& a) { return std::hypot(a[0], a[1]); }}},
    {"fmod",      {2, [](const auto& a) { return std::fmod(a[0], a[1]); }}},
    {"remainder", {2, [](const auto& a) { return std::remainder(a[0], a[1]); }}},
    {"min",       {2, [](const auto& a) { return std::fmin(a[0], a[1]); }}},
    {"max",       {2, [](const auto& a) { return std::fmax(a[0], a[1]); }}},
    {"log", {2, [](const auto& a) {
                                      if (a[0] <= 0 || a[0] == 1.0) throw std::runtime_error("Math Error: Invalid log base");
                                      return std::log(a[1]) / std::log(a[0]);
                                  }}}
};
