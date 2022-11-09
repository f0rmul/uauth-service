#pragma once

#include <userver/formats/json/value.hpp>
#include <string_view>



userver::formats::json::Value MakeError(std::string_view code,std::string_view message)
{
     userver::formats::json::ValueBuilder builder;
     builder["code"] = code;
     builder["message"] = message;
     return builder.ExtractValue();
}

